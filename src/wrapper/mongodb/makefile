
ifndef $(LCT_SVC_PRJ_ROOT)
ROOT_DIR_SUFFIX_PART := $(lastword  $(subst src, ,$(CURDIR)))
LCT_SVC_PRJ_ROOT     := $(firstword $(subst /src$(ROOT_DIR_SUFFIX_PART), ,$(CURDIR)))
endif

TARGET_TYPE       = lib
SRC_SUFFIX        = cpp
INC_DIR           += -I$(LCT_SVC_PRJ_ROOT)/src/common -I$(LCT_SVC_PRJ_ROOT)/src/log  -I$(LCT_SVC_PRJ_ROOT)/src/3rdparty/mongocxx
SYS_LIB           += 
DEP_OBJ           +=
TARGET            := libmongo_wrapper.a

include $(LCT_SVC_PRJ_ROOT)/src/common/common.mk

