import os
import shutil
import subprocess
import tempfile


def dpl_run(cmd, content, valgrind=False, stdin=False):
    cmd = [ 'dayplan' ] + cmd
    if valgrind:
        cmd.insert(0, 'valgrind')
        cmd.insert(1, '--leak-check=full')
    if stdin:
        p = subprocess.Popen(
                cmd, 
                stdout=subprocess.PIPE, 
                stderr=subprocess.PIPE,
                stdin=subprocess.PIPE)
        p.stdin.write(content.encode('utf-8'))
        out, err = p.communicate()
    else:
        d = tempfile.mkdtemp()
        dplfile = os.path.join(d, 'input.dat')
        try:
            with open(dplfile, 'w') as f:
                f.write(content)
            cmd.extend([ '-f', dplfile ])
            p = subprocess.Popen(
                    cmd, 
                    stdout=subprocess.PIPE, 
                    stderr=subprocess.PIPE)
            out, err = p.communicate()
        finally:
            shutil.rmtree(d)
    out, err = out.decode('utf-8'), err.decode('utf-8')
    if valgrind:
        print(err)
        assert 'All heap blocks were freed -- no leaks are possible' in err
    return p.returncode, out, err


def assert_dpl_cmd_error(cmd, content):
    ret, out, err = dpl_run(cmd, content)
    print(ret, out, err)
    assert ret != 0


def assert_dpl_cmd_ok(cmd, content, expected):
    ret, out, err = dpl_run(cmd, content, valgrind=True, stdin=False)
    print(ret, "\n", out, "\n", err, "\n", expected)
    assert out.strip() == expected.strip()
    assert ret == 0
    # ensure the same result when using stdin for input
    reti, outi, erri = dpl_run(cmd, content, valgrind=True, stdin=True)
    assert reti == ret
    assert outi == out
