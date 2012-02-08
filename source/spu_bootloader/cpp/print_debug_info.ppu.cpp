/*
 *   SCE CONFIDENTIAL                                      
 *   PlayStation(R)3 Programmer Tool Runtime Library 300.001
 *   Copyright (C) 2009 Sony Computer Entertainment Inc.   
 *   All Rights Reserved.                                  
 *
 * File: print_debug_info.ppu.c
 * Description:
 *  This sample outputs some debugging informations to TTY.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/dbg.h>
#include "xunittest/print_debug_info.h"


int print_process_info(void)
{
        int ret;
        printf(" <Process: 0x%x>\n", sys_process_getpid());
        printf("  |\n");


        /* PPU thread */
        ret = print_ppu_thread_ids();
        if (ret != CELL_OK) {
                return ret;
        }
        printf("  |\n");


        /* SPU thread group and SPU thread */        
        ret = print_spu_thread_group_ids();
        if (ret != CELL_OK) {
                return ret;
        }
        printf("  |\n");

        
        /* Mutex */
        ret = print_sync_ids(SYS_MUTEX_OBJECT);
        if (ret != CELL_OK) {
                return ret;
        }
        printf("  |\n");        


        /* Lwmutex */
        ret = print_sync_ids(SYS_LWMUTEX_OBJECT);
        if (ret != CELL_OK) {
                return ret;
        }
        printf("  |\n");        

        
        /* Condition variable */
        ret = print_sync_ids(SYS_COND_OBJECT);
        if (ret != CELL_OK) {
                return ret;
        }
        printf("  |\n");        


        /* Lightweight condition variable */
        ret = print_sync_ids(SYS_LWCOND_OBJECT);
        if (ret != CELL_OK) {
                return ret;
        }
        printf("  |\n");
        

        /* Event queue */
        ret = print_sync_ids(SYS_EVENT_QUEUE_OBJECT);
        if (ret != CELL_OK) {
                return ret;
        }
        printf("  |\n");

        
        /* Reader/Writer lock */
        ret = print_sync_ids(SYS_RWLOCK_OBJECT);
        if (ret != CELL_OK) {
                return ret;
        }
        printf("  |\n");
        
        
        /* Semaphore */
        ret = print_sync_ids(SYS_SEMAPHORE_OBJECT);
        if (ret != CELL_OK) {
                return ret;
        }
        printf("  |\n");
        
        /* Event flag */
        ret = print_sync_ids(SYS_EVENT_FLAG_OBJECT);
        if (ret != CELL_OK) {
                return ret;
        }

        return CELL_OK;
}


int print_ppu_thread_ids(void)
{
        int ret;

        sys_ppu_thread_t* ids;
        uint64_t ids_num;
        uint64_t all_ids_num;

        /*
         * Get a number of PPU thread IDs in my process
         *
         */

        ret = sys_dbg_get_ppu_thread_ids(NULL, NULL, &all_ids_num);
        if (ret != CELL_OK) {
                return ret;
        }


        /*
         * Allocate memory for storing a list of PPU thread IDs
         *
         */

        ids = (sys_ppu_thread_t *)(malloc(sizeof(sys_ppu_thread_t) * (size_t)all_ids_num));
        if (ids == NULL) {
                return -1; // memory allocation will be failed.
        }

        
        /*
         * Get a list of PPU thread IDs
         *
         */

        ids_num = all_ids_num;	// set allocated buffer size

        ret = sys_dbg_get_ppu_thread_ids(ids, &ids_num, &all_ids_num);
        if (ret != CELL_OK) {
                free(ids);
                return ret;
        }


        /*
         * Output PPU thread ID list
         *
         */
        
        printf("  +-[PPU Thread]: %llu \n", all_ids_num);


        int index = (int)ids_num -1;
        
        for (int i=0; i<(int)ids_num; i++) {

                printf("  |     |\n");                
                printf("  |     +-[%d]: 0x%llx\n", i, ids[index]);

                ret = print_ppu_thread_info(ids[index]);
                if (ret != CELL_OK) {
                        free(ids);
                        return ret;
                }

                index--;
        }

        /* free memory space */
        free(ids);

        return CELL_OK;
}


int print_ppu_thread_info(sys_ppu_thread_t id)
{
        int ret;
        
        /*
         * Get a priority of the target PPU thread
         *
         */
        
        int prio;

        ret = sys_ppu_thread_get_priority(id, &prio);
        if (ret != CELL_OK) {
                return ret;
        }
        

        /*
         * Get a status of the target PPU thread
         *
         */
        
        sys_dbg_ppu_thread_status_t status;

        ret = sys_dbg_get_ppu_thread_status(id, &status);
        if (ret != CELL_OK) {
                return ret;
        }


        /*
         * Get a name of the target PPU thread.
         * 
         */
        
        char name[MAX_THREAD_NAME];
        
        ret = sys_dbg_get_ppu_thread_name(id, name);
        if (ret != CELL_OK) {
                return ret;
        }

        
        /*
         * Print ppu thread information to TTY
         *
         */
        
        printf("  |     |        |-[Priority]: 0x%x\n", prio);
        printf("  |     |        |-[Name]: %s\n", name);
        printf("  |     |        |-[Status]: ");        
        switch (status) {
        case PPU_THREAD_STATUS_IDLE:
                printf("IDLE\n");
                break;
        case PPU_THREAD_STATUS_RUNNABLE:
                printf("RUNNABLE\n");
                break;
        case PPU_THREAD_STATUS_ONPROC:
                printf("ONPROC\n");
                break;
        case PPU_THREAD_STATUS_SLEEP:
                printf("SLEEP\n");
                break;
        case PPU_THREAD_STATUS_STOP:
                printf("STOP\n");
                break;
        case PPU_THREAD_STATUS_ZOMBIE:
                printf("ZOMBIE\n");
                break;
        case PPU_THREAD_STATUS_DELETED:
                printf("DELETED\n");
                break;
        case PPU_THREAD_STATUS_UNKNOWN:
                printf("UNKNOWN\n");
                break;
        default:
                printf("????????\n");
                break;
        }        

        return CELL_OK;
}

int print_spu_thread_group_ids(void)
{
        int ret;
        
        sys_spu_thread_group_t* ids;        
        uint64_t ids_num;
        uint64_t all_ids_num;

        /*
         * Get a number of SPU thread group IDs in my process
         *
         */

        ret = sys_dbg_get_spu_thread_group_ids(NULL, NULL, &all_ids_num);
        if (ret != CELL_OK) {
                return ret;
        }


        /*
         * Allocate memory for storing a list of SPU thread group IDs
         *
         */

        ids = (sys_spu_thread_group_t *)(malloc(sizeof(sys_spu_thread_group_t) * (size_t)all_ids_num));
        if (ids == NULL) {
                return -1; // memory allocation will be failed.
        }
        

        /*
         * Get a list of SPU thread group IDs
         *
         */

        ids_num = all_ids_num;	// set allocated buffer size

        ret = sys_dbg_get_spu_thread_group_ids(ids, &ids_num, &all_ids_num);
        if (ret != CELL_OK) {
                free(ids);
                return ret;
        }


        /*
         * Output SPU thread group ID list
         *
         */
        
        printf("  +-[SPU Thread group]: %llu \n", all_ids_num);
        
        for (int i=0; i<(int)ids_num; i++) {

                printf("  |     |\n");                
                printf("  |     +-[%d]: 0x%x \n", i, ids[i]);

                ret =  print_spu_thread_group_info(ids[i]);
                if (ret != CELL_OK) {
                        free(ids);
                        return ret;
                }
        }


        /* free memory space */
        free(ids);

        return CELL_OK;
        
}


int print_spu_thread_group_info(sys_spu_thread_group_t id)
{
        int ret;

        /*
         * Get a name of the target SPU thread group
         *
         */
        
        char name[MAX_THREAD_NAME];

        ret = sys_dbg_get_spu_thread_group_name(id, name);
        if (ret != CELL_OK) {
                return ret;
        }


        /*
         * Get a priority of the target SPU thread group
         *
         */
        
        int prio;

        ret = sys_spu_thread_group_get_priority(id, &prio);
        if (ret != CELL_OK) {
                return ret;
        }


        /*
         * Get a status of the target SPU thread group
         *
         */

        sys_dbg_spu_thread_group_status_t status;
        
        ret = sys_dbg_get_spu_thread_group_status(id, &status);
        if (ret != CELL_OK) {
                return ret;
        }
        

        /*
         * Print SPU thread group information to TTY
         *
         */

        printf("  |     |        |-[Priority]: 0x%x\n", prio);        
        printf("  |     |        |-[Name]: %s\n", name);
        printf("  |     |        |-[Status]: ");
        switch (status) {
        case SPU_THREAD_GROUP_STATUS_NOT_INITIALIZED:
                printf("NOT_INITIALIZED\n");
                break;
        case SPU_THREAD_GROUP_STATUS_INITIALIZED:
                printf("INITIALIZED\n");
                break;
        case SPU_THREAD_GROUP_STATUS_READY:
                printf("READY\n");
                break;
        case SPU_THREAD_GROUP_STATUS_WAITING:
                printf("WAITING\n");
                break;
        case SPU_THREAD_GROUP_STATUS_SUSPENDED:
                printf("SUSPENDED\n");
                break;
        case SPU_THREAD_GROUP_STATUS_WAITING_AND_SUSPENDED:
                printf("WAITING AND SUSPENDED\n");
                break;
        case SPU_THREAD_GROUP_STATUS_RUNNING:
                printf("RUNNING\n");
                break;
        case SPU_THREAD_GROUP_STATUS_STOPPED:
                printf("STOPPED\n");
                break;
        case SPU_THREAD_GROUP_STATUS_UNKNOWN:
                printf("UNKNOWN\n");
                break;
        default:
                printf("???????\n");
                break;
        }

        /* Print spu thread information */
        ret = print_spu_thread_ids(id);
        if (ret != CELL_OK) {
                return ret;
        }
        
        return CELL_OK;

}

int print_spu_thread_ids(sys_spu_thread_group_t id)
{
        int ret;
        sys_spu_thread_t ids[MAX_SPU_THREAD_NUM];
        uint64_t ids_num = MAX_SPU_THREAD_NUM;	// set allocated buffer size
        uint64_t all_ids_num;

        /*
         * Get a list of SPU thread IDs in the target SPU thread group
         *
         */
        
        ret = sys_dbg_get_spu_thread_ids(id, ids, &ids_num, &all_ids_num);
        if (ret != CELL_OK) {
                return ret;
        }


        /*
         * Output SPU thread ID list
         *
         */
        
        printf("  |     |        +-[SPU Thread]: %llu \n", all_ids_num);

        for (int i=0; i<(int)ids_num; i++) {
          
                printf("  |     |               |\n");
                printf("  |     |               +-[%d]: 0x%x\n", i, ids[i]);

                ret = print_spu_thread_info(ids[i]);
                if (ret != CELL_OK) {
                        return ret;
                }
        }

        return CELL_OK;
}


int print_spu_thread_info(sys_spu_thread_t id)
{
        int ret;

        /*
         * Get a name of the target SPU thread
         *
         */
        
        char name[MAX_THREAD_NAME];

        ret = sys_dbg_get_spu_thread_name(id, name);
        if (ret != CELL_OK) {
                return ret;
        }


        /*
         * Print SPU thread information to TTY
         *
         */

        printf("  |     |               |         |-[Name]: %s\n", name);

        return CELL_OK;

}


int print_sync_ids(uint32_t type)
{
        int ret;	// Store return code
        size_t size;
        size_t set_size;


        /*
         * Get a number of specified object
         *
         */

        ret = sys_process_get_number_of_object(type, &size);
        if (ret != CELL_OK) {
                return ret;
                
        }

        
        /*
         * Allocate memory for storing a list of specified object ID
         * 
         */
        
        uint32_t* ids = (uint32_t *)(malloc(sizeof(uint32_t) * size));
        if (ids == NULL) {
                return -1;	// memory allocation will be failed.
        }

        
        /*
         * Get a list of specified object ID
         *
         */
        
        ret = sys_process_get_id(type, ids, size, &set_size);
        if (ret != CELL_OK) {
                free(ids);
                return ret;
        }
        
        switch (type) {
        case SYS_MUTEX_OBJECT:
                printf("  +-[Mutex]: %d\n", size);
                break;
        case SYS_COND_OBJECT:
                printf("  +-[Condition variable]: %d\n", size);
                break;
        case SYS_LWCOND_OBJECT:
                printf("  +-[Lightweight condition variable]: %d\n", size);
                break;
        case SYS_RWLOCK_OBJECT:
                printf("  +-[Reader/Writer lock]: %d\n", size);
                break;
        case SYS_EVENT_QUEUE_OBJECT:
                printf("  +-[Event queue]: %d\n", size);
                break;
        case SYS_LWMUTEX_OBJECT:
                printf("  +-[Lwmutex]: %d\n", size);
                break;
        case SYS_SEMAPHORE_OBJECT:
                printf("  +-[Semaphore]: %d\n", size);
                break;
        case SYS_EVENT_FLAG_OBJECT:
                printf("  +-[Event flag]: %d\n", size);
                break;
        default:
                printf("  +-[????????]: %d\n", size);
                break;
        }


        for (int i=0; i<(int)set_size; i++) {

                printf("  |     |\n");
                printf("  |     +-[%d]: 0x%x \n", i, ids[i]);

                ret = print_sync_info(type, ids[i]);
                if (ret != CELL_OK) {
                        free(ids);
                        return ret;
                }
        }
        

        /*
         * free allocated buffer
         */
        
        free(ids);

        return CELL_OK;
}



int print_sync_info(uint32_t type, uint32_t id)
{
        int ret = CELL_OK;

        switch (type) {
        case SYS_MUTEX_OBJECT:
                ret = print_mutex_info(id);
                break;
        case SYS_COND_OBJECT:
                ret = print_cond_info(id);
                break;
        case SYS_LWCOND_OBJECT:
                ret = print_lwcond_info(id);
                break;
        case SYS_RWLOCK_OBJECT:
                ret = print_rwlock_info(id);
                break;
        case SYS_EVENT_QUEUE_OBJECT:
                ret = print_event_queue_info(id);
                break;
        case SYS_LWMUTEX_OBJECT:
                ret = print_lwmutex_info(id);
                break;
        case SYS_SEMAPHORE_OBJECT:
                ret = print_semaphore_info(id);
                break;
        case SYS_EVENT_FLAG_OBJECT:
                ret = print_event_flag_info(id);
                break;
        default:
                ret = -1;
                break;
        }

        return ret;
}




int print_mutex_info(sys_mutex_t id)
{
        int ret;
        sys_dbg_mutex_information_t info;

        /*
         * Get a number of waiting PPU thread IDs at target mutex object.
         *
         */

        memset(&info, 0, sizeof(info));
        info.wait_threads_num = 0;
        info.wait_id_list = NULL;
        info.wait_all_threads_num = 0;

        ret = sys_dbg_get_mutex_information(id, &info);
        if (ret != CELL_OK) {
                return ret;
        }

        uint32_t wait_threads_num = info.wait_all_threads_num;

        /*
         * Allocate memory for storing a list of waiting PPU thread IDs
         *
         */
        
        sys_ppu_thread_t* wait_id_list =
                (sys_ppu_thread_t *)(malloc(sizeof(sys_ppu_thread_t) * wait_threads_num));
        if (wait_id_list == NULL) {
                return ret;
        }
        

        /* Re-initialize */
        memset(&info, 0, sizeof(info));
        info.wait_threads_num = wait_threads_num; 	// set allocated buffer size
        info.wait_id_list = wait_id_list;		// set a pointer 


        /*
         * Get a mutex information
         *
         */
        
        ret = sys_dbg_get_mutex_information(id, &info);
        if (ret != CELL_OK) {
                free(wait_id_list);
                return ret;
        }


        /*
         * Print some information
         *
         */
        
        printf("  |     |       |-[OwnerPPUThreadID]: ");
        if (info.owner == 0) {
                printf(" None\n");
        }
        else {
                printf("0x%llx\n", info.owner);
        }

        printf("  |     |       |-[RecursLockCounter]: %d \n", info.lock_counter);
        printf("  |     |       |-[CondRefCounter]: %d \n", info.cond_ref_counter);
        printf("  |     |       +-[Attribute]\n");
        printf("  |     |       |       |-[Protocol]: 0x%x \n", info.attr.attr_protocol);
        printf("  |     |       |       |-[Recursive]: 0x%x \n", info.attr.attr_recursive);
        printf("  |     |       |       |-[Name]: %s \n", info.attr.name);
        printf("  |     |       +-[WaitingThreadIDList]: %d \n", info.wait_all_threads_num);

        for (int i=0; i<(int)info.wait_threads_num; i++) {
                printf("  |     |               |- [%d]: 0x%llx\n", i, wait_id_list[i]);                
        }


        /* free memory space */
        free(wait_id_list);
        

        return CELL_OK;
}

int print_lwmutex_info(sys_lwmutex_pseudo_id_t id)
{

        int ret;
        sys_dbg_lwmutex_information_t info;

        /*
         * Get a number of waiting PPU thread IDs at target lwmutex object.
         *
         */

        memset(&info, 0, sizeof(info));
        info.wait_threads_num = 0;
        info.wait_id_list = NULL;
        info.wait_all_threads_num = 0;

        ret = sys_dbg_get_lwmutex_information(id, &info);
        if (ret != CELL_OK) {
                return ret;
        }

        uint32_t wait_threads_num = info.wait_all_threads_num;


        /*
         * Allocate memory for storing a list of waiting PPU thread IDs
         *
         */
        
        sys_ppu_thread_t* wait_id_list =
                (sys_ppu_thread_t *)(malloc(sizeof(sys_ppu_thread_t) * wait_threads_num));
        if (wait_id_list == NULL) {
                return ret;
        }
        

        /* Re-initialize */
        memset(&info, 0, sizeof(info));
        info.wait_threads_num = wait_threads_num; 	// set allocated buffer size
        info.wait_id_list = wait_id_list;		// set a pointer 


        /*
         * Get a lwmutex information
         *
         */
        
        ret = sys_dbg_get_lwmutex_information(id, &info);
        if (ret != CELL_OK) {
                free(wait_id_list);
                return ret;
        }


        /*
         * Print some information
         *
         */
        
        printf("  |     |       |-[OwnerPPUThreadID]: ");
        if (info.owner == 0 ||
            info.owner == 0xffffffff ||
            info.owner == 0xfffffffe ||
            info.owner == 0xfffffffd) {
                printf(" None\n");
        }
        else {
                printf("0x%llx\n", info.owner);
        }

        printf("  |     |       |-[RecursLockCounter]: %d \n", info.lock_counter);
        printf("  |     |       +-[Attribute]\n");
        printf("  |     |       |       |-[Protocol]: 0x%x \n", info.attr.attr_protocol);
        printf("  |     |       |       |-[Recursive]: 0x%x \n", info.attr.attr_recursive);
        printf("  |     |       |       |-[Name]: %s \n", info.attr.name);
        printf("  |     |       +-[WaitingThreadIDList]: %d \n", info.wait_all_threads_num);

        for (int i=0; i<(int)info.wait_threads_num; i++) {
                printf("  |     |               |- [%d]: 0x%llx\n", i, wait_id_list[i]);
        }

        /* free memory space */
        free(wait_id_list);

        return CELL_OK;

}


int print_cond_info(sys_cond_t id)
{
        int ret;
        sys_dbg_cond_information_t info;
        
        /*
         * Get a number of waiting PPU thread IDs at target cond object.
         *
         */

        memset(&info, 0, sizeof(info));
        info.wait_threads_num = 0;
        info.wait_id_list = NULL;
        info.wait_all_threads_num = 0;

        ret = sys_dbg_get_cond_information(id, &info);
        if (ret != CELL_OK) {
                return ret;
        }

        uint32_t wait_threads_num = info.wait_all_threads_num;


        /*
         * Allocate memory for storing a list of waiting PPU thread IDs
         *
         */
        
        sys_ppu_thread_t* wait_id_list =
                (sys_ppu_thread_t *)(malloc(sizeof(sys_ppu_thread_t) * wait_threads_num));
        if (wait_id_list == NULL) {
                return ret;
        }


        /* Re-initialize */
        memset(&info, 0, sizeof(info));
        info.wait_threads_num = wait_threads_num; 	// set allocated buffer size
        info.wait_id_list = wait_id_list;		// set a pointer


        /*
         * Get a cond information
         *
         */
        
        ret = sys_dbg_get_cond_information(id, &info);
        if (ret != CELL_OK) {
                free(wait_id_list);
                return ret;
        }


        /*
         * Print some information
         *
         */
        
        printf("  |     |       |-[MutexID]: 0x%x \n", info.mutex_id);
        printf("  |     |       +-[Attribute]\n");
        printf("  |     |       |       |-[Name]: %s \n", info.attr.name);
        printf("  |     |       +-[WaitingThreadIDList]: %d \n", info.wait_all_threads_num);        

        for (int i=0; i<(int)info.wait_threads_num; i++) {
                printf("  |     |               |- [%d]: 0x%llx \n", i, wait_id_list[i]);

        }


        /* free memory space */
        free(wait_id_list);

        return CELL_OK;
}


int print_lwcond_info(sys_lwcond_pseudo_id_t id)
{
        int ret;
        sys_dbg_lwcond_information_t info;
        
        /*
         * Get a number of waiting PPU thread IDs at target cond object.
         *
         */

        memset(&info, 0, sizeof(info));
        info.wait_threads_num = 0;
        info.wait_id_list = NULL;
        info.wait_all_threads_num = 0;

        ret = sys_dbg_get_lwcond_information(id, &info);
        if (ret != CELL_OK) {
                return ret;
        }

        uint32_t wait_threads_num = info.wait_all_threads_num;


        /*
         * Allocate memory for storing a list of waiting PPU thread IDs
         *
         */
        
        sys_ppu_thread_t* wait_id_list =
                (sys_ppu_thread_t *)(malloc(sizeof(sys_ppu_thread_t) * wait_threads_num));
        if (wait_id_list == NULL) {
                return ret;
        }

        /* Re-initialize */
        memset(&info, 0, sizeof(info));
        info.wait_threads_num = wait_threads_num; 	// set allocated buffer size
        info.wait_id_list = wait_id_list;		// set a pointer


        /*
         * Get a lwcond information
         *
         */
        
        ret = sys_dbg_get_lwcond_information(id, &info);
        if (ret != CELL_OK) {
                free(wait_id_list);
                return ret;
        }


        /*
         * Print some information
         *
         */
        
        printf("  |     |       |-[LwutexID]: 0x%x \n", info.lwmutex->sleep_queue);
        printf("  |     |       +-[Attribute]\n");
        printf("  |     |       |       |-[Name]: %s \n", info.attr.name);
        printf("  |     |       +-[WaitingThreadIDList]: %d \n", info.wait_all_threads_num);        

        for (int i=0; i<(int)info.wait_threads_num; i++) {
                printf("  |     |               |- [%d]: 0x%llx \n", i, wait_id_list[i]);

        }


        /* free memory space */
        free(wait_id_list);

        return CELL_OK;
}


int print_event_queue_info(sys_event_queue_t id)

{
        int ret;
        sys_dbg_event_queue_information_t info;

        /*
         * Get a number of waiting PPU thread IDs at target event queue object.
         *
         */

        memset(&info, 0, sizeof(info));
        info.wait_threads_num = 0;
        info.wait_id_list = NULL;
        info.wait_all_threads_num = 0;
        
        info.readable_equeue_num = 0;
        info.equeue_list = NULL;
        info.readable_all_equeue_num = 0;
        

        ret = sys_dbg_get_event_queue_information(id, &info);
        if (ret != CELL_OK) {
                return ret;
        }

        uint32_t wait_threads_num = info.wait_all_threads_num;
        uint32_t readable_equeue_num = info.readable_all_equeue_num;

        
        /*
         * Allocate memory for storing a list of waiting PPU thread IDs
         *
         */
        
        sys_ppu_thread_t* wait_id_list =
                (sys_ppu_thread_t *)(malloc(sizeof(sys_ppu_thread_t) * wait_threads_num));
        if (wait_id_list == NULL) {
                return ret;
        }

        
        /*
         * Allocate memory for storing a list of readable events
         *
         */

        sys_event_t* equeue_list =
                (sys_event_t *)(malloc(sizeof(sys_event_t) * readable_equeue_num));
        if (equeue_list == NULL) {
                free(wait_id_list);
                return ret;

        }


        /* Re-initialize */
        memset(&info, 0, sizeof(info));
        info.wait_threads_num = wait_threads_num; 	// set allocated buffer size
        info.wait_id_list = wait_id_list;		// set a pointer 

        info.readable_equeue_num = readable_equeue_num;	// set allocated buffer size
        info.equeue_list = equeue_list;			// set a pointer 

        
        /*
         * Get a event queue information
         *
         */
        
        ret = sys_dbg_get_event_queue_information(id, &info);
        if (ret != CELL_OK) {
                free(wait_id_list);
                free(equeue_list);
                return ret;
        }


        /*
         * Print some information
         *
         */

        printf("  |     |       |-[Key]: 0x%llx \n", info.event_queue_key);
        printf("  |     |       |-[Depth]: %d \n", info.queue_size);
        printf("  |     |       +-[Attribute]\n");
        printf("  |     |       |       |-[Protocol]: 0x%x \n", info.attr.attr_protocol);
        printf("  |     |       |       |-[Type]: 0x%x \n", info.attr.type);
        printf("  |     |       |       |-[Name]: %s \n", info.attr.name);        
        printf("  |     |       +-[WaitingThreadIDList]: %d \n", info.wait_all_threads_num);

        for (int i=0; i<(int)info.wait_threads_num; i++) {
                printf("  |     |       |       |- [%d]: 0x%llx\n", i, wait_id_list[i]);
        }


        printf("  |     |       +-[ReadableEventList]: %d \n", info.readable_all_equeue_num);


        for (int i=0; i<(int)info.readable_equeue_num; i++) {
                printf("  |     |               |- [%d]: source: 0x%llx data1: 0x%llx data2: 0x%llx data3: 0x%llx \n",
                       i, equeue_list[i].source, equeue_list[i].data1, equeue_list[i].data2, equeue_list[i].data3);                

        }

        /* free memory space */
        free(wait_id_list);
        free(equeue_list);

        return CELL_OK;

}

int print_rwlock_info(sys_rwlock_t id)

{

        int ret;
        sys_dbg_rwlock_information_t info;
        
        /*
         * Get a number of waiting PPU thread IDs for reader and writer at target rwlock object.
         *
         */

        memset(&info, 0, sizeof(info));
        info.r_wait_threads_num = 0;
        info.w_wait_threads_num = 0;
        
        info.r_wait_id_list = NULL;
        info.w_wait_id_list = NULL;
        
        info.r_wait_all_threads_num = 0;
        info.w_wait_all_threads_num = 0;

        ret = sys_dbg_get_rwlock_information(id, &info);
        if (ret != CELL_OK) {
                return ret;
        }

        uint32_t r_wait_threads_num = info.r_wait_all_threads_num;
        uint32_t w_wait_threads_num = info.w_wait_all_threads_num;

        /*
         * Allocate memory for storing a list of waiting PPU thread IDs
         *
         */

        /* for reader*/
        sys_ppu_thread_t* r_wait_id_list =
                (sys_ppu_thread_t *)(malloc(sizeof(sys_ppu_thread_t) * r_wait_threads_num));
        if (r_wait_id_list == NULL) {
                return ret;
        }

        /* for writer */
        sys_ppu_thread_t* w_wait_id_list =
                (sys_ppu_thread_t *)(malloc(sizeof(sys_ppu_thread_t) * w_wait_threads_num));
        if (w_wait_id_list == NULL) {
                free(r_wait_id_list);
                return ret;
        }
        

        /* Re-initialize */
        memset(&info, 0, sizeof(info));
        info.r_wait_threads_num = r_wait_threads_num; 	// set allocated buffer size
        info.r_wait_id_list = r_wait_id_list;		// set a pointer 
        info.w_wait_threads_num = w_wait_threads_num; 	// set allocated buffer size
        info.w_wait_id_list = w_wait_id_list;		// set a pointer 


        /*
         * Get a rwlock information
         *
         */
        
        ret = sys_dbg_get_rwlock_information(id, &info);
        if (ret != CELL_OK) {
                free(r_wait_id_list);
                free(w_wait_id_list);                
                return ret;
        }


        /*
         * Print some information
         *
         */
        
        printf("  |     |       |-[OwnerPPUThreadID]: ");
        if (info.owner == 0) {
                printf(" None\n");
        }
        else {
                printf("0x%llx\n", info.owner);
        }
        
        printf("  |     |       +-[Attribute]\n");
        printf("  |     |       |       |-[Protocol]: 0x%x \n", info.attr.attr_protocol);
        printf("  |     |       |       |-[Name]: %s \n", info.attr.name);

        printf("  |     |       +-[WaitingThreadIDList (R)]: %d \n", info.r_wait_all_threads_num);
        for (int i=0; i<(int)info.r_wait_threads_num; i++) {
                printf("  |     |       |       |- [%d]: 0x%llx\n", i, r_wait_id_list[i]);
        }

        printf("  |     |       +-[WaitingThreadIDList (W)]: %d \n", info.w_wait_all_threads_num);
        for (int i=0; i<(int)info.w_wait_threads_num; i++) {
                printf("  |     |               |- [%d]: 0x%llx\n", i, w_wait_id_list[i]);
        }



        /* free memory space */
        free(r_wait_id_list);
        free(w_wait_id_list);

        return CELL_OK;

}




int print_semaphore_info(sys_semaphore_t id)
{
        
        int ret;
        sys_dbg_semaphore_information_t info;

        /*
         * Get a number of waiting PPU thread IDs at target semaphore object.
         *
         */

        memset(&info, 0, sizeof(info));
        info.wait_threads_num = 0;
        info.wait_id_list = NULL;
        info.wait_all_threads_num = 0;

        ret = sys_dbg_get_semaphore_information(id, &info);
        if (ret != CELL_OK) {
                return ret;
        }

        uint32_t wait_threads_num = info.wait_all_threads_num;

        /*
         * Allocate memory for storing a list of waiting PPU thread IDs
         *
         */
        
        sys_ppu_thread_t* wait_id_list =
                (sys_ppu_thread_t *)(malloc(sizeof(sys_ppu_thread_t) * wait_threads_num));
        if (wait_id_list == NULL) {
                return ret;
        }
        

        /* Re-initialize */
        
        memset(&info, 0, sizeof(info));
        info.wait_threads_num = wait_threads_num; 	// set allocated buffer size
        info.wait_id_list = wait_id_list;		// set a pointer 


        /*
         * Get a semaphore information
         *
         */
        
        ret = sys_dbg_get_semaphore_information(id, &info);
        if (ret != CELL_OK) {
                free(wait_id_list);
                return ret;
        }

        
        /*
         * Print some information
         *
         */
        
        printf("  |     |       |-[Max value]: %d \n", info.max_val);
        printf("  |     |       |-[Current value]: %d \n", info.cur_val);
        printf("  |     |       +-[Attribute]\n");
        printf("  |     |       |       |-[Protocol]: 0x%x \n", info.attr.attr_protocol);
        printf("  |     |       |       |-[Name]: %s \n", info.attr.name);
        printf("  |     |       +-[WaitingThreadIDList]: %d \n", info.wait_all_threads_num);

        for (int i=0; i<(int)info.wait_threads_num; i++) {
                printf("  |     |               |- [%d]: 0x%llx\n", i, wait_id_list[i]);
        }


        /* free memory space */
        free(wait_id_list);

        return CELL_OK;

}


int print_event_flag_info(sys_event_flag_t id)
{
        int ret;
        sys_dbg_event_flag_information_t info;

        /*
         * Get a number of waiting PPU thread IDs at target event flag object.
         *
         */

        memset(&info, 0, sizeof(info));
        info.wait_threads_num = 0;
        info.wait_id_list = NULL;
        info.wait_info_list = NULL;
        info.wait_all_threads_num = 0;

        ret = sys_dbg_get_event_flag_information(id, &info);
        if (ret != CELL_OK) {
                return ret;
        }

        uint32_t wait_threads_num = info.wait_all_threads_num;

        /*
         * Allocate memory for storing a list of waiting PPU thread IDs
         *
         */
        
        sys_ppu_thread_t* wait_id_list =
                (sys_ppu_thread_t *)(malloc(sizeof(sys_ppu_thread_t) * wait_threads_num));
        if (wait_id_list == NULL) {
                return ret;
        }
        
        /*
         * Allocate memory for storing a list of waiting PPU thread Information
         *
         */
        
        sys_dbg_event_flag_wait_information_t* wait_info_list =
            (sys_dbg_event_flag_wait_information_t *)(malloc(sizeof(sys_dbg_event_flag_wait_information_t) * wait_threads_num));
        if (wait_info_list == NULL) {
                free(wait_id_list);
                return ret;
        }


        /* Re-initialize */
        memset(&info, 0, sizeof(info));
        info.wait_threads_num = wait_threads_num; 	// set allocated buffer size
        info.wait_id_list = wait_id_list;			// set a pointer 
        info.wait_info_list = wait_info_list;		// set a pointer 


        /*
         * Get a event flag information
         *
         */
        
        ret = sys_dbg_get_event_flag_information(id, &info);
        if (ret != CELL_OK) {
                free(wait_id_list);
                free(wait_info_list);
                return ret;
        }


        /*
         * Print some information
         *
         */
        
        printf("  |     |       +-[Attribute]\n");
        printf("  |     |       |       |-[Protocol]: 0x%x \n", info.attr.attr_protocol);
        printf("  |     |       |       |-[Type]: 0x%x \n", info.attr.type);
        printf("  |     |       |       |-[Name]: %s \n", info.attr.name);
        printf("  |     |       |-[Current Bit Pattern]: 0x%016llx \n", info.cur_bitptn);
        printf("  |     |       +-[WaitingThreadIDList]: %d \n", info.wait_all_threads_num);
        for (int i=0; i<(int)info.wait_threads_num; i++) {
                printf("  |     |       |       |- [%d]: 0x%llx\n", i, wait_id_list[i]);
        }
        printf("  |     |       +-[WaitingInformationList]: %d \n", info.wait_all_threads_num);
        for (int i=0; i<(int)info.wait_threads_num; i++) {
                printf("  |     |               |- [%d][Bit Pattern]: 0x%016llx\n", i, wait_info_list[i].bitptn);
                printf("  |     |               |- [%d][Mode]: 0x%08x\n", i, wait_info_list[i].mode);
        }


        /* free memory space */
        free(wait_id_list);
        free(wait_info_list);
        

        return CELL_OK;
}


