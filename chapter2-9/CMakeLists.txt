include_directories(.)

project(chapter2-9)

add_executable(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/src/glad.c main.cpp GLObject.h InclinedPlane.h Cube.h)

include_directories(${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR})
link_directories(${CMAKE_SOURCE_DIR}/lib)
target_link_libraries(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/lib/glfw3.dll)

file(COPY ${CMAKE_SOURCE_DIR}/lib/glfw3.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})