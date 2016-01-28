PROJECT(cavrgfx)
SET(sources
    "source/ray.cpp"
    "source/renderer.cpp"
    "source/shapes.cpp"
    "source/sphere.cpp"
)

INCLUDE_DIRECTORIES(
    "include"
    ${CAVR_INCLUDES}
)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX11_FLAGS}")
ADD_LIBRARY(cavrgfx SHARED ${sources})
ADD_DEPENDENCIES(cavrgfx cavr)
SET_TARGET_PROPERTIES(cavrgfx PROPERTIES LINKER_LANGUAGE CXX)
INSTALL(TARGETS cavrgfx LIBRARY DESTINATION lib)
INSTALL(DIRECTORY include/cavr
        DESTINATION include
        FILES_MATCHING PATTERN "*.h")
TARGET_LINK_LIBRARIES(cavrgfx cavr ${OPENGL_LIBRARY})

