
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
# Reference: http://qt-project.org/doc/qt-5.0/qtdoc/cmake-manual.html

cmake_minimum_required(VERSION 3.0)

# Add folder where are supportive functions
LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Include Qt basic functions
include(QtCommon)

if (APPLE)
    set(Qt5_DIR "/Users/joppich/Qt/5.7/clang_64/")
endif()

if (WIN32)

    set(ENV{LUA_DIR} "D:/libraries/lua-5.3.3/;C:/lua53/")
    message(WARNING "IN MINGW")
    set(Qt5_DIR "C:/Qt/Qt5.7.0/5.7/mingw53_32")
    set(ENV{OPENSSL_ROOT_DIR} "C:/OpenSSL-Win32/lib/")


endif()

message(WARNING "QT PATH: ${Qt5_DIR}")

LIST(APPEND CMAKE_PREFIX_PATH "${Qt5_DIR}/lib/cmake/")
LIST(APPEND CMAKE_MODULE_PATH "${Qt5_DIR}/lib/cmake/Qt5")
set(QT_QMAKE_EXECUTABLE "${Qt5_DIR}/bin/qmake")
set(CMAKE_FRAMEWORK_PATH "${Qt5_DIR}/lib/cmake")

# Basic information about project
project(bioGUI VERSION 1.0)

# Set PROJECT_VERSION_PATCH and PROJECT_VERSION_TWEAK to 0 if not present, needed by add_project_meta
fix_project_version()

# Set additional project information
set(COMPANY "LFE Bioinformatik")
set(COPYRIGHT "Copyright (c) 2016 Markus Joppich. All rights reserved.")
set(IDENTIFIER "de.lmu.ifi.bio.mjoppich")

if (OPENMP_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS} ")
else()

    #set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp")
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")

endif()

find_package(Lua 5.2 REQUIRED)
include_directories(${LUA_INCLUDE_DIR})

message(WARNING "   *** Lua include: ${LUA_INCLUDE_DIR}")
message(WARNING "   *** Lua libs: ${LUA_LIBRARIES}")
message(WARNING "   *** Lua lib: ${LUA_LIBRARY}")
message(WARNING "   *** Lua DLL dir: ${LUA_DLL_DIR}")

# Search OpenSSL
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

add_project_meta(META_FILES_TO_INCLUDE)

set(RESOURCE_FILES "${PROJECT_NAME}.qrc")

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5Xml REQUIRED)
find_package(Qt5XmlPatterns REQUIRED)


if (APPLE)
    set(BIOGUI_NETOWRK_INI "${CMAKE_CURRENT_SOURCE_DIR}/config.ini")
    set_source_files_properties(${BIOGUI_NETOWRK_INI} PROPERTIES MACOSX_PACKAGE_LOCATION MacOS)
    LIST(APPEND META_FILES_TO_INCLUDE "${BIOGUI_NETOWRK_INI}")

    set(BIOGUI_INSTALL_TEMPLATES_GUI "${CMAKE_CURRENT_SOURCE_DIR}/templates/WSL_install.gui")
    set_source_files_properties(${BIOGUI_INSTALL_TEMPLATES_GUI} PROPERTIES MACOSX_PACKAGE_LOCATION MacOS/templates/)
    LIST(APPEND META_FILES_TO_INCLUDE "${BIOGUI_INSTALL_TEMPLATES_GUI}")

endif()


# source_group("UI Files" FILES ${UI_FILES})

add_executable(${PROJECT_NAME} ${OS_BUNDLE} # Expands to WIN32 or MACOS_BUNDLE depending on OS
    ${bioGUI_H} ${bioGUIapp_H} ${bioGUI_SRC} ${parsinglib_SRC} ${parsinglib_H} ${parsinglib_nodes_H} ${META_FILES_TO_INCLUDE} ${RESOURCE_FILES}
)

ADD_LIBRARY(mylibeay STATIC IMPORTED)
SET_TARGET_PROPERTIES(mylibeay PROPERTIES IMPORTED_LOCATION "${OPENSSL_SSL_LIBRARY}")

ADD_LIBRARY(myssleay STATIC IMPORTED)
SET_TARGET_PROPERTIES(myssleay PROPERTIES IMPORTED_LOCATION "${OPENSSL_CRYPTO_LIBRARY}")

ADD_LIBRARY(lualib STATIC IMPORTED)
SET_TARGET_PROPERTIES(lualib PROPERTIES IMPORTED_LOCATION "${LUA_LIBRARY}")

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

TARGET_LINK_LIBRARIES(${PROJECT_NAME} mylibeay myssleay lualib)

qt5_use_modules(${PROJECT_NAME} Core Gui Widgets Network Xml XmlPatterns)

set(BIOGUI_INSTALL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/deploy)
install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX} )

file(GLOB biogui_files
        "config.ini"
        "LICENSE"
        "README.md"
        "./templates/WSL_install.gui"
        )

install(FILES ${biogui_files} DESTINATION ${CMAKE_INSTALL_PREFIX})

message(WARNING "files to copy ${META_FILES_TO_INCLUDE}")
message(WARNING "exe location ${PROJECT_BINARY_DIR}")
message(WARNING "installing to: ${PROJECT_BINARY_DIR}")

if( TRUE )
    #get_filename_component(MINGW_BUILD_BIN ${CMAKE_CXX_COMPILER} PATH)
    install(TARGETS bioGUI DESTINATION ${PROJECT_BINARY_DIR} )


    #install(TARGETS bioGUI DESTINATION ${BIOGUI_INSTALL_PATH} )
    #add_custom_command( TARGET deploy PRE_BUILD COMMAND ${CMAKE_COMMAND} -E remove_directory  ${BIOGUI_INSTALL_PATH})


    add_custom_command( TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR})
    add_custom_command( TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/templates)
    add_custom_command( TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/install_templates)

    add_custom_command( TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/config.ini ${PROJECT_BINARY_DIR})
    add_custom_command( TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE ${PROJECT_BINARY_DIR})
    add_custom_command( TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/README.md ${PROJECT_BINARY_DIR})
    add_custom_command( TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/templates/WSL_install.gui ${PROJECT_BINARY_DIR}/templates)



    # find the release *.dll file
    get_target_property(Qt5_CoreLocation Qt5::Core LOCATION_RELEASE)
    get_target_property(Qt5_CoreSOName Qt5::Core "IMPORTED_SONAME_RELEASE")

    get_target_property(Qt5_WidgetsLocation Qt5::Widgets LOCATION_RELEASE)
    get_target_property(Qt5_WidgetsSOName Qt5::Widgets "IMPORTED_SONAME_RELEASE")

    get_target_property(Qt5_XmlLocation Qt5::Xml LOCATION_RELEASE)
    get_target_property(Qt5_XmlSOName Qt5::Xml "IMPORTED_SONAME_RELEASE")

    get_target_property(Qt5_XmlPatternsLocation Qt5::XmlPatterns LOCATION_RELEASE)
    get_target_property(Qt5_XmlPatternsSOName Qt5::XmlPatterns "IMPORTED_SONAME_RELEASE")

    get_target_property(Qt5_NetworkLocation Qt5::Network LOCATION_RELEASE)
    get_target_property(Qt5_NetworkSOName Qt5::Network "IMPORTED_SONAME_RELEASE")

    get_target_property(Qt5_GuiLocation Qt5::Gui LOCATION_RELEASE)
    get_target_property(Qt5_GuiSOName Qt5::Gui "IMPORTED_SONAME_RELEASE")



    get_target_property(Qt5_CoreLocation_Debug Qt5::Core LOCATION_DEBUG)
    get_target_property(Qt5_CoreSOName_Debug Qt5::Core "IMPORTED_SONAME_DEBUG")

    get_target_property(Qt5_WidgetsLocation_Debug Qt5::Widgets LOCATION_DEBUG)
    get_target_property(Qt5_WidgetsSOName_Debug Qt5::Widgets "IMPORTED_SONAME_DEBUG")

    get_target_property(Qt5_XmlLocation_Debug Qt5::Xml LOCATION_DEBUG)
    get_target_property(Qt5_XmlSOName_Debug Qt5::Xml "IMPORTED_SONAME_DEBUG")

    get_target_property(Qt5_XmlPatternsLocation_Debug Qt5::XmlPatterns LOCATION_DEBUG)
    get_target_property(Qt5_XmlPatternsSOName_Debug Qt5::XmlPatterns "IMPORTED_SONAME_DEBUG")

    get_target_property(Qt5_NetworkLocation_Debug Qt5::Network LOCATION_DEBUG)
    get_target_property(Qt5_NetworkSOName_Debug Qt5::Network "IMPORTED_SONAME_DEBUG")

    get_target_property(Qt5_GuiLocation_Debug Qt5::Gui LOCATION_DEBUG)
    get_target_property(Qt5_GuiSOName_Debug Qt5::Gui "IMPORTED_SONAME_DEBUG")




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

    if (NOT ${Qt5_CoreSOName_Debug})
        set(Qt5_CoreSOName_Debug "")
    endif()

    if (NOT ${Qt5_WidgetsSOName_Debug})
        set(Qt5_WidgetsSOName_Debug "")
    endif()

    if (NOT ${Qt5_XmlSOName_Debug})
        set(Qt5_XmlSOName_Debug "")
    endif()

    if (NOT ${Qt5_XmlPatternsSOName_Debug})
        set(Qt5_XmlPatternsSOName_Debug "")
    endif()

    if (NOT ${Qt5_NetworkSOName_Debug})
        set(Qt5_NetworkSOName_Debug "")
    endif()

    if (NOT ${Qt5_GuiSOName_Debug})
        set(Qt5_GuiSOName_Debug "")
    endif()

        message(WARNING "Qt5 Config: ${Qt5_CoreConfig}")
    message(WARNING "Found Qt5 Core ${Qt5_CoreSOName}")
    message(WARNING "Found Qt5 Widgets ${Qt5_WidgetsLocation}")
    message(WARNING "Found Qt5 Xml ${Qt5_XmlLocation}")
    message(WARNING "Found Qt5 XmlPatterns ${Qt5_XmlPatternsLocation}")
    message(WARNING "Found Qt5 Network ${Qt5_NetworkLocation}")
    message(WARNING "Found Qt5 Gui ${Qt5_GuiLocation}")

    # find the debug *d.dll file
    #get_target_property(Qt5_CoreLocationDebug Qt5::Core IMPORTED_LOCATION_DEBUG)

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different $<$<CONFIG:Debug>:${Qt5_CoreLocation_Debug}> $<$<NOT:$<CONFIG:Debug>>:${Qt5_CoreLocation}> $<$<CONFIG:Debug>:${PROJECT_BINARY_DIR}/${Qt5_CoreSOName_Debug}> $<$<NOT:$<CONFIG:Debug>>:${PROJECT_BINARY_DIR}/${Qt5_CoreSOName}>)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different $<$<CONFIG:Debug>:${Qt5_WidgetsLocation_Debug}> $<$<NOT:$<CONFIG:Debug>>:${Qt5_WidgetsLocation}> $<$<CONFIG:Debug>:${PROJECT_BINARY_DIR}/${Qt5_WidgetSOName_Debug}> $<$<NOT:$<CONFIG:Debug>>:${PROJECT_BINARY_DIR}/${Qt5_WidgetSOName}>)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different $<$<CONFIG:Debug>:${Qt5_XmlLocation_Debug}> $<$<NOT:$<CONFIG:Debug>>:${Qt5_XmlLocation}> $<$<CONFIG:Debug>:${PROJECT_BINARY_DIR}/${Qt5_XmlSOName_Debug}> $<$<NOT:$<CONFIG:Debug>>:${PROJECT_BINARY_DIR}/${Qt5_XmlSOName}>)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different $<$<CONFIG:Debug>:${Qt5_XmlPatternsLocation_Debug}> $<$<NOT:$<CONFIG:Debug>>:${Qt5_XmlPatternsLocation}> $<$<CONFIG:Debug>:${PROJECT_BINARY_DIR}/${Qt5_XmlPatternsSOName_Debug}> $<$<NOT:$<CONFIG:Debug>>:${PROJECT_BINARY_DIR}/${Qt5_XmlPatternsSOName}>)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different $<$<CONFIG:Debug>:${Qt5_NetworkLocation_Debug}> $<$<NOT:$<CONFIG:Debug>>:${Qt5_NetworkLocation}> $<$<CONFIG:Debug>:${PROJECT_BINARY_DIR}/${Qt5_NetworkSOName_Debug}> $<$<NOT:$<CONFIG:Debug>>:${PROJECT_BINARY_DIR}/${Qt5_NetworkSOName}>)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different $<$<CONFIG:Debug>:${Qt5_GuiLocation_Debug}> $<$<NOT:$<CONFIG:Debug>>:${Qt5_GuiLocation}> $<$<CONFIG:Debug>:${PROJECT_BINARY_DIR}/${Qt5_GuiSOName_Debug}> $<$<NOT:$<CONFIG:Debug>>:${PROJECT_BINARY_DIR}/${Qt5_GuiSOName}>)


    file(GLOB biogui_libs
            "${Qt5_CoreLocation}"
            "${Qt5_WidgetsLocation}"
            "${Qt5_XmlLocation}"
            "${Qt5_XmlPatternsLocation}"
            "${Qt5_NetworkLocation}"
            "${Qt5_GuiLocation}"

            )
    install(FILES ${biogui_libs} DESTINATION ${CMAKE_INSTALL_PREFIX})


    if (MINGW)

            get_filename_component(MINGW_BUILD_BIN ${CMAKE_CXX_COMPILER} PATH)

            add_custom_command( TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${MINGW_BUILD_BIN}/libgcc_s_dw2-1.dll ${PROJECT_BINARY_DIR})
            add_custom_command( TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${MINGW_BUILD_BIN}/libstdc++-6.dll ${PROJECT_BINARY_DIR})
            add_custom_command( TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${MINGW_BUILD_BIN}/libwinpthread-1.dll ${PROJECT_BINARY_DIR})
        endif()

        if (UNIX)
            get_filename_component(QT5GUI_DIR ${Qt5_GuiLocation} DIRECTORY)

            add_custom_command( TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${QT5GUI_DIR}/libicudata.so.56 ${PROJECT_BINARY_DIR})
            add_custom_command( TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${QT5GUI_DIR}/libicui18n.so.56 ${PROJECT_BINARY_DIR})
            add_custom_command( TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${QT5GUI_DIR}/libicuuc.so.56 ${PROJECT_BINARY_DIR})

        endif()
    #add_custom_command( TARGET deploy POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${LUA_LIBRARIES} ${BIOGUI_INSTALL_PATH})

endif()