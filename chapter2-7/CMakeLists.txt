include_directories(.)

add_executable(chapter2-7 ${PROJECT_SOURCE_DIR}/src/glad.c main.cpp GLObject.h TextureRectangle.h MutiTextureRectangle.h)

include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR})
link_directories(${PROJECT_SOURCE_DIR}/lib)
target_link_libraries(chapter2-7 ${PROJECT_SOURCE_DIR}/lib/glfw3.dll)

file(COPY ${CMAKE_SOURCE_DIR}/lib/glfw3.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})