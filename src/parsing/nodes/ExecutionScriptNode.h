//
// Created by joppich on 10/18/16.
//

#ifndef BIOGUI_EXECUTIONSCRIPTNODE_H
#define BIOGUI_EXECUTIONSCRIPTNODE_H

#include "ExecutionNode.h"
#include "ExecutionFileNode.h"
#include <string>
#include <iostream>

extern "C" {
#include <lua.h>                                /* Always include this when calling Lua */
#include <lauxlib.h>                            /* Always include this when calling Lua */
#include <lualib.h>                             /* Always include this when calling Lua */
}

class ExecutionScriptNode : public ExecutionNode {

public:
    ExecutionScriptNode(QDomElement* pElement)
            : ExecutionNode(pElement)
    {
        m_sScriptFile = this->getDomElementAttribute(pElement, "script", "").toStdString();
        m_sArguments = this->getDomElementAttribute(pElement, "argv", "").toStdString();

        // get only child
        if (pElement->hasChildNodes() && pElement->childNodes().item(0).isCDATASection())
        {
            m_sInlineLUA = pElement->childNodes().item(0).toCDATASection().data().toStdString();
            m_bScriptBased = false;

        } else {

            std::cout << "no inline script detected" << std::endl;

            //throw ExecutionNodeException("Script nodes may only have one CDATA child!");
        }

        if ((m_sScriptFile.size() > 0) && (m_sInlineLUA.size() > 0))
        {
            //throw ExecutionNodeException("Script nodes may only have a script attribute or inline CDATA LUA!");
        }

        if (m_sScriptFile.size() > 0)
        {
            m_bScriptBased = true;

            std::cout << "script location is set " << std::endl;
        }

        std::cout << m_bScriptBased << std::endl;

        std::cout << m_sInlineLUA << std::endl;


    }

    std::string evaluate(std::map<std::string, ExecutionNode *> *pID2Node,
                         std::map <std::string, std::string> *pInputID2Value,
                         std::map<std::string, QWidget *> *pInputID2Widget) {

        bool bLUAFileExists = ExecutionFileNode::file_exists( m_sScriptFile );

        if ((m_bScriptBased) && (!bLUAFileExists))
        {
            LOGERROR("LUA script does not exist at " + m_sScriptFile);

            return "";
        }


        // parse arguments
        std::vector<std::string> vArguments;

        QString oArgvString(m_sArguments.c_str());
        QStringList vArgs = oArgvString.split(",");

        if (!oArgvString.contains(','))
        {
            if (oArgvString.size() == 0)
                vArgs.clear();
        }

        for (QString& oString : vArgs)
        {
            std::string sArgName = oString.toStdString();

            if (sArgName.size() < 3)
            {
                vArguments.push_back( sArgName );
            } else {

                if ((sArgName[0] == '$') && (sArgName[1] == '{') && (sArgName[sArgName.size()-1] == '}'))
                {

                    std::string sFieldName = sArgName.substr(2, sArgName.size()-3);

                    std::string sArg = this->getNodeValueOrValue(sFieldName, sFieldName, pID2Node, pInputID2Value, pInputID2Widget);

                    vArguments.push_back( sArg );

                } else {
                    vArguments.push_back( sArgName );
                }

            }

        }


        std::string sReturn = this->runLUA(vArguments);

        return sReturn;

    }

    virtual ~ExecutionScriptNode()
    {

    }

protected:

    void bail(lua_State *L, std::string sMessage){

        std::cerr << "FATAL ERROR: " << sMessage << ": " << lua_tostring(L, -1) << std::endl;

    }

    std::string runLUA( std::vector<std::string>& vArguments )
    {

        lua_State *L;

        L = luaL_newstate();                        /* Create Lua state variable */
        luaL_openlibs(L);                           /* Load Lua libraries */

        //if (luaL_loadstring(L, "io.write(\"This is coming from lua.\\n\")"))    /* Load but don't run the Lua script */

        if (m_bScriptBased)
        {

            if (luaL_loadfile(L, m_sScriptFile.c_str()))
                bail(L, "luaL_loadfile() failed");

        } else {

            if (luaL_loadstring(L, m_sInlineLUA.c_str()))
                bail(L, "luaL_loadfile() failed");

        }

        if (lua_pcall(L, 0, 0, 0))                  /* PRIMING RUN. FORGET THIS AND YOU'RE TOAST */
            bail(L, "lua_pcall() failed");          /* Error out if Lua file has an error */

        lua_getglobal(L, "evaluate");/* Tell it to run callfuncscript.lua->tellme() */

        for (size_t i = 0; i < vArguments.size(); ++i)
        {
            lua_pushstring(L, vArguments[i].c_str());
        }

        if (lua_pcall(L, vArguments.size(), 1, 0))                  /* Run the function */
            bail(L, "calling evaluate failed");          /* Error out if Lua file has an error */

        std::string sResult = lua_tostring(L, -1);

        std::cerr << "LUA result: " << sResult << std::endl;

        lua_close(L);                               /* Clean up, free the Lua state var */

        return sResult;
    }

    void addNodeAttributes(std::vector<std::string>& vAttributes)
    {
        vAttributes.push_back("SCRIPT");
        vAttributes.push_back("ARGV");

    }


    std::string m_sScriptFile;
    std::string m_sInlineLUA;
    std::string m_sArguments;

    bool m_bScriptBased = false;

};


#endif //BIOGUI_EXECUTIONSCRIPTNODE_H
