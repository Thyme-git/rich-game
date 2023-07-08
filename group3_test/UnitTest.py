import subprocess
import concurrent.futures
import random
import os
import sys
import time
import platform
from colorama import Fore, Style
from functools import wraps
from pathlib import Path
from alive_progress import alive_bar


class Test:
    def __init__(self, filePath:str, testName:str = "", testNum:int = 0, hasIllegal:bool = False, ErrorRate:float = 0.05, TestCasePath:str = "", DoneFunc=None) -> None:
        self._tests = []
        self._filePath = filePath
        self._test_gen_path, self.__file_name = os.path.split(self._filePath)
        self._TestOutPath = self._test_gen_path + "/TestCase_Print"
        self._testName = testName
        self._testNum = testNum # test tasks num
        self._hasIllegal = hasIllegal # whether the program generated illegal data
        self._ErrorRate = ErrorRate # if illegal data is generated, the probability of illegal data generation
        self._weights = [1 - ErrorRate, ErrorRate]
        self._results = None # task results
        self._testCasePath = TestCasePath
        self._AllCasePath = []
        self._colors = [Fore.LIGHTGREEN_EX, Fore.RED, Fore.BLUE, Style.RESET_ALL, Fore.MAGENTA, Fore.CYAN] # ANSI color: front_green, front_red, front_blue, front_reset, front_magenta, front_cyan
        self.__default_tests()
        self._successNum = 0 # success test num
        self._system = platform.system()
        self._DoneFunc = DoneFunc

        if not os.path.exists(self._filePath):
            self.__print_error_info(self, "Fail to open file!")
            exit(1)

    def set_tests(self, tests:str) -> None: # customize test
        self._tests = tests
    
    def __read_cases_files(self, _path:str):
        for file in os.listdir(_path):
            file_path = os.path.join(_path, file)
            if os.path.isdir(file_path):
                self.__read_cases_files(file_path)
            else:
                self._AllCasePath.append(file_path)

    def read_test_case(self, TestCasePath:str = "") -> tuple[bool, int]:
        if self._testCasePath == "" and TestCasePath == "":
            self.__print_error_info(self, "Path does not exist!")
            return False, 0
        else:
            if TestCasePath != "":
                self._testCasePath = TestCasePath

            self.__read_cases_files(self._testCasePath)

            self._tests = []
            already_read = []
            for i in self._AllCasePath:
                f_dict = {"path":"", "input":[], "expected_output": ""}
                file_name, extension = os.path.splitext(i)
               
                if extension != '.in' and extension != '.out':
                    continue

                if file_name in already_read:
                    continue

                if '_r' in file_name:
                    continue

                f_dict["path"] = self._TestOutPath + file_name.replace("TestCase", "") + "_r.out"
                # print(f_dict["path"])
                try:
                    with open(file_name + '.in', 'r') as in_file:
                        f_dict["input"] = in_file.readlines()
                    with open(file_name + '.out', 'r') as out_file:
                        f_dict["expected_output"] = ''.join(out_file.readlines()).strip()
                except FileNotFoundError as err:
                    self.__print_warn_info(self, f"{err}")

                self._tests.append(f_dict)
                already_read.append(file_name)

            self._testNum = len(self._tests)
            return True, self._testNum

    def set_test_name(self, testName:str):
        self._testName = testName

    def get_test_name(self) -> str:
        return self._testName

    def __default_tests(self) -> None:
        self._tests = [
            {"path":"", "input":[], "expected_output":""}
        ]

    def __print_test_name(_end:str='') -> None: # print test name
        def _print(print_func):
            @wraps(print_func)
            def warp(self, *args, **kwargs):
                print(f'[{self._colors[2]}{self._testName}{self._colors[3]}]', end=_end)
                print_func(*args, **kwargs)
            return warp
        return _print
    
    @__print_test_name()
    def __print_finish_info(self) -> None:
        print(f'{self._colors[0]}Test finished!{self._colors[3]} Total:{self._testNum}, Success:{self._successNum}, Fail:{self._testNum - self._successNum}{self._colors[3]}')

    @__print_test_name()
    def __print_pass_info(self) -> None:
        print(f'{self._colors[0]}Test passed!{self._colors[3]}')

    @__print_test_name()
    def __print_fail_info(self, _input:list, _actual:str, printPath:bool=True) -> None:
        if printPath:
            print(f'{self._colors[1]}Test failed! test_case_path:{self._testCasePath}, output_path:{_input["path"]}{self._colors[3]}')
        else:
            print(f'{self._colors[1]}Test failed! input:{_input["input"]}, expected_output:{repr(_input["expected_output"])}, actual_output:{repr(_actual)}{self._colors[3]}')

    @__print_test_name()
    def __print_error_info(self, msg:str) -> None:
        print(f"{self._colors[4]}{msg}{self._colors[3]}")

    @__print_test_name()
    def __print_warn_info(self, msg:str) -> None:
        print(f"{self._colors[5]}{msg}{self._colors[3]}")

    def __gen_input(self) -> None: # generate special test input
        pass

    def __gen_expected_output(self) -> None: # generate special test expected output
        pass

    def gen_test(self) -> None: # generate special test
        pass

    def compare_str(self, str1:str, str2:str) -> tuple[bool, str]: # find the difference of str1, str2
        if str1 == str2:
            return True, []
        
        output = ""
        i = 0
        for char1, char2 in zip(str1, str2):
            if char1 == char2:
                output += char1
            else:
                output += f"{self._colors[1]}{char2}{self._colors[3]}"
            i += 1
        
        return False, output

    def __run_test(self, _data:list) -> str: # run single test
        p = subprocess.Popen([self._filePath, _data["path"]], stdin=subprocess.PIPE, stdout=subprocess.PIPE)
        try:
            for i, item in enumerate(_data["input"]):
                p.stdin.write((item + '\n').encode())
            # os.system(f"echo {''.join(_data['input'])} | {self._filePath} {_data['path']} > /dev/null")

            try:
                _stdout, _ = p.communicate(timeout=1)
            except subprocess.TimeoutExpired as err:
                self.__print_warn_info(self, f"Running warn:{err}")
            # p.stdin.flush()
            # p.stdout.flush()
            # p.stdin.close()
            # p.stdout.close()
            # print(_stdout.decode())
            p.terminate()
            p.wait(timeout=10)
            time.sleep(0.1)
            _dump_out = ""
            try:
                with open(_data["path"], 'r') as out_obj:
                    _dump_out = ''.join(out_obj.readlines())
            except FileNotFoundError as err:
                self.__print_warn_info(self, f"{err}")

            if self._DoneFunc:
                # time.sleep(0.1)
                self._DoneFunc()
            
            return _dump_out.strip()
        except BrokenPipeError:
            self.__print_warn_info(self, "Running warn:[BrokenPipeError]. Other tasks not terminated!")
            return ""

    def create_test_tasks(self, parallel:bool = False, concise:bool = False, printPath:bool = True) -> None: # create test tasks, tesk num = self._testNum
        self._successNum = 0
        if parallel:
            self._results = []
            with concurrent.futures.ThreadPoolExecutor() as executor:
                self._results = executor.map(self.__run_test, [test for test in self._tests])
                self._results = list(self._results)
        else:
            self._results = [self.__run_test(_) for _ in self._tests]

        for test, output in zip(self._tests, self._results):
            if output.strip() == test["expected_output"].strip():
                self._successNum += 1

                if concise:
                    continue
                
                self.__print_pass_info(self)
            else:
                self.__print_fail_info(self, test ,output, printPath)
        
        self.__print_finish_info(self)


class NormalTest(Test):
    def __init__(self, filePath:str, testName:str="", testNum:int = 0, hasIllegal:bool = False, ErrorRate:float = 0.05, TestCasePath:str = "", DoneFunc=None) -> None:
        super().__init__(filePath, testName, testNum, hasIllegal, ErrorRate, TestCasePath, DoneFunc)
        self.__default_tests()
        if not hasIllegal:
            self._weights = [1, 0]
            self._ErrorRate = 0

    def __default_tests(self) -> None:
        self._tests = [
            {"path":"", "input":[], "expected_output":""}
        ]


def SingleCommandTest(program_path:str, case_path:str, DoneFunc = None) -> list[int, int]:
    print(f"{Fore.LIGHTCYAN_EX}Running single command test...{Fore.RESET}")
    _path = f"{case_path}/SingleCommand/"
    test_content = {"dump": "TestDump", "init": "TestInit", "set": "TestSet", "step": "TestStep", }
    for key, value in test_content.items():
        test_content[key] = NormalTest(filePath=program_path, testName=value, TestCasePath=_path + key, DoneFunc=DoneFunc)
        test_content[key].read_test_case()

    total, success = 0, 0
    for key, value in test_content.items():
        value.create_test_tasks(parallel=True, concise=False, printPath=True)
        _total, _success = value._testNum, value._successNum
        total += _total
        success += _success

    print(f"{Fore.LIGHTCYAN_EX}Finished.{Fore.RESET}")
    return total, success


def MultiCommandTest(program_path:str, case_path:str, DoneFunc = None) -> list[int, int]:
    print(f"{Fore.LIGHTCYAN_EX}Running multiple command test...{Fore.RESET}")
    _path = f"{case_path}/MultiCommand/"
    test_content = {
        "item": "TestItem", "broke": "TestBroke", "hospital": "TestHospital", "buff": "TestBuff", "gifthouse": "TestGiftHouse",
        "house": "TestHouse", "magicroom": "TestMagicRoom", "mine": "TestMine", "pointstore": "TestPointStore", "prison": "TestPrison",
    }

    for key, value in test_content.items():
        test_content[key] = NormalTest(filePath=program_path, testName=value, TestCasePath=_path + key, DoneFunc=DoneFunc)
        test_content[key].read_test_case()

    total, success = 0, 0
    for key, value in test_content.items():
        value.create_test_tasks(parallel=True, concise=False, printPath=True)
        _total, _success = value._testNum, value._successNum
        total += _total
        success += _success

    print(f"{Fore.LIGHTCYAN_EX}Finished.{Fore.RESET}")
    return total, success

def OtherTest(program_path:str, case_path:str, DoneFunc = None) -> list[int, int]:
    print(f"{Fore.LIGHTCYAN_EX}Running other test...{Fore.RESET}")
    _path = f"{case_path}/Other/"

    other_test = NormalTest(filePath=program_path, testName="TestOther", TestCasePath=_path, DoneFunc=DoneFunc)
    other_test.read_test_case()

    other_test.create_test_tasks(parallel=True, concise=False, printPath=True)
    total, success = other_test._testNum, other_test._successNum

    print(f"{Fore.LIGHTCYAN_EX}Finished.{Fore.RESET}")
    return total, success

def main():
    if platform.system() == "Windows":
        os.system("cls")
    else:
        os.system("clear")

    if len(sys.argv) != 3:
        print('python UnitTest.py main TestCase_dir')
        return
    program_path = sys.argv[1]
    case_path = sys.argv[2][:-1] if sys.argv[2].endswith('/') else sys.argv[2]

    os.system(f"cp -r ./{case_path}/TestCase_Print {program_path.replace('richman', '')}")
    total, success = 0, 0
    with alive_bar(total=81, title="total progress") as bar:
        _total, _success = SingleCommandTest(program_path=program_path, case_path=case_path, DoneFunc=bar)
        total += _total
        success += _success
        _total, _success = MultiCommandTest(program_path=program_path, case_path=case_path, DoneFunc=bar)
        total += _total
        success += _success
        # _total, _success = OtherTest(program_path=program_path, case_path=case_path, DoneFunc=bar)
        # total += _total
        # success += _success
    
    print(f"[{Fore.BLUE}Result{Fore.RESET}]"
          f"{Fore.LIGHTCYAN_EX}Total:{total}{Fore.RESET}, "
          f"{Fore.LIGHTGREEN_EX}Success:{success}{Fore.RESET}, " 
          f"{Fore.LIGHTRED_EX}Fail:{total-success}{Fore.RESET}")
    

if __name__ == '__main__':
    main()