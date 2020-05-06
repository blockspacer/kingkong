project (mvvm)
ADD_DEFINITIONS(-DBOOST_ALL_NO_LIB)


FILE(GLOB libs_dir ../third_party/boost/libs/*)
SET(dirlist "")
foreach (child_dir ${libs_dir})
    if(IS_DIRECTORY ${child_dir})
        list(APPEND dirlist ${child_dir}/include)
        list(APPEND dirlist " ")
    endif()
endforeach()

INCLUDE_DIRECTORIES(. .. ../.. ${dirlist} ../third_party/boost/libs/numeric/conversion/include ../third_party/boringssl/include ../third_party/protobuf-3.11.4/src)

FILE (GLOB header_files *.h)
FILE (GLOB source_files *.c *.cc)


FILE (GLOB jni jni/*.cc jni/*.h)
SOURCE_GROUP("jni" FILES ${jni})

FILE (GLOB pb pb/*.cc pb/*.h)
SOURCE_GROUP("pb" FILES ${pb})

if(ANDROID)
    ADD_LIBRARY (mvvm ${header_files} ${source_files} ${jni} ${pb})
    target_link_libraries(mvvm libprotobuf libprotobuf-lite)
else(ANDROID)
    ADD_LIBRARY (mvvm ${header_files} ${source_files} ${pb})
endif(ANDROID)



