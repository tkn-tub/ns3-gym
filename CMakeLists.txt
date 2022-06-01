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

find_package(Protobuf 3.0.0)
if(NOT Protobuf_FOUND)
    message(STATUS "protobuf not found")
  return()
endif()

set(proto_source_files
    model/messages.proto
)

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${proto_source_files})
file(RELATIVE_PATH PROTO_HDRS_REL ${CMAKE_CURRENT_SOURCE_DIR} ${PROTO_HDRS})

# TODO generate python file and move to the right location

set(source_files
    helper/opengym-helper.cc
    model/container.cc
    model/opengym_env.cc
    model/opengym_interface.cc
    model/spaces.cc
    ${PROTO_SRCS}
)

set(header_files
    helper/opengym-helper.h
    model/container.h
    model/opengym_env.h
    model/opengym_interface.h
    model/spaces.h
    ${PROTO_HDRS_REL}
)

build_lib(
  LIBNAME opengym
  SOURCE_FILES ${source_files}
  HEADER_FILES ${header_files}
  LIBRARIES_TO_LINK
    ${libcore}
    ${ZMQ_LIBRARIES}
    ${Protobuf_LIBRARIES}
  TEST_SOURCES
    test/opengym-test-suite.cc
)

# add location of generated messages.pb.h to include directories
target_include_directories(
  ${libopengym-obj}
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)
