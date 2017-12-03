/**********************************************************************
 * @copyright   Copyright (C), 2017
 * @file        version_manager.cpp
 * @version     1.0
 * @date        Oct 21, 2017 8:35:17 PM
 * @author      wlc2rhyme@gmail.com
 * @brief       TODO
 *********************************************************************/

#include <sstream>
#include <fstream>
#include <iostream>
#include "version_manager.h"

std::vector<std::string> Split(const std::string& str, const char* delim = " ")
{
    std::vector<std::string> parsed;
    std::string::size_type pos = 0;
    while (true){
        const std::string::size_type colon = str.find(delim, pos);
        if (colon == std::string::npos){
            parsed.push_back(str.substr(pos));
            break;
        } else {
            parsed.push_back(str.substr(pos, colon - pos));
            pos = colon + 1;
        }
    }
    return std::move(parsed);
}

std::string Trim(const std::string& str, const std::string& delc = " \t\r\n")
{
    const size_t pos = str.find_first_not_of(delc);
    if (std::string::npos == pos){
        return "";
    }

    return str.substr(pos, str.find_last_not_of(delc) - pos + 1);
}

static bool IsVersionInfoLine(const std::string& line)
{
    const std::string::size_type colon = line.find("constexpr");
    return colon != std::string::npos;
}

CVersionParser* CVersionParser::instance()
{
    static CVersionParser _instance;
    return &_instance;
}


bool CVersionParser::init(const std::string& versionFile)
{
    m_versionFile = versionFile;

    bool errCode = parse();
    if (true != errCode) {
        std::cout << "Failed to parse version file" << std::endl;
        return errCode;
    }

    return errCode;
}

bool CVersionParser::parse()
{
    bool errCode  = true;
    std::ifstream ifs;
    do {
        try {
            ifs.open(m_versionFile, std::ios::in);

            if (!ifs.good()) {
                std::cout << "Failed to open file(" << m_versionFile << ")" << std::endl;
                errCode = false;
                break;
            }

            char buf[512] = {0};
            while (ifs.getline(buf, sizeof(buf))) {
                std::istringstream line(Trim(buf));
                std::string key;
                if (std::getline(line, key, '=')) {
                    if (IsVersionInfoLine(key)) {
                        std::string value;
                        if (std::getline(line, value)) {
                            m_versionMap.emplace_back(key, std::stoi(Trim(value, " \t\r\n;")));
                        }
                    }
                }
            }
        } catch (std::exception& e) {
            std::cout << " e.what(" << e.what() << ")" << std::endl;
            errCode = false;
        } catch (...) {
            errCode = false;
        }
    }while(0);

    if (ifs.is_open()) {
       ifs.close();
    }

    return errCode;
}


bool CVersionParser::autoVersion()
{
    bool errCode = true;

    auto it = m_versionMap.rbegin();
    if (m_versionMap.rend() == it) {
        std::cout << "Version Info is empty" << std::endl;
        return false;
    }
    it->Value++;

    errCode = persist();
    if (true != errCode) {
        std::cout << "Failed to save version info to file" << std::endl;
        return errCode;
    }

    return errCode;
}

bool CVersionParser::forceVersion(const std::string& vs)
{
    bool errCode = true;

    std::vector<std::string> values = Split(vs, ".");

    if (values.size() != m_versionMap.size()) {
        std::cout << "vs(" << vs << ") is invalid" << std::endl;
        return false;
    }

    for(std::size_t i = 0; i < values.size(); ++i) {
        CVersionInfo& tmp =  m_versionMap.at(i);
        tmp.Value = std::stoi(values.at(i));
    }

    errCode = persist();
    if (true != errCode) {
        std::cout << "Failed to save version info to file" << std::endl;
        return errCode;
    }

    return errCode;
}

bool CVersionParser::version() const
{

    bool errCode = true;
    std::stringstream ss;
    for (auto itRe = m_versionMap.cbegin(); itRe != m_versionMap.cend(); ++itRe) {
        const CVersionInfo& ref = *itRe;
        if (m_versionMap.cbegin() != itRe) {
            ss << ".";
        }
        ss << ref.Value;
    }

    std::cout << ss.str() << "\n" << std::endl;

    return errCode;
}


bool CVersionParser::persist() const
{
    bool errCode = true;

    const std::string backUpFile = m_versionFile + "-bakup";

    std::ifstream ifs;
    std::ofstream ofs;
    do {
        try {
            ifs.open(m_versionFile, std::ios::in);
            ofs.open(backUpFile, std::ios::out);

            if (!ifs.good()) {
                errCode = false;
                break;
            }

            if ( !ofs.good()) {
                errCode = false;
                break;
            }

            char buf[512] = {0};
            auto itRe = m_versionMap.begin();
            while (ifs.getline(buf, sizeof(buf))) {
                std::istringstream line(Trim(buf));
                std::string key;
                if (std::getline(line, key)){
                    if (IsVersionInfoLine(key)) {
                        ofs << itRe->Key << "= " << std::to_string(itRe->Value) << ";\n";
                        ++itRe;
                    } else {
                        ofs << key << "\n";
                    }
                } else {
                    ofs << "\n";
                }
            }

            ofs.flush();

        } catch (std::exception& e) {
            std::cout << " e.what(" << e.what() << ")";
            errCode = false;
        } catch (...) {
            errCode = false;
        }
    }while(0);

    if (ifs.is_open()) {
       ifs.close();
    }

    if (ofs.is_open()) {
        ofs.close();
    }

    if (std::remove(m_versionFile.c_str()) != 0) {
        std::cout << "Failed to remove file(" << m_versionFile << ")";
        return false;
    }

    if (std::rename(backUpFile.c_str(), m_versionFile.c_str()) != 0) {
        std::cout << "Failed to rename file(" << backUpFile << ")";
        return false;
    }

    return errCode;
}


CVersionParser::CVersionParser()
{
}

CVersionParser::~CVersionParser()
{
}

