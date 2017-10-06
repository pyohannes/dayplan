cmake_minimum_required(VERSION 3.5)


if (NOT WIN32)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall -pedantic -Wextra")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unused-parameter -Wno-unused-variable")
endif ()

if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions (-DYYDEBUG=1)
endif ()

find_package (BISON 3.0.0 REQUIRED)
BISON_TARGET(dpl_parser src/parser.y "${CMAKE_CURRENT_BINARY_DIR}/parser.c")


set (SOURCES src/task.c
             src/ref.c
             src/tasklist.c
             src/time.c
             src/utils.c
             src/acc.c
             src/group.c
             ${BISON_dpl_parser_OUTPUTS}
)


include_directories (inc)


add_executable (dayplan src/main.c ${SOURCES})
add_library (dayplan_static STATIC ${SOURCES})


enable_testing ()

set (TESTLIST test/unit/task_title
              test/unit/task_desc
              test/unit/task_begin
              test/unit/task_end
              test/unit/task_ref
              test/unit/ref_id
              test/unit/ref_title
              test/unit/ref_desc
              test/unit/tasklist_new
              test/unit/tasklist_push
              test/unit/tasklist_remove
              test/unit/tasklist_filter_period_1
              test/unit/tasklist_filter_today_1
              test/unit/tasklist_filter_today_2
              test/unit/tasklist_filter_task_ref_1
              test/unit/tasklist_filter_task_ref_2
              test/unit/tasklist_filter_ref_done_1
              test/unit/parse_empty
              test/unit/parse_task_single_1
              test/unit/parse_task_single_2
              test/unit/parse_task_single_paddings
              test/unit/parse_error_indent_1
              test/unit/parse_error_indent_2
              test/unit/parse_task_list_1
              test/unit/parse_task_list_2
              test/unit/parse_task_list_3
              test/unit/parse_task_multiline_1
              test/unit/parse_task_list_end_1
              test/unit/parse_task_list_end_2
              test/unit/parse_task_list_ref_conflict_1
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
