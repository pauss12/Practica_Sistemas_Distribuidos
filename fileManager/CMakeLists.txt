cmake_minimum_required (VERSION 2.8.11)

project (serverFileManager LANGUAGES CXX)
add_executable (serverFileManager mainServer.cpp filemanager.cpp filemanager.h utils.h utils.cpp operacionesFiles.h fileManager_imp.h)
target_link_libraries(serverFileManager pthread)

project(clienteFileManager LANGUAGES CXX)
add_executable(clienteFileManager  utils.h utils.cpp main_fm.cpp filemanager.h filemanager.cpp operacionesFiles.h fileManager_stub.h)
target_link_libraries(clienteFileManager pthread)
