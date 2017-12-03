#!/bin/bash -i 


BASE_DIR=$( cd "$( dirname "$0" )" && pwd )

export LD_LIBRARY_PATH=${BASE_DIR}/lib
export LCT_PROCESSES=$(cat ${BASE_DIR}/script/process.type)

for i in ${LCT_PROCESSES[@]}
do
    echo "$i: "$(${BASE_DIR}/bin/$i --version)
done


