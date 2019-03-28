#ifndef COMMON_H_
#define COMMON_H_

#define MAX_COMMAND_NUMBER 10
#define MAX_COMMAND_LENGTH 10
//5,2 return 3
#define MAX_MAGNIFICATION(value, base) ((value)%(base) == 0 ? ((value)/(base)) : ((value)/(base) + 1))
void delay(int n);
int caculate_power(int value, int power);
int data_adjustment(unsigned* input);

#endif