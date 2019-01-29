cmake_minimum_required(VERSION 3.10.0)
project(multiagent)
#-DCMAKE_C_COMPILER=clang-5.0 -DCMAKE_CXX_COMPILER=clang++-5.0 gives all the weird __1 linking errors

set (AIRSIM_ROOT $ENV{AIRSIM_ROOT})

add_subdirectory("${AIRSIM_ROOT}/cmake/rpclib_wrapper" rpclib_wrapper)
add_subdirectory("${AIRSIM_ROOT}/cmake/AirLib" AirLib)
add_subdirectory("${AIRSIM_ROOT}/cmake/MavLinkCom" MavLinkCom)

LIST(APPEND CMAKE_MODULE_PATH "${AIRSIM_ROOT}/cmake")
LIST(APPEND CMAKE_MODULE_PATH "${AIRSIM_ROOT}/cmake/cmake-modules")
INCLUDE("${AIRSIM_ROOT}/cmake/cmake-modules/CommonSetup.cmake")
#CommonSetup()


#setup include and lib for rpclib which will be referenced by other projects
set(RPCLIB_VERSION_FOLDER rpclib-2.2.1)
set(RPC_LIB_INCLUDES " ${AIRSIM_ROOT}/external/rpclib/${RPCLIB_VERSION_FOLDER}/include")
#name of .a file with lib prefix
set(RPC_LIB rpc)
message(STATUS "found RPC_LIB_INCLUDES=${RPC_LIB_INCLUDES}")

include_directories(${AIRSIM_ROOT}/AirLib/deps/eigen3)

set(CMAKE_CXX_FLAGS "\
    -std=c++14 -ggdb -Wall -Wextra -Wstrict-aliasing -Wunreachable-code -Wcast-qual -Wctor-dtor-privacy \
    -Wdisabled-optimization -Wformat=2 -Winit-self -Wmissing-include-dirs -Wswitch-default \
    -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wstrict-overflow=5 -Wswitch-default -Wundef \
    -Wno-variadic-macros -Wno-parentheses -Wno-unused-function -Wno-unused -Wno-documentation -fdiagnostics-show-option \
    -pthread \
    ${RPC_LIB_DEFINES} ${CMAKE_CXX_FLAGS}")
      set(CXX_EXP_LIB "-lstdc++fs -fmax-errors=10 -Wnoexcept -Wstrict-null-sentinel")

      set(CMAKE_CXX_STANDARD 14)
      set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
 ${AIRSIM_ROOT}/../airsim-test
  ${AIRSIM_ROOT}/AirLib/deps/eigen3
  ${AIRSIM_ROOT}/AirLib/include
  ${RPC_LIB_INCLUDES}
  ${AIRSIM_ROOT}/MavLinkCom/include
  ${AIRSIM_ROOT}/MavLinkCom/common_utils
  $ENV{MADARA_ROOT}/include
  $ENV{CAPNP_ROOT}/c++/src
  $ENV{GAMS_ROOT}/src
)

link_directories(
  ${AIRSIM_ROOT}/build_debug/output/lib/
  $ENV{MADARA_ROOT}/lib
  $ENV{GAMS_ROOT}/lib
)

file(GLOB_RECURSE PROJECT_CPP "${AIRSIM_ROOT}/${PROJECT_NAME}/*.cpp")

set(PROJECT_CPP ${PROJECT_NAME}_sources)
file(GLOB_RECURSE PROJECT_CPP
  "algorithms/*.cpp"
  "platforms/*.cpp"
  "main.cpp"
)
add_executable(${PROJECT_NAME} ${PROJECT_CPP})

#add_executable(airsimtest "main.cpp")

#CommonTargetLink()

target_link_libraries(${PROJECT_NAME} AirLib)
target_link_libraries(${PROJECT_NAME} ${RPC_LIB})
target_link_libraries(${PROJECT_NAME} MavLinkCom)
target_link_libraries(${PROJECT_NAME} MADARA)
target_link_libraries(${PROJECT_NAME} GAMS)
target_link_libraries(${PROJECT_NAME} boost_system)
