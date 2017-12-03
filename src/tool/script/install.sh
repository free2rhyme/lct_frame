#!/bin/bash -i 

CUR_DIR=$( cd "$( dirname "$0" )" && pwd )
BASE_DIR=${CUR_DIR}/..

cronfile="crontab.${USER}"

crontab -l > ${cronfile}

grep -q "${CUR_DIR}/script/monitor.sh" ${cronfile}
if [ $? -eq 0 ]; then
    echo "There is already an installed item in crontab"
    exit 1
fi

echo "*/1 * * * * ${CUR_DIR}/script/lct_monitor.sh >/dev/null 2>&1 "   >> ${cronfile}
echo "10 1 * * * ${CUR_DIR}/script/lct_log_rotate.sh >/dev/null 2>&1 " >> ${cronfile}
if [ $? -ne 0 ];then
    echo "Failed to add item to crontab-file."
    exit 1;
fi 

crontab ${cronfile}
if [ $? -ne 0 ];then
    echo "Failed to reset crontab."
    exit 1;
fi

rm -rf ${cronfile}

echo "Successful to install lowcost service"


