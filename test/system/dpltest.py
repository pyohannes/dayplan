import os
import shutil
import subprocess
import tempfile


def dpl_run(cmd, content, valgrind=False):
    d = tempfile.mkdtemp()
    dplfile = os.path.join(d, 'input.dat')
    with open(dplfile, 'w') as f:
        f.write(content)
    try:
        cmd = [ 'dayplan', '-f', dplfile ] + cmd
        if valgrind:
            cmd.insert(0, 'valgrind')
        p = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        out, err = p.communicate()
        out, err = out.decode('utf-8'), err.decode('utf-8')
        if valgrind:
            assert 'All heap blocks were freed -- no leaks are possible' in err
        return p.returncode, out, err
    finally:
        shutil.rmtree(d)


def assert_dpl_cmd_error(cmd, content):
    ret, out, err = dpl_run(cmd, content)
    print(ret, out, err)
    assert ret != 0


def assert_dpl_cmd_ok(cmd, content, expected):
    ret, out, err = dpl_run(cmd, content, valgrind=True)
    print(ret, "\n", out, "\n", err, "\n", expected)
    assert out.strip() == expected.strip()
    assert ret == 0
