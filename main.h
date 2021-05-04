#ifndef MAIN_H
#define MAIN_H

static enum ALARM_STATE{LOCKED=0x00, UNLOCKED=0x01, TRIGGERED=0x10, ALARM=0x11};

void USART1_Init(void);

#endif