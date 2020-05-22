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

FILE (GLOB net_tcp_client net/tcp_client/*.cc net/tcp_client/*.h)
SOURCE_GROUP("net\\tcp_client" FILES ${net_tcp_client})

FILE (GLOB net_http net/http/*.cc net/http/*.h net/http/*.c)
SOURCE_GROUP("net\\http" FILES ${net_http})

FILE (GLOB jni jni_util/*.cc jni_util/*.h)
SOURCE_GROUP("jni" FILES ${jni})

FILE (GLOB net_tcp_server net/tcp_server/*.cc net/tcp_server/*.h)
SOURCE_GROUP("net\\tcp_server" FILES ${net_tcp_server})

Set (base_common_src ${header_files} 
    ${source_files} 
    ${message_loop}
    ${message_loop_platform}
    ${net} 
    ${net_websocket}
    ${net_tcp_client}
    ${net_http}
    ${net_tcp_server}
	)


if (ANDROID)
    ADD_LIBRARY (base  ${base_common_src} ${jni})
    target_link_libraries(base boost ssl crypto android)
else(ANDROID)
    ADD_LIBRARY (base ${base_common_src})
endif(ANDROID)

if (ENABLE_BASE_TEST)
    add_subdirectory(test)
endif()
