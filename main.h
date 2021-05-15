#include <stdio.h>
#include <stdint.h>
#ifndef MAIN_H
#define MAIN_H

enum ALARM_STATE{LOCKED=0, UNLOCKED=1, TRIGGERED=2, ALARM=3};
static enum ALARM_STATE alarm_state = UNLOCKED;

void USART1_Init(void);
void wait(uint64_t iters);
void countdownThread(void);


#endif