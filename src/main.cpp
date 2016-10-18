#include <iostream>

#include <QtCore>
#include <QDebug>
#include <QtGui>
#include <QWidget>
#include <QLabel>
#include <QApplication>
#include <QXmlQuery>

#include <QMainWindow>
#include <QDomElement>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QGroupBox>
#include <QComboBox>
#include <QRadioButton>
#include <QCheckBox>
#include <QFileDialog>
#include <QPushButton>
#include "bioGUIapp.h"

extern "C" {
#include <lua.h>                                /* Always include this when calling Lua */
#include <lauxlib.h>                            /* Always include this when calling Lua */
#include <lualib.h>                             /* Always include this when calling Lua */
}

void bail(lua_State *L, char *msg){
    fprintf(stderr, "\nFATAL ERROR:\n  %s: %s\n\n",
        msg, lua_tostring(L, -1));
    exit(1);
}

int main(int argc, char **argv)
{

    lua_State *L;

    L = luaL_newstate();                        /* Create Lua state variable */
    luaL_openlibs(L);                           /* Load Lua libraries */

    if (luaL_loadfile(L, "lua_scripts/test.lua"))    /* Load but don't run the Lua script */
    bail(L, "luaL_loadfile() failed");      /* Error out if file can't be read */

    printf("In C, calling Lua\n");

    if (lua_pcall(L, 0, 0, 0))                  /* Run the loaded Lua script */
    bail(L, "lua_pcall() failed");          /* Error out if Lua file has an error */

    printf("Back in C again\n");

    lua_close(L);


    bioGUIapp* pApp = new bioGUIapp(argc, argv);

    pApp->exec();

    delete pApp;

    return 0;
}
