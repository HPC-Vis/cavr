FIND_PACKAGE(Lua51 REQUIRED)
FIND_PACKAGE(GLOG REQUIRED)

INCLUDE_DIRECTORIES(
    ${gtest_SOURCE_DIR}/include
    ${cavr_SOURCE_DIR}/include
    ${LUA_INCLUDE_DIR}
    ${CAVR_INCLUDES}
)
SET(test_sources
    "cavr/config/config_test.cpp"
    "cavr/config/configuration_specification_test.cpp"
    "cavr/config/lua_reader_test.cpp"
    "cavr/config/parameter_specification_test.cpp"
    "cavr/input/analog_test.cpp"
    "cavr/input/input_test.cpp"
    "cavr/input/sixdof_test.cpp"
    "cavr/input/switch_test.cpp"
    "cavr/math/vector_test.cpp"
    "cavr/math/matrix_test.cpp"
    "cavr/util/lua_state_test.cpp"
    "cavr/util/string_test.cpp"
    "etc/cavr/schema/schema_test.cpp"
)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX11_FLAGS}")
ADD_EXECUTABLE(tests ${test_sources})
TARGET_LINK_LIBRARIES(tests gtest_main cavr ${LUA_LIBRARIES} ${GLOG_LIBRARIES})
