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
SET(CAVR_CONFIG_LUA "${CMAKE_CURRENT_BINARY_DIR}/cavr_config_lua.cpp")
ADD_CUSTOM_COMMAND(OUTPUT "${CAVR_CONFIG_LUA}"
                   COMMAND ${SWIG_EXECUTABLE}
                   ARGS "-lua" "-includeall" "-c++"
                   "-o" "${CAVR_CONFIG_LUA}"
                   "${CMAKE_CURRENT_SOURCE_DIR}/cavr_config.i"
                   DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/cavr_config.i"
                   WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
SET_SOURCE_FILES_PROPERTIES("${CAVR_CONFIG_LUA}" GENERATED)
ADD_CUSTOM_TARGET(cavr_config_lua_cpp DEPENDS "${CAVR_CONFIG_LUA}")

FILE(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/cavr")
FILE(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/cavr/util")
SET(SWIG_LUART "${CMAKE_CURRENT_BINARY_DIR}/cavr/util/swigluart.h")

ADD_CUSTOM_COMMAND(OUTPUT "${SWIG_LUART}"
                   COMMAND ${SWIG_EXECUTABLE}
                   ARGS "-lua" "-external-runtime" "${SWIG_LUART}"
                   WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
                   VERBATIM)
SET_SOURCE_FILES_PROPERTIES("${SWIG_LUART}" GENERATED)
ADD_CUSTOM_TARGET(swigluart_header DEPENDS "${SWIG_LUART}")

SET(cavr_sources
    "source/input/analog.cpp"
    "source/input/input.cpp"
    "source/input/input_manager.cpp"
    "source/input/marker.cpp"
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
    "source/util/paths.cpp"
)

ADD_LIBRARY(cavr SHARED ${cavr_sources} ${CAVR_CONFIG_LUA})
ADD_DEPENDENCIES(cavr swigluart_header cavr_config_lua_cpp)
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
