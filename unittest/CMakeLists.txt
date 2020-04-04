project (mytest)
ADD_DEFINITIONS(-DUNICODE -D_UNICODE -D_SCL_SECURE_NO_WARNINGS)
ADD_DEFINITIONS(-DBOOST_THREAD_BUILD_LIB -DBOOST_ALL_NO_LIB)


FILE(GLOB libs_dir ../third_party/boost/libs/*)
SET(dirlist "")
foreach (child_dir ${libs_dir})
    if(IS_DIRECTORY ${child_dir})
        list(APPEND dirlist ${child_dir}/include)
        list(APPEND dirlist " ")
    endif()
endforeach()

message(${dirlist})

INCLUDE_DIRECTORIES(${dirlist} ../third_party/boost/libs/numeric/conversion/include)

FILE (GLOB header_files *.h)
FILE (GLOB source_files *.c *.cpp *.cc)

ADD_EXECUTABLE (mytest ${header_files} ${source_files})
TARGET_LINK_LIBRARIES (mytest  ws2_32 wbemuuid    Wldap32 boost)
