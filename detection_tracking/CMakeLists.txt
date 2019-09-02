cmake_minimum_required(VERSION 2.8.3)
project(detection_tracking)

## Add support for C++11, supported in ROS Kinetic and newer
add_definitions(-std=c++11 -pthread -g -O0)
#add_definitions(-std=c++11 -O3 -mcpu=cortex-a9 -mfloat-abi=hard -ffast-math -mfpu=neon -ftree-vectorize)


## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  image_transport
  cv_bridge
  message_generation
  sensor_msgs
  tld_msgs
  communication REQUIRED
)

set (CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE} -lpthread -lrt")

## System dependencies are found with CMake's conventions
find_package(OpenCV REQUIRED)
find_package(Boost REQUIRED COMPONENTS system)
find_package(Qt4 REQUIRED COMPONENTS
  QtCore
  QtGui
)


set(OPENTLD_OUTPUT_DIR ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/../)

include(ExternalProject)

externalproject_add(opentld
  PREFIX libopentld
  SVN_REPOSITORY  https://github.com/Ronan0912/OpenTLD/trunk/
  SVN_REVISION    -rHEAD
  PATCH_COMMAND   patch -p0 -i ${PROJECT_SOURCE_DIR}/opentld/patches/diff1.patch
                  &&
                  patch -p0 -i ${PROJECT_SOURCE_DIR}/opentld/patches/diff2.patch
  BUILD_IN_SOURCE 1
  BUILD_COMMAND   cmake --build .
  INSTALL_COMMAND ${PROJECT_SOURCE_DIR}/opentld/install.sh
                  ${OPENTLD_OUTPUT_DIR}
)


## System dependencies are found with CMake's conventions
# find_package(Boost REQUIRED COMPONENTS system)
#blabla blabla 

## Uncomment this if the package has a setup.py. This macro ensures
## modules and global scripts declared therein get installed
## See http://ros.org/doc/api/catkin/html/user_guide/setup_dot_py.html
# catkin_python_setup()

################################################
## Declare ROS messages, services and actions ##
################################################

## To declare and build messages, services or actions from within this
## package, follow these steps:
## * Let MSG_DEP_SET be the set of packages whose message types you use in
##   your messages/services/actions (e.g. std_msgs, actionlib_msgs, ...).
## * In the file package.xml:
##   * add a build_depend tag for "message_generation"
##   * add a build_depend and a run_depend tag for each package in MSG_DEP_SET
##   * If MSG_DEP_SET isn't empty the following dependency has been pulled in
##     but can be declared for certainty nonetheless:
##     * add a run_depend tag for "message_runtime"
## * In this file (CMakeLists.txt):
##   * add "message_generation" and every package in MSG_DEP_SET to
##     find_package(catkin REQUIRED COMPONENTS ...)
##   * add "message_runtime" and every package in MSG_DEP_SET to
##     catkin_package(CATKIN_DEPENDS ...)
##   * uncomment the add_*_files sections below as needed
##     and list every .msg/.srv/.action file to be processed
##   * uncomment the generate_messages entry below
##   * add every package in MSG_DEP_SET to generate_messages(DEPENDENCIES ...)

## Generate messages in the 'msg' folder
#add_message_files(
#   FILES
#   area_location.msg
#)

## Generate services in the 'srv' folder
# add_service_files(
#   FILES
#   Service1.srv
#   Service2.srv
# )

## Generate actions in the 'action' folder
# add_action_files(
#   FILES
#   Action1.action
#   Action2.action
# )

## Generate added messages and services with any dependencies listed here
generate_messages(
  DEPENDENCIES
  std_msgs
)

################################################
## Declare ROS dynamic reconfigure parameters ##
################################################

## To declare and build dynamic reconfigure parameters within this
## package, follow these steps:
## * In the file package.xml:
##   * add a build_depend and a run_depend tag for "dynamic_reconfigure"
## * In this file (CMakeLists.txt):
##   * add "dynamic_reconfigure" to
##     find_package(catkin REQUIRED COMPONENTS ...)
##   * uncomment the "generate_dynamic_reconfigure_options" section below
##     and list every .cfg file to be processed

## Generate dynamic reconfigure parameters in the 'cfg' folder
# generate_dynamic_reconfigure_options(
#   cfg/DynReconf1.cfg
#   cfg/DynReconf2.cfg
# )

###################################
## catkin specific configuration ##
###################################
## The catkin_package macro generates cmake config files for your package
## Declare things to be passed to dependent projects
## INCLUDE_DIRS: uncomment this if you package contains header files
## LIBRARIES: libraries you create in this project that dependent projects also need
## CATKIN_DEPENDS: catkin_packages dependent projects also need
## DEPENDS: system dependencies of this project that dependent projects also need
catkin_package(
  #  INCLUDE_DIRS include
  LIBRARIES libopentld
  CATKIN_DEPENDS 
  roscpp 
  std_msgs 
  message_runtime
  communication
  cv_bridge
  sensor_msgs
  tld_msgs
  #  DEPENDS system_lib
)

###########
## Build ##
###########

#Qt4 specific for the GUI
set(TLD_QT_HEADERS  src/base_frame.hpp 
                    src/ui_baseFrame.h
                    src/base_frame_graphics_view.hpp)

QT4_WRAP_CPP(TLD_QT_HEADERS_MOC ${TLD_QT_HEADERS})
include(${QT_USE_FILE})

## Specify additional locations of header files
## Your package locations should be listed before other locations
# include_directories(include)
include_directories(
  ${catkin_INCLUDE_DIRS} 
  /opt/ros/kinetic/include/image_transport
  ${OpenCV_INCLUDE_DIRS}
  ${OPENTLD_OUTPUT_DIR}/include/libopentld
  /usr/local/include
  ../shared_memory_lib/src
)

link_directories(${OPENTLD_OUTPUT_DIR}/lib/libopentld/)

## Declare a C++ library
#add_library(${PROJECT_NAME}(
#   src/${PROJECT_NAME}/detection_tracking.cpp /opt/ros/kinetic/lib/libimage_transport.so
#)

## Add cmake target dependencies of the library
## as an example, code may need to be generated before libraries
## either from message generation or dynamic reconfigure
#add_dependencies(detection_tracking detection_tracking_generate_messages_cpp)

## Declare a C++ executable
## With catkin_make all packages are built within a single CMake context
## The recommended prefix ensures that target names across packages don't collide
add_executable(detection_tracking_node
			src/detection_tracking_node.cpp
      src/main.cpp 
      src/debug.c	
      src/utils.cpp
)

add_executable(tld_gui_node     src/tld_gui_node.cpp
                                src/base_frame.cpp 
                                src/base_frame_graphics_view.cpp
                                ${TLD_QT_HEADERS_MOC}
)

## Rename C++ executable without prefix
## The above recommended prefix causes long target names, the following renames the
## target back to the shorter Version for ease of user use
## e.g. "rosrun someones_pkg node" instead of "rosrun someones_pkg someones_pkg_node"
# set_target_properties(${PROJECT_NAME}_node PROPERTIES OUTPUT_NAME node PREFIX "")

## Add cmake target dependencies of the executable
## same as for the library above
# add_dependencies(${PROJECT_NAME}_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
#add_dependencies(detection_tracking_node detection_tracking_generate_messages_cpp)
add_dependencies(${PROJECT_NAME}_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
add_dependencies(${PROJECT_NAME}_node opentld)
add_dependencies(${PROJECT_NAME}_node tld_msgs_generate_messages_cpp)

add_dependencies(tld_gui_node tld_msgs_generate_messages_cpp)

## Specify libraries to link a library or executable target against
 target_link_libraries(detection_tracking_node
  ${catkin_LIBRARIES}
  /opt/ros/kinetic/lib/libimage_transport.so
  ${OpenCV_LIBRARIES}
  libopentld
  /usr/local/lib/
  shared_memory_lib
 )

 target_link_libraries(tld_gui_node
  ${QT_LIBRARIES}
  ${catkin_LIBRARIES}
)

#############
## Install ##
#############

# all install targets should use catkin DESTINATION variables
# See http://ros.org/doc/api/catkin/html/adv_user_guide/variables.html

## Mark executable scripts (Python etc.) for installation
## in contrast to setup.py, you can choose the destination
# install(PROGRAMS
#   scripts/my_python_script
#   DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark executables and/or libraries for installation
# install(TARGETS ${PROJECT_NAME} ${PROJECT_NAME}_node
#   ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark cpp header files for installation
# install(DIRECTORY include/${PROJECT_NAME}/
#   DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
#   FILES_MATCHING PATTERN "*.h"
#   PATTERN ".svn" EXCLUDE
# )

## Mark other files for installation (e.g. launch and bag files, etc.)
# install(FILES
#   # myfile1
#   # myfile2
#   DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
# )

#############
## Testing ##
#############

## Add gtest based cpp test target and link libraries
# catkin_add_gtest(${PROJECT_NAME}-test test/test_detection_tracking.cpp)
# if(TARGET ${PROJECT_NAME}-test)
#   target_link_libraries(${PROJECT_NAME}-test ${PROJECT_NAME})
# endif()

## Add folders to be run by python nosetests
# catkin_add_nosetests(test)
