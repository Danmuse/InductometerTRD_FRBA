.PHONY: clean

-include config.mk

BUILD_DIR ?= build/
SOURCE_DIR ?= .

ECHO = echo
FIND = find
MKDIR = mkdir -p
RM = rm -rf

ARCH = arm-none-eabi
AS = $(ARCH)-as
CC = $(ARCH)-gcc
CXX = $(ARCH)-g++
GDB = $(ARCH)-gdb
OBJCOPY = $(ARCH)-objcopy
OBJDUMP = $(ARCH)-objdump
SIZE = $(ARCH)-size
LD = $(ARCH)-g++

DEBUG_CLFAGS = -g3 -ggdb
COMMON_CFLAGS = -Wall -Wextra -ffreestanding -ffunction-sections -fdata-sections -O3
ARCH_CFLAGS = -mlittle-endian -mcpu=cortex-m3 -march=armv7-m -mthumb -mfloat-abi=soft
CFLAGS ?= -nostdlib -nostartfiles -specs=nosys.specs -specs=nano.specs -std=gnu99
CXXFLAGS ?= -nostdlib -nostartfiles -specs=nosys.specs


CFLAGS := ${COMMON_CFLAGS} ${DEBUG_CLFAGS} ${ARCH_CFLAGS} ${CFLAGS}
CXXFLAGS := ${COMMON_CFLAGS} ${DEBUG_CLFAGS} ${ARCH_CFLAGS} ${CXXFLAGS}
AFLAGS := --warn --fatal-warnings ${AFLAGS}
LDFLAGS := ${CFLAGS} -Wl,--gc-sections ${LDFLAGS}


TARGET = $(BUILD_DIR)$(PROJECT_NAME)
VPATH = ${SOURCE_DIR}


SOURCES = $(shell $(FIND) ${SOURCE_DIR} -iregex ".*\.\\(c\\|s\\|cpp\\)" -printf "%P\n")
objects = $(patsubst %.cpp,%.o,$(patsubst %.c,%.o,$(patsubst %.s,%.o,$(SOURCES))))
OBJECTS = $(addprefix $(BUILD_DIR), $(objects))
LINKERS = $(shell $(FIND) ${SOURCE_DIR} -name '*.ld')


$(TARGET).elf: $(OBJECTS) $(LINKERS) Makefile config.mk
	$(LD) $(addprefix -T ,$(LINKERS)) -Wl,-Map,$(@:.elf=.map) -o $@ $(OBJECTS) $(LDFLAGS)
	@$(OBJCOPY) -O binary $@ $(@:.elf=.bin)
	@$(OBJCOPY) -O ihex $@ $(@:.elf=.hex)
	@$(ECHO)
	@$(SIZE) $@ | tail -1 - | awk '{print "Size: "$$1+$$2" B"}'


$(BUILD_DIR)%.o: %.c Makefile config.mk
	@$(MKDIR) `dirname $@`
	$(CC) -c -MMD -MP $(CFLAGS) -o $@ $<


$(BUILD_DIR)%.o: %.cpp Makefile config.mk
	@$(MKDIR) `dirname $@`
	$(CXX) -c -MMD -MP $(CXXFLAGS) -o $@ $<


$(BUILD_DIR)%.o: %.s Makefile config.mk
	@$(MKDIR) `dirname $@`
	$(AS) $(AFLAGS) -o $@ $<


-include $(OBJECTS:.o=.d)


clean:
	$(RM) $(BUILD_DIR)*


run: $(TARGET).elf example/image.png.dat
	$(GDB) --command=run.gdb -nh -q --batch $(TARGET).elf


example/image.png.dat: example/image.png
	python example/convert_image.py example/image.png
