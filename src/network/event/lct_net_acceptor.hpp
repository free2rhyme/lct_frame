/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_acceptor.hpp
 * @version     1.0
 * @date        Aug 18, 2017 9:52:20 AM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/
#ifndef SRC_NETWORK_LCT_NET_ACCEPTOR_HPP_
#define SRC_NETWORK_LCT_NET_ACCEPTOR_HPP_

#include "lct_log.h"
#include "socket/lct_base_socket.h"

//template <typename AcceptHandler>
//LCT_ERR_CODE CLctNetAcceptor::accept(AcceptHandler&& handler)
//{
//    LCT_ERR_CODE errCode = LCT_SUCCESS;
//
//    LOG_DEBUG_MARK();
//
//    if (!isInitiated()) {
//        LOG_WARNG<< "Acceptor has NOT been initiated yet";
//        errCode = init();
//        if(LCT_SUCCESS != errCode) {
//            LOG_ERROR << "Failed to init acceptor";
//            return errCode;
//        }
//    }
//
//    handler();
//
//    return errCode;
//}

//template <typename AcceptHandler>
//LCT_ERR_CODE CLctNetAcceptor::asyncAccept(AcceptHandler&& handler)
//{
//    LCT_ERR_CODE errCode = LCT_SUCCESS;
//
//    LOG_DEBUG_MARK();
//
//    return errCode;
//}

#endif /* SRC_NETWORK_LCT_NET_ACCEPTOR_HPP_ */
