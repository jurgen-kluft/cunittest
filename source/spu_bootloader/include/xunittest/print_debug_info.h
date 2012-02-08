#ifndef _PRINT_DEBUG_INFO_H_
#define _PRINT_DEBUG_INFO_H_

#include <sys/types.h>
#include <sys/ppu_thread.h>
#include <sys/spu_thread.h>
#include <sys/spu_thread_group.h>
#include <sys/synchronization.h>


#define MAX_THREAD_NAME		128
#define MAX_SPU_THREAD_NUM	6


/*
 * Call tree
 * 
 * print_process_info()
 *   |
 *   +- print_ppu_thread_ids()
 *   |     |
 *   |     +- print_ppu_thread_info()
 *   |     |
 *   |     |
 *   +- print_spu_thread_group_ids()
 *   |     |
 *   |     +- print_spu_thread_group_info()
 *   |              |
 *   |              +- print_spu_thread_ids()
 *   |                      |
 *   |                      +- print_spu_thread_info()
 *   |     
 *   +- print_sync_ids()
 *   |     |
 *   |     +- print_sync_info()
 *   |           | 
 *   |           +- print_{mutex/lwmutex/cond/event_queue/rwlock/semaphore/event_flag}_info()
 * 
 */

/* for all debug information in my process */
int print_process_info(void);

/* for PPU thread */
int print_ppu_thread_ids(void);
int print_ppu_thread_info(sys_ppu_thread_t id);

/* for SPU thread group */
int print_spu_thread_group_ids(void);
int print_spu_thread_group_info(sys_spu_thread_group_t id);

/* for SPU thread */
int print_spu_thread_ids(sys_spu_thread_group_t id);
int print_spu_thread_info(sys_spu_thread_t id);


/* for sychronization primitives */
int print_sync_ids(uint32_t type);
int print_sync_info(uint32_t type, uint32_t id);


/* for mutex */
int print_mutex_info(sys_mutex_t id);

/* for lwmutex */
int print_lwmutex_info(sys_lwmutex_pseudo_id_t id);

/* for condition variable */
int print_cond_info(sys_cond_t cond_id);

/* for lightweight condition variable */
int print_lwcond_info(sys_lwcond_pseudo_id_t id);

/* for event queue */
int print_event_queue_info(sys_event_queue_t id);

/* for reader/writer lock */
int print_rwlock_info(sys_rwlock_t id);

/* for semaphore*/
int print_semaphore_info(sys_semaphore_t semaphore_id);

/* for event flag */
int print_event_flag_info(sys_event_flag_t id);

#endif //_PRINT_DEBUG_INFO_H_
