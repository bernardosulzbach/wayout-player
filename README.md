# Wayout Player

[![Build Status](https://travis-ci.com/bernardosulzbach/wayout-player.svg?token=BH5q1d6v3PWdPAzWnpKC&branch=master)](https://travis-ci.com/bernardosulzbach/wayout-player)

An automated player for the [Wayout](https://store.steampowered.com/app/551110/Wayout/) game written using modern (C++ 20) features.

# Building

A complete script for building and running this project is [`build.sh`](build.sh).

The Travis CI pipeline uses Ubuntu Focal Fossa (20.04), CMake 3.12.4, Clang 7.0.0, GCC 9.3.0, Boost 1.71, OpenSSL 1.1.1f, and OpenCV 4.2.0.
You may need other dependencies as well.

# Running

```bash
# Because this process can use a TON of RAM, limiting it is a good idea.
# If you run with no swap, it is basically a requirement to ensure system stability.
# The following line limits the memory usage to 1 GiB on Linux.
systemd-run --scope -p MemoryMax=1G ./player ../input/$INPUT.txt
```

## Inputs

The boards may be supplied in a textual format as exemplified by the inputs in the repository.
The inputs are organized by the [SHA-512](https://en.wikipedia.org/wiki/SHA-2) of the file contents.
The Python 3 script [`reorganize_inputs.py`](scripts/reorganize_inputs.py) organizes them by this.

# License

The code is licensed under the [BSD 3-Clause "New" or "Revised" License](LICENSE).
