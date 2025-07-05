cmake_minimum_required(VERSION 4.1.0)
project(minaEngine VERSION 0.1.0 LANGUAGES C CXX)

add_executable(minaEngine minaEngine/main.cpp)
set_property(TARGET minaEngine PROPERTY CXX_STANDARD 20)

if(WIN32)
  set_target_properties(minaEngine PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/win"
  )
elseif(EMSCRIPTEN)
  set_target_properties(minaEngine PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/web"
    LINK_FLAGS "-o ${CMAKE_SOURCE_DIR}/bin/web/minaEngine.js"
  )
endif()