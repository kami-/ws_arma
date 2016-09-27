# RUN cmake . -G "Visual Studio 14 2015"

# HAVE TO MANUALLY ADD websockets.lib to Linker > Input > Additional Dependencies


cmake_minimum_required (VERSION 3.2)
PROJECT (ws_arma_extension)

SET(SOURCES
    ../include/extension.h
    ../include/id_generator.h
    ../include/log.h
    ../include/server.h
    ../src/extension.cpp
    ../src/id_generator.cpp 
    ../src/log.cpp
    ../src/server.cpp
    ../src/main.cpp
)

SET(POCO_HOME "c:/dev/c++/poco-1.7.5")
SET(LWS_HOME "c:/dev/c++/libwebsockets")

INCLUDE_DIRECTORIES(../include)
INCLUDE_DIRECTORIES(${POCO_HOME}/Foundation/include)
INCLUDE_DIRECTORIES(${POCO_HOME}/JSON/include)
INCLUDE_DIRECTORIES(${POCO_HOME}/NET/include)
INCLUDE_DIRECTORIES(${LWS_HOME})
INCLUDE_DIRECTORIES(${LWS_HOME}/lib)

LINK_DIRECTORIES(${POCO_HOME}/lib)
LINK_DIRECTORIES(${LWS_HOME}/lib)

ADD_EXECUTABLE(ws_arma_extension_console ${SOURCES})
TARGET_COMPILE_DEFINITIONS(ws_arma_extension_console PRIVATE WSA_CONSOLE)

ADD_LIBRARY(ws_arma_extension SHARED ${SOURCES})
