cmake_minimum_required(VERSION 3.0.2)
project(orione)
enable_testing()

message( STATUS "Orion Embeddable: " ${PROJECT_SRC_DIR} )


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/lib)
set(CMAKE_FILES_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/include)

include_directories(${CMAKE_SOURCE_DIR}/src/Hlprs)
include_directories(${CMAKE_SOURCE_DIR}/src/Thrft)
include_directories(${CMAKE_SOURCE_DIR}/src/Engne)
include_directories(${CMAKE_SOURCE_DIR}/src/Extrn)
include_directories(${CMAKE_SOURCE_DIR}/src/Proto)
include_directories(${CMAKE_SOURCE_DIR}/src/Extrn/md5)
include_directories(${CMAKE_SOURCE_DIR}/src/Ordbc)
include_directories(${CMAKE_SOURCE_DIR}/src/Mains)
#include_directories(/opt/gtest/include)
include_directories(/usr/local/Cellar/thrift/0.9.2/include)
include_directories(/usr/local/Cellar/thrift/0.9.2/include/thrift)
#include_directories(/usr/local/Cellar/libevent/2.0.22/include)
#include_directories(/opt/liblfds/include)
#include_directories(/usr/local/include)
#link_directories( /opt/gtest/lib/ )

add_definitions(-DsCpipename='"ORIONPIPE_"' -DAPPLEOS  -DiCfastpool -DHAVE_NETINET_IN_H -DHAVE_INTTYPES_H -DiCtcmalloc -DiCoo2sortedindex -DVERBOSE=1)

set(CMAKE_CXX_COMPILER /usr/bin/g++)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -g -w -fmessage-length=0 -std=c++0x -static-libstdc++ -stdlib=libc++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -g -w -fmessage-length=0 -std=c++0x -static-libstdc++ -stdlib=libc++")
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

# Executable
add_executable(orion ${SOURCE_FILES})
target_link_libraries (orion thrift thriftnb event snappy re2 tcmalloc pthread protobuf gflags glog boost_system boost_filesystem)


# Library
add_library(orione SHARED ${SOURCE_FILES})
add_library(orione_static STATIC ${SOURCE_FILES})
set_target_properties(orione PROPERTIES OUTPUT_NAME orion CLEAN_DIRECTORY_OUTPUT 1)
set_target_properties(orione_static PROPERTIES OUTPUT_NAME orion CLEAN_DIRECTORY_OUTPUT 1)


target_link_libraries (orione thrift thriftnb event snappy re2 tcmalloc pthread protobuf gflags glog boost_system boost_filesystem)
target_link_libraries (orione_static thrift thriftnb event snappy re2 tcmalloc pthread protobuf gflags glog boost_system boost_filesystem)
#target_link_libraries (orione ${CMAKE_SOURCE_DIR}/lib//static/thrift.a ${CMAKE_SOURCE_DIR}/lib//static/thrift.a ./static_lib/event.a ./static_lib/snappy.a re2 tcmalloc pthread protobuf gflags glog boost_system boost_filesystem)

install(FILES ${CMAKE_SOURCE_DIR}/Engne/IfceOrn_.hpp DESTINATION build/include RENAME orion.h)

install(TARGETS orion RUNTIME DESTINATION .)
install(TARGETS orione DESTINATION lib)
install(TARGETS orione_static DESTINATION lib)
