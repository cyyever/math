cmake_minimum_required(VERSION 3.15)

project(CyyMathLib LANGUAGES CXX)

include(cmake/all.cmake)

find_package(range-v3 REQUIRED)

set(DATA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/data")
set(INSTALLED_DATA_DIR "${CMAKE_INSTALL_PREFIX}/lib")

file(GLOB SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_library(CyyMathLib ${SOURCE})

target_link_libraries(CyyMathLib PUBLIC range-v3)
add_subdirectory(src/algebra)

target_include_directories(
  CyyMathLib PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
                    $<INSTALL_INTERFACE:include/cyy/math>)
configure_file(src/config.hpp.in config.hpp)
target_include_directories(CyyMathLib PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

add_custom_command(
  OUTPUT ${DATA_DIR}/prime.txt
  COMMAND ${CMAKE_COMMAND} -E tar x prime.tar.xz
  COMMAND ${CMAKE_COMMAND} -E touch prime.txt
  DEPENDS ${DATA_DIR}/prime.tar.xz
  WORKING_DIRECTORY ${DATA_DIR})

add_custom_target(decompress_data DEPENDS ${DATA_DIR}/prime.txt)
add_dependencies(CyyMathLib decompress_data)

# test
add_subdirectory(test)

# install lib
install(
  TARGETS CyyMathLib
  EXPORT CyyMathLibConfig
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib COMPONENT library
  INCLUDES
  DESTINATION include)

install(
  DIRECTORY src/
  DESTINATION include/cyy/math
  FILES_MATCHING
  PATTERN "*.hpp")

install(FILES ${DATA_DIR}/prime.txt DESTINATION lib)

# install cmake config file
install(EXPORT CyyMathLibConfig DESTINATION lib/cmake/CyyMathLib)
