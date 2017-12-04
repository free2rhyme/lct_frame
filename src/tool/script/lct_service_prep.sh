#!/bin/bash

#set -e  
#set -u 
#set -x  

CUR_DIR=$( cd "$( dirname "$0" )" && pwd )
BASE_DIR=${CUR_DIR}/..

export LD_LIBRARY_PATH=${BASE_DIR}/lib

if [ -n $1 ] && [ -n $2 ]; then
    ${BASE_DIR}/bin/iva-id-table-prep $1 $2
else 
    ${BASE_DIR}/bin/iva-id-table-prep
fi

## source environment variables and library functions

