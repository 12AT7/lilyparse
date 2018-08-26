cmake_minimum_required(VERSION 3.4.1)
project(StaffNotationModel CXX)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost COMPONENTS )
find_package(Threads)
find_program(CLANG_TIDY "clang-tidy" REQUIRED)

include_directories("${CMAKE_SOURCE_DIR}/include")

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ferror-limit=5 -fcolor-diagnostics")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libstdc++")
    set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -stdlib=libstdc++")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-instr-generate -fcoverage-mapping")
    set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -fprofile-instr-generate -fcoverage-mapping")
endif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")

set(BUILD_SHARED_LIBS TRUE)  # Consumed by fmt
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

enable_testing()

add_subdirectory(dependencies/mettle)
add_subdirectory(dependencies/type_safe)
add_subdirectory(dependencies/fmt)
add_subdirectory(dependencies/rapidcheck)
add_subdirectory(src)
add_subdirectory(test)

