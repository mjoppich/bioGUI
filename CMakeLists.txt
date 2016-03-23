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

#find_package(Qt5 PATHS "C:/Qt/5.5/msvc2013_64" COMPONENTS QTCORE QTGUI QTWIDGETS)

find_package(Qt5Core PATHS "C:/Qt/5.5/msvc2013_64" REQUIRED)
find_package(Qt5Gui PATHS "C:/Qt/5.5/msvc2013_64" REQUIRED)
find_package(Qt5Widgets PATHS "C:/Qt/5.5/msvc2013_64" REQUIRED)

set (CMAKE_AUTOMOC ON)
set (CMAKE_PREFIX_PATH "C:/Qt/5.5/msvc2013_64")

file(GLOB parsinglib_H
"src/parsing/*.h"
)

file(GLOB parsinglib_SRC
"src/parsing/*.cpp"
)
add_library(parsing ${parsinglib_SRC} ${parsinglib_H} )
target_include_directories (parsing PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
install(TARGETS parsing DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/lib)
install(FILES ${parsinglib_H} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/include/parsing)
qt5_use_modules(parsing Core Widgets Gui)


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../splitter/include)

ADD_LIBRARY(splitutils STATIC IMPORTED)
SET_TARGET_PROPERTIES(splitutils PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/../splitter/lib/libsplitutils.a)
TARGET_LINK_LIBRARIES(parsing splitutils)


# aortack
file(GLOB bioGUI_H
        "src/*.h"
        )
file(GLOB bioGUI_SRC
        "src/main.cpp"
        )
ADD_EXECUTABLE(bioGUI ${bioGUI_SRC} ${bioGUI_H})
target_include_directories(bioGUI PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
TARGET_LINK_LIBRARIES(bioGUI parsing)
install(TARGETS bioGUI DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)
