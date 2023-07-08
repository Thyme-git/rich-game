import os
import shutil

def rename_and_copy_files(src_dir, dest_dir, file_type):
    for root, dirs, files in os.walk(src_dir):
        for file in files:
            old_file_path = os.path.join(root, file)
            new_file_name = file.replace('.txt', '').replace('.in', '')
            test_set_folder = os.path.basename(root)
            
            new_folder_path = os.path.join(dest_dir, f"{test_set_folder}_{new_file_name}")
            os.makedirs(new_folder_path, exist_ok=True)
            
            new_file_path = os.path.join(new_folder_path, file_type)
            
            # if file exists
            if os.path.exists(new_file_path):
                print(f"Duplicate file found: {new_file_path}. Deleting...")
                os.remove(new_file_path)
            shutil.copy(old_file_path, new_file_path)

def main():
    input_src_dir = 'test_folder/input'
    output_src_dir = 'test_folder/output'
    
    dest_dir = 'test'
    

    rename_and_copy_files(input_src_dir, dest_dir, 'input')
    

    rename_and_copy_files(output_src_dir, dest_dir, 'output')

if __name__ == "__main__":
    main()
