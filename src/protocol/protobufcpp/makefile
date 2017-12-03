
ifndef $(LCT_SVC_PRJ_ROOT)
ROOT_DIR_SUFFIX_PART := $(lastword  $(subst src, ,$(CURDIR)))
LCT_SVC_PRJ_ROOT     := $(firstword $(subst /src$(ROOT_DIR_SUFFIX_PART), ,$(CURDIR)))
endif

TARGET_TYPE       = lib
SRC_SUFFIX        = cc
CPP_FLAG_VERSION  = 3rd

INC_DIR           += -I$(LCT_SVC_PRJ_ROOT)/src/3rdparty/grpc
INC_DIR           += -I$(LCT_SVC_PRJ_ROOT)/src/protocol/protobufcpp

SYS_LIB_DIR       +=
SYS_LIB           += 

DEP_OBJ           += 
TARGET            := liblct_protobuf.a

include $(LCT_SVC_PRJ_ROOT)/src/common/common.mk