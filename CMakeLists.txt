CMAKE_MINIMUM_REQUIRED( VERSION 2.8 )

PROJECT(vidanueva)

# Copy resources needed
FILE(COPY debug-run.sh messages static-content DESTINATION .)

SET(DEBUG true)

IF( "${CMAKE_BUILD_TYPE}" STREQUAL Release )
  SET(DEBUG false)
ENDIF()

configure_file(wt-config.xml wt-config.xml)

# Sort out our libraries
find_library(wt wt)
find_library(wthttp wthttp)
find_library(mongoclient mongoclient)
find_library(ssl ssl) # open ssl for the sha1 for the passwords

# Boost
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
#set(Boost_COMPILER gcc45)
#set(Boost_REALPATH ON)
#set(DYLD_LIBRARY_PATH /opt/local/lib)
find_package(Boost 1.42.0 REQUIRED COMPONENTS system)

# Build the executable
ADD_EXECUTABLE(${PROJECT_NAME} App.cpp MainWindow.cpp LoginWindow.cpp UserManager.cpp)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${Boost_SYSTEM_LIBRARY} wt wthttp mongoclient ssl)
