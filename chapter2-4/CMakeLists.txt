include_directories(.)

add_executable(chapter2-4 ${PROJECT_SOURCE_DIR}/src/glad.c main.cpp GLObject.h)

include_directories(${PROJECT_SOURCE_DIR}/include)
link_directories(${PROJECT_SOURCE_DIR}/lib)
target_link_libraries(chapter2-4 ${PROJECT_SOURCE_DIR}/lib/glfw3.dll)

file(COPY ${CMAKE_SOURCE_DIR}/lib/glfw3.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
