import os
import shutil

input_dir = "test1/input"
output_dir = "test1/output"
new_dir = "group1_test"

os.makedirs(new_dir, exist_ok=True)

for input_file in os.listdir(input_dir):
    base_name, ext = os.path.splitext(input_file)
    # 找到编号相同的输入输出对
    output_file = base_name.replace("input", "output") + ext#任意扩展名
    output_file_path = os.path.join(output_dir, output_file)

    if os.path.exists(output_file_path):
        new_case_dir = os.path.join(new_dir, base_name)
        os.makedirs(new_case_dir, exist_ok=True)
        shutil.copy(os.path.join(input_dir, input_file), os.path.join(new_case_dir, "input"))
        shutil.copy(output_file_path, os.path.join(new_case_dir, "output"))
