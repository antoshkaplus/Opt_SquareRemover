#
# test is actually depends on google test
#

# macros
macro(create_source_groups curdir)
    file(GLOB children RELATIVE ${PROJECT_SOURCE_DIR}/${curdir} ${PROJECT_SOURCE_DIR}/${curdir}/*)
    foreach(child ${children})
    if(IS_DIRECTORY ${PROJECT_SOURCE_DIR}/${curdir}/${child})
        create_source_groups(${curdir}/${child})
    else()
        string(REPLACE "/" "\\" groupname ${curdir})
        source_group(${groupname} FILES ${PROJECT_SOURCE_DIR}/${curdir}/${child})
    endif()
    endforeach()
endmacro()

# body
cmake_minimum_required (VERSION 3.2)
project (SquareRemover)

list(APPEND CMAKE_CXX_FLAGS "-std=c++1y -Wall -Werror -Wno-sign-compare -Wno-char-subscripts  -Wno-error=char-subscripts -Wno-macro-redefined -Wno-unused-const-variable")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O3")


set(BinDir ${PROJECT_SOURCE_DIR}/bin)
set(LibDir ${PROJECT_SOURCE_DIR}/lib)

# archive is for static libraries
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BinDir} )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LibDir} )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${LibDir} )
foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
    string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
    set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${BinDir} )
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${LibDir} )
    set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${LibDir} )
endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )


include_directories("/usr/local/include")
include_directories("/usr/include")
include_directories("./include")

find_library(ANT_LIBRARY ant)
if (NOT ANT_LIBRARY)
    message(FATAL_ERROR "ant library not found")
endif()

# names of executables should be caps

file(GLOB SQUARE_REMOVER_FILES "./src/*" "./include/*")
add_library(square_remover ${SQUARE_REMOVER_FILES})
target_link_libraries(square_remover ${ANT_LIBRARY})

add_executable(square_remover_app "./app/app.cpp")
target_link_libraries(square_remover_app square_remover)

add_executable(offline "./app/offline.cpp")
target_link_libraries(offline square_remover)

# submissions
add_executable(v_1 "submit/v_1.cpp" "app/main_template.cpp")
target_link_libraries(v_1 square_remover)

add_executable(v_1_tester "submit/v_1.cpp" "app/tester_template.cpp")
target_link_libraries(v_1_tester square_remover)

# methods
add_executable(naive app/methods.cpp app/main_template.cpp)
target_link_libraries(naive square_remover)
target_compile_definitions(naive PUBLIC -DSQR_REM_NAIVE)

# others
add_executable(psyho_tester others/psyho.cpp app/tester_template.cpp)
target_link_libraries(psyho_tester square_remover)
set_target_properties(psyho_tester PROPERTIES COMPILE_FLAGS "-Wno-unused-variable -Wno-logical-op-parentheses -Wno-return-type")

add_executable(tanzaku_tester others/tanzaku.cpp app/tester_template.cpp)
target_link_libraries(tanzaku_tester square_remover)
set_target_properties(tanzaku_tester PROPERTIES COMPILE_FLAGS "-Wno-logical-op-parentheses -Wno-unused-const-variable")

add_executable(wrong_tester others/wrong.cpp app/tester_template.cpp)
target_link_libraries(wrong_tester square_remover)
set_target_properties(wrong_tester PROPERTIES COMPILE_FLAGS "-Wno-logical-op-parentheses -Wno-unused-const-variable")


find_library(GTEST_LIB gtest)
if (NOT GTEST_LIB)
    message(FATAL_ERROR "gtest library not found")
endif()

add_executable(tester "app/tester_template.cpp")
target_link_libraries(tester square_remover ${GTEST_LIB} )
