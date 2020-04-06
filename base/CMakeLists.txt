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

INCLUDE_DIRECTORIES(. ${dirlist} ../third_party/boost/libs/numeric/conversion/include)

FILE (GLOB header_files *.h)
FILE (GLOB source_files *.c *.cc)

ADD_LIBRARY (base ${header_files} ${source_files})
if (ENABLE_BASE_TEST)
    add_subdirectory(test)
endif()
