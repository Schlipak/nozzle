#!/usr/bin/env python
#
# Simple echo script
# Outputs a JSON object with input as a name
#

from __future__ import print_function
import sys
import os

def main():
    while True:
        sys.stderr.write('> ')
        input = sys.stdin.readline()
        if len(input) == 0:
            break;
        input = input.rstrip(os.linesep)
        print("[{{\\\"name\\\": \\\"{}\\\"}}]".format(input))

if __name__ == "__main__": main()
