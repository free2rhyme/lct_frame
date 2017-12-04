
prepare_script() 
{
    if [ -f ${PROCESS_FILE} ]; then
        rm -f ${PROCESS_FILE}
    fi
    
    cp ${RPMTYPES_FILE} ${PROCESS_FILE}
    
    if [ -f ${PREBUILD_DIR}/process.sh ]; then
        rm -f ${PREBUILD_DIR}/process.sh
    fi
    cp ${LCT_SVC_PRJ_ROOT}/src/tool/serve/process.sh ${PREBUILD_DIR}/
    cp ${LCT_SVC_PRJ_ROOT}/src/tool/script/lct_service_prep.sh ${PREBUILD_SCRIPT_DIR}/
    cp ${BUILD_BIN_DIR}/iva-id-table-prep ${PREBUILD_BIN_DIR}/
    cp ${LCT_SVC_PRJ_ROOT}/src/tool/script/install.sh ${PREBUILD_DIR}/
    cp ${LCT_SVC_PRJ_ROOT}/src/tool/script/uninstall.sh ${PREBUILD_DIR}/
    cp ${LCT_SVC_PRJ_ROOT}/src/tool/script/lct_monitor.sh ${PREBUILD_SCRIPT_DIR}/
    cp ${LCT_SVC_PRJ_ROOT}/src/tool/script/lct_log_rotate.sh ${PREBUILD_SCRIPT_DIR}/
    cp ${LCT_SVC_PRJ_ROOT}/src/tool/script/version.sh ${PREBUILD_DIR}/
}
