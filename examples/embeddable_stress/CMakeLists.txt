cmake_minimum_required(VERSION 2.8.4)
project(orion_embeddable_stress)
enable_testing()

message( STATUS "Orion Embeddable: " ${PROJECT_SRC_DIR} )


include_directories(${CMAKE_SOURCE_DIR}/src/Hlprs)
include_directories(${CMAKE_SOURCE_DIR}/src/Thrft)
include_directories(${CMAKE_SOURCE_DIR}/src/Thrft)
include_directories(${CMAKE_SOURCE_DIR}/src/Engne)
include_directories(${CMAKE_SOURCE_DIR}/src/Extrn)
include_directories(${CMAKE_SOURCE_DIR}/src/Proto)
include_directories(${CMAKE_SOURCE_DIR}/src/Extrn/md5)
include_directories(${CMAKE_SOURCE_DIR}/src/Ordbc)



#include_directories(${CMAKE_SOURCE_DIR}/Hlprs)
#include_directories(${CMAKE_SOURCE_DIR}/Thrft)
#include_directories(${CMAKE_SOURCE_DIR}/Engne)
#include_directories(${CMAKE_SOURCE_DIR}/Extrn)
#include_directories(${CMAKE_SOURCE_DIR}/Proto)
#include_directories(${CMAKE_SOURCE_DIR}/Extrn/md5)
#include_directories(${CMAKE_SOURCE_DIR}/Ordbc)
include_directories(/opt/gtest/include)
include_directories(/usr/local/Cellar/thrift/0.9.2/include)
include_directories(/usr/local/Cellar/thrift/0.9.2/include/thrift)
include_directories(/usr/local/Cellar/libevent/2.0.22/include)
include_directories(/opt/liblfds/include)
include_directories(/usr/local/include)
link_directories( /opt/gtest/lib/ )


add_definitions(-DsCpipename='"ORIONPIPE_"' -DAPPLEOS  -DiCfastpool -DHAVE_NETINET_IN_H -DHAVE_INTTYPES_H -DiCtcmalloc -DiCoo2sortedindex -DVERBOSE=1)

set(CMAKE_CXX_COMPILER /usr/bin/g++)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -g -w -fmessage-length=0 -std=c++0x -static-libstdc++ -stdlib=libc++")
set(CMAKE_INSTALL_PREFIX dist)
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/debug)

set_target_properties( ${targetname} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/Debug )

FILE(GLOB SOURCE_FILES  "${CMAKE_SOURCE_DIR}/src/Engne/*.cpp"
                        "${CMAKE_SOURCE_DIR}/src/Hlprs/*.cpp"
			"${CMAKE_SOURCE_DIR}/src/Thrft/*.cpp"
			"${CMAKE_SOURCE_DIR}/src/Extrn/md5/*.cpp"
			"${CMAKE_SOURCE_DIR}/src/Extrn/*.cpp"
			"${CMAKE_SOURCE_DIR}/src/Proto/*.cc"
			#"${CMAKE_SOURCE_DIR}/src/Unitt/*.cpp"
			"${CMAKE_SOURCE_DIR}/examples/embeddable_stress/*.cpp")

message( STATUS ${CMAKE_SOURCE_DIR} )
message( STATUS ${SOURCE_FILES} )

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

add_executable(orion_embeddable_stress ${SOURCE_FILES})
target_link_libraries (orion_embeddable_stress  thrift thriftnb event snappy re2 tcmalloc pthread protobuf gflags glog gtest boost_system boost_filesystem)

install(TARGETS orion_embeddable_stress  RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/dist)