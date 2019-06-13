.text			/* Indicate code section */
.Global _start 	/* Indicate Global variable */
_start:
	b reset
	ldr pc, _undifined_instruction
	ldr pc, _software_interrupt
	ldr pc, _prefetch_abort
	ldr pc, _data_abort
	ldr pc, _not_used
	ldr pc, _irq
	ldr pc, _fiq
	

_undifined_instruction: .word undifined_instruction
_software_interrupt: .word software_interrupt
_prefetch_abort: .word prefetch_abort
_data_abort: .word data_abort
_not_used: .word not_used
_irq: .word irq
_fiq: .word reset

undifined_instruction:
	bl UndifinedInstruction
	b reset

software_interrupt:
	bl SwiInterrupt
	b reset
	
prefetch_abort:
	bl PrefetchAbort
	b reset
	
data_abort:
	bl DataAbort
	b reset

not_used:
	nop
#define IRQMODE 0x12
#define SVCMODE 0x13	
#define NOINT 0xc0
irq:
	//We will use R1-R3 as temporary registers
	stmfd sp!, {r1-r3}
	
	//     R1--SP  (IRQ MODE)
	//	   R2--PC   (TASK POINTER) 
	//     R3--SPSR  (TASK CPSR)
	mov r1, sp
	add sp, sp, #12
	sub r2, lr, #4
	mrs r3, spsr
	
	//Change to SVC mode
	msr cpsr_cxsf, #(SVCMODE|NOINT)
	
	//Save task's context into task's stack
	//Take care! This SP is not the one before!
	
	stmfd sp!, {r2}
	stmfd sp!, {r4-r12, lr}
	
	ldmfd r1!, {r4-r6}
	stmfd sp!, {r4-r6}
	stmfd sp!, {r0}
	
	//Push task's CPSR
	stmfd sp!, {r3}
	
	//OSIntNesting++
	ldr r0, =OSIntNesting
	ldrb r1, [r0]
	add r1, r1, #1
	strb r1, [r0]
	
	//if(OSIntNesting == 1){
	cmp r1, #1
	bne NotFirstInterrupt
	
	//OSTCBCur->OSTCBStkPtr = SP;
	ldr r4, =OSTCBCur
	ldr r5, [r4]
	str sp, [r5]
	//}
	
NotFirstInterrupt:	
	//Change to IRQ mode to use IRQ stack to handle interrupt
	msr cpsr_c, #(IRQMODE|NOINT)
	
	ldr lr, =IRQIsrVect
	ldr pc, =HandleIrq
	
IRQIsrVect:
    msr cpsr_cxsf, #(SVCMODE|NOINT)
	bl OSIntExit
	ldmfd sp!, {r4}
	msr spsr_cxsf, r4
	ldmfd sp!,{r0-r12,lr,pc}^
	
fiq:	
	nop

.extern nandflash_init
.extern copy_to_ram
.extern gboot_main

#define _TEXT_BASE 0x30010000
.Global reset
reset:
	bl set_svc	
	bl disable_watchdog
	bl disable_interrupt
	bl disable_mmu
	bl init_clock
	bl init_sdram
	bl init_stack

	bl nandflash_init
	
	bl light_led
	
	adr r0, _start
	ldr r1, =_TEXT_BASE
	cmp r0, r1
	blne copy_to_ram
	bl clean_bss
	ldr pc, =gboot_main

set_svc:
	mrs r0, cpsr
	bic r0, r0,#0x1f
	orr r0, r0,#0xd3
	msr cpsr, r0
	mov pc, lr

#define pWTCON 0x53000000
disable_watchdog:
	ldr r0, =pWTCON
	mov r1, #0x0
	str r1, [r0]
	mov pc, lr

disable_interrupt:
	mvn r1, #0x0
	ldr r0, =0x4a000008
	str r1, [r0]
	mov pc, lr
	
disable_mmu:
	mcr p15,0,r0,c7,c7,0
	mrc p15,0,r0,c1,c0,0
	bic r0, r0, #0x00000007
	mcr p15,0,r0,c1,c0,0
	mov pc, lr

#define CLKDIVN 0x4c000014
#define MPLLCON 0x4c000004
#define MPLL_405MHZ ((127<<12)|(2<<4)|(1<<0))

init_clock:
	ldr r0, =CLKDIVN
	mov r1, #0x5		//FCLK : HCLK : PCLK = 1 : 4 : 8(Divide Ratio)
	str r1, [r0]
	
	mcr p15,0,r0,c1,c0,0
	orr r0,r0,#0xc0000000
	mcr p15,0,r0,c1,c0,0
	
	ldr r0, =MPLLCON
	ldr r1, =MPLL_405MHZ
	str r1, [r0]
	mov pc, lr

#define mem_contrl 0x48000000
init_sdram:
	ldr r0, =mem_contrl 
	add r3, r0, #4*13
	adrl r1, mem_data

0:
	ldr r2, [r1], #4
	str r2, [r0], #4
	cmp r0, r3
	bne 0b
	mov pc, lr
	
#define copy_length 0x150000	
copy_to_ram:
	mov r0,#0x00
	ldr r1,=_start
	ldr r2,=bss_end
	sub r2,r2,r1
	mov ip,lr
	bl nand_to_ram
	mov lr,ip
	mov pc,lr
	
init_stack:
	msr cpsr_c, #0xd2	//Switch irq mode and set sps
	ldr sp, =0x33000000
	msr cpsr_c, #0xd3	//Switch back to SVC
	ldr sp, =0x34000000
	mov pc, lr

clean_bss:
	ldr r0, =bss_start
	ldr r1, =bss_end
	cmp r0, r1
	moveq pc, lr

clean_loop:
	mov r2, #0
	str r2, [r0], #4
	cmp r0, r1
	bne clean_loop
	mov pc, lr

mem_data:
	.long 0x22000000 
	.long 0x00000700
	.long 0x00000700
	.long 0x00000700
	.long 0x00000700
	.long 0x00000700
	.long 0x00000700
	.long 0x00018001
	.long 0x00018001
	.long 0x008c04f5
	.long 0x000000b1
	.long 0x00000030
	.long 0x00000030

#define GPBCON 0x56000010
#define GPBDAT 0x56000014
.Global light_led
light_led:
	ldr r0, =GPBCON
	mov r1, #0x400
	str r1, [r0]
	
	ldr r0, =GPBDAT
	mov r1, #0x0
	str r1, [r0]
	mov pc, lr
	
dead_loop:
	b dead_loop
	
.Global OSCtxSw	
OSCtxSw:
	//1. Save the break pointer to task's stack.
	stmfd sp!, {lr}
	
	//2. Save the general register to task's stack
	stmfd sp!, {r0-r12, lr}
	mrs r0, cpsr
	stmfd sp!, {r0}
	
	//3. Save the stack pointer to the OS_TCB->OSTCBStkPtr 
	// OSTCBCur->OSTCBStkPtr = SP
	ldr r0, =OSTCBCur
	ldr r0, [r0]
	str sp, [r0]
	
	//4. Get the task's OS_TCB which will be exec
	// OSTCBCur = OSTCBHighRdy;
	ldr r0, =OSTCBHighRdy
	ldr r1, =OSTCBCur
	ldr r0, [r0]
	str r0, [r1]
	
	// OSPrioCur = OSPrioHighRdy;
	ldr r0, =OSPrioHighRdy
	ldr r1, =OSPrioCur
	ldrb r0, [r0]
	strb r0, [r1]
	
	//5. Get the stack pointer from the OS_TCB and set the stack register
	// SP = OSTCBHighRdy->OSTCBStkPtr;
	ldr r0, = OSTCBCur
	ldr r0, [r0]
	ldr sp, [r0]
	
	//6. Get the content of general register which store in the stack and set the register
	//7. Get the new pointer and set to the PC register
	ldmfd sp!,{r0}
	msr SPSR_cxsf, r0
	ldmfd sp!, {r0-r12, lr, pc}^

	
.Global OSIntCtxSw	
OSIntCtxSw:
	// OSTCBCur = OSTCBHighRdy;
	ldr r0, =OSTCBHighRdy
	ldr r1, =OSTCBCur
	ldr r0, [r0]
	str r0, [r1]
	
	// OSPrioCur = OSPrioHighRdy;
	ldr r0, =OSPrioHighRdy
	ldr r1, =OSPrioCur
	ldrb r0, [r0]
	strb r0, [r1]
	
	// SP = OSTCBHighRdy->OSTCBStkPtr;
	ldr r0, =OSTCBHighRdy
	ldr r0, [r0]
	ldr sp, [r0]
	
	// Restore New Task context
	ldmfd sp!,{r0}
	msr spsr_cxsf, r0
	ldmfd sp!, {r0-r12, lr, pc}^
	
