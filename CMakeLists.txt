cmake_minimum_required(VERSION 3.13)

project(MyMathLib LANGUAGES CXX)

include(cmake/all.cmake)

file(GLOB SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp  ${CMAKE_CURRENT_SOURCE_DIR}/src/*/*.cpp)

add_library(MyMathLib ${SOURCE})

# test
option(BUILD_TEST "Build test" ON)
if(BUILD_TEST)
  add_subdirectory(test)
endif()

# install lib
install(TARGETS MyMathLib EXPORT ${PROJECT_NAME}Targets
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  COMPONENT library
  INCLUDES DESTINATION include
  )

install(DIRECTORY src/ DESTINATION include/cyy/math
  FILES_MATCHING PATTERN "*.hpp")

# install cmake config file
install(EXPORT ${PROJECT_NAME}Targets
  FILE ${PROJECT_NAME}Config.cmake
  DESTINATION lib/cmake/${PROJECT_NAME})
