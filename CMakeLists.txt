cmake_minimum_required(VERSION 3.4.1)
project(lilyparse CXX)
set(CMAKE_CXX_STANDARD 17)

include_directories("${CMAKE_SOURCE_DIR}/include")

find_package(Boost COMPONENTS )
find_package(Threads)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ferror-limit=5 -fcolor-diagnostics")
endif()

set(BUILD_SHARED_LIBS TRUE)  # Consumed by fmt

add_subdirectory(dependencies/mettle)
add_subdirectory(dependencies/type_safe)
add_subdirectory(dependencies/fmt)
add_subdirectory(dependencies/rapidcheck)
add_subdirectory(src)
add_subdirectory(test)



