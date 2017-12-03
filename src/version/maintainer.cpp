/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        maintainer.cpp
 * @version     1.0
 * @date        Oct 21, 2017 5:08:42 PM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/

#include <iostream>
#include "version_manager.h"


static constexpr const char* VERSION_COMMAND = "make version";

static void usage(const char* executive = VERSION_COMMAND)
{
    std::cout << "usage:" << "\n\t";
    std::cout << executive << "                          # show current version number\n\t";
    std::cout << executive << " force-version 1.0.0.0    # force set current version\n\t";
    std::cout << executive << " auto                     # set version number increment automatically \n\t";
    std::cout << executive << " help                     # show version tool usage\n\t"<< std::endl;
}

static bool argvHandle(int argc, char* argv[])
{
    bool errCode = true;

    switch(argc) {
        case 1:
            break;
        case 2: {
            if (std::string(argv[1]) != "auto") {
                errCode = false;
            } else {
                errCode = VERSION_MGR->autoVersion();
                if (true != errCode) {
                    std::cout << "Failed to auto set version number";
                }
            }
            break;
        }
        case 3: {
            if (std::string(argv[1]) != std::string("force-version")) {
                errCode = false;
                break;
            }
            errCode = VERSION_MGR->forceVersion(std::string(argv[2]));
            if (true != errCode) {
                std::cout << "Failed to force set version(" << argv[2] << ")";
            }
            break;
        }
        default: {
            errCode = false;
        }
    }

    if (true != errCode) {
        return errCode;
    }

    return errCode;
}

int main(int argc, char* argv[]){
    bool errCode = true;

    do {
        errCode = VERSION_MGR->init();
        if (true != errCode) {
            std::cout << "Failed to read version file";
            break;
        }

        errCode = argvHandle(argc, argv);
        if (true != errCode) {
            usage(VERSION_COMMAND);
            break;
        }
    } while (0);

    if (true == errCode) {
        VERSION_MGR->version();
    }

    return 0;
}

