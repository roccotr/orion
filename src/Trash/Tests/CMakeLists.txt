cmake_minimum_required(VERSION 2.8.4)
enable_testing()

include_directories(/usr/local/Cellar/boost/1.58.0/include)
include_directories(. ./osx/sharedmem/)

include_directories(. ../../Hlprs ../../Thrft ../../Engne ../../Extrn ../../Proto ../../Extrn/md5 ../../Ordbc)
include_directories(~/Documents/ORION/gtest-1.7.0/include)
include_directories(/usr/local/Cellar/thrift/0.9.2/include)
include_directories(/usr/local/Cellar/thrift/0.9.2/include/thrift)
include_directories(/usr/local/Cellar/libevent/2.0.22/include)
include_directories(extern_header)
include_directories(/usr/local/include)

set(CMAKE_CXX_COMPILER /usr/bin/g++)
#add_definitions(-DsCpipename='"ORIONPIPE_"' -DAPPLEOS  -DiCfastpool -DHAVE_NETINET_IN_H -DHAVE_INTTYPES_H -DiCtcmalloc -DiCoo2sortedindex -DVERBOSE=1)
add_definitions(-DAPPLEOS)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g -w -fmessage-length=0 -std=c++0x -static-libstdc++ -stdlib=libc++")

link_directories( ~/Documents/ORION/gtest-1.7.0/lib/ )

add_executable(shared_writer ./osx/sharedmem/shared_writer.cpp)
add_executable(shared_reader ./osx/sharedmem/shared_reader.cpp)
add_executable(embeddable ./osx/embeddable/embeddable.cpp)
add_executable(performance ./osx/performance/performance.cpp)
add_executable(put ./osx/putget/put.cpp)
add_executable(get ./osx/putget/get.cpp)

link_directories( ~/Documents/ORION/orion/trunk/Source/Trash/Tests/ )
target_link_libraries(shared_reader ${LIBS} boost_system pthread)
target_link_libraries(shared_writer ${LIBS} boost_system pthread)
target_link_libraries(embeddable ${LIBS} orion_embeddable gtest boost_random)
target_link_libraries(performance ${LIBS} orion_embeddable gtest)
target_link_libraries(put ${LIBS} orion_embeddable gtest)
target_link_libraries(get ${LIBS} orion_embeddable gtest)


#add_test(shared_reader ${CMAKE_CURRENT_BINARY_DIR}/shared_reader)
#add_test(shared_writer ${CMAKE_CURRENT_BINARY_DIR}/shared_writer)
#add_test(embeddable ${CMAKE_CURRENT_BINARY_DIR}/embeddable)
#add_test(performance ${CMAKE_CURRENT_BINARY_DIR}/performance)
