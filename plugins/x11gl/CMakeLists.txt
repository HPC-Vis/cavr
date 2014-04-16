PROJECT(x11gl)
INCLUDE_DIRECTORIES(
    ${CAVR_INCLUDES}
    "${cavrgfx_SOURCE_DIR}/include"
    "${cavrgl_SOURCE_DIR}/include"
)

FIND_PACKAGE(X11 REQUIRED)
FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE(GLEW REQUIRED)

SET(x11gl_sources
    "x11gl.cpp"
    "window.cpp"
)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX11_FLAGS}")
ADD_LIBRARY(x11gl MODULE ${x11gl_sources})
ADD_DEPENDENCIES(x11gl cavr)
SET_TARGET_PROPERTIES(x11gl PROPERTIES LINKER_LANGUAGE CXX)
INSTALL(TARGETS x11gl
        LIBRARY DESTINATION "${CAVR_PLUGIN_PATH}")
TARGET_LINK_LIBRARIES(x11gl 
                      cavr 
                      cavrgl
                      cavrgfx
                      ${X11_LIBRARIES} 
                      ${OPENGL_LIBRARY} 
                      ${GLEW_LIBRARIES})
