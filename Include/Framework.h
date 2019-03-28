#ifndef FRAMEWORK_H_
#define FRAMEWORK_H_


typedef struct _diags_commonds
{
	char* cmmond_string;
	char* message;
	MINI2440_STATUS(*function)(int argc, char(*argv)[MAX_COMMAND_LENGTH]);
} Diags_Commonds;

#endif