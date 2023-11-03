cmake_minimum_required (VERSION 2.8.11)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project (clienteMultMatrix LANGUAGES CXX)
add_executable (clienteMultMatrix main_multMatrix.cpp multmatrix.cpp multmatrix.h matrix_imp.h operaciones.h utils.cpp utils.h)
target_link_libraries(clienteMultMatrix pthread)

project(serverMultMatrix LANGUAGES CXX)
add_executable(serverMultMatrix utils.h utils.cpp multmatrix.cpp multmatrix.h matrix_stub.h operaciones.h)
target_link_libraries(serverMultMatrix pthread)