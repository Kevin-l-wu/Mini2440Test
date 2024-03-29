
.PHONY : all compile link clean rebuild

MODULES := Init \
	Common \
	SimpleFile \
	LibCLite \
	Core \
	Main \
	Driver \
	Net \
	Test \
	Protocol \
	Command \
	UCOS\
	Xui \
	Shell
		   
MKDIR := mkdir
RM := rm -fr

CC := arm-none-eabi-gcc
LD := arm-none-eabi-ld
#LFLAGS := -Tgboot.lds
LFLAGS := -Tgboot.lds -L /Applications/ARM/lib/gcc/arm-none-eabi/10.3.1 -lgcc
#LFLAGS := -Tgboot.lds -L /usr/local/arm/4.3.2/lib/gcc/arm-none-linux-gnueabi/4.3.2/armv4t -lgcc
#LFLAGS := -Tgboot.lds -L /usr/local/5.4.0/lib/gcc/arm-none-linux-gnueabi/5.4.0/ -lgcc

OBJCOPY := arm-none-eabi-objcopy
OFLAG := -O binary

DIR_PROJECT := $(realpath .)
DIR_BUILD := Build
DIR_COMMON_INC := Include
DIR_BUILD_SUB := $(addprefix $(DIR_BUILD)/, $(MODULES))
MODULE_LIB := $(addsuffix .o, $(MODULES))
MODULE_LIB := $(addprefix $(DIR_BUILD)/, $(MODULE_LIB))

START := start.o
START := $(addprefix $(DIR_BUILD)/, $(START))

OUTPUT := gboot.elf
OUTPUT := $(addprefix $(DIR_BUILD)/, $(OUTPUT))

OUTPUT_BIN := gboot.bin
OUTPUT_BIN := $(addprefix $(DIR_BUILD)/, $(OUTPUT_BIN))

DEBUG := true

all : compile $(OUTPUT) $(OUTPUT_BIN)
	@echo "Success! Target ==> $(OUTPUT)"
	:
compile : $(DIR_BUILD) $(DIR_BUILD_SUB)
	@echo "Begin to compile ..."
	@set -e; \
	for dir in $(MODULES); \
	do \
		cd $$dir && \
		$(MAKE) all \
			DEBUG:=$(DEBUG) \
			DIR_BUILD:=$(addprefix $(DIR_PROJECT)/, $(DIR_BUILD)) \
			DIR_COMMON_INC:=$(addprefix $(DIR_PROJECT)/, $(DIR_COMMON_INC)) && \
		cd .. ; \
	done
	@echo "Compile Success!"
	
link $(OUTPUT) : $(MODULE_LIB) $(START)
	@echo "Begin to link ..."
	$(LD) -o $(OUTPUT) $(MODULE_LIB) $(START) $(LFLAGS)
	@echo "Link Success!"

$(OUTPUT_BIN) : $(OUTPUT)
	$(OBJCOPY) $(OFLAG) $(OUTPUT) $(OUTPUT_BIN)
	
$(DIR_BUILD) $(DIR_BUILD_SUB) : 
	$(MKDIR) $@
	
$(START) : start.S
	$(CC) -march=armv4t -g -c -o $@ $^
	
clean : 
	@echo "Begin to clean ..."
	$(RM) $(DIR_BUILD)
	@echo "Clean Success!"
	
rebuild : clean all
