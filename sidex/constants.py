#!/usr/bin/env python
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
#-----------------------------------------------------------

import sys

#  @cond NODOC
PY3 = sys.version > '3'
PY2 = sys.version < '3'
#  @endcond

# ------------------ sidex data types--------------
## @defgroup SDXDataTypeConst SIDEX data type codes
#  Constant values to identify sidex variant data types.
#  @ingroup TMLSIDEXConst

## @brief Datatype unknown.
#  @ingroup SDXDataTypeConst
SIDEX_DATA_TYPE_UNKNOWN = 0

## @brief None type.
#  @ingroup SDXDataTypeConst
SIDEX_DATA_TYPE_NONE = 1

## @brief Boolean type.
#  @ingroup SDXDataTypeConst
SIDEX_DATA_TYPE_BOOLEAN = 2

## @brief Integer type.
#  @ingroup SDXDataTypeConst
SIDEX_DATA_TYPE_INTEGER = 3

## @brief Float type.
#  @ingroup SDXDataTypeConst
SIDEX_DATA_TYPE_FLOAT = 4

## @brief String type.
#  @ingroup SDXDataTypeConst
SIDEX_DATA_TYPE_STRING = 5

## @brief List type.
#  @ingroup SDXDataTypeConst
SIDEX_DATA_TYPE_LIST = 6

## @brief Dictionary type.
#  @ingroup SDXDataTypeConst
SIDEX_DATA_TYPE_DICT = 7

## @brief Table type.
#  @ingroup SDXDataTypeConst
SIDEX_DATA_TYPE_TABLE = 8

## @brief DateTime type.
#  @ingroup SDXDataTypeConst
SIDEX_DATA_TYPE_DATETIME = 9

## @brief binary type.
#  @ingroup SDXDataTypeConst
SIDEX_DATA_TYPE_BINARY = 10
