/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        service_main.cpp
 * @version     1.0
 * @date        Jun 30, 2017 7:21:58 PM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/

#include <iostream>
#include <signal.h>

#include "id_config_mgr.h"
#include "id_service_mgr.h"
#include "lct_common_util.h"
#include "lct_log.h"

static void usage(const char* executive)
{
    std::cout << "usage:" << "\n\t";
    std::cout << executive << "                    # service booting up with default config_file\n\t";
    std::cout << executive << " -f config_file     # service booting up with specified config_file\n\t";
    std::cout << executive << " --version          # print out current version\n\t" << std::endl;
}

static LCT_ERR_CODE argvHandle(int argc, char* argv[], std::string& configFilePath)
{
    LCT_ERR_CODE errCode = LCT_SUCCESS;

    switch(argc) {
        case 1: {
            break;
        }
        case 2: {
            if (std::string(argv[1]) != std::string("--version")) {
                errCode = LCT_FAIL;
                break;
            }
            errCode = LctVersion();
            break;
        }
        case 3: {
            if (std::string(argv[1]) != std::string("-f")) {
                errCode = LCT_FAIL;
                break;
            }
            configFilePath = argv[2];
            break;
        }
        default: {
            errCode = LCT_FAIL;
        }
    }

    if (LCT_SUCCESS != errCode) {
        return errCode;
    }

    return errCode;
}

static void InterruptedCallback(int signal){
    std::cout << __LINE__ << "\t InterruptedCallback is triggered......" << std::endl;

    LCT_SID_MGR->shutdown();

    std::cout << __LINE__ << "\t InterruptedCallback is closing......" << std::endl;
}

static void InitSignalHandle(){
    signal(SIGPIPE, InterruptedCallback);
    signal(SIGHUP, InterruptedCallback);
    signal(SIGQUIT, InterruptedCallback);
    signal(SIGTERM, InterruptedCallback);
    signal(SIGINT, InterruptedCallback);
}

int main(int argc, char* argv[]){
    LCT_ERR_CODE errCode = LCT_SUCCESS;

    std::string configFilePath = "";
    errCode = argvHandle(argc, argv, configFilePath);
    if (LCT_SUCCESS != errCode) {
        if (LCT_VERSION_INFO == errCode) {
            return 0;
        }
        usage(argv[0]);
        return errCode;
    }

    std::cout << std::endl << "**********************lowcost IVA Unique Id Service**********************" << std::endl;
    std::cout << "Booting up lowcost IVA Unique Id Service ......" << std::endl;

    InitSignalHandle();

    std::cout << "Start initiating lowcost IVA Unique Id Service......" << std::endl;

    /**************** Config Manager ****************/
    if (configFilePath.empty()) {
        errCode = SID_CONFIG_MGR->init();
    } else {
        errCode = SID_CONFIG_MGR->init(configFilePath);
    }

    if(LCT_SUCCESS != errCode){
        LOG_ERROR << "Failed to init IVA Log System due to error(" << ErrCodeFormat(errCode) << ")";
        return errCode;
    } else {
        std::cout << "Successful to init SID Config Manager" << std::endl;
    }

    /**************** Log System ****************/
    errCode = LCT_LOG->init(SID_CONFIG_MGR->logDir());
    if(LCT_SUCCESS != errCode){
        LOG_ERROR << "Failed to init IVA Log System due to error(" << ErrCodeFormat(errCode) << ")";
        return errCode;
    } else {
        std::cout << "Successful to init IVA Log System" << std::endl;
    }

    LOG_INFOR << "**********************lowcost IVA Unique Id Service**********************";
    LOG_INFOR << "Booting up lowcost IVA Unique Id Service ......";
    LOG_INFOR << "Start initiating lowcost IVA Unique Id Service......";
    LOG_INFOR << "Successful to init SID Config Manager";
    LOG_INFOR << "Successful to init IVA Log System";

    /**************** IVA Unique Id Service ****************/
    errCode = LCT_SID_MGR->init();
    if(LCT_SUCCESS != errCode){
        std::cout << __PRETTY_FUNCTION__ << "\t" << __LINE__<< "\tFailed to initiate SID service manager" << std::endl;
        return errCode;
    } else {
        std::cout << "lowcost IVA Unique Id Service finished initiation and start running......" << std::endl;
        LOG_INFOR << "lowcost IVA Unique Id Service finished initiation and start running......";
    }

    errCode = LCT_SID_MGR->run();

    std::cout << "Shutting down lowcost IVA Unique Id Service......" << std::endl;
    LOG_INFOR << "Shutting down lowcost IVA Unique Id Service......";

    if(LCT_SUCCESS != errCode){
        std::cout << __PRETTY_FUNCTION__ << "\t" << __LINE__<< "\t Error happened during shutting down service manager" << std::endl;
        return errCode;
    } else {
        std::cout << "lowcost IVA Unique Id Service is shutdown and exiting" << std::endl;
    }

    errCode = LCT_LOG->close();
    if(LCT_SUCCESS != errCode){
        std::cout << __PRETTY_FUNCTION__ << "\t" << __LINE__<< "\t Error happened during shutting down IVA Log System" << std::endl;
        return errCode;
    } else {
        std::cout << "lowcost Log System is shutdown and exiting" << std::endl;
    }

    return 0;
}




