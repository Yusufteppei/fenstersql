#!bin/sh

find . -type f \( -name "*.c" -o -name "*.h" \) ! -name "xxhash.*" -print0 | xargs -0 wc -l
