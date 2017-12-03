
ifndef $(LCT_SVC_PRJ_ROOT)
ROOT_DIR_SUFFIX_PART := $(lastword  $(subst src, ,$(CURDIR)))
LCT_SVC_PRJ_ROOT     := $(firstword $(subst /src$(ROOT_DIR_SUFFIX_PART), ,$(CURDIR)))
endif

TARGET_TYPE       = app
SRC_SUFFIX        = cpp
INC_DIR           += 
SYS_LIB           += 
TARGET            := version-tool

include $(LCT_SVC_PRJ_ROOT)/src/common/common.mk
