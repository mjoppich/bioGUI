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

        // get only child
        if (pElement->hasChildNodes() && pElement->childNodes().item(0).isCDATASection())
        {
            m_sInlineLUA = pElement->childNodes().item(0).toCDATASection().data().toStdString();
        } else {

            throw "Script nodes may only have one CDATA child!";
        }

        if ((m_sScriptFile.size() > 0) && (m_sInlineLUA.size() > 0))
        {
            throw "Script nodes may only have a script attribute or inline CDATA LUA!";
        }

        if (m_sScriptFile.size() > 0)
        {
            bool bLUAFileExists = ExecutionFileNode::file_exists( m_sScriptFile );

            if (!bLUAFileExists)
                throw "Script for node does not exist!";

        }

        std::cerr << m_sInlineLUA << std::endl;


    }

    std::string evaluate(std::map<std::string, ExecutionNode *> *pID2Node,
                         std::map <std::string, std::string> *pInputID2Value,
                         std::map<std::string, QWidget *> *pInputID2Widget) {



        return "";

    }


protected:

    void bail(lua_State *L, std::string sMessage){

        std::cerr << "FATAL ERROR: " << sMessage << ": " << lua_tostring(L, -1) << std::endl;

    }

    std::string runLUA()
    {

        lua_State *L;

        L = luaL_newstate();                        /* Create Lua state variable */
        luaL_openlibs(L);                           /* Load Lua libraries */

        //if (luaL_loadstring(L, "io.write(\"This is coming from lua.\\n\")"))    /* Load but don't run the Lua script */
        if (luaL_loadfile(L, "lua_scripts/test.lua"))
            bail(L, "luaL_loadfile() failed");      /* Error out if file can't be read */

        if (lua_pcall(L, 0, 0, 0))                  /* PRIMING RUN. FORGET THIS AND YOU'RE TOAST */
            bail(L, "lua_pcall() failed");          /* Error out if Lua file has an error */

        printf("In C, calling Lua->tellme()\n");

        lua_getglobal(L, "tellme");                 /* Tell it to run callfuncscript.lua->tellme() */
        if (lua_pcall(L, 0, 0, 0))                  /* Run the function */
            bail(L, "lua_pcall() failed");          /* Error out if Lua file has an error */

        printf("Back in C again\n");
        printf("In C, calling Lua->square(6)\n");

        lua_getglobal(L, "square");                 /* Tell it to run callfuncscript.lua->square() */
        lua_pushnumber(L, 6);                       /* Submit 6 as the argument to square() */
        if (lua_pcall(L, 1, 1, 0))                  /* Run function, !!! NRETURN=1 !!! */
            bail(L, "lua_pcall() failed");

        printf("Back in C again\n");
        int mynumber = lua_tonumber(L, 0);
        std::string sResult = lua_tostring(L, 0);

        printf("Returned number=%d\n", mynumber);

        std::cerr << sResult << std::endl;

        lua_close(L);                               /* Clean up, free the Lua state var */

    }


    std::string m_sScriptFile;
    std::string m_sInlineLUA;

};


#endif //BIOGUI_EXECUTIONSCRIPTNODE_H
