cmake_minimum_required(VERSION 3.18)
project(CaffeineForest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)

# ---- Fetch GLM ----
FetchContent_Declare(glm URL https://github.com/g-truc/glm/archive/refs/tags/0.9.9.8.zip)
FetchContent_MakeAvailable(glm)

# ---- Fetch GLFW ----
FetchContent_Declare(glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.3.8
)
FetchContent_MakeAvailable(glfw)

# ---- Fetch GLAD ----
FetchContent_Declare(glad
  GIT_REPOSITORY https://github.com/Dav1dde/glad.git
  GIT_TAG v0.1.36
)
FetchContent_MakeAvailable(glad)

# ---- Fetch ImGui ----
FetchContent_Declare(imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG v1.89.8
)
FetchContent_MakeAvailable(imgui)

# ---- Fetch Assimp ----
FetchContent_Declare(assimp
  GIT_REPOSITORY https://github.com/assimp/assimp.git
  GIT_TAG v5.2.5
)
FetchContent_MakeAvailable(assimp)

# ---- Fetch stb_image ----
FetchContent_Declare(stb
  GIT_REPOSITORY https://github.com/nothings/stb.git
  GIT_TAG master
)
FetchContent_MakeAvailable(stb)

# ---- Project sources ----
file(GLOB_RECURSE SRC src/*.cpp src/*.cxx src/*.cc src/*.c)
add_executable(${PROJECT_NAME} ${SRC})

# include dirs
target_include_directories(${PROJECT_NAME} PRIVATE
  ${glad_SOURCE_DIR}/include
  ${glfw_SOURCE_DIR}/include
  ${imgui_SOURCE_DIR}
  ${glm_SOURCE_DIR}
  ${assimp_SOURCE_DIR}/include
  ${stb_SOURCE_DIR}
  src
)

# link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
  glfw
  assimp
)

# GLAD - check if generated loader exists
if(EXISTS ${CMAKE_SOURCE_DIR}/src/glad/src/glad.c)
    add_library(glad STATIC ${CMAKE_SOURCE_DIR}/src/glad/src/glad.c)
    target_include_directories(glad PUBLIC ${CMAKE_SOURCE_DIR}/src/glad/include)
    target_link_libraries(${PROJECT_NAME} PRIVATE glad)
    message(STATUS "Using GLAD from src/glad/")
else()
    message(WARNING "GLAD loader not found. Please generate it:")
    message(WARNING "  pip install glad")
    message(WARNING "  glad --generator=c --spec=gl --api=gl=4.5 --profile=core --out-path=src/glad")
    message(WARNING "Or see BUILD.md for instructions")
    # Try to use GLAD from FetchContent if available
    if(EXISTS ${glad_SOURCE_DIR}/src/glad.c)
        add_library(glad STATIC ${glad_SOURCE_DIR}/src/glad.c)
        target_include_directories(glad PUBLIC ${glad_SOURCE_DIR}/include)
        target_link_libraries(${PROJECT_NAME} PRIVATE glad)
        message(STATUS "Using GLAD from FetchContent")
    endif()
endif()

# ImGui build
add_subdirectory(${imgui_SOURCE_DIR} ${CMAKE_BINARY_DIR}/imgui_build)
target_link_libraries(${PROJECT_NAME} PRIVATE imgui)

# platform specifics
if (WIN32)
  target_link_libraries(${PROJECT_NAME} PRIVATE opengl32)
elseif(APPLE)
  find_library(COCOA Cocoa)
  find_library(IOKIT IOKit)
  find_library(COREVIDEO CoreVideo)
  find_library(OPENGL OpenGL)
  target_link_libraries(${PROJECT_NAME} PRIVATE ${COCOA} ${IOKIT} ${COREVIDEO} ${OPENGL})
else()
  find_package(OpenGL REQUIRED)
  target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENGL_gl_LIBRARY})
  target_link_libraries(${PROJECT_NAME} PRIVATE dl pthread)
endif()

# copy assets post build
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
  COMMENT "Copying assets to build directory"
)

