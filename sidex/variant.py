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

import sidex
import time
import datetime
from sidex.constants import *

## @defgroup SDXVariantGroup SIDEX variant type
#  @ingroup highLevelClasses
#  @brief Description of SIDEX variant data type.
#
#  The SIDEX variant data type is the foundation for type safe data exchange between platforms and development
#  environments.


## @brief Convert a python value into a SIDEX variant
#  @ingroup SDXVariantGroup
#
#  @param pvalue  :[\b object] value to convert of type, str, int, float, bool, list, datetime, time.structime,
#                              dict, time tuple
#
#  @returns [\b SDXBase] SIDEX variant object
def py_to_sdx(pvalue):

    if isinstance(pvalue, SDXBase):
        #nothing to do
        return pvalue
    elif PY3 and isinstance(pvalue, str):
        return SDXString(pvalue)
    elif PY2 and isinstance(pvalue, (str, unicode)):
        return SDXString(pvalue)
    elif isinstance(pvalue, float):
        return SDXFloat(pvalue)
    elif isinstance(pvalue, bool):
        return SDXBoolean(pvalue)
    elif PY3 and isinstance(pvalue, int):
        return SDXInteger(pvalue)
    elif PY2 and isinstance(pvalue, (int, long)):
        return SDXInteger(pvalue)
    elif PY3 and isinstance(pvalue, bytes):
        return SDXBinary(pvalue)
    elif pvalue is None:
        return SDXNone()
    elif isinstance(pvalue, list):
        result = SDXList()
        for lvalue in pvalue:
            result.append(lvalue)
        return result
    elif isinstance(pvalue, dict):
        result = SDXDict()
        for lkey in pvalue.keys():
            result[lkey] = pvalue[lkey]
        return result
    elif isinstance(pvalue, (time.struct_time, datetime.datetime, datetime.date, tuple)):
        return SDXDateTime(pvalue)
    else:
        raise TypeError("invalid argument type (%s)" % (type(pvalue)))


## @def Convert SIDEX variant to python object.
#  @ingroup SDXVariantGroup
#
#  Passing a python object will return it unchanged. A SDXBase object
#  will be converted into a python data type.
#
#  @param pvalue : [\b SDXBase, \b object] value to convert
#
#  @return [\b object] python object
def sdx_to_py(pvalue):
    if isinstance(pvalue, SDXBase):
        return sidex.SidexVariantAsPyObject(pvalue.vhandle)
    else:
        return pvalue


## @brief Wrap a SIDEX variant handle into a SDXBase object.
#  @ingroup SDXVariantGroup
#
#  SIDEX variant handles can be management by the corresponding SDXBase object. This function
#  creates the class instance depending on the SIDEX variant type. In case og a borrowed reference,
#  the reference counter has to be increased.
#
#
#  @param phandle  :[\b int] sidex handle
#  @param owner    :[\b bool][\em optional] instance is owner of the sidex handle (default: True)
#  @param borrowed : [\b bool][\em optional] borrowed sidex handle (default: False)
#
#  @returns [\b SDXBase] SIDEX variant type object
def wrap_sdx_handle(phandle, owner=True, borrowed=False):
    ltype = sidex.sidex_Variant_GetType(phandle)
    if ltype == SIDEX_DATA_TYPE_NONE:
        return SDXNone(vhandle=phandle, thisown=owner, borrowed=borrowed)
    elif ltype == SIDEX_DATA_TYPE_BOOLEAN:
        return SDXBoolean(vhandle=phandle, thisown=owner, borrowed=borrowed)
    elif ltype == SIDEX_DATA_TYPE_INTEGER:
        return SDXInteger(vhandle=phandle, thisown=owner, borrowed=borrowed)
    elif ltype == SIDEX_DATA_TYPE_FLOAT:
        return SDXFloat(vhandle=phandle, thisown=owner, borrowed=borrowed)
    elif ltype == SIDEX_DATA_TYPE_STRING:
        return SDXString(vhandle=phandle, thisown=owner, borrowed=borrowed)
    elif ltype == SIDEX_DATA_TYPE_LIST:
        return SDXList(vhandle=phandle, thisown=owner, borrowed=borrowed)
    elif ltype == SIDEX_DATA_TYPE_DICT:
        return SDXDict(vhandle=phandle, thisown=owner, borrowed=borrowed)
    elif ltype == SIDEX_DATA_TYPE_DATETIME:
        return SDXDateTime(vhandle=phandle, thisown=owner, borrowed=borrowed)
    elif ltype == SIDEX_DATA_TYPE_TABLE:
        return SDXTable(vhandle=phandle, thisown=owner)
    elif ltype == SIDEX_DATA_TYPE_BINARY:
        return SDXBinary(vhandle=phandle, thisown=owner)
    else:
        raise TypeError("sidex type not supported (%s)" % ltype)


## @brief Abstract base class for sidex variants.
#  @ingroup SDXVariantGroup
#
#  The base class manages a SIDEX variant handle. Basically it takes care of decrementing its
#  reference counter on destruction. SDXBase is an abstract base class for all SIDEX variant
#  wrapper objects. It should never be used directly.
class SDXBase(object):

    ## @brief Constructor of SDXBase
    #
    #  @param vhandle  :[\b int][\em optional] handle of a sidex variant
    #  @param thisown  :[\b bool][\em optional] instance is owner of the sidex variant handle
    #  @param borrowed : [\b bool][\em optional] borrowed sidex handle (default: False)
    def __init__(self, vhandle=None, thisown=True, borrowed=False):
        if type(self) is SDXBase:
            raise TypeError("abstract base class can't be used directly")

        if vhandle is not None:
            self._thisown = thisown
            self._vhandle = vhandle
            if self._thisown and borrowed:
                sidex.sidex_Variant_IncRef(self._vhandle)
        else:
            self._thisown = False
            self._vhandle = None

        return

        # ------------ properties -----------------
        # the code here is for documentation only
        self.vhandle = None
        # ------------ properties -----------------

    ## @brief SDXBase destructior
    #
    #  On deleting the object the reference counter of the SIDEX variant handle is
    #  decremented if the handle is owned by the class.
    #
    #  @sa sidex_Variant_DecRef()
    def __del__(self):
        if self._thisown and (self._vhandle is not None):
            sidex.sidex_Variant_DecRef(self._vhandle)

    ## @property vhandle
    #  @brief [\b int] SIDEX variant handle
    #
    #  @par Access:
    #    read

    #  @cond NODOC
    @property
    def vhandle(self):
        return self._vhandle
    #  @endcond

    ## @brief Convert the value to a string.
    #
    #  The string conversion is based on the python type that results from a conversion of a SIDEX variant.
    #
    #  @returns [\b str] string representation of a variant type
    #
    #  @sa SidexVariantAsPyObject()
    def __str__(self):
        return str(sidex.SidexVariantAsPyObject(self._vhandle))

    ## @brief Convert the value to a string.
    #
    #  The string conversion is based on the python type that results from a conversion of a SIDEX variant.
    #
    #  @returns [\b str] string representation of a variant type
    #
    #  @sa SidexVariantAsPyObject()
    def __unicode__(self):
        return unicode(sidex.SidexVariantAsPyObject(self._vhandle))

    ## @brief Convert the value to a string representation.
    #
    #  The string conversion is based on the python type that results from a conversion of a SIDEX variant.
    #
    #  @returns [\b str] string representation of a variant type
    #
    #  @sa SidexVariantAsPyObject()
    def __repr__(self):
        return repr(sidex.SidexVariantAsPyObject(self._vhandle))

    ## @brief Check if the variable is 0 for boolean check
    #
    #  @returns [\b bool] True if the value is not zero
    def __nonzero__(self):
        return bool(sidex.SidexVariantAsPyObject(self._vhandle))


## @brief Implementation of a \ref nonetype "None type variant".
#  @ingroup SDXVariantGroup
#
#  When working with databases the None type is very useful to store the \c null value of a record.
class SDXNone(SDXBase):

    ## @brief Constructor of SDXNone
    #
    #  If no handle is passed, a new one will be created and owned by the class instance.
    #
    #  @param vhandle :[\b int][\em optional] handle of a sidex variant
    #  @param thisown :[\b bool][\em optional] instance is owner of the sidex variant handle
    #  @param borrowed : [\b bool][\em optional] borrowed sidex handle (default: False)
    #
    #  @sa sidex_Variant_New_None()
    def __init__(self, vhandle=None, thisown=True, borrowed=False):
        SDXBase.__init__(self, vhandle, thisown, borrowed)
        if self._vhandle is None:
            self._thisown = True
            self._vhandle = sidex.sidex_Variant_New_None()

        return

        # ------------ properties -----------------
        # the code here is for documentation only
        self.value = None
        # ------------ properties -----------------

    ## @property value
    #  @brief [\b obect] The value of the variant.
    #
    #  The value property of the None type is read only because it does not make sense to assign something.
    #  The return value is always None.
    #
    #  @par Access:
    #    read

    #  @cond NODOC
    @property
    def value(self):
        return None
    #  @endcond

    ## @brief Check if the variable is 0 for boolean check
    #
    #  @returns [\b bool] True if the value is not zero
    def __nonzero__(self):
        return False



## @brief Implementation of a \ref booltype "Boolean type variant".
#  @ingroup SDXVariantGroup
class SDXBoolean(SDXBase):

    ## @brief Constructor of SDXBoolean.
    #
    #  @param value   :[\b bool][\em optional] value for the sidex boolean (default: None)
    #  @param vhandle :[\b int][\em optional] handle of a sidex variant (default: None)
    #  @param thisown :[\b bool][\em optional] instance is owner of the sidex variant handle (default: True)
    #  @param borrowed : [\b bool][\em optional] borrowed sidex handle (default: False)
    def __init__(self, value=None, vhandle=None, thisown=True, borrowed=False):
        SDXBase.__init__(self, vhandle, thisown, borrowed)
        if self._vhandle is None:
            self.value = value

        return

        # ------------ properties -----------------
        # the code here is for documentation only
        self.value = None
        # ------------ properties -----------------

    ## @property value
    #  @brief [\b bool] The value of the variant.
    #
    #  Assigning a value creates a new handle. The reference counter of the existing handle is decremented.
    #
    #  @par Access:
    #    read/write
    #
    #  @sa sidex_Variant_As_Boolean()

    #  @cond NODOC
    @property
    def value(self):
        return sidex.sidex_Variant_As_Boolean(self._vhandle)

    @value.setter
    def value(self, value):
        if self._thisown and(self._vhandle is not None):
            sidex.sidex_Variant_DecRef(self._vhandle)
        self._thisown = True
        self._vhandle = sidex.sidex_Variant_New_Boolean(value)
    #  @endcond

    ## @brief Check if the variable is 0 for boolean check
    #
    #  @returns [\b bool] True if the value is not zero
    def __nonzero__(self):
        return self.value


## @brief Implementation of a \ref inttype "Integer type variant".
#  @ingroup SDXVariantGroup
#
#  A SIDEX integer is defined as signed int64.
class SDXInteger(SDXBase):

    ## @brief Constructor
    #
    #  If no handle is passed, a new one with the given value will be created and owned by the class instance.
    #
    #  @param value   :[\b int][\em optional] value for the sidex boolean
    #  @param vhandle :[\b int][\em optional] handle of a sidex variant
    #  @param thisown :[\b bool][\em optional] instance is owner of the sidex variant handle
    #  @param borrowed : [\b bool][\em optional] borrowed sidex handle (default: False)
    #
    #  @sa sidex_Variant_New_Integer()
    def __init__(self, value=None, vhandle=None, thisown=True, borrowed=False):
        SDXBase.__init__(self, vhandle, thisown, borrowed)
        if self._vhandle is None:
            self.value = value

        return

        # ------------ properties -----------------
        # the code here is for documentation only
        self.value = None
        # ------------ properties -----------------

    ## @property value
    #  @brief [\b int] The value
    #
    #  Assigning a value creates a new handle. The reference counter of the existing handle is decremented.
    #
    #  @par Access:
    #    read/write

    #  @cond NODOC
    @property
    def value(self):
        return sidex.sidex_Variant_As_Integer(self._vhandle)

    @value.setter
    def value(self, value):
        if self._thisown and(self._vhandle is not None):
            sidex.sidex_Variant_DecRef(self._vhandle)
        self._thisown = True
        self._vhandle = sidex.sidex_Variant_New_Integer(value)
    #  @endcond

    ## @brief Check if the variable is 0 for boolean check
    #
    #  @returns [\b bool] True if the value is not zero
    def __nonzero__(self):
        return self.value != 0

    ## @brief convert to int
    #
    #  @returns [\b int] integer value
    def __int__(self):
        if PY2:
            return int(self.value)
        else:
            return self.value

    ## @brief convert to int
    #
    #  This method keeps the class compatible to Python 2.
    #
    #  @returns [\b int] integer value
    def __long__(self):
        return self.value

    ## @brief Use the variant in slice expressions
    #
    #  @returns [\b int] integer value
    def __index__(self):
        return self.value


## @brief Implementation of a \ref floattype "Float type variant".
#  @ingroup SDXVariantGroup
class SDXFloat(SDXBase):

    ## @brief Constructor of SDXFloat.
    #
    #  @param value   :[\b float][\em optional] value for the sidex boolean
    #  @param vhandle :[\b int][\em optional] handle of a sidex variant
    #  @param thisown :[\b bool][\em optional] instance is owner of the sidex variant handle
    #  @param borrowed : [\b bool][\em optional] borrowed sidex handle (default: False)
    def __init__(self, value=None, vhandle=None, thisown=True, borrowed=False):
        SDXBase.__init__(self, vhandle, thisown, borrowed)
        if self._vhandle is None:
            self.value = value

        return

        # ------------ properties -----------------
        # the code here is for documentation only
        self.value = None
        # ------------ properties -----------------

    ## @property value
    #  @brief [\b float] The value
    #
    #  @par Access:
    #    read/write

    #  @cond NODOC
    @property
    def value(self):
        return sidex.sidex_Variant_As_Float(self._vhandle)

    @value.setter
    def value(self, value):
        if self._thisown and(self._vhandle is not None):
            sidex.sidex_Variant_DecRef(self._vhandle)
        self._thisown = True
        self._vhandle = sidex.sidex_Variant_New_Float(value)
    #  @endcond

    ## @brief Check if the variable is 0 for boolean check
    #
    #  @returns [\b bool] True if the value is not zero
    def __nonzero__(self):
        return self.value != 0.0

    ## @brief convert to float
    #
    #  @returns [\b float] integer value
    def __float__(self):
        return self.value


## @brief Implementation of a \ref datetimetype "DateTime type variant".
#  @ingroup SDXVariantGroup
#
#  The DateTime type is independent from platform, language and programming environment to avoid problems
#  exchanging date and time information. Datetime information is internally mapped to a string
#  representation of the format "YYYY-MM-DD hh:mm:ss:ttt".
class SDXDateTime(SDXBase):

    ## @brief Constructor of SDXDateTime.
    #
    #  @param value   :[\b str, \b datetime, \b tuple][\em optional] value for the sidex datetime.
    #                  Format like "YYYY-MM-DD hh:mm:ss:ttt" if passing a string.
    #  @param vhandle :[\b int][\em optional] handle of a sidex variant (default: None)
    #  @param thisown :[\b bool][\em optional] instance is owner of the sidex variant handle (default: True)
    #  @param borrowed : [\b bool][\em optional] borrowed sidex handle (default: False)
    def __init__(self, value=None, vhandle=None, thisown=True, borrowed=False):
        SDXBase.__init__(self, vhandle, thisown,borrowed)
        if self._vhandle is None:
            self.value = value

        return

        # ------------ properties -----------------
        # the code here is for documentation only
        self.value = None
        # ------------ properties -----------------

    ## @property value
    #  @brief [\b str, \b datetime, \b tuple] The value
    #
    #  Reading the value returns a time tuple. For assigning values other types like str and datetime or
    #  time.struct_time can be used.
    #
    #  @par Access:
    #    read/write

    #  @cond NODOC
    @property
    def value(self):
        return time.strptime(sidex.sidex_Variant_As_DateTime(self._vhandle), '%Y-%m-%d %H:%M:%S:%f')

    @value.setter
    def value(self, value):
        if self._thisown and(self._vhandle is not None):
            sidex.sidex_Variant_DecRef(self.svalue)
        self._thisown = True
        if isinstance(value, str):
            value = datetime.datetime.strptime(value, '%Y-%m-%d %H:%M:%S:%f')
            self._vhandle = sidex.sidex_Variant_New_DateTime(value)
        elif isinstance(value, (time.struct_time, tuple)):
            tempval = datetime.datetime.fromtimestamp(time.mktime(value))
            self._vhandle = sidex.sidex_Variant_New_DateTime(tempval)
        elif isinstance(value, (datetime.datetime, datetime.date)):
            self._vhandle = sidex.sidex_Variant_New_DateTime(value.replace(microsecond=0))
        else:
            raise TypeError("invalid date/time type")

    #  @endcond


## @brief Implementation of a \ref stringtype "String type variant".
#  @ingroup SDXVariantGroup
class SDXString(SDXBase):

    ## @brief Constructor of SDXString.
    #
    #  @param value   :[\b str][\em optional] value for the sidex string
    #  @param vhandle :[\b int][\em optional] handle of a sidex variant
    #  @param thisown :[\b bool][\em optional] instance is owner of the sidex variant handle
    #  @param borrowed : [\b bool][\em optional] borrowed sidex handle (default: False)
    def __init__(self, value=None, vhandle=None, thisown=True, borrowed=False):
        SDXBase.__init__(self, vhandle, thisown, borrowed)
        if self._vhandle is None:
            self.value = value

        return

        # ------------ properties -----------------
        # the code here is for documentation only
        self.value = None
        # ------------ properties -----------------

    ## @property value
    #  @brief [\b str] Value of the variant.
    #
    #  @par Access:
    #    read/write

    #  @cond NODOC
    @property
    def value(self):
        return sidex.sidex_Variant_As_String(self._vhandle)

    @value.setter
    def value(self, value):
        if self._thisown and(self._vhandle is not None):
            sidex.sidex_Variant_DecRef(self._vhandle)
        self._thisown = True
        self._vhandle = sidex.sidex_Variant_New_String(value)
    #  @endcond

    ## @brief Check if the variable is 0 for boolean check.
    #
    #  @returns [\b bool] True if the value is not zero
    def __nonzero__(self):
        return self.value != ""

    ## @brief Get length of string.
    #
    #  @returns {\b int] length of string
    def __len__(self):
        return sidex.sidex_Variant_As_String_Length(self._vhandle)


## @brief Implementation of a \ref bintype "Binary type variant".
#  @ingroup SDXVariantGroup
class SDXBinary(SDXBase):

    ## @brief Constructor of SDXBinary.
    #
    #  @param value   :[\b str][\em optional] value for the sidex binary
    #  @param vhandle :[\b int][\em optional] handle of a sidex variant
    #  @param thisown :[\b bool][\em optional] instance is owner of the sidex variant handle
    #  @param borrowed : [\b bool][\em optional] borrowed sidex handle (default: False)
    def __init__(self, value=None, vhandle=None, thisown=True, borrowed=False):
        SDXBase.__init__(self, vhandle, thisown, borrowed)
        if self._vhandle is None:
            self.value = value

        return

        # ------------ properties -----------------
        # the code here is for documentation only
        self.value = None
        # ------------ properties -----------------

    ## @property value
    #  @brief [\b str, \b bytes] Value of the variant.
    #
    #  @par Access:
    #    read/write

    #  @cond NODOC
    @property
    def value(self):
        return sidex.sidex_Variant_As_Binary(self._vhandle)

    @value.setter
    def value(self, value):
        if self._thisown and(self._vhandle is not None):
            sidex.sidex_Variant_DecRef(self._vhandle)
        self._thisown = True
        self._vhandle = sidex.sidex_Variant_New_Binary(value)
    #  @endcond

    ## @brief Check if the variable is 0 for boolean check
    #
    #  @returns [\b bool] True if the value is not zero
    def __nonzero__(self):
        return self.value != ""

    ## @brief Get length of binary data.
    #
    #  @returns {\b int] length in bytes
    def __len__(self):
        return sidex.sidex_Variant_As_Binary_Length(self._vhandle)


# @cond INTERNAL


## @brief Iterator class for SDXList.
class SDXListIter(object):
    def __init__(self, parent):
        self.__parent = parent
        self.__index = 0

    def __next__(self):
        if self.__index < len(self.__parent):
            result = self.__parent[self.__index]
            self.__index += 1
            return result
        else:
            raise StopIteration

    ## @brief Iterator function for python 2.x compatibility.
    def next(self):
        return self.__next__()
# @endcond


## @brief Implementation of a \ref listtype "List type variant".
#  @ingroup SDXVariantGroup
class SDXList(SDXBase):

    ## @brief Constructor of SDXList
    #
    #  @param vhandle :[\b int][\em optional] handle of a sidex variant
    #  @param thisown :[\b bool][\em optional] instance is owner of the sidex variant handle
    #  @param borrowed : [\b bool][\em optional] borrowed sidex handle (default: False)
    def __init__(self, vhandle=None, thisown=True, borrowed=False):
        SDXBase.__init__(self, vhandle, thisown, borrowed)
        if self._vhandle is None:
            self._thisown = True
            self._vhandle = sidex.sidex_Variant_New_List()

    ## @brief Append a value to the list.
    #
    #  @param value :[\b SDXBase, object] Value to append to the list
    #
    #  @sa sidex_Variant_List_Append()
    def append(self, value):
        sdxval = py_to_sdx(value)
        return sidex.sidex_Variant_List_Append(self._vhandle, sdxval.vhandle)

    ## @brief Insert a value into a specific position inside the list.
    #
    #  @param index :[\b int] position index
    #  @param value :[\b SDXBase, object] value to insert
    #
    #  @sa sidex_Variant_List_Insert()
    def insert(self, index, value):
        sdxval = py_to_sdx(value)
        return sidex.sidex_Variant_List_Insert(self._vhandle, sdxval.vhandle, index)

    ## @brief Number of items in the list.
    #
    #  @code{.py}
    #  sdxvar = sidex.variant.SDXList()
    #  sdxvar.append(3.14)
    #  sdxvar.append('Test')
    #  print('Number of items in the list: ',len(sdxvar))
    #  @endcode
    #
    #  The example will print
    #  @verbatim
    #  Number of items in the list: 2
    #  @endverbatim
    #
    #  @returns [\b int] number of items in the list
    #
    #  @sa sidex_Variant_List_Size()
    def __len__(self):
        return sidex.sidex_Variant_List_Size(self._vhandle)

    ## @brief Get an item by index.
    #
    #  Get an item from the list is similar to the python list type. Slicing is supported.
    #
    #  @code
    #  itemval = mylist[1]
    #  @endcode
    #
    #  @param index : [\b integer] item index
    #
    #  @return [\b SDXBase] value at index position
    #
    #  @exception IndexError index is out of bounds
    #
    #  @sa sidex_Variant_List_Get()
    def __getitem__(self, index):
        if isinstance(index, slice):
            result = SDXList()
            m = index.indices(len(self))
            for n in range(m[0], m[1], m[2]):
                result.append(self[n])
            return result
        else:
            try:
                hvariant = sidex.sidex_Variant_List_Get(self._vhandle, index)
            except sidex.sidexIndexError as E:
                raise IndexError(str(E))

            return wrap_sdx_handle(hvariant, owner=True, borrowed=True)

    ## @brief Replace an item at index position.
    #
    #  Replace an item in the list is similar to the python list type.
    #
    #  @code
    #  mylist[1] = 3.14
    #  @endcode
    #
    #  @param index : [\b integer] item index
    #  @param value : [\b SDXBase, object] new item value
    #
    #  @exception IndexError index is out of bounds
    #
    #  @sa sidex_Variant_List_Set()
    def __setitem__(self, index, value):
        sdxval = py_to_sdx(value)
        return sidex.sidex_Variant_List_Set(self._vhandle, sdxval.vhandle, index)

    ## @brief Delete an item from the list at index position.
    #
    #  Delete an item from the list is similar to the python list type.
    #
    #  @code
    #  del mylist[1]
    #  @endcode
    #
    #  @param index : [\b integer] item index
    #
    #  @exception IndexError index is out of bounds
    #
    #  @sa sidex_Variant_List_DeleteItem()
    def __delitem__(self, index):
        sidex.sidex_Variant_List_DeleteItem(self._vhandle, index)

    ## @brief Check if the list contains a value.
    #
    #  Currently this function is not supported. An identity check of variants is planned for the next version.
    #  If the \c in operator is used on SDXList an error is raised.
    #
    #  @exception TypeError unsupported function.
    def __contains__(self, item):
 				return item in sdx_to_py(self)

    ## @brief Return an iterator for the list.
    #
    #  @code{.py}
    #  sdxvar = sidex.variant.SDXList()
    #  sdxvar.append(3.14)
    #  sdxvar.append('Test')
    #  for value in sdxvar:
    #      print(value)
    #  @endcode
    def __iter__(self):
        return SDXListIter(self)

    ## @brief Check if the variable is 0 for boolean check
    #
    #  @returns [\b bool] True if the value is not zero
    def __nonzero__(self):
        return self.__len__() != 0


## @brief Implementation of a \ref dicttype "Dictionary type variant".
#
#  Unlike a python dictionary the SIDEX implementation of a dictionary accepts only stings as keys.
#
#  @ingroup SDXVariantGroup
class SDXDict(SDXBase):

    ## @brief Constructor of SDXDict
    #
    #  @param vhandle  :[\b int][\em optional] handle of a sidex variant (default: None)
    #  @param thisown :[\b bool][\em optional] instance is owner of the sidex variant handle (default: True)
    #  @param borrowed : [\b bool][\em optional] borrowed sidex handle (default: False)
    def __init__(self, vhandle=None, thisown=True, borrowed=False):
        SDXBase.__init__(self, vhandle, thisown, borrowed)
        if self._vhandle is None:
            self._thisown = True
            self._vhandle = sidex.sidex_Variant_New_Dict()

    ## @brief Number of key/value pairs.
    #
    #  @code{.py}
    #  sdxvar = sidex.variant.SDXDict()
    #  sdxvar['PI'] = 3.14
    #  sdxvar['Text'] = Test'
    #  print('Number of key/value pairs: ',len(sdxvar))
    #  @endcode
    #
    #  The example will print
    #  @verbatim
    #  Number of key/value pairs: 2
    #  @endverbatim
    #
    #  @returns [\b int] number of key/value pairs
    #
    #  @sa sidex_Variant_Dict_Size()
    def __len__(self):
        return sidex.sidex_Variant_Dict_Size(self._vhandle)

    ## @brief Get a value by key.
    #
    #  Reading a value from SDXDict is similar to the python dict type.
    #
    #  @code
    #  itemval = mylist['PI']
    #  @endcode
    #
    #  @param key : [\b str, \b SDXString] key name
    #
    #  @return [\b SDXBase] value referenced by key
    #
    #  @exception KeyError key not found
    #
    #  @sa sidex_Variant_Dict_Get()
    def __getitem__(self, key):
        lkey = sdx_to_py(key)
        if sidex.sidex_Variant_Dict_HasKey(self._vhandle, lkey):
            hvariant = sidex.sidex_Variant_Dict_Get(self._vhandle, lkey)
            return wrap_sdx_handle(hvariant, owner=True, borrowed=True)
        else:
            raise KeyError('key %s not found' % lkey)

    ## @brief Replace or set a key value/pair.
    #
    #  Replace or set a key value/pair in SDXDict is similar to the python dict type.
    #
    #  @code
    #  mylist['PI'] = 3.14+5
    #  @endcode
    #
    #  @param key   : [\b str] key name
    #  @param value : [\b SDXBase, objec] new item value
    #
    #  @sa sidex_Variant_Dict_Set()
    def __setitem__(self, key, value):
        sdxval = py_to_sdx(value)
        sidex.sidex_Variant_Dict_Set(self._vhandle, sdx_to_py(key), sdxval.vhandle)

    ## @brief Delete a key/value pair from a dictionary.
    #
    #  Delete from SDXDict is similar to the python dict type.
    #
    #  @code
    #  del mylist['PI']
    #  @endcode
    #
    #  @param key : [\b str] key name
    #
    #  @exception KeyError key not found
    #
    #  @sa sidex_Variant_Dict_Delete()
    def __delitem__(self, key):
        if sidex.sidex_Variant_Dict_HasKey(self._vhandle, sdx_to_py(key)):
            sidex.sidex_Variant_Dict_Delete(self._vhandle, sdx_to_py(key))
        else:
            raise KeyError('KeyError: %s' % key)

    ## @brief Get the dictionary's keys as a list
    #
    #  @returns [\b SDXList, SDXNone] return a list or a None object
    def keys(self):
        hvariant = sidex.sidex_Variant_Dict_Keys(self._vhandle)
        return wrap_sdx_handle(hvariant, owner=True)

    ## @brief Return key value pairs.
    #
    #  @code
    #    pval = sidex.variant.py_to_sdx({'PI': 3.14, 'Anwer': 42, 'SciFi': 'StarWars', 'When women say No': True, 'Nothing': None})
    #    for key, value in pval.items():
    #        print('iter ', key, value, type(value))
    #  @endcode
    #  The output of this example is:
    #  @verbatim
    #    iter  SciFi StarWars <class 'sidex.variant.SDXString'>
    #    iter  PI 3.14 <class 'sidex.variant.SDXFloat'>
    #    iter  Answer 42 <class 'sidex.variant.SDXInteger'>
    #    iter  When women say No True <class 'sidex.variant.SDXBoolean'>
    #    iter  Nothing None <class 'sidex.variant.SDXNone'>
    #  @endverbatim
    #
    #  @returns [\b list] a list of tuples containing key/value pairs
    def items(self):
        result = []
        sidex.sidex_Variant_Dict_First(self._vhandle)
        item = sidex.sidex_Variant_Dict_Next(self._vhandle)
        while item:
            result.append((item[0], wrap_sdx_handle(item[1], borrowed=True)))
            item = sidex.sidex_Variant_Dict_Next(self._vhandle)
        return result

    ## @brief Get key/value iterator
    def iteritems(self):
        raise Exception("iteritems not supported")

    ## @brief Check if the variable is 0 for boolean check
    #
    #  @returns [\b bool] True if the value is not zero
    def __nonzero__(self):
        return self.__len__() != 0


## @brief Implementation of a \ref tabletype "Table type variant".
#  @ingroup SDXVariantGroup
#
#  @snippet sdx_doc_02_table.py basic_usage
class SDXTable(SDXBase):

    ## @brief Constructor of SDXTable.
    #
    #  @param vhandle  :[\b int][\em optional] handle of a sidex variant (default: None)
    #  @param thisown :[\b bool][\em optional] instance is owner of the sidex variant handle (default: True)
    #  @param borrowed : [\b bool][\em optional] borrowed sidex handle (default: False)
    def __init__(self, vhandle=None, thisown=True, borrowed=False):
        SDXBase.__init__(self, vhandle, thisown, borrowed)
        if self._vhandle is None:
            self._thisown = True
            self._vhandle = sidex.sidex_Variant_New_Table()

            self.__current_row = 0
            self.__eot = self.row_count == 0

            return

            # ------------ properties ------------
            self.column_names = None
            self.column_count = 0
            self.row_count = 0
            self.eot = False
            self.row = None

    ## @property column_names
    #  @brief [\b SDXList] list of column names
    #
    #  @par Access:
    #    read
    #
    #  @sa sidex_Variant_Table_ColumnNames()

    #  @cond NODOC
    @property
    def column_names(self):
        return wrap_sdx_handle(sidex.sidex_Variant_Table_ColumnNames(self._vhandle))
    #  @endcond

    ## @property column_count
    #  @brief [\b int] Number of columns.
    #
    #  @par Access:
    #    read
    #
    #  @sa sidex_Variant_Table_Columns()

    #  @cond NODOC
    @property
    def column_count(self):
        return sidex.sidex_Variant_Table_Columns(self._vhandle)
    #  @endcond

    ## @property row_count
    #  @brief [\b int] Number of rows.
    #
    #  @par Access:
    #    read
    #
    #  @sa sidex_Variant_Table_Rows()

    #  @cond NODOC
    @property
    def row_count(self):
        return sidex.sidex_Variant_Table_Rows(self._vhandle)
    #  @endcond

    ## @property eot
    #  @brief [\b bool] end of table reached.
    #
    #  This property will be set on a call to \c first() or \c next().
    #
    #  @par Access:
    #    read
    #
    #  @sa sidex_Variant_Table_ColumnNames()

    #  @cond NODOC
    @property
    def eot(self):
        return self.__eot
    #  @endcond

    ## @property row
    #  @brief [\b SDXTableRow] current row data
    #
    #  @par Access:
    #    read

    #  @cond NODOC
    @property
    def row(self):
        return SDXTableRow(self, self.__current_row)
    #  @endcond

    ## @brief Add a column to the table.
    #
    #  If row data is already added to the table, a default value can be set for the
    #  new column. If no default value is given, it will be set to None.
    #
    #  @param key    : [\b str, \b list] name of the column
    #  @param default: [\b SDXBase][\em optional] default value of the column
    #
    #  @sa sidex_Variant_Table_AddColumn(), SDXTable.add_row()
    def add_column(self, key, default=None):
        if isinstance(key, list):
            for lkey in key:
                self.add_column(lkey, default)
        else:
            if not self.has_column(key):
                sidex.sidex_Variant_Table_AddColumn(self._vhandle, key)
            else:
                raise KeyError("column %s already exists" % key)

            if default is not None:
                lrow = self.__current_row
                self.first()
                while not self.eot:
                    self.row[key] = default
                    self.next()
                self.__current_row = lrow

    ## @brief Remove a column from the table.
    #
    #  @param key    : [\b str] name of the column to delete
    #
    #  @sa sidex_Variant_Table_DeleteColumn()
    def delete_column(self, key):
        if isinstance(key, list):
            for lkey in key:
                self.delete_column(lkey)
        else:
            if self.has_column(key):
                sidex.sidex_Variant_Table_DeleteColumn(self._vhandle, key)

    ## @brief Remove a row from the table.
    #
    #  @sa sidex_Variant_Table_DeleteRow()
    def delete_row(self):
        if self.row_count == 0:
            raise IndexError("table has no row to delete")
        else:
            sidex.sidex_Variant_Table_DeleteRow(self._vhandle, self.__current_row)
            if self.row_count == 0:
                self.__eot = True
                self.__current_row = 0
            elif self.__current_row > 0:
                self.__current_row -= 1

    ## @brief Move the cursor to the first row of the table.
    #
    #  If no rows available EOT will be set to False.
    #
    #  \code{.py}
    #    tablevar.first()
    #    while not tablevar.eot:
    #        for key, value in tablevar.row.items():
    #            print('item ', key, value)
    #        tablevar.next()
    #  \endcode
    #
    #  @sa SDXTable.eot, SDXTable.next
    def first(self):
        self.__eot = self.row_count == 0
        self.__current_row = 0

    ## @brief Move the cursor to the next row of the table.
    #
    #  @sa SDXTable.eot, SDXTable.first
    def next(self):
        self.__current_row += 1
        self.__eot = not self.__current_row < self.row_count
        if self.__eot:
            self.__current_row = self.row_count - 1
        return self.__eot

    ## @brief Check if a column name exists in the table.
    #
    #  The property \c column_names returns a list but it is
    #  recommended to use \c has_column to check for a specific name,
    #  because it does not retrieve the complete list of the columns.
    #
    #  @param name     :[\b str] column name
    #
    #  @returns [\b bool] True if the column name exists
    #
    #  @sa sidex_Variant_Table_HasColumn(), column_names
    def has_column(self, name):
        return sidex.sidex_Variant_Table_HasColumn(self._vhandle, name)

    ## @brief Add a new row to the table.
    #
    #  A row can be added without data. If data is passed, it must be a
    #  dictionary of column names and values. A subset of existing columns is possible.
    #
    #  \code{.py}
    #    pval = sidex.variant.SDXTable()
    #    pval.add_column(["Name", "Street", "Number"])
    #
    #    pval.add_row({"Name": "maik", "Street": "Wittland", "Number": 42})
    #    pval.add_row({"Name": "Hase", "Street": "Feld", "Number": 84})
    #  \endcode
    #
    #  @param data  : [\b dict][\em optional] row data
    #
    #  @sa sidex_Variant_Table_AddRow()
    def add_row(self, data=None):
        self.__current_row = sidex.sidex_Variant_Table_AddRow(self._vhandle)
        self.__eot = False
        ldata = py_to_sdx(data)
        if isinstance(ldata, SDXDict):
            try:
                for lkey, lvalue in ldata.items():
                    self.row[lkey] = lvalue
            except Exception as e:
                self.delete_row()
                raise e

    ## @brief Clear row data.
    #
    #  @sa sidex_Variant_Table_DeleteRows()
    def clear(self):
        sidex.sidex_Variant_Table_DeleteRows(self._vhandle)
        self.first()


## @brief SIDEX table row.
#
#  This class allows access to a table row data. Fields can be accessed by column names.
class SDXTableRow(object):

    ## @brief Constructor
    #
    #  @param table  :[\b SDXTable] parent object
    #  @param index  :[\b int]      index of the row inside the table
    def __init__(self, table, index):
        self.__index = index
        self.__table = table

    ## @brief Get a field value.
    #
    #  \code{.py}
    #  fieldval = tablevar.rows['Columnname']
    #  print(fieldval)
    #  \endcode
    #
    #  @param key : [\b str] column name
    #
    #  @sa SDXTableRow.__setitem__()
    def __getitem__(self, key):
        return wrap_sdx_handle(sidex.sidex_Variant_Table_GetField(self.__table.vhandle, self.__index, key))

    ## @brief Set a field value.
    #
    #  \code{.py}
    #  tablevar.rows['Columname'] = 42
    #  \endcode
    #
    #  @param key   : [\b str] column name
    #  @param value : [\b object, \b SDXBase] field value
    #
    #  @sa SDXTableRow.__getitem__()
    def __setitem__(self, key, value):
        if not self.__table.has_column(key):
            raise KeyError("column %s does not exist" % key)

        pyval = py_to_sdx(value)
        sidex.sidex_Variant_Table_SetField(self.__table.vhandle, self.__index, key, pyval.vhandle)

    ## @brief Number of columns.
    #
    #  @sa SDXTable.column_count
    def __len__(self):
        return self.__table.column_count

    ## @brief String representation of the row.
    #
    #  Convert the column data into a string.
    #
    #  @returns [\b str] string representation of the column
    def __str__(self):
        lrow = wrap_sdx_handle(sidex.sidex_Variant_Table_GetRow(self.__table.vhandle, self.__index), borrowed=True)
        return str(lrow)

    ## @brief Delete item not supported.
    #
    #  @param key : [\b str] name of the column
    #
    #  @exception TypeError Delete column is not supported for rows
    #
    #  @sa SDXTable.delete_column()
    def __delitem__(self, key):
        raise TypeError("delete not supported - use delete_column of the table object")

    ## @brief Return a list of key value pairs as tuple.
    #
    #   Column name and value are returned as a list of tuples.
    #
    #   \code{.py}
    #      for key, value in tablevar.row.items():
    #          print('item ', key, value)
    #   \endcode
    #
    #  @returns [\b list] a list of tuples
    def items(self):
        lrow = wrap_sdx_handle(sidex.sidex_Variant_Table_GetRow(self.__table.vhandle, self.__index), borrowed=True)
        if isinstance(lrow, SDXDict):
            return lrow.items()
        else:
            return []

    ## @brief Check if the variable is 0 for boolean check
    #
    #  @returns [\b bool] True if the value is not zero
    def __nonzero__(self):
        return self.__len__() != 0
