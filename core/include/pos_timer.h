/*
* @title Software Timer Management
* timer_manager.h
* @author Hamid Reza Mehrabian
* @version 1.0.0
*/
#ifndef _TIMER_MANAGER_H_
#define _TIMER_MANAGER_H_




PosStatusType pos_add_timer(uint32_t time,timer_id_t *id,_PID pid,uint8_t mode) ;
uint8_t pos_kill_timer(timer_id_t tid);
uint8_t pos_start_timer(timer_id_t tid);
uint8_t pos_resume_timer(timer_id_t tid);
uint8_t pos_stop_timer(timer_id_t tid);
void pos_init_timers(void);
void pos_kill_all_timers(void);

#endif