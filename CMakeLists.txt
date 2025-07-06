cmake_minimum_required(VERSION 3.10)
project(minaEngine VERSION 0.1.0 LANGUAGES C CXX)

add_executable(minaEngine minaEngine/main.cpp)
set_property(TARGET minaEngine PROPERTY CXX_STANDARD 20)

set(MINIFB_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(dependencies/minifb)
target_link_libraries(minaEngine minifb)

if(WIN32)
  set_target_properties(minaEngine PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/win"
  )
elseif(EMSCRIPTEN)
  #set(CMAKE_EXECUTABLE_SUFFIX ".html")
  set(ASYNCIFY_FLAGS "-s ASYNCIFY=1")
  set(EXPORTED_METHODS "-s EXPORTED_RUNTIME_METHODS=['requestFullscreen']")
  set_target_properties(minaEngine PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/web"
    LINK_FLAGS "-o ${CMAKE_SOURCE_DIR}/bin/web/minaEngine.js ${ASYNCIFY_FLAGS} ${EXPORTED_METHODS}"
  )
endif()