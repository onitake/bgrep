# bgrep

bgrep - a binary string search tool.

No fancy features like regular expressions or case insensitivity,
just a bland tool that can find a needle (aka string) in a haystack
(aka binary file).

Prints a list of offsets in decimal and hex form from the start of the file.

Built around `mmap()` and `memcmp()` to speed up the search.
Requires long pointers to map large files - or in other words:
It will crash if you try to scan a file > 4GiB on a 32-bit system.

## Compile

```bash
make
```

## Use

```bash
bgrep filename.ext string-to-search
```
