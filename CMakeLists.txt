cmake_minimum_required(VERSION 3.5)


if (NOT WIN32)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall -pedantic -Wextra")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unused-parameter -Wno-unused-variable")
    set (CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3")
endif ()

if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions (-DYYDEBUG=1)
endif ()

add_definitions (-D_DEFAULT_SOURCE -D_XOPEN_SOURCE)

find_package (BISON 3.0.0 REQUIRED)
BISON_TARGET(dpl_parser src/parser.y "${CMAKE_CURRENT_BINARY_DIR}/parser.c")


set (SOURCES src/entry.c
             src/list.c
             src/time.c
             src/utils.c
             src/acc.c
             src/group.c
             src/error.c
             ${BISON_dpl_parser_OUTPUTS}
)


include_directories (inc)


add_executable (dayplan src/main.c ${SOURCES})
add_library (dayplan_static STATIC ${SOURCES})


enable_testing ()

set (TESTLIST test/unit/entry_task_1
              test/unit/entry_work_1
              test/unit/entry_work_2
              test/unit/list_new
              test/unit/list_push
              test/unit/list_remove
              test/unit/list_remove_2
              test/unit/list_sort
              test/unit/list_filter_period_1
              test/unit/list_filter_task_ref_1
              test/unit/list_filter_task_ref_2
              test/unit/list_filter_ref_done_1
              test/unit/filter_task_for_work_1
              test/unit/filter_unique_1
              test/unit/parse_empty
              test/unit/parse_task_single_1
              test/unit/parse_task_single_2
              test/unit/parse_task_single_paddings
              test/unit/parse_task_list_1
              test/unit/parse_task_list_2
              test/unit/parse_task_list_3
              test/unit/parse_task_multiline_1
              test/unit/parse_task_list_end_1
              test/unit/parse_task_list_end_2
              test/unit/parse_task_list_ref_conflict_1
              test/unit/parse_task_list_newlinesep_1
              test/unit/time_fmt_durance
              test/unit/parse_ref_list_1
              test/unit/acc_durance_1
              test/unit/group_by_day_1
              test/unit/group_by_title_1
              test/unit/skip_whitespaces_1
)

set (TEST_SOURCE_LIST)
foreach (testname ${TESTLIST})
    set (TEST_SOURCE_LIST ${TEST_SOURCE_LIST} "${testname}.c")
endforeach ()

create_test_sourcelist (SOURCES_TEST main.c 
    ${TEST_SOURCE_LIST}
)
add_executable (dayplan_test main.c test/unit/dpl_test.c ${TEST_SOURCE_LIST})
target_link_libraries (dayplan_test dayplan_static)

foreach (testname ${TESTLIST})
    get_filename_component (tname ${testname} NAME_WE)
    add_test (${tname} dayplan_test "${testname}")
endforeach ()

add_test (
    NAME test/system 
    COMMAND pytest 
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
set_tests_properties (
    test/system 
    PROPERTIES ENVIRONMENT "PATH=${CMAKE_CURRENT_BINARY_DIR}:$ENV{PATH}")
