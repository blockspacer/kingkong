#
project(sqlite3)

FILE(GLOB libs_dir ../boost/libs/*)
SET(dirlist "")
foreach (child_dir ${libs_dir})
    if(IS_DIRECTORY ${child_dir})
        list(APPEND dirlist ${child_dir}/include)
        list(APPEND dirlist " ")
    endif()
endforeach()

INCLUDE_DIRECTORIES (. .. ${dirlist} ../boost/libs/numeric/conversion/include)
ADD_DEFINITIONS(-DSQLITE_ENABLE_RTREE -DSQLITE_ENABLE_COLUMN_METADATA)

FILE (GLOB header_files *.h)
FILE (GLOB source_files *.c *.cpp)
FILE (GLOB def_files *.def)


ADD_LIBRARY (sqlite3 ${header_files} ${source_files} ${def_files})


