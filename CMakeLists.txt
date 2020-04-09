cmake_minimum_required(VERSION 3.15)
project(lua_pam)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_DIR src)

add_library(lua_pam SHARED ${SOURCE_DIR}/main.cpp)
#add_executable(lua_pam ${SOURCE_DIR}/main.cpp)
target_link_libraries(lua_pam lua pam pam_misc)

#add_executable(test test.cpp)
#target_link_libraries(test pam pam_misc)
