
.PHONY : all compile link clean rebuild

MODULES := Init \
           CommonLib \
           Core \
		   Main \
           Driver \
		   Net \
		   Test \
		   Protocol \
		   Command \
		   UCos \
		   Pax
           
MKDIR := mkdir
RM := rm -fr

CC := arm-linux-gcc
LD := arm-linux-ld
LFLAGS := -Tgboot.lds -L /usr/local/arm/4.3.2/lib/gcc/arm-none-linux-gnueabi/4.3.2/armv4t -lgcc

OBJCOPY := arm-linux-objcopy
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
	$(CC) -g -c -o $@ $^
	
clean : 
	@echo "Begin to clean ..."
	$(RM) $(DIR_BUILD)
	@echo "Clean Success!"
	
rebuild : clean all