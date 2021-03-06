CMAKE_MINIMUM_REQUIRED( VERSION 2.8 )

PROJECT(vidanueva)

ADD_DEFINITIONS(-Wall -Wextra)

# Copy resources needed
FILE(COPY debug-run.sh run.sh messages static-content DESTINATION .)

SET(DEBUG true)

IF( "${CMAKE_BUILD_TYPE}" STREQUAL Release )
  SET(DEBUG false)
ENDIF()

configure_file(wt-config.xml wt-config.xml)

# Sort out our libraries
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    find_library(wt wtd)
    find_library(wthttp wthttpd)
else()
    find_library(wt wt)
    find_library(wthttp wthttp)
endif()
find_library(mongoclient mongoclient)
find_library(ssl ssl) # open ssl for the sha1 for the passwords

# Boost
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
#set(Boost_COMPILER gcc45)
set(Boost_REALPATH ON)
#set(DYLD_LIBRARY_PATH /opt/local/lib)
find_package(Boost 1.42.0 REQUIRED COMPONENTS system program_options signals date_time filesystem regex)
include_directories(BEFORE ${Boost_INCLUDE_DIRS})

# Build the executable
ADD_EXECUTABLE(${PROJECT_NAME} App.cpp
    MainWindow.cpp LoginWindow.cpp UserManager.cpp ControlPanel.cpp Page.cpp PageEdit.cpp
    UserManager.cpp SessionStore.cpp SessionHandle.cpp)
TARGET_LINK_LIBRARIES(${PROJECT_NAME}
    ${Boost_SYSTEM_LIBRARY} ${Boost_PROGRAM_OPTIONS_LIBRARY} ${Boost_SIGNALS_LIBRARY}
    ${Boost_DATE_TIME_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} ${Boost_REGEX_LIBRARY}
    ${wt} ${wthttp} mongoclient ssl)

# Just to get started, this program will set up the DB
ADD_EXECUTABLE(setupDB setupDB.cpp UserManager.cpp)
TARGET_LINK_LIBRARIES(setupDB ${Boost_SYSTEM_LIBRARY} mongoclient ssl)
