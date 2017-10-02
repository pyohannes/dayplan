cmake_minimum_required(VERSION 3.5)


if (NOT WIN32)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall -pedantic -Wextra")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unused-parameter")
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

set (TESTLIST test/task_title
              test/task_desc
              test/task_begin
              test/task_end
              test/task_ref
              test/ref_id
              test/ref_title
              test/ref_desc
              test/tasklist_new
              test/tasklist_push
              test/tasklist_remove
              test/tasklist_filter_period_1
              test/tasklist_filter_today_1
              test/tasklist_filter_today_2
              test/tasklist_filter_task_ref_1
              test/tasklist_filter_ref_done_1
              test/parse_empty
              test/parse_task_single_1
              test/parse_task_single_2
              test/parse_task_single_paddings
              test/parse_error_indent_1
              test/parse_error_indent_2
              test/parse_task_list_1
              test/parse_task_list_2
              test/parse_task_list_end_1
              test/parse_task_list_end_2
              test/time_fmt_durance
              test/parse_ref_list_1
              test/acc_durance_1
              test/group_by_day_1
              test/group_by_title_1
)

set (TEST_SOURCE_LIST)
foreach (testname ${TESTLIST})
    set (TEST_SOURCE_LIST ${TEST_SOURCE_LIST} "${testname}.c")
endforeach ()

create_test_sourcelist (SOURCES_TEST main.c 
    ${TEST_SOURCE_LIST}
)
add_executable (dayplan_test main.c test/dpl_test.c ${TEST_SOURCE_LIST})
target_link_libraries (dayplan_test dayplan_static)

foreach (testname ${TESTLIST})
    get_filename_component (tname ${testname} NAME_WE)
    add_test (${tname} dayplan_test "${testname}")
endforeach ()
