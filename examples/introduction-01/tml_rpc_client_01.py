#!/usr/bin/python
# -*- coding: utf-8 -*-
#-----------------------------------------------------------
# sidex - basic tests
#-----------------------------------------------------------

import tml
import tml.core
import tml.command

import sys

PY3 = sys.version > '3'
PY2 = sys.version < '3'


def progress_handler(sender, cmd, progress):
    print("progress", progress)


def main():
    profile = 'urn:your-domain:com:pyexample01'
    port = '12345'
    host = '127.0.0.1'

    core = tml.core.TMLCore()

    # call command 42
    cmd = tml.command.TMLCmd(42)
    core.call_sync(host, port, profile, cmd, 5000)
    print(cmd.data['Result']['Value'])
    print(cmd.data['Result']['Test'])

    # call command 43
    cmd = tml.command.TMLCmd(43)
    core.call_sync(host, port, profile, cmd, 5000)
    print(cmd.data['Result']['Value'])
    print(cmd.data['Result']['Test'])

    # call command 44
    core.on_progress = progress_handler
    cmd = tml.command.TMLCmd(44)
    core.call_sync(host, port, profile, cmd, 5000)

    core.close()

if __name__ == '__main__':
    main()

