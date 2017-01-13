# Reference: http://qt-project.org/doc/qt-5.0/qtdoc/cmake-manual.html

cmake_minimum_required(VERSION 3.0)

# Add folder where are supportive functions
LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Include Qt basic functions
include(QtCommon)

if (APPLE)
set(Qt5_DIR "/Users/joppich/Qt/5.7/clang_64/")
LIST(APPEND CMAKE_PREFIX_PATH "${Qt5_DIR}/lib/cmake/")
LIST(APPEND CMAKE_MODULE_PATH "${Qt5_DIR}/lib/cmake/Qt5")
set(ENV{OPENSSL_ROOT_DIR} "/usr/local/opt/openssl/")
set(QT_QMAKE_EXECUTABLE "${Qt5_DIR}/bin/qmake")
set(CMAKE_FRAMEWORK_PATH "${Qt5_DIR}/lib/cmake")
endif()

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


set(BIOGUI_NETOWRK_INI "${CMAKE_CURRENT_SOURCE_DIR}/config.ini")
set_source_files_properties(${BIOGUI_NETOWRK_INI} PROPERTIES MACOSX_PACKAGE_LOCATION MacOS)
LIST(APPEND META_FILES_TO_INCLUDE "${BIOGUI_NETOWRK_INI}")

set(BIOGUI_INSTALL_TEMPLATES_GUI "${CMAKE_CURRENT_SOURCE_DIR}/templates/WSL_install.gui")
set_source_files_properties(${BIOGUI_INSTALL_TEMPLATES_GUI} PROPERTIES MACOSX_PACKAGE_LOCATION MacOS/templates/)
LIST(APPEND META_FILES_TO_INCLUDE "${BIOGUI_INSTALL_TEMPLATES_GUI}")


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

add_custom_target(deploy COMMAND ${CMAKE_MAKE_PROGRAM} install)
set(BIOGUI_INSTALL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/deploy)

install(TARGETS ${PROJECT_NAME} DESTINATION ${BIOGUI_INSTALL_PATH} )

message(WARNING "files to copy ${META_FILES_TO_INCLUDE}")
