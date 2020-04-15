project (base)
SET (ENABLE_BASE_TEST ON)
ADD_DEFINITIONS(-DBOOST_ALL_NO_LIB)


FILE(GLOB libs_dir ../third_party/boost/libs/*)
SET(dirlist "")
foreach (child_dir ${libs_dir})
    if(IS_DIRECTORY ${child_dir})
        list(APPEND dirlist ${child_dir}/include)
        list(APPEND dirlist " ")
    endif()
endforeach()

message(${dirlist})

INCLUDE_DIRECTORIES(. .. ${dirlist} ../third_party/boost/libs/numeric/conversion/include ../third_party/boringssl/include)

FILE (GLOB header_files *.h)
FILE (GLOB source_files *.c *.cc)

FILE (GLOB message_loop message_loop/*.cc message_loop/*.h)
SOURCE_GROUP("message_loop" FILES ${message_loop})

FILE (GLOB net net/*.cc net/*.h)
SOURCE_GROUP("net" FILES ${net})

FILE (GLOB net_websocket net/websocket/*.cc net/websocket/*.h)
SOURCE_GROUP("net\\websocket" FILES ${net_websocket})

FILE (GLOB net_tcp net/tcp/*.cc net/tcp/*.h)
SOURCE_GROUP("net\\tcp" FILES ${net_tcp})

ADD_LIBRARY (base ${header_files} ${source_files} ${message_loop} ${net} ${net_websocket} ${net_tcp})
if (ENABLE_BASE_TEST)
    add_subdirectory(test)
endif()
