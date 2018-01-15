from dpltest import assert_dpl_cmd_ok


def test_group_by_name():
    assert_dpl_cmd_ok(["sum"],
"""2017-09-09
    08:00 A/B/C
    09:00

2017-09-11
    08:00 A/B/D
    18:00

2017-09-13
    08:00 E/F
    10:00 A/X/D
    11:00 A/X/E
    12:00

2017-09-12
    08:00 E/F/G
    11:00
""",
"""   1h  0m A/B/C
  10h  0m A/B/D
   1h  0m A/X/D
   1h  0m A/X/E
   2h  0m E/F
   3h  0m E/F/G
""")


def test_group_by_day():
    assert_dpl_cmd_ok(["sum", "--group-by=day"],
"""2017-09-09
    08:00 Work item
    09:00

2017-09-11
    08:00 Work item
    18:00

2017-09-13
    08:00 Work item
    10:00

2017-09-12
    08:00 Work item
    11:00
""",
"""   1h  0m 2017-09-09
  10h  0m 2017-09-11
   3h  0m 2017-09-12
   2h  0m 2017-09-13
""")


def test_group_by_category():
    assert_dpl_cmd_ok(["sum", "--group-by=category"],
"""2017-09-09
    08:00 (a/b/c) foo
    09:00

2017-09-11
    08:00 (a/b/d) bar
    18:00

2017-09-13
    08:00 (e/f) More Foo
    10:00 (a/x/d) Spam
    11:00 (a/x/e) Eggs
    12:00

2017-09-12
    08:00 (e/f/g) Bar
    11:00
""",
"""  13h  0m a
  11h  0m a/b
   1h  0m a/b/c
  10h  0m a/b/d
   2h  0m a/x
   1h  0m a/x/d
   1h  0m a/x/e
   5h  0m e
   5h  0m e/f
   3h  0m e/f/g
""")


def test_group_by_category_empty():
    assert_dpl_cmd_ok(["sum", "--group-by=category"],
"""2017-09-09
    08:00 foo
    09:00
""",
"""   1h  0m 
""")
