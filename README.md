# Wayout Player

An automated player for the [Wayout](https://store.steampowered.com/app/551110/Wayout/) game.

```bash
# Because this process can use a TON of RAM, limiting it is a good idea.
# If you run with no swap, it is basically a requirement to ensure system stability.
# The following line limits the memory usage to 1 GiB on Linux.
systemd-run --scope -p MemoryMax=1G ./player ../input/Ж.txt
```

# License

The code is licensed under the [BSD 3-Clause "New" or "Revised" License](LICENSE).
