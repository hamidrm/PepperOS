
#ifndef _POS_DELAY_H
#define _POS_DELAY_H


PosStatusType pos_delay_init(void);
PosStatusType pos_delay_add(_PID pid,uint32_t time_ms,PosTaskStatus status);
PosStatusType pos_delay_remove(_PID pid);
void pos_delay_tick(void);
#endif