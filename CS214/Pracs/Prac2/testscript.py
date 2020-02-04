

#!/usr/bin/python
import subprocess, threading, glob, codecs, re, os
from sys import argv, exit

mode_dirs = ['pmx/', 'mpsx/', 'prefilt/'] # TODO Add other modes for phase 2

def atoi(text):
    return int(text) if text.isdigit() else text

def natural_keys(text):
    return [atoi(c) for c in re.split('(\d+)', text)]

class Command:
    def __init__(self, cmd, cwd='./'):
        self.cmd = cmd
        self.cwd = cwd
        self.process = None

    def run(self, timeout=5):
        def target():
            self.process = subprocess.Popen(self.cmd, stdout=subprocess.PIPE,
                    stderr=subprocess.PIPE, cwd=self.cwd, shell=True)
            self.out, self.err = self.process.communicate()

        thread = threading.Thread(target=target)
        thread.start()

        thread.join(timeout)
        if thread.is_alive():
            self.process.terminate()
            thread.join()

        return self.process.returncode, self.out.decode('utf-8'), \
                self.err.decode('utf-8')

def parse_tests(testdir='tests/'):
    tests = []
    outs = []
    for filename in glob.glob(testdir + '*.in'):
        tests.append(filename)

    tests.sort(key=natural_keys)
    for filename in tests:
        outname = filename.replace('.in', '.out')
        with open(outname, 'r') as f:
            outs.append(f.read())
    return tests, outs

def run_tests(bin_path, tests, outs, char_threshold=200):
    print('#'*20, '#'*20)
    correct_count = 0
    total_count = len(tests)
    for test, expected_out in zip(tests, outs):
        cmd = Command("java -cp '{}' PracTest2 < {}".format(
            bin_path, test), './')
        sig, out, err = cmd.run()

        if sig != 0:
            print('ERR', test)
            print('>>>>>>>> ERR >>>>>>>>')
            print(err)
            continue

        if out == expected_out:
            correct_count += 1
            print('✔', test)
        else:
            print('X', test)
            if len(out) < char_threshold and len(expected_out) < char_threshold:
                print('>>>>>>>> OUT >>>>>>>>')
                print(out)
                print('>>>>>>>> EXPECTED OUT >>>>>>>>')
                print(expected_out)
                print('>>>>>>>> ERR >>>>>>>>')
                print(err)
    print('#'*20, '#'*20)

    print('\n{} / {} CORRECT'.format(correct_count, total_count))

def compile_code(src_path, bin_path):
    cmd = Command('mkdir -p {}'.format(bin_path), './')
    cmd.run()
    cmd = Command('rm -f *.class', bin_path)
    cmd.run()

    cmd = Command('javac PracTest2.java', src_path)
    sig, out, err = cmd.run()

    if sig != 0:
        print('COMPILATION ERROR')
        print('>>>>>>>> OUT >>>>>>>>')
        print(out)
        print('>>>>>>>> ERR >>>>>>>>')
        print(err)
        print('COMPILATION ERROR')
        return False

    cmd = Command('mv {}/*.class {}'.format(src_path, bin_path), './')
    cmd.run()

    return True

if __name__ == '__main__':
    if len(argv) == 1:
        src_path = './'
        bin_path = 'bin/'
    elif len(argv) == 3:
        src_path = os.path.normpath(argv[1]) + '/'
        bin_path = os.path.normpath(argv[2]) + '/'
    else:
        print('Usage: python testscript.py <mode> [<src_dir> <bin_dir>]')
        exit()

    if compile_code(src_path, bin_path):
        tests, outs = parse_tests()
        run_tests(bin_path, tests, outs) 

