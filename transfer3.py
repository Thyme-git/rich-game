import os
import shutil
import sys

class Test:
    def __init__(self, TestCasePath:str = "", DoneFunc=None) -> None:
        self._tests = []
        self._testCasePath = TestCasePath
        self._AllCasePath = []
        self._DoneFunc = DoneFunc

    def __read_cases_files(self, _path:str):
        for file in os.listdir(_path):
            file_path = os.path.join(_path, file)
            if os.path.isdir(file_path):
                self.__read_cases_files(file_path)
            else:
                self._AllCasePath.append(file_path)

    def read_test_case(self, TestCasePath:str = "") -> tuple[bool, int]:
        if self._testCasePath == "" and TestCasePath == "":
            print("Path does not exist!")
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

                f_dict["path"] = file_name.replace("TestCase", "") + "_r.out"
                try:
                    with open(file_name + '.in', 'r') as in_file:
                        f_dict["input"] = in_file.readlines()
                    with open(file_name + '.out', 'r') as out_file:
                        f_dict["expected_output"] = ''.join(out_file.readlines()).strip()
                except FileNotFoundError as err:
                    print(f"{err}")

                self._tests.append(f_dict)
                already_read.append(file_name)

            self._testNum = len(self._tests)
            return True, self._testNum

    def save_test_case(self, output_dir):
        for test_case in self._tests:

            parts = test_case["path"].lstrip('/').split('/')
            file_name, _ = os.path.splitext(parts[-1])
            parts[-1] = file_name
            new_folder_name = '_'.join(parts)
            test_case_dir = os.path.join(output_dir, new_folder_name)
            os.makedirs(test_case_dir, exist_ok=True)
            with open(os.path.join(test_case_dir, 'input'), 'w') as f:
                f.writelines(test_case["input"])
            with open(os.path.join(test_case_dir, 'output'), 'w') as f:
                f.write(test_case["expected_output"])
            
if __name__ == '__main__':
    if len(sys.argv) != 3:
        print('Usage: python test.py <test_folder> <output_folder>')
        sys.exit(1)
    test_folder = sys.argv[1]
    output_folder = sys.argv[2]
    tester = Test(test_folder)
    tester.read_test_case()
    tester.save_test_case(output_folder)
