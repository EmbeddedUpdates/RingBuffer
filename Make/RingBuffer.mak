#Component specific makefile for ProtoBuf
$(info Executing: Components/ProtoBuf/make/ProtoBuf.mak)
#C files that should be compiled in this component
C_SOURCE_FILES += Components/ProtoBuf/Source/ProtoBuf.c

#include path for header files in this component
INCLUDE_PATH += $(ROOT_DIR)/Components/ProtoBuf/Include
