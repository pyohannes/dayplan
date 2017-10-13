from dpltest import assert_dpl_cmd_ok


def test_date_to_explicit():
    assert_dpl_cmd_ok(["work", "--oneline", "--date-from", "2017-09-09"],
"""2017-09-09
    08:00 Work item
    09:00
""",
"""Sat Sep  9 08:00:00 2017  1h  0m  Work item
""")
