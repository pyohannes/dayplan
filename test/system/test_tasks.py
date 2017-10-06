from dpltest import assert_dpl_cmd_error, assert_dpl_cmd_ok


def test_ref_id_conflict():
    assert_dpl_cmd_error(["tasks", "--strict"],
"""2017-09-09
    #1  Task 1
2017-09-10
    #1  Task 2
    08:00
""")


def test_ref_id():
    assert_dpl_cmd_ok(["tasks", "--oneline"],
"""2017-09-09
    #1  Task 1
    08:00
""",
"""(open) #1 Task 1
""")
