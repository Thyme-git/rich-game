import os
import glob
import filecmp
import subprocess
import itertools
import time

class TestProgram:
    def __init__(self, program_path, test_folder, dump_path):
        self.program_path = program_path
        self.dump_path = dump_path
        self.test_folder = test_folder
        self.passed_tests = 0
        self.failed_tests = 0

    def get_test_cases(self):
        # 获取所有的测试用例文件夹
        return glob.glob(f'{self.test_folder}/*')

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

        # 重命名和移动dump
        os.rename(f'{self.dump_path}', f'{test_case_folder}/dump')
        # 清除文件末尾的空行
        with open(f'{test_case_folder}/dump', 'r') as dump_file:
            lines = dump_file.readlines()
        if lines:
            lines[-1] = lines[-1].rstrip('\n')        
        with open(f'{test_case_folder}/dump', 'w') as dump_file:
            dump_file.writelines(lines)

        # 比较输出文件和预期输出文件
        if filecmp.cmp(f'{test_case_folder}/output', f'{test_case_folder}/dump'):
            print('\033[32m' + f':) Test "{test_case_folder}" passed!' + '\033[0m')
            self.passed_tests += 1
        else:
            print('\033[31m' + f':( Test "{test_case_folder}" failed!' + '\033[0m')
            with open(f'{test_case_folder}/output', 'r') as expected_file, open(f'{test_case_folder}/dump', 'r') as actual_file:

                for i, (expected_line, actual_line) in enumerate(itertools.zip_longest(expected_file, actual_file), start=1):
                    expected_line = expected_line.rstrip() if expected_line is not None else "None"
                    actual_line = actual_line.rstrip() if actual_line is not None else "None"
                    if repr(expected_line) != repr(actual_line):
                        print(f'    Line {i}: expected "{expected_line}", but got "{actual_line}"')
                          
            self.failed_tests += 1

    def run_all_tests(self):
        # 运行所有测试用例
        for test_case_folder in self.get_test_cases():
            self.run_test(test_case_folder)

        print('\033[33m' + 'RESULT'.center(80, '=') + '\033[0m')
        print('\033[32m' + f'Passed tests: ({self.passed_tests}/{self.failed_tests + self.passed_tests})' + '\033[0m')

if __name__ == '__main__':
    tester = TestProgram('./main', 'test', 'dump')#改test为你的文件夹名称，会自动遍历里面的小文件夹
    tester.run_all_tests()