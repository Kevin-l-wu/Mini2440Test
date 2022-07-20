.PHONY : all

#DIR_BUILD := /home/Linux/Mini2440Test/build
#DIR_COMMON_INC := /home/Linux/Mini2440Test/Include

DIR_SRC := src
DIR_INC := inc

TYPE_INC := .h
TYPE_SRC := .c
TYPE_OBJ := .o
TYPE_DEP := .dep

AR := arm-none-eabi-ar
ARFLAGS := crs

LD := arm-none-eabi-ld
LFLAGS := -r

CC := arm-none-eabi-gcc
#CFLAGS_SUB := -I$(DIR_INC) -I$(DIR_COMMON_INC) -fno-builtin -march=armv4t

ifeq ($(DEBUG),true)
CFLAGS_SUB += -g
endif

MODULE := $(realpath .)
MODULE := $(notdir $(MODULE))
DIR_OUTPUT := $(addprefix $(DIR_BUILD)/, $(MODULE))

OUTPUT := $(MODULE).o
OUTPUT := $(addprefix $(DIR_BUILD)/, $(OUTPUT))

SRCS := $(wildcard $(DIR_SRC)/*$(TYPE_SRC))
OBJS := $(SRCS:$(TYPE_SRC)=$(TYPE_OBJ))
OBJS := $(patsubst $(DIR_SRC)/%, $(DIR_OUTPUT)/%, $(OBJS))
DEPS := $(SRCS:$(TYPE_SRC)=$(TYPE_DEP))
DEPS := $(patsubst $(DIR_SRC)/%, $(DIR_OUTPUT)/%, $(DEPS))

vpath %$(TYPE_INC) $(DIR_INC)
vpath %$(TYPE_INC) $(DIR_COMMON_INC)
vpath %$(TYPE_SRC) $(DIR_SRC)

-include $(DEPS)

all : $(OUTPUT)
	@echo "Success! Target ==> $(OUTPUT)"
	
$(OUTPUT) : $(OBJS)
#	$(AR) $(ARFLAGS) $@ $^
	$(LD) $(LFLAGS) -o $@ $^
	
$(DIR_OUTPUT)/%$(TYPE_OBJ) : %$(TYPE_SRC)
	$(CC) $(CFLAGS_SUB) -o $@ -c $(filter %$(TYPE_SRC), $^)
	
	
$(DIR_OUTPUT)/%$(TYPE_DEP) : %$(TYPE_SRC)
	@echo "Creating $@ ..."
	@set -e; \
	$(CC) $(CFLAGS_SUB) -MM -E $(filter %$(TYPE_SRC), $^) | sed 's,\(.*\)\.o[ :]*,$(DIR_OUTPUT)/\1$(TYPE_OBJ) $@ : ,g' > $@
