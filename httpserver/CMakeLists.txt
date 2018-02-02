project(httpserver)
cmake_minimum_required(VERSION 2.8)

if(CMAKE_BUILD_TYPE STREQUAL "")
    #set(CMAKE_BUILD_TYPE Debug)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra")

aux_source_directory(. SRC_LIST)

# boost
find_package(Boost 1.54.0 COMPONENTS system thread regex chrono filesystem REQUIRED)
if (Boost_FOUND)
    include_directories (SYSTEM ${Boost_INCLUDE_DIR})
endif ()

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../common/debug)

find_library( POCO_FOUNDATION PocoFoundation )
find_library( POCO_NET PocoNet )
find_library( POCO_UTIL PocoUtil )

add_executable(${PROJECT_NAME} ${SRC_LIST})

target_link_libraries(${PROJECT_NAME} "common" ${Boost_LIBRARIES} ${POCO_FOUNDATION} ${POCO_NET} ${POCO_UTIL} ${REQUIRED_LIBS})
