cmake_minimum_required(VERSION 3.10)
project(minaEngine VERSION 0.1.0 LANGUAGES C CXX)

include(FetchContent)

add_executable(minaEngine WIN32 
  minaEngine/main.cpp
  minaEngine/engine/minaEngine.cpp
)
set_property(TARGET minaEngine PROPERTY CXX_STANDARD 20)

target_include_directories(minaEngine PRIVATE
  ${CMAKE_SOURCE_DIR}/dependencies/stb_image
)
FetchContent_Declare(
	glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG 1.0.1
)
FetchContent_MakeAvailable(glm)

set(MINIFB_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(dependencies/minifb)
target_link_libraries(minaEngine 
  minifb
  glm::glm-header-only
)

if(WIN32)
  set(OUTPUT_DIR "${CMAKE_SOURCE_DIR}/bin/win")
  set_target_properties(minaEngine PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/win"
  )

  set(ASSETS_SOURCE_DIR "${CMAKE_SOURCE_DIR}/assets")
  set(ASSETS_DEST_DIR "${OUTPUT_DIR}/assets")
  add_custom_command(
    TARGET minaEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${ASSETS_SOURCE_DIR}" "${ASSETS_DEST_DIR}"
  )
elseif(EMSCRIPTEN)
  set(OUTPUT_DIR "${CMAKE_SOURCE_DIR}/bin/web")

  set_target_properties(minaEngine PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}"
  )

  target_link_options(minaEngine PRIVATE
    "-sASYNCIFY=1"
    "-sEXPORTED_RUNTIME_METHODS=['requestFullscreen']"
    "--preload-file=${CMAKE_SOURCE_DIR}/assets@assets"
    "-o${OUTPUT_DIR}/minaEngine.js"
  )
endif()