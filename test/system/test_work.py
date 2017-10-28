from dpltest import assert_dpl_cmd_error, assert_dpl_cmd_ok


def test_work_simple():
    assert_dpl_cmd_ok(["work", "--oneline"],
"""2017-09-09
    08:00 Work item
    09:00
""",
"""Sat Sep  9 08:00:00 2017  1h  0m  Work item
""")


def test_work_multiline_comment():
    assert_dpl_cmd_ok(["work"],
"""2017-09-09
    08:00 Work item
        This could be a work item title.

        Then, it git style, several other comment lines can be added. The
        following comment can also contain empty lines, which should be
        preserved.

        In this way paragraphs can be supported.
    09:00
""",
"""Date:    Sat Sep  9 08:00:00 2017
Durance: 1h  0m

    Work item

    This could be a work item title.
    
    Then, it git style, several other comment lines can be added. The
    following comment can also contain empty lines, which should be
    preserved.
    
    In this way paragraphs can be supported.

""")


def test_work_missing_endmarker():
    assert_dpl_cmd_error(["work"],
"""2017-09-09
    08:00 Work item 1
    09:00 Work item 2
""")
