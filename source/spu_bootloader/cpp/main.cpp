﻿#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/spu_initialize.h>
#include <sys/spu_image.h>
#include <sys/spu_thread.h>
#include <sys/spu_thread_group.h>
#include <sys/spu_utility.h>
#include <sys/ppu_thread.h>
#include <sys/event.h>
#include <sys/paths.h>
#include <sys/process.h>
#include <sys/timer.h>

#include <spu_printf.h>

#include <cell/cell_fs.h>

#include "xunittest/ppu_exception_handler.h"

SYS_PROCESS_PARAM(1001, 0x10000)

#define MAX_PHYSICAL_SPU       4 
#define MAX_RAW_SPU            0
#define NUM_SPU_THREADS        1 /* The number of SPU threads in the group */ 
#define PRIORITY             100
//#define SPU_PROG       (SYS_APP_HOME "/event_receiver.spu.self")

#define QUEUE_SIZE             32

bool exception_detected = false;

void spu_thr_exception_handler(uint64_t queue_id);

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
	int spu_file_size = 0;

	/*
	 * Variables for the event queue and event handler PPU thread
	 */
	sys_event_queue_t queue;
	sys_event_queue_attribute_t queue_attr;
	sys_ppu_thread_t event_handle_thread; 

	int ret;
	bool first_time_execution = true;
	int exception_count = 0;
	
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
	 * Initialize the event queue
	 * This event queue will be connected to the SPU threads.
	 */
	printf("Creating an event queue.\n");
	queue_attr.attr_protocol = SYS_SYNC_PRIORITY;
	queue_attr.type = SYS_PPU_QUEUE;
	ret = sys_event_queue_create(&queue, &queue_attr, SYS_EVENT_QUEUE_LOCAL, 
								 QUEUE_SIZE);
	if (ret != CELL_OK) {
		fprintf(stderr, "sys_event_queue_create failed: %#.8x\n", ret);
		exit(ret);
	}
	
	/*
	 * Create a PPU thread to handle SPU thread events.
	 *
	 * This thread receives and handles the SPU thread group exceptions.  
	 */
	sys_ppu_thread_t my_thread_id;
	int event_handle_prio;
	const char *event_handle_name = "SPU Thread Exception Handler";

	sys_ppu_thread_get_id(&my_thread_id);
	ret = sys_ppu_thread_get_priority(my_thread_id, &event_handle_prio);
	if (ret != CELL_OK) {
		fprintf(stderr, "sys_ppu_thread_get_priority failed: %#.8x\n", ret);
		exit(ret);	
	} else 	{
		event_handle_prio += 1;
	}

	ret = sys_ppu_thread_create(&event_handle_thread, spu_thr_exception_handler,
								(uint64_t)queue, event_handle_prio,
								4096, SYS_PPU_THREAD_CREATE_JOINABLE,
								event_handle_name);
	if (ret != CELL_OK) {
		fprintf(stderr, "sys_ppu_thread_create failed: %#.8x\n", ret);
		exit(ret);
	}


	/*
	 * Turn on the spu_printf
	 */
	ret = spu_printf_initialize(1000, NULL);
	if (ret != CELL_OK) {
		printf("spu_printf_initialize failed %x\n", ret);
		exit(-1);
	}
	
	// Open SPU image
	if (argc > 1)
	{
		char image_name[256] = "";
		sprintf(image_name, "/app_home/%s", argv[1]);

		// open file
		int fd;
		ret = cellFsOpen(image_name, CELL_FS_O_RDONLY, &fd, NULL, 0);
		if (ret != CELL_FS_SUCCEEDED) {
			fprintf(stderr, "cellFsOpen %s failed: 0x%x\n", image_name, ret);
			return -1;
		}
		
		// obtain file size
		CellFsStat status;
		memset(&status, 0, sizeof(CellFsStat));
		ret = cellFsFstat(fd, &status);
		if (ret != CELL_FS_SUCCEEDED) {
			fprintf(stderr, "cellFsFstat failed: 0x%x\n", ret);
			cellFsClose(fd);
			return -1;
		}

		spu_file_size = status.st_size;
		printf("spu program file size = %d byte \n", spu_file_size);

		// close file
		cellFsClose(fd);

		ret = sys_spu_image_open(&spu_img, image_name);
		if (ret != CELL_OK) {
			printf("sys_spu_image_open failed with error %x opening program: %s\n", ret, image_name);

			if(ret == ENOENT)
				printf("SPU program file does not exist.\n");
			else if(ret == EINVAL)
				printf("Path to program is not absolute.\n");
			else if(ret == EMFILE)
				printf("Too many file descriptors?\n");
			else if(ret == EISDIR)
				printf("Path is directory..\n");
			else if(ret == ENOTDIR)
				printf("Elements of path include non directory\n");
			else if(ret == ENAMETOOLONG)
				printf("Length of path elements is exceeding the limit\n");
			else if(ret == EFSSPECIFIC)
				printf("Error specific to the file system occured.\n");
			else if(ret == ENOEXEC)
				printf("The format used in the elf file is not supported OR authorization error\n");
			else if(ret == EAGAIN)
				printf("Not enough resources to process\n");
			else if(ret == ENOMEM)
				printf("Insufficient user memory\n");
			else if(ret == EFAULT)
				printf("A specified address was invalid\n");
			else if(ret == ENOTMOUNTED)
				printf("The file system which corresponds to path is not mounted\n");
			else
				printf("Unknown error????\n");

			exit(1);
		}
	}
	else
	{
		printf("ERROR:  no SPU image name found! \n");
		printf("NOTICE: please pass the SPU image name as Command Line parameter \n");
		exit(1);
	}

	while (1)
	{
		if (exception_detected || first_time_execution)
		{
			if (exception_detected)
			{
				++exception_count;
				exception_detected = false;

				//Disconnect the Event to Event Queue
				ret = sys_spu_thread_group_disconnect_event(group, SYS_SPU_THREAD_GROUP_EVENT_EXCEPTION);
				if (ret != CELL_OK) {
					printf("sys_spu_thread_group_disconnect_event failed %x\n", ret);
					exit(1);
				}

				//Detach the SPU printf thread from the thread group
				ret = spu_printf_detach_group(group);
				if (ret != CELL_OK) {
					printf("spu_printf_detach_group failed %x\n", ret);
					exit(-1);
				}

				/*
				 * Destroy the SPU thread group
				 */
	
				ret = sys_spu_thread_group_destroy(group);
				if (ret != CELL_OK) {
					fprintf(stderr, "sys_spu_thread_group_destroy() failed: %#.8x\n", ret);
				} else {
					printf("Destroyed the SPU thread group.\n");
				}
			}

			if (first_time_execution)
			{
				first_time_execution = false;
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

			/*
			 * Initialize SPU threads in the SPU thread group.
			 * This sample loads the same image to all SPU threads.
			 */
			sys_spu_thread_attribute_initialize(thread_attr);
			for (int i = 0; i < NUM_SPU_THREADS; i++) {
				sys_spu_thread_argument_t thread_args;

		// set the SPU Program Size as the thread argument 1
		thread_args.arg1 = SYS_SPU_THREAD_ARGUMENT_LET_32(spu_file_size);
		
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
			
			
			// Attach spu printf thread to the thread group
			ret = spu_printf_attach_group(group);
			if (ret != CELL_OK) {
				printf("spu_printf_attach_group failed %x\n", ret);
				exit(-1);
			}
			
			
			//Connect the Event to Event Queue
			ret = sys_spu_thread_group_connect_event(group, queue, SYS_SPU_THREAD_GROUP_EVENT_EXCEPTION);
			if (ret != CELL_OK) {
				printf("sys_spu_thread_group_connect_event failed %x\n", ret);
				exit(1);
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

			// TODO: Temporary added to prevent the code from running forever. To be removed.
			if (exception_count == 5)
			{
				break;			
			}
		}
		else
		{
			break;
		}	
	}

	//Disconnect the Event to Event Queue
	ret = sys_spu_thread_group_disconnect_event(group, SYS_SPU_THREAD_GROUP_EVENT_EXCEPTION);
	if (ret != CELL_OK) {
		printf("sys_spu_thread_group_disconnect_event failed %x\n", ret);
		exit(1);
	}

	ret = sys_event_queue_destroy(queue, 0);
	if (ret != CELL_OK) {
		fprintf(stderr, "sys_event_queue_destroy failed: %#.8x\n", ret);
	} else {
		printf("Destroyed the event queue.\n");
	}
	
	
	//Detach the SPU printf thread from the thread group
	ret = spu_printf_detach_group(group);
	if (ret != CELL_OK) {
		printf("spu_printf_detach_group failed %x\n", ret);
		exit(-1);
	}

	// Finalize SPU printf thread
	ret = spu_printf_finalize();
	if (ret != CELL_OK) {
		printf("spu_printf_finalize failed %x\n", ret);
		exit(-1);
	}
	
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

/*
 * SPU Thread Event handler
 *
 * \param queue_id Event queue ID (Casted to uint64_t)
 */
void spu_thr_exception_handler(uint64_t uint64_t_queue_id) 
{
	sys_event_queue_t queue_id = uint64_t_queue_id;
	sys_spu_thread_group_t source_group_id;
	sys_spu_thread_t source_thread_id;
	uint32_t spu_npc;
	uint32_t exception_cause;

	int ret;

	while (1) {
		sys_event_t event;
		ret = sys_event_queue_receive(queue_id, &event, SYS_NO_TIMEOUT);
		if (ret != CELL_OK) {
			if (ret == (int)ECANCELED) {
				break;
			} else {
				fprintf(stderr, "sys_event_queue_receive failed: %#.8x\n", ret);
				sys_ppu_thread_exit(ret);
			}
		}

		if (event.source == SYS_SPU_THREAD_GROUP_EVENT_EXCEPTION_KEY) {
			source_group_id = (sys_spu_thread_group_t)(event.data1>>32);
			source_thread_id = (sys_spu_thread_t)event.data1;
			spu_npc = (uint32_t)(event.data2>>32);
			exception_cause = (uint32_t)event.data2;
			
			printf("SPU exception detected.\n");
			printf("SPU Thread Group ID: %x\n", source_group_id);
			printf("SPU Thread ID: %x\n", source_thread_id);
			printf("SPU NPC: %x\n", spu_npc);
			printf("SPU Exception Cause:\n");

			/*
			 * Show the cause of exception
			 */
			switch(exception_cause) {
			case SYS_SPU_EXCEPTION_DMA_ALIGNMENT:
				printf("An MFC DMA alignment interrupt occurred.\n");
				break;
			case SYS_SPU_EXCEPTION_DMA_COMMAND:
				printf("An invalid DMA command interrupt occurred.\n");
				break;
			case SYS_SPU_EXCEPTION_SPU_ERROR:
				printf("An SPU error interrupt occurred.\n");
				break;
			case SYS_SPU_EXCEPTION_MFC_FIR:
				printf("An MFC FIR interrupt occurred.\n");
				break;
			case SYS_SPU_EXCEPTION_MFC_SEGMENT:
				printf("An MFC data segment interrupt occurred.\n");
				printf("MFC_DAR: %x\n", event.data3);
				break;
			case SYS_SPU_EXCEPTION_MFC_STORAGE:
				printf("An MFC data storage interrupt occurred.\n");
				printf("MFC_DAR: %x\n", event.data3);
				break;
			case SYS_SPU_EXCEPTION_MAT:
				printf("An MAT (memory access trap) exception occurred.\n");
				printf("MFC_DAR: %x\n", event.data3);
				break;
			case SYS_SPU_EXCEPTION_STOP_BREAK:
				printf("A stopd instruction or a stop 0x3fff instruction was issued.\n");
				break;
			case SYS_SPU_EXCEPTION_HALT:
				printf("A halt instruction was issued.\n");
				break;
			case SYS_SPU_EXCEPTION_UNKNOWN_SIGNAL:
				printf("A stop instruction that cannot be understood by the system was issued.\n");
				printf("SPU_Status: %x\n", event.data3);
				break;
			default:
				fprintf(stderr, "Unknown exception cause: %d\n", exception_cause);
				break;
			}

			ret = sys_spu_thread_group_terminate(source_group_id, 0);

			if (ret != CELL_OK) {
				fprintf(stderr, "sys_spu_thread_group_terminate failed: %#.8x\n", ret);
			}
			else
			{
				exception_detected = true;

				while (exception_detected)
				{
					sys_timer_usleep(30);
				}
			}
		} else {
			printf("Unexpected Event.\n");
		}
	}
	printf("SPU Thread Event Handler is exiting.\n");
	sys_ppu_thread_exit(0);
}