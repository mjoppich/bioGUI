//
// Created by joppich on 12/6/16.
//

#ifndef BIOGUI_LOGGING_H
#define BIOGUI_LOGGING_H

#include <string>
#include <iostream>

#ifndef LOGDEF

#define LOGDEF

#define LOG(msg) Logging::log(std::string("") + " " + __FILE__ + "[" + std::to_string(__LINE__) + "]" + "\t" + std::string(msg));
#define LOGLVL(msg, lvl) Logging::log(std::string("") + " " + __FILE__ + "[" + std::to_string(__LINE__) + "]" + "\t" + std::string(msg), lvl);
#define LOGERROR(msg) LOGLVL(msg, Logging::ERROR)

#endif

class Logging {
public:
    enum LEVEL {ERROR, INFO, DEBUG};


    static void setLoggingLevel(LEVEL eLevel)
    {
        m_eCurrentLoggingLevel = eLevel;
    }

    static void log( std::string sMessage, LEVEL eLevel = DEBUG)
    {

        std::string sLevel = "ERROR";

        switch (eLevel)
        {
            DEBUG: sLevel = "DEBUG"; break;
            INFO: sLevel = "INFO"; break;
            default:
                sLevel = "Unknown";
        }

        if (eLevel == ERROR)
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

#endif //BIOGUI_LOGGING_H
