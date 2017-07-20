#!/usr/bin/env python
#
# Simple script that performs arbitrary math operations
#
# Needs PyExpressionEval, install with `pip install py_expression_eval`
#

from __future__ import print_function
from py_expression_eval import Parser
import sys
import os

def main():
    parser = Parser()
    while True:
        sys.stderr.write('> ')
        input = sys.stdin.readline()
        if len(input) == 0:
            break;
        input = input.rstrip(os.linesep)
        try:
            result = parser.parse(input).evaluate({})
            print("[{{\\\"name\\\":\\\"= %g\\\",\\\"icon\\\":\\\"accessories-calculator\\\"}}]" % result)
        except:
            print('Not a valid math expression, ignoring', file=sys.stderr)

if __name__ == "__main__": main()
