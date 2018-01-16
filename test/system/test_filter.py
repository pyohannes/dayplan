import time

from dpltest import assert_dpl_cmd_ok

TODAY = time.strftime('%Y-%m-%d')
YESTERDAY = time.strftime('%Y-%m-%d', time.localtime(time.time() - 3600 * 24))
TOMORROW = time.strftime('%Y-%m-%d', time.localtime(time.time() + 3600 * 24))

def test_date_to_explicit():
    assert_dpl_cmd_ok(["work", "--oneline", "--from", "2017-09-09"],
"""2017-09-09
    08:00 Work item
    09:00
""",
"""Sat Sep  9 08:00:00 2017  1h  0m  Work item
""")


def test_date_range_by_word_1():
    assert_dpl_cmd_ok(["tasks", "--oneline", "--to", "yesterday"],
"""%s
    #1    yesterday
    09:00
%s
    #2    today
    09:00
%s
    #3    tomorrow
    09:00
""" % (YESTERDAY, TODAY, TOMORROW),
"""[open] #1 yesterday
""")


def test_date_range_by_word_2():
    assert_dpl_cmd_ok(["tasks", "--oneline", "--to", "today"],
"""%s
    #1    yesterday
    09:00
%s
    #2    today
    09:00
%s
    #3    tomorrow
    09:00
""" % (YESTERDAY, TODAY, TOMORROW),
"""[open] #1 yesterday
[open] #2 today
""")


def test_date_range_by_word_3():
    assert_dpl_cmd_ok(["tasks", "--oneline", "--to", "tomorrow"],
"""%s
    #1    yesterday
    09:00
%s
    #2    today
    09:00
%s
    #3    tomorrow
    09:00
""" % (YESTERDAY, TODAY, TOMORROW),
"""[open] #1 yesterday
[open] #2 today
[open] #3 tomorrow
""")
