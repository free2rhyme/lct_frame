

handle_so_line() 
{

    local so_line=${1}
    
    local column_count=`(echo "${so_line}" | awk '{ print NF }')`
    
    declare -a so_libs=`(echo "${so_line}" | awk '{ for(i=3;i<="'"$column_count"'";++i) print $i }')`
    
    declare -a array;
    for so in "${so_libs[@]}"
    do
        array=`(echo "${so}" | sed 's/\-l/lib/g' | sed 's/\-/lib/g' | sed 's/$/&.so/g')`
    done
    
    for so in "${array[@]}"
    do
        so_array="$so_array ${so}"
    done  
}

handle_so_define() 
{
    local old_ifs=$IFS
    IFS=\n
    local so_lines_name=$1[@]
    local so_lines=("${!so_lines_name}")
    for sl in "${so_lines[@]}"
    do
        handle_so_line "${sl}"
    done
    IFS=$old_ifs
}

handle_so_dir_line() 
{

    local so_dir_line="${1}"
    
    local column_count=`(echo "${so_dir_line}" | awk '{ print NF }')`
    
    declare -a so_libs=`(echo "${so_dir_line}" | awk '{ for(i=3;i<="'"$column_count"'";++i) print $i }')`
    
    declare -a array;
    for so in "${so_libs[@]}"
    do
        array=`(echo "${so}" | sed 's/\-L$(LCT_SVC_PRJ_ROOT)//g')`
    done
    
    for so in "${array[@]}"
    do
        so_dir_array="$so_dir_array ${so}"
    done
   
}

handle_so_dir_define() 
{
    local old_ifs=$IFS
    IFS=\n
    local so_dir_lines_name=$1[@]
    so_dir_lines=("${!so_dir_lines_name}")
    for sl in "${so_dir_lines[@]}"
    do
        handle_so_dir_line "$sl"
    done
    IFS=${old_ifs}
}

prepare_lib() 
{
    local so_dir_array_name=$1[@]
    local so_dir_array=("${!so_dir_array_name}")
    local so_array_name=$2[@]
    local so_array=("${!so_array_name}")
    
    for dir in ${so_dir_array[@]}
    do
        local abs_dir=$LCT_SVC_PRJ_ROOT$dir

        for so in ${so_array[@]}
        do
            local file_path=`(echo "${abs_dir}"/"${so}" | sed 's/ //g')`;
            
            if [ -f $file_path ]; then
                for file in `ls $file_path*`
                do
                    if [ -L $file ]; then
                        cp -P $file $PREBUILD_LIB_DIR
                    else
                        cp $file $PREBUILD_LIB_DIR
                    fi
                done
            fi
        done
    done
}






