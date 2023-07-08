import os

def convert_to_lf(root_dir):
    for root, dirs, files in os.walk(root_dir):
        for file in files:
            print(file)
            file_path = os.path.join(root, file)
            with open(file_path, 'r') as f:
                content = f.read()
            content = content.replace('\r\n', '\n').replace('\r', '\n')
            with open(file_path, 'w') as f:
                f.write(content)

def main():
    root_dir = 'group2_test'  # Replace with your directory
    convert_to_lf(root_dir)

if __name__ == "__main__":
    main()
