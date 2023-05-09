include(FindPkgConfig)
if(NOT PKG_CONFIG_FOUND)
    message(STATUS "pkgconf not found")
    return()
endif()

pkg_check_modules(ZMQ libzmq)
if(NOT ZMQ_FOUND)
    message(STATUS "zeromq not found")
    return()
endif()

find_package(Protobuf REQUIRED)
if(NOT Protobuf_FOUND)
    message(STATUS "protobuf not found")
  return()
endif()

set(proto_source_files
    model/messages.proto
)

set(source_files
    helper/opengym-helper.cc
    model/container.cc
    model/opengym_env.cc
    model/opengym_interface.cc
    model/spaces.cc
    ${proto_source_files}
)

set(header_files
    helper/opengym-helper.h
    model/container.h
    model/opengym_env.h
    model/opengym_interface.h
    model/spaces.h
)

build_lib(
  LIBNAME opengym
  SOURCE_FILES ${source_files}
  HEADER_FILES ${header_files}
  LIBRARIES_TO_LINK
    ${libcore}
    ${ZMQ_LIBRARIES}
    protobuf::libprotobuf
  TEST_SOURCES
    test/opengym-test-suite.cc
)

# need protobuf_generate func to generate messages
check_function_exists(protobuf_generate protobuf_generate_exists)
if(${protobuf_generate_exists})
    message(STATUS "protobuf_generate command found")
else()
  message(STATUS "protobuf_generate command not found -> use a local copy from ${CMAKE_CURRENT_SOURCE_DIR}/protobuf-generate.cmake")
  include(${CMAKE_CURRENT_SOURCE_DIR}/protobuf-generate.cmake)
endif()


protobuf_generate(
  TARGET ${libopengym-obj}
  IMPORT_DIRS model/
  LANGUAGE cpp
  PROTOC_OUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/model
)

protobuf_generate(
  TARGET ${libopengym-obj}
  IMPORT_DIRS model/
  LANGUAGE python
  PROTOC_OUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/model/ns3gym/ns3gym
)
