project (logic)
ADD_DEFINITIONS(-DBOOST_ALL_NO_LIB)
add_definitions(-DUNICODE -D_UNICODE)
ADD_DEFINITIONS(-DBOOST_THREAD_BUILD_LIB)

FILE(GLOB libs_dir ../../third_party/boost/libs/*)
SET(dirlist "")
foreach (child_dir ${libs_dir})
    if(IS_DIRECTORY ${child_dir})
        list(APPEND dirlist ${child_dir}/include)
        list(APPEND dirlist " ")
    endif()
endforeach()


link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
INCLUDE_DIRECTORIES(. .. ../../ ${dirlist} ../../third_party/boost/libs/numeric/conversion/include ../../third_party/boringssl/include)

FILE (GLOB header_files *.h)
FILE (GLOB source_files *.c *.cc *.cpp)


add_library (logic ${header_files} ${source_files})