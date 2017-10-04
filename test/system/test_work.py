from dpltest import assert_dpl_cmd_error, assert_dpl_cmd_ok


def test_work_simple():
    assert_dpl_cmd_ok(["work", "--oneline"],
"""2017-09-09
    08:00 Work item
    09:00
""",
"""Sat Sep  9 08:00:00 2017  1h  0m  Work item
""")
