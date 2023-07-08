#####################################
# @file Makefile
# @brief GCC编译文件, 从开源项目的Makefile修改而来, 支持自动搜索源文件, 支持CPP混编(有BUG)
# @author cwxiaos
# @date 2023-04-06
#####################################


######################################
# target
######################################
TARGET = main

######################################
# device
# 这里指定串口设备的位置, 在Ubuntu20.04下, 通过ls /dev/tty*命令查看
# 一般是/dev/ttyUSB*或者/dev/ttyACM*
# 如果不指定, 会自动查找, 如果有多个串口设备, 需要手动指定
######################################
DEVICE ?= ${shell find /dev -name "ttyACM*" -or -name "ttyUSB*" | head -n 1}
# DEVICE := /dev/ttyACM0

######################################
# building variables
######################################
# Select debug or retail build
DEBUG = 1

# optimization
# 这里开了O3优化, 一些逻辑可能会出现问题, 如果出现问题, 可以尝试关闭优化
OPT = -O3

#######################################
# paths
#######################################
# source path

# Build path
BUILD_DIR = build

######################################
# source
######################################
# C sources
# 通过shell命令查找当前目录下所有的.c文件，需要保持目录干净, 不要有其他的.c文件
C_SOURCES = $(shell find ./ -name '*.c')

# 这里支持CPP混编, 但是如果需要加入CPP, 需要修改-mfloat-abi=softfp, 会出现一些问题
# 如果工程中存在需要编译的CPP文件, 需要取消下面一行的注释
# CPP_SOURCES = $(shell find ./ -name '*.cpp')

# ASM sources
ASM_SOURCES = startup_stm32f407xx.s

#######################################
# binaries
#######################################
PREFIX = arm-none-eabi-
# The gcc compiler bin path can be either defined in make command via GCC_PATH variable (> make GCC_PATH=xxx)
# either it can be added to the PATH environment variable.
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
CXX = $(GCC_PATH)/$(PREFIX)g++
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
else
CC = $(PREFIX)gcc
CXX = $(PREFIX)g++
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

#######################################
# CFLAGS
#######################################
# cpu
CPU = -mcpu=cortex-m4

# fpu
FPU = -mfpu=fpv4-sp-d16

# float-abi
FLOAT-ABI = -mfloat-abi=hard

# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS = 

# C defines
# 定义目标芯片平台,使用标准库
C_DEFS =  \
-DSTM32F40_41xxx \
-DUSE_STDPERIPH_DRIVER\

# AS includes
AS_INCLUDES =

# C includes
# 添加头文件路径
C_INCLUDES = -Isrc/include
C_INCLUDES += -ILibrary/FreeRTOS/include
C_INCLUDES += -ILibrary/FreeRTOS/portable/GCC/ARM_CM4F
C_INCLUDES += -ILibrary/STM32F4x_StdPeriph_Driver/inc
C_INCLUDES += -ILibrary/CMSIS/DEVICE
C_INCLUDES += -ILibrary/CMSIS/CORE


# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -ggdb -gdwarf-2
endif


# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)"

# 指定app_log.h中的LOG_LOCAL_LEVEL
CFLAGS += -DLOG_LOCAL_LEVEL=LOG_VERBOSE

# 指定串口波特率
CFLAGS += -DUART_BAUDRATE=115200

CXXFLAGS=-c $(MCFLAGS) $(DEFS) $(OPTIMIZE) $(addprefix -I,$(INCLUDE_DIRS)) -std=c++11
CXXFLAGS+=-U__STRICT_ANSI__


#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = STM32F407ZGx_FLASH.ld


# libraries
# 这里使用了newlib-nano, 这里的库与浮点数和printf有关
LIBS = -lc -lm -lrdimon -lnosys
LIBDIR = 
LDFLAGS = $(MCU) -Wl,-u_printf_float -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections
LDFLAGS += --specs=nano.specs

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin


#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of C++ program objects
# 添加CPP文件
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(CPP_SOURCES:.cpp=.o)))
vpath %.cpp $(sort $(dir $(CPP_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))


$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	$(info ************  Building C Files ************)
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

# 编译CPP文件
$(BUILD_DIR)/%.o: %.cpp Makefile | $(BUILD_DIR)
	$(info ************  Building CPP Files ************)
	$(CXX) -c $(CXXFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.cpp=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(info ************  Building AS Files ************)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(info ************  Link objects & build ELF Files ************)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(info ************  Calling SZ ************)
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@
	
$(BUILD_DIR):
	mkdir $@

#######################################
# clean up
#######################################
clean:
	rm -rf .dep $(BUILD_DIR)

#######################################
# flash
# 这里是烧录命令, 使用OpenOCD, 也可以使用stm32flash
#######################################
flash:
	openocd -f /usr/local/share/openocd/scripts/interface/cmsis-dap.cfg -f /usr/local/share/openocd/scripts/target/stm32f4x.cfg -c "program build/main.bin 0x08000000 verify reset exit;"
# 	stm32flash -b 115200 -w $(BUILD_DIR)/$(TARGET).bin -v -g 0x0 $(DEVICE)

#######################################
# monitor
# 使用picocom, Ubuntu下可以使用sudo apt-get install picocom安装
#######################################
monitor:
	picocom -b 115200 $(DEVICE)