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
import sidex.variant

from sidex.constants import *

## @defgroup SDXDocGroup SIDEX document
#  @ingroup highLevelClasses
#  @brief Using SIDEX XML documents.
#
#  The SIDEX document manages a group/key based data container. Data types are
#  handled automatically by the document class and the variant data objects to ensure the type is
#  saved together with the data.
#
#  @section SIDEX Document example
#  The example shows the creation of a sidex document and how to add some basic data in the \c General group.
#  The \c Container group contains a list of basic types and other container types like a list
#  and a dictionary. Nesting of container types is the foundation to handle complex data structures.
#
#  @snippet sdx_doc_01.py basic_usage
#
#  The resulting SIDEX XML file contains all data types.
#
#  @include sdx_doc_01_result.xml


## @brief SIDEX parameter group.
#  @ingroup SDXDocGroup
#
#  The class emulates a dictionary to simplify access of the key value pairs
#  in a parameter group.
class SDXDocumentGroup(object):

    ## @brief Constructor
    #
    #  @param sdxdoc  :[\b SDXDocument] parent document
    #  @param grpname :[\b str] name of the group
    def __init__(self, sdxdoc, grpname):
        self.__grpname = sidex.variant.sdx_to_py(grpname)
        self.__sdxdoc = sdxdoc

    ## @brief Get an value by key name.
    #
    #  To read a value from a SIDEX document the SDXDocument instance can be used like
    #  a nested dictionary.
    #
    #  @code{.py}
    #  value = sdx_document['General']['ParamKey']
    #  @endcode
    #
    #  @param key : [\b str] name of the key
    #
    #  @returns [\b sidex.variant.SDXBase] the value
    #
    #  @exception KeyError the requested key does not exist
    #
    #  @sa SDXDcoument.__getitem__(), SDXDocument.read_value(), SDXDocument.autocast
    def __getitem__(self, key):
        lkey = sidex.variant.sdx_to_py(key)
        if sidex.sidex_HasKey(self.__sdxdoc.shandle, self.__grpname, lkey):
            if self.__sdxdoc.autocast:
                return sidex.variant.sdx_to_py(self.__sdxdoc.read_value(self.__grpname, lkey))
            else:
                return self.__sdxdoc.read_value(self.__grpname, lkey)
        else:
            raise KeyError("invalid key %s" % lkey)

    ## @brief Add or replace a key value pair.
    #
    #  Values can be added to a document by assigning to the double indexed SDXDocument instance.
    #
    #  @code
    #  sdoc = sidex.document.SDXDocument('SDXDocTest')
    #  sdoc['MyGroup']['MyKey'] = 3.14
    #  @endcode
    #
    #  @param key   : [\b str] name of the key
    #  @param value : [\b sidex.variant.SDXBase, \b str, \b list, \b tuple, \b dict, \b float,
    #    \b int, \b bool, \b none] value to write
    #
    #  @sa SDXDocument.write_value()
    def __setitem__(self, key, value):
        self.__sdxdoc.write_value(self.__grpname, sidex.variant.sdx_to_py(key), value)

    ## @brief Delete a key value pair from the document.
    #
    #  To Deleta a key/value pair from the document the \c del command
    #  can be used.
    #
    #  @code{.py}
    #  del sdx_document['MyGroup']['MyParamKey']
    #  @endcode
    #
    #  @param key : [\b str] name of the key
    def __delitem__(self, key):
        lkey = sidex.variant.sdx_to_py(key)
        if sidex.sidex_HasKey(self.__sdxdoc.shandle, self.__grpname, lkey):
            sidex.sidex_DeleteKey(self.__sdxdoc.shandle, self.__grpname, lkey)

    ## @brief Get a list of key names.
    #
    #  @code{.py}
    #  keynames = sdxdocument['MyGroup'].keys()
    #  @endcode
    #
    #  @returns [\b sidex.variant.SDXList] a list of strings containing the keys
    #
    #  @sa SDXDocumentGroup.keys()
    def keys(self):
        return sidex.variant.wrap_sdx_handle(sidex.sidex_GetKeys(self.__sdxdoc.shandle, self.__grpname))

    ## @brief Get the number of keys in a group.
    #
    #  @returns [\b int] number of keys
    def __len__(self):
        return len(self.keys())

    ## @brief Check if a key with name exists.
    #
    #  @param name : [\b str] key name
    #
    #  @returns [\b bool] True if the key exists
    def has_key(self, name):
        return sidex.sidex_HasKey(self.__sdxdoc.shandle, self.__grpname, sidex.variant.sdx_to_py(name))


## @brief SIDEX document.
#  @ingroup SDXDocGroup
#
#  A SIDEX document can be converted to a SIDEX XML string to be saved as a file or to be send to as a message
#  to another process or host. If a SIDEX file or string has to be parsed, it is simply loaded into a SIDEX
#  document that enables type safe access and modification.
class SDXDocument(object):

    ## @brief Constructor
    #
    #  @param document  :[\b int, \b str] pass either the handle of a
    #    sidex document or the name of the sidex document. The name will be used as root node.
    def __init__(self, document=None, content=None):
        if document is None:
            document = "SDXRoot"

        if PY3 and isinstance(document, str):
            self.__thisown = True
            self.__shandle = sidex.sidex_Create(document)
        elif PY2 and isinstance(document, (str, unicode)):
            self.__thisown = True
            self.__shandle = sidex.sidex_Create(document)
        elif PY3 and isinstance(document, int):
            self.__thisown = False
            self.__shandle = document
        elif PY2 and isinstance(document, long):
            self.__thisown = False
            self.__shandle = document
        else:
            raise TypeError("invalid document type %s" % (type(document)))

        if content is not None:
            self.content = content

        self.__autocast = True

        return

        # ------------ properties -----------------
        # the code here is for documentation only
        self.shandle = None
        self.content = ''
        self.document_name = ''
        self.autocast = True
        # ------------ properties -----------------

    ## @brief Destructor of SDXDocument.
    #
    #  If the document object owns the SIDEX handle, it will be freed on destruction.
    def __del__(self):
        if self.__thisown:
            sidex.sidex_Free(self.__shandle)

    ## @property autocast
    #  @brief [\b bool] automatically cast SDX variant objects to python.
    #
    #  If values are read from the document with __getitem__, they are automatically
    #  casted to python objects. If this cast is not necessary assign False to this property.
    #
    #  @par Access:
    #    read/write
    #

    #  @cond NODOC
    @property
    def autocast(self):
        return self.__autocast

    @autocast.setter
    def autocast(self, value):
        self.__autocast = value
    #  @endcond

    ## @property document_name
    #  @brief [\b str] document and root node name in XML data
    #
    #  @par Access:
    #    read/write
    #
    #  @sa sidex_Get_DocumentName(), sidex_Set_DocumentName()

    #  @cond NODOC
    @property
    def document_name(self):
        return sidex.sidex_Get_DocumentName(self.__shandle)

    @document_name.setter
    def document_name(self, value):
        sidex.sidex_Set_DocumentName(self.__shandle, value)
    #  @endcond

    ## @property shandle
    #  @brief [\b None] SIDEX document handle
    #
    #  @par Access:
    #    read

    #  @cond NODOC
    @property
    def shandle(self):
        return self.__shandle
    #  @endcond

    ## @property content
    #  @brief [\b str] Accessing XML content of the document.
    #
    #  @par Access:
    #    read/write
    #
    #  @sa __str__(), sidex_Get_Content(), sidex_Set_Content()

    #  @cond NODOC
    @property
    def content(self):
        return sidex.sidex_Get_Content(self.__shandle)

    @content.setter
    def content(self, value):
        sidex.sidex_Set_Content(self.__shandle, sidex.variant.sdx_to_py(value))
    #  @endcond

    ## @brief Access a parameter group by name.
    #
    #  @param key : [\b str] name of the parameter group
    #
    #  @returns   [\b SDXDocumentGroup] parameter group object
    #
    #  @exception KeyError key does not exist
    def __getitem__(self, key):
        return SDXDocumentGroup(self, key)

    ## @brief Set a group of values.
    #
    #  Setting a group of values directly requires a dictionary with alphanumeric keys matching
    #  the specification for XML tag names.
    #
    #  @code{.py}
    #  sdoc = sidex.document.SDXDocument('SDXDocTest')
    #  sdoc['MyGroup'] = {'KeyA':42, 'PI':3.14}
    #  @endcode
    #
    #  An existing group will be replaced.
    #
    #  @param key   : [\b str] name of the group
    #  @param value : [\b dict] dictionary with string keys and values
    #
    #  @exception TypeError group does not match the requirements
    def __setitem__(self, key, value):
        if not isinstance(value, dict):
            raise TypeError("value is not a dictionary")

        for lkey in value.keys():
            if not isinstance(key, str):
                raise TypeError("key in group in not a string %s" % lkey)

        if sidex.sidex_HasGroup(self.__shandle, key):
            self.__delitem__(key)

        for lkey, lvalue in value.items():
            self.write_value(key, lkey, lvalue)

    ## @brief Delete a group from the document.
    #
    #  To delete a complete parameter group from the document the \c del command
    #  can be used.
    #
    #  @code{.py}
    #  del sdx_document['MyGroup']
    #  @endcode
    #
    #  @param key : [\b str] name of the group
    def __delitem__(self, key):
        if sidex.sidex_HasGroup(self.__shandle, key):
            sidex.sidex_DeleteGroup(self.__shandle, key)

    ## @brief Get a list of group names.
    #
    #  The SIDEX document works more a less like a python dict. That's why the function name is \c keys
    #  and not \c groups. In fact it returns a list of group names.
    #
    #  @code{.py}
    #  groupnames = sdxdocument.keys()
    #  @endcode
    #
    #  @returns [\b sidex.variant.SDXList] a list of strings containing the keys
    #
    #  @sa SDXDocumentGroup.keys()
    def keys(self):
        return sidex.variant.wrap_sdx_handle(sidex.sidex_GetGroups(self.__shandle), owner=True)

    ## @brief Get the number of groups in the document.
    #
    #  @returns [\b int] number of groups
    def __len__(self):
        return len(self.keys())

    ## @brief Convert the document data into a SIDEX XML string.
    #
    #  @returns [\b str] SIDEX formatted XML string
    def __str__(self):
        return self.content

    ## @brief Write a value to the document.
    #
    #  @param group : [\b str] group name
    #  @param key   : [\b str] key name
    #  @param value : [\b sidex.variant.SDXBase, \b str, \b list, \b tuple, \b dict, \b float,
    #    \b int, \b bool, \b none] value to write
    def write_value(self, group, key, value):
        sdxvalue = sidex.variant.py_to_sdx(value)
        sidex.sidex_Variant_Write(self.__shandle, sidex.variant.sdx_to_py(group), sidex.variant.sdx_to_py(key), sdxvalue.vhandle)

    ## @brief Read a value from the document.
    #
    #  @param group : [\b str] group name
    #  @param key   : [\b str] key name
    #
    #  @return         [\b sidex.variant.SDXBase] value from the document
    def read_value(self, group, key):
        return sidex.variant.wrap_sdx_handle(sidex.sidex_Variant_Read(self.__shandle, sidex.variant.sdx_to_py(group),
                                                                      sidex.variant.sdx_to_py(key)), owner=True)

    ## @brief Save content to file.
    #
    #  @param path : [\b str] filename and path
    def save_to_file(self, path):
        sidex.sidex_Save_Content(self.__shandle, path)

    ## @brief Load content from file.
    #
    #  @param path : [\b str] filename and path
    def load_from_file(self, path):
        sidex.sidex_Load_Content(self.__shandle, path)

    ## @brief Delete all values from the document.
    #
    #  @sa sidex_Clear()
    def clear(self):
        sidex.sidex_Clear(self.__shandle)

    ## @brief Check if a group with \c name exists.
    #
    #  @param name : [\b str] group name
    #
    #  @returns [\b bool] True if the group exists
    #
    #  @sa sidex_HasGroup()
    def has_key(self, name):
        return sidex.sidex_HasGroup(self.__shandle, name)





