find_package(doctest REQUIRED)

file(GLOB test_sources ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
     ${CMAKE_CURRENT_SOURCE_DIR}/la/*.cpp)

foreach(test_source IN LISTS test_sources)
  get_filename_component(test_prog ${test_source} NAME_WE)
  add_executable(${test_prog} ${test_source})
  target_link_libraries(${test_prog} PRIVATE CyyMathLib)
  target_link_libraries(${test_prog} PRIVATE doctest::doctest)
  target_include_directories(${test_prog}
                             PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)
  target_compile_definitions(${test_prog} PRIVATE _USE_MATH_DEFINES
                             DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN)
  add_test_with_runtime_analysis(TARGET ${test_prog} TSAN OFF)
endforeach()
