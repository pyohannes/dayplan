from dpltest import assert_dpl_cmd_error, assert_dpl_cmd_ok

import os


def test_parse_only():
    assert_dpl_cmd_ok([],
"""2017-09-09
    08:00 a
    09:00 b
    10:00
""",
"")

def test_version():
    assert_dpl_cmd_ok(['--version'],
"",
"""dayplan version %s
""" % os.environ['DAYPLAN_VERSION'])
