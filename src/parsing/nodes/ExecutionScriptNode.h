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

#include "../../duktape/duktape.h"
}

class ExecutionScriptNode : public ExecutionNode {

public:
    ExecutionScriptNode(QDomElement* pElement)
            : ExecutionNode(pElement)
    {
        m_sLanguage = this->getDomElementAttribute(pElement, "lang", "lua").toUpper().toStdString();
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

    virtual std::vector<std::string> inputs()
    {
        std::vector<std::string> vArgs = this->getArguments(NULL,NULL,NULL);

        return vArgs;
    }

    std::string evaluate(std::map<std::string, ExecutionNode *> *pID2Node,
                         std::map <std::string, std::string> *pInputID2Value,
                         std::map<std::string, WidgetFunctionNode*>* pInputID2FunctionWidget) {

        bool bLUAFileExists = ExecutionFileNode::file_exists( m_sScriptFile );

        if ((m_bScriptBased) && (!bLUAFileExists))
        {
            LOGERROR("Script does not exist at " + m_sScriptFile);

            return "";
        }


        std::vector<std::string> vArguments = this->getArguments(pID2Node, pInputID2Value, pInputID2FunctionWidget);

        std::string sReturn = "";
        if (m_sLanguage.compare("LUA") == 0)
        {
            sReturn = this->runLUA(vArguments);
        } else if (m_sLanguage.compare("JAVASCRIPT") == 0)
        {
            sReturn = this->runJS(vArguments);
        }

        return sReturn;

    }

    virtual ~ExecutionScriptNode()
    {

    }

protected:

    std::vector<std::string> getArguments(std::map<std::string, ExecutionNode *> *pID2Node,
                                          std::map <std::string, std::string> *pInputID2Value,
                                          std::map<std::string, WidgetFunctionNode*>* pInputID2FunctionWidget)
    {
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
                    std::string sArg = this->getNodeValueOrValue(sFieldName, sFieldName, pID2Node, pInputID2Value, pInputID2FunctionWidget);
                    vArguments.push_back( sArg );

                } else {
                    vArguments.push_back( sArgName );
                }

            }

        }

        return vArguments;
    }


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

    std::string runJS( std::vector<std::string>& vArguments )
    {

        for (int i = 0; i < vArguments.size(); ++i)
        {
            std::cerr << "JS INPUT" << i << " " << vArguments.at(i) << std::endl;
        }


        duk_context *ctx = NULL;
        char line[4096];
        size_t idx;
        int ch;

        ctx = duk_create_heap_default();
        if (!ctx) {
            printf("Failed to create a Duktape heap.\n");
            exit(1);
        }

        if (m_bScriptBased)
        {

            FILE *f;
            size_t len;
            char buf[16384];

            f = fopen(m_sScriptFile.c_str(), "rb");
            if (f) {
                len = fread((void *) buf, 1, sizeof(buf), f);
                fclose(f);
                duk_push_lstring(ctx, (const char *) buf, (duk_size_t) len);
            } else {
                duk_push_undefined(ctx);
            }

        } else {

            duk_push_lstring(ctx, (const char *) m_sInlineLUA.c_str(), (duk_size_t) m_sInlineLUA.size());
        }

        if (duk_peval(ctx) != 0) {
            printf("Error: %s\n", duk_safe_to_string(ctx, -1));
            return "";
        }
        duk_pop(ctx);  /* ignore result */


        duk_push_global_object(ctx);

        duk_get_prop_string(ctx, -1 /*index*/, "evaluate"); // function to call

        for (size_t i = 0; i < vArguments.size(); ++i)
        {
            duk_push_string(ctx, vArguments[i].c_str());
        }

        if (duk_pcall(ctx, vArguments.size() /*nargs*/) != 0) {
            printf("Error: %s\n", duk_safe_to_string(ctx, -1));
        } else {
            const char* pRet = duk_safe_to_string(ctx, -1);
            std::string sRet = std::string(pRet);

            std::cout << sRet << std::endl;

            return sRet;
        }
        duk_pop(ctx);




        return "";
    }

    void addNodeAttributes(std::vector<std::string>& vAttributes)
    {
        vAttributes.push_back("SCRIPT");
        vAttributes.push_back("ARGV");

    }


    std::string m_sScriptFile;
    std::string m_sInlineLUA;
    std::string m_sArguments;
    std::string m_sLanguage;

    bool m_bScriptBased = false;

};


#endif //BIOGUI_EXECUTIONSCRIPTNODE_H
