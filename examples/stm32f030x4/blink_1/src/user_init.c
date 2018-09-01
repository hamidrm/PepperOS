#include "pepper_os.h"

#include "task1.h"
#include "task2.h"

uint8_t s1[512];
uint8_t s2[512];

void main(void){
  pos_init();
  pos_create_task(Task1_Main,Task1_Proc,s1,512,POS_TASK_HIGH_PRIORITY);
  pos_create_task(Task2_Main,Task2_Proc,s2,512,POS_TASK_HIGH_PRIORITY);
  pos_scheduler_start(); 
}