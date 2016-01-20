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

## @defgroup sdxGroup SIDEX API
#  @ingroup basicAPI
#  @brief Description of SIDEX API functions.

## @ingroup sdxGroup
#  @defgroup sdxgeneral General functions
#  @brief General SIDEX API functions.

## @ingroup sdxGroup
#  @defgroup sidexHandle SIDEX document
#  @brief Handling of SIDEX documents


## @ingroup sidexHandle
# @defgroup navigationType Group/Key managment
# @brief Managing groups and keys in SIDEX documents


## @ingroup sidexHandle
# @defgroup sdxDocRWValues Read/Write values
# @brief Read/Write values from/to the SIDEX document.


## @ingroup sdxGroup
#  @defgroup datatypes SIDEX datatypes
#  @brief SIDEX API for supported datatypes

## @ingroup datatypes
# @defgroup sdxconvert Conversion functions
# @brief Convert python and sidex datatypes

## @ingroup datatypes
# @defgroup varianttype General variant functions
# @brief General functions to handle data objects.


## @ingroup datatypes
# @defgroup simpletypes Simple data types
# @brief Simple data objects.

## @ingroup datatypes
# @defgroup complextypes container types
# @brief Datatypes containing multiple variant values.


## @ingroup simpletypes
# @defgroup inttype Integer
# @brief Handling of integer values


## @ingroup simpletypes
# @defgroup nonetype None
# @brief Handling of None values


## @ingroup simpletypes
# @defgroup booltype Boolean
# @brief Handling of Boolean values


## @ingroup simpletypes
# @defgroup floattype Float
# @brief Handling of Float values


## @ingroup simpletypes
# @defgroup stringtype String
# @brief Handling of String values

## @ingroup simpletypes
# @defgroup bintype Binary
# @brief Handling of Binary data

## @ingroup simpletypes
# @defgroup datetimetype DateTime
# @brief Handling of DateTime values


## @ingroup complextypes
# @defgroup listtype List
# @brief A list of values.


## @ingroup complextypes
# @defgroup dicttype Dictionary
# @brief Values organized in a dictionary with a string key.


## @ingroup complextypes
# @defgroup tabletype Table
# @brief A table with rows and columns.

## @ingroup sdxgeneral
# @brief    Returns current interface api and library version.
#
#  <TABLE>
#   <TR><TD><B>Name</B></TD><TD><B>Type    </B></TD><TD><B>Description          </B></TD></TR>
#   <TR><TD>apiVer   </TD><TD>integer </TD><TD>api version of TML            </TD></TR>
#   <TR><TD>libVer   </TD><TD>integer </TD><TD>library version of TML            </TD></TR>
#   <TR><TD>verStr  </TD><TD>string </TD><TD>version string              </TD></TR>
#  </TABLE>
#
# @return    [\b tuple]  (apiVer, libVer ,verStr)
def sidex_Get_Version(): pass


## @ingroup sdxgeneral
#  @brief   Returns the copyright information.
#
#  The copyright information contains all hints and links concerning the LGPL related
#  libraries used in this SDK.
#
#  @return  [\b str] copyright information
#
#  @see sidex_Get_Version()
def sidex_Get_Copyright(): pass

## @ingroup sdxgeneral
#  @brief    Set the license key.
# 
#  Before the sdk functions can be used a license key must be applied.
# 
#  @param    pUserName : [\b str] user name (case sensitive)
#  @param    pPassWord : [\b str] password (case insensitive)
def sidex_Set_Password(pUserName, pPassWord): pass

## @ingroup sidexHandle
#  @brief    Create a SIDEX document handle.
# 
#  SIDEX documents contain data based on SIDEX data objects organized in groups containing a number of keys.
#  Groups and keys are names to find a specific value in the document. The document can be exported to/imported from an
#  xml string or file.
# 
#  @param    pname : [\b str] name of the document/xml root node
#
#  @return        [\b int] SIDEX document handle (SIDEX_HANDLE)
def sidex_Create(pname): pass

## @ingroup sidexHandle
#  @brief    Change the document name / XML root node.
# 
#  @param    shandle : [\b int] SIDEX document handle (SIDEX_HANDLE)
#  @param    pname   : [\b str] name of the document/xml root node
# 
#  @see sidex_Get_DocumentName()
def sidex_Set_DocumentName(shandle, pname): pass

## @ingroup sidexHandle
#  @brief    Get the document name / XML root node name.
# 
#  @param    shandle : [\b int] SIDEX document handle (SIDEX_HANDLE)
#
#  @return    [\b str] name of the document/xml root node
# 
#  @see sidex_Set_DocumentName(), sidex_Load_Content(), sidex_Set_Content()
def sidex_Get_DocumentName(shandle): pass

## @ingroup sidexHandle
#  @brief    Remove all values from the sidex document.
# 
#  @param    shandle : [\b int] SIDEX document handle (SIDEX_HANDLE)
def sidex_Clear(shandle): pass

## @ingroup sidexHandle
#  @brief    Free a SIDEX document object.
# 
#  @param    shandle : [\b int] reference to SIDEX document handle (SIDEX_HANDLE)
# 
#  @see sidex_Create()
def sidex_Free(shandle): pass

## @ingroup sidexHandle
#  @brief Read a SIDEX document from a file.
# 
#  @note If the document already contains data it will be deleted.
# 
#  @param shandle : [\b int] SIDEX document handle (SIDEX_HANDLE)
#  @param path    : [\b str] path to SIDEX file
# 
#  @see sidex_Set_Content(), sidex_Save_Content()
def sidex_Load_Content(shandle, path): pass

## @ingroup sidexHandle
#  @brief Read a SIDEX document from a string.
# 
#  @note If the document already contains data it will be deleted.
# 
#  @param shandle : [\b int] SIDEX document handle (SIDEX_HANDLE)
#  @param content : [\b str] SIDEX document data
# 
#  @see sidex_Load_Content(), sidex_Get_Content()
def sidex_Set_Content(shandle, content): pass

## @ingroup sidexHandle
#  @brief Write a SIDEX document to a file.
# 
#  @note If the file exits, it will be overwritten.
# 
#  @param shandle : [\b int] SIDEX document handle (SIDEX_HANDLE)
#  @param path    : [\b str] file path and name
# 
#  @see sidex_Load_Content()
def sidex_Save_Content(shandle, path): pass

## @ingroup sidexHandle
#  @brief Get the SIDEX document data as string.
# 
#  @param    shandle : [\b int] SIDEX document handle (SIDEX_HANDLE)
#
#  @return   [\b str] content string
# 
#  @see sidex_Set_Content()
def sidex_Get_Content(shandle): pass

## @ingroup sidexHandle
#  @brief Get the buffer length of the document content returned by sidex_Get_Content().
# 
#  @param    shandle : [\b int] SIDEX document handle (SIDEX_HANDLE)
#  @return   [\b str] length of a string returned by sidex_Get_Content().
# 
#  @see sidex_Get_Content()
def sidex_Get_Content_Length(shandle): pass


## @ingroup sdxDocRWValues
#  @brief   Write a none value (empty value) to a SIDEX document.
# 
#  @param   shandle: [\b int] SIDEX document handle (SIDEX_HANDLE)
#  @param   ngroup : [\b str] name of the group
#  @param   nkey   : [\b str] name of the key
def sidex_None_Write(shandle,  ngroup, nkey): pass

## @ingroup sdxDocRWValues
#  @brief   Write a boolean value to a SIDEX document.
# 
#  @param   shandle: [\b int] SIDEX document handle (SIDEX_HANDLE)
#  @param   ngroup : [\b str] name of the group
#  @param   nkey   : [\b str] name of the key
#  @param   value  : [\b bool] value to write
def sidex_Boolean_Write(shandle,  ngroup, nkey, value): pass

## @ingroup sdxDocRWValues
#  @brief   Get a boolean value from a SIDEX document.
# 
#  @param   shandle: [\b int] SIDEX document handle (SIDEX_HANDLE)
#  @param   ngroup : [\b str] name of the group
#  @param   nkey   : [\b str] name of the key
#
#  @return [\b bool] value from the document
def sidex_Boolean_Read(shandle,  ngroup, nkey): pass

## @ingroup sdxDocRWValues
#  @brief   Write an integer value to a SIDEX document.
# 
#  @param   shandle: [\b int] SIDEX document handle (SIDEX_HANDLE)
#  @param   ngroup : [\b str] name of the group
#  @param   nkey   : [\b str] name of the key
#  @param   value  : [\b int] value to write
def sidex_Integer_Write(shandle,  ngroup, nkey, value): pass

## @ingroup sdxDocRWValues
#  @brief   Get an integer value from a SIDEX document.
# 
#  @param   shandle: [\b int] SIDEX document handle (SIDEX_HANDLE)
#  @param   ngroup : [\b str] name of the group
#  @param   nkey   : [\b str] name of the key
#
#  @return [\b int] value from the document
def sidex_Integer_Read(shandle,  ngroup, nkey): pass

## @ingroup sdxDocRWValues
#  @brief   Write a float value to a SIDEX document.
# 
#  @param   shandle: [\b int] SIDEX document handle (SIDEX_HANDLE)
#  @param   ngroup : [\b str] name of the group
#  @param   nkey   : [\b str] name of the key
#  @param   value  : [\b float] value to write
def sidex_Float_Write(shandle,  ngroup, nkey, value): pass

## @ingroup sdxDocRWValues
#  @brief   Get a float value from a SIDEX document.
# 
#  @param   shandle: [\b int] SIDEX document handle (SIDEX_HANDLE)
#  @param   ngroup : [\b str] name of the group
#  @param   nkey   : [\b str] name of the key
#
#  @return [\b float] value from the document
def sidex_Float_Read(shandle,  ngroup, nkey): pass

## @ingroup sdxDocRWValues
#  @brief   Write a SIDEX_VARIANT value to a SIDEX document.
# 
#  @param   shandle: [\b int] SIDEX document handle (SIDEX_HANDLE)
#  @param   ngroup : [\b str] name of the group
#  @param   nkey   : [\b str] name of the key
#  @param   value  : [\b int] SIDEX_VARIANT to write
def sidex_Variant_Write(shandle,  ngroup, nkey, value): pass

## @ingroup sdxDocRWValues
#  @brief   Get a SIDEX_VARIANT value from a SIDEX document.
# 
#  @param   shandle: [\b int] SIDEX document handle (SIDEX_HANDLE)
#  @param   ngroup : [\b str] name of the group
#  @param   nkey   : [\b str] name of the key
#
#  @return [\b int] SIDEX_VARIANT from the document
def sidex_Variant_Read(shandle,  ngroup, nkey): pass

## @ingroup sdxDocRWValues
#  @brief   Write a SIDEX_VARIANT of type datetime to a SIDEX document.
# 
#  @param   shandle: [\b int] SIDEX document handle (SIDEX_HANDLE)
#  @param   ngroup : [\b str] name of the group
#  @param   nkey   : [\b str] name of the key
#  @param   value  : [\b int] SIDEX_VARIANT to write
def sidex_DateTime_Write(shandle,  ngroup, nkey, value): pass

## @ingroup sdxDocRWValues
#  @brief   Get a SIDEX_VARIANT of type datetime from a SIDEX document.
# 
#  @param   shandle: [\b int] SIDEX document handle (SIDEX_HANDLE)
#  @param   ngroup : [\b str] name of the group
#  @param   nkey   : [\b str] name of the key
#
#  @return [\b int] SIDEX_VARIANT from the document
def sidex_DateTime_Read(shandle,  ngroup, nkey): pass

## @ingroup sdxDocRWValues
#  @brief   Get the length of a string value in a SIDEX document.
# 
#  @param   shandle: [\b int] SIDEX document handle (SIDEX_HANDLE)
#  @param   ngroup : [\b str] name of the group
#  @param   nkey   : [\b str] name of the key
#
#  @return [\b int] string length
def sidex_String_Length(shandle, ngroup, nkey): pass

## @ingroup sdxDocRWValues
#  @brief   Get a string value from a SIDEX document.
# 
#  @param   shandle: [\b int] SIDEX document handle (SIDEX_HANDLE)
#  @param   ngroup : [\b str] name of the group
#  @param   nkey   : [\b str] name of the key
#
#  @return [\b str] string from the SIDEX document
def sidex_String_Read(shandle,  ngroup, nkey): pass

## @ingroup sdxDocRWValues
#  @brief   Get the length of a binary value in a SIDEX document.
# 
#  @param   shandle: [\b int] SIDEX document handle (SIDEX_HANDLE)
#  @param   ngroup : [\b str] name of the group
#  @param   nkey   : [\b str] name of the key
#
#  @return [\b int] length
def sidex_Binary_Length(shandle, ngroup, nkey): pass

## @ingroup sdxDocRWValues
#  @brief   Get a binary value from a SIDEX document.
# 
#  @param   shandle: [\b int] SIDEX document handle (SIDEX_HANDLE)
#  @param   ngroup : [\b str] name of the group
#  @param   nkey   : [\b str] name of the key
#
#  @return [\b str] string (binary) from the SIDEX document
def sidex_Binary_Read(shandle,  ngroup, nkey): pass

## @ingroup sdxDocRWValues
#  @brief   Write a string value to a SIDEX document.
# 
#  @param   shandle: [\b int] SIDEX document handle (SIDEX_HANDLE)
#  @param   ngroup : [\b str] name of the group
#  @param   nkey   : [\b str] name of the key
#  @param   value  : [\b string/None] string or None for an empty string.
def sidex_String_Write(shandle, ngroup, nkey, value): pass

## @ingroup sdxDocRWValues
#  @brief   Write a binary value to a SIDEX document.
# 
#  @param   shandle: [\b int] SIDEX document handle (SIDEX_HANDLE)
#  @param   ngroup : [\b str] name of the group
#  @param   nkey   : [\b str] name of the key
#  @param   value  : [\b str/None] string (binary) or None for an empty string.
def sidex_Binary_Write(shandle, ngroup, nkey, value): pass

## @ingroup sdxDocRWValues
#  @brief   Write a list to a SIDEX document.
# 
#  @param   shandle: [\b int] SIDEX document handle (SIDEX_HANDLE)
#  @param   ngroup : [\b str] name of the group
#  @param   nkey   : [\b str] name of the key
#  @param   value  : [\b int] SIDEX_VARIANT to write
def sidex_List_Write(shandle,  ngroup, nkey, value): pass

## @ingroup sdxDocRWValues
#  @brief   Get a list from a SIDEX document.
# 
#  @param   shandle: [\b int] SIDEX document handle (SIDEX_HANDLE)
#  @param   ngroup : [\b str] name of the group
#  @param   nkey   : [\b str] name of the key
#
#  @return [\b int] SIDEX_VARIANT from the document
def sidex_List_Read(shandle,  ngroup, nkey): pass

## @ingroup sdxDocRWValues
#  @brief   Write a dictionary to a SIDEX document.
# 
#  @param   shandle: [\b int] SIDEX document handle (SIDEX_HANDLE)
#  @param   ngroup : [\b str] name of the group
#  @param   nkey   : [\b str] name of the key
#  @param   value  : [\b int] SIDEX_VARIANT to write
def sidex_Dict_Write(shandle,  ngroup, nkey, value): pass

## @ingroup sdxDocRWValues
#  @brief   Get a dictionary from a SIDEX document.
# 
#  @param   shandle: [\b int] SIDEX document handle (SIDEX_HANDLE)
#  @param   ngroup : [\b str] name of the group
#  @param   nkey   : [\b str] name of the key
#
#  @return [\b int] SIDEX_VARIANT from the document
def sidex_Dict_Read(shandle,  ngroup, nkey): pass

## @ingroup sdxDocRWValues
#  @brief   Write a table to a SIDEX document.
# 
#  @param   shandle: [\b int] SIDEX document handle (SIDEX_HANDLE)
#  @param   ngroup : [\b str] name of the group
#  @param   nkey   : [\b str] name of the key
#  @param   value  : [\b int] SIDEX_VARIANT to write
def sidex_Table_Write(shandle,  ngroup, nkey, value): pass

## @ingroup sdxDocRWValues
#  @brief   Get a table from a SIDEX document.
# 
#  @param   shandle: [\b int] SIDEX document handle (SIDEX_HANDLE)
#  @param   ngroup : [\b str] name of the group
#  @param   nkey   : [\b str] name of the key
#
#  @return [\b int] SIDEX_VARIANT from the document
def sidex_Table_Read(shandle,  ngroup, nkey): pass

## @ingroup navigationType
#  @brief   Get a list of group names from the SIDEX document.
# 
#  @param   shandle: [\b int] SIDEX document handle (SIDEX_HANDLE)
#
#  @return [\b int] SIDEX_VARIANT of type list containing the group names from the document
def sidex_GetGroups(shandle): pass

## @ingroup navigationType
#  @brief   Check if a group exists.
# 
#  @param   shandle: [\b int] SIDEX document handle (SIDEX_HANDLE)
#  @param   group  : [\b str]  group name
#
#  @return [\b bool] True=exists, False=found
def sidex_HasGroup(shandle, group): pass

## @ingroup navigationType
#  @brief   Get a list of key names from the SIDEX document.
# 
#  @param   shandle: [\b int] SIDEX document handle (SIDEX_HANDLE)
#  @param   group  : [\b str]  group name
#
#  @return [\b int] SIDEX_VARIANT of type list containing the key names from the document
def sidex_GetKeys(shandle, group): pass

## @ingroup navigationType
#  @brief   Check if a key exists.
# 
#  @param   shandle: [\b int] SIDEX document handle (SIDEX_HANDLE)
#  @param   group  : [\b str]  group name
#  @param   key    : [\b str]  key name
#
#  @return [\b bool] True=exists, False=not found
def sidex_HasKey(shandle, group, key): pass

## @ingroup navigationType
#  @brief   Delete a group from the document.
# 
#  @param   shandle: [\b int] SIDEX document handle (SIDEX_HANDLE)
#  @param   group  : [\b str]  group name
def sidex_DeleteGroup(shandle, group): pass

## @ingroup navigationType
#  @brief   Delete a key, value pair from the document.
# 
#  @param   shandle: [\b int] SIDEX document handle (SIDEX_HANDLE)
#  @param   group  : [\b str]  group name
#  @param   key    : [\b str]  key name
def sidex_DeleteKey(shandle, group, key): pass


## @ingroup varianttype
#  @brief   Decrement the reference counter of a SIDEX_VARIANT data object.
# 
#           A reference counter value of 0 signals that there is no more reference to the object.
#           In this case the SIDEX memory management will free the object.
# 
#  @param   variant : [\b int] SIDEX_VARIANT handle
# 
#  @see sidex_Variant_IncRef()
def sidex_Variant_IncRef(variant): pass

## @ingroup varianttype
#  @brief   Decrement the reference counter of a SIDEX_VARIANT data object.
# 
#           A reference counter value of 0 signals that there is no more reference to the object.
#           In this case the SIDEX memory management will free the object.
# 
#  @param   variant : [\b int] SIDEX_VARIANT handle
# 
#  @see sidex_Variant_IncRef()
def sidex_Variant_DecRef(variant): pass

## @ingroup varianttype
#  @brief   Increment the reference counter of a SIDEX_VARIANT data object.
# 
#           If a new reference to the SIDEX_VARIANT handle is created,
#           this function must be called. Many of the SIDEX API functions increment or
#           decrement the reference counter automatically. Please refer the function description.
# 
#  @param   variant : [\b int] SIDEX_VARIANT handle
# 
#  @see sidex_Variant_DecRef()
def sidex_Variant_IncRef(variant): pass

## @ingroup nonetype
#  @brief   Create a SIDEX_VARIANT of type none.
# 
#  @return [\b int] SIDEX_VARIANT handle<br>
#                   This call increments the reference counter of the SIDEX_VARIANT.<br>
#                   It must be decremented (sidex_Variant_DecRef()), if the value is no longer used.
def sidex_Variant_New_None(): pass

## @ingroup booltype
#  @brief   Create a SIDEX_VARIANT of type boolean.
# 
#  @param   value : [\b bool] True, False
# 
#  @return [\b int] SIDEX_VARIANT handle<br>
#                   This call increments the reference counter of the SIDEX_VARIANT.<br>
#                   It must be decremented (sidex_Variant_DecRef()), if the value is no longer used.
def sidex_Variant_New_Boolean(value): pass

## @ingroup inttype
#  @brief   Create a SIDEX_VARIANT of type integer.
# 
#  @param   value : [\b int] initial value
# 
#  @return [\b int] SIDEX_VARIANT handle<br>
#                   This call increments the reference counter of the SIDEX_VARIANT.<br>
#                   It must be decremented (sidex_Variant_DecRef()), if the value is no longer used.
def sidex_Variant_New_Integer(value): pass

## @ingroup floattype
#  @brief   Create a SIDEX_VARIANT of type float.
# 
#  @param   value : [\b float] initial value
# 
#  @return [\b int] SIDEX_VARIANT handle<br>
#                   This call increments the reference counter of the SIDEX_VARIANT.<br>
#                   It must be decremented (sidex_Variant_DecRef()), if the value is no longer used.
def sidex_Variant_New_Float(value): pass

## @ingroup datetimetype
#  @brief   Create a SIDEX_VARIANT of type datetime from a string.
# 
#           The value must be formatted according to the specification:<br>
#           YYYY-MM-DD hh:mm:ss:ttt <br><br>
#           YYYY year (0001-9999 / 0000 for undefined)<br>
#           MM monst (01-12 / 00 for undefined)<br>
#           DD day of month (01-31  / 00 for undefined)<br>
#           hh hour (00-23)<br>
#           mm minutes (00-59)<br>
#           ss seconds (00-59)<br>
#           ttt milliseseconds (000-999)<br>
#           The date attributes may also be undefined. In that case year, month and day must be set to 0.
#
#  @param   value : [\b str] initial value
# 
#  @return [\b int] SIDEX_VARIANT handle<br>
#                   This call increments the reference counter of the SIDEX_VARIANT.<br>
#                   It must be decremented (sidex_Variant_DecRef()), if the value is no longer used.
def sidex_Variant_New_DateTime(value): pass

## @ingroup stringtype
#  @brief   Create a SIDEX_VARIANT of type string.
# 
#  @param   value : [\b str] initial value
# 
#  @return [\b int] SIDEX_VARIANT handle<br>
#                   This call increments the reference counter of the SIDEX_VARIANT.<br>
#                   It must be decremented (sidex_Variant_DecRef()), if the value is no longer used.
def sidex_Variant_New_String(value): pass

## @ingroup bintype
#  @brief   Create a SIDEX_VARIANT of type binary from a char array.
# 
#  @param   value : [\b str, \b bytes] initial value
# 
#  @return [\b int] SIDEX_VARIANT handle<br>
#                   This call increments the reference counter of the SIDEX_VARIANT.<br>
#                   It must be decremented (sidex_Variant_DecRef()), if the value is no longer used.
def sidex_Variant_New_Binary(value): pass
 
## @ingroup listtype
#  @brief   Create a SIDEX_VARIANT of type list.
# 
#  @return [\b int] SIDEX_VARIANT handle<br>
#                   This call increments the reference counter of the SIDEX_VARIANT.<br>
#                   It must be decremented (sidex_Variant_DecRef()), if the value is no longer used.
def sidex_Variant_New_List(): pass

## @ingroup dicttype
#  @brief   Create a SIDEX_VARIANT of type dictionary.
# 
#  @return [\b int] SIDEX_VARIANT handle<br>
#                   This call increments the reference counter of the SIDEX_VARIANT.<br>
#                   It must be decremented (sidex_Variant_DecRef()), if the value is no longer used.
def sidex_Variant_New_Dict(): pass

## @ingroup tabletype
#  @brief   Create a SIDEX_VARIANT of type table.
# 
#  @return [\b int] SIDEX_VARIANT handle<br>
#                   This call increments the reference counter of the SIDEX_VARIANT.<br>
#                   It must be decremented (sidex_Variant_DecRef()), if the value is no longer used.
def sidex_Variant_New_Table(): pass

## @ingroup dicttype
#  @brief   Create a SIDEX_VARIANT of type dictionary with a predefined size.
# 
#  @param   iSize : [\b int] number of dictionary entries
#
#  @return [\b int] SIDEX_VARIANT handle<br>
#                   This call increments the reference counter of the SIDEX_VARIANT.<br>
#                   It must be decremented (sidex_Variant_DecRef()), if the value is no longer used.
def sidex_Variant_New_DictBySize(iSize): pass

## @ingroup varianttype
#  @brief   Returns the type of a SIDEX_VARIANT value.
# 
#   <TABLE>
#    <TR><TD><B>Name       </B></TD><TD><B>Value </B></TD><TD><B>Description  </B></TD></TR>
#    <TR><TD>SIDEX_DATA_TYPE_UNKNOWN </TD><TD>0 </TD><TD>error or value of SIDEX_HANDLE_TYPE_NULL</TD></TR>
#    <TR><TD>SIDEX_DATA_TYPE_NONE    </TD><TD>1 </TD><TD>None type</TD></TR>
#    <TR><TD>SIDEX_DATA_TYPE_BOOLEAN </TD><TD>2 </TD><TD>boolean value</TD></TR>
#    <TR><TD>SIDEX_DATA_TYPE_INTEGER </TD><TD>3 </TD><TD>integer value</TD></TR>
#    <TR><TD>SIDEX_DATA_TYPE_FLOAT   </TD><TD>4 </TD><TD>float value</TD></TR>
#    <TR><TD>SIDEX_DATA_TYPE_STRING  </TD><TD>5 </TD><TD>string value</TD></TR>
#    <TR><TD>SIDEX_DATA_TYPE_LIST    </TD><TD>6 </TD><TD>a list of values</TD></TR>
#    <TR><TD>SIDEX_DATA_TYPE_DICT    </TD><TD>7 </TD><TD>a dictionary</TD></TR>
#    <TR><TD>SIDEX_DATA_TYPE_TABLE   </TD><TD>8 </TD><TD>a table</TD></TR>
#    <TR><TD>SIDEX_DATA_TYPE_DATETIME</TD><TD>9 </TD><TD>date and time</TD></TR>
#    <TR><TD>SIDEX_DATA_TYPE_BINARY  </TD><TD>10 </TD><TD>binary string</TD></TR>
#   </TABLE>
# 
#  @param   variant : [\b int] SIDEX_VARIANT handle
# 
#  @returns [\b int] type of the value 
def sidex_Variant_GetType(variant): pass


## @ingroup varianttype
#  @brief   Returns a copy of a SIDEX_VARIANT.
# 
#  @param   variant : [\b int] SIDEX_VARIANT handle
# 
#  @returns  [\b int] a copy of a SIDEX_VARIANT handle on success or SIDEX_HANDLE_TYPE_NULL on error
def sidex_Variant_Copy(variant): pass

## @ingroup nonetype
#  @brief   Check if SIDEX_VARIANT type is none.
# 
#  @param   variant : [\b int] SIDEX_VARIANT handle
# 
#  @returns  [\b bool] true, false
def sidex_Variant_None_Check(variant): pass

## @ingroup booltype
#  @brief   Check if SIDEX_VARIANT type is boolean.
# 
#  @param   variant : [\b int] SIDEX_VARIANT handle
# 
#  @returns  [\b int] a copy of a SIDEX_VARIANT handle on success or SIDEX_HANDLE_TYPE_NULL on error
def sidex_Variant_Boolean_Check(variant): pass

## @ingroup inttype
#  @brief   Check if SIDEX_VARIANT type is integer.
# 
#  @param   variant : [\b int] SIDEX_VARIANT handle
# 
#  @returns  [\b bool] true, false
def sidex_Variant_Integer_Check(variant): pass

## @ingroup floattype
#  @brief   Check if SIDEX_VARIANT type is float.
# 
#  @param   variant : [\b int] SIDEX_VARIANT handle
# 
#  @returns  [\b bool] true, false
def sidex_Variant_Float_Check(variant): pass

## @ingroup datetimetype
#  @brief   Check if SIDEX_VARIANT type is datetime.
# 
#  @param   variant : [\b int] SIDEX_VARIANT handle
# 
#  @returns  [\b bool] true, false
def sidex_Variant_DateTime_Check(variant): pass


## @ingroup bintype
#  @brief   Check if SIDEX_VARIANT type is binary.
# 
#  @param   variant : [\b int] SIDEX_VARIANT handle
# 
#  @returns  [\b bool] true, false
def sidex_Variant_Binary_Check(variant): pass

## @ingroup stringtype
#  @brief   Check if SIDEX_VARIANT type is string.
# 
#  @param   variant : [\b int] SIDEX_VARIANT handle
# 
#  @returns  [\b bool] true, false
def sidex_Variant_String_Check(variant): pass

## @ingroup booltype
#  @brief   Get the value from a boolean SIDEX_VARIANT.
# 
#  @param   variant : [\b int] SIDEX_VARIANT handle
# 
#  @returns  [\b bool] variant value
def sidex_Variant_As_Boolean(variant): pass

## @ingroup inttype
#  @brief   Get the value from an integer SIDEX_VARIANT.
# 
#  @param   variant : [\b int] SIDEX_VARIANT handle
# 
#  @returns  [\b int] variant value
def sidex_Variant_As_Integer(variant): pass

## @ingroup floattype
#  @brief   Get the value from a float SIDEX_VARIANT.
# 
#  @param   variant : [\b int] SIDEX_VARIANT handle
# 
#  @returns  [\b float] variant value
def sidex_Variant_As_Float(variant): pass

## @ingroup datetimetype
#  @brief   Get the value from a datetime SIDEX_VARIANT. Returns a formatted string.
# 
#  refer to sidex_Variant_New_DateTime() for information about the string format.
# 
#  @param   variant : [\b int] SIDEX_VARIANT handle
# 
#  @returns  [\b str] variant value
#
#  @see sidex_Variant_New_DateTime()
def sidex_Variant_As_DateTime(variant): pass

## @ingroup stringtype
#  @brief   Get the length of a string SIDEX_VARIANT.
# 
#  @param   variant : [\b int] SIDEX_VARIANT handle
# 
#  @returns  [\b int] string length
#
#  @see sidex_Variant_As_String()
def sidex_Variant_As_String_Length(variant): pass

## @ingroup stringtype
#  @brief   Get value and length from a string SIDEX_VARIANT.
# 
#  @param   variant : [\b int] SIDEX_VARIANT handle
# 
#  @returns  [\b str] variant value
#
#  @see sidex_Variant_As_String_Length()
def sidex_Variant_As_String(variant): pass

## @ingroup bintype
#  @brief   Get the length from a binary SIDEX_VARIANT.
# 
#  @param   variant : [\b int] SIDEX_VARIANT handle
# 
#  @returns  [\b int] string length
#
#  @see sidex_Variant_As_Binary()
def sidex_Variant_As_Binary_Length(variant): pass

## @ingroup bintype
#  @brief   Get value and length from a binary SIDEX_VARIANT.
#  The return value is of type bytes in Python 3.
# 
#  @param   variant : [\b int] SIDEX_VARIANT handle
#
#  @returns  [\b str, \b bytes] variant value
# 
#  @see sidex_Variant_As_Binary_Length()
def sidex_Variant_As_Binary(variant): pass

## @ingroup stringtype
#  @brief   Set a string format attribute.
# 
#  A string can be formatted in a special way. The format attribute allows to
#  store this format information with the string.
# 
#  Valid formats: 
# 
#   <TABLE>
#    <TR><TD><B>Name       </B></TD><TD><B>Description  </B></TD></TR>
#    <TR><TD>unknown           </TD><TD>no special format (default)</TD></TR>
#    <TR><TD>sidex           </TD><TD>SIDEX formatted string</TD></TR>
#    <TR><TD>xml             </TD><TD>XML formatted string</TD></TR>
#    <TR><TD>json            </TD><TD>JSON formatted string</TD></TR>
#    <TR><TD>cvs             </TD><TD>comma seperated fields</TD></TR>
#    <TR><TD>ini             </TD><TD>window ini format</TD></TR>
#   </TABLE>
# 
#  @param   variant : [\b int] SIDEX_VARIANT handle
#  @param   format  : [\b int] string format
# 
#  @see sidex_Variant_String_GetFormat()
def sidex_Variant_String_SetFormat(variant, format): pass

## @ingroup stringtype
#  @brief   Get a string format attribute.
# 
#  See sidex_Variant_String_SetFormat() for further reference.
# 
#  @param   variant : [\b int] SIDEX_VARIANT handle
# 
#  @returns [\b int] format
# 
#  @see sidex_Variant_String_SetFormat()
def sidex_Variant_String_GetFormat(variant): pass

## @ingroup listtype
#  @brief   Check if SIDEX_VARIANT type is list.
# 
#  @param   variant : [\b int] SIDEX_VARIANT handle
# 
#  @returns  [\b bool] true, false
def sidex_Variant_List_Check(variant): pass

## @ingroup listtype
#  @brief   Get the number of items in a List.
# 
#  @param   variant : [\b int] SIDEX_VARIANT handle
#
#  @returns [\b int] item count
def sidex_Variant_List_Size(variant): pass

## @ingroup listtype
#  @brief   Append a value to a list.
# 
#  @param   variant : [\b int] SIDEX_VARIANT handle
#  @param   value   : [\b int] SIDEX_VARIANT handle of the value
# 
#  @returns [\b int] list index position of the new element
# 
#  @see sidex_Variant_List_Insert(), sidex_Variant_List_Set()
def sidex_Variant_List_Append(variant, value): pass

## @ingroup listtype
#  @brief   Insert a value into a list.
# 
#  @param   variant : [\b int] SIDEX_VARIANT handle
#  @param   value   : [\b int] SIDEX_VARIANT handle of the value
#  @param   position :[\b int] list index position
# 
#  @see sidex_Variant_List_Append(), sidex_Variant_List_Set()
def sidex_Variant_List_Insert(variant, value, position): pass

## @ingroup listtype
#  @brief   Overwrite a value in a list.
# 
#  @param   variant : [\b int] SIDEX_VARIANT handle
#  @param   value   : [\b int] SIDEX_VARIANT handle of the value
#  @param   position :[\b int] list index position
# 
#  @see sidex_Variant_List_Append(), sidex_Variant_List_Insert()
def sidex_Variant_List_Set(variant, value, position): pass

## @ingroup listtype
#  @brief   Remove a value from a list.
# 
#  @param   variant : [\b int] SIDEX_VARIANT handle
#  @param   position :[\b int] list index position
# 
#  @see sidex_Variant_List_Clear()
def sidex_Variant_List_DeleteItem(variant, position): pass

## @ingroup listtype
#  @brief   Remove all values from a list.
# 
#  @param   variant : [\b int] SIDEX_VARIANT handle
# 
#  @see sidex_Variant_List_DeleteItem()
def sidex_Variant_List_Clear(variant): pass

## @ingroup listtype
#  @brief   Get a value from a list.
# 
#  @param   variant : [\b int] SIDEX_VARIANT handle
#  @param   position :[\b int] list index position
# 
#  @returns [\b int] SIDEX_VARIANT handle
# 
#  @see sidex_Variant_List_Set()
def sidex_Variant_List_Get(variant, position): pass

## @ingroup dicttype
#  @brief   Check if SIDEX_VARIANT type is dictionary.
# 
#  @param   variant : [\b int] SIDEX_VARIANT handle
# 
#  @returns  [\b bool] true, false
def sidex_Variant_Dict_Check(variant): pass

## @ingroup dicttype
#  @brief   Write a value to a dictionary.
# 
#  @param   variant : [\b int] SIDEX_VARIANT handle
#  @param   key     : [\b str] key in the dictionary
#  @param   value   : [\b int] SIDEX_VARIANT handle of the value
# 
#  @returns [\b int] SIDEX_VARIANT handle
# 
#  @see sidex_Variant_Dict_Get()
def sidex_Variant_Dict_Set(variant, key, value): pass

## @ingroup dicttype
#  @brief   Get the number of entries in a dictionary.
# 
#  @param   variant : [\b int] SIDEX_VARIANT handle
# 
#  @returns [\b int] key/value pair count
def sidex_Variant_Dict_Size(variant): pass

## @ingroup dicttype
#  @brief   Get all key names of a dictionary.
# 
#  @param   variant : [\b int] SIDEX_VARIANT handle
# 
#  @returns [\b int] SIDEX_VARIANT handle of None or list  
# 
#  @see sidex_Variant_Dict_HasKey()
def sidex_Variant_Dict_Keys(variant): pass

## @ingroup dicttype
#  @brief   Set the cursor to the first element of a dictionary.
# 
#  @param   variant : [\b int] SIDEX_VARIANT handle
# 
#  @see sidex_Variant_Dict_Next()
def sidex_Variant_Dict_First(variant): pass

## @ingroup dicttype
#  @brief   Get the value from a dictionary at the current cursor position and move the cursor to the next value.
# 
#  @param   variant : [\b int] SIDEX_VARIANT handle
# 
#  @returns [\b (string, integer)] element key, SIDEX_VARIANT handle of the value
# 
#  @see sidex_Variant_Dict_First()
def sidex_Variant_Dict_Next(variant): pass

## @ingroup dicttype
#  @brief   Check if a key exists in a dictionary.
# 
#  @param   variant : [\b int] SIDEX_VARIANT handle
#  @param   key     : [\b str] key in the dictionary
# 
#  @returns [\b bool] true if key exists
# 
#  @see sidex_Variant_Dict_Keys()
def sidex_Variant_Dict_HasKey(variant, key): pass

## @ingroup dicttype
#  @brief   Delete a value from a dictionary.
# 
#  @param   variant : [\b int] SIDEX_VARIANT handle
#  @param   key     : [\b str] key in the dictionary
# 
#  @see sidex_Variant_Dict_Clear()
def sidex_Variant_Dict_Delete(variant, key): pass

## @ingroup dicttype
#  @brief   Remove all entries of from a dictionary.
# 
#  @param   variant : [\b int] SIDEX_VARIANT handle
# 
#  @see sidex_Variant_Dict_Delete()
def sidex_Variant_Dict_Clear(variant): pass

## @ingroup dicttype
#  @brief   Get a value from a dictionary.
# 
#  @param   variant : [\b int] SIDEX_VARIANT handle
#  @param   key     : [\b str] key in the dictionary
# 
#  @returns [\b int] SIDEX_VARIANT handle of the value
# 
#  @see sidex_Variant_Dict_Set()
def sidex_Variant_Dict_Get(variant, key): pass

## @ingroup tabletype
#  @brief   Check if SIDEX_VARIANT type is table.
# 
#  @param   variant : [\b int] SIDEX_VARIANT handle
# 
#  @returns  [\b bool] true, false
def sidex_Variant_Table_Check(variant): pass

##  @ingroup tabletype
#  @brief   Get the number of rows of a table.
# 
#  @param   variant : [\b int] number of rows
# 
#  @returns [\b int] number of rows
# 
#  @see sidex_Variant_Table_Columns()
def sidex_Variant_Table_Rows(variant): pass

## @ingroup tabletype
#  @brief   Get the number of columns of a table.
# 
#  @param   variant : [\b int] number of columns
# 
#  @returns [\b int] number of columns
# 
#  @see sidex_Variant_Table_Rows(), sidex_Variant_Table_ColumnNames()
def sidex_Variant_Table_Columns(variant): pass

## @ingroup tabletype
#  @brief   Get column names of a table.
# 
#  @param   variant : [\b int] SIDEX_VARIANT handle
# 
#  @returns [\b int] SIDEX_VARIANT handle to a list of column names
# 
#  @see sidex_Variant_Table_Columns(), sidex_Variant_Table_HasColumn()
def sidex_Variant_Table_ColumnNames(variant): pass

## @ingroup tabletype
#  @brief   Check if a column name exists.
# 
#  @param   variant : [\b int] SIDEX_VARIANT handle
#  @param   cname   : [\b str] column name
# 
#  @returns [\b bool] true = column exists
# 
#  @see sidex_Variant_Table_ColumnNames()
def sidex_Variant_Table_HasColumn(variant, cname): pass

## @ingroup tabletype
#  @brief   Add a column to a Table.
# 
#  For column fields in existing rows the value will be set to None.
# 
#  @param   variant : [\b int] SIDEX_VARIANT handle
#  @param   cname   : [\b str] column name
# 
#  @see sidex_Variant_Table_AddRow(), sidex_Variant_Table_DeleteColumn
def sidex_Variant_Table_AddColumn(variant, cname): pass

## @ingroup tabletype
#  @brief   Remove a column from a Table.
# 
#  @param   variant : [\b int] SIDEX_VARIANT handle
#  @param   cname   : [\b str] column name
# 
#  @see sidex_Variant_Table_AddColumn()
def sidex_Variant_Table_DeleteColumn(variant, cname): pass

## @ingroup tabletype
#  @brief    Get the name of a table column.
# 
#  @param   variant  : [\b int] SIDEX_VARIANT handle
#  @param   colindex : [\b int] column index
# 
#  @returns [\b str] column name
def sidex_Variant_Table_GetColumnName(variant, colindex): pass

## @ingroup tabletype
#  @brief   Add a row to a table.
# 
#  Fields of the new row will be set to None.
# 
#  @note Rows can be added before columns but
#  sidex_Variant_Table_GetRow() will return SIDEX_ERR_NOCONTENT.
# 
#  @param   variant  : [\b int] SIDEX_VARIANT handle
# 
#  @returns [\b int] row index
# 
#  @see sidex_Variant_Table_AddColumn()
def sidex_Variant_Table_AddRow(variant): pass

## @ingroup tabletype
#  @brief   Delete a row from a table.
# 
#  @param   variant  : [\b int] SIDEX_VARIANT handle
#  @param   rowindex : [\b int] row index
# 
#  @see sidex_Variant_Table_AddRow(), sidex_Variant_Table_DeleteRows()
def sidex_Variant_Table_DeleteRow(variant, rowindex): pass

## @ingroup tabletype
#  @brief   Delete all rows from a table.
# 
#  @param   variant  : [\b int] SIDEX_VARIANT handle
# 
#  @see sidex_Variant_Table_DeleteRow()
def sidex_Variant_Table_DeleteRows(variant): pass

## @ingroup tabletype
#  @brief   Get row from a Table.
# 
#  @param   variant  : [\b int] SIDEX_VARIANT handle
#  @param   rowindex : [\b int] row index
# 
#  @returns [\b int] SIDEX_VARIANT handle with row data as a dictionary
# 
#  @see sidex_Variant_Table_AddRow()
def sidex_Variant_Table_GetRow(variant, rowindex): pass

## @ingroup tabletype
#  @brief   Get field from a Table.
# 
#  @param   variant  : [\b int] SIDEX_VARIANT handle
#  @param   rowindex : [\b int] row index
#  @param   colname  : [\b str] name of the column
# 
#  @returns  [\b int] SIDEX_VARIANT handle with the value
# 
#  @see sidex_Variant_Table_SetField()
def sidex_Variant_Table_GetField(variant, rowindex, colname): pass

## @ingroup tabletype
#  @brief   Set a field in a table.
# 
#  @note If the row index does not exist, the missing rows will automatically be added to the table.
# 
#  @param   variant  : [\b int] SIDEX_VARIANT handle
#  @param   rowindex : [\b int] row index
#  @param   colname  : [\b str] name of the column
#  @param   value    : [\b int] SIDEX_VARIANT handle
# 
#  @see sidex_Variant_Table_GetField()
def sidex_Variant_Table_SetField(variant, rowindex, colname, value): pass

## @ingroup sdxconvert
#  @brief   Convert a python object to a SIDEX datatype if possible.
# 
#  @param   value  : [\b object] python object to convert
#
#  @returns [\b int] SIDEX_VARIANT handle with the value
#
#  @see SidexVariantAsPyObject()
def PyObjectAsSidexVariant(value): pass

## @ingroup sdxconvert
#  @brief   Convert a SIDEX variant into a python object if possible.
# 
#  @param   value  : [\b int] SIDEX_VARIANT handle
#
#  @returns [\b object] a python object
#
#  @see PyObjectAsSidexVariant()
def SidexVariantAsPyObject(value): pass