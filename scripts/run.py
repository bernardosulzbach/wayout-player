import os
import subprocess
import shutil

INPUT_PATH = '../input'
OUTPUT_PATH = '../output'

if __name__ == '__main__':
    if not os.geteuid() == 0:
        print('This script may prompt your password several times. Consider running it as root.')
    shutil.rmtree(OUTPUT_PATH, ignore_errors=True)
    os.mkdir(OUTPUT_PATH)
    print('Removed all output files.')
    for filename in os.listdir(INPUT_PATH):
        print('Working on {}...'.format(filename))
        with open(os.path.join(OUTPUT_PATH, filename), 'w') as output_file:
            subprocess.run(['systemd-run', '--scope', '-p', 'MemoryMax=16G', './player', os.path.join(INPUT_PATH, filename)], stdout=output_file)
