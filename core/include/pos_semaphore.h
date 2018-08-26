
#ifndef _POS_SEMAPHORE_H
#define _POS_SEMAPHORE_H


PosStatusType pos_semaphore_init(pos_semaphore_t *s,int16_t iv);
PosStatusType pos_semaphore_wait(pos_semaphore_t *s);
PosStatusType pos_semaphore_wait_until(pos_semaphore_t *s,uint32_t time_ms);
PosStatusType pos_semaphore_signal(pos_semaphore_t *s,_PID * pid);
#endif
