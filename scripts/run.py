import os
import subprocess

INPUT_PATH = '../input'
OUTPUT_PATH = '../output'

if __name__ == '__main__':
    for filename in os.listdir(INPUT_PATH):
        print('Working on {}...'.format(filename))
        with open(os.path.join(OUTPUT_PATH, filename), 'w') as output_file:
            subprocess.run(["./player", os.path.join(INPUT_PATH, filename)], stdout=output_file)
