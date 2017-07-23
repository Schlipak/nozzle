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

def serialize(result):
    template = "{\"backend\":\"math\",\"version\":\"1.0.0\",\"results\":[]}"
    if result == None:
        return template
    template = "{\"backend\":\"math\",\"version\":\"1.0.0\",\"results\":[{\"name\":\"= %g\",\"icon\":\"accessories-calculator\"}]}"
    return (template % result)

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
            print(serialize(result))
            sys.stdout.flush()
        except:
            print(serialize(None))
            sys.stdout.flush()

if __name__ == "__main__": main()
