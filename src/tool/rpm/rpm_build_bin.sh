
prepare_bin() 
{
    local bin_exec=${1}
    local bin_path=
    if [ -x $BUILD_BIN_DIR/$bin_exec ]; then
        cp $BUILD_BIN_DIR/$bin_exec $PREBUILD_BIN_DIR
    else
        fatal "This binary $bin_exec could't be found"
    fi
}

