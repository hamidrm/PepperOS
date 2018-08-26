
#ifndef _POS_IPC_H
#define _POS_IPC_H
PosStatusType pos_ipc_init(void);
PosStatusType pos_send_message(_PID target,	pos_process_message_type message_type,pos_process_message_content message_content);
PosStatusType pos_get_message(void);
#endif
