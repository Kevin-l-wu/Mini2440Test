#ifndef COMMON_H_
#define COMMON_H_

//5,2 return 3
#define MAX_MAGNIFICATION(value, base) ((value)%(base) == 0 ? ((value)/(base)) : ((value)/(base) + 1))
void delay(int n);
int caculate_power(int value, int power);
int data_adjustment(unsigned* input);
void PrintAscii(unsigned char* buff, int length, int length_per_line);

double pow( double x, double y );

#endif