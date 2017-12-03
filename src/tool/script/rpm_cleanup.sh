#!/bin/bash

set -e  
set -u 
#set -x  

CUR_DIR=$( cd "$( dirname "$0" )" && pwd )
BASE_DIR=${CUR_DIR}/../../..

rm -fr ${BASE_DIR}/build/obj/*
rm -fr  ${BASE_DIR}/build/rpm/*
