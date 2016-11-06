#Splitter Application Package - some toolkit to work with gtf/gff files
#Copyright (C) 2015  Markus Joppich
#
#The Splitter Application Package is free software: you can redistribute it and/or modify
#it under the terms of the GNU General Public License as published by
#the Free Software Foundation, either version 3 of the License, or
#(at your option) any later version.
#
#The Splitter Application Package is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#GNU General Public License for more details.
#
#You should have received a copy of the GNU General Public License
#along with this program.  If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 11)
cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

project (bioGUI)
set(CMAKE_CXX_STANDARD 11)

find_package(OpenMP REQUIRED)

#set(ENV{CMAKE_PREFIX_PATH} "$ENV{CMAKE_PREFIX_PATH};D:/libraries/lua-5.3.3/;C:/lua53/src")

if (OPENMP_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
#     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS} ")
endif()

set(ENV{LUA_DIR} "D:/libraries/lua-5.3.3/;C:/lua53/src")
find_package(Lua REQUIRED)
include_directories(${LUA_INCLUDE_DIR})

message(WARNING "   *** Lua include: ${LUA_INCLUDE_DIR}")
message(WARNING "   *** Lua lib: ${LUA_LIBRARIES}")

# Search OpenSSL
#set(OPENSSL_USE_STATIC_LIBS TRUE)
find_package(OpenSSL REQUIRED)

if( OPENSSL_FOUND )

    include_directories(${OPENSSL_INCLUDE_DIRS})
    #set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OPENSSL_C_FLAGS}")
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OPENSSL_CXX_FLAGS}")
    message(WARNING "Using OpenSSL ${OPENSSL_VERSION}")
    message(WARNING ${OPENSSL_LIBRARIES})
    message(WARNING ${OPENSSL_SSL_LIBRARY})
    message(WARNING ${OPENSSL_CRYPTO_LIBRARY})

    set(MYOPENSSL_LIBS ${OPENSSL_LIBRARIES})
    message(WARNING ${MYOPENSSL_LIBS})


else()
    # Error; with REQUIRED, pkg_search_module() will throw an error by it's own
endif()

MESSAGE(WARNING ${CMAKE_SYSTEM_NAME})

if ( CMAKE_SYSTEM_NAME STREQUAL "Windows" OR CYGWIN OR MINGW)
    SET(ENV{Qt5_DIR} "/usr/lib/qt5/x86_64-w64-mingw32/")
    set(QT_QMAKE_EXECUTABLE /usr/lib/qt5/bin/qmake.exe)

    if (MINGW)
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}") # -mwindows
        SET(ENV{Qt5_DIR} "C:\\Qt\\5.7\\mingw53_32\\")
        #set(QT_QMAKE_EXECUTABLE G:\\Qt\\5.6\\mingw49_32\\bin\\qmake.exe)
    endif()

else()

    if (EXISTS "${/home/users/joppich/progs/Qt/5.5/gcc_64/}")
        SET(ENV{Qt5_DIR} "/home/users/joppich/progs/Qt/5.5/gcc_64/" )
    endif()

    set(QT_QMAKE_EXECUTABLE ${Qt5_DIR}/bin/qmake)
    set(QT_ROOT ${Qt5_DIR})

endif()

set ( CMAKE_PREFIX_PATH ${Qt5_DIR})

MESSAGE(WARNING "Qt5_DIR " ${Qt5_DIR})
MESSAGE(WARNING "QT_QMAKE_EXECUTABLE " ${QT_QMAKE_EXECUTABLE})

if (MINGW)
    find_package(Qt5 COMPONENTS Core Gui Widgets Network Xml XmlPatterns PATHS "/home/users/joppich/progs/Qt/5.7/gcc_64/" REQUIRED)
else()
    find_package(Qt5 COMPONENTS Core Gui Widgets Network Xml XmlPatterns PATHS "/home/users/joppich/Qt/5.7/gcc_64/" "/usr/lib/qt5/x86_64-w64-mingw32/" REQUIRED )
endif()

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

install(TARGETS parsing DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/lib)
install(FILES ${parsinglib_H} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/include/parsing)
install(FILES ${parsinglib_nodes_H} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/include/parsing/nodes)

qt5_use_modules(parsing Core Widgets Gui Xml XmlPatterns Network)


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../splitter/include)

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

ADD_EXECUTABLE(bioGUI ${bioGUI_SRC} ${bioGUI_H} ${bioGUIapp_H} )
target_include_directories(bioGUI PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

ADD_LIBRARY(mylibeay STATIC IMPORTED)
SET_TARGET_PROPERTIES(mylibeay PROPERTIES IMPORTED_LOCATION ${OPENSSL_SSL_LIBRARY})

ADD_LIBRARY(myssleay STATIC IMPORTED)
SET_TARGET_PROPERTIES(myssleay PROPERTIES IMPORTED_LOCATION ${OPENSSL_CRYPTO_LIBRARY})

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

TARGET_LINK_LIBRARIES(bioGUI parsing mylibeay myssleay)
TARGET_LINK_LIBRARIES(bioGUI ${LUA_LIBRARIES})

install(TARGETS bioGUI DESTINATION ${CMAKE_CURRENT_SOURCE_DIR} )

INCLUDE(CPack)

add_custom_target(run_install COMMAND ${CMAKE_MAKE_PROGRAM} install)

if( MINGW)
    get_filename_component(MINGW_BUILD_BIN ${CMAKE_CXX_COMPILER} PATH)

    # find the release *.dll file
    get_target_property(Qt5_CoreLocation Qt5::Core LOCATION)
    get_target_property(Qt5_WidgetsLocation Qt5::Widgets LOCATION)
    get_target_property(Qt5_XmlLocation Qt5::Xml LOCATION)
    get_target_property(Qt5_XmlPatternsLocation Qt5::XmlPatterns LOCATION)
    get_target_property(Qt5_NetworkLocation Qt5::Network LOCATION)
    get_target_property(Qt5_GuiLocation Qt5::Gui LOCATION)

    get_target_property(Qt5_CoreLocationDebug Qt5::Core IMPORTED_LOCATION_DEBUG)
    get_target_property(Qt5_WidgetsLocationDebug Qt5::Widgets IMPORTED_LOCATION_DEBUG)
    get_target_property(Qt5_XmlLocationDebug Qt5::Xml IMPORTED_LOCATION_DEBUG)
    get_target_property(Qt5_XmlPatternsLocationDebug Qt5::XmlPatterns IMPORTED_LOCATION_DEBUG)
    get_target_property(Qt5_NetworkLocationDebug Qt5::Network IMPORTED_LOCATION_DEBUG)
    get_target_property(Qt5_GuiLocationDebug Qt5::Gui IMPORTED_LOCATION_DEBUG)

    # find the debug *d.dll file
    #get_target_property(Qt5_CoreLocationDebug Qt5::Core IMPORTED_LOCATION_DEBUG)
    add_custom_command(TARGET run_install POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different $<$<CONFIG:Debug>:${Qt5_CoreLocationDebug}> $<$<NOT:$<CONFIG:Debug>>:${Qt5_CoreLocation}> ${CMAKE_CURRENT_SOURCE_DIR})
    add_custom_command(TARGET run_install POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different $<$<CONFIG:Debug>:${Qt5_WidgetsLocationDebug}> $<$<NOT:$<CONFIG:Debug>>:${Qt5_WidgetsLocation}> ${CMAKE_CURRENT_SOURCE_DIR})
    add_custom_command(TARGET run_install POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different $<$<CONFIG:Debug>:${Qt5_XmlLocationDebug}> $<$<NOT:$<CONFIG:Debug>>:${Qt5_XmlLocation}> ${CMAKE_CURRENT_SOURCE_DIR})
    add_custom_command(TARGET run_install POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different $<$<CONFIG:Debug>:${Qt5_XmlPatternsLocationDebug}> $<$<NOT:$<CONFIG:Debug>>:${Qt5_XmlPatternsLocation}> ${CMAKE_CURRENT_SOURCE_DIR})
    add_custom_command(TARGET run_install POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different $<$<CONFIG:Debug>:${Qt5_NetworkLocationDebug}> $<$<NOT:$<CONFIG:Debug>>:${Qt5_NetworkLocation}> ${CMAKE_CURRENT_SOURCE_DIR})
    add_custom_command(TARGET run_install POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different $<$<CONFIG:Debug>:${Qt5_GuiLocationDebug}> $<$<NOT:$<CONFIG:Debug>>:${Qt5_GuiLocation}> ${CMAKE_CURRENT_SOURCE_DIR})
    add_custom_command(TARGET run_install POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different $<$<CONFIG:Debug>:${LUA_LIBRARIES}> $<$<NOT:$<CONFIG:Debug>>:${LUA_LIBRARIES}> ${CMAKE_CURRENT_SOURCE_DIR})

    add_custom_command( TARGET run_install POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${MINGW_BUILD_BIN}/libgcc_s_dw2-1.dll ${CMAKE_CURRENT_SOURCE_DIR})
    add_custom_command( TARGET run_install POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${MINGW_BUILD_BIN}/libstdc++-6.dll ${CMAKE_CURRENT_SOURCE_DIR})
    add_custom_command( TARGET run_install POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${MINGW_BUILD_BIN}/libwinpthread-1.dll ${CMAKE_CURRENT_SOURCE_DIR})
    add_custom_command( TARGET run_install POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${LUA_LIBRARIES} ${CMAKE_CURRENT_SOURCE_DIR})

endif()