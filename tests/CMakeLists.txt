INCLUDE_DIRECTORIES(
    ${gtest_SOURCE_DIR}/include
    ${cavr_SOURCE_DIR}/include
)
SET(test_sources
    "cavr/math/vector_test.cpp"
)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX11_FLAGS}")
ADD_EXECUTABLE(tests ${test_sources})
TARGET_LINK_LIBRARIES(tests gtest_main)