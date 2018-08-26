#ifndef _QUEUE_H_
#define _QUEUE_H_


PosStatusType pos_queue_enq(pos_queue_t * queue,void * data,size_t len);
PosStatusType pos_queue_deq(pos_queue_t * queue,void ** data,size_t * len);
PosStatusType pos_queue_count(pos_queue_t * queue,size_t * cnt);
PosStatusType pos_create_queue(pos_queue_t ** queue);

#endif