ifndef $(LCT_SVC_PRJ_ROOT)
ROOT_DIR_SUFFIX_PART := $(lastword  $(subst src, ,$(CURDIR)))
LCT_SVC_PRJ_ROOT     := $(firstword $(subst /src$(ROOT_DIR_SUFFIX_PART), ,$(CURDIR)))
endif

SRC_DIR           := detail
TARGET_TYPE       = lib
SRC_SUFFIX        = cpp
INC_DIR           += -I$(LCT_SVC_PRJ_ROOT)/src/common -I$(LCT_SVC_PRJ_ROOT)/src/log  
INC_DIR           += -I$(LCT_SVC_PRJ_ROOT)/src/3rdparty/cassandra/include -I$(LCT_SVC_PRJ_ROOT)/src/wrapper/cassandra
SYS_LIB           += 
DEP_OBJ           +=
TARGET            := libcassandra_wrapper.a

include $(LCT_SVC_PRJ_ROOT)/src/common/common.mk