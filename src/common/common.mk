######################################################################
 # @copyright    Copyright (C), 2017
 # @file         common.mk
 # @version      1.0
 # @date         Jun 8, 2017 5:18:16 PM
 # @author       wlc2rhyme@gmail.com
 # @brief        TODO
 #####################################################################

CXX             = g++
CPP_FLAGS       += -g -O3 --std=c++11

ifndef SRC_DIR
SRC_DIR := ./
endif

VPATH           := $(SRC_DIR)
CPPS            := $(foreach dir, $(VPATH), $(wildcard $(dir)/*.$(SRC_SUFFIX)))
OBJS            := $(patsubst %.$(SRC_SUFFIX), %.o, $(notdir $(CPPS)))
DEPS            := $(patsubst %.$(SRC_SUFFIX), %.d, $(notdir $(CPPS)))

CUR_SUB_DIRS   = $(subst src, ,$(CURDIR))
CUR_SUFFIX_DIR = $(lastword $(CUR_SUB_DIRS))
ROOT_SUB_DIRS  = $(subst src, ,$(LCT_SVC_PRJ_ROOT))
ROOT_PRE_DIR   = $(word $(words $(ROOT_SUB_DIRS)), $(ROOT_SUB_DIRS))
BUILD_OBJ_DIR := $(ROOT_PRE_DIR)/build/obj$(CUR_SUFFIX_DIR)

ifeq ($(TARGET_TYPE), app)
    FIXED_TARGET_DIR    := $(ROOT_PRE_DIR)/build/bin
else ifeq ($(TARGET_TYPE), lib)
    FIXED_TARGET_DIR    := $(ROOT_PRE_DIR)/build/lib
else
    FIXED_TARGET_DIR    := $(BUILD_OBJ_DIR)
endif

FIXED_TARGET        :=$(addprefix $(FIXED_TARGET_DIR)/, $(TARGET))
FIXED_OBJS          :=$(addprefix $(BUILD_OBJ_DIR)/, $(OBJS))
FIXED_DEPS          :=$(addprefix $(BUILD_OBJ_DIR)/, $(DEPS))

LIB_DIR       += $(ROOT_PRE_DIR)/build/lib
INC_DIR       += -I$(LCT_SVC_PRJ_ROOT)/src/common

all debug release build:$(FIXED_OBJS)

ifneq ($(FIXED_TARGET_DIR), $(wildcard $(FIXED_TARGET_DIR)))
	@mkdir -p $(FIXED_TARGET_DIR)
endif

	@echo 'Building target: $@'
ifeq ($(TARGET_TYPE), app)
	$(CXX) $^ $(DEP_OBJ) $(SYS_LIB_DIR) $(SYS_LIB) -o $(FIXED_TARGET) 
else ifeq ($(TARGET_TYPE), lib)
	ar -r $(FIXED_TARGET) $(FIXED_OBJS)
else 
.PHONY: $(TARGET)
endif
	@echo 'Finished building target: $@'
	@echo ' '

-include $(FIXED_DEPS)

$(BUILD_OBJ_DIR)/%.o:%.$(SRC_SUFFIX)

ifneq ($(BUILD_OBJ_DIR), $(wildcard $(BUILD_OBJ_DIR)))
	@mkdir -p $(BUILD_OBJ_DIR)
endif

	@echo "Start building $@"
	$(CXX) $(CPP_FLAGS) $(INC_DIR) -o $@ -c $< -MMD -MF"$(@:%.o=%.d)"
	@echo "Finished building $@"
	@echo " "

clean: reclean
	rm -fr $(BUILD_OBJ_DIR)
	@echo " "

reclean:
	rm -f $(FIXED_OBJS)
	rm -f $(FIXED_DEPS)
	rm -f $(FIXED_TARGET)
	@echo " "

rebuild:reclean all

.PHONY:clean reclean rebuild


