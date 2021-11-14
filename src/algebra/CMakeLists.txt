find_package(Eigen3)

if(NOT TARGET Eigen3::Eigen)
  return()
endif()
file(GLOB SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
target_sources(CyyMathLib PRIVATE ${SOURCE})
target_link_libraries(CyyMathLib PUBLIC Eigen3::Eigen)
