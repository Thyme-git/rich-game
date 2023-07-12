import os
import glob
import filecmp
import subprocess
import itertools
import time
import sys

class TestProgram:
    def __init__(self, program_path, test_folder, dump_path):
        self.program_path = program_path
        self.dump_path = dump_path
        self.test_folder = test_folder
        self.passed_tests = 0
        self.failed_tests = 0

    def get_test_cases(self):
        # 获取所有的测试用例文件夹
        test_cases = []
        for dirpath, dirnames, files in os.walk(self.test_folder):
            if 'input' in files and 'output' in files:
                test_cases.append(dirpath)
        return test_cases

    def run_test(self, test_case_folder):
        
        # 创建一个子进程来运行游戏程序
        with open(f'{test_case_folder}/input', 'r') as input_file:
            process = subprocess.Popen([f"{self.program_path}","dump"], stdin=input_file, stdout=subprocess.DEVNULL, universal_newlines=True)
        time.sleep(0.05)

        #强制结束子进程
        process.terminate()

        #检查进程是否已经结束，如果没有，等待它结束
        if process.poll() is None:
            process.communicate()
        # 删除原来的dum

        if os.path.exists(f'{test_case_folder}/dump'):
            os.remove(f'{test_case_folder}/dump')
        time.sleep(0.05)
        
        # 重命名和移动dump
        os.rename(f'{self.dump_path}', f'{test_case_folder}/dump')
        # 清除文件末尾的空行
        with open(f'{test_case_folder}/dump', 'r') as dump_file:
            lines = dump_file.readlines()
        if lines:
            lines[-1] = lines[-1].rstrip('\n')        
        with open(f'{test_case_folder}/dump', 'w') as dump_file:
            dump_file.writelines(lines)

        #清除output文件末尾的空行
        with open(f'{test_case_folder}/output', 'r') as output_file:
            lines = output_file.readlines()
        if lines:
            lines[-1] = lines[-1].rstrip('\n')
        with open(f'{test_case_folder}/output', 'w') as output_file:
            output_file.writelines(lines)


        # 比较输出文件和预期输出文件
        if filecmp.cmp(f'{test_case_folder}/output', f'{test_case_folder}/dump'):
            print('\033[32m' + f':) Test "{test_case_folder}" passed!' + '\033[0m')
            self.passed_tests += 1
        else:
            strc = ""
            flag = 0
            with open(f'{test_case_folder}/output', 'r', encoding='utf-8') as expected_file, open(f'{test_case_folder}/dump', 'r') as actual_file:
                for i, (expected_line, actual_line) in enumerate(itertools.zip_longest(expected_file, actual_file), start=1):
                    expected_line = expected_line.rstrip() if expected_line is not None else "None"
                    actual_line = actual_line.rstrip() if actual_line is not None else "None"
                    if repr(expected_line) != repr(actual_line):
                        strc+=(f'\tLine {i}: expected "{expected_line}", but got "{actual_line}"\n')
                        flag += 1
            if flag == 0:
                print('\033[32m' + f':) Test "{test_case_folder}" passed!' + '\033[0m')
                self.passed_tests += 1
            else:
                print('\033[31m' + f':( Test "{test_case_folder}" failed!' + '\033[0m')
                print(strc.rstrip())
                self.failed_tests += 1

    def run_all_tests(self):
        # 运行所有测试用例
        for test_case_folder in self.get_test_cases():
            self.run_test(test_case_folder)
            #即时显示
            total_tests = len(self.get_test_cases())
            sys.stdout.write('\033[32m' + f'Passed tests: ({self.passed_tests}/{total_tests})' + '\033[0m' + '\r')
            sys.stdout.flush()

        # Print the final result
        print('\033[33m' + 'RESULT'.center(80, '=') + '\033[0m')
        print('\033[32m' + f'Passed tests: ({self.passed_tests}/{self.failed_tests + self.passed_tests})' + '\033[0m', end=' ')
if __name__ == '__main__':
    if len(sys.argv) != 3:
        print('Usage: python test.py <game> <test_folder>')
        sys.exit(1)
    game = sys.argv[1]
    test_folder = sys.argv[2]

    tester = TestProgram(f'./{game}', test_folder, './dump')
    start = time.perf_counter()

    tester.run_all_tests()

    end = time.perf_counter()
    print(f'Totally takes time: {end - start}s')
