project (base)
ADD_DEFINITIONS(-DBOOST_ALL_NO_LIB)


FILE(GLOB libs_dir ../third_party/boost/libs/*)
SET(dirlist "")
foreach (child_dir ${libs_dir})
    if(IS_DIRECTORY ${child_dir})
        list(APPEND dirlist ${child_dir}/include)
        list(APPEND dirlist " ")
    endif()
endforeach()

#message(${dirlist})

INCLUDE_DIRECTORIES(. .. ${dirlist} ../third_party/boost/libs/numeric/conversion/include ../third_party/boringssl/include)

FILE (GLOB header_files *.h)
FILE (GLOB source_files *.c *.cc)

set (message_loop 
    message_loop/message_loop.h
    message_loop/message_loop_impl.cc
    message_loop/message_loop_impl.h
    message_loop/message_pump.h
    message_loop/message_pump_default.cc
    message_loop/message_pump_default.h
    message_loop/message_pump_impl.cc
    message_loop/message_pump_impl.h)
IF(WIN32)
list (APPEND message_loop
    message_loop/message_pump_win.h
    message_loop/message_pump_win.cc)
ELSEIF (ANDROID)
list (APPEND message_loop
    message_loop/message_pump_android.cc
    message_loop/message_pump_android.h)
ENDIF()

SOURCE_GROUP("message_loop" FILES ${message_loop})



FILE (GLOB net net/*.cc net/*.h)
SOURCE_GROUP("net" FILES ${net})

FILE (GLOB net_websocket net/websocket/*.cc net/websocket/*.h)
SOURCE_GROUP("net\\websocket" FILES ${net_websocket})

FILE (GLOB net_tcp net/tcp/*.cc net/tcp/*.h)
SOURCE_GROUP("net\\tcp" FILES ${net_tcp})

ADD_LIBRARY (base ${header_files} ${source_files} ${message_loop} ${message_loop_platform} ${net} ${net_websocket} ${net_tcp})
if (ENABLE_BASE_TEST)
    add_subdirectory(test)
endif()


if(ANDROID)
    target_link_libraries(base boost ssl crypto)
endif(ANDROID)