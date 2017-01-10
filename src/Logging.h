/*
 *  bioGUI - a new universal GUI for bioinformatic applications
 *  Copyright (C) 2016  Markus Joppich
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

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
