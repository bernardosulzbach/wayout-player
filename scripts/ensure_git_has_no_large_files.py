import subprocess
import os

MAXIMUM_SIZE = 128 * 1024  # 128 KiB


def main():
    infractions = 0
    for line in subprocess.check_output(['git', 'ls-files'], text=True).split('\n'):
        if not line:
            continue
        size = os.path.getsize(line)
        if size > MAXIMUM_SIZE:
            print(f'{line} ({size} bytes) is over the maximum size of {MAXIMUM_SIZE} bytes.')
            infractions += 1
    if infractions:
        exit(1)
    exit(0)


if __name__ == '__main__':
    main()
