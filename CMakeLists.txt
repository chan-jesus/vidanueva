CMAKE_MINIMUM_REQUIRED( VERSION 2.8 )

PROJECT(vidanueva)

# Copy resources needed
FILE(COPY debug-run.sh vidanueva.xml static-content DESTINATION .)

SET(DEBUG true)

IF( "${CMAKE_BUILD_TYPE}" STREQUAL Release )
  SET(DEBUG false)
ENDIF()

configure_file(wt-config.xml wt-config.xml)

# Sort out our libraries
find_library(wt wt)
find_library(wthttp wthttp)

# Build the executable
ADD_EXECUTABLE(${PROJECT_NAME} App.cpp)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} wt wthttp)
