#!/usr/bin/python3
# -*- coding: utf-8 -*-
#-----------------------------------------------------------
#  libTML:  A BEEP based Messaging Suite
#  Copyright (C) 2015 wobe-systems GmbH
#
#  This program is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public License
#  as published by the Free Software Foundation; either version 2.1
#  of the License, or (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public
#  License along with this program; if not, write to the Free
#  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
#  02111-1307 USA
#
#  You may find a copy of the license under this software is released
#  at COPYING file. This is LGPL software: you are welcome to develop
#  proprietary applications using this library without any royalty or
#  fee but returning back any change, improvement or addition in the
#  form of source code, project image, documentation patches, etc.
#
#  Homepage:
#    http://www.libtml.org
#
#  For professional support contact us:
#
#    wobe-systems GmbH
#    support@libtml.org

from distutils.core import setup, Extension
import sys
import os
#-----------------------------------------------------------

if sys.maxsize == 2147483647:
    libsidex = 'sidex11'
    libtmlCore = 'tmlcore11'
else:
    libsidex = 'sidex11_64'
    libtmlCore = 'tmlcore11_64'

if sys.platform.startswith('linux'):
    print("linux detected ...")
    platform_macros = [('LINUX', ''), ('GCC4', '')]
    compiler_options = ['-std=c++0x']
    if os.uname()[4].startswith('arm'):
        print("arm detected ...")
        platform_macros = platform_macros + [('__STDC_FORMAT_MACROS', '')]
    else:
        compiler_options = compiler_options + ['-Wno-attributes',
                                               '-Wno-conversion-null',
                                               '-Wno-pointer-arith',
                                               '-Wno-unused-variable',
                                               '-Wno-unused-but-set-variable']
elif sys.platform.startswith('win32'):
    print("windows detected ...")
    platform_macros = []
    compiler_options = []
elif sys.platform.startswith('darwin'):
    print("OS X detected ...")
    platform_macros = [('LINUX', ''), ('GCC4', '')]
    compiler_options = ['-std=c++11',
                        '-Wno-attributes',
                        '-Wno-conversion-null',
                        '-Wno-null-arithmetic',
                        '-Wno-unused-variable']
elif sys.platform.startswith('freebsd'):
    print("freeBSD detected ...")
    platform_macros = [('LINUX', ''), ('GCC4', '')]
    compiler_options = ['-std=c++0x',
                        '-Wno-attributes',
                        '-Wno-conversion-null',
                        '-Wno-pointer-arith',
                        '-Wno-unused-variable']
else:
    platform_macros = []
    compiler_options = []

_sidex_ext = Extension('sidex._sidex',
                       define_macros=[('SIDEX_UNICODE', '')]+platform_macros,
                       include_dirs=['%s/include/python%s.%s' % (sys.prefix, sys.version_info[0], sys.version_info[1]),
                                     'src/sidex',
                                     'src/common',
                                     '%s/include' % sys.prefix,
                                     '%s/include/tml' % sys.prefix,
                                     '%s/local/include/tml' % sys.prefix,
                                     '/usr/include/tml'],
                       libraries=[libsidex],
                       library_dirs=['%s/lib' % sys.prefix,
                                     '%s/local/lib' % sys.prefix,
                                     '/usr/lib'],
                       sources=['src/sidex/_sidex_mod.cpp',
                                'src/sidex/_sidex_conversion.cpp',
                                'src/common/_py_unicode.cpp'],
                       extra_compile_args=compiler_options)

_tml_ext = Extension('tml._tml',
                     define_macros=[('TML_UNICODE', ''),
                                    ('SIDEX_UNICODE', '')]+platform_macros,
                     include_dirs=['%s/include/python%s.%s' % (sys.prefix, sys.version_info[0], sys.version_info[1]),
                                   'src/tml',
                                   'src/common',
                                   'src/sidex',
                                   '%s/include' % sys.prefix,
                                   '%s/include/tml' % sys.prefix,
                                   '%s/local/include/tml' % sys.prefix,
                                   '/usr/include/tml'],
                     libraries=[libsidex, libtmlCore],
                     library_dirs=['%s/lib' % sys.prefix,
                                   '%s/local/lib' % sys.prefix,
                                     '/usr/lib'],
                     sources=['src/tml/_tml_mod.cpp',
                              'src/common/_py_unicode.cpp'],
                     extra_compile_args=compiler_options)

setup(name='libtml-python',
      version='1.1.5',
      description='TML Messaging Suite for Python',
      author='wobe-systems GmbH',
      author_email='team@libtml.org',
      url='http://www.libtml.org/',
      long_description='''TML Messaging Suite is a network messaging library
       for rapid development of extensible and scalable interfaces.''',
      license='LGPL 2.1 or later',
      platforms=["Windows", "Linux", "OS-X", "freeBSD"],
      packages=['sidex', 'tml'],
      package_data={},
      ext_modules=[_sidex_ext, _tml_ext])
