project (ui)
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

set(CMAKE_MFC_FLAG 2) 

link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
INCLUDE_DIRECTORIES(. .. ../../ ${dirlist} ../../third_party/boost/libs/numeric/conversion/include ../../third_party/boringssl/include)

FILE (GLOB header_files *.h)
FILE (GLOB source_files *.c *.cc *.cpp)

FILE(GLOB RC_FILES *.rc)


add_executable (ui ${header_files} ${source_files} ${RC_FILES})
TARGET_LINK_LIBRARIES (ui  ws2_32 wbemuuid    Wldap32 boost base ssl crypto mvvm logic)
set_target_properties(ui PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")