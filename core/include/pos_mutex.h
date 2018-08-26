
#ifndef _POS_MUTEX_H
#define _POS_MUTEX_H

void pos_mutex_init(void);
void pos_mutex_create(pos_mutex_id_t * mutex);
void pos_mutex_free(pos_mutex_id_t * mutex);
void pos_mutex_acquire(pos_mutex_id_t * mutex);
void pos_mutex_release(pos_mutex_id_t * mutex);

#endif
