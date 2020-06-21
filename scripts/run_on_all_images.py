import os
import subprocess
import shutil

IMAGES_INPUT_PATH = '../images/'
DEBUGGING_PATH = './debugging/'

if __name__ == '__main__':
    shutil.rmtree(DEBUGGING_PATH, ignore_errors=True)
    os.mkdir(DEBUGGING_PATH)
    print('Removed all debugging files.')
    for filename in sorted(os.listdir(IMAGES_INPUT_PATH)):
        print('Working on {}...'.format(filename))
        base_filename, _ = os.path.splitext(os.path.basename(filename))
        current_debugging_path = os.path.join(DEBUGGING_PATH, base_filename)
        os.mkdir(current_debugging_path)
        command = ['systemd-run', '--user', '--scope', '-p', 'MemoryMax=16G']
        command.append('./player')
        command.append('--input')
        command.append(os.path.join(IMAGES_INPUT_PATH, filename))
        command.append('--debugging-path')
        command.append(current_debugging_path)
        subprocess.run(command)
