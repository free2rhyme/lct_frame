#!/bin/bash

set -e  
set -u 
#set -x  

CUR_DIR=$( cd "$( dirname "$0" )" && pwd )
LCT_SVC_PRJ_ROOT=$CUR_DIR/../../..

declare -a so_array=""
declare -a so_dir_array=""

usage() 
{
    echo "usage: $(basename $0) [-release | -debug ] [-pre] [-publish] [targets]"
    exit 1
}

fatal() 
{
    echo "[Error] $1"
    exit 1
}

## source environment variables and library functions
. ${LCT_SVC_PRJ_ROOT}/src/tool/rpm/rpm_build.env
. ${LCT_SVC_PRJ_ROOT}/src/tool/rpm/rpm_build_lib.sh
. ${LCT_SVC_PRJ_ROOT}/src/tool/rpm/rpm_build_bin.sh
. ${LCT_SVC_PRJ_ROOT}/src/tool/rpm/rpm_build_config.sh
. ${LCT_SVC_PRJ_ROOT}/src/tool/rpm/rpm_build_script.sh

root_disallowed() 
{
    if [ $(whoami) = "root" ]
    then
        fatal "This script ($0) should not be run by root"
    fi
}


check_dir() 
{
    if [ -d "$PREBUILD_DIR" ]; then  
         rm -fr "$PREBUILD_DIR"  
    fi

    if [ ! -d "$PREBUILD_LIB_DIR" ]; then  
        mkdir -p "$PREBUILD_LIB_DIR"  
    fi
    
    if [ ! -d "$PREBUILD_BIN_DIR" ]; then  
        mkdir -p "$PREBUILD_BIN_DIR"  
    fi
    
    if [ ! -d "$PREBUILD_CONFIG_DIR" ]; then  
        mkdir -p "$PREBUILD_CONFIG_DIR"  
    fi
    
    if [ ! -d "$PREBUILD_DATA_DIR" ]; then  
        mkdir -p "$PREBUILD_DATA_DIR"  
    fi
    
    if [ ! -d "$PREBUILD_DOC_DIR" ]; then  
        mkdir -p "$PREBUILD_DOC_DIR"  
    fi 
    
    if [ ! -d "$PREBUILD_SCRIPT_DIR" ]; then  
        mkdir -p "$PREBUILD_SCRIPT_DIR"  
    fi

    if [ ! -d "$PREBUILD_LOG_DIR" ]; then  
        mkdir -p "$PREBUILD_LOG_DIR"  
    fi
}

handle_mk() 
{
    local mak_file=${1}
    
    local rpm_type_name=$2[@]
    
    local rpm_type=("${!rpm_type_name}")
    
    for rt in ${rpm_type[@]}
    do
        if grep -wq TARGET "${mak_file}" && grep -wq $rt "${mak_file}" ; then
            readarray -t so_lines < <(grep -w SYS_LIB "${mak_file}" | grep -v '#')
            handle_so_define so_lines[@]
            #echo $so_array
           
            readarray -t so_dir_lines < <(grep -w SYS_LIB_DIR "${mak_file}" | grep -v '#')
            handle_so_dir_define so_dir_lines[@]
            #echo $so_dir_array
            
            if [ 0 -ne ${#so_dir_array[@]} ]; then
                prepare_lib so_dir_array[@] so_array[@]
                prepare_bin $rt
            fi
        fi
    done
}

handle_mk_list() 
{
    for mf in ${MAKEFILES_LIST}
    do
        if grep -wq TARGET_TYPE "$mf" && grep -wq app "$mf" ; then
            handle_mk ${mf} RPMTYPES[@]
        fi
        
        prepare_config
        
    done
}

version=""

prepare_version() 
{
   
    declare -a versions=($(grep constexpr ${LCT_VERSON_FILE} | sed 's/ \{2,80\}/ /g' | awk -F '[ ;]' '{print $6}'))
    version_size=${#versions[@]}   
    
    if [ $version_size -eq 4 ]; then
        version=${versions[0]}.${versions[1]}.${versions[2]}.${versions[3]}
    else
        fatal "version file is invalid"
    fi
}

prepare_tar() 
{
    local tar_name=${RPM_TAR_NAME_PRE}_${version}.tar.gz
    
    echo "tarfile: $(pwd)/${tar_name}"
    
    cd ${LCT_SVC_PRJ_ROOT}/build/rpm
    
    if [ -f ${tar_name} ]; then
        rm -f ${tar_name}
    fi
    
    tar -czpf ${LCT_SVC_PRJ_ROOT}/build/rpm/${tar_name} `(basename ${PREBUILD_DIR})`
    rm -fr ${PREBUILD_DIR}
    
    echo "rpm ${tar_name} is successful built"
}

root_disallowed
check_dir
prepare_version
handle_mk_list
prepare_script
prepare_tar

