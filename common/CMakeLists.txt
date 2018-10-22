project(common)
cmake_minimum_required(VERSION 2.8)

if(CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE Debug)
    #set(CMAKE_BUILD_TYPE Release)
endif()


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra")
set(SRC_LIST ${SRC_LIST} "appserver.h" "sync.h" "msg.h" "singleton.h" "file_utils.h")

aux_source_directory(. SRC_LIST)
set(MODULE_NAME "${PROJECT_NAME}${BUILD_SUFFIX}")


get_property(COMMON_D GLOBAL PROPERTY COMMON_DEFINED)

if(NOT COMMON_D STREQUAL "YES")
    set_property(GLOBAL PROPERTY COMMON_DEFINED "YES")
    add_library(${MODULE_NAME} SHARED ${SRC_LIST})
endif()

# boost
find_package(Boost 1.54.0 COMPONENTS system thread regex chrono filesystem REQUIRED)

find_library( POCO_FOUNDATION PocoFoundation )
find_library( POCO_NET PocoNet )
find_library( POCO_UTIL PocoUtil )

target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} ${POCO_FOUNDATION} ${POCO_NET} ${POCO_UTIL} ${REQUIRED_LIBS})
