from dpltest import assert_dpl_cmd_error, assert_dpl_cmd_ok


def test_date_space():
    assert_dpl_cmd_ok(["tasks", "--oneline"],
"""2017-09-09


    #1  Task 1
    08:00
""",
"""[open] #1 Task 1
""")


def test_desc_indent():
    assert_dpl_cmd_ok(["tasks"],
"""2017-09-09
    #1  Task 1
        Task name
            - 1
            - 2
            - 3
    08:00
""",
"""Id:      #1
Status:  open

    Task 1

    Task name
        - 1
        - 2
        - 3
""")


def test_empty():
    assert_dpl_cmd_ok(["tasks", "--oneline"],
"",
"")


def test_error_deindent_1():
    assert_dpl_cmd_error(["tasks"],
"""2017-09-09
  08:00  Projects/Dayplan
 09:00  Projects/Dayplan
""")


def test_sorted():
    assert_dpl_cmd_ok(["work", "--oneline"],
"""2017-09-09
    08:00 A
    09:00 

    13:00 B
    14:00

    10:00 C
    11:00

2017-09-10
    08:00 D
    09:00

2017-09-01
    08:00 E
    09:00
""",
"""Fri Sep  1 08:00:00 2017  1h  0m  E
Sat Sep  9 08:00:00 2017  1h  0m  A
Sat Sep  9 10:00:00 2017  1h  0m  C
Sat Sep  9 13:00:00 2017  1h  0m  B
Sun Sep 10 08:00:00 2017  1h  0m  D
""")


