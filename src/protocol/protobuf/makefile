

ifndef $(LCT_SVC_PRJ_ROOT)
ROOT_DIR_SUFFIX_PART := $(lastword  $(subst src, ,$(CURDIR)))
LCT_SVC_PRJ_ROOT     := $(firstword $(subst /src$(ROOT_DIR_SUFFIX_PART), ,$(CURDIR)))
endif

GRPC_HOME_PATH := $(LCT_SVC_PRJ_ROOT)/src/3rdparty/grpc
GRPC_BIN_PATH  := $(GRPC_HOME_PATH)/bin
GRPC_LIB_PATH  := $(GRPC_HOME_PATH)/lib

SRC_SUFFIX       := pb.cc
HERDER_SUFFIX    := pb.h

PROTOS        := $(wildcard *.proto)
OUT_DIR       := $(abspath $(dir $(lastword $(MAKEFILE_LIST)))/../protobufcpp)
OBJS          := $(PROTOS:%.proto=$(OUT_DIR)/%.$(SRC_SUFFIX))

export LD_LIBRARY_PATH := $(GRPC_LIB_PATH):$(LD_LIBRARY_PATH)

gRPC_CPP_PLUGIN_EXECUTABLE := $(GRPC_BIN_PATH)/grpc_cpp_plugin
gPROTOBUF_EXECUTABLE       := $(GRPC_BIN_PATH)/protoc

all debug release build: ${OBJS}

$(OUT_DIR)/%.$(SRC_SUFFIX): %.proto
	$(gPROTOBUF_EXECUTABLE) --cpp_out $(@D) $<
	$(gPROTOBUF_EXECUTABLE) --grpc_out $(@D) --plugin=protoc-gen-grpc=$(gRPC_CPP_PLUGIN_EXECUTABLE) $<

clean:
	rm -f $(OUT_DIR)/*.$(SRC_SUFFIX)
	rm -f $(OUT_DIR)/*.$(HERDER_SUFFIX)

rebuild:clean all

.PHONY:clean rebuild

