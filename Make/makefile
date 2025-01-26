
#Compiler Executables (In $(PATH) or Absolute path)
CC = clang

#Makefile.project.part.defines and component specific make files should extend these
C_SOURCE_FILES = 
S_ASSEMBLY_FILES = 
C_OBJ_FILES = 
S_OBJ_FILES =
INCDIR = 

#include directories as compiler options
INC := -I/usr/local/include

CFLAGS=-std=c89
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -Wpointer-arith
CFLAGS += -Wcast-align
CFLAGS += -Wwrite-strings
CFLAGS += -Wswitch-default
CFLAGS += -Wunreachable-code
CFLAGS += -Winit-self
CFLAGS += -Wmissing-field-initializers
CFLAGS += -Wno-unknown-pragmas
CFLAGS += -Wstrict-prototypes
CFLAGS += -Wundef
CFLAGS += -Wold-style-definition
CFLAGS += -Wl,-map,$(MODULE_NAME)_Test.map
CFLAGS += -DVIRTUAL_TARGET
CFLAGS += -g

# Some warnings can be ignored due to test environment
# pointers on my development target have 64bit size, software is 
# CFLAGS += -Wno-pointer-to-int-cast
# Some code is unreachable due to test dummys (i.e. Watchdog_RP2040.h)
CFLAGS += -Wno-unreachable-code
# Printf statements are not important (yet)
CFLAGS += -Wno-incompatible-pointer-types-discards-qualifiers
CFLAGS += -Wno-format

#CFLAGS += -Wno-misleading-indentation

#Compiler flags
COMPILER_DEFINES += 
CCFLAGS = $(CFLAGS)
LDFLAGS = -T $(LINKER_COMMAND_FILE) -Map $(OUT_DIR)/$(PROJECT_NAME).map
#-L/Library/Developer/CommandLineTools/usr/lib/clang/13.1.6/lib/darwin/libclang_rt.builtins-arm.a -lgcc
ASFLAGS = 

#----------------------------------------------------
# General Makefile configuration - not to be adjusted
#----------------------------------------------------
# Test builds will be performed from the mak folder, so we have to go to the component root here.
ROOT_DIR=..
MODULE_NAME=RingBuffer
UNITY_ROOT=$(ROOT_DIR)/../../uti/tools/unity

# Test Runner will be $(ROOT_DIR)/Test/$(MODULE_NAME)_TestRunner.c
# Tests file will be (for now), one big tests file.
# So we can assume to make one executable and test with that one executable.
TEST_RUNNER=$(ROOT_DIR)/Test/$(MODULE_NAME)_TestRunner.c
TESTS_FILE=$(ROOT_DIR)/Test/$(MODULE_NAME)_Tests.c
SOURCE_FILES=$(ROOT_DIR)/Source/$(MODULE_NAME).c
C_SOURCE_FILES += $(TEST_RUNNER) $(TESTS_FILE) $(SOURCE_FILES) $(UNITY_ROOT)/src/unity.c

TEST_INDEPENDENT_EXE = $(ROOT_DIR)/Test/exe/$(MODULE_NAME)_Test_INDEPENDENT.out
TEST_MEMPOOL_EXE = $(ROOT_DIR)/Test/exe/$(MODULE_NAME)_Test_MEMPOOL.out

INCLUDE_PATH += ../Include
INCLUDE_PATH += $(UNITY_ROOT)/src
INCLUDE_PATH += $(ROOT_DIR)/../BRS_RP2040/include

C_SOURCE_FILES_MEMPOOL = $(C_SOURCE_FILES) $(ROOT_DIR)/../MemPool/Source/MemPool.c
INCLUDE_PATH_MEMPOOL = $(INCLUDE_PATH) $(ROOT_DIR)/../MemPool/Include

INC += $(addprefix -I, $(INCLUDE_PATH))
INC_IND = $(INC)
INC_MEMPOOL = $(INC)
INC_MEMPOOL += $(addprefix -I, $(INCLUDE_PATH_MEMPOOL))

#default make target
all: test_independent test_mempool

test_independent: $(SRC_FILES1)
	mkdir -p $(ROOT_DIR)/Test/exe
	@echo "\033[1;32mTesting The Indepdendent Build\033[0m"
	$(CC) $(CCFLAGS) $(INC_IND) $(C_SOURCE_FILES) -o $(TEST_INDEPENDENT_EXE) -DVIRTUAL_TARGET -DRINGBUFFER_USECASE=0
	- ./$(TEST_INDEPENDENT_EXE)

test_mempool: $(SRC_FILES1)
	mkdir -p $(ROOT_DIR)/Test/exe
	@echo "\033[1;32mTesting The MEMPOOL Build\033[0m"
	$(CC) $(CCFLAGS) $(INC_MEMPOOL) $(C_SOURCE_FILES_MEMPOOL) -o $(TEST_MEMPOOL_EXE) -DVIRTUAL_TARGET -DRINGBUFFER_USECASE=1
	- ./$(TEST_MEMPOOL_EXE)


clean :
	@rm -f *.o $(PROJECT_NAME).elf $(PROJECT_NAME).list $(PROJECT_NAME).bin $(PROJECT_NAME).uf2 $(PROJECT_NAME).map 

cleaner: clean
	@rm -r -f $(OBJ_DIR)
	@rm -r -f $(OUT_DIR)
	@rm -r -f $(LST_DIR)

