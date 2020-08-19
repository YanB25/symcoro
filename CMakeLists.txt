cmake_minimum_required(VERSION 3.12.4)

project (symcoro)

option(BUILD_BINARIES "Build the binaries in {project}/bin directory" ON)
option(BUILD_STATIC_LIBS "Build the static library" ON)
option(BUILD_SHARED_LIBS "Build the shared library" ON)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# dir to include
set(SYMCORO_INCLUDE_DIRECTORIES include)

add_compile_options("-Wall" "-W" "-Wextra" "-fPIC")

# headers to install
file(GLOB_RECURSE SYMCORO_PUBLIC_HEADER include/*)

# source for the lib
file(GLOB_RECURSE SYMCORO_SOURCE src/*)
add_library(symcoro_obj OBJECT ${SYMCORO_SOURCE})
target_include_directories(symcoro_obj PUBLIC ${SYMCORO_INCLUDE_DIRECTORIES})

find_package(Boost 1.4 COMPONENTS coroutine REQUIRED)

if (${BUILD_BINARIES})
    add_subdirectory(bin)
endif()

if (${BUILD_STATIC_LIBS})
    add_library(symcoro-static STATIC $<TARGET_OBJECTS:symcoro_obj>)

    # install instructions
    set_target_properties(symcoro-static PROPERTIES PUBLIC_HEADER ${SYMCORO_PUBLIC_HEADER})
    install(TARGETS symcoro-static PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_PREFIX}/include/symcoro)
endif()

if (${BUILD_SHARED_LIBS})
    add_library(symcoro-dynamic SHARED $<TARGET_OBJECTS:symcoro_obj>)

    # install instructions
    set_target_properties(symcoro-dynamic PROPERTIES PUBLIC_HEADER ${SYMCORO_PUBLIC_HEADER})
    install(TARGETS symcoro-dynamic PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_PREFIX}/include/symcoro)
endif()