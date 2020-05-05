project (cpp_java)
ADD_DEFINITIONS(-DUNICODE -D_UNICODE -D_SCL_SECURE_NO_WARNINGS )
ADD_DEFINITIONS(-DBOOST_THREAD_BUILD_LIB -DBOOST_ALL_NO_LIB)


FILE(GLOB libs_dir ../../third_party/boost/libs/*)
SET(dirlist "")
foreach (child_dir ${libs_dir})
    if(IS_DIRECTORY ${child_dir})
        list(APPEND dirlist ${child_dir}/include)
        list(APPEND dirlist " ")
    endif()
endforeach()

link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
INCLUDE_DIRECTORIES(. .. ../../ ${dirlist} ../../third_party/boost/libs/numeric/conversion/include)

FILE (GLOB header_files *.h)
FILE (GLOB source_files *.c *.cc)

ADD_EXECUTABLE (cpp_java ${header_files} ${source_files})
TARGET_LINK_LIBRARIES (cpp_java  ws2_32 wbemuuid    Wldap32 boost base ssl crypto)
