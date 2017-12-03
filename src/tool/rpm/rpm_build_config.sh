

prepare_config() 
{
    if [ -f $LCT_CONFIG_FILE ]; then
        cp $LCT_CONFIG_FILE $PREBUILD_CONFIG_DIR
    else
        fatal "This config file $LCT_CONFIG_FILE could't be found"
    fi
}

