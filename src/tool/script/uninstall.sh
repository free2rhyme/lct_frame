#!/bin/bash -i

CUR_DIR=$( cd "$( dirname "$0" )" && pwd )
BASE_DIR=${CUR_DIR}/..

cronfile="crontab.${USER}"

crontab -l > ${cronfile}
if [ $? -ne 0 ];then
    echo "Failed to query crontab."
    exit 1;
fi

sed -i '/script\/lct_monitor.sh/d'    ${cronfile}
sed -i '/script\/lct_log_rotate.sh/d' ${cronfile}
if [ $? -ne 0 ];then
    echo "Failed to remove item from crontab-file."
    exit 1;
fi 

crontab ${cronfile}
if [ $? -ne 0 ];then
    echo "Failed to reset crontab."
    exit 1;
fi

rm -fr ${cronfile}

echo "Successful to uninstall lowcost service"
