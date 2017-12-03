/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        lct_error_code.cpp
 * @version     1.0
 * @date        Jul 21, 2017 4:18:52 PM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/

#include "lct_error_code.h"

#include <unordered_map>
#include <sstream>

typedef std::unordered_map<LCT_ERR_CODE, std::string> LctErrInfoMapType;

static const LctErrInfoMapType LctErrInfoMap =
{
   {LCT_SUCCESS,            "Success"                  },
   {LCT_FAIL,               "General Failure"          },
   {LCT_INVALID_PARAM,      "Invalid Parameter"        },
   {LCT_DUPLICATE_REQUEST,  "Duplicate Request"        },
   {LCT_INVALID_POINTER,    "Invalid Pointer"          },
   {LCT_OVERFLOW,           "Overflow Error"           },
   {LCT_IMPROPER_STATUS,    "Improper status"          },
   {LCT_OUT_OF_BOUNDARY,    "Out of Boundary Access"   },
   {LCT_INVALID_FD,         "Invalid File Description" },
   {LCT_EMPTY_RECORD,       "Empty Record"             },
   {LCT_UNIMPLEMENTED,      "Unimplemented Function"   },
   {LCT_UNEXPECTED,        "Unexpected Result"         }
};

std::string ErrCodeFormat(const LCT_ERR_CODE errCode)
{
   std::stringstream stream;

   auto itRe = LctErrInfoMap.find(errCode);
   if (LctErrInfoMap.end() != itRe) {
      stream << " error(" << itRe->second << ")";
   }
   stream << std::hex << " errorCode(0x" << errCode << std::dec << ")";
   return stream.str();
}



