from dpltest import assert_dpl_cmd_error, assert_dpl_cmd_ok


def test_task_id_conflict():
    assert_dpl_cmd_error(["tasks", "--strict"],
"""2017-09-09
    #1  Task 1
2017-09-10
    #1  Task 2
    08:00
""")


def test_task_id_invalid_1():
    assert_dpl_cmd_error(["tasks"],
"""2017-09-09
    #0  Task 1
    08:00
""")


def test_task_id_invalid_2():
    assert_dpl_cmd_error(["tasks"],
"""2017-09-09
    #a  Task 1
    08:00
""")


def test_task_id():
    assert_dpl_cmd_ok(["tasks", "--oneline"],
"""2017-09-09
    #1  Task 1
    08:00
""",
"""(open) #1 Task 1
""")


def test_done_oldertask():
    assert_dpl_cmd_ok(["tasks", "--oneline", "--date-from", "2017-09-10",
        "--date-to", "2017-09-11"],
"""2017-09-09
    #1  Task 1
    08:00

2017-09-10
    08:00  Task 1
      Fixes #1.
    09:00
""",
"""(done) #1 Task 1
""")
