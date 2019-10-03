cmake_minimum_required(VERSION 2.8.3)
project(libopentld)

## Add support for C++11, supported in ROS Kinetic and newer
add_definitions(-std=c++11 -fPIC -g -O0)
#add_definitions(-std=c++11 -O3 -mcpu=cortex-a9 -mfloat-abi=hard -ffast-math -mfpu=neon -ftree-vectorize)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS)
find_package(OpenCV REQUIRED)

SET(CMAKE_CXX_FLAGS " -I/usr/local/include ")
set (CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE} ")

###################################
## catkin specific configuration ##
###################################
## The catkin_package macro generates cmake config files for your package
## Declare things to be passed to dependent projects
## INCLUDE_DIRS: uncomment this if you package contains header files
## LIBRARIES: libraries you create in this project that dependent projects also need
## CATKIN_DEPENDS: catkin_packages dependent projects also need
## DEPENDS: system dependencies of this project that dependent projects also need
catkin_package(CATKIN_DEPENDS)

include_directories(${catkin_INCLUDE_DIRS}
    mftracker
    tld
    ${OpenCV_INCLUDE_DIRS}
)

link_directories(${OpenCV_LIB_DIR})

add_library(${PROJECT_NAME} SHARED
    mftracker/BB.cpp
    mftracker/BBPredict.cpp
    mftracker/FBTrack.cpp
    mftracker/Lk.cpp
    mftracker/Median.cpp
    tld/Clustering.cpp
    tld/DetectionResult.cpp
    tld/DetectorCascade.cpp
    tld/EnsembleClassifier.cpp
    tld/ForegroundDetector.cpp
    tld/MedianFlowTracker.cpp
    tld/NNClassifier.cpp
    tld/TLD.cpp
    tld/TLDUtil.cpp
    tld/VarianceFilter.cpp
    mftracker/BB.h
    mftracker/BBPredict.h
    mftracker/FBTrack.h
    mftracker/Lk.h
    mftracker/Median.h
    tld/Clustering.h
    tld/DetectionResult.h
    tld/DetectorCascade.h
    tld/EnsembleClassifier.h
    tld/ForegroundDetector.h
    tld/IntegralImage.h
    tld/MedianFlowTracker.h
    tld/NNClassifier.h
    tld/NormalizedPatch.h
    tld/TLD.h
    tld/TLDUtil.h
    tld/VarianceFilter.h
)

target_link_libraries(libopentld ${OpenCV_LIBS} ${catkin_LIBRARIES})

set_target_properties(libopentld PROPERTIES OUTPUT_NAME opentld)

