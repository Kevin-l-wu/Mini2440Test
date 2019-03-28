OUTPUT_ARCH(arm)
ENTRY(_start)
SECTIONS {
	. = 0x30050000;
	
	. = ALIGN(4);
	.text :
	{
	Build/start.o (.text)
	Build/Main.o (.text)
	Build/Init.o (.text)
	*(.text)
	}

	. = ALIGN(4);
	.data : 
	{
	*(.data)
	}
	
	. = ALIGN(4);
	Module_begin = .;
	.Module : 
	{
	*(.Module) 
	}
	Module_end = .;
	
	. = ALIGN(4);
	Command_begin = .;
	.Command : 
	{
	*(.Command) 
	}
	Command_end = .;
	
	. = ALIGN(4);
	bss_start = .;
	.bss : 
	{
	*(.bss) 
	}
	bss_end = .;
}
