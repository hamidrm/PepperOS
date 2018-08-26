#ifndef _POS_SCHEDULER_H_
#define _POS_SCHEDULER_H_


void pos_scheduler_init(void);
PosStatusType pos_schedule_init_task(task_start_handler_t start_handler,task_msg_proc_t proc_handler,uint8_t * stack,size_t stack_size,PosTaskPriority priority,_PID * pid);
void pos_task_finish(void);
extern inline PosSchedulerStatus pos_schedule_tasks(void);
void pos_scheduler_start(void);
_PID _pos_get_current_pid(void);
pos_task_type * pos_get_current_task(void);
void pos_kill_task(_PID pid);
pos_task_type * pos_get_task_by_pid(_PID pid);
void pos_get_total_elapsed_time(uint64_t *time);
void pos_get_task_elapsed_time(_PID pid,uint32_t *time);
void pos_force_cs_by_pid(_PID pid);
void pos_force_cs(void);
extern inline void pos_sleep_cpu(void);
extern inline void pos_set_current_mode(PosOsMode mode);

#endif

