cmake_minimum_required(VERSION 4.0.0)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "a9e1cf81-9932-4810-974b-6eccaf14e457")
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_MODULE_STD 1)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_C_COMPILER clang)
set(CMAKE_C_FLAGS -stdlib=libc)
set(CMAKE_CXX_FLAGS -stdlib=libc++)


project(CyyMathLib LANGUAGES CXX)

include(cmake/all.cmake)

find_package(Boost REQUIRED)

set(DATA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/data")
set(INSTALLED_DATA_DIR "${CMAKE_INSTALL_PREFIX}/lib")

file(GLOB SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_library(CyyMathLib ${SOURCE})

target_link_libraries(CyyMathLib PUBLIC Boost::headers)
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

option(BUILD_FUZZING "Build fuzzing" OFF)
if(BUILD_FUZZING)
  add_subdirectory(fuzz_test)
endif()

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
