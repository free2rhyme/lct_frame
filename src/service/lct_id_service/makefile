
ifndef $(LCT_SVC_PRJ_ROOT)
ROOT_DIR_SUFFIX_PART := $(lastword  $(subst src, ,$(CURDIR)))
LCT_SVC_PRJ_ROOT     := $(firstword $(subst /src$(ROOT_DIR_SUFFIX_PART), ,$(CURDIR)))
endif

TARGET_TYPE       = app
SRC_SUFFIX        = cpp
INC_DIR           += -I$(LCT_SVC_PRJ_ROOT)/src/common -I$(LCT_SVC_PRJ_ROOT)/src/log 
INC_DIR           += -I$(LCT_SVC_PRJ_ROOT)/src/model -I$(LCT_SVC_PRJ_ROOT)/src/event 
INC_DIR           += -I$(LCT_SVC_PRJ_ROOT)/src/3rdparty/cassandra/include -I$(LCT_SVC_PRJ_ROOT)/src/wrapper/cassandra 
INC_DIR           += -I$(LCT_SVC_PRJ_ROOT)/src/3rdparty/grpc -I$(LCT_SVC_PRJ_ROOT)/src/protocol/protobufcpp -I$(LCT_SVC_PRJ_ROOT)/src/wrapper/grpc 

SYS_LIB_DIR       += -L$(LCT_SVC_PRJ_ROOT)/src/3rdparty/grpc/lib
SYS_LIB_DIR       += -L$(LCT_SVC_PRJ_ROOT)/src/3rdparty/openssl/lib

SYS_LIB           += -lgrpc++ -lgrpc -lgrpc++_reflection -lgpr -lprofiler -lcares -lunwind
SYS_LIB           += -lprotobuf -lssl -lcrypto -pthread -ldl

DEP_OBJ           += $(LIB_DIR)/libcassandra_wrapper.a 
DEP_OBJ           += $(LCT_SVC_PRJ_ROOT)/src/3rdparty/cassandra/lib/libcassandra_static.a
DEP_OBJ           += $(LCT_SVC_PRJ_ROOT)/src/3rdparty/uv/lib/libuv.a
DEP_OBJ           += $(LIB_DIR)/liblct_protobuf.a $(LIB_DIR)/liblct_log.a $(LIB_DIR)/liblct_common.a 

TARGET            := lct-id-service

include $(LCT_SVC_PRJ_ROOT)/src/common/common.mk
