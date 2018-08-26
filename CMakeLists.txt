cmake_minimum_required(VERSION 3.4.1)
project(StaffNotationModel CXX)

set(CMAKE_CXX_STANDARD 17)

option(build_shared_libs "build stan as a shared library" ON)
set(BUILD_SHARED_LIBS ${build_shared_libs})

# if (CMAKE_BUILD_TYPE STREQUAL "Debug")
include(cmake/CodeCoverage.cmake)
# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
APPEND_COVERAGE_COMPILER_FLAGS()
# endif (CMAKE_BUILD_TYPE STREQUAL "Debug")

# set(COVERAGE_EXCLUDES 
# 	'${CMAKE_SOURCE_DIR}/dependencies/*' 
# 	'/usr/include/*'
# 	'${CMAKE_BINARY_DIR}/CMakeFiles/*'
# 	)

find_package(Boost COMPONENTS )
find_package(Threads)
find_program(CLANG_TIDY "clang-tidy" REQUIRED)

include_directories("${CMAKE_SOURCE_DIR}/include")

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ferror-limit=5 -fcolor-diagnostics")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libstdc++")
    set(CMAKE_CXX_LINK_FLAGS "${CMAKE_LINK_FLAGS} -stdlib=libstdc++")
endif()

set(BUILD_SHARED_LIBS TRUE)  # Consumed by fmt
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# GCov 
set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE TRUE)

enable_testing()

add_subdirectory(dependencies/mettle)
add_subdirectory(dependencies/type_safe)
add_subdirectory(dependencies/fmt)
add_subdirectory(dependencies/rapidcheck)
add_subdirectory(src)
add_subdirectory(test)

