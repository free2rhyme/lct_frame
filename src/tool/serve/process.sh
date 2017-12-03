#!/bin/bash -i 

ulimit -c unlimited

#set -x  

USAGE="Usage: <./script-name> <command(start/stop/restart)> <process-name/all>"

# Source function library.
. /etc/init.d/functions

kill_config=15

BASE_DIR=$( cd "$( dirname "$0" )" && pwd )

export LD_LIBRARY_PATH=${BASE_DIR}/lib
export LCT_PROCESSES=$(cat ${BASE_DIR}/script/process.type)

start()
{
    echo "Starting $1..."
    PROG_STATUS=`status ${1}`
    RETVAL=$?
    if [ $RETVAL -eq 0 ]; then
        #If any previous instance of the process is running, kill it and start afresh   
        /usr/bin/kill -${kill_config} $1 >> /dev/null 2>&1
    fi
    
    local running_process=`(/usr/bin/ps -ef | grep "$1" | grep -v grep | awk '{print $2}')`
    if [ "" != "${running_process}" ]; then 
        /usr/bin/ps -ef | grep "$1" | grep -v grep | awk '{print $2}' | xargs kill -9;
    fi
    
    nohup ${BASE_DIR}/bin/$1 -f ${BASE_DIR}/config/lct_service_config.properties 2>&1 &
}

stop()
{
    echo "Stopping $1..."
    /usr/bin/kill -${kill_config} $1 >> /dev/null 2>&1
    
    local running_process=`(/usr/bin/ps -ef | grep "$1" | grep -v grep | awk '{print $2}')`
    if [ "" != "${running_process}" ]; then 
        /usr/bin/ps -ef | grep "$1" | grep -v grep | awk '{print $2}' | xargs kill -9;
    fi
}

restart()
{
    stop $1
    start $1
}

error_check()
{
    local is_found = false;

    for i in ${LCT_PROCESSES[@]}
    do
        if [ "$i" == "$1" ] && [ "$i" -ne "" ]; then
            is_found=true
        fi
    done

    if is_found != true; then
        echo "Error:Invalid Processes"
        exit 1
    fi
}

start_all() 
{
    for svc in ${LCT_PROCESSES}
    do
        start ${svc}
    done
}

stop_all() 
{
    for svc in ${LCT_PROCESSES}
    do
        stop ${svc}
    done
}

restart_all() 
{
    for svc in ${LCT_PROCESSES}
    do
        stop ${svc}
        start ${svc}
    done
}

status_all() 
{
    for svc in ${LCT_PROCESSES}
    do
        status ${svc}
    done
}

# Script starts here...
if [ "$1" = "start" ] && [ "$2" != "all" ]; then    
    error_check $2
    start $2

elif [ "$1" = "stop" ] && [ "$2" != "all" ]; then
    error_check $2
    stop $2

elif [ "$1" = "restart" ] && [ "$2" != "all" ]; then
    error_check $2
    restart $2

elif [ "$1" = "status" ] && [ "$2" != "all" ]; then
    error_check "$2"
    status "$2"

elif [ "$1" = "start" ] && [ "$2" = "all" ]; then
    start_all

elif [ "$1" = "stop" ] && [ "$2" = "all" ]; then
    stop_all
    
elif [ "$1" = "restart" ] && [ "$2" = "all" ]; then
    restart_all

elif [ "$1" = "status" ] && [ "$2" = "all" ]; then
    status_all

else
    echo "Error " $USAGE
    exit 1
fi
