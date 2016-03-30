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

find_package(OpenMP)
if (OPENMP_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}") #-pg
#     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS} ")
endif()

MESSAGE(WARNING ${CMAKE_SYSTEM_NAME})

if ( CMAKE_SYSTEM_NAME STREQUAL "Windows" OR CYGWIN OR MINGW)
    SET(Qt5_DIR "/")
    set(QT_QMAKE_EXECUTABLE /usr/lib/qt5/bin/qmake.exe)

else()
    SET(Qt5_DIR "/home/users/joppich/progs/Qt/5.5/gcc_64/" )
    set(QT_QMAKE_EXECUTABLE ${Qt5_DIR}/bin/qmake)
    set(QT_ROOT ${Qt5_DIR})

endif()

set ( CMAKE_PREFIX_PATH ${Qt5_DIR})

MESSAGE(WARNING "Qt5_DIR " ${Qt5_DIR})
MESSAGE(WARNING "QT_QMAKE_EXECUTABLE " ${QT_QMAKE_EXECUTABLE})

find_package(Qt5 5.5 COMPONENTS Core Gui Widgets Network Xml XmlPatterns PATHS "/home/users/joppich/progs/Qt/5.5/gcc_64/" REQUIRED NO_SYSTEM_ENVIRONMENT_PATH)

#find_package(Qt5Core 5.5 PATHS "C:/Qt/5.5/msvc2013_64" "/home/users/joppich/progs/Qt/5.5/gcc_64/" REQUIRED NO_SYSTEM_ENVIRONMENT_PATH)
#find_package(Qt5Gui 5.5 PATHS "C:/Qt/5.5/msvc2013_64" "/home/users/joppich/progs/Qt/5.5/gcc_64/" REQUIRED NO_SYSTEM_ENVIRONMENT_PATH)
#find_package(Qt5Widgets 5.5 PATHS "C:/Qt/5.5/msvc2013_64" "/home/users/joppich/progs/Qt/5.5/gcc_64/" REQUIRED NO_SYSTEM_ENVIRONMENT_PATH)
#find_package(Qt5Network 5.5 PATHS "C:/Qt/5.5/msvc2013_64" "/home/users/joppich/progs/Qt/5.5/gcc_64/" REQUIRED NO_SYSTEM_ENVIRONMENT_PATH)
#find_package(Qt5XmlPatterns 5.5 PATHS "C:/Qt/5.5/msvc2013_64" "/home/users/joppich/progs/Qt/5.5/gcc_64/" REQUIRED NO_SYSTEM_ENVIRONMENT_PATH)

MESSAGE(WARNING "LIBRARY PATH " ${CMAKE_LIBRARY_PATH})
MESSAGE(WARNING "QT5 DIR " ${Qt5_DIR})

set (CMAKE_AUTOMOC ON)

file(GLOB parsinglib_H
        "src/parsing/*.h"
)

file(GLOB parsinglib_nodes_H
        "src/parsing/nodes/*.h"
        )

file(GLOB parsinglib_SRC
        "src/parsing/*.cpp"
        "src/parsing/nodes/*.cpp"
)
add_library(parsing ${parsinglib_SRC} ${parsinglib_H} ${parsinglib_nodes_H} )
target_include_directories (parsing PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
install(TARGETS parsing DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/lib)
install(FILES ${parsinglib_H} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/include/parsing)
install(FILES ${parsinglib_nodes_H} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/include/parsing/nodes)
qt5_use_modules(parsing Core Widgets Gui Xml XmlPatterns)


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../splitter/include)

ADD_LIBRARY(splitutils STATIC IMPORTED)
SET_TARGET_PROPERTIES(splitutils PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/../splitter/lib/libsplitutils.a)
TARGET_LINK_LIBRARIES(parsing splitutils)


# aortack
file(GLOB bioGUI_H
        "src/*.h"
        )
file(GLOB bioGUI_SRC
        "src/*.cpp"
        )
ADD_EXECUTABLE(bioGUI ${bioGUI_SRC} ${bioGUI_H})
target_include_directories(bioGUI PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
TARGET_LINK_LIBRARIES(bioGUI parsing)
install(TARGETS bioGUI DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)
