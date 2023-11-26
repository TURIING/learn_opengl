include_directories(.)

project(chapter3-3)

file(GLOB_RECURSE SRC CONFIGURE_DEPENDS ./*.h ./*.cpp)
add_executable(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/src/glad.c ${SRC})

include_directories(${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR})
link_directories(${CMAKE_SOURCE_DIR}/lib)
target_link_libraries(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/lib/glfw3.dll)

file(COPY ${CMAKE_SOURCE_DIR}/lib/glfw3.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})