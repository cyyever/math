file(GLOB sources ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
set(test_sources
    "${test_sources};${sources}"
    PARENT_SCOPE)
