#ifndef USR_COMMON_H
#define USR_COMMON_H
/**
  *This file is used to define the common algorithms,such as the string algorithm
  * and data structure
  *
 * */
#include "stm32f10x.h"
/***************Variables*************/
typedef enum {TRUE=1,FALSE=0} BOOL;

/***********Functions****************/

void float_to_char_V2(float src, uint8_t *buffer,uint8_t buffersize);
void float_to_char(float src, uint8_t *buffer,int buffersize);
void int_to_char(uint16_t src, uint8_t *buffer,int buffersize);
BOOL Is_In_Str_Moore(const char key[],uint8_t *src);
#endif // USR_COMMON_H

