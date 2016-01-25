PROJECT(example_app)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX11_FLAGS}")
INCLUDE_DIRECTORIES(
    ${CAVR_INCLUDES}
    "${cavrgl_SOURCE_DIR}/include"
    "${cavrgfx_SOURCE_DIR}/include"
    "${PROJECT_SOURCE_DIR}/IrrKlang/include"
)

LINK_DIRECTORIES("${PROJECT_SOURCE_DIR}/IrrKlang/lib")

ADD_EXECUTABLE(example_app app.cpp)


add_custom_target(BUILD_SUCCESSFUL ALL
                  DEPENDS example_app
                  COMMAND ${CMAKE_COMMAND} -E echo ""
                  COMMAND ${CMAKE_COMMAND} -E echo "====================="
                  COMMAND ${CMAKE_COMMAND} -E echo "  Compile complete!"
                  COMMAND ${CMAKE_COMMAND} -E echo "====================="
                  COMMAND ${CMAKE_COMMAND} -E echo "${CMAKE_CURRENT_BINARY_DIR}"
                  COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/shader.frag ${CMAKE_CURRENT_BINARY_DIR}
                  COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/shader.vert ${CMAKE_CURRENT_BINARY_DIR}
		  COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/media/ ${CMAKE_CURRENT_BINARY_DIR}/media
                 )

TARGET_LINK_LIBRARIES(example_app cavr cavrgl cavrgfx IrrKlang)
