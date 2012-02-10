#ifndef _PPU_EXCEPTION_HANDLER_H_
#define _PPU_EXCEPTION_HANDLER_H_

#include <fenv.h>

void exception_trigger_thread(uint64_t fvalue_ptr);
void ppu_exception_handler(uint64_t exp_cause, sys_ppu_thread_t pu_thr_id, uint64_t dar);
void write_tty_and_anydevice(char *buf);
int init_exception_handler(void);
int enable_fp_exception(fexcept_t enables);
int disable_fp_exception(void);
int write_floating_point_exception_type(char *buf, sys_ppu_thread_t pu_thr_id);

#endif //_PPU_EXCEPTION_HANDLER_H_