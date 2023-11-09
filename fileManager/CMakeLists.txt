cmake_minimum_required (VERSION 2.8.11)

project (fileManager LANGUAGES CXX)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_executable (fileManager main_fm.cpp filemanager.cpp filemanager.h)

add_executable (serverFileManager mainServer.cpp filemanager.cpp filemanager.h utils.h utils.cpp operacionesFiles.h fileManager_imp.h)
target_link_libraries(serverFileManager pthread)

add_executable(clienteFileManager  utils.h utils.cpp main_fm.cpp filemanager.h filemanager.cpp operacionesFiles.h fileManager_stub.h)
target_link_libraries(clienteFileManager pthread)
