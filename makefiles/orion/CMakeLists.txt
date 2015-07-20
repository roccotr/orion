cmake_minimum_required(VERSION 2.8.4)
project(orion)

include_directories(${CMAKE_SOURCE_DIR}/Hlprs)
include_directories(${CMAKE_SOURCE_DIR}/Thrft)
include_directories(${CMAKE_SOURCE_DIR}/Engne)
include_directories(${CMAKE_SOURCE_DIR}/Extrn)
include_directories(${CMAKE_SOURCE_DIR}/Proto)
include_directories(${CMAKE_SOURCE_DIR}/Extrn/md5)
include_directories(${CMAKE_SOURCE_DIR}/Ordbc)
include_directories(${CMAKE_SOURCE_DIR}/Mains)
include_directories(/opt/gtest/include)
include_directories(/usr/local/Cellar/thrift/0.9.2/include)
include_directories(/usr/local/Cellar/thrift/0.9.2/include/thrift)
include_directories(/usr/local/Cellar/libevent/2.0.22/include)
include_directories(/opt/liblfds/include)
include_directories(/usr/local/include)
link_directories( /opt/gtest/lib/ )

add_definitions(-DsCpipename='"ORIONPIPE_"' -DAPPLEOS  -DiCfastpool -DHAVE_NETINET_IN_H -DHAVE_INTTYPES_H -DiCtcmalloc -DiCoo2sortedindex -DVERBOSE=1)

set(CMAKE_CXX_COMPILER /usr/bin/g++)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g -w -fmessage-length=0 -std=c++0x -static-libstdc++ -stdlib=libc++")
set(CMAKE_INSTALL_PREFIX dist)
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/debug)

set_target_properties( ${targetname} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/Debug )

FILE(GLOB SOURCE_FILES  "${CMAKE_SOURCE_DIR}/Engne/*.cpp"
                        "${CMAKE_SOURCE_DIR}/Hlprs/*.cpp"
			"${CMAKE_SOURCE_DIR}/Thrft/*.cpp"
			"${CMAKE_SOURCE_DIR}/Extrn/md5/*.cpp"
			"${CMAKE_SOURCE_DIR}/Extrn/*.cpp"
			"${CMAKE_SOURCE_DIR}/Proto/*.cc"
			"${CMAKE_SOURCE_DIR}/Unitt/*.cpp"
			"*.cpp"
                        )

message( STATUS "Orion:" ${PROJECT_SOURCE_DIR} )
message( STATUS ${SOURCE_FILES} )

list(REMOVE_ITEM SOURCE_FILES ${CMAKE_SOURCE_DIR}/Hlprs/TmplFlqu.cpp)
list(REMOVE_ITEM SOURCE_FILES ${CMAKE_SOURCE_DIR}/Hlprs/TmplClps.cpp)
list(REMOVE_ITEM SOURCE_FILES ${CMAKE_SOURCE_DIR}/Hlprs/TmplFsys.cpp)
list(REMOVE_ITEM SOURCE_FILES ${CMAKE_SOURCE_DIR}/Hlprs/TmplList.cpp)
list(REMOVE_ITEM SOURCE_FILES ${CMAKE_SOURCE_DIR}/Hlprs/TmplSdel.cpp)
list(REMOVE_ITEM SOURCE_FILES ${CMAKE_SOURCE_DIR}/Hlprs/TmplSdmg.cpp)
list(REMOVE_ITEM SOURCE_FILES ${CMAKE_SOURCE_DIR}/Hlprs/TmplSdmq.cpp)
list(REMOVE_ITEM SOURCE_FILES ${CMAKE_SOURCE_DIR}/Hlprs/TmplOo2w.cpp)
list(REMOVE_ITEM SOURCE_FILES ${CMAKE_SOURCE_DIR}/Hlprs/TmplOo2t.cpp)
list(REMOVE_ITEM SOURCE_FILES ${CMAKE_SOURCE_DIR}/Hlprs/TmplClps.cpp)
list(REMOVE_ITEM SOURCE_FILES ${CMAKE_SOURCE_DIR}/Hlprs/TmplShst.cpp)
list(REMOVE_ITEM SOURCE_FILES ${CMAKE_SOURCE_DIR}/Hlprs/CoreTevv.cpp)
list(REMOVE_ITEM SOURCE_FILES ${CMAKE_SOURCE_DIR}/Hlprs/CoreSmph.cpp)
list(REMOVE_ITEM SOURCE_FILES ${CMAKE_SOURCE_DIR}/Hlprs/CoreSdmq.cpp)
list(REMOVE_ITEM SOURCE_FILES ${CMAKE_SOURCE_DIR}/Hlprs/CoreHutv.cpp)
list(REMOVE_ITEM SOURCE_FILES ${CMAKE_SOURCE_DIR}/Hlprs/CoreSckw.cpp)
list(REMOVE_ITEM SOURCE_FILES ${CMAKE_SOURCE_DIR}/Hlprs/CoreScpw.cpp)
#list(REMOVE_ITEM SOURCE_FILES ${CMAKE_SOURCE_DIR}/Hlprs/CoreShme.cpp)
list(REMOVE_ITEM SOURCE_FILES ${CMAKE_SOURCE_DIR}/Mains/CoreOdbb.cpp)
list(REMOVE_ITEM SOURCE_FILES ${CMAKE_SOURCE_DIR}/Hlprs/CoreSmpq.cpp)
list(REMOVE_ITEM SOURCE_FILES ${CMAKE_SOURCE_DIR}/Hlprs/CoreXutv.cpp)
list(REMOVE_ITEM SOURCE_FILES ${CMAKE_SOURCE_DIR}/Hlprs/CoreTthv.cpp)
list(REMOVE_ITEM SOURCE_FILES ${CMAKE_SOURCE_DIR}/Mains/Ordbc.cpp)
list(REMOVE_ITEM SOURCE_FILES ${CMAKE_SOURCE_DIR}/Thrft/ThrfOrn__server.skeleton.cpp)

# Create the Static Library
#add_definitions( -DiCdisableprintlog )

add_executable(orion ${SOURCE_FILES})
target_link_libraries (orion thrift thriftnb event snappy re2 tcmalloc pthread protobuf gflags glog gtest boost_system boost_filesystem)

install(TARGETS orion RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/dist)
