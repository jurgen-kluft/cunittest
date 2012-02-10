#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/spu_initialize.h>
#include <sys/spu_image.h>
#include <sys/spu_thread.h>
#include <sys/spu_thread_group.h>
#include <sys/spu_utility.h>
#include <sys/event.h>
#include <sys/paths.h>
#include <sys/process.h>

#include "xunittest/ppu_exception_handler.h"

SYS_PROCESS_PARAM(1001, 0x10000)

#define MAX_PHYSICAL_SPU       4 
#define MAX_RAW_SPU            0
#define NUM_SPU_THREADS        1 /* The number of SPU threads in the group */ 
#define PRIORITY             100
//#define SPU_PROG       (SYS_APP_HOME "/event_receiver.spu.self")

#include <spu_printf.h>

void spu_thr_event_handler(uint64_t queue_id);

int main(int argc, char** argv)
{
	sys_spu_thread_group_t group; /* SPU thread group ID */
	const char *group_name = "Group";
	sys_spu_thread_group_attribute_t group_attr;/* SPU thread group attribute*/
	sys_spu_thread_t threads[NUM_SPU_THREADS];  /* SPU thread IDs */
	sys_spu_thread_attribute_t thread_attr;     /* SPU thread attribute */
	const char *thread_names[NUM_SPU_THREADS] = 
		{"SPU Thread 0"}; /* The names of SPU threads */
    sys_spu_image_t spu_img;

	int ret;
	
	/*
	 * Initialize Exception Handler
	 */
	ret = init_exception_handler();
	
	/*
	 * Initialize SPUs
	 */
	printf("Initializing SPUs\n");
	ret = sys_spu_initialize(MAX_PHYSICAL_SPU, MAX_RAW_SPU);
	if (ret != CELL_OK) {
		fprintf(stderr, "sys_spu_initialize failed: %#.8x\n", ret);
		exit(ret);
	}

	/*
	 * Create an SPU thread group
	 * 
	 * The SPU thread group is initially in the NOT INITIALIZED state.
	 */
	printf("Creating an SPU thread group.\n");

	sys_spu_thread_group_attribute_initialize(group_attr);
	sys_spu_thread_group_attribute_name(group_attr,group_name);

	ret = sys_spu_thread_group_create(&group, 
									  NUM_SPU_THREADS,
									  PRIORITY, 
									  &group_attr);
	if (ret != CELL_OK) {
		fprintf(stderr, "sys_spu_thread_group_create failed: %#.8x\n", ret);
		exit(ret);
	}
	
	if (argc > 1)
	{
		char image_name[128] = "";
		sprintf(image_name, "/app_home/%s", argv[1]);
		ret = sys_spu_image_open(&spu_img, image_name);
		if (ret != CELL_OK) {
			printf("sys_spu_image_open failed %x\n", ret);
			exit(1);
	}
	}else
	{
		printf("ERROR:  no SPU image name found! \n");
		printf("NOTICE: please pass the SPU image name as Command Line parameter \n");
		exit(1);
	}

	/*
	 * Initialize SPU threads in the SPU thread group.
	 * This sample loads the same image to all SPU threads.
	 */
	sys_spu_thread_attribute_initialize(thread_attr);
	for (int i = 0; i < NUM_SPU_THREADS; i++) {
		sys_spu_thread_argument_t thread_args;

		// TODO: jinlin, SpuProgramSize can be set here
		thread_args.arg1 = SYS_SPU_THREAD_ARGUMENT_LET_32(0);
		
		// TODO: jinlin, SpuStackSize can be set here, maybe 0x2000?
		thread_args.arg2 = SYS_SPU_THREAD_ARGUMENT_LET_32(0);

		printf("Initializing SPU thread %d\n", i);
		
		sys_spu_thread_attribute_name(thread_attr,thread_names[i]);
		
		ret = sys_spu_thread_initialize(&threads[i],
										group,
										i,
										&spu_img,
										&thread_attr,
										&thread_args);
		if (ret != CELL_OK) {
			fprintf(stderr, "sys_spu_thread_initialize failed: %#.8x\n", ret);
			exit(ret);
		}
	}

	printf("All SPU threads have been successfully initialized.\n");
	
	/*
	 * Turn on the spu_printf
	 */
	ret = spu_printf_initialize(1000, NULL);
	if (ret != CELL_OK) {
		printf("spu_printf_initialize failed %x\n", ret);
		exit(-1);
	}

	ret = spu_printf_attach_group(group);
	if (ret != CELL_OK) {
		printf("spu_printf_attach_group failed %x\n", ret);
		exit(-1);
	}

	/*
	 * Start the SPU thread group
	 */
	printf("Starting the SPU thread group.\n");
	ret = sys_spu_thread_group_start(group);
	if (ret != CELL_OK) {
		fprintf(stderr, "sys_spu_thread_group_start failed: %#.8x\n", ret);
		exit(ret);
	}
	
	/*
	 * Wait for the termination of the SPU thread group
	 */
	printf("Waiting for the SPU thread group to be terminated.\n");
	int cause, status;
	ret = sys_spu_thread_group_join(group, &cause, &status);
	if (ret != CELL_OK) {
		fprintf(stderr, "sys_spu_thread_group_join failed: %#.8x\n", ret);
		exit(ret);
	}

	/*
	 * Show the exit cause and status.
	 */
	switch(cause) {
	case SYS_SPU_THREAD_GROUP_JOIN_GROUP_EXIT:
		printf("The SPU thread group exited by sys_spu_thread_group_exit().\n");
		printf("The group's exit status = %d\n", status);
		break;
	case SYS_SPU_THREAD_GROUP_JOIN_ALL_THREADS_EXIT:
		printf("All SPU thread exited by sys_spu_thread_exit().\n");
		for (int i = 0; i < NUM_SPU_THREADS; i++) {
			int thr_exit_status;
			ret = sys_spu_thread_get_exit_status(threads[i], &thr_exit_status);
			if (ret != CELL_OK) {
				fprintf(stderr, "sys_spu_thread_get_exit_status failed: %#.8x\n", ret);
			}
			printf("SPU thread %d's exit status = %d\n", i, thr_exit_status);
		}
		break;
	case SYS_SPU_THREAD_GROUP_JOIN_TERMINATED:
		printf("The SPU thread group is terminated by sys_spu_thread_terminate().\n");
		printf("The group's exit status = %d\n", status);
		break;
	default:
		fprintf(stderr, "Unknown exit cause: %d\n", cause);
		break;
	}

	printf("The expected sum of all exit status is 300.\n");

	/*
	 * Destroy the SPU thread group
	 */
	
	ret = sys_spu_thread_group_destroy(group);
	if (ret != CELL_OK) {
		fprintf(stderr, "sys_spu_thread_group_destroy() failed: %#.8x\n", ret);
	} else {
		printf("Destroyed the SPU thread group.\n");
	}

	ret = sys_spu_image_close(&spu_img);
	if (ret != CELL_OK) {
		fprintf(stderr, "sys_spu_image_close failed: %.8x\n", ret);
	}
	
	printf("Exiting.\n");

	return 0;
}