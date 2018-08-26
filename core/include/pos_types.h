#ifndef _POS_TYPES_H_
#define _POS_TYPES_H_


#include "pos_config.h"


#define MHz     *1000000
#define KHz     *1000

#define MB     KB * 1024
#define KB     *1024

#define TRUE    1
#define FALSE   0
#define POS_PID_BASE 1;
#define NULL_PTR    0
#define POS_KERNEL_MESSAGES_OFFSET      0x80000000
#define POS_KERNEL_PID    0
#define POS_MUTEX_INVALID               0xFFFFFFFF
#define POS_TASK_HIGH_PRIORITY_QUANTUM  (MEAN_QUANTUM_LENGTH * 1.25)
#define POS_TASK_NORMAL_PRIORITY_QUANTUM  (MEAN_QUANTUM_LENGTH * 1.00)
#define POS_TASK_LOW_PRIORITY_QUANTUM  (MEAN_QUANTUM_LENGTH * 0.75)
#define POS_ENABLE      1
#define POS_DISABLE     0
#define POS_ASSERT(cond)        if(cond) return POS_ERROR
#define POS_ASSERT_SEND_ERROR(cond,error)        if(cond){\
                                                    pos_error(error);\
                                                    return POS_ERROR;\
                                                 }
#define POS_ERROR_MEM_OFFSET    0x10000000
#define POS_ERROR_SYS_OFFSET    0x20000000
#define POS_ERROR_POS_OFFSET    0x30000000


typedef uint16_t _PID;
typedef uint32_t pos_process_message_type;
typedef uint32_t pos_process_message_content;
typedef uint32_t time_value;
typedef uint32_t timer_id_t;
typedef uint32_t handle;
typedef uint32_t size_t;
typedef uint32_t pos_t;
typedef uint32_t pos_mutex_id_t;
typedef void(*task_start_handler_t)(_PID);
typedef void(*task_msg_proc_t)(pos_process_message_type,pos_process_message_content,_PID src);
typedef struct _pos_scheduler_t pos_scheduler_t;
typedef struct pos_timer_t _pos_timer_t;
typedef struct _pos_semaphore_queue_t pos_semaphore_queue_t;
typedef struct pos_delay_t _pos_delay_t;
typedef struct _pos_process_message_queue_t pos_process_message_queue_t;
typedef struct _pos_queue_ll_t pos_queue_ll_t;
typedef enum
{
  POS_KERNEL_ACTIVE,
  POS_TASKS_ACTIVE,
  POS_SYSTEM_SLEEP,
} PosOsMode;


typedef enum 
{
  POS_TASK_STARTUP = POS_KERNEL_MESSAGES_OFFSET,
  POS_TASK_SUICIDE,
  POS_TASK_TIMER ,
  POS_TASK_CONSOLE_RX ,
} PosKernelMessagesType;
typedef enum 
{
  POS_OK       = 1<<0,
  POS_ERROR    = 1<<1,
  POS_BUSY     = 1<<2,
  POS_TIMEOUT  = 1<<3,
} PosStatusType;

typedef enum
{
  POS_TASK_HIGH_PRIORITY,
  POS_TASK_NORMAL_PRIORITY,
  POS_TASK_LOW_PRIORITY,
} PosTaskPriority;

typedef enum
{
  /* Memmory Errors */
  POS_MEM_LOW_DST_SIZE  = POS_ERROR_MEM_OFFSET | 1,
  POS_MEM_LOW_SRC_SIZE  = POS_ERROR_MEM_OFFSET | 2,
  POS_MEM_LOW_SPACE  = POS_ERROR_MEM_OFFSET | 3,
  POS_MEM_INV_ADDR  = POS_ERROR_MEM_OFFSET | 4,
  POS_MEM_UNALLOCATED = POS_ERROR_MEM_OFFSET | 5,
  
  /* System Errors */
  POS_SYS_INVALID_CLK  = POS_ERROR_SYS_OFFSET | 1,
  
  /* PepperOS Errors */
  POS_OS_UNEXPECTED_ERROR = POS_ERROR_POS_OFFSET | 1,
  POS_OS_PID_NOT_FOUND = POS_ERROR_POS_OFFSET | 2, 
  POS_OS_RESOURCE_NOT_FOUND = POS_ERROR_POS_OFFSET | 3, 
  POS_OS_QUEUE_EMPTY = POS_ERROR_POS_OFFSET | 4,
  POS_OS_QUEUE_FULL = POS_ERROR_POS_OFFSET | 5,
  POS_OS_STACKOVERFLOW =  POS_ERROR_POS_OFFSET | 6,
} PosErrorType;

typedef enum
{
  POS_SYS_CALL_NOP=0x00,
  POS_SYS_CALL_SLEEP_CPU=0x01,
} PosSysCallType;

typedef enum     {
  POS_TASK_STATUS_ACTIVE,
  POS_TASK_STATUS_IDLE,
  POS_TASK_STATUS_SEM_WAITING,
  POS_TASK_STATUS_SEM_WAITING_UNTIL,
  POS_TASK_STATUS_DELAY,
  POS_TASK_STATUS_DELAY_TIMEOUT, 
  POS_TASK_STATUS_SUSPENDED,
} PosTaskStatus;

typedef enum  {
  POS_SCHEDULER_OK,
  POS_SCHEDULER_NO_TASK_READY,
  POS_SCHEDULER_OS_NOT_STARTED,
} PosSchedulerStatus;

typedef enum  {
  TIMER_MODE_ONE_SHOT,
  TIMER_MODE_PERIODICALLY
} PosTimerType;

typedef enum  {
  TIMER_STATUS_STOP,
  TIMER_STATUS_RUNNING
}PosTimerStatus;


typedef enum {
  POS_QUEUE_EMPTY,
  POS_QUEUE_NOT_EMPTY,
  POS_QUEUE_FULL,
}PosQueueSTatus;

typedef struct {
  _PID          pid;
  union{
    uint8_t     dummy[2];
    uint16_t    stack_size;
  };
  void *        sp;
#if CHECK_STACK_OVER_FLOW == TRUE
  void *        orginal_sp;
#endif
  PosTaskStatus    status;
  PosTaskPriority  priority;
  uint32_t      time_elapsed_active;
  task_start_handler_t start_fun;
  task_msg_proc_t          proc_fun;
}pos_task_type;

struct _pos_semaphore_queue_t{
	_PID	pid;
	pos_semaphore_queue_t * next;
} ;


typedef struct{
	int16_t cnt;
        _PID released_task;
	pos_semaphore_queue_t * q;
}pos_semaphore_t;


struct _pos_scheduler_t {
  pos_task_type task;
  pos_scheduler_t * next_task;
};


struct pos_timer_t {
        uint32_t		otime;
	uint32_t		time;
	uint32_t		timerId;
        uint8_t                 mode;
        uint32_t		complete_time;
        uint8_t                 status;
        _pos_timer_t *        next;
        _PID            pid;
        uint8_t         dummy[2];
};


struct pos_delay_t{
  _PID pid;
  int32_t elapsed_time;
  _pos_delay_t * next;
  uint16_t dummy;
};



typedef struct{
	_PID src;
	pos_process_message_type message_type;
        pos_process_message_content message_content;
} pos_process_message_t;

typedef struct{
  uint64_t total_elapsed_time;
  uint64_t sleep_elapsed_time;
  uint64_t kernel_elapsed_time;
} pos_elapsed_time_t;

struct _pos_queue_ll_t{
	void * elem_data;
        size_t elem_len;
	pos_queue_ll_t * next;
} ;

typedef struct {
  pos_queue_ll_t * head;
  pos_semaphore_t * sem;
} pos_queue_t;


struct _pos_process_message_queue_t{
	pos_process_message_t element;
	pos_process_message_queue_t * next;
} ;
#endif