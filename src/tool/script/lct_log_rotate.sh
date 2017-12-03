#!/bin/bash

#set -x  

CUR_DIR=$( cd "$( dirname "$0" )" && pwd )
BASE_DIR=${CUR_DIR}/..
LOG_DIR=${BASE_DIR}/log

YESTERDAY=`date +"%Y-%m-%d" -d "-1day"`

ROTATE_KEY_WORD=${YESTERDAY}

LOG_FILE_SUFFIX=".log"
LOG_TAR_SUFFIX=".tar.gz"

log_rotate_each_file()
{
    local svc_name="${1}"
    local svc_log_file="${2}"

    local abs_svc_dir=${LOG_DIR}/${svc_name}
    
    local svc_file_src=${LOG_DIR}/${svc_log_file}
    local svc_file_dest=${abs_svc_dir}/${svc_log_file}

    mv ${svc_file_src} ${svc_file_dest}
    if [ $? -ne 0 ]; then
        echo "Failed to move ${svc_log_file}."
        exit 1;
    fi 
    
    local svc_log_tar_name=`echo "${svc_log_file}" | sed "s/${LOG_FILE_SUFFIX}/${LOG_TAR_SUFFIX}/g"`

    cd ${abs_svc_dir}
    
    tar -czpf ${svc_log_tar_name} ${svc_log_file}
    if [ $? -ne 0 ]; then
        echo "Failed to tar ${svc_file_dest}."
        exit 1;
    fi 
    
    rm -f ${svc_file_dest}
}

log_rotate_each_svc()
{
    local service="${1}"
    
    local abs_svc_dir=${LOG_DIR}/${service}
    
    if [ ! -d ${abs_svc_dir} ]; then
        mkdir -p ${abs_svc_dir}
        if [ $? -ne 0 ]; then
            echo "Failed to mkdir ${abs_svc_dir}."
            exit 1;
        fi 
    fi
    
    local svc_log=${service}_${ROTATE_KEY_WORD}
    
    cd ${LOG_DIR}
    declare -a log_files=($(ls -l ${svc_log}*${LOG_FILE_SUFFIX} 2>>/dev/null | awk '{ print $NF }'))
    
    log_num=${#log_files[@]}
    if [ 0 -eq ${log_num} ]; then
        echo "There is no log file for ${YESTERDAY}"
        exit 0;
    fi
    
    for log_file in "${log_files[@]}"
    do 
        log_rotate_each_file ${service} ${log_file}
    done
}


log_rotate()
{
    cd ${LOG_DIR}
    declare -a services=($(ls -l *${ROTATE_KEY_WORD}*${LOG_FILE_SUFFIX} 2>>/dev/null | awk '{ print $NF }' | sed "s/_${ROTATE_KEY_WORD}.*${LOG_FILE_SUFFIX}//g" | uniq))

    svc_num=${#services[@]}
    if [ 0 -eq ${svc_num} ]; then
        echo "There is no service log file for ${YESTERDAY}"
        exit 0;
    fi
    
    for service in "${services[@]}"
    do 
        log_rotate_each_svc ${service}
    done
}

log_rotate

