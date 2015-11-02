cmake_minimum_required(VERSION 2.8.11)
project(orione)
enable_testing()

message( STATUS "Orion Embeddable: " ${PROJECT_SRC_DIR} )


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/lib)
set(CMAKE_FILES_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/include)
SET(BASEPATH "${CMAKE_SOURCE_DIR}")
INCLUDE_DIRECTORIES("${BASEPATH}")

include_directories(${CMAKE_SOURCE_DIR}/src/Hlprs)
include_directories(${CMAKE_SOURCE_DIR}/src/Thrft)
include_directories(${CMAKE_SOURCE_DIR}/src/Engne)
include_directories(${CMAKE_SOURCE_DIR}/src/Extrn)
include_directories(${CMAKE_SOURCE_DIR}/src/Proto)
include_directories(${CMAKE_SOURCE_DIR}/src/Extrn/md5)
include_directories(${CMAKE_SOURCE_DIR}/src/Ordbc)
include_directories(${CMAKE_SOURCE_DIR}/src/Mains)

find_path(THRIFT_INCLUDE_DIR
    NAMES
        thrift/Thrift.h
    HINTS
        ${THRIFT_HOME}
        ENV THRIFT_HOME
        /usr/local
        /opt/local
    PATH_SUFFIXES
        include
)

include_directories(${THRIFT_INCLUDE_DIR}/thrift)


MESSAGE ( STATUS "THRIFT PATH ${THRIFT_INCLUDE_DIR}")


#include_directories(/opt/gtest/include)
#include_directories(/usr/local/Cellar/thrift/0.9.2/include)
#include_directories(/usr/local/Cellar/thrift/0.9.2/include/thrift)
#include_directories(/usr/local/Cellar/libevent/2.0.22/include)
#include_directories(/opt/liblfds/include)
#include_directories(/usr/local/include)
#link_directories( /opt/gtest/lib/ )

add_definitions(-DsCpipename='"ORIONPIPE_"' -DAPPLEOS  -DiCfastpool -DHAVE_NETINET_IN_H -DHAVE_INTTYPES_H -DiCtcmalloc -DiCoo2sortedindex -DVERBOSE=1)

set(CMAKE_CXX_COMPILER /usr/bin/g++)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -g -w -fmessage-length=0 -std=c++0x -static-libstdc++ -stdlib=libc++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -g -w -fmessage-length=0 -std=c++0x -static-libstdc++")
set(CMAKE_INSTALL_PREFIX dist)
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/debug)
add_definitions(-fPIC)


set_target_properties( ${targetname} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/Debug )

FILE(GLOB SOURCE_FILES  "${CMAKE_SOURCE_DIR}/src/Engne/*.cpp"
                        "${CMAKE_SOURCE_DIR}/src/Hlprs/*.cpp"
			"${CMAKE_SOURCE_DIR}/src/Thrft/*.cpp"
			"${CMAKE_SOURCE_DIR}/src/Extrn/md5/*.cpp"
			"${CMAKE_SOURCE_DIR}/src/Extrn/*.cpp"
			"${CMAKE_SOURCE_DIR}/src/Proto/*.cc"
			#"${CMAKE_SOURCE_DIR}/src/Unitt/*.cpp"
			"${CMAKE_SOURCE_DIR}/makefiles/orione/*.cpp"
                        )

#message( STATUS ${CMAKE_SOURCE_DIR} )
#message( STATUS ${SOURCE_FILES} )

list(REMOVE_ITEM SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/Hlprs/TmplFlqu.cpp)
list(REMOVE_ITEM SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/Hlprs/TmplClps.cpp)
list(REMOVE_ITEM SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/Hlprs/TmplFsys.cpp)
list(REMOVE_ITEM SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/Hlprs/TmplList.cpp)
list(REMOVE_ITEM SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/Hlprs/TmplSdel.cpp)
list(REMOVE_ITEM SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/Hlprs/TmplSdmg.cpp)
list(REMOVE_ITEM SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/Hlprs/TmplSdmq.cpp)
list(REMOVE_ITEM SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/Hlprs/TmplOo2w.cpp)
list(REMOVE_ITEM SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/Hlprs/TmplOo2t.cpp)
list(REMOVE_ITEM SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/Hlprs/TmplClps.cpp)
list(REMOVE_ITEM SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/Hlprs/TmplShst.cpp)
list(REMOVE_ITEM SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/Hlprs/TmplCche.cpp)
list(REMOVE_ITEM SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/Hlprs/TmplLiss.cpp)
list(REMOVE_ITEM SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/Hlprs/TmplLssc.cpp)
list(REMOVE_ITEM SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/Hlprs/TmplLstc.cpp)
list(REMOVE_ITEM SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/Hlprs/TmplObpl.cpp)
list(REMOVE_ITEM SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/Hlprs/TmplLstc.cpp)
list(REMOVE_ITEM SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/Hlprs/TmplObpl.cpp)
list(REMOVE_ITEM SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/Hlprs/TmplTsrl.cpp)
list(REMOVE_ITEM SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/Hlprs/CoreTevv.cpp)
list(REMOVE_ITEM SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/Hlprs/CoreSmph.cpp)
list(REMOVE_ITEM SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/Hlprs/CoreSdmq.cpp)
list(REMOVE_ITEM SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/Hlprs/CoreHutv.cpp)
list(REMOVE_ITEM SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/Hlprs/CoreSckw.cpp)
list(REMOVE_ITEM SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/Hlprs/CoreScpw.cpp)
#list(REMOVE_ITEM SOURCE_FILES ${CMAKE_SOURCE_DIR}/Hlprs/CoreShme.cpp)
list(REMOVE_ITEM SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/Mains/CoreOdbb.cpp)
list(REMOVE_ITEM SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/Hlprs/CoreSmpq.cpp)
list(REMOVE_ITEM SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/Hlprs/CoreXutv.cpp)
list(REMOVE_ITEM SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/Hlprs/CoreTthv.cpp)
list(REMOVE_ITEM SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/Mains/Ordbc.cpp)
list(REMOVE_ITEM SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/Thrft/ThrfOrn__server.skeleton.cpp)

# Create the Static Library
add_definitions( -DiCdisableprintlog )

set(LIB_PATH_DIRS /usr/lib /usr/lib64 /usr/local/lib /usr/local/lib64 )


# Static Dependencies
find_library( THRIFT_STATIC_PATH NAMES libthrift.a PATHS ${LIB_PATH_DIRS} REQUIRED)
find_library( THRIFTNB_STATIC_PATH NAMES libthriftnb.a PATHS ${LIB_PATH_DIRS} REQUIRED)
find_library( EVENT_STATIC_PATH NAMES libevent.a PATHS ${LIB_PATH_DIRS} REQUIRED)
find_library( SNAPPY_STATIC_PATH NAMES libsnappy.a PATHS ${LIB_PATH_DIRS} REQUIRED)
find_library( TCMALLOC_STATIC_PATH NAMES libtcmalloc.a PATHS ${LIB_PATH_DIRS} REQUIRED)
find_library( RE2_STATIC_PATH NAMES libre2.a PATHS ${LIB_PATH_DIRS} REQUIRED)
find_library( PROTOBUF_STATIC_PATH NAMES libprotobuf.a PATHS ${LIB_PATH_DIRS} REQUIRED)
find_library( GFLAGS_STATIC_PATH NAMES libgflags.a PATHS ${LIB_PATH_DIRS} REQUIRED)
find_library( GLOG_STATIC_PATH NAMES libglog.a PATHS ${LIB_PATH_DIRS} REQUIRED)
find_library( BOOST_SYSTEM_STATIC_PATH NAMES libboost_system.a PATHS ${LIB_PATH_DIRS} REQUIRED)
find_library( BOOST_FILESYSTEM_STATIC_PATH NAMES libboost_filesystem.a PATHS ${LIB_PATH_DIRS} REQUIRED)
#find_library( PTHREAD_STATIC_PATH NAMES libpthread.a PATHS ${LIB_PATH_DIRS} REQUIRED)

MESSAGE(STATUS "THRIFT ${THRIFT_STATIC_PATH}")
MESSAGE(STATUS "TCMALLOC ${SNAPPY_STATIC_PATH}")
#MESSAGE(STATUS "PTHREAD ${PTHREAD_STATIC_PATH}")


set(STATIC_DEPENDENCIES ${THRIFT_STATIC_PATH} ${THRIFTNB_STATIC_PATH} ${EVENT_STATIC_PATH} ${SNAPPY_STATIC_PATH} ${RE2_STATIC_PATH} ${TCMALLOC_STATIC_PATH} pthread ${PROTOBUF_STATIC_PATH} ${GFLAGS_STATIC_PATH} ${GLOG_STATIC_PATH} ${BOOST_SYSTEM_STATIC_PATH} ${BOOST_FILESYSTEM_STATIC_PATH})

# Executable
add_executable(orion ${SOURCE_FILES})
target_link_libraries (orion ${STATIC_DEPENDENCIES})


# Library
add_library(orione SHARED ${SOURCE_FILES})
add_library(orione_static STATIC ${SOURCE_FILES})
set_target_properties(orione PROPERTIES OUTPUT_NAME orion CLEAN_DIRECTORY_OUTPUT 1)
set_target_properties(orione_static PROPERTIES OUTPUT_NAME orion CLEAN_DIRECTORY_OUTPUT 1)


target_link_libraries (orione thrift thriftnb event snappy re2 tcmalloc pthread protobuf gflags glog boost_system boost_filesystem)
target_link_libraries (orione_static ${STATIC_DEPENDENCIES})
#target_link_libraries (orione ${CMAKE_SOURCE_DIR}/lib//static/thrift.a ${CMAKE_SOURCE_DIR}/lib//static/thrift.a ./static_lib/event.a ./static_lib/snappy.a re2 tcmalloc pthread protobuf gflags glog boost_system boost_filesystem)

file(COPY
    ${BASEPATH}/src/Engne/IfceOrn_.hpp
   DESTINATION ${BASEPATH}/build/include)

file(RENAME ${BASEPATH}/build/include/IfceOrn_.hpp ${BASEPATH}/build/include/orion.h)

install(TARGETS orion RUNTIME DESTINATION .)
install(TARGETS orione DESTINATION lib)
install(TARGETS orione_static DESTINATION lib)
