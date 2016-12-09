//
// Created by joppich on 12/6/16.
//

#ifndef BIOGUI_LOGGING_H
#define BIOGUI_LOGGING_H

#include <string>
#include <iostream>

class Logging {
public:
    enum LEVEL {ERR=0, INFO, DEBUG};


    static void setLoggingLevel(LEVEL eLevel)
    {
        m_eCurrentLoggingLevel = eLevel;
    }

    static void log( std::string sMessage)
    {

        Logging::log(sMessage, m_eCurrentLoggingLevel);

    }

    static void log( std::string sMessage, int iLevel)
    {
        Logging::log(sMessage, (LEVEL) iLevel);
    }

    static void log( std::string sMessage, LEVEL eLevel)
    {

        std::string sLevel = "ERROR";

        switch (eLevel)
        {
            ERR: sLevel = "ERROR"; break;
            DEBUG: sLevel = "DEBUG"; break;
            INFO: sLevel = "INFO"; break;
            default:
                sLevel = "Unknown";
            break;
        }

        if (eLevel == ERR)
        {

            if (eLevel <= Logging::m_eCurrentLoggingLevel)
            {
                std::cerr << "[" << sLevel << "] " << sMessage << std::endl;
            }

        } else {

            if (eLevel <= Logging::m_eCurrentLoggingLevel)
            {
                std::cout << "[" << sLevel << "] " << sMessage << std::endl;
            }

        }


    }

protected:



    static LEVEL m_eCurrentLoggingLevel;

};

#ifndef LOGLVL

//#define LOG(msg) Logging::log(std::string("") + " " + __FILE__ + "[" + std::to_string(__LINE__) + "]" + "\t" + std::string(msg));
#define LOGLVL(msg, lvl) Logging::log(std::string("") + " " + __FILE__ + "[" + std::to_string(__LINE__) + "]" + "\t" + std::string(msg), lvl);
#define LOGERROR(msg) Logging::log(std::string("") + " " + __FILE__ + "[" + std::to_string(__LINE__) + "]" + "\t" + std::string(msg), 0);

#endif

#endif //BIOGUI_LOGGING_H
