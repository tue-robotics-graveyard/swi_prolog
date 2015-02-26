cmake_minimum_required(VERSION 2.8.3)
project(swi_prolog)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  roslib
  message_generation
)

add_message_files(
  FILES
  Bindings.msg
)

add_service_files(
  FILES
  Query.srv
)

generate_messages(
  DEPENDENCIES
  std_msgs
)

################################################
#                                              #
#       CATKIN SPECIFIC CONFIGURATION          #
#                                              #
################################################

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES swipl_ros
  CATKIN_DEPENDS roscpp roslib
)

################################################
#                                              #
#                    BUILD                     #
#                                              #
################################################

# Must have <SWI-Prolog.h>
INCLUDE(CheckIncludeFiles)
CHECK_INCLUDE_FILES ("/usr/lib/swi-prolog/include/SWI-Prolog.h" HAVE_SWI_PROLOG_H)
if(HAVE_SWI_PROLOG_H)
  message(STATUS "<SWI-Prolog.h> found")
  INCLUDE_DIRECTORIES("/usr/lib/swi-prolog/include/")
else(HAVE_SWI_PROLOG_H)
  message(FATAL_ERROR "Can't proceed without <SWI-Prolog.h>")
endif(HAVE_SWI_PROLOG_H)

find_library(SWIPL_LIB NAMES libpl.so libswipl.so)

message("${SWIPL_LIB}")

include_directories(
    include
    ${PROJECT_SOURCE_DIR}/3rdparty
    ${catkin_INCLUDE_DIRS}
)

add_library(swipl_ros
    include/reasoner/reasoner.h
    src/reasoner.cpp
)
target_link_libraries(swipl_ros ${SWIPL_LIB} ${catkin_LIBRARIES})

add_executable(swipl_server src/reasoner_server.cpp)
target_link_libraries(swipl_server swipl_ros)
add_dependencies(swipl_server ${PROJECT_NAME}_gencpp)

add_executable(test_swipl test/test_reasoner.cpp)
target_link_libraries(test_swipl swipl_ros)

