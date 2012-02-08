#include <stdio.h>
#include <stdlib.h>
#include <sys/spu_initialize.h>
#include <sys/raw_spu.h>
#include <sys/spu_utility.h>
#include <sys/ppu_thread.h>
#include <sys/interrupt.h>
#include <sys/paths.h>
#include <sys/process.h>
#include <spu_printf.h>
#include <sysutil/sysutil_sysparam.h>  
#include <sys/timer.h>
#include <sysutil/sysutil_common.h>

#include "xunittest/ppu_exception_handler.h"

SYS_PROCESS_PARAM(1001, 0x10000)

#define EIEIO                 __asm__ volatile ("eieio");
#define INT_STAT_MAILBOX      0x01UL
#define SPU_PROG  (SYS_APP_HOME "/hello.spu.self")
#define PPU_STACK_SIZE 4096

#define MAX_PHYSICAL_SPU       6 
#define MAX_RAW_SPU            1

void handle_syscall(uint64_t arg);
void sysutil_callback(uint64_t status,
					  uint64_t param,
					  void    *userdata);

void sysutil_callback(uint64_t status,
					  uint64_t param,
					  void    *userdata)
{
	(void)param;
	(void)userdata;

	switch (status){
	case CELL_SYSUTIL_REQUEST_EXITGAME:
	exit(0);
		break;
	default:
		break;
    }

    return;
}

/*E
 * Loaded on an interrupt PPU thread, handles SPU's printf request.
 */
void handle_syscall(uint64_t arg)
{
	sys_raw_spu_t id = arg;
	uint64_t stat;
	uint32_t mail;
	int ret;
	int sys_ret = -1;

	/*E
	 * Create a tag to handle class 2 interrupt, because PPU Interrupt MB is 
	 * handled by class 2. 
	 */
	ret = sys_raw_spu_get_int_stat(id, 2, &stat);
	if (ret) {
		printf("sys_raw_spu_get_int_stat failed %x\n", ret);
		sys_interrupt_thread_eoi();
	}

	/*E
	 * If the caught class 2 interrupt includes mailbox interrupt, handle it.
	 */
	if ((stat & INT_STAT_MAILBOX) == INT_STAT_MAILBOX) {
		ret = sys_raw_spu_read_puint_mb(id, &mail);
		if (ret) {
			printf("sys_raw_spu_read_puint_mb failed %x\n", ret);
			sys_interrupt_thread_eoi();
		}
		uint32_t lsaddr;
		switch (mail >> 24) {
		case 0x1:
			lsaddr = sys_raw_spu_mmio_read(id, SPU_Out_MBox);
			sys_ret = raw_spu_printf(LS_BASE_ADDR(id), lsaddr);
			break;
		default:
			printf("undefined syscall.\n");
			break;
		}

		/*E
		 * Reset the PPU_INTR_MB interrupt status bit.
		 */
		ret = sys_raw_spu_set_int_stat(id, 2, stat & INT_STAT_MAILBOX);
		if (ret) {
			printf("sys_raw_spu_set_int_stat failed %x\n", ret);
			sys_interrupt_thread_eoi();
		}

		/*E 
		 * SPU's printf is expecting an acknowledgement and a return value are 
		 * passed to SPU MB.  
		 * For the acknowledgement, it uses 0 as successful. And return the
		 * return value. This is going to be the return value of SPU's printf.
		 */

		sys_raw_spu_mmio_write(id, SPU_In_MBox, 0);
		sys_raw_spu_mmio_write(id, SPU_In_MBox, sys_ret);
		EIEIO;

	} else {
		/*E
		 * Must reset interrupt status bit of those not handled.  
		 *
		 */
		ret = sys_raw_spu_set_int_stat(id, 2, stat);
		if (ret) {
			printf("sys_raw_spu_set_int_stat failed %x\n", ret);
			sys_interrupt_thread_eoi();
		}
	}

	/*E
	 * End of interrupt
	 */
	sys_interrupt_thread_eoi();
}


int main(void)
{ 
	int ret;
	sys_raw_spu_t id;
	sys_spu_image_t spu_img;

	//ret = init_exception_handler();

	/*
	 * Register sysutil callback function for shutdown handling
	 */
	ret = cellSysutilRegisterCallback(0, 
									  (CellSysutilCallback)sysutil_callback, 
									  NULL);

	if (ret != CELL_OK) {
		printf("systemUtilityInit() failed %x\n", ret);
		exit(ret);
	}

	/*E
	 * Initialize SPUs
	 */
	printf("Initializing SPUs\n");
	ret = sys_spu_initialize(MAX_PHYSICAL_SPU, MAX_RAW_SPU);
	if (ret) {
		printf("sys_spu_initialize failed %x\n", ret);
		exit(ret);
	}

	/*E
	 * Execute a series of system calls to load a program to a Raw SPU.
	 */
	ret = sys_raw_spu_create(&id, NULL);
	if (ret) {
		printf("sys_raw_spu_create failed %x\n", ret);
		exit(ret);
	}
	printf("sys_raw_spu_create succeeded. raw_spu number is %d\n", id);


	ret = sys_spu_image_open(&spu_img, SPU_PROG);
	if (ret != CELL_OK) {
		printf("sys_spu_image_open failed %x\n", ret);
		exit(1);
	}

	ret = sys_raw_spu_image_load(id, &spu_img);
	if (ret) {
		printf("sys_raw_spu_load failed %x\n", ret);
		exit(1);
	}

	/*E 
	 * Create an interrupt handler and establish it on an interrupt PPU 
	 * thread.  This PPU interrupt thread is going to handle SPU's printf
	 * request.
	 */
	sys_interrupt_tag_t intrtag;
	sys_ppu_thread_t handler;
	sys_interrupt_thread_handle_t ih;

	ret = sys_ppu_thread_create(&handler, handle_syscall, 0, 100,
								PPU_STACK_SIZE, SYS_PPU_THREAD_CREATE_INTERRUPT,
								"Interrupt Thread");
	if (ret) {
		printf("sys_ppu_thread_create is faild %x\n", ret);
		exit(1);
	}

	ret = sys_ppu_thread_set_priority(handler, 111);
	if (ret) {
		printf("sys_ppu_thread_set_priority faild %x\n", ret);
		exit(ret);
	}

	ret = sys_raw_spu_create_interrupt_tag(id, 2, 0, &intrtag);
	if (ret) {
		printf("sys_raw_spu_create_interrupt_tag failed %x\n", ret);
		exit(ret);
	}

	ret = sys_interrupt_thread_establish(&ih, intrtag, handler, id);
	if (ret) {
		printf("sys_intr_thread_establish failed %x\n", ret);
		exit(ret);
	}

	/*E
	 * Set interrupt mask.
	 * The third argument = 7 enables Halt, Stop-and-Signal and PPU Mailbox 
	 * interrupts.
	 */
	ret = sys_raw_spu_set_int_mask(id, 2, 7);
	if (ret) {
		printf("sys_raw_spu_set_int_mask failed %x\n", ret);
		exit(ret);
	}

	/*E
	 * Run the Raw SPU.
	 */
	sys_raw_spu_mmio_write(id, SPU_RunCntl, 0x1);

	uint32_t stat;

 	do {
		stat = sys_raw_spu_mmio_read(id, SPU_Status);
		sys_timer_usleep(1000);
	} while ((stat & 0x02U) == 0);


	printf("Destroying Raw SPU %d\n", id);
	if ((ret = sys_raw_spu_destroy(id)) != CELL_OK) {
		printf("sys_raw_spu_destroy failed %x\n", ret);
		exit(ret);
	}

	ret = sys_spu_image_close(&spu_img);
	if (ret != CELL_OK) {
		printf("sys_spu_image_close failed %x\n", ret);
		exit(ret);
	}

	return 0;
}

