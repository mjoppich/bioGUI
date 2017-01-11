
#  bioGUI - a new universal GUI for bioinformatic applications
#  Copyright (C) 2016  Markus Joppich
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 11)
cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

project (bioGUI)

set(COMPANY "Example")
set(COPYRIGHT "Copyright (c) 2016 Markus Joppich. All rights reserved.")
set(IDENTIFIER "de.lmu.ifi.bio.mjoppich")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-fno-exceptions")

find_package(OpenMP REQUIRED)

#set(ENV{CMAKE_PREFIX_PATH} "$ENV{CMAKE_PREFIX_PATH};D:/libraries/lua-5.3.3/;C:/lua53/src")

if (OPENMP_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
#     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS} ")
endif()

set(ENV{LUA_DIR} "D:/libraries/lua-5.3.3/;C:/lua53/")
find_package(Lua 5.2 REQUIRED)
include_directories(${LUA_INCLUDE_DIR})

message(WARNING "   *** Lua include: ${LUA_INCLUDE_DIR}")
message(WARNING "   *** Lua libs: ${LUA_LIBRARIES}")
message(WARNING "   *** Lua lib: ${LUA_LIBRARY}")
message(WARNING "   *** Lua DLL dir: ${LUA_DLL_DIR}")

# Search OpenSSL
#set(OPENSSL_USE_STATIC_LIBS TRUE)
find_package(OpenSSL REQUIRED)

if( OPENSSL_FOUND )

    include_directories(${OPENSSL_INCLUDE_DIRS})
    #set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OPENSSL_C_FLAGS}")
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OPENSSL_CXX_FLAGS}")
    message(WARNING "Using OpenSSL ${OPENSSL_VERSION}")
    message(WARNING "OPENSSL LIBRARIES ${OPENSSL_LIBRARIES}")
    message(WARNING "SSL LIBRARY ${OPENSSL_SSL_LIBRARY}")
    message(WARNING "CRYPTO LIBRARY ${OPENSSL_CRYPTO_LIBRARY}")

else()
    # Error; with REQUIRED, pkg_search_module() will throw an error by it's own
endif()

MESSAGE(WARNING ${CMAKE_SYSTEM_NAME})

if ( CMAKE_SYSTEM_NAME STREQUAL "Windows" OR CYGWIN OR MINGW)
    SET(ENV{Qt5_DIR} "/usr/lib/qt5/x86_64-w64-mingw32/")
    set(QT_QMAKE_EXECUTABLE /usr/lib/qt5/bin/qmake.exe)

    if (MINGW)
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mwindows")
        SET(ENV{Qt5_DIR} "C:\\Qt\\Qt5.7.0\\5.7\\mingw53_32")
        #set(QT_QMAKE_EXECUTABLE G:\\Qt\\5.6\\mingw49_32\\bin\\qmake.exe)
    endif()

else()

    if (EXISTS "/home/users/joppich/Qt/5.7/gcc_64/")
    endif()

    set(QT_QMAKE_EXECUTABLE ENV{Qt5_DIR}/bin/qmake)
    set(QT_ROOT ${Qt5_DIR})

endif()

set ( CMAKE_PREFIX_PATH ${Qt5_DIR})

SET(Qt5_DIR "/home/users/joppich/Qt/5.7/gcc_64/" )
set(QT_QMAKE_EXECUTABLE ${Qt5_DIR}/bin/qmake)


MESSAGE(WARNING "Qt5_DIR " ${Qt5_DIR})
MESSAGE(WARNING "QT_QMAKE_EXECUTABLE " ${QT_QMAKE_EXECUTABLE})

if (MINGW)
    find_package(Qt5 COMPONENTS Core Gui Widgets Network Xml XmlPatterns PATHS "/home/users/joppich/progs/Qt/5.7/gcc_64/"  REQUIRED)
else()
    #find_package(Qt5 COMPONENTS Core Gui Widgets Network Xml XmlPatterns PATHS "/home/users/joppich/Qt/5.7/gcc_64/" "/usr/lib/qt5/x86_64-w64-mingw32/" REQUIRED )
endif()

set(CMAKE_FRAMEWORK_PATH "${Qt5_DIR}/lib/cmake" ${CMAKE_FRAMEWORK_PATH})


find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5Xml REQUIRED)
find_package(Qt5XmlPatterns REQUIRED)

MESSAGE(WARNING "LIBRARY PATH " ${CMAKE_LIBRARY_PATH})
MESSAGE(WARNING "QT5 DIR " ${Qt5_DIR})

set (CMAKE_AUTOMOC ON)

file(GLOB parsinglib_H
        "src/parsing/*.h"
)

file(GLOB parsinglib_nodes_H
        "src/parsing/nodes/*.h"
        "src/parsing/visual_nodes/*.h"
        )

file(GLOB parsinglib_SRC
        "src/parsing/*.cpp"
        "src/parsing/nodes/*.cpp"
        "src/parsing/visual_nodes/*.cpp"
)
add_library(parsing ${parsinglib_SRC} ${parsinglib_H} ${parsinglib_nodes_H} )
target_include_directories (parsing PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

#install(TARGETS parsing DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/lib)
#install(FILES ${parsinglib_H} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/include/parsing)
#install(FILES ${parsinglib_nodes_H} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/include/parsing/nodes)

target_link_libraries(parsing Qt5::Core)
target_link_libraries(parsing Qt5::Widgets)
target_link_libraries(parsing Qt5::Gui)
target_link_libraries(parsing Qt5::Xml)
target_link_libraries(parsing Qt5::XmlPatterns)
target_link_libraries(parsing Qt5::Network)

#qt5_use_modules(parsing Core Widgets Gui Xml XmlPatterns Network)
MESSAGE( WARNING "QT LIBS ${QT_LIBRARIES}")


#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../splitter/include)

#ADD_LIBRARY(splitutils STATIC IMPORTED)
#SET_TARGET_PROPERTIES(splitutils PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/../splitter/lib/libsplitutils.a)
#TARGET_LINK_LIBRARIES(parsing splitutils)


# aortack
file(GLOB bioGUI_H
        "src/*.h"
        )

file(GLOB bioGUIapp_H
        "src/app/*.h"
        )

file(GLOB bioGUI_SRC
        "src/*.cpp"
        "src/app/*.cpp"
        )

#set(CMAKE_VERBOSE_MAKEFILE "TRUE")

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,-rpath-link,'$ORIGIN'")
#SET(CMAKE_INSTALL_RPATH "\$ORIGIN")


ADD_EXECUTABLE(bioGUI ${bioGUI_SRC} ${bioGUI_H} ${bioGUIapp_H} ${CMAKE_CURRENT_SOURCE_DIR}/res/icon.rc)
target_include_directories(bioGUI PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

ADD_LIBRARY(mylibeay STATIC IMPORTED)
SET_TARGET_PROPERTIES(mylibeay PROPERTIES IMPORTED_LOCATION "${OPENSSL_SSL_LIBRARY}")

ADD_LIBRARY(myssleay STATIC IMPORTED)
SET_TARGET_PROPERTIES(myssleay PROPERTIES IMPORTED_LOCATION "${OPENSSL_CRYPTO_LIBRARY}")

ADD_LIBRARY(lualib STATIC IMPORTED)
SET_TARGET_PROPERTIES(lualib PROPERTIES IMPORTED_LOCATION "${LUA_LIBRARY}")

#set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

TARGET_LINK_LIBRARIES(bioGUI parsing mylibeay myssleay lualib)
target_link_libraries(bioGUI Qt5::Core)
target_link_libraries(bioGUI Qt5::Widgets)
target_link_libraries(bioGUI Qt5::Gui)
target_link_libraries(bioGUI Qt5::Xml)
target_link_libraries(bioGUI Qt5::XmlPatterns)
target_link_libraries(bioGUI Qt5::Network)

message(WARNING "main libs ${Qt5Core_LIBRARIES}")

add_custom_target(deploy COMMAND ${CMAKE_MAKE_PROGRAM} install)

if( TRUE )
    #get_filename_component(MINGW_BUILD_BIN ${CMAKE_CXX_COMPILER} PATH)
        set(BIOGUI_INSTALL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/deploy)

    install(TARGETS bioGUI DESTINATION ${BIOGUI_INSTALL_PATH} )


    #install(TARGETS bioGUI DESTINATION ${BIOGUI_INSTALL_PATH} )
    #add_custom_command( TARGET deploy PRE_BUILD COMMAND ${CMAKE_COMMAND} -E remove_directory  ${BIOGUI_INSTALL_PATH})


    add_custom_command( TARGET deploy POST_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory ${BIOGUI_INSTALL_PATH})
    add_custom_command( TARGET deploy POST_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory ${BIOGUI_INSTALL_PATH}/templates)
    add_custom_command( TARGET deploy POST_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory ${BIOGUI_INSTALL_PATH}/install_templates)

    add_custom_command( TARGET deploy POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/config.ini ${BIOGUI_INSTALL_PATH})
    add_custom_command( TARGET deploy POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE ${BIOGUI_INSTALL_PATH})
    add_custom_command( TARGET deploy POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/README.md ${BIOGUI_INSTALL_PATH})
    add_custom_command( TARGET deploy POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/templates/WSL_install.gui ${BIOGUI_INSTALL_PATH}/templates)



    # find the release *.dll file
    get_target_property(Qt5_CoreLocation Qt5::Core LOCATION)
    get_target_property(Qt5_CoreConfig Qt5::Core IMPORTED_CONFIGURATIONS)
    get_target_property(Qt5_CoreSOName Qt5::Core "IMPORTED_SONAME_${Qt5_CoreConfig}")

    get_target_property(Qt5_WidgetsLocation Qt5::Widgets LOCATION)
    get_target_property(Qt5_WidgetsConfig Qt5::Widgets IMPORTED_CONFIGURATIONS)
    get_target_property(Qt5_WidgetsSOName Qt5::Widgets "IMPORTED_SONAME_${Qt5_CoreConfig}")

    get_target_property(Qt5_XmlLocation Qt5::Xml LOCATION)
    get_target_property(Qt5_XmlConfig Qt5::Xml IMPORTED_CONFIGURATIONS)
    get_target_property(Qt5_XmlSOName Qt5::Xml "IMPORTED_SONAME_${Qt5_CoreConfig}")

    get_target_property(Qt5_XmlPatternsLocation Qt5::XmlPatterns LOCATION)
    get_target_property(Qt5_XmlPatternsConfig Qt5::XmlPatterns IMPORTED_CONFIGURATIONS)
    get_target_property(Qt5_XmlPatternsSOName Qt5::XmlPatterns "IMPORTED_SONAME_${Qt5_CoreConfig}")

    get_target_property(Qt5_NetworkLocation Qt5::Network LOCATION)
    get_target_property(Qt5_NetworkConfig Qt5::Network IMPORTED_CONFIGURATIONS)
    get_target_property(Qt5_NetworkSOName Qt5::Network "IMPORTED_SONAME_${Qt5_CoreConfig}")

    get_target_property(Qt5_GuiLocation Qt5::Gui LOCATION)
    get_target_property(Qt5_GuiConfig Qt5::Gui IMPORTED_CONFIGURATIONS)
    get_target_property(Qt5_GuiSOName Qt5::Gui "IMPORTED_SONAME_${Qt5_CoreConfig}")

    if (NOT ${Qt5_CoreSOName})
        set(Qt5_CoreSOName "")
    endif()

    if (NOT ${Qt5_WidgetsSOName})
        set(Qt5_WidgetsSOName "")
    endif()

    if (NOT ${Qt5_XmlSOName})
        set(Qt5_XmlSOName "")
    endif()

    if (NOT ${Qt5_XmlPatternsSOName})
        set(Qt5_XmlPatternsSOName "")
    endif()

    if (NOT ${Qt5_NetworkSOName})
        set(Qt5_NetworkSOName "")
    endif()

    if (NOT ${Qt5_GuiSOName})
        set(Qt5_GuiSOName "")
    endif()

    message(WARNING "Found Qt5 Core ${Qt5_CoreSOName}")
    message(WARNING "Found Qt5 Widgets ${Qt5_WidgetsLocation}")
    message(WARNING "Found Qt5 Xml ${Qt5_XmlLocation}")
    message(WARNING "Found Qt5 XmlPatterns ${Qt5_XmlPatternsLocation}")
    message(WARNING "Found Qt5 Network ${Qt5_NetworkLocation}")
    message(WARNING "Found Qt5 Gui ${Qt5_GuiLocation}")

    # find the debug *d.dll file
    #get_target_property(Qt5_CoreLocationDebug Qt5::Core IMPORTED_LOCATION_DEBUG)
    add_custom_command(TARGET deploy POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${Qt5_CoreLocation} ${BIOGUI_INSTALL_PATH}/${Qt5_CoreSOName})
    add_custom_command(TARGET deploy POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${Qt5_WidgetsLocation} ${BIOGUI_INSTALL_PATH}/${Qt5_WidgetsSOName})
    add_custom_command(TARGET deploy POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${Qt5_XmlLocation} ${BIOGUI_INSTALL_PATH}/${Qt5_XmlSOName})
    add_custom_command(TARGET deploy POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${Qt5_XmlPatternsLocation} ${BIOGUI_INSTALL_PATH}/${Qt5_XmlPatternsSOName})
    add_custom_command(TARGET deploy POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${Qt5_NetworkLocation} ${BIOGUI_INSTALL_PATH}/${Qt5_NetworkSOName})
    add_custom_command(TARGET deploy POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${Qt5_GuiLocation} ${BIOGUI_INSTALL_PATH}/${Qt5_GuiSOName})
    #add_custom_command(TARGET deploy POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different $<$<CONFIG:Debug>:${LUA_LIBRARIES}> $<$<NOT:$<CONFIG:Debug>>:${LUA_LIBRARIES}> ${BIOGUI_INSTALL_PATH})


        if (MINGW)

            get_filename_component(MINGW_BUILD_BIN ${CMAKE_CXX_COMPILER} PATH)

            add_custom_command( TARGET deploy POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${MINGW_BUILD_BIN}/libgcc_s_dw2-1.dll ${BIOGUI_INSTALL_PATH})
            add_custom_command( TARGET deploy POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${MINGW_BUILD_BIN}/libstdc++-6.dll ${BIOGUI_INSTALL_PATH})
            add_custom_command( TARGET deploy POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${MINGW_BUILD_BIN}/libwinpthread-1.dll ${BIOGUI_INSTALL_PATH})
        endif()

        if (UNIX)
            get_filename_component(QT5GUI_DIR ${Qt5_GuiLocation} DIRECTORY)

            add_custom_command( TARGET deploy POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${QT5GUI_DIR}/libicudata.so.56 ${BIOGUI_INSTALL_PATH})
            add_custom_command( TARGET deploy POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${QT5GUI_DIR}/libicui18n.so.56 ${BIOGUI_INSTALL_PATH})
            add_custom_command( TARGET deploy POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${QT5GUI_DIR}/libicuuc.so.56 ${BIOGUI_INSTALL_PATH})

        endif()
    #add_custom_command( TARGET deploy POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${LUA_LIBRARIES} ${BIOGUI_INSTALL_PATH})

endif()