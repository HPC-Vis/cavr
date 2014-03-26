PROJECT(cavr)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX11_FLAGS}")

FIND_PACKAGE(Lua51 REQUIRED)
FIND_PACKAGE(SWIG REQUIRED)
FIND_PACKAGE(GLOG REQUIRED)

INCLUDE_DIRECTORIES(
    "include" 
    "${CMAKE_CURRENT_BINARY_DIR}" 
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${LUA_INCLUDE_DIR}"
    "${GLOG_INCLUDE_DIRS}"
)
INCLUDE(${SWIG_USE_FILE})
FIND_PACKAGE(Protobuf REQUIRED)
SET(CMAKE_SWIG_FLAGS "")
SET_SOURCE_FILES_PROPERTIES("cavr_config.i" PROPERTIES CPLUSPLUS ON)
SET_SOURCE_FILES_PROPERTIES("cavr_config.i" PROPERTIES SWIG_FLAGS "-includeall")
SET(SWIG_MODULE_cavr_config_EXTRA_DEPS
    "include/cavr/config/config.h"
)
SWIG_ADD_MODULE(cavr_config lua "cavr_config.i")
SET(cavr_config_INCLUDE_DIRS ${LUA_INCLUDE_DIR})
TARGET_LINK_LIBRARIES(cavr_config ${LUA_LIBRARIES})


SET(cavr_sources
    "source/input/analog.cpp"
    "source/input/input.cpp"
    "source/input/sixdof.cpp"
    "source/input/switch.cpp"
    "source/math/matrix.cpp"
    "source/math/vector.cpp"
    "source/config/config.cpp"
    "source/config/configuration_specification.cpp"
    "source/config/lua_reader.cpp"
    "source/config/parameter_specification.cpp"
    "source/util/file.cpp"
    "source/util/lua_state.cpp"
)

ADD_LIBRARY(cavr SHARED ${cavr_sources})
TARGET_LINK_LIBRARIES(cavr
                      ${PROTOBUF_LIBRARIES}
                      ${GLOG_LIBRARIES}
                      ${LUA_LIBRARIES})
SET_TARGET_PROPERTIES(cavr PROPERTIES LINKER_LANGUAGE CXX)

INSTALL(DIRECTORY include/cavr
        DESTINATION include
        FILES_MATCHING PATTERN "*.h")

IF(UNIX)
#	TARGET_LINK_LIBRARIES(cavr_spec pthread dl)
#	TARGET_LINK_LIBRARIES(cavr_sim pthread dl ${ADDITIONAL_LIBS})
#	INSTALL(TARGETS cavr_spec cavr_sim LIBRARY DESTINATION lib)
ENDIF(UNIX)
