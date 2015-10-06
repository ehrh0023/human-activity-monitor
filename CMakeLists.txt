CMAKE_MINIMUM_REQUIRED(VERSION 2.8.8 FATAL_ERROR)
PROJECT(human-activity-monitor)

# setup version numbers
set(VERSION_MAJOR 0)
set(VERSION_MINOR 0000001)
set(VERSION_PATCH 0)

set(INSTALL_LIB_DIR lib ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

#########################################################
# FIND fuzzylite
#########################################################
find_package(fuzzylite REQUIRED)
include_directories(${FUZZYLITE_INCLUDE_DIRS})
link_directories(${FUZZYLITE_LIBRARY_DIRS})
add_definitions(${FUZZYLITE_DEFINITIONS})
if(NOT GLUT_FOUND)
    message(ERROR " fuzzylite not found!")
endif(NOT GLUT_FOUND)

file(GLOB_RECURSE HUMAN_ACTIVITY_MONITOR_HEADERS
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
)

file(GLOB_RECURSE HUMAN_ACTIVITY_MONITOR_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

SET(HUMAN_ACTIVITY_MONITOR_FILES ${HUMAN_ACTIVITY_MONITOR_HEADERS} ${HUMAN_ACTIVITY_MONITOR_SRCS})

IF(WIN32)
    # hide insecure CRT warnings, common practice
    ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)
    IF(MSVC)
        SET( CMAKE_DEBUG_POSTFIX "d" )
    ENDIF(MSVC)
    
    # enable the use of Win2000 APIs (needed for really old compilers like MSVC6)
    ADD_DEFINITIONS(-D_WIN32_WINNT=0x0500)
    ADD_DEFINITIONS(-DWINVER=0x0500)
ENDIF()

foreach(f ${HUMAN_ACTIVITY_MONITOR_FILES})
    # Get the path of the file relative to ${DIRECTORY},
    # then alter it (not compulsory)
    file(RELATIVE_PATH SRCGR ${CMAKE_CURRENT_SOURCE_DIR} ${f})
    set(SRCGR ${SRCGR})

    # Extract the folder, ie remove the filename part
    string(REGEX REPLACE "(.*)(/[^/]*)$" "\\1" SRCGR ${SRCGR})

    # Source_group expects \\ (double antislash), not / (slash)
    string(REPLACE / \\ SRCGR ${SRCGR})
    source_group("${SRCGR}" FILES ${f})
endforeach()

ADD_EXECUTABLE(human-activity-monitor ${HUMAN_ACTIVITY_MONITOR_FILES})

target_link_libraries(flaming-archer ${FUZZYLITE_LIBRARIES})