
ifndef $(LCT_SVC_PRJ_ROOT)
ROOT_DIR_SUFFIX_PART := $(lastword  $(subst src, ,$(CURDIR)))
LCT_SVC_PRJ_ROOT     := $(firstword $(subst /src$(ROOT_DIR_SUFFIX_PART), ,$(CURDIR)))
endif

TARGET_TYPE       = app
SRC_SUFFIX        = cpp
INC_DIR           += -I$(LCT_SVC_PRJ_ROOT)/src/common -I$(LCT_SVC_PRJ_ROOT)/src/log
SYS_LIB           += -pthread
DEP_OBJ           += $(LIB_DIR)/liblct_log.a $(LIB_DIR)/liblct_common.a
TARGET            := circular_buffer_sanity_test

include $(LCT_SVC_PRJ_ROOT)/src/common/common.mk

