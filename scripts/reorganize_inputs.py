import os
import hashlib
import shutil

INPUT_PATH = '../input'


def sha512sum(filename):
    hash_object = hashlib.sha512()
    buffer = bytearray(128 * 1024)
    memory_view = memoryview(buffer)
    with open(filename, 'rb', buffering=0) as f:
        for n in iter(lambda: f.readinto(memory_view), 0):
            hash_object.update(memory_view[:n])
    return hash_object.hexdigest()


if __name__ == '__main__':
    mapping = {}
    to_be_removed = []
    for filename in os.listdir(INPUT_PATH):
        full_path = os.path.join(INPUT_PATH, filename)
        digest = sha512sum(full_path)
        if digest in mapping:
            print('Found redundant input: {} collides with {}'.format(filename, mapping[digest]))
            to_be_removed.append(filename)
        else:
            mapping.update({digest: filename})
    for removal in to_be_removed:
        target = os.path.join(INPUT_PATH, removal)
        os.remove(target)
        print('Removed {}.'.format(target))
    for entry in mapping:
        if mapping[entry] != entry + '.txt':
            source = os.path.join(INPUT_PATH, mapping[entry])
            destination = os.path.join(INPUT_PATH, entry + '.txt')
            shutil.move(source, destination)
            print('Moved {} to {}.'.format(source, destination))
