project(OpenTLD)

cmake_minimum_required(VERSION 2.6)

find_package(OpenCV REQUIRED)

#------------------------------------------------------------------------------
#build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING
        "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
        FORCE)
endif(NOT CMAKE_BUILD_TYPE)

add_definitions(-fPIC)

#-------------------------------------------------------------------------------
#add subdirectories

add_subdirectory(src/libopentld)

