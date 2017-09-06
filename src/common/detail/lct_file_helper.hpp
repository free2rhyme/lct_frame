/**********************************************************************
 * @copyright    Copyright (C), 2017
 * @file         lct_file_helper.hpp
 * @version      1.0
 * @date         Jun 9, 2017 3:15:08 PM
 * @author       wlc2rhyme@gmail.com
 * @brief        TODO
 *********************************************************************/
#ifndef SRC_COMMON_LCT_FILE_HELPER_HPP_
#define SRC_COMMON_LCT_FILE_HELPER_HPP_

template <typename T>
LCT_ERR_CODE CFileHelper::write(const T& val)
{
    if (!m_fd) {
        return LCT_INVALID_FD;
    }
    try {
        if (m_isBinary) {
            if (std::fwrite(&val, sizeof(T), 1, m_fd) != 1){
                return LCT_FAIL;
            }
        } else {
            WriteTextValue(val);
        }
    } catch (const std::exception& e) {
        return LCT_UNEXPECTED;
    } catch (...) {
        return LCT_UNEXPECTED;
    }
    return LCT_SUCCESS;
}

template <typename T>
std::size_t CFileHelper::writeTextValue(const T& val)
{
    return std::fwrite(&val, sizeof(T), 1, m_fd);
}

template <>
LCT_ERR_CODE CFileHelper::write<const char*>(const char* const& msg);

template <>
LCT_ERR_CODE CFileHelper::write<std::string>(const std::string& msg);

template <>
LCT_ERR_CODE CFileHelper::write<std::shared_ptr<std::string>>(const std::shared_ptr<std::string>& msg);

template <>
LCT_ERR_CODE CFileHelper::write<std::shared_ptr<const std::string>>(const std::shared_ptr<const std::string>& msg);

template <>
std::size_t CFileHelper::writeTextValue<uint32_t>(const uint32_t& val);

template <>
std::size_t CFileHelper::writeTextValue<uint64_t>(const uint64_t& val);

template <>
std::size_t CFileHelper::writeTextValue<int32_t>(const int32_t& val);

template <>
std::size_t CFileHelper::writeTextValue<int64_t>(const int64_t& val);

template <>
std::size_t CFileHelper::writeTextValue<float>(const float& val);


#endif /* SRC_COMMON_LCT_FILE_HELPER_HPP_ */
