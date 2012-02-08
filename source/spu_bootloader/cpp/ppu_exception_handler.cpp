/*
 *   SCE CONFIDENTIAL                                      
 *   PlayStation(R)3 Programmer Tool Runtime Library 300.001
 *   Copyright (C) 2009 Sony Computer Entertainment Inc.   
 *   All Rights Reserved.                                  
 *
 * File: exception_handler_main.ppu.c
 * Description:
 *  This sample shows how to use exception handling thread and get
 *  PPU and SPU thread context when the exception occured in the ppu
 *  thread and express them to stdout.
 *  In this sample you will see an example of the followings.
 *
 *  - Fig.1 is the program structure before exception occur.
 *
 *  - Before all of ppu/spu threads are launched, main program intialize 
 *    exception handler facility. Then main program regisiter the exception
 *    handler(*2) next.
 * 
 *  - Creating a PPU thread(*3)
 *    This thread started, wait for 5 seconds, then occur an exception.
 *
 *  - Creating an SPU thread group(*4) and SPU thread(*5)
 *    While PPU thread(*3) is waiting for 5 seconds, these SPU threads will be launched.
 *    These threads are just looping for getting these context when the exception occur.
 *
 *  - When the exception occured, exception handling thread(*2) will be called.
 *    In this thread, you can get PPU and SPU threads contexts in the same process, 
 *    Make this context to appropriate format text and then express them to console.
 *    And if you implement source code for output other device (network, file systems, etc),
 *    You can transfer ppu/spu thread context to anywhere at this time.
 *
 *  [Program Structure : Fig.1 ]
 *
 *  main(*1)								: main program thread
 *   |
 *   +---> exception handling thread(*2)	: exception handler
 *   |
 *   +---> ppu thread(*3)   				: Exception will be occured in this module
 *   |
 *   +-+-> spu thread group(*4)				: loop thread group
 *     |
 *     +--> spu thread 1(*5)				: loop thread
 *     |
 *     +--> spu thread 2(*5)				: loop thread
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ppu_thread.h>
#include <sys/spu_initialize.h>
#include <sys/spu_image.h>
#include <sys/spu_thread.h>
#include <sys/spu_thread_group.h>
#include <sys/spu_utility.h>
#include <sys/paths.h>
#include <sys/dbg.h>
#include <sys/timer.h>
#include <cell/sysmodule.h>

#include "xunittest/print_debug_info.h"
#include "xunittest/ppu_exception_handler.h"

/* Macros */
#define STACK_SIZE	0x4000	/* 16KB */
#define EXIT_CODE	0xbee

#define MAX_PHYSICAL_SPU	6
#define MAX_RAW_SPU			0
#define NUM_SPU_THREADS		2 /* The number of SPU threads in the group */ 
#define PRIORITY            100
#define HANDLER_PRIORITY    0x3e9

#define SPU_PROG     		(SYS_APP_HOME "/idleloop.spu.self")
#define SPU_PROG_FOR_BDVD	(SYS_DEV_BDVD "/PS3_GAME/USRDIR/idleloop.spu.self")		/* In case for DEX */
#define WRITE_BUF_SIZE	0x4000	/* 16KB */

/* Function prototype */
void exception_trigger_thread(uint64_t);
void ppu_exception_handler(uint64_t, sys_ppu_thread_t, uint64_t);
void write_tty_and_anydevice(char *);
int  init_exception_handler(void);
int  enable_fp_exception(fexcept_t);
int  disable_fp_exception(void);
int  write_floating_point_exception_type(char *, sys_ppu_thread_t);

static sys_spu_thread_t threads[NUM_SPU_THREADS];  /* SPU thread IDs */

/*
 * Exception trigger thread.
 */
void exception_trigger_thread(uint64_t fvalue_ptr)
{
	int ret;

	float fvalue = *((float*)((uintptr_t)fvalue_ptr));

	/*
	 * Wait 5 seconds for waiting spu threads launched.
	 */
	sys_timer_sleep(5);

	/*
	 * Enable floating-point exception.
	 */
	ret = enable_fp_exception(FE_ALL_EXCEPT);
	if (ret != CELL_OK) {
		fprintf(stderr, "enable_fp_exception failed: %#.8x\n", ret);
		sys_ppu_thread_exit(1);
	}

	puts("Exception occured.");

	/*
	 * Attempt to divide the number by zero to occure exception.
	 */
	float f = fvalue / 0.0f;	// Exception will occure here.
	printf("%lf\n", f);		// Not reached here.

	sys_ppu_thread_exit(EXIT_CODE);
}

/*
 * Exception handler
 * This function will be called when any exception occured in a PPU thread.
 *
 * The 1st argument specifies the exception cause.
 * 	One of following value will be passed.
 *		SYS_DBG_PPU_EXCEPTION_TRAP
 *		SYS_DBG_PPU_EXCEPTION_PREV_INST
 *		SYS_DBG_PPU_EXCEPTION_ILL_INST
 *		SYS_DBG_PPU_EXCEPTION_TEXT_HTAB_MISS
 *		SYS_DBG_PPU_EXCEPTION_TEXT_SLB_MISS
 *		SYS_DBG_PPU_EXCEPTION_DATA_HTAB_MISS
 *		SYS_DBG_PPU_EXCEPTION_DATA_SLB_MISS
 *		SYS_DBG_PPU_EXCEPTION_FLOAT
 *		SYS_DBG_PPU_EXCEPTION_DABR_MATCH
 *		SYS_DBG_PPU_EXCEPTION_ALIGNMENT
 *
 * The 2nd argument speifies the ppu-thread id which occured the exception.
 *
 * The 3rd argument specifies DAR(Data Address Register) value.
 *	In case of following exception cause is passed to the 1st argument.
 *  This value will be passed. In other cases, this value will be set to 0.
 *		SYS_DBG_PPU_EXCEPTION_DABR_MATCH
 *		SYS_DBG_PPU_EXCEPTION_ALIGNMENT
 *
 */
void ppu_exception_handler(uint64_t exp_cause,
                           sys_ppu_thread_t pu_thr_id,
						   uint64_t dar)
{
	int	ret;
	int	regnum;
	sys_dbg_ppu_thread_context_t	ppu_context;
	sys_dbg_spu_thread_context2_t	spu_context;

	/*
	 * Variable definition for accessing filesystems
	 */
	char	tmp_buf[128];
	char	*write_buf;

	/*
	 * Get write buffer memory
	 */
	write_buf = (char *)malloc(WRITE_BUF_SIZE);

	strcpy(write_buf, "+----------------------------+\n");
	strcat(write_buf, "| Exception handler started. |\n");
	strcat(write_buf, "+----------------------------+\n");
	strcat(write_buf, "Exception cause : ");
	switch(exp_cause){
		case SYS_DBG_PPU_EXCEPTION_TRAP:
			strcat(write_buf, "SYS_DBG_PPU_EXCEPTION_TRAP.\n");
			break;

		case SYS_DBG_PPU_EXCEPTION_PREV_INST:
			strcat(write_buf, "SYS_DBG_PPU_EXCEPTION_PREV_INST.\n");
			break;

		case SYS_DBG_PPU_EXCEPTION_ILL_INST:
			strcat(write_buf, "SYS_DBG_PPU_EXCEPTION_ILL_INST.\n");
			break;

		case SYS_DBG_PPU_EXCEPTION_TEXT_HTAB_MISS:
			strcat(write_buf, "SYS_DBG_PPU_EXCEPTION_TEXT_HTAB_MISS.\n");
			break;

		case SYS_DBG_PPU_EXCEPTION_TEXT_SLB_MISS:
			strcat(write_buf, "SYS_DBG_PPU_EXCEPTION_TEXT_SLB_MISS.\n");
			break;

		case SYS_DBG_PPU_EXCEPTION_DATA_HTAB_MISS:
			strcat(write_buf, "SYS_DBG_PPU_EXCEPTION_DATA_HTAB_MISS.\n");
			break;

		case SYS_DBG_PPU_EXCEPTION_DATA_SLB_MISS:
			strcat(write_buf, "SYS_DBG_PPU_EXCEPTION_DATA_SLB_MISS.\n");
			break;

		case SYS_DBG_PPU_EXCEPTION_FLOAT:
			strcat(write_buf, "SYS_DBG_PPU_EXCEPTION_FLOAT ");
			ret = write_floating_point_exception_type(write_buf, pu_thr_id);
			if (ret != CELL_OK) {
				fprintf(stderr, "write_floating_point_exception_type was failed: %#.8x\n", ret);
			}
			break;

		case SYS_DBG_PPU_EXCEPTION_DABR_MATCH:
			strcat(write_buf, "SYS_DBG_PPU_EXCEPTION_DABR_MATCH.\n");
			sprintf(tmp_buf, "DAR(Data Address Register) = %016llx\n", dar);
			strcat(write_buf, tmp_buf);
			break;

		case SYS_DBG_PPU_EXCEPTION_ALIGNMENT:
			strcat(write_buf, "SYS_DBG_PPU_EXCEPTION_ALIGNMENT.\n");
			sprintf(tmp_buf, "DAR(Data Address Register) = %016llx\n", dar);
			strcat(write_buf, tmp_buf);
			break;

		default:
			strcat(write_buf, "unknown.\n");
			break;
	}

	/*
	 * Output to console and file system
	 */
	write_tty_and_anydevice(write_buf);
	
	/*
	 * Get PPU thread context when exception occured
	 *
	 * The 1st argument specifies the ppu-thread id that you want to get it's context data.
	 *
	 * The 2nd argument specifies the pointer to the ppu-thread context data structure.
	 *
	 * This function read the ppu-thread context of specified ppu-thread id. 
	 */
	ret = sys_dbg_read_ppu_thread_context(pu_thr_id, &ppu_context);
	if(ret != CELL_OK){
		fprintf(stderr, "sys_dbg_read_ppu_thread_context for 0x%016llx failed.\n", pu_thr_id);
	}
	else{
		strcpy(write_buf, "+--------------------------+\n");
		strcat(write_buf, "| PPU thread context data  |\n");
		sprintf(tmp_buf,     "| TID = 0x%016llx |\n", pu_thr_id);
		strcat(write_buf, tmp_buf);
		strcat(write_buf, "+--------------------------+\n");

		for(regnum = 0 ; regnum < PPU_GPR_NUM ; regnum++){
			sprintf(tmp_buf, "GPR[%02d] = 0x%016llx\n", regnum, ppu_context.gpr[regnum]);
			strcat(write_buf, tmp_buf);
		}

		sprintf(tmp_buf, "CR      = 0x%08x\n", ppu_context.cr);
		strcat(write_buf, tmp_buf);

		sprintf(tmp_buf, "XER     = 0x%016llx\n", ppu_context.xer);
		strcat(write_buf, tmp_buf);

		sprintf(tmp_buf, "LR      = 0x%016llx\n", ppu_context.lr);
		strcat(write_buf, tmp_buf);

		sprintf(tmp_buf, "CTR     = 0x%016llx\n", ppu_context.ctr);
		strcat(write_buf, tmp_buf);

		sprintf(tmp_buf, "PC      = 0x%016llx\n", ppu_context.pc);
		strcat(write_buf, tmp_buf);

		for(regnum = 0 ; regnum < PPU_FPR_NUM ; regnum++){
			sprintf(tmp_buf, "FPR[%02d] = 0x%016llx\n", regnum, ppu_context.fpr[regnum]);
			strcat(write_buf, tmp_buf);
		}

		sprintf(tmp_buf, "FPSCR   = 0x%08x\n", ppu_context.fpscr);
		strcat(write_buf, tmp_buf);

		for(regnum = 0 ; regnum < PPU_VR_NUM ; regnum++){
			sprintf(tmp_buf, "VMX[%02d] = 0x%016llx%016llx\n", regnum, ppu_context.vr[regnum].dw[0], ppu_context.vr[regnum].dw[1]);
			strcat(write_buf, tmp_buf);
		}

		sprintf(tmp_buf, "VSCR    = 0x%016llx%016llx\n", ppu_context.vscr.dw[0], ppu_context.vscr.dw[1]);
		strcat(write_buf, tmp_buf);

		/*
		 * Output to console and any device
		 */
		write_tty_and_anydevice(write_buf);
	}		

	for (int i = 0; i < NUM_SPU_THREADS; i++) {
		/*
		 * Get SPU thread context when exception occured
		 *
		 * The 1st argument specifies the spu-thread id that you want to get it's spu-thread context data.
		 *
		 * The 2nd argument specifies the pointer to the spu-thread context data structure.
		 */
		ret = sys_dbg_read_spu_thread_context2(threads[i], &spu_context);
		if(ret != CELL_OK){
			fprintf(stderr, "sys_dbg_read_spu_thread_context2 for 0x%08x failed(0x%x).\n", threads[i], ret);
		}
		else{
			strcpy(write_buf, "+-------------------------+\n");
			strcat(write_buf, "| SPU thread context data |\n");
			sprintf(tmp_buf,     "|     TID = 0x%08x    |\n", threads[i]);
			strcat(write_buf, tmp_buf);
			strcat(write_buf, "+-------------------------+\n");

			for(regnum = 0 ; regnum < SPU_GPR_NUM ; regnum++){
				sprintf(tmp_buf, "GPR[%03d]       = 0x%016llx%016llx\n", regnum, spu_context.gpr[regnum].dw[0], spu_context.gpr[regnum].dw[1]);
				strcat(write_buf, tmp_buf);
			}

			sprintf(tmp_buf, "NPC            = 0x%08x\n", spu_context.npc);
			strcat(write_buf, tmp_buf);

			sprintf(tmp_buf, "FPSCR          = 0x%016llx%016llx\n", spu_context.fpscr.dw[0], spu_context.fpscr.dw[1]);
			strcat(write_buf, tmp_buf);

			sprintf(tmp_buf, "SRR0           = 0x%08x\n", spu_context.srr0);
			strcat(write_buf, tmp_buf);

			sprintf(tmp_buf, "SPU_STATUS     = 0x%08x\n", spu_context.spu_status);
			strcat(write_buf, tmp_buf);

			sprintf(tmp_buf, "SPU_CFG        = 0x%016llx\n", spu_context.spu_cfg);
			strcat(write_buf, tmp_buf);

			sprintf(tmp_buf, "MB_STAT        = 0x%08x\n", spu_context.mb_stat);
			strcat(write_buf, tmp_buf);

			sprintf(tmp_buf, "PPU_MB         = 0x%08x\n", spu_context.ppu_mb);
			strcat(write_buf, tmp_buf);

			for(regnum = 0 ; regnum < SPU_MB_NUM ; regnum++){
				sprintf(tmp_buf, "SPU_MB[%d]      = 0x%08x\n", regnum, spu_context.spu_mb[regnum]);
				strcat(write_buf, tmp_buf);
			}

			sprintf(tmp_buf, "DECREMENTER    = 0x%08x\n", spu_context.decrementer);
			strcat(write_buf, tmp_buf);

			for(regnum = 0 ; regnum < SPU_MFC_CQ_SR_NUM ; regnum++){
				sprintf(tmp_buf, "MFC_CQ_SR[%02d]  = 0x%016llx\n", regnum, spu_context.mfc_cq_sr[regnum]);
				strcat(write_buf, tmp_buf);
			}

			/*
			 * Output to console and file system
	 		 */
			write_tty_and_anydevice(write_buf);
		}
	}		

	/*
	 * Free buffer
	 */
	free(write_buf);


        /*
         * Output some debugging information in this process
         *
         */
        ret = print_process_info();
        if (ret != CELL_OK) {
                fprintf(stderr, "print_process_info() was failed: %#.8x\n", ret);                
        }



	/*
	 * Finalizing ppu exception handler
	 *
	 * No argument.
	 *
	 * This function will be called for terminating excption handling thread.
	 */
	ret = sys_dbg_finalize_ppu_exception_handler();
	if (ret != CELL_OK) {
		fprintf(stderr, "sys_dbg_finalize_ppu_exception_handler failed: %#.8x\n", ret);
	}

	/*
	 * Unregistering ppu exception handler
	 *
	 * No argument.
	 *
	 * This function will be called for unregistering unused excption handling thread.
	 */
	ret = sys_dbg_unregister_ppu_exception_handler();
	if (ret != CELL_OK) {
		fprintf(stderr, "sys_dbg_unregister_ppu_exception_handler failed: %#.8x\n", ret);
	}

}


/*
 * Function that output text to console and any device
 */
void write_tty_and_anydevice(char *buf){
	/*
	 * Show the ppu/spu contexts to console
 	 */
	puts((char*)(uintptr_t)buf);

	/*
	 * You implement source code for output to any devices (Network, Filesystem, etc)
 	 */

}

/*
 * Function that initialize exception_handler
 */
int init_exception_handler(void)
{
    int ret;

	/* 
	 * Loading prx for liblv2dbg via libsysmodule
	 */ 
	ret = cellSysmoduleLoadModule(CELL_SYSMODULE_LV2DBG);
	if (ret != CELL_OK) {
	  printf("cellSysmoduleLoadModule() error 0x%x !\n", ret);
	  exit(ret);
	}

  	/*
	 * Initialize exception handler
	 *
	 * The 1st argument specifies the priority of the exception handling thread will run.
	 * The range of priority is between 0 to 3071. The highest priority value is 0.
	 * 
	 */
	ret = sys_dbg_initialize_ppu_exception_handler(HANDLER_PRIORITY);
	if (ret != CELL_OK) {
		return ret;
	}

	/*
	 * Register exception handler
	 *
	 * The 1st argument specifies the pointer to exception handling thread.
	 * This thread will be started when exception occur in a ppu-thread.
	 *
	 * The 2nd argument specifies the control flag by following constants.
	 * User can specify the behavior when exception occur in a ppu-thread.
	 * These constants mean
	 *
	 * SYS_DBG_PPU_THREAD_STOP :
	 *		Stop all of PPU-threads in the same process when exception occur in any 
	 * SYS_DBG_SPU_THREAD_GROUP_STOP :
	 *		Stop SPU-threads in the same process when exception occur.
	 */
	ret = sys_dbg_register_ppu_exception_handler(ppu_exception_handler, SYS_DBG_PPU_THREAD_STOP | SYS_DBG_SPU_THREAD_GROUP_STOP );
	if (ret != CELL_OK) {
		return ret;
	}

	return CELL_OK;
}


/*
 * Function that enable floating-point exception handling to 
 * the caller thread.
 */
int enable_fp_exception(fexcept_t enables)
{
	int ret;
	sys_ppu_thread_t id;
	
	ret = feclearexcept(FE_ALL_EXCEPT);
	if (ret != 0) {
		return ret;
	}

	ret = sys_ppu_thread_get_id(&id);
	if (ret != CELL_OK) {
		return ret;
	}

	ret = sys_dbg_enable_floating_point_enabled_exception(id, 0, 0, 0);
	if (ret != CELL_OK) {
		return ret;
	}

	/*
	 * Set the floating-point exception mask to allow all exception types.
	 */
	ret = fesettrapenable(enables);
	if (ret != 0) {
		return ret;
	}

	return CELL_OK;
}


/*
 * Function that disable floating-point exception handling to 
 * the caller thread.
 */
int disable_fp_exception(void)
{
	int ret;
	sys_ppu_thread_t id;
	
	ret = sys_ppu_thread_get_id(&id);
	if (ret != CELL_OK) {
		return ret;
	}

	ret = sys_dbg_disable_floating_point_enabled_exception(id, 0, 0, 0);
	if (ret != CELL_OK) {
		return ret;
	}

	/*
	 * Set the floating-point exception mask to deny all exception types.
	 */
	ret = fesettrapenable(0);
	if (ret != 0) {
		return ret;
	}

	return CELL_OK;

}

/*
 * Get and write floating-point exception types.
 */
int write_floating_point_exception_type(char *buf, 
										sys_ppu_thread_t pu_thr_id)
{
	int ret;
	sys_dbg_ppu_thread_context_t ppu_context;

	ret = sys_dbg_read_ppu_thread_context(pu_thr_id, &ppu_context);
	if (ret != CELL_OK) {
		return ret;
	}

	uint32_t fpscr = ppu_context.fpscr;
	strcat(buf, "(");
	if (fpscr & FE_DIVBYZERO)
		strcat(buf, " FE_DIVBYZERO");
	if (fpscr & FE_INEXACT) 
		strcat(buf, " FE_INEXACT");
	if (fpscr & FE_INVALID)
		strcat(buf, " FE_INVALID");
	if (fpscr & FE_OVERFLOW)
		strcat(buf, " FE_OVERFLOW");
	if (fpscr & FE_UNDERFLOW)
		strcat(buf, " FE_UNDERFLOW");
	strcat(buf, " )\n");

	return CELL_OK;

}
