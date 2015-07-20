cmake_minimum_required(VERSION 2.8.4)
project(orion)
enable_testing()
add_subdirectory(Trash/Tests)

STRING(TOLOWER "${PLAYER}" PLAYER_LOWER)

if(PLAYER_LOWER STREQUAL "peppemas")
    include_directories(. Hlprs Thrft Engne Extrn Proto Extrn/md5 Ordbc)
    include_directories(~/Documents/ORION/gtest-1.7.0/include)
    include_directories(/usr/local/Cellar/thrift/0.9.2/include)
    include_directories(/usr/local/Cellar/thrift/0.9.2/include/thrift)
    include_directories(/usr/local/Cellar/libevent/2.0.22/include)
    include_directories(~/Documents/ORION/liblfds/include)
    include_directories(/usr/local/include)
    link_directories( ~/Documents/ORION/gtest-1.7.0/lib/ )
else()
  message("NOT PEPPEMAS")
  message(${PLAYER})
  message(${PLAYER_LOWER})
endif()

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
			"${CMAKE_SOURCE_DIR}/*.cpp"
                        )

message( STATUS "QUI QUI QUI -------------------------------------------------" ${CMAKE_SOURCE_DIR} )
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
add_definitions( -DiCdisableprintlog )
ADD_LIBRARY(orion_embeddable STATIC
       ${SOURCE_FILES}
)
TARGET_LINK_LIBRARIES(orion_embeddable thrift thriftnb event snappy re2 tcmalloc pthread protobuf gflags glog gtest boost_system boost_filesystem)
SET(LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/debug)

add_executable(orion ${SOURCE_FILES})
target_link_libraries (orion thrift thriftnb event snappy re2 tcmalloc pthread protobuf gflags glog gtest boost_system boost_filesystem)

install(TARGETS orion RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/dist)
