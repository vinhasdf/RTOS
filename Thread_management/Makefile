MAKEDIR = .
TARGET = main
BUILDPATH = $(MAKEDIR)/Build
BUILDROOT = $(MAKEDIR)/..

VPATH =	$(BUILDROOT)/Libraries/CMSIS/CM3/CoreSupport \
			$(BUILDROOT)/Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x \
			$(BUILDROOT)/Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/startup/gcc_ride7 \
			$(BUILDPATH) \
			$(BUILDROOT)/Libraries\STM32F10x_StdPeriph_Driver\src \
			.

# define linker location and chip architecture
LD_SCRIPT = STM32F103C8Tx_FLASH.ld
MCU_SPEC = cortex-m3
DEVICE_SPEC = STM32F10X_MD

C_DEFS = \
	-D$(DEVICE_SPEC)\
	-DUSE_STDPERIPH_DRIVER

# toolchain definenition
ifdef GCC_PATH
CC = $(GCC_PATH)/bin/arm-none-eabi-gcc
AS = $(GCC_PATH)/bin/arm-none-eabi-as
LD = $(GCC_PATH)/bin/arm-none-eabi-ld
OC = $(GCC_PATH)/bin/arm-none-eabi-objcopy
OD = $(GCC_PATH)/bin/arm-none-eabi-objdump
OS = $(GCC_PATH)/bin/arm-none-eabi-size
else
CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
LD = arm-none-eabi-ld
OC = arm-none-eabi-objcopy
OD = arm-none-eabi-objdump
OS = arm-none-eabi-size
endif

# assembly directives.
ASFLAGS += -c
ASFLAGS += -O0
ASFLAGS += -mcpu=$(MCU_SPEC)
ASFLAGS += -mthumb
ASFLAGS += -Wall
# set error message to appear on single line
ASFLAGS += -fmessage-length=0
ASFLAGS += -Og

# C compilation directives
CFLAGS += $(C_DEFS)
CFLAGS += -mcpu=$(MCU_SPEC)
CFLAGS += -mthumb
CFLAGS += -Wall
CFLAGS += -g
# set error message to appear on single line
CFLAGS += -fmessage-length=0
# set system to ignore semihosted junk
CFLAGS += --specs=nosys.specs
CFLAGS += --specs=nano.specs
CFLAGS += -Og

# linker directives.
LSCRIPT = $(LD_SCRIPT)
LFLAGS += -mthumb
LFLAGS += -mcpu=$(MCU_SPEC)
LFLAGS += -Wall
LFLAGS += --specs=nosys.specs
LFLAGS += --specs=nano.specs
LFLAGS += -nostdlib
LFLAGS += -lgcc
LFLAGS += -T$(LSCRIPT)

VECT_TBL = startup_stm32f10x_md.s
			#systick_hand.s

C_SRC = system_stm32f10x.c \
			main.c \
			stm32f10x_it.c \
			Thread_Ram.c \
			stm32f10x_rcc.c \
			stm32f10x_gpio.c \
			Os.c

OBJS = $(VECT_TBL:.s=.o)
OBJS += $(C_SRC:.c=.o)

INC = -I$(BUILDROOT)/Libraries/CMSIS/CM3/CoreSupport \
			-I$(BUILDROOT)/Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x \
			-I$(BUILDROOT)/Libraries\STM32F10x_StdPeriph_Driver\inc \
			-I.

.PHONY: all
all: $(BUILDPATH) $(TARGET).bin

%.o: %.s
	rm -f $@
	$(CC) -x assembler-with-cpp $(ASFLAGS) $< -o $(notdir $@)
	mv $(notdir $@) $(BUILDPATH)
	@echo "Compile assembly file $<"

%.o: %.c
	rm -f $@
	@echo "Compile C file $<"
	$(CC) -c $(CFLAGS) $(INC) $< -o $(notdir $@)
	mv $(notdir $@) $(BUILDPATH)

$(TARGET).elf: $(OBJS)
	rm -f $@
	$(CC) $(addprefix $(BUILDPATH)/, $(notdir $^)) $(LFLAGS) -o $(notdir $@)
	mv $(notdir $@) $(BUILDPATH)

$(TARGET).bin: $(TARGET).elf
	rm -f $@
	$(OC) -S -O binary $(BUILDPATH)/$(notdir $<) $(notdir $@)
	$(OS) $(BUILDPATH)/$(notdir $<)
	mv $(notdir $@) $(BUILDPATH)

$(BUILDPATH):
	@echo "Make $(BUILDPATH) folder"
	mkdir $(BUILDPATH)

.PHONY: clean
clean:
	cd $(BUILDPATH) && rm -f *
	rm -f $(BUILDPATH)/$(TARGET).elf
	rm -f $(BUILDPATH)/$(TARGET).bin