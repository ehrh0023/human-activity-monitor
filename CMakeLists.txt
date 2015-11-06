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
# Force Static Lib 
# http://stackoverflow.com/questions/10113017/setting-the-msvc-runtime-in-cmake
#########################################################

macro(configure_msvc_runtime)
  if(MSVC)
    # Default to statically-linked runtime.
    if("${MSVC_RUNTIME}" STREQUAL "")
      set(MSVC_RUNTIME "static")
    endif()
    # Set compiler options.
    set(variables
      CMAKE_C_FLAGS_DEBUG
      CMAKE_C_FLAGS_MINSIZEREL
      CMAKE_C_FLAGS_RELEASE
      CMAKE_C_FLAGS_RELWITHDEBINFO
      CMAKE_CXX_FLAGS_DEBUG
      CMAKE_CXX_FLAGS_MINSIZEREL
      CMAKE_CXX_FLAGS_RELEASE
      CMAKE_CXX_FLAGS_RELWITHDEBINFO
    )
    if(${MSVC_RUNTIME} STREQUAL "static")
      message(STATUS
        "MSVC -> forcing use of statically-linked runtime."
      )
      foreach(variable ${variables})
        if(${variable} MATCHES "/MD")
          string(REGEX REPLACE "/MD" "/MT" ${variable} "${${variable}}")
        endif()
      endforeach()
    else()
      message(STATUS
        "MSVC -> forcing use of dynamically-linked runtime."
      )
      foreach(variable ${variables})
        if(${variable} MATCHES "/MT")
          string(REGEX REPLACE "/MT" "/MD" ${variable} "${${variable}}")
        endif()
      endforeach()
    endif()
  endif()
endmacro()

configure_msvc_runtime()

#########################################################
# FIND OPENCV
#########################################################
if(WIN32)
  set(OpenCV_DIR "lib/opencv/x86/vc12/staticlib")
else()
  set(OpenCV_DIR "/usr/lib/opencv")
endif()
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

#########################################################
# Create Human Activity Monitor
#########################################################
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

target_link_libraries(human-activity-monitor ${OpenCV_LIBS})