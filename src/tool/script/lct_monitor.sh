#!/bin/bash

ulimit -c unlimited

#set -x  

CUR_DIR=$( cd "$( dirname "$0" )" && pwd )
BASE_DIR=${CUR_DIR}/..

export LD_LIBRARY_PATH=${BASE_DIR}/lib
export LCT_PROCESSES=$(cat ${BASE_DIR}/script/process.type)

start()
{
    echo "Starting $1..."

    local running_process=`(/usr/bin/ps -ef | grep "$1" | grep -v grep | awk '{print $2}')`
    if [ "" != "${running_process}" ]; then 
        /usr/bin/ps -ef | grep "$1" | grep -v grep | awk '{print $2}' | xargs kill -9;
    fi
    
    nohup ${BASE_DIR}/bin/$1 -f ${BASE_DIR}/config/lct_service_config.properties >> ${BASE_DIR}/log/lowcost-monitor.log 2>&1 &
}

for svc in ${LCT_PROCESSES[@]}
do
    running_process=`(/usr/bin/ps -ef | grep "${svc}" | grep -v grep | awk '{print $2}')`
    if [ "" == "${running_process}" ]; then 
        current_time=`date`
        echo "${current_time} service(${svc}) is down, monitor is starting it now... " >> ${BASE_DIR}/log/lowcost-monitor.log
        start ${svc}
    fi
done


