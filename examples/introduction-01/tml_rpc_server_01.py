#!/usr/bin/python
# -*- coding: utf-8 -*-
#-----------------------------------------------------------
# TML - simple procedure based RPC server
#-----------------------------------------------------------

import tml
import tml.core
import tml.command
import tml.common

import sys
import time


PY3 = sys.version > '3'
PY2 = sys.version < '3'


def cmd42handler(tmlprofile, cmd):
    cmd.data['Result']['Value'] = 42
    cmd.data['Result']['Test'] = "一切是伟大的"
    print(time.strftime("%H:%M:%S", time.localtime()), 'cmd42handler')


def cmd43handler(tmlprofile, cmd):
    cmd.data['Result']['Value'] = 43
    cmd.data['Result']['Test'] = [1, 2, 3, 'test', 3.14]
    print(time.strftime("%H:%M:%S", time.localtime()), 'cmd43handler')


def cmd44_progress_test(tmlprofile, cmd):
    print(time.strftime("%H:%M:%S", time.localtime()), 'cmd43handler')
    time.sleep(1)
    for p in range(0, 100, 10):
        cmd.data['Result']['Progress'] = p
        time.sleep(0.5)
        cmd.progress = p


def run():
    while True:
        c = tml.common.getch()
        if isinstance(c, str):
            c = c.encode()
        # terminate on CTRL+C
        if c in [b'\x03', b'\x18']:
            break


def main():
    tmlcore = tml.core.TMLCore()
    tmlcore.listener_binding = "127.0.0.1:12345"
    print("listener binding", tmlcore.listener_binding)

    profileid = "urn:your-domain:com:pyexample01"

    # register a command and profile
    tmlcore.profiles.add_profile(profileid)

    tmlcore.profiles[profileid].register_cmd(42, cmd42handler)
    tmlcore.profiles[profileid].register_cmd(43, cmd43handler)
    tmlcore.profiles[profileid].register_cmd(44, cmd44_progress_test)
    tmlcore.listener_enabled = True

    print("listener enabled", tmlcore.listener_enabled)

    run()

    tmlcore.close()

if __name__ == '__main__':
    main()
