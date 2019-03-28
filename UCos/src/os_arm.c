
#include "ucos_ii.h"


void DumpAllOSTCB()
{
	OS_TCB* OSTCBPtr = OSTCBList;
	int i = 0;
	
	while(OSTCBPtr != 0)
	{
		print_string("------------------------------------------\n");
		printf_string("OSTCBPtr->OSTCBPrio = 0x%x\n", OSTCBPtr->OSTCBPrio);
		printf_string("OSTCBPtr->OSTCBStkPtr = 0x%x\n", OSTCBPtr->OSTCBStkPtr);
		
		for(i = 0; i < 17; i++)
		{
			printf_string("OSTCBPtr->OSTCBStkPtr[%d] = 0x%x\n", i, OSTCBPtr->OSTCBStkPtr[i]);
		}
		
		OSTCBPtr = OSTCBPtr->OSTCBNext;
		print_string("------------------------------------------\n");
	}
}

void OSCPURestoreSR(OS_CPU_SR cpu_sr)
{
	__asm__ __volatile__(	
	"ldr r0, %0\n"
	"msr cpsr, r0\n"
	:
	:"m"(cpu_sr)
	);
	
	return; 
}

OS_CPU_SR  OSCPUSaveSR(void)
{
	__asm__ __volatile__(	
	"mrs r0, cpsr\n"
	"orr r1, r0, #0xC0\n"
	"msr cpsr, r1\n"
	"mrs r1, cpsr\n"
	"and r1, r1, #0xC0\n"
	"cmp r1, #0xC0\n"
	"bne OSCPUSaveSR\n"
	:
	:
	);
}
/*
void OSCtxSw(void)
{
//	OS_STK spValue = 0;
	
	//1. Save the break pointer to task's stack.
	
	//2. Save the general register to task's stack
	
	//3. Save the stack pointer to the OS_TCB->OSTCBStkPtr
	
	//4. Get the task's OS_TCB which will be exec
	
	//5. Get the stack pointer from the OS_TCB and set the stack register
	
	//6. Get the content of general register which store in the stack and set the register
	
	//7. Get the new pointer and set to the PC register
	
	print_string("OSCtxSw start----\n");
	printf_string("OSPrioCur = %d\n", OSPrioCur);
	printf_string("OSPrioHighRdy = %d\n", OSPrioHighRdy);
	
	__asm__ __volatile__(	
	//1. Save the break pointer to task's stack.
	"stmfd sp!, {lr}\n"
	
	//2. Save the general register to task's stack
	"stmfd sp!, {r0-r12, lr}\n"
	"mrs r0, cpsr\n"
	"stmfd sp!, {r0}\n"
	
	//3. Save the stack pointer to the OS_TCB->OSTCBStkPtr 
	// OSTCBCur->OSTCBStkPtr = SP
	"ldr r0, =OSTCBCur\n"
	"ldr r0, [r0]\n"
	"str sp, [r0]\n"
	
	//4. Get the task's OS_TCB which will be exec
	// OSTCBCur = OSTCBHighRdy;
	"ldr r0, =OSTCBHighRdy\n"
	"ldr r1, =OSTCBCur\n"
	"ldr r0, [r0]\n"
	"str r0, [r1]\n"
	
	// OSPrioCur = OSPrioHighRdy;
	"ldr r0, =OSPrioHighRdy\n"
	"ldr r1, =OSPrioCur\n"
	"ldrb r0, [r0]\n"
	"strb r0, [r1]\n"
	
	//5. Get the stack pointer from the OS_TCB and set the stack register
	// SP = OSTCBHighRdy->OSTCBStkPtr;
	"ldr r0, = OSTCBCur\n"
	"ldr r0, [r0]\n"
	"ldr sp, [r0]\n"
	
	//6. Get the content of general register which store in the stack and set the register
	//7. Get the new pointer and set to the PC register
	"ldmfd sp!,{r0}\n"
	"msr SPSR_cxsf, r0\n"
	"ldmfd sp!, {r0-r12, lr, pc}^\n"
	);
	
	return ;
}
*/
/*
void OSIntCtxSw(void)
{
	print_string("OSIntCtxSw start----\n");
	printf_string("OSPrioCur = %d\n", OSPrioCur);
	printf_string("OSPrioHighRdy = %d\n", OSPrioHighRdy);
	
	__asm__ __volatile__(	
	//Call OSTaskSwHook();
//	"bl OSTaskSwHook\n"
	
//	"ldmfd sp!, {r0-r3, r12, lr}\n"
	
//	"stmfd sp!, {r0-r3}\n"
	
//	"mov r1, sp\n"
//	"add sp, sp, #16\n"
//	"sub r2, lr, #4\n"
//	"mrs r3, spsr\n"
	
//	"\n"
//	"\n"
//	"\n"
//	"\n"
	
	// OSTCBCur = OSTCBHighRdy;
	"ldr r0, =OSTCBHighRdy\n"
	"ldr r1, =OSTCBCur\n"
	"ldr r0, [r0]\n"
	"str r0, [r1]\n"
	
	// OSPrioCur = OSPrioHighRdy;
	"ldr r0, =OSPrioHighRdy\n"
	"ldr r1, =OSPrioCur\n"
	"ldrb r0, [r0]\n"
	"strb r0, [r1]\n"
	
	// SP = OSTCBHighRdy->OSTCBStkPtr;
	"ldr r0, =OSTCBHighRdy\n"
	"ldr r0, [r0]\n"
	"ldr sp, [r0]\n"
	
	// Restore New Task context
	"ldmfd sp!,{r0}\n"
	"msr spsr_cxsf, r0\n"
	"ldmfd sp!, {r0-r12, lr, pc}^\n"
	);
}
*/
void OSStartHighRdy(void)
{
	__asm__ __volatile__(	
	
	// Switch to SVC mode
	"msr cpsr_cxsf, #(0x13|0xC0)\n"
	
	// Call user define Task switch hook
	"bl OSTaskSwHook\n"
	
	// OSRunning = TRUE;
	"ldr r0, =OSRunning\n"
	"mov r1, #1\n"
	"strb r1, [r0]\n"
	
	// SP = OSTCBHighRdy->OSTCBStkPtr;
	"ldr r0, =OSTCBHighRdy\n"
	"ldr r0, [r0]\n"
	"ldr sp, [r0]\n"
	
	// Prepare to return to proper mode
	"ldmfd sp!,{r0}\n"
	"msr spsr_cxsf, r0\n"
	"ldmfd sp!, {r0-r12, lr, pc}^\n"
	);
}
#define SRCPND              (*(volatile unsigned int *)0x4A000000)
#define INTPND              (*(volatile unsigned int *)0x4A000010)
void OSTickISR()
{
	print_string("------OSTickISR-------\n");
		
//	DumpAllOSTCB();
	
	OSTimeTick();
	
	return;
}

void OSTaskReturnHook(OS_TCB* ptcb)
{
	
}