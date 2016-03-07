/* 
 *  libTML:  A BEEP based Messaging Suite
 *  Copyright (C) 2015 wobe-systems GmbH
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  as published by the Free Software Foundation; either version 2.1
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this program; if not, write to the Free
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *  02111-1307 USA
 *
 *  You may find a copy of the license under this software is released
 *  at COPYING file. This is LGPL software: you are welcome to develop
 *  proprietary applications using this library without any royalty or
 *  fee but returning back any change, improvement or addition in the
 *  form of source code, project image, documentation patches, etc.
 *
 *  Homepage:
 *    http://www.libtml.org
 *
 *  For professional support contact us:
 *
 *    wobe-systems GmbH
 *    support@libtml.org
 */
 
#ifdef LINUX
#include <string.h>
#else
#include <Windows.h>
#endif
#include <sidex.h>
#include <Python.h>
#include <structmember.h>
#include "_sidex_conversion.h"
#include <iostream>
#include "_py_unicode.h"
#include "_sidex_conversion.h"

using namespace std;

/*
 *************************************************************************************
 *                                 exception definition
 *************************************************************************************
 */
static PyObject *sidexError;
static PyObject *sidexIndexError;

/*
 *************************************************************************************
 *                                  function definitions
 *************************************************************************************
 */

/*
 *------------------------------------------------------
 *               sidex_Get_Version
 *------------------------------------------------------
 */
static PyObject * _sidex_Get_Version(PyObject *self)
{
    SIDEX_INT32   apiVer = 0;
    SIDEX_INT32   libVer  = 0;
    SIDEX_TCHAR*  libStr;

    SIDEX_INT32   err = SIDEX_SUCCESS;

    Py_BEGIN_ALLOW_THREADS;
    try {
        sidex_Get_Version(&apiVer, &libVer, &libStr);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Get_Version failed (%ld)",err);
            return NULL;
    }
    
    PyObject* pyLib = utf32_utf16_decode(libStr, -1);
    PyObject* pRetVal = Py_BuildValue("(l,l,O)", apiVer, libVer, pyLib);
    Py_XDECREF(pyLib);

    return pRetVal;
}

/*
 *------------------------------------------------------
 *               sidex_Get_Copyright
 *------------------------------------------------------
 */
static PyObject * _sidex_Get_Copyright(PyObject *self, PyObject *args)
{
    SIDEX_TCHAR*  sresult;
    SIDEX_INT32   slength;

    SIDEX_INT32    err = SIDEX_SUCCESS;

    Py_BEGIN_ALLOW_THREADS;
    try {
        sidex_Get_Copyright(&sresult, &slength);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Get_Copyright failed (%ld)",err);
            return NULL;
    }

    PyObject* py_copyright = utf32_utf16_decode(sresult, slength);
    PyObject* pRetVal = Py_BuildValue("O#",py_copyright, slength);
    Py_XDECREF(py_copyright);

    return pRetVal;
}


/*
 *------------------------------------------------------
 *               sidex_Set_Password
 *------------------------------------------------------
 */
static PyObject * _sidex_Set_Password(PyObject *self, PyObject *args)
{
    PyObject*    py_UserName16;
    PyObject*    py_PassWord16;
    SIDEX_TCHAR* pUserName;
    SIDEX_TCHAR* pPassWord;
    PyObject*    py_UserName;
    PyObject*    py_PassWord;
    
    SIDEX_INT32   err = SIDEX_SUCCESS;

    if (!PyArg_ParseTuple(args, "OO", &py_UserName, &py_PassWord))
        return NULL;
    if ( _unicodeConversion(py_UserName, &pUserName, &py_UserName16, sidexError) ) return NULL;
    if ( _unicodeConversion(py_PassWord, &pPassWord, &py_PassWord16, sidexError) ) return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
      err = sidex_Set_Password(pUserName, pPassWord);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    unicodeFree(py_UserName16, pUserName);
    unicodeFree(py_PassWord16, pPassWord);

    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Set_Password failed (%ld)",err);
            return NULL;
    }

    Py_RETURN_NONE;
}

/*
 *------------------------------------------------------
 *               sidex_Create
 *------------------------------------------------------
 */
static PyObject * _sidex_Create(PyObject *self, PyObject *args)
{
    PyObject*    py_pname16;
    SIDEX_TCHAR* pname;
    PyObject*    py_pname;
    SIDEX_HANDLE  shandle;
    
    SIDEX_INT32   err = SIDEX_SUCCESS;

    if (!PyArg_ParseTuple(args, "O", &py_pname))
        return NULL;
    if ( _unicodeConversion(py_pname, &pname, &py_pname16, sidexError) ) return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = sidex_Create(pname, &shandle);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    unicodeFree(py_pname16, pname);

    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Create failed (%ld)",err);
            return NULL;
    }

    return Py_BuildValue("L",shandle);
}

/*
 *------------------------------------------------------
 *               sidex_Set_DocumentName
 *------------------------------------------------------
 */
static PyObject * _sidex_Set_DocumentName(PyObject *self, PyObject *args)
{
    PyObject*    py_pname16;
    SIDEX_TCHAR* pname;
    PyObject*    py_pname;
    SIDEX_HANDLE  shandle;

    SIDEX_INT32   err = SIDEX_SUCCESS;

    if (!PyArg_ParseTuple(args, "LO", &shandle, &py_pname))
        return NULL;
    if ( _unicodeConversion(py_pname, &pname, &py_pname16, sidexError) ) return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = sidex_Set_DocumentName(shandle, pname);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    unicodeFree(py_pname16, pname);

    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Set_DocumentName failed (%ld)",err);
            return NULL;
    }

    Py_RETURN_NONE;
}

/*
 *------------------------------------------------------
 *               sidex_Get_DocumentName
 *------------------------------------------------------
 */
static PyObject * _sidex_Get_DocumentName(PyObject *self, PyObject *args)
{
    SIDEX_TCHAR * pname;
    SIDEX_HANDLE  shandle;

    SIDEX_INT32   err = SIDEX_SUCCESS;

    if (!PyArg_ParseTuple(args, "L", &shandle))
        return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = sidex_Get_DocumentName(shandle, &pname);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Get_DocumentName failed (%ld)",err);
            return NULL;
    }

    PyObject* pRetVal = utf32_utf16_decode(pname, -1);

    return pRetVal;
}

/*
 *------------------------------------------------------
 *               sidex_Clear
 *------------------------------------------------------
 */
static PyObject * _sidex_Clear(PyObject *self, PyObject *args)
{
    SIDEX_HANDLE  shandle;

    SIDEX_INT32   err = SIDEX_SUCCESS;

    if (!PyArg_ParseTuple(args, "L", &shandle))
        return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        sidex_Clear(shandle);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Clear failed (%ld)",err);
            return NULL;
    }

    Py_RETURN_NONE;
}

/*
 *------------------------------------------------------
 *               sidex_Free
 *------------------------------------------------------
 */
static PyObject * _sidex_Free(PyObject *self, PyObject *args)
{
    SIDEX_HANDLE  shandle;

    SIDEX_INT32   err = SIDEX_SUCCESS;

    if (!PyArg_ParseTuple(args, "L", &shandle))
        return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        sidex_Free(&shandle);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Free failed (%ld)",err);
            return NULL;
    }

    Py_RETURN_NONE;
}

/*
 *------------------------------------------------------
 *               sidex_Load_Content
 *------------------------------------------------------
 */
static PyObject * _sidex_Load_Content(PyObject *self, PyObject *args)
{
    PyObject*    py_pname16;
    SIDEX_TCHAR* pname;
    PyObject*    py_pname;
    SIDEX_HANDLE shandle;

    SIDEX_INT32   err = SIDEX_SUCCESS;

    if (!PyArg_ParseTuple(args, "LO", &shandle, &py_pname))
        return NULL;
    if ( _unicodeConversion(py_pname, &pname, &py_pname16, sidexError) ) return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = sidex_Load_Content(shandle, pname);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    unicodeFree(py_pname16, pname);

    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Load_Content failed (%ld)",err);
            return NULL;
    }

    Py_RETURN_NONE;
}

/*
 *------------------------------------------------------
 *               sidex_Set_Content
 *------------------------------------------------------
 */
static PyObject * _sidex_Set_Content(PyObject *self, PyObject *args)
{
    PyObject*    py_pcontent16;
    SIDEX_TCHAR* pcontent;
    PyObject*    py_pcontent;
    SIDEX_HANDLE shandle;

    SIDEX_INT32   err = SIDEX_SUCCESS;

    if (!PyArg_ParseTuple(args, "LO", &shandle, &py_pcontent))
        return NULL;
    if ( _unicodeConversion(py_pcontent, &pcontent, &py_pcontent16, sidexError) ) return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = sidex_Set_Content(shandle, pcontent);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    unicodeFree(py_pcontent16, pcontent);

    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Set_Content failed (%ld)",err);
            return NULL;
    }

    Py_RETURN_NONE;
}

/*
 *------------------------------------------------------
 *               sidex_Get_Content
 *------------------------------------------------------
 */
static PyObject * _sidex_Get_Content(PyObject *self, PyObject *args)
{
    SIDEX_HANDLE  shandle;
    SIDEX_TCHAR * content;
    SIDEX_INT32   clength;
    PyObject *    result;

    SIDEX_INT32   err = SIDEX_SUCCESS;

    if (!PyArg_ParseTuple(args, "L", &shandle))
        return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = sidex_Get_Content(shandle, &content, &clength);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Get_Content failed (%ld)",err);
            return NULL;
    }

    PyObject* pRetVal = utf32_utf16_decode(content, clength);

    result = Py_BuildValue("O#",pRetVal, clength);
    Py_XDECREF(pRetVal);
    sidex_Free_Content(content);

    return result;
}

/*
 *------------------------------------------------------
 *               sidex_Get_Content_Length
 *------------------------------------------------------
 */
static PyObject * _sidex_Get_Content_Length(PyObject *self, PyObject *args)
{
    SIDEX_HANDLE  shandle;
    SIDEX_INT32   clength;
    PyObject *    result;

    SIDEX_INT32   err = SIDEX_SUCCESS;

    if (!PyArg_ParseTuple(args, "L", &shandle))
        return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = sidex_Get_Content_Length(shandle, &clength);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Get_Content_Length failed (%ld)",err);
            return NULL;
    }

    result = Py_BuildValue("l",clength);
    return result;
}

/*
 *------------------------------------------------------
 *               sidex_Save_Content
 *------------------------------------------------------
 */
static PyObject * _sidex_Save_Content(PyObject *self, PyObject *args)
{
    SIDEX_HANDLE  shandle;
    PyObject*    py_filename16;
    SIDEX_TCHAR* filename;
    PyObject*    py_filename;

    SIDEX_INT32   err = SIDEX_SUCCESS;

    if (!PyArg_ParseTuple(args, "LO", &shandle, &py_filename))
        return NULL;
    if ( _unicodeConversion(py_filename, &filename, &py_filename16, sidexError) ) return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = sidex_Save_Content(shandle, filename);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    unicodeFree(py_filename16, filename);

    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Save_Content failed (%ld)",err);
            return NULL;
    }

    Py_RETURN_NONE;
}

/*
 *------------------------------------------------------
 *               sidex_None_Write
 *------------------------------------------------------
 */
static PyObject * _sidex_None_Write(PyObject *self, PyObject *args)
{
    SIDEX_HANDLE  shandle;
    PyObject*    py_ngroup16;
    PyObject*    py_nkey16;
    SIDEX_TCHAR* ngroup;
    SIDEX_TCHAR* nkey;
    PyObject*    py_ngroup;
    PyObject*    py_nkey;

    SIDEX_INT32   err = SIDEX_SUCCESS;

    if (!PyArg_ParseTuple(args, "LOO", &shandle, &py_ngroup, &py_nkey))
        return NULL;
    if ( _unicodeConversion(py_ngroup, &ngroup, &py_ngroup16, sidexError) ) return NULL;
    if ( _unicodeConversion(py_nkey, &nkey, &py_nkey16, sidexError) ) return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = sidex_None_Write(shandle, ngroup, nkey);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    unicodeFree(py_ngroup16, ngroup);
    unicodeFree(py_nkey16, nkey);

    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_None_Write failed (%ld)",err);
            return NULL;
    }

    Py_RETURN_NONE;
}

/*
 *------------------------------------------------------
 *               sidex_Boolean_Write
 *------------------------------------------------------
 */
static PyObject * _sidex_Boolean_Write(PyObject *self, PyObject *args)
{
    SIDEX_HANDLE shandle;
    PyObject*    py_ngroup16;
    PyObject*    py_nkey16;
    SIDEX_TCHAR* ngroup;
    SIDEX_TCHAR* nkey;
    PyObject*    py_ngroup;
    PyObject*    py_nkey;
    PyObject*    val;

    SIDEX_INT32   err = SIDEX_SUCCESS;

    if (!PyArg_ParseTuple(args, "LOOO", &shandle, &py_ngroup, &py_nkey, &val))
        return NULL;
    if ( _unicodeConversion(py_ngroup, &ngroup, &py_ngroup16, sidexError) ) return NULL;
    if ( _unicodeConversion(py_nkey, &nkey, &py_nkey16, sidexError) ) return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = sidex_Boolean_Write(shandle, ngroup, nkey, PyObject_IsTrue(val));
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    unicodeFree(py_ngroup16, ngroup);
    unicodeFree(py_nkey16, nkey);

    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Boolean_Write failed (%ld)",err);
            return NULL;
    }

    Py_RETURN_NONE;
}

/*
 *------------------------------------------------------
 *               sidex_Boolean_Read
 *------------------------------------------------------
 */
static PyObject * _sidex_Boolean_Read(PyObject *self, PyObject *args)
{
    SIDEX_HANDLE shandle;
    PyObject*    py_ngroup16;
    PyObject*    py_nkey16;
    SIDEX_TCHAR* ngroup;
    SIDEX_TCHAR* nkey;
    PyObject*    py_ngroup;
    PyObject*    py_nkey;

    SIDEX_BOOL    rval;

    SIDEX_INT32   err = SIDEX_SUCCESS;

    if (!PyArg_ParseTuple(args, "LOO", &shandle, &py_ngroup, &py_nkey))
        return NULL;
    if ( _unicodeConversion(py_ngroup, &ngroup, &py_ngroup16, sidexError) ) return NULL;
    if ( _unicodeConversion(py_nkey, &nkey, &py_nkey16, sidexError) ) return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = sidex_Boolean_Read(shandle, ngroup, nkey, &rval);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    unicodeFree(py_ngroup16, ngroup);
    unicodeFree(py_nkey16, nkey);

    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Boolean_Read failed (%ld)",err);
            return NULL;
    }

    if (SIDEX_TRUE == rval)
        Py_RETURN_TRUE;
    else
        Py_RETURN_FALSE;
}

/*
 *------------------------------------------------------
 *               sidex_Integer_Write
 *------------------------------------------------------
 */
static PyObject * _sidex_Integer_Write(PyObject *self, PyObject *args)
{
    SIDEX_HANDLE shandle;
    PyObject*    py_ngroup16;
    PyObject*    py_nkey16;
    SIDEX_TCHAR* ngroup;
    SIDEX_TCHAR* nkey;
    PyObject*    py_ngroup;
    PyObject*    py_nkey;
    SIDEX_INT64  val;

    SIDEX_INT32   err = SIDEX_SUCCESS;

    if (!PyArg_ParseTuple(args, "LOOL", &shandle, &py_ngroup, &py_nkey, &val))
        return NULL;
    if ( _unicodeConversion(py_ngroup, &ngroup, &py_ngroup16, sidexError) ) return NULL;
    if ( _unicodeConversion(py_nkey, &nkey, &py_nkey16, sidexError) ) return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = sidex_Integer_Write(shandle, ngroup, nkey, val);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    unicodeFree(py_ngroup16, ngroup);
    unicodeFree(py_nkey16, nkey);

    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Integer_Write failed (%ld)",err);
            return NULL;
    }

    Py_RETURN_NONE;
}

/*
 *------------------------------------------------------
 *               sidex_Integer_Read
 *------------------------------------------------------
 */
static PyObject * _sidex_Integer_Read(PyObject *self, PyObject *args)
{
    SIDEX_HANDLE shandle;
    PyObject*    py_ngroup16;
    PyObject*    py_nkey16;
    SIDEX_TCHAR* ngroup;
    SIDEX_TCHAR* nkey;
    PyObject*    py_ngroup;
    PyObject*    py_nkey;

    SIDEX_INT64   rval;

    SIDEX_INT32   err = SIDEX_SUCCESS;

    if (!PyArg_ParseTuple(args, "LOO", &shandle, &py_ngroup, &py_nkey))
        return NULL;
    if ( _unicodeConversion(py_ngroup, &ngroup, &py_ngroup16, sidexError) ) return NULL;
    if ( _unicodeConversion(py_nkey, &nkey, &py_nkey16, sidexError) ) return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = sidex_Integer_Read(shandle, ngroup, nkey, &rval);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    unicodeFree(py_ngroup16, ngroup);
    unicodeFree(py_nkey16, nkey);

    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Integer_Read failed (%ld)",err);
            return NULL;
    }

    return Py_BuildValue("L",rval);
}

/*
 *------------------------------------------------------
 *               sidex_Float_Write
 *------------------------------------------------------
 */
static PyObject * _sidex_Float_Write(PyObject *self, PyObject *args)
{
    SIDEX_HANDLE shandle;
    PyObject*    py_ngroup16;
    PyObject*    py_nkey16;
    SIDEX_TCHAR* ngroup;
    SIDEX_TCHAR* nkey;
    PyObject*    py_ngroup;
    PyObject*    py_nkey;
    SIDEX_DOUBLE val;

    SIDEX_INT32   err = SIDEX_SUCCESS;

    if (!PyArg_ParseTuple(args, "LOOd", &shandle, &py_ngroup, &py_nkey, &val))
        return NULL;
    if ( _unicodeConversion(py_ngroup, &ngroup, &py_ngroup16, sidexError) ) return NULL;
    if ( _unicodeConversion(py_nkey, &nkey, &py_nkey16, sidexError) ) return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = sidex_Float_Write(shandle, ngroup, nkey, val);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    unicodeFree(py_ngroup16, ngroup);
    unicodeFree(py_nkey16, nkey);

    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Float_Write failed (%ld)",err);
            return NULL;
    }

    Py_RETURN_NONE;
}

/*
 *------------------------------------------------------
 *               sidex_Float_Read
 *------------------------------------------------------
 */
static PyObject * _sidex_Float_Read(PyObject *self, PyObject *args)
{
    SIDEX_HANDLE shandle;
    PyObject*    py_ngroup16;
    PyObject*    py_nkey16;
    SIDEX_TCHAR* ngroup;
    SIDEX_TCHAR* nkey;
    PyObject*    py_ngroup;
    PyObject*    py_nkey;

    SIDEX_DOUBLE  rval;

    SIDEX_INT32   err = SIDEX_SUCCESS;

    if (!PyArg_ParseTuple(args, "LOO", &shandle, &py_ngroup, &py_nkey))
        return NULL;
    if ( _unicodeConversion(py_ngroup, &ngroup, &py_ngroup16, sidexError) ) return NULL;
    if ( _unicodeConversion(py_nkey, &nkey, &py_nkey16, sidexError) ) return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = sidex_Float_Read(shandle, ngroup, nkey, &rval);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    unicodeFree(py_ngroup16, ngroup);
    unicodeFree(py_nkey16, nkey);

    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Float_Read failed (%ld)",err);
            return NULL;
    }

    return Py_BuildValue("d",rval);
}

/*
 *------------------------------------------------------
 *               sidex_Variant_Write
 *------------------------------------------------------
 */
static PyObject * _sidex_Variant_Write(PyObject *self, PyObject *args)
{
    SIDEX_HANDLE  shandle;
    PyObject*     py_ngroup16;
    PyObject*     py_nkey16;
    SIDEX_TCHAR*  ngroup;
    SIDEX_TCHAR*  nkey;
    PyObject*     py_ngroup;
    PyObject*     py_nkey;
    SIDEX_VARIANT val;

    SIDEX_INT32   err = SIDEX_SUCCESS;

    if (!PyArg_ParseTuple(args, "LOOL", &shandle, &py_ngroup, &py_nkey, &val))
        return NULL;
    if ( _unicodeConversion(py_ngroup, &ngroup, &py_ngroup16, sidexError) ) return NULL;
    if ( _unicodeConversion(py_nkey, &nkey, &py_nkey16, sidexError) ) return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = sidex_Variant_Write(shandle, ngroup, nkey, val);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    unicodeFree(py_ngroup16, ngroup);
    unicodeFree(py_nkey16, nkey);

    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Variant_Write failed (%ld)",err);
            return NULL;
    }

    Py_RETURN_NONE;
}

/*
 *------------------------------------------------------
 *               sidex_Variant_Read
 *------------------------------------------------------
 */
static PyObject * _sidex_Variant_Read(PyObject *self, PyObject *args)
{
    SIDEX_HANDLE  shandle;
    PyObject*     py_ngroup16;
    PyObject*     py_nkey16;
    SIDEX_TCHAR*  ngroup;
    SIDEX_TCHAR*  nkey;
    PyObject*     py_ngroup;
    PyObject*     py_nkey;

    SIDEX_VARIANT  rval;

    SIDEX_INT32   err = SIDEX_SUCCESS;

    if (!PyArg_ParseTuple(args, "LOO", &shandle, &py_ngroup, &py_nkey))
        return NULL;
    if ( _unicodeConversion(py_ngroup, &ngroup, &py_ngroup16, sidexError) ) return NULL;
    if ( _unicodeConversion(py_nkey, &nkey, &py_nkey16, sidexError) ) return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = sidex_Variant_Read(shandle, ngroup, nkey, &rval);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    unicodeFree(py_ngroup16, ngroup);
    unicodeFree(py_nkey16, nkey);

    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Variant_Read failed (%ld)",err);
            return NULL;
    }

    return Py_BuildValue("L",rval);
}

/*
 *------------------------------------------------------
 *               sidex_Variant_DecRef
 *------------------------------------------------------
 */
static PyObject * _sidex_Variant_DecRef(PyObject *self, PyObject *args)
{
    SIDEX_VARIANT svariant;

    SIDEX_INT32   err = SIDEX_SUCCESS;

    if (!PyArg_ParseTuple(args, "L", &svariant))
        return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        sidex_Variant_DecRef(svariant);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Variant_DecRef failed (%ld)",err);
            return NULL;
    }

    Py_RETURN_NONE;
}

/*
 *------------------------------------------------------
 *               sidex_Variant_IncRef
 *------------------------------------------------------
 */
static PyObject * _sidex_Variant_IncRef(PyObject *self, PyObject *args)
{
    SIDEX_VARIANT svariant;

    SIDEX_INT32   err = SIDEX_SUCCESS;

    if (!PyArg_ParseTuple(args, "L", &svariant))
        return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        sidex_Variant_IncRef(svariant);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Variant_IncRef failed (%ld)",err);
            return NULL;
    }

    Py_RETURN_NONE;
}

/*
 *------------------------------------------------------
 *               sidex_Variant_New_None
 *------------------------------------------------------
 */
static PyObject * _sidex_Variant_New_None(PyObject *self)
{
    SIDEX_INT32   err;

    SIDEX_VARIANT svariant = _sidex_Variant_New_NoneS(&err);
    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Variant_New_None failed (%ld)",err);
            return NULL;
    }

    return Py_BuildValue("L",svariant);
}

/*
 *------------------------------------------------------
 *               sidex_Variant_New_Boolean
 *------------------------------------------------------
 */
static PyObject * _sidex_Variant_New_Boolean(PyObject *self, PyObject *args)
{
    SIDEX_INT32   err;

    SIDEX_VARIANT svariant = _sidex_Variant_New_BooleanS(args, &err);
    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Variant_New_Boolean failed (%ld)",err);
            return NULL;
    }

    return Py_BuildValue("L",svariant);
}

/*
 *------------------------------------------------------
 *               sidex_Variant_New_Integer
 *------------------------------------------------------
 */
static PyObject * _sidex_Variant_New_Integer(PyObject *self, PyObject *args)
{
    SIDEX_INT32   err;

    SIDEX_VARIANT svariant = _sidex_Variant_New_IntegerS(args, &err);
    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Variant_New_Integer failed (%ld)",err);
            return NULL;
    }

    return Py_BuildValue("L",svariant);
}

/*
 *------------------------------------------------------
 *               sidex_Variant_New_Float
 *------------------------------------------------------
 */
static PyObject * _sidex_Variant_New_Float(PyObject *self, PyObject *args)
{
    SIDEX_INT32   err;

    SIDEX_VARIANT svariant = _sidex_Variant_New_FloatS(args, &err);
    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Variant_New_Float failed (%ld)",err);
            return NULL;
    }
    return Py_BuildValue("L",svariant);
}

/*
 *------------------------------------------------------
 *               sidex_Variant_New_DateTime
 *------------------------------------------------------
 */
static PyObject * _sidex_Variant_New_DateTime(PyObject *self, PyObject *args)
{
    SIDEX_INT32   err = SIDEX_SUCCESS;

    SIDEX_VARIANT svariant = _sidex_Variant_New_DateTimeS(args, &err, sidexError);
    
    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Variant_New_DateTime failed (%ld)",err);
            return NULL;
    }
    if (SIDEX_HANDLE_TYPE_NULL == svariant) {
            // Seems to be no Unicode / String parameter
            return NULL;
    }

    return Py_BuildValue("L",svariant);
}

/*
 *------------------------------------------------------
 *               sidex_Variant_New_String
 *------------------------------------------------------
 */
static PyObject * _sidex_Variant_New_String(PyObject *self, PyObject *args)
{
    SIDEX_INT32   err;
    SIDEX_VARIANT svariant;

    //DebugBreak();
#if PY_MAJOR_VERSION < 3 // On Python 3 we only have Unicode:
          PyObject* value;
          ////////////////////////////////////////////
          // Find out String or Unicode- String
          if (!PyArg_ParseTuple(args, "O", &value)){
            return NULL;
          }
          if (true == PyString_Check(value)){
            svariant = _sidex_Variant_New_StringS(args, &err);
          }
          else
#endif
          {
            svariant = _sidex_Variant_New_UnicodeStringS(args, &err, sidexError);
          }


    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Variant_New_String failed (%ld)",err);
            return NULL;
    }
    if (SIDEX_HANDLE_TYPE_NULL == svariant) {
            // Seems to be no Unicode / String parameter
            return NULL;
    }

    return Py_BuildValue("L",svariant);
}

/*
 *------------------------------------------------------
 *               sidex_Variant_New_Binary
 *------------------------------------------------------
 */
static PyObject * _sidex_Variant_New_Binary(PyObject *self, PyObject *args)
{
    SIDEX_INT32   err = SIDEX_SUCCESS;

    SIDEX_VARIANT svariant = _sidex_Variant_New_BinaryS(args, &err);
    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Variant_New_Binary failed (%ld)",err);
            return NULL;
    }

    return Py_BuildValue("L",svariant);
}

/*
 *------------------------------------------------------
 *               sidex_Variant_New_List
 *------------------------------------------------------
 */
static PyObject * _sidex_Variant_New_List(PyObject *self)
{
    SIDEX_INT32   err;

    SIDEX_VARIANT svariant = _sidex_Variant_New_ListS(&err);
    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Variant_New_List failed (%ld)",err);
            return NULL;
    }

    return Py_BuildValue("L",svariant);
}

/*
 *------------------------------------------------------
 *               sidex_Variant_New_Dict
 *------------------------------------------------------
 */
static PyObject * _sidex_Variant_New_Dict(PyObject *self)
{
    SIDEX_INT32   err;

    SIDEX_VARIANT svariant = _sidex_Variant_New_DictS(&err);

    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Variant_New_Dict failed (%ld)",err);
            return NULL;
    }

    return Py_BuildValue("L",svariant);
}

/*
 *------------------------------------------------------
 *               sidex_Variant_New_Table
 *------------------------------------------------------
 */
static PyObject * _sidex_Variant_New_Table(PyObject *self)
{
    SIDEX_VARIANT svariant;

    SIDEX_INT32   err = SIDEX_SUCCESS;

    Py_BEGIN_ALLOW_THREADS;
    try {
        svariant = sidex_Variant_New_Table();
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Variant_New_Table failed (%ld)",err);
            return NULL;
    }

    return Py_BuildValue("L",svariant);
}

/*
 *------------------------------------------------------
 *               sidex_Variant_New_DictBySize
 *------------------------------------------------------
 */
static PyObject * _sidex_Variant_New_DictBySize(PyObject *self, PyObject *args)
{
    SIDEX_VARIANT svariant;
    SIDEX_INT32   pvalue;

    SIDEX_INT32   err = SIDEX_SUCCESS;

    if (!PyArg_ParseTuple(args, "L", &pvalue))
        return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        svariant = sidex_Variant_New_DictBySize(pvalue);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Variant_New_DictBySize failed (%ld)",err);
            return NULL;
    }

    return Py_BuildValue("L",svariant);
}

/*
 *------------------------------------------------------
 *               sidex_Variant_List_Check
 *------------------------------------------------------
 */
static PyObject * _sidex_Variant_List_Check(PyObject *self, PyObject *args)
{
    SIDEX_VARIANT  vhandle;
    SIDEX_BOOL     sresult;

    SIDEX_INT32    err = SIDEX_SUCCESS;

    if (!PyArg_ParseTuple(args, "L", &vhandle))
        return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        sresult = sidex_Variant_List_Check(vhandle);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Variant_List_Check failed (%ld)",err);
            return NULL;
    }

    if (SIDEX_TRUE == sresult)
        Py_RETURN_TRUE;
    else
        Py_RETURN_FALSE;

}

/*
 *------------------------------------------------------
 *               sidex_Variant_List_Size
 *------------------------------------------------------
 */
static PyObject * _sidex_Variant_List_Size(PyObject *self, PyObject *args)
{
    SIDEX_VARIANT svariant;
    SIDEX_INT32   plength;

    SIDEX_INT32    err = SIDEX_SUCCESS;

    if (!PyArg_ParseTuple(args, "L", &svariant))
        return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = sidex_Variant_List_Size(svariant, &plength);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Variant_List_Size failed (%ld)",err);
            return NULL;
    }

    return Py_BuildValue("l",plength);
}

/*
 *------------------------------------------------------
 *               sidex_Variant_List_Append
 *------------------------------------------------------
 */
static PyObject * _sidex_Variant_List_Append(PyObject *self, PyObject *args)
{
    SIDEX_VARIANT svariant;
    SIDEX_VARIANT slist;
    SIDEX_INT32   ppos;

    SIDEX_INT32    err = SIDEX_SUCCESS;

    if (!PyArg_ParseTuple(args, "LL", &slist, &svariant))
        return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = sidex_Variant_List_Append(slist, svariant, &ppos);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Variant_List_Append failed (%ld)",err);
            return NULL;
    }

    return Py_BuildValue("l",ppos);
}

/*
 *------------------------------------------------------
 *               sidex_Variant_List_Insert
 *------------------------------------------------------
 */
static PyObject * _sidex_Variant_List_Insert(PyObject *self, PyObject *args)
{
    SIDEX_VARIANT svariant;
    SIDEX_VARIANT slist;
    SIDEX_INT32   ppos;
    SIDEX_INT64   iTemp64; // Umgehung von Speicherzugriffsverletzung bei Python 3.4 unter LINUX x64

    SIDEX_INT32    err = SIDEX_SUCCESS;

    if (!PyArg_ParseTuple(args, "LLl", &slist, &svariant, &iTemp64))
        return NULL;
    ppos = (SIDEX_INT32)iTemp64;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = sidex_Variant_List_Insert(slist, svariant, ppos);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Variant_List_Insert failed (%ld)",err);
            return NULL;
    }

    Py_RETURN_NONE;
}

/*
 *------------------------------------------------------
 *               sidex_Variant_List_Set
 *------------------------------------------------------
 */
static PyObject * _sidex_Variant_List_Set(PyObject *self, PyObject *args)
{
    SIDEX_VARIANT svariant;
    SIDEX_VARIANT slist;
    SIDEX_INT32   ppos;
    SIDEX_INT64   iTemp64; // Umgehung von Speicherzugriffsverletzung bei Python 3.4 unter LINUX x64

    SIDEX_INT32    err = SIDEX_SUCCESS;

    if (!PyArg_ParseTuple(args, "LLl", &slist, &svariant, &iTemp64))
        return NULL;
    ppos = (SIDEX_INT32)iTemp64;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = sidex_Variant_List_Set(slist, svariant, ppos);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Variant_List_Set failed (%ld)",err);
            return NULL;
    }

    Py_RETURN_NONE;
}

/*
 *------------------------------------------------------
 *               sidex_Variant_List_DeleteItem
 *------------------------------------------------------
 */
static PyObject * _sidex_Variant_List_DeleteItem(PyObject *self, PyObject *args)
{
    SIDEX_VARIANT slist;
    SIDEX_INT32   ppos;
    SIDEX_INT64   iTemp64; // Umgehung von Speicherzugriffsverletzung bei Python 3.4 unter LINUX x64

    SIDEX_INT32    err = SIDEX_SUCCESS;

    if (!PyArg_ParseTuple(args, "Ll", &slist, &iTemp64))
        return NULL;
    ppos = (SIDEX_INT32)iTemp64;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = sidex_Variant_List_DeleteItem(slist, ppos);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Variant_List_DeleteItem failed (%ld)",err);
            return NULL;
    }

    Py_RETURN_NONE;
}

/*
 *------------------------------------------------------
 *               sidex_Variant_List_Clear
 *------------------------------------------------------
 */
static PyObject * _sidex_Variant_List_Clear(PyObject *self, PyObject *args)
{
    SIDEX_VARIANT slist;

    SIDEX_INT32    err = SIDEX_SUCCESS;

    if (!PyArg_ParseTuple(args, "L", &slist))
        return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = sidex_Variant_List_Clear(slist);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Variant_List_Clear failed (%ld)",err);
            return NULL;
    }

    Py_RETURN_NONE;
}

/*
 *------------------------------------------------------
 *               sidex_Variant_List_Get
 *------------------------------------------------------
 */
static PyObject * _sidex_Variant_List_Get(PyObject *self, PyObject *args)
{
    SIDEX_VARIANT slist;
    SIDEX_VARIANT svalue;
    SIDEX_INT32   ppos;
    SIDEX_INT64   iTemp64; // Umgehung von Speicherzugriffsverletzung bei Python 3.4 unter LINUX x64

    SIDEX_INT32    err = SIDEX_SUCCESS;

    if (!PyArg_ParseTuple(args, "Ll", &slist, &iTemp64))
        return NULL;
    ppos = (SIDEX_INT32)iTemp64;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = sidex_Variant_List_Get(slist, ppos, &svalue);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != SIDEX_SUCCESS) {
        switch(err) {
        case SIDEX_ERR_NOCONTENT:
                    PyErr_Format(sidexError, "sidex_Variant_List_Get failed (%ld)",err);
                    return NULL;
        default:	
                    PyErr_Format(sidexError, "sidex_Variant_List_Get failed (%ld)",err);
                    return NULL;
        }
    }

    return Py_BuildValue("L",svalue);
}

/*
 *------------------------------------------------------
 *               sidex_Variant_Dict_Check
 *------------------------------------------------------
 */
static PyObject * _sidex_Variant_Dict_Check(PyObject *self, PyObject *args)
{
    SIDEX_VARIANT  vhandle;
    SIDEX_BOOL     sresult;

    SIDEX_INT32    err = SIDEX_SUCCESS;

    if (!PyArg_ParseTuple(args, "L", &vhandle))
        return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        sresult = sidex_Variant_Dict_Check(vhandle);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Variant_Dict_Check failed (%ld)",err);
            return NULL;
    }

    if (SIDEX_TRUE == sresult)
        Py_RETURN_TRUE;
    else
        Py_RETURN_FALSE;

}

/*
 *------------------------------------------------------
 *               sidex_Variant_Dict_Set
 *------------------------------------------------------
 */
static PyObject * _sidex_Variant_Dict_Set(PyObject *self, PyObject *args)
{
    SIDEX_VARIANT sdict;
    SIDEX_VARIANT svalue;
    PyObject*     py_key16;
    SIDEX_TCHAR*  pkey;
    PyObject*     py_key;

    SIDEX_INT32    err = SIDEX_SUCCESS;

    if (!PyArg_ParseTuple(args, "LOL", &sdict, &py_key, &svalue))
        return NULL;
    if ( _unicodeConversion(py_key, &pkey, &py_key16, sidexError) ) return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = sidex_Variant_Dict_Set(sdict, pkey, svalue);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    unicodeFree(py_key16, pkey);

    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Variant_Dict_Set failed (%ld)",err);
            return NULL;
    }

    Py_RETURN_NONE;
}

/*
 *------------------------------------------------------
 *               sidex_GetGroups
 *------------------------------------------------------
 */
static PyObject * _sidex_GetGroups(PyObject *self, PyObject *args)
{
    SIDEX_VARIANT svalue;
    SIDEX_HANDLE  shandle;

    SIDEX_INT32    err = SIDEX_SUCCESS;

    if (!PyArg_ParseTuple(args, "L", &shandle))
        return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = sidex_GetGroups(shandle, &svalue);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_GetGroups failed (%ld)",err);
            return NULL;
    }

    return Py_BuildValue("L",svalue);
}

/*
 *------------------------------------------------------
 *               sidex_HasGroup
 *------------------------------------------------------
 */
static PyObject * _sidex_HasGroup(PyObject *self, PyObject *args)
{
    SIDEX_BOOL    svalue;
    SIDEX_HANDLE  shandle;
    PyObject*     py_group16;
    SIDEX_TCHAR*  pgroup;
    PyObject*     py_pgroup;

    SIDEX_INT32    err = SIDEX_SUCCESS;

    if (!PyArg_ParseTuple(args, "LO", &shandle, &py_pgroup))
        return NULL;
    if ( _unicodeConversion(py_pgroup, &pgroup, &py_group16, sidexError) ) return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        svalue = sidex_HasGroup(shandle, pgroup);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    unicodeFree(py_group16, pgroup);

    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_HasGroup failed (%ld)",err);
            return NULL;
    }

    if (SIDEX_TRUE == svalue)
        Py_RETURN_TRUE;
    else
        Py_RETURN_FALSE;
}

/*
 *------------------------------------------------------
 *               sidex_GetKeys
 *------------------------------------------------------
 */
static PyObject * _sidex_GetKeys(PyObject *self, PyObject *args)
{
    SIDEX_VARIANT svalue;
    SIDEX_HANDLE  shandle;
    PyObject*     py_group16;
    SIDEX_TCHAR*  pgroup;
    PyObject*     py_group;

    SIDEX_INT32    err = SIDEX_SUCCESS;

    if (!PyArg_ParseTuple(args, "LO", &shandle, &py_group))
        return NULL;
    if ( _unicodeConversion(py_group, &pgroup, &py_group16, sidexError) ) return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = sidex_GetKeys(shandle, pgroup, &svalue);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    unicodeFree(py_group16, pgroup);

    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_GetKeys failed (%ld)",err);
            return NULL;
    }

    return Py_BuildValue("L",svalue);
}

/*
 *------------------------------------------------------
 *               sidex_HasKey
 *------------------------------------------------------
 */
static PyObject * _sidex_HasKey(PyObject *self, PyObject *args)
{
    SIDEX_BOOL    svalue;
    SIDEX_HANDLE  shandle;
    PyObject*     py_group16;
    SIDEX_TCHAR*  pgroup;
    PyObject*     py_group;
    PyObject*     py_key16;
    SIDEX_TCHAR*  pkey;
    PyObject*     py_key;

    SIDEX_INT32    err = SIDEX_SUCCESS;

    if (!PyArg_ParseTuple(args, "LOO", &shandle, &py_group, &py_key))
        return NULL;
    if ( _unicodeConversion(py_group, &pgroup, &py_group16, sidexError) ) return NULL;
    if ( _unicodeConversion(py_key, &pkey, &py_key16, sidexError) ) return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        svalue = sidex_HasKey(shandle, pgroup, pkey);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    unicodeFree(py_group16, pgroup);
    unicodeFree(py_key16, pkey);

    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_HasKey failed (%ld)",err);
            return NULL;
    }

    if (SIDEX_TRUE == svalue)
        Py_RETURN_TRUE;
    else
        Py_RETURN_FALSE;
}

/*
 *------------------------------------------------------
 *               sidex_DeleteGroup
 *------------------------------------------------------
 */
static PyObject * _sidex_DeleteGroup(PyObject *self, PyObject *args)
{
    SIDEX_HANDLE shandle;
    PyObject*    py_group16;
    SIDEX_TCHAR* pgroup;
    PyObject*    py_group;

    SIDEX_INT32    err = SIDEX_SUCCESS;

    if (!PyArg_ParseTuple(args, "LO", &shandle, &py_group))
        return NULL;
    if ( _unicodeConversion(py_group, &pgroup, &py_group16, sidexError) ) return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = sidex_DeleteGroup(shandle, pgroup);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    unicodeFree(py_group16, pgroup);

    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_DeleteGroup failed (%ld)",err);
            return NULL;
    }

    Py_RETURN_NONE;
}

/*
 *------------------------------------------------------
 *               sidex_DeleteKey
 *------------------------------------------------------
 */
static PyObject * _sidex_DeleteKey(PyObject *self, PyObject *args)
{
    SIDEX_HANDLE  shandle;
    PyObject*     py_group16;
    SIDEX_TCHAR*  pgroup;
    PyObject*     py_group;
    PyObject*     py_key16;
    SIDEX_TCHAR*  pkey;
    PyObject*     py_key;

    SIDEX_INT32    err = SIDEX_SUCCESS;

    if (!PyArg_ParseTuple(args, "LOO", &shandle, &py_group, &py_key))
        return NULL;
    if ( _unicodeConversion(py_group, &pgroup, &py_group16, sidexError) ) return NULL;
    if ( _unicodeConversion(py_key, &pkey, &py_key16, sidexError) ) return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = sidex_DeleteKey(shandle, pgroup, pkey);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    unicodeFree(py_group16, pgroup);
    unicodeFree(py_key16, pkey);

    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_DeleteKey failed (%ld)",err);
            return NULL;
    }

    Py_RETURN_NONE;
}

/*
 *------------------------------------------------------
 *               sidex_Variant_Dict_Size
 *------------------------------------------------------
 */
static PyObject * _sidex_Variant_Dict_Size(PyObject *self, PyObject *args)
{
    SIDEX_VARIANT svalue;
    SIDEX_INT32   ssize;

    SIDEX_INT32    err = SIDEX_SUCCESS;

    if (!PyArg_ParseTuple(args, "L", &svalue))
        return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = sidex_Variant_Dict_Size(svalue, &ssize);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Variant_Dict_Size failed (%ld)",err);
            return NULL;
    }

    return Py_BuildValue("l",ssize);
}

/*
 *------------------------------------------------------
 *               sidex_Variant_Dict_Keys
 *------------------------------------------------------
 */
static PyObject * _sidex_Variant_Dict_Keys(PyObject *self, PyObject *args)
{
    SIDEX_VARIANT svalue;
    SIDEX_VARIANT skeys;

    SIDEX_INT32    err = SIDEX_SUCCESS;

    if (!PyArg_ParseTuple(args, "L", &svalue))
        return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = sidex_Variant_Dict_Keys(svalue, &skeys);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Variant_Dict_Keys failed (%ld)",err);
            return NULL;
    }

    return Py_BuildValue("L",skeys);
}

/*
 *------------------------------------------------------
 *               sidex_Variant_Dict_First
 *------------------------------------------------------
 */
static PyObject * _sidex_Variant_Dict_First(PyObject *self, PyObject *args)
{
    SIDEX_VARIANT svalue;

    SIDEX_INT32    err = SIDEX_SUCCESS;

    if (!PyArg_ParseTuple(args, "L", &svalue))
        return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = sidex_Variant_Dict_First(svalue);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Variant_Dict_First failed (%ld)",err);
            return NULL;
    }

    Py_RETURN_NONE;
}

/*
 *------------------------------------------------------
 *               sidex_Variant_Dict_Next
 *------------------------------------------------------
 */
static PyObject * _sidex_Variant_Dict_Next(PyObject *self, PyObject *args)
{
    SIDEX_VARIANT svalue;
    SIDEX_VARIANT selement;
    SIDEX_TCHAR*  sKey;

    SIDEX_INT32    err = SIDEX_SUCCESS;

    if (!PyArg_ParseTuple(args, "L", &svalue))
        return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = sidex_Variant_Dict_Next(svalue, &sKey, &selement);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
	if (err == SIDEX_ERR_NOCONTENT) {
		Py_RETURN_NONE;
	}
    else if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Variant_Dict_Next failed (%ld)",err);
            return NULL;
    } 
    else{
      SIDEX_INT32 iLength = (SIDEX_INT32)wcslen(sKey);

      PyObject* py_Value = utf32_utf16_decode(sKey, iLength);

      PyObject* pRetVal = Py_BuildValue("OL", py_Value, selement);
      Py_XDECREF(py_Value);

      return pRetVal;
    }
}

/*
 *------------------------------------------------------
 *               sidex_Variant_Dict_HasKey
 *------------------------------------------------------
 */
static PyObject * _sidex_Variant_Dict_HasKey(PyObject *self, PyObject *args)
{
    SIDEX_VARIANT svalue;
    SIDEX_BOOL    sresult;
    PyObject*     py_key16;
    SIDEX_TCHAR*  pkey;
    PyObject*     py_key;

    SIDEX_INT32    err = SIDEX_SUCCESS;

    if (!PyArg_ParseTuple(args, "LO", &svalue, &py_key))
        return NULL;
    if ( _unicodeConversion(py_key, &pkey, &py_key16, sidexError) ) return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = sidex_Variant_Dict_HasKey(svalue, pkey, &sresult);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    unicodeFree(py_key16, pkey);

    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Variant_Dict_HasKey failed (%ld)",err);
            return NULL;
    }

    if (SIDEX_TRUE == sresult)
        Py_RETURN_TRUE;
    else
        Py_RETURN_FALSE;
}

/*
 *------------------------------------------------------
 *               sidex_Variant_Dict_Delete
 *------------------------------------------------------
 */
static PyObject * _sidex_Variant_Dict_Delete(PyObject *self, PyObject *args)
{
    SIDEX_VARIANT svalue;
    PyObject*     py_key16;
    SIDEX_TCHAR*  pkey;
    PyObject*     py_key;

    SIDEX_INT32    err = SIDEX_SUCCESS;

    if (!PyArg_ParseTuple(args, "LO", &svalue, &py_key))
        return NULL;
    if ( _unicodeConversion(py_key, &pkey, &py_key16, sidexError) ) return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = sidex_Variant_Dict_Delete(svalue, pkey);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    unicodeFree(py_key16, pkey);

    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Variant_Dict_Delete failed (%ld)",err);
            return NULL;
    }

    Py_RETURN_NONE;
}

/*
 *------------------------------------------------------
 *               sidex_Variant_Dict_Clear
 *------------------------------------------------------
 */
static PyObject * _sidex_Variant_Dict_Clear(PyObject *self, PyObject *args)
{
    SIDEX_VARIANT svalue;

    SIDEX_INT32    err = SIDEX_SUCCESS;

    if (!PyArg_ParseTuple(args, "L", &svalue))
        return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = sidex_Variant_Dict_Clear(svalue);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Variant_Dict_Clear failed (%ld)",err);
            return NULL;
    }

    Py_RETURN_NONE;
}

/*
 *------------------------------------------------------
 *               sidex_Variant_Dict_Get
 *------------------------------------------------------
 */
static PyObject * _sidex_Variant_Dict_Get(PyObject *self, PyObject *args)
{
    SIDEX_VARIANT svalue;
    PyObject*     py_key16;
    SIDEX_TCHAR*  pkey;
    PyObject*     py_key;
    SIDEX_VARIANT selement;

    SIDEX_INT32    err = SIDEX_SUCCESS;

    if (!PyArg_ParseTuple(args, "LO", &svalue, &py_key))
        return NULL;
    if ( _unicodeConversion(py_key, &pkey, &py_key16, sidexError) ) return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = sidex_Variant_Dict_Get(svalue, pkey, &selement);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    unicodeFree(py_key16, pkey);

    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Variant_Dict_Get failed (%ld)",err);
            return NULL;
    }

    return Py_BuildValue("L", selement);
}

/*
 *------------------------------------------------------
 *               sidex_Variant_GetType
 *------------------------------------------------------
 */
static PyObject * _sidex_Variant_GetType(PyObject *self, PyObject *args)
{
    SIDEX_VARIANT   svalue;
    SIDEX_DATA_TYPE stype;

    SIDEX_INT32    err = SIDEX_SUCCESS;

    if (!PyArg_ParseTuple(args, "L", &svalue))
        return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        stype = sidex_Variant_GetType(svalue);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Variant_GetType failed (%ld)",err);
            return NULL;
    }

    return Py_BuildValue("l", stype);
}

/*
 *------------------------------------------------------
 *               sidex_Variant_Copy
 *------------------------------------------------------
 */
static PyObject * _sidex_Variant_Copy(PyObject *self, PyObject *args)
{
    SIDEX_VARIANT   svalue;
    SIDEX_VARIANT   scopy;

    SIDEX_INT32    err = SIDEX_SUCCESS;

    if (!PyArg_ParseTuple(args, "L", &svalue))
        return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        scopy = sidex_Variant_Copy(svalue);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Variant_Copy failed (%ld)",err);
            return NULL;
    }

    return Py_BuildValue("L", scopy);
}

/*
 *------------------------------------------------------
 *               sidex_Variant_None_Check
 *------------------------------------------------------
 */
static PyObject * _sidex_Variant_None_Check(PyObject *self, PyObject *args)
{
    SIDEX_BOOL    sresult;
    SIDEX_VARIANT svalue;

    SIDEX_INT32    err = SIDEX_SUCCESS;

    if (!PyArg_ParseTuple(args, "L", &svalue))
        return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        sresult = sidex_Variant_None_Check(svalue);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Variant_None_Check failed (%ld)",err);
            return NULL;
    }

    if (SIDEX_TRUE == sresult)
        Py_RETURN_TRUE;
    else
        Py_RETURN_FALSE;
}

/*
 *------------------------------------------------------
 *               sidex_Variant_Boolean_Check
 *------------------------------------------------------
 */
static PyObject * _sidex_Variant_Boolean_Check(PyObject *self, PyObject *args)
{
    SIDEX_BOOL    sresult;
    SIDEX_VARIANT svalue;

    SIDEX_INT32    err = SIDEX_SUCCESS;

    if (!PyArg_ParseTuple(args, "L", &svalue))
        return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        sresult = sidex_Variant_Boolean_Check(svalue);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Variant_Boolean_Check failed (%ld)",err);
            return NULL;
    }

    if (SIDEX_TRUE == sresult)
        Py_RETURN_TRUE;
    else
        Py_RETURN_FALSE;
}

/*
 *------------------------------------------------------
 *               sidex_Variant_Integer_Check
 *------------------------------------------------------
 */
static PyObject * _sidex_Variant_Integer_Check(PyObject *self, PyObject *args)
{
    SIDEX_BOOL    sresult;
    SIDEX_VARIANT svalue;

    SIDEX_INT32    err = SIDEX_SUCCESS;

    if (!PyArg_ParseTuple(args, "L", &svalue))
        return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        sresult = sidex_Variant_Integer_Check(svalue);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Variant_Integer_Check failed (%ld)",err);
            return NULL;
    }

    if (SIDEX_TRUE == sresult)
        Py_RETURN_TRUE;
    else
        Py_RETURN_FALSE;
}

/*
 *------------------------------------------------------
 *               sidex_Variant_Float_Check
 *------------------------------------------------------
 */
static PyObject * _sidex_Variant_Float_Check(PyObject *self, PyObject *args)
{
    SIDEX_BOOL    sresult;
    SIDEX_VARIANT svalue;

    SIDEX_INT32    err = SIDEX_SUCCESS;

    if (!PyArg_ParseTuple(args, "L", &svalue))
        return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        sresult = sidex_Variant_Float_Check(svalue);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Variant_Float_Check failed (%ld)",err);
            return NULL;
    }

    if (SIDEX_TRUE == sresult)
        Py_RETURN_TRUE;
    else
        Py_RETURN_FALSE;
}

/*
 *------------------------------------------------------
 *               sidex_Variant_DateTime_Check
 *------------------------------------------------------
 */
static PyObject * _sidex_Variant_DateTime_Check(PyObject *self, PyObject *args)
{
    SIDEX_BOOL    sresult;
    SIDEX_VARIANT svalue;

    SIDEX_INT32    err = SIDEX_SUCCESS;

    if (!PyArg_ParseTuple(args, "L", &svalue))
        return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        sresult = sidex_Variant_DateTime_Check(svalue);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Variant_DateTime_Check failed (%ld)",err);
            return NULL;
    }

    if (SIDEX_TRUE == sresult)
        Py_RETURN_TRUE;
    else
        Py_RETURN_FALSE;
}

/*
 *------------------------------------------------------
 *               sidex_Variant_String_Check
 *------------------------------------------------------
 */
static PyObject * _sidex_Variant_String_Check(PyObject *self, PyObject *args)
{
    SIDEX_BOOL    sresult;
    SIDEX_VARIANT svalue;

    SIDEX_INT32    err = SIDEX_SUCCESS;

    if (!PyArg_ParseTuple(args, "L", &svalue))
        return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        sresult = sidex_Variant_String_Check(svalue);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Variant_String_Check failed (%ld)",err);
            return NULL;
    }

    if (SIDEX_TRUE == sresult)
        Py_RETURN_TRUE;
    else
        Py_RETURN_FALSE;
}

/*
 *------------------------------------------------------
 *               _sidex_Variant_Binary_Check
 *------------------------------------------------------
 */
static PyObject * _sidex_Variant_Binary_Check(PyObject *self, PyObject *args)
{
    SIDEX_BOOL    sresult;
    SIDEX_VARIANT svalue;

    SIDEX_INT32    err = SIDEX_SUCCESS;

    if (!PyArg_ParseTuple(args, "L", &svalue))
        return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        sresult = sidex_Variant_Binary_Check(svalue);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Variant_Binary_Check failed (%ld)",err);
            return NULL;
    }

    if (SIDEX_TRUE == sresult)
        Py_RETURN_TRUE;
    else
        Py_RETURN_FALSE;
}

/*
 *------------------------------------------------------
 *               sidex_DateTime_Read
 *------------------------------------------------------
 */
static PyObject * _sidex_DateTime_Read(PyObject *self, PyObject *args)
{
    SIDEX_VARIANT svalue;
    PyObject*     py_group16;
    SIDEX_TCHAR*  pgroup;
    PyObject*     py_group;
    PyObject*     py_key16;
    SIDEX_TCHAR*  pkey;
    PyObject*     py_key;
    SIDEX_VARIANT selement;

    SIDEX_INT32    err = SIDEX_SUCCESS;

    if (!PyArg_ParseTuple(args, "LOO", &svalue, &py_group, &py_key))
        return NULL;
    if ( _unicodeConversion(py_group, &pgroup, &py_group16, sidexError) ) return NULL;
    if ( _unicodeConversion(py_key, &pkey, &py_key16, sidexError) ) return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = sidex_DateTime_Read(svalue, pgroup, pkey, &selement);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    unicodeFree(py_group16, pgroup);
    unicodeFree(py_key16, pkey);

    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_DateTime_Read failed (%ld)",err);
            return NULL;
    }

    return Py_BuildValue("L", selement);
}

/*
 *------------------------------------------------------
 *               sidex_DateTime_Write
 *------------------------------------------------------
 */
static PyObject * _sidex_DateTime_Write(PyObject *self, PyObject *args)
{
    SIDEX_VARIANT svalue;
    PyObject*     py_group16;
    SIDEX_TCHAR*  pgroup;
    PyObject*     py_group;
    PyObject*     py_key16;
    SIDEX_TCHAR*  pkey;
    PyObject*     py_key;
    SIDEX_VARIANT selement;

    SIDEX_INT32    err = SIDEX_SUCCESS;

    if (!PyArg_ParseTuple(args, "LOOL", &svalue, &py_group, &py_key, &selement))
        return NULL;
    if ( _unicodeConversion(py_group, &pgroup, &py_group16, sidexError) ) return NULL;
    if ( _unicodeConversion(py_key, &pkey, &py_key16, sidexError) ) return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = sidex_DateTime_Write(svalue, pgroup, pkey, selement);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    unicodeFree(py_group16, pgroup);
    unicodeFree(py_key16, pkey);

    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_DateTime_Write failed (%ld)",err);
            return NULL;
    }

    Py_RETURN_NONE;
}

/*
 *------------------------------------------------------
 *               sidex_String_Read
 *------------------------------------------------------
 */
static PyObject * _sidex_String_Read(PyObject *self, PyObject *args)
{
    SIDEX_VARIANT svalue;
    PyObject*     py_group16;
    SIDEX_TCHAR*  pgroup;
    PyObject*     py_group;
    PyObject*     py_key16;
    SIDEX_TCHAR*  pkey;
    PyObject*     py_key;

    SIDEX_TCHAR * sresult;
    SIDEX_INT32   slength;

    PyObject *    presult;

    SIDEX_INT32    err = SIDEX_SUCCESS;

    if (!PyArg_ParseTuple(args, "LOO", &svalue, &py_group, &py_key))
        return NULL;
    if ( _unicodeConversion(py_group, &pgroup, &py_group16, sidexError) ) return NULL;
    if ( _unicodeConversion(py_key, &pkey, &py_key16, sidexError) ) return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = sidex_String_Read(svalue, pgroup, pkey, &sresult, &slength);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    unicodeFree(py_group16, pgroup);
    unicodeFree(py_key16, pkey);

    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_String_Read failed (%ld)",err);
            return NULL;
    }

    PyObject* pRetVal = utf32_utf16_decode(sresult, slength);

    presult = Py_BuildValue("O#", pRetVal, slength);
    Py_XDECREF(pRetVal);
    sidex_Free_ReadString(sresult);

    return presult;
}

/*
 *------------------------------------------------------
 *               sidex_String_Length
 *------------------------------------------------------
 */
static PyObject * _sidex_String_Length(PyObject *self, PyObject *args)
{
    SIDEX_VARIANT svalue;
    PyObject*     py_group16;
    SIDEX_TCHAR*  pgroup;
    PyObject*     py_group;
    PyObject*     py_key16;
    SIDEX_TCHAR*  pkey;
    PyObject*     py_key;

    SIDEX_INT32   slength;

    SIDEX_INT32    err = SIDEX_SUCCESS;

    if (!PyArg_ParseTuple(args, "LOO", &svalue, &py_group, &py_key))
        return NULL;
    if ( _unicodeConversion(py_group, &pgroup, &py_group16, sidexError) ) return NULL;
    if ( _unicodeConversion(py_key, &pkey, &py_key16, sidexError) ) return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = sidex_String_Length(svalue, pgroup, pkey, &slength);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    unicodeFree(py_group16, pgroup);
    unicodeFree(py_key16, pkey);

    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_String_Length failed (%ld)",err);
            return NULL;
    }

    return Py_BuildValue("l",slength);
}

/*
 *------------------------------------------------------
 *               sidex_Binary_Read
 *------------------------------------------------------
 */
static PyObject * _sidex_Binary_Read(PyObject *self, PyObject *args)
{
    SIDEX_VARIANT svalue;
    PyObject*     py_group16;
    SIDEX_TCHAR*  pgroup;
    PyObject*     py_group;
    PyObject*     py_key16;
    SIDEX_TCHAR*  pkey;
    PyObject*     py_key;

    unsigned char* sresult;
    SIDEX_INT32    slength;

    PyObject *    presult;

    SIDEX_INT32    err = SIDEX_SUCCESS;

    if (!PyArg_ParseTuple(args, "LOO", &svalue, &py_group, &py_key))
        return NULL;
    if ( _unicodeConversion(py_group, &pgroup, &py_group16, sidexError) ) return NULL;
    if ( _unicodeConversion(py_key, &pkey, &py_key16, sidexError) ) return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = sidex_Binary_Read(svalue, pgroup, pkey, &sresult, &slength);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    unicodeFree(py_group16, pgroup);
    unicodeFree(py_key16, pkey);

    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Binary_Read failed (%ld)",err);
            return NULL;
    }
#if PY_MAJOR_VERSION == 2
  #if PY_MINOR_VERSION >= 6
    presult = PyBytes_FromStringAndSize((const char*)sresult, slength);
  #else
    presult = Py_BuildValue("s#", sresult, slength);
  #endif
#else
  #if PY_MAJOR_VERSION > 2
    presult = PyBytes_FromStringAndSize((const char*)sresult, slength);
  #else
    presult = Py_BuildValue("s#", sresult, slength);
  #endif
#endif
    sidex_Free_Binary_ReadString(sresult);
    return presult;
}

/*
 *------------------------------------------------------
 *               sidex_Binary_Length
 *------------------------------------------------------
 */
static PyObject * _sidex_Binary_Length(PyObject *self, PyObject *args)
{
    SIDEX_VARIANT svalue;
    PyObject*     py_group16;
    SIDEX_TCHAR*  pgroup;
    PyObject*     py_group;
    PyObject*     py_key16;
    SIDEX_TCHAR*  pkey;
    PyObject*     py_key;

    SIDEX_INT32   slength;

    SIDEX_INT32    err = SIDEX_SUCCESS;

    if (!PyArg_ParseTuple(args, "LOO", &svalue, &py_group, &py_key))
        return NULL;
    if ( _unicodeConversion(py_group, &pgroup, &py_group16, sidexError) ) return NULL;
    if ( _unicodeConversion(py_key, &pkey, &py_key16, sidexError) ) return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = sidex_Binary_Length(svalue, pgroup, pkey, &slength);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    unicodeFree(py_group16, pgroup);
    unicodeFree(py_key16, pkey);

    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Binary_Length failed (%ld)",err);
            return NULL;
    }

    return Py_BuildValue("l",slength);
}

/*
 *------------------------------------------------------
 *               sidex_String_Write
 *------------------------------------------------------
 */
static PyObject * _sidex_String_Write(PyObject *self, PyObject *args)
{
    SIDEX_VARIANT svalue;
    PyObject*     py_group16;
    SIDEX_TCHAR*  pgroup;
    PyObject*     py_group;
    PyObject*     py_key16;
    SIDEX_TCHAR*  pkey;
    PyObject*     py_key;
    PyObject*     py_result16;
    SIDEX_TCHAR*  result;
    PyObject*     py_result;

    SIDEX_INT32    err = SIDEX_SUCCESS;

    if (!PyArg_ParseTuple(args, "LOOO", &svalue, &py_group, &py_key, &py_result))
        return NULL;
    if ( _unicodeConversion(py_group, &pgroup, &py_group16, sidexError) ) return NULL;
    if ( _unicodeConversion(py_key, &pkey, &py_key16, sidexError) ) return NULL;
    if ( _unicodeConversion(py_result, &result, &py_result16, sidexError) ) return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = sidex_String_Write(svalue, pgroup, pkey, result);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    unicodeFree(py_group16, pgroup);
    unicodeFree(py_key16, pkey);
    unicodeFree(py_result16, result);

    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_String_Write failed (%ld)",err);
            return NULL;
    }

    Py_RETURN_NONE;
}

/*
 *------------------------------------------------------
 *               sidex_Binary_Write
 *------------------------------------------------------
 */
static PyObject * _sidex_Binary_Write(PyObject *self, PyObject *args)
{
    SIDEX_VARIANT svalue;
    PyObject*     py_group16;
    SIDEX_TCHAR*  pgroup;
    PyObject*     py_group;
    PyObject*     py_key16;
    SIDEX_TCHAR*  pkey;
    PyObject*     py_key;

    unsigned char *        sresult;
    SIDEX_INT32   slength;

    SIDEX_INT32    err = SIDEX_SUCCESS;

    if (!PyArg_ParseTuple(args, "LOOs#", &svalue, &py_group, &py_key, &sresult, &slength))
        return NULL;
    if ( _unicodeConversion(py_group, &pgroup, &py_group16, sidexError) ) return NULL;
    if ( _unicodeConversion(py_key, &pkey, &py_key16, sidexError) ) return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = sidex_Binary_Write(svalue, pgroup, pkey, sresult, slength);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    unicodeFree(py_group16, pgroup);
    unicodeFree(py_key16, pkey);

    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Binary_Write failed (%ld)",err);
            return NULL;
    }

    Py_RETURN_NONE;
}

/*
 *------------------------------------------------------
 *               sidex_List_Read
 *------------------------------------------------------
 */
static PyObject * _sidex_List_Read(PyObject *self, PyObject *args)
{
    SIDEX_VARIANT svalue;
    PyObject*     py_group16;
    SIDEX_TCHAR*  pgroup;
    PyObject*     py_group;
    PyObject*     py_key16;
    SIDEX_TCHAR*  pkey;
    PyObject*     py_key;
    SIDEX_VARIANT selement;

    SIDEX_INT32    err = SIDEX_SUCCESS;

    if (!PyArg_ParseTuple(args, "LOO", &svalue, &py_group, &py_key))
        return NULL;
    if ( _unicodeConversion(py_group, &pgroup, &py_group16, sidexError) ) return NULL;
    if ( _unicodeConversion(py_key, &pkey, &py_key16, sidexError) ) return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = sidex_List_Read(svalue, pgroup, pkey, &selement);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    unicodeFree(py_group16, pgroup);
    unicodeFree(py_key16, pkey);

    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_List_Read failed (%ld)",err);
            return NULL;
    }

    return Py_BuildValue("L", selement);
}

/*
 *------------------------------------------------------
 *               sidex_List_Write
 *------------------------------------------------------
 */
static PyObject * _sidex_List_Write(PyObject *self, PyObject *args)
{
    SIDEX_VARIANT svalue;
    PyObject*     py_group16;
    SIDEX_TCHAR*  pgroup;
    PyObject*     py_group;
    PyObject*     py_key16;
    SIDEX_TCHAR*  pkey;
    PyObject*     py_key;
    SIDEX_VARIANT selement;

    SIDEX_INT32    err = SIDEX_SUCCESS;

    if (!PyArg_ParseTuple(args, "LOOL", &svalue, &py_group, &py_key,&selement))
        return NULL;
    if ( _unicodeConversion(py_group, &pgroup, &py_group16, sidexError) ) return NULL;
    if ( _unicodeConversion(py_key, &pkey, &py_key16, sidexError) ) return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = sidex_List_Write(svalue, pgroup, pkey, selement);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    unicodeFree(py_group16, pgroup);
    unicodeFree(py_key16, pkey);

    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_List_Write failed (%ld)",err);
            return NULL;
    }

    Py_RETURN_NONE;
}

/*
 *------------------------------------------------------
 *               sidex_Dict_Read
 *------------------------------------------------------
 */
static PyObject * _sidex_Dict_Read(PyObject *self, PyObject *args)
{
    SIDEX_VARIANT svalue;
    PyObject*     py_group16;
    SIDEX_TCHAR*  pgroup;
    PyObject*     py_group;
    PyObject*     py_key16;
    SIDEX_TCHAR*  pkey;
    PyObject*     py_key;
    SIDEX_VARIANT selement;

    SIDEX_INT32    err = SIDEX_SUCCESS;

    if (!PyArg_ParseTuple(args, "LOO", &svalue, &py_group, &py_key))
        return NULL;
    if ( _unicodeConversion(py_group, &pgroup, &py_group16, sidexError) ) return NULL;
    if ( _unicodeConversion(py_key, &pkey, &py_key16, sidexError) ) return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = sidex_Dict_Read(svalue, pgroup, pkey, &selement);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    unicodeFree(py_group16, pgroup);
    unicodeFree(py_key16, pkey);

    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Dict_Read failed (%ld)",err);
            return NULL;
    }

    return Py_BuildValue("L", selement);
}

/*
 *------------------------------------------------------
 *               sidex_Dict_Write
 *------------------------------------------------------
 */
static PyObject * _sidex_Dict_Write(PyObject *self, PyObject *args)
{
    SIDEX_VARIANT svalue;
    PyObject*     py_group16;
    SIDEX_TCHAR*  pgroup;
    PyObject*     py_group;
    PyObject*     py_key16;
    SIDEX_TCHAR*  pkey;
    PyObject*     py_key;
    SIDEX_VARIANT selement;

    SIDEX_INT32    err = SIDEX_SUCCESS;

    if (!PyArg_ParseTuple(args, "LOOL", &svalue, &py_group, &py_key,&selement))
        return NULL;
    if ( _unicodeConversion(py_group, &pgroup, &py_group16, sidexError) ) return NULL;
    if ( _unicodeConversion(py_key, &pkey, &py_key16, sidexError) ) return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = sidex_Dict_Write(svalue, pgroup, pkey, selement);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    unicodeFree(py_group16, pgroup);
    unicodeFree(py_key16, pkey);

    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Dict_Write failed (%ld)",err);
            return NULL;
    }

    Py_RETURN_NONE;
}

/*
 *------------------------------------------------------
 *               sidex_Table_Read
 *------------------------------------------------------
 */
static PyObject * _sidex_Table_Read(PyObject *self, PyObject *args)
{
    SIDEX_VARIANT svalue;
    PyObject*     py_group16;
    SIDEX_TCHAR*  pgroup;
    PyObject*     py_group;
    PyObject*     py_key16;
    SIDEX_TCHAR*  pkey;
    PyObject*     py_key;
    SIDEX_VARIANT selement;

    SIDEX_INT32    err = SIDEX_SUCCESS;

    if (!PyArg_ParseTuple(args, "LOO", &svalue, &py_group, &py_key))
        return NULL;
    if ( _unicodeConversion(py_group, &pgroup, &py_group16, sidexError) ) return NULL;
    if ( _unicodeConversion(py_key, &pkey, &py_key16, sidexError) ) return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = sidex_Table_Read(svalue, pgroup, pkey, &selement);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    unicodeFree(py_group16, pgroup);
    unicodeFree(py_key16, pkey);

    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Table_Read failed (%ld)",err);
            return NULL;
    }

    return Py_BuildValue("L", selement);
}

/*
 *------------------------------------------------------
 *               sidex_Table_Write
 *------------------------------------------------------
 */
static PyObject * _sidex_Table_Write(PyObject *self, PyObject *args)
{
    SIDEX_VARIANT svalue;
    PyObject*     py_group16;
    SIDEX_TCHAR*  pgroup;
    PyObject*     py_group;
    PyObject*     py_key16;
    SIDEX_TCHAR*  pkey;
    PyObject*     py_key;
    SIDEX_VARIANT selement;

    SIDEX_INT32    err = SIDEX_SUCCESS;


    if (!PyArg_ParseTuple(args, "LOOL", &svalue, &py_group, &py_key,&selement))
        return NULL;
    if ( _unicodeConversion(py_group, &pgroup, &py_group16, sidexError) ) return NULL;
    if ( _unicodeConversion(py_key, &pkey, &py_key16, sidexError) ) return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = sidex_Table_Write(svalue, pgroup, pkey, selement);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    unicodeFree(py_group16, pgroup);
    unicodeFree(py_key16, pkey);

    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Table_Write failed (%ld)",err);
            return NULL;
    }

    Py_RETURN_NONE;
}

/*
 *------------------------------------------------------
 *               sidex_Variant_As_Boolean
 *------------------------------------------------------
 */
static PyObject * _sidex_Variant_As_Boolean(PyObject *self, PyObject *args)
{
    SIDEX_VARIANT svalue;
    SIDEX_BOOL    sresult;

    SIDEX_INT32    err = SIDEX_SUCCESS;
    

    if (!PyArg_ParseTuple(args, "L", &svalue))
        return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = sidex_Variant_As_Boolean(svalue, &sresult);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Variant_As_Boolean failed (%ld)",err);
            return NULL;
    }

    if (SIDEX_TRUE == sresult)
        Py_RETURN_TRUE;
    else
        Py_RETURN_FALSE;
}

/*
 *------------------------------------------------------
 *               sidex_Variant_As_Integer
 *------------------------------------------------------
 */
static PyObject * _sidex_Variant_As_Integer(PyObject *self, PyObject *args)
{
    SIDEX_VARIANT svalue;
    SIDEX_INT64   sresult;

    SIDEX_INT32    err = SIDEX_SUCCESS;
    

    if (!PyArg_ParseTuple(args, "L", &svalue))
        return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = sidex_Variant_As_Integer(svalue, &sresult);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Variant_As_Integer failed (%ld)",err);
            return NULL;
    }

    return Py_BuildValue("L",sresult);
}

/*
 *------------------------------------------------------
 *               sidex_Variant_As_Float
 *------------------------------------------------------
 */
static PyObject * _sidex_Variant_As_Float(PyObject *self, PyObject *args)
{
    SIDEX_VARIANT svalue;
    SIDEX_DOUBLE  sresult;

    SIDEX_INT32    err = SIDEX_SUCCESS;
    

    if (!PyArg_ParseTuple(args, "L", &svalue))
        return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = sidex_Variant_As_Float(svalue, &sresult);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Variant_As_Float failed (%ld)",err);
            return NULL;
    }

    return Py_BuildValue("d",sresult);
}

/*
 *------------------------------------------------------
 *               sidex_Variant_As_DateTime
 *------------------------------------------------------
 */
static PyObject * _sidex_Variant_As_DateTime(PyObject *self, PyObject *args)
{
    SIDEX_VARIANT svalue;
    SIDEX_TCHAR*  sresult;

    SIDEX_INT32    err = SIDEX_SUCCESS;
    

    if (!PyArg_ParseTuple(args, "L", &svalue))
        return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = sidex_Variant_As_DateTime(svalue, &sresult);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Variant_As_DateTime failed (%ld)",err);
            return NULL;
    }

    SIDEX_INT32 iYear;
    SIDEX_INT32 iMonth;
    SIDEX_INT32 iDay;
    SIDEX_INT32 iHour;
    SIDEX_INT32 iMinute;
    SIDEX_INT32 iSecond;
    SIDEX_INT32 iMillisecond;
    int weekday = 0;
    int yearday = 0;
    int isDstTime   = 0;// W. Europe Standard Time - UTC
    //int isDstTime = 1;// W. Europe Daylight Time - DST
    
#ifdef LINUX
    swscanf(sresult, L"%4d-%2d-%2d %2d:%2d:%2d:%3d", &iYear, &iMonth, &iDay, &iHour, &iMinute, &iSecond, &iMillisecond);
#else // LINUX
    swscanf_s(sresult, L"%4d-%2d-%2d %2d:%2d:%2d:%3d", &iYear, &iMonth, &iDay, &iHour, &iMinute, &iSecond, &iMillisecond);
#endif // LINUX
    weekday = DayOfWeek (iDay, iMonth, iYear);
    yearday = DayOfYear(iDay, iMonth, iYear);


    PyObject* retValue = Py_BuildValue("(iiiiiiiii)", iYear, iMonth, iDay, iHour, iMinute, iSecond, weekday, yearday, isDstTime);

    return retValue;
}

/*
 *------------------------------------------------------
 *               sidex_Variant_As_String
 *------------------------------------------------------
 */
static PyObject * _sidex_Variant_As_String(PyObject *self, PyObject *args)
{
    SIDEX_VARIANT svalue;
    SIDEX_TCHAR*  sresult;
    SIDEX_INT32   slength;

    SIDEX_INT32    err = SIDEX_SUCCESS;
    

    if (!PyArg_ParseTuple(args, "L", &svalue))
        return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = sidex_Variant_As_String(svalue, &sresult, &slength);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Variant_As_String failed (%ld)",err);
            return NULL;
    }

    PyObject* pRetVal = utf32_utf16_decode(sresult, slength);

    return pRetVal;
}

/*
 *------------------------------------------------------
 *               sidex_Variant_String_GetFormat
 *------------------------------------------------------
 */
static PyObject * _sidex_Variant_String_GetFormat(PyObject *self, PyObject *args)
{
    SIDEX_VARIANT svalue;
    const SIDEX_TCHAR*  sresult;

    SIDEX_INT32    err = SIDEX_SUCCESS;
    

    if (!PyArg_ParseTuple(args, "L", &svalue))
        return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = sidex_Variant_String_GetFormat(svalue, &sresult);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Variant_String_GetFormat failed (%ld)",err);
            return NULL;
    }

    PyObject* pRetVal = utf32_utf16_decode((SIDEX_TCHAR*)sresult, -1);

    return pRetVal;
}

/*
 *------------------------------------------------------
 *               _sidex_Variant_String_SetFormat
 *------------------------------------------------------
 */
static PyObject * _sidex_Variant_String_SetFormat(PyObject *self, PyObject *args)
{
    PyObject*    py_pFormat16;
    SIDEX_TCHAR* pFormat;
    PyObject*    py_pFormat;
    SIDEX_HANDLE shandle;

    SIDEX_INT32   err = SIDEX_SUCCESS;

    if (!PyArg_ParseTuple(args, "LO", &shandle, &py_pFormat))
        return NULL;
    if ( _unicodeConversion(py_pFormat, &pFormat, &py_pFormat16, sidexError) ) return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = sidex_Variant_String_SetFormat(shandle, pFormat);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    unicodeFree(py_pFormat16, pFormat);

    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "_sidex_Variant_String_SetFormat failed (%ld)",err);
            return NULL;
    }

    Py_RETURN_NONE;
}

/*
 *------------------------------------------------------
 *               sidex_Variant_As_String_Length
 *------------------------------------------------------
 */
static PyObject * _sidex_Variant_As_String_Length(PyObject *self, PyObject *args)
{
    SIDEX_VARIANT svalue;
    SIDEX_INT32   slength;

    SIDEX_INT32    err = SIDEX_SUCCESS;
    

    if (!PyArg_ParseTuple(args, "L", &svalue))
        return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = sidex_Variant_As_String_Length(svalue, &slength);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Variant_As_String_Length failed (%ld)",err);
            return NULL;
    }

    return Py_BuildValue("l",slength);
}

/*
 *------------------------------------------------------
 *               sidex_Variant_As_Binary
 *------------------------------------------------------
 */
static PyObject * _sidex_Variant_As_Binary(PyObject *self, PyObject *args)
{
    SIDEX_VARIANT svalue;
    unsigned char*         sresult;
    SIDEX_INT32   slength;

    SIDEX_INT32    err = SIDEX_SUCCESS;
    

    if (!PyArg_ParseTuple(args, "L", &svalue))
        return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = sidex_Variant_As_Binary(svalue, &sresult, &slength);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Variant_As_Binary failed (%ld)",err);
            return NULL;
    }
    PyObject* pRes;

#if PY_MAJOR_VERSION == 2
  #if PY_MINOR_VERSION >= 6
    pRes = PyBytes_FromStringAndSize((const char*)sresult, slength);
  #else
    pRes = Py_BuildValue("s#", sresult, slength);
  #endif
#else
  #if PY_MAJOR_VERSION > 2
    pRes = PyBytes_FromStringAndSize((const char*)sresult, slength);
  #else
    pRes = Py_BuildValue("s#", sresult, slength);
  #endif
#endif
    return pRes;
}

/*
 *------------------------------------------------------
 *               sidex_Variant_As_Binary_Length
 *------------------------------------------------------
 */
static PyObject * _sidex_Variant_As_Binary_Length(PyObject *self, PyObject *args)
{
    SIDEX_VARIANT svalue;
    SIDEX_INT32   slength;

    SIDEX_INT32    err = SIDEX_SUCCESS;
    

    if (!PyArg_ParseTuple(args, "L", &svalue))
        return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = sidex_Variant_As_Binary_Length(svalue, &slength);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Variant_As_Binary_Length failed (%ld)",err);
            return NULL;
    }

    return Py_BuildValue("l",slength);
}

/*
 *------------------------------------------------------
 *               sidex_Variant_Table_Check
 *------------------------------------------------------
 */
static PyObject * _sidex_Variant_Table_Check(PyObject *self, PyObject *args)
{
    SIDEX_VARIANT  vhandle;
    SIDEX_BOOL     sresult;

    SIDEX_INT32    err = SIDEX_SUCCESS;
    

    if (!PyArg_ParseTuple(args, "L", &vhandle))
        return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        sresult = sidex_Variant_Table_Check(vhandle);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Variant_Table_Check failed (%ld)",err);
            return NULL;
    }

    if (SIDEX_TRUE == sresult)
        Py_RETURN_TRUE;
    else
        Py_RETURN_FALSE;

}

/*
 *------------------------------------------------------
 *               sidex_Variant_Table_Rows
 *------------------------------------------------------
 */
static PyObject * _sidex_Variant_Table_Rows(PyObject *self, PyObject *args)
{
    SIDEX_VARIANT svalue;
    SIDEX_INT32   iRows;

    SIDEX_INT32    err = SIDEX_SUCCESS;
    

    if (!PyArg_ParseTuple(args, "L", &svalue))
        return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = sidex_Variant_Table_Rows(svalue, &iRows);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Variant_Table_Rows failed (%ld)",err);
            return NULL;
    }
    return Py_BuildValue("l",iRows);
}

/*
 *------------------------------------------------------
 *               sidex_Variant_Table_Columns
 *------------------------------------------------------
 */
static PyObject * _sidex_Variant_Table_Columns(PyObject *self, PyObject *args)
{
    SIDEX_VARIANT svalue;
    SIDEX_INT32   iColumns;

    SIDEX_INT32    err = SIDEX_SUCCESS;
    

    if (!PyArg_ParseTuple(args, "L", &svalue))
        return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = sidex_Variant_Table_Columns(svalue, &iColumns);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Variant_Table_Columns failed (%ld)",err);
            return NULL;
    }

    return Py_BuildValue("l",iColumns);
}

/*
 *------------------------------------------------------
 *               sidex_Variant_Table_ColumnNames
 *------------------------------------------------------
 */
static PyObject * _sidex_Variant_Table_ColumnNames(PyObject *self, PyObject *args)
{
    SIDEX_VARIANT svalue;
    SIDEX_VARIANT sColumns;

    SIDEX_INT32    err = SIDEX_SUCCESS;
    

    if (!PyArg_ParseTuple(args, "L", &svalue))
        return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = sidex_Variant_Table_ColumnNames(svalue, &sColumns);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Variant_Table_ColumnNames failed (%ld)",err);
            return NULL;
    }

    return Py_BuildValue("L",sColumns);
}

/*
 *------------------------------------------------------
 *               sidex_Variant_Table_HasColumn
 *------------------------------------------------------
 */
static PyObject * _sidex_Variant_Table_HasColumn(PyObject *self, PyObject *args)
{
    SIDEX_VARIANT svalue;
    SIDEX_BOOL    sresult;
    PyObject*     py_pColumn16;
    SIDEX_TCHAR*  pColumn;
    PyObject*     py_pColumn;

    SIDEX_INT32    err = SIDEX_SUCCESS;
    

    if (!PyArg_ParseTuple(args, "LO", &svalue, &py_pColumn))
        return NULL;
    if ( _unicodeConversion(py_pColumn, &pColumn, &py_pColumn16, sidexError) ) return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = sidex_Variant_Table_HasColumn(svalue, pColumn, &sresult);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    unicodeFree(py_pColumn16, pColumn);

    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Variant_Table_HasColumn failed (%ld)",err);
            return NULL;
    }

    if (SIDEX_TRUE == sresult)
        Py_RETURN_TRUE;
    else
        Py_RETURN_FALSE;
}

/*
 *------------------------------------------------------
 *               sidex_Variant_Table_AddColumn
 *------------------------------------------------------
 */
static PyObject * _sidex_Variant_Table_AddColumn(PyObject *self, PyObject *args)
{
    SIDEX_VARIANT vhandle;
    PyObject*     py_pColumnName16;
    SIDEX_TCHAR*  pColumnName;
    PyObject*     py_pColumnName;

    SIDEX_INT32    err = SIDEX_SUCCESS;
    

    if (!PyArg_ParseTuple(args, "LO", &vhandle, &py_pColumnName))
        return NULL;
    if ( _unicodeConversion(py_pColumnName, &pColumnName, &py_pColumnName16, sidexError) ) return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = sidex_Variant_Table_AddColumn(vhandle, pColumnName);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    unicodeFree(py_pColumnName16, pColumnName);

    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Variant_Table_AddColumn failed (%ld)",err);
            return NULL;
    }
    Py_RETURN_NONE;
}

/*
 *------------------------------------------------------
 *               sidex_Variant_Table_DeleteColumn
 *------------------------------------------------------
 */
static PyObject * _sidex_Variant_Table_DeleteColumn(PyObject *self, PyObject *args)
{
    SIDEX_VARIANT svalue;
    PyObject*     py_pColumn16;
    SIDEX_TCHAR*  pColumn;
    PyObject*     py_pColumn;

    SIDEX_INT32    err = SIDEX_SUCCESS;
    

    if (!PyArg_ParseTuple(args, "LO", &svalue, &py_pColumn))
        return NULL;
    if ( _unicodeConversion(py_pColumn, &pColumn, &py_pColumn16, sidexError) ) return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = sidex_Variant_Table_DeleteColumn(svalue, pColumn);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    unicodeFree(py_pColumn16, pColumn);

    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Variant_Table_DeleteColumn failed (%ld)",err);
            return NULL;
    }

    Py_RETURN_NONE;
}

/*
 *------------------------------------------------------
 *               sidex_Variant_Table_GetColumnName
 *------------------------------------------------------
 */
static PyObject * _sidex_Variant_Table_GetColumnName(PyObject *self, PyObject *args)
{
    SIDEX_VARIANT svalue;
    SIDEX_INT32   iColIndex;
    SIDEX_TCHAR * pColumn;

    SIDEX_INT32    err = SIDEX_SUCCESS;
    

    if (!PyArg_ParseTuple(args, "Ll", &svalue, &iColIndex))
        return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = sidex_Variant_Table_GetColumnName(svalue, iColIndex, &pColumn);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Variant_Table_GetColumnName failed (%ld)",err);
            return NULL;
    }

    PyObject* pyColumn = utf32_utf16_decode(pColumn, -1);

    return pyColumn;
}

/*
 *------------------------------------------------------
 *               sidex_Variant_Table_AddRow
 *------------------------------------------------------
 */
static PyObject * _sidex_Variant_Table_AddRow(PyObject *self, PyObject *args)
{
    SIDEX_INT32   iRowIndex;
    SIDEX_VARIANT  vhandle;

    SIDEX_INT32    err = SIDEX_SUCCESS;
    

    if (!PyArg_ParseTuple(args, "L", &vhandle))
        return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = sidex_Variant_Table_AddRow(vhandle, &iRowIndex);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Variant_Table_AddRow failed (%ld)",err);
            return NULL;
    }
    return Py_BuildValue("l",iRowIndex);
}

/*
 *------------------------------------------------------
 *               sidex_Variant_Table_DeleteRow
 *------------------------------------------------------
 */
static PyObject * _sidex_Variant_Table_DeleteRow(PyObject *self, PyObject *args)
{
    SIDEX_VARIANT svalue;
    SIDEX_INT32   iRow;
    SIDEX_INT64   iTemp64; // Umgehung von Speicherzugriffsverletzung bei Python 3.4 unter LINUX x64

    SIDEX_INT32    err = SIDEX_SUCCESS;
    

    if (!PyArg_ParseTuple(args, "Ll", &svalue, &iTemp64))
        return NULL;
    iRow = (SIDEX_INT32)iTemp64;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = sidex_Variant_Table_DeleteRow(svalue, iRow);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Variant_Table_DeleteRow failed (%ld)",err);
            return NULL;
    }

    Py_RETURN_NONE;
}

/*
 *------------------------------------------------------
 *               sidex_Variant_Table_DeleteRows
 *------------------------------------------------------
 */
static PyObject * _sidex_Variant_Table_DeleteRows(PyObject *self, PyObject *args)
{
    SIDEX_VARIANT svalue;

    SIDEX_INT32    err = SIDEX_SUCCESS;
    

    if (!PyArg_ParseTuple(args, "L", &svalue))
        return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = sidex_Variant_Table_DeleteRows(svalue);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Variant_Table_DeleteRows failed (%ld)",err);
            return NULL;
    }

    Py_RETURN_NONE;
}

/*
 *------------------------------------------------------
 *               sidex_Variant_Table_GetRow
 *------------------------------------------------------
 */
static PyObject * _sidex_Variant_Table_GetRow(PyObject *self, PyObject *args)
{
    SIDEX_VARIANT svalue;
    SIDEX_INT32   iColIndex;
    SIDEX_INT64   iTemp64; // Umgehung von Speicherzugriffsverletzung bei Python 3.4 unter LINUX x64
    SIDEX_VARIANT pRow;

    SIDEX_INT32    err = SIDEX_SUCCESS;
    

    if (!PyArg_ParseTuple(args, "Ll", &svalue, &iTemp64))
        return NULL;
    iColIndex = (SIDEX_INT32)iTemp64;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = sidex_Variant_Table_GetRow(svalue, iColIndex, &pRow);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Variant_Table_GetRow failed (%ld)",err);
            return NULL;
    }

    return Py_BuildValue("L",pRow);
}

/*
 *------------------------------------------------------
 *               sidex_Variant_Table_GetField
 *------------------------------------------------------
 */
static PyObject * _sidex_Variant_Table_GetField(PyObject *self, PyObject *args)
{
    SIDEX_VARIANT vhandle;
    SIDEX_INT32   iRowIndex;
    SIDEX_INT64   iTemp64; // Umgehung von Speicherzugriffsverletzung bei Python 3.4 unter LINUX x64
    PyObject*     py_pColumnName16;
    SIDEX_TCHAR*  pColumnName;
    PyObject*     py_pColumnName;
    SIDEX_VARIANT pValue;

    SIDEX_INT32    err = SIDEX_SUCCESS;
    

    if (!PyArg_ParseTuple(args, "LlO", &vhandle, &iTemp64, &py_pColumnName))
        return NULL;
    iRowIndex = (SIDEX_INT32)iTemp64;
    if ( _unicodeConversion(py_pColumnName, &pColumnName, &py_pColumnName16, sidexError) ) return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = sidex_Variant_Table_GetField(vhandle, iRowIndex, pColumnName, &pValue);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    unicodeFree(py_pColumnName16, pColumnName);

    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Variant_Table_GetField failed (%ld)",err);
            return NULL;
    }
    return Py_BuildValue("L",pValue);
}

/*
 *------------------------------------------------------
 *               sidex_Variant_Table_SetField
 *------------------------------------------------------
 */
static PyObject * _sidex_Variant_Table_SetField(PyObject *self, PyObject *args)
{
    SIDEX_VARIANT vhandle;
    SIDEX_INT32   iRowIndex;
    SIDEX_INT64   iTemp64; // Umgehung von Speicherzugriffsverletzung bei Python 3.4 unter LINUX x64
    PyObject*     py_pColumnName16;
    SIDEX_TCHAR*  pColumnName;
    PyObject*     py_pColumnName;
    SIDEX_VARIANT pValue;

    SIDEX_INT32    err = SIDEX_SUCCESS;
    

    if (!PyArg_ParseTuple(args, "LlOL", &vhandle, &iTemp64, &py_pColumnName, &pValue))
        return NULL;
    iRowIndex = (SIDEX_INT32)iTemp64;
    if ( _unicodeConversion(py_pColumnName, &pColumnName, &py_pColumnName16, sidexError) ) return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = sidex_Variant_Table_SetField(vhandle, iRowIndex, pColumnName, pValue);
    }
    catch (...) {
        err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    unicodeFree(py_pColumnName16, pColumnName);

    if (err != SIDEX_SUCCESS) {
            PyErr_Format(sidexError, "sidex_Variant_Table_SetField failed (%ld)",err);
            return NULL;
    }
    Py_RETURN_NONE;
}

/*
 *------------------------------------------------------
 *               _PyObjectAsSidexVariant
 *------------------------------------------------------
 */
static PyObject * _PyObjectAsSidexVariant(PyObject *self, PyObject *args)
{
    PyObject*      value;
    SIDEX_VARIANT  sVariant = SIDEX_HANDLE_TYPE_NULL;
    SIDEX_INT32    err = SIDEX_SUCCESS;
    

    if (!PyArg_ParseTuple(args, "O", &value))
        return NULL;

    sVariant = _PyObjectAsSidexVariantS (value, &err, sidexError);

    if (SIDEX_SUCCESS != err){
            PyErr_Format(sidexError, "_PyObjectAsSidexVariant failed (%ld)",err);
            return NULL;
    }
    if (SIDEX_HANDLE_TYPE_NULL == sVariant) {
            // Seems to be no Unicode / String parameter
            return NULL;
    }
    return Py_BuildValue("L",sVariant);
}

/*
 *------------------------------------------------------
 *               _SidexVariantAsPyObject
 *------------------------------------------------------
 */

static PyObject * _SidexVariantAsPyObject(PyObject *self, PyObject *args)
{
    PyObject*      pValue;
    PyObject*      retValue = NULL;
    SIDEX_INT32    err = SIDEX_SUCCESS;
    PyObject*      tuple = NULL;

    if (!PyArg_ParseTuple(args, "O", &pValue))
        return NULL;

    if (Py_None == pValue){
      err = SIDEX_ERR_COMMON;
      printf ("## Type Error ##\n");
    }
    else{
#if PY_MAJOR_VERSION >= 3
      if (true == PyLong_Check(pValue))
#else
      if ((true == PyLong_Check(pValue)) || (true == PyInt_Check(pValue)))
#endif
      {
        tuple = Py_BuildValue("(O)",pValue);
      }
      else{
        printf ("## Type Error ##\n");
        err = SIDEX_ERR_COMMON;
      }
    }
    if (err == SIDEX_SUCCESS){
      if (NULL != tuple){
        SIDEX_VARIANT sVariant;
        if (!PyArg_ParseTuple(tuple, "L", &sVariant)){
          err = SIDEX_ERR_COMMON;
        }
        else{
          retValue = _SidexVariantDecode(sVariant, &err);
        }
        Py_XDECREF(tuple);
      }
    }

    if (SIDEX_SUCCESS != err){
            PyErr_Format(sidexError, "_SidexVariantAsPyObject failed (%ld)",err);
            return NULL;
    }
    return retValue;
}

/*
 *************************************************************************************
 *                                  function defintion table
 *************************************************************************************
 */
static PyMethodDef _sidex_methods[] = {
    {"sidex_Get_Version",  (PyCFunction)_sidex_Get_Version, METH_NOARGS,"sidex_Get_Version() -> (<api version>,<lib version>,<lib version string>)"},
    {"sidex_Get_Copyright",  _sidex_Get_Copyright, METH_VARARGS,"sidex_Get_Copyright() -> <value>"},
    {"sidex_Set_Password",  _sidex_Set_Password, METH_VARARGS,"sidex_Set_Password(<name>,<password>)"},
    {"sidex_Create",  _sidex_Create, METH_VARARGS,"sidex_Create(<name>) -> sidex handle"},
    {"sidex_Set_DocumentName",  _sidex_Set_DocumentName, METH_VARARGS,"sidex_Set_DocumentName(<sidex_handle>,<name>)"},
    {"sidex_Get_DocumentName",  _sidex_Get_DocumentName, METH_VARARGS,"sidex_Get_DocumentName(<sidex_handle>) -> <name>"},
    {"sidex_Clear",  _sidex_Clear, METH_VARARGS,"sidex_Clear(<sidex_handle>)"},
    {"sidex_Free",  _sidex_Free, METH_VARARGS,"sidex_Free(<sidex_handle>)"},
    {"sidex_Load_Content",  _sidex_Load_Content, METH_VARARGS,"sidex_Load_Content(<sidex_handle>,<path>)"},
    {"sidex_Set_Content",  _sidex_Set_Content, METH_VARARGS,"sidex_Set_Content(<sidex_handle>,<content>)"},
    {"sidex_Save_Content",  _sidex_Save_Content, METH_VARARGS,"sidex_Save_Content(<sidex_handle>,<path>)"},
    {"sidex_Get_Content_Length",  _sidex_Get_Content_Length, METH_VARARGS,"sidex_Get_Content_Length(<sidex_handle>) -><length>"},
    {"sidex_Get_Content",  _sidex_Get_Content, METH_VARARGS,"sidex_Get_Content(<sidex_handle>) -> xml string"},
    /////////////////////////////////////////////////////////////////////////
    // sidex_Free_Content is not necessary because of internal Py_BuildValue
    /////////////////////////////////////////////////////////////////////////
    {"sidex_None_Write",  _sidex_None_Write, METH_VARARGS,"sidex_None_Write(<sidex_handle>,<group>,<key>)"},
    {"sidex_Boolean_Write",  _sidex_Boolean_Write, METH_VARARGS,"sidex_Boolean_Write(<sidex_handle>,<group>,<key>,<value>)"},
    {"sidex_Boolean_Read",  _sidex_Boolean_Read, METH_VARARGS,"sidex_Boolean_Read(<sidex_handle>,<group>,<key>) -> <value>"},
    {"sidex_Integer_Write",  _sidex_Integer_Write, METH_VARARGS,"sidex_Integer_Write(<sidex_handle>,<group>,<key>,<value>)"},
    {"sidex_Integer_Read",  _sidex_Integer_Read, METH_VARARGS,"sidex_Integer_Read(<sidex_handle>,<group>,<key>) -> <value>"},
    {"sidex_Float_Write",  _sidex_Float_Write, METH_VARARGS,"sidex_Float_Write(<sidex_handle>,<group>,<key>,<value>)"},
    {"sidex_Float_Read",  _sidex_Float_Read, METH_VARARGS,"sidex_Float_Read(<sidex_handle>,<group>,<key>) -> <value>"},
    {"sidex_Variant_Write",  _sidex_Variant_Write, METH_VARARGS,"sidex_Variant_Write(<sidex_handle>,<group>,<key>,<value>)"},
    {"sidex_Variant_Read",  _sidex_Variant_Read, METH_VARARGS,"sidex_Variant_Read(<sidex_handle>,<group>,<key>) -> <value>"},
    {"sidex_DateTime_Read",  _sidex_DateTime_Read, METH_VARARGS,"sidex_DateTime_Read(<sidex_handle>,<group>,<key>) -> <value>"},
    {"sidex_DateTime_Write",  _sidex_DateTime_Write, METH_VARARGS,"sidex_DateTime_Write(<sidex_handle>,<group>,<key>,<value>)"},
    {"sidex_String_Length",  _sidex_String_Length, METH_VARARGS,"sidex_String_Length>(<sidex_handle>,<group>,<key>) -> <length>"},
    {"sidex_String_Read",  _sidex_String_Read, METH_VARARGS,"sidex_String_Read(<sidex_handle>,<group>,<key>) -> <string>"},
    /////////////////////////////////////////////////////////////////////////
    // sidex_Free_ReadString is not necessary because of internal Py_BuildValue
    /////////////////////////////////////////////////////////////////////////
    {"sidex_Binary_Length",  _sidex_Binary_Length, METH_VARARGS,"sidex_Binary_Length(<sidex_handle>,<group>,<key>) -> <length>"},
    {"sidex_Binary_Read",  _sidex_Binary_Read, METH_VARARGS,"sidex_Binary_Read(<sidex_handle>,<group>,<key>) -> <binary>"},
    {"sidex_String_Write",  _sidex_String_Write, METH_VARARGS,"sidex_String_Write(<sidex_handle>,<group>,<key>,<value>)"},
    {"sidex_Binary_Write",  _sidex_Binary_Write, METH_VARARGS,"sidex_Binary_Write(<sidex_handle>,<group>,<key>,<value>)"},
    /////////////////////////////////////////////////////////////////////////
    // sidex_Free_Binary_ReadString is not necessary because of internal Py_BuildValue
    /////////////////////////////////////////////////////////////////////////
    
    {"sidex_List_Read",  _sidex_List_Read, METH_VARARGS,"sidex_List_Read(<sidex_handle>,<group>,<key>) -> <value>"},
    {"sidex_List_Write",  _sidex_List_Write, METH_VARARGS,"sidex_List_Write(<sidex_handle>,<group>,<key>,<value>)"},
    {"sidex_Dict_Read",  _sidex_Dict_Read, METH_VARARGS,"sidex_Dict_Read(<sidex_handle>,<group>,<key>) -> <value>"},
    {"sidex_Dict_Write",  _sidex_Dict_Write, METH_VARARGS,"sidex_Dict_Write(<sidex_handle>,<group>,<key>,<value>)"},
    {"sidex_Table_Read",  _sidex_Table_Read, METH_VARARGS,"sidex_Table_Read(<sidex_handle>,<group>,<key>) -> <value>"},
    {"sidex_Table_Write",  _sidex_Table_Write, METH_VARARGS,"sidex_Table_Write(<sidex_handle>,<group>,<key>,<value>)"},

    {"sidex_GetGroups",  _sidex_GetGroups, METH_VARARGS,"sidex_GetGroups(<sidex_handle>) -> <groups>"},
    {"sidex_HasGroup",  _sidex_HasGroup, METH_VARARGS,"sidex_HasGroup(<sidex_handle>,<group>) -> True,False"},
    {"sidex_GetKeys",  _sidex_GetKeys, METH_VARARGS,"sidex_GetKeys(<sidex_handle>,<group>) -> <keys>"},
    {"sidex_HasKey",  _sidex_HasKey, METH_VARARGS,"sidex_HasKey(<sidex_handle>,<group>,<key>) -> True,False"},
    {"sidex_DeleteGroup",  _sidex_DeleteGroup, METH_VARARGS,"sidex_DeleteGroup(<sidex_handle>,<group>)"},
    {"sidex_DeleteKey",  _sidex_DeleteKey, METH_VARARGS,"sidex_DeleteKey(<sidex_handle>,<group>,<key>)"},

    // <-- sidex variant
    {"sidex_Variant_IncRef",  _sidex_Variant_IncRef, METH_VARARGS,"sidex_Variant_IncRef(<variant>)"}, 
    {"sidex_Variant_DecRef",  _sidex_Variant_DecRef, METH_VARARGS,"sidex_Variant_DecRef(<variant>)"},
    {"sidex_Variant_New_None",  (PyCFunction)_sidex_Variant_New_None, METH_NOARGS,"sidex_Variant_New_None() -> (<variant>)"},
    {"sidex_Variant_New_Boolean", _sidex_Variant_New_Boolean, METH_VARARGS,"sidex_Variant_New_Boolean(<value>) -> (<variant>)"},
    {"sidex_Variant_New_Integer", _sidex_Variant_New_Integer, METH_VARARGS,"sidex_Variant_New_Integer(<value>) -> (<variant>)"},
    {"sidex_Variant_New_Float", _sidex_Variant_New_Float, METH_VARARGS,"sidex_Variant_New_Float(<value>) -> (<variant>)"},
    {"sidex_Variant_New_DateTime", _sidex_Variant_New_DateTime, METH_VARARGS,"sidex_Variant_New_DateTime(<value>) -> (<variant>)"},
    {"sidex_Variant_New_String", _sidex_Variant_New_String, METH_VARARGS,"sidex_Variant_New_String(<value>) -> (<variant>)"},
    {"sidex_Variant_New_Binary", _sidex_Variant_New_Binary, METH_VARARGS,"sidex_Variant_New_Binary(<value>) -> (<variant>)"},
    {"sidex_Variant_New_List",  (PyCFunction)_sidex_Variant_New_List, METH_NOARGS,"sidex_Variant_New_List() -> (<variant>)"},
    {"sidex_Variant_New_Dict",  (PyCFunction)_sidex_Variant_New_Dict, METH_NOARGS,"sidex_Variant_New_Dict() -> (<variant>)"},
    {"sidex_Variant_New_Table",  (PyCFunction)_sidex_Variant_New_Table, METH_NOARGS,"sidex_Variant_New_Table() -> (<variant>)"},
    {"sidex_Variant_New_DictBySize", _sidex_Variant_New_DictBySize, METH_VARARGS,"sidex_Variant_New_DictBySize(<value>) -> (<variant>)"},
    {"sidex_Variant_GetType",  _sidex_Variant_GetType, METH_VARARGS,"sidex_Variant_GetType(<variant>) -> <type>"},
    {"sidex_Variant_Copy",  _sidex_Variant_Copy, METH_VARARGS,"sidex_Variant_Copy(<variant>) -> <variant>"},
    {"sidex_Variant_None_Check",  _sidex_Variant_None_Check, METH_VARARGS,"sidex_Variant_None_Check(<variant>) -> True,False"},
    {"sidex_Variant_Boolean_Check",  _sidex_Variant_Boolean_Check, METH_VARARGS,"sidex_Variant_Boolean_Check(<variant>) -> True,False"},
    {"sidex_Variant_Integer_Check",  _sidex_Variant_Integer_Check, METH_VARARGS,"sidex_Variant_Integer_Check(<variant>) -> True,False"},
    {"sidex_Variant_Float_Check",  _sidex_Variant_Float_Check, METH_VARARGS,"sidex_Variant_Float_Check(<variant>) -> True,False"},
    {"sidex_Variant_DateTime_Check",  _sidex_Variant_DateTime_Check, METH_VARARGS,"sidex_Variant_DateTime_Check(<variant>) -> True,False"},
    {"sidex_Variant_Binary_Check",  _sidex_Variant_Binary_Check, METH_VARARGS,"sidex_Variant_Binary_Check(<variant>) -> True,False"},
    {"sidex_Variant_String_Check",  _sidex_Variant_String_Check, METH_VARARGS,"sidex_Variant_String_Check(<variant>) -> True,False"},
    {"sidex_Variant_As_Boolean",  _sidex_Variant_As_Boolean, METH_VARARGS,"sidex_Variant_As_Boolean(<variant>) -> <value>"},
    {"sidex_Variant_As_Integer",  _sidex_Variant_As_Integer, METH_VARARGS,"sidex_Variant_As_Integer(<variant>) -> <value>"},
    {"sidex_Variant_As_Float",  _sidex_Variant_As_Float, METH_VARARGS,"sidex_Variant_As_Float(<variant>) -> <value>"},
    {"sidex_Variant_As_DateTime",  _sidex_Variant_As_DateTime, METH_VARARGS,"sidex_Variant_As_DateTime(<variant>) -> <value>"},
    {"sidex_Variant_As_String_Length",  _sidex_Variant_As_String_Length, METH_VARARGS,"sidex_Variant_As_String_Length(<variant>) -> <length>"},
    {"sidex_Variant_As_String",  _sidex_Variant_As_String, METH_VARARGS,"sidex_Variant_As_String(<variant>) -> <value>"},
    {"sidex_Variant_As_Binary_Length",  _sidex_Variant_As_Binary_Length, METH_VARARGS,"sidex_Variant_As_Binary_Length(<variant>) -> <length>"},
    {"sidex_Variant_As_Binary",  _sidex_Variant_As_Binary, METH_VARARGS,"sidex_Variant_As_Binary(<variant>) -> <binary>"},
    {"sidex_Variant_String_GetFormat",  _sidex_Variant_String_GetFormat, METH_VARARGS,"sidex_Variant_String_GetFormat(<variant>) -> <value>"},
    {"sidex_Variant_String_SetFormat",  _sidex_Variant_String_SetFormat, METH_VARARGS,"sidex_Variant_String_SetFormat(<variant>,<format>)"},


    // <-- list methods
    {"sidex_Variant_List_Check", _sidex_Variant_List_Check, METH_VARARGS,"sidex_Variant_List_Check(<list>) -> true,false"},
    {"sidex_Variant_List_Size", _sidex_Variant_List_Size, METH_VARARGS,"sidex_Variant_List_Size(<list>) -> <size>"},
    {"sidex_Variant_List_Append", _sidex_Variant_List_Append, METH_VARARGS,"sidex_Variant_List_Append(<list>,<value>) -> position"},
    {"sidex_Variant_List_Insert", _sidex_Variant_List_Insert, METH_VARARGS,"sidex_Variant_List_Insert(<list>,<value>,<position>)"},
    {"sidex_Variant_List_Set", _sidex_Variant_List_Set, METH_VARARGS,"sidex_Variant_List_Set(<list>,<value>,<position>)"},
    {"sidex_Variant_List_DeleteItem", _sidex_Variant_List_DeleteItem, METH_VARARGS,"sidex_Variant_List_DeleteItem(<list>,<position>)"},
    {"sidex_Variant_List_Clear", _sidex_Variant_List_Clear, METH_VARARGS,"sidex_Variant_List_Clear(<list>)"},
    {"sidex_Variant_List_Get", _sidex_Variant_List_Get, METH_VARARGS,"sidex_Variant_List_Get(<list>,<position>) -> <value>"},

    // <-- dict methods
    {"sidex_Variant_Dict_Check", _sidex_Variant_Dict_Check, METH_VARARGS,"sidex_Variant_Dict_Check(<dict>) -> true, false"},
    {"sidex_Variant_Dict_Set", _sidex_Variant_Dict_Set, METH_VARARGS,"sidex_Variant_Dict_Set(<dict>,<key>,<value>)"},
    {"sidex_Variant_Dict_Size", _sidex_Variant_Dict_Size, METH_VARARGS,"sidex_Variant_Dict_Size(<dict>) -> <size>"},
    {"sidex_Variant_Dict_Keys", _sidex_Variant_Dict_Keys, METH_VARARGS,"sidex_Variant_Dict_Keys(<dict>) -> <keys>"},
    {"sidex_Variant_Dict_First", _sidex_Variant_Dict_First, METH_VARARGS,"sidex_Variant_Dict_First(<dict>)"},
    {"sidex_Variant_Dict_Next", _sidex_Variant_Dict_Next, METH_VARARGS,"sidex_Variant_Dict_Next(<dict>) -> <value>"},
    {"sidex_Variant_Dict_HasKey", _sidex_Variant_Dict_HasKey, METH_VARARGS,"sidex_Variant_Dict_HasKey(<dict>,<key>) -> True,False"},
    {"sidex_Variant_Dict_Delete", _sidex_Variant_Dict_Delete, METH_VARARGS,"sidex_Variant_Dict_Delete(<dict>,<key>)"},
    {"sidex_Variant_Dict_Clear", _sidex_Variant_Dict_Clear, METH_VARARGS,"sidex_Variant_Dict_Clear(<dict>)"},
    {"sidex_Variant_Dict_Get", _sidex_Variant_Dict_Get, METH_VARARGS,"sidex_Variant_Dict_Get(<dict>,<key>) -> <value>"},

    // <-- table methods
    {"sidex_Variant_Table_Check", _sidex_Variant_Table_Check, METH_VARARGS,"sidex_Variant_Table_Check(<table>) -> true, false"},
    {"sidex_Variant_Table_Rows", _sidex_Variant_Table_Rows, METH_VARARGS,"sidex_Variant_Table_Rows(<table>) -> <nRows>"},
    {"sidex_Variant_Table_Columns", _sidex_Variant_Table_Columns, METH_VARARGS,"sidex_Variant_Table_Columns(<table>) -> <nColumns>"},
    {"sidex_Variant_Table_ColumnNames", _sidex_Variant_Table_ColumnNames, METH_VARARGS,"sidex_Variant_Table_ColumnNames(<table>) -> <columns>"},
    {"sidex_Variant_Table_HasColumn", _sidex_Variant_Table_HasColumn, METH_VARARGS,"sidex_Variant_Table_HasColumn(<table>,<colName>) -> True,False"},
    {"sidex_Variant_Table_AddColumn", _sidex_Variant_Table_AddColumn, METH_VARARGS,"sidex_Variant_Table_AddColumn(<table>,<colName>)"},
    {"sidex_Variant_Table_DeleteColumn", _sidex_Variant_Table_DeleteColumn, METH_VARARGS,"sidex_Variant_Table_DeleteColumn(<table>,<colName>)"},
    {"sidex_Variant_Table_GetColumnName", _sidex_Variant_Table_GetColumnName, METH_VARARGS,"sidex_Variant_Table_GetColumnName(<table>,<colIndex>) -> <colName>"},
    {"sidex_Variant_Table_AddRow", _sidex_Variant_Table_AddRow, METH_VARARGS,"sidex_Variant_Table_AddRow(<table>) -> <rowIndex>"},
    {"sidex_Variant_Table_DeleteRow", _sidex_Variant_Table_DeleteRow, METH_VARARGS,"sidex_Variant_Table_DeleteRow(<table>,<iRowIdex>)"},
    {"sidex_Variant_Table_DeleteRows", _sidex_Variant_Table_DeleteRows, METH_VARARGS,"sidex_Variant_Table_DeleteRows(<table>)"},
    {"sidex_Variant_Table_GetRow", _sidex_Variant_Table_GetRow, METH_VARARGS,"sidex_Variant_Table_GetRow(<table>,<rowIndex>) -> <row>"},
    {"sidex_Variant_Table_GetField", _sidex_Variant_Table_GetField, METH_VARARGS,"sidex_Variant_Table_GetField(<table>,<rowIndex>,<colName>) -> <value>"},
    {"sidex_Variant_Table_SetField", _sidex_Variant_Table_SetField, METH_VARARGS,"sidex_Variant_Table_SetField(<table>,<rowIndex>,<colName>,<value>)"},

    // <-- convert methods
    {"PyObjectAsSidexVariant",  _PyObjectAsSidexVariant, METH_VARARGS,"PyObjectAsSidexVariant(<pyObject>)"},
    {"SidexVariantAsPyObject",  _SidexVariantAsPyObject, METH_VARARGS,"SidexVariantAsPyObject(<pyObject>)"},

    {NULL, NULL, 0, NULL}        /* Sentinel */
};

/*
 *************************************************************************************
 *                                  module init 
 *************************************************************************************
 */

struct module_state {
    PyObject *error;
};

#if PY_MAJOR_VERSION >= 3
#define GETSTATE(m) ((struct module_state*)PyModule_GetState(m))
#else
#define GETSTATE(m) (&_state)
static struct module_state _state;
#endif

#if PY_MAJOR_VERSION >= 3

static int _sidex_traverse(PyObject *m, visitproc visit, void *arg) {
    Py_VISIT(GETSTATE(m)->error);
    return 0;
}

static int _py3sidex_clear(PyObject *m) {
    Py_CLEAR(GETSTATE(m)->error);
    return 0;
}

static struct PyModuleDef moduledef = {
        PyModuleDef_HEAD_INIT,
        "_sidex",
        NULL,
        sizeof(struct module_state),
        _sidex_methods,
        NULL,
        _sidex_traverse,
        _py3sidex_clear,
        NULL
};
#define INITERROR return NULL
PyMODINIT_FUNC PyInit__sidex(void)

#else
#define INITERROR return
PyMODINIT_FUNC init_sidex(void)
#endif
/*
 *------------------------------------------------------
 *               module init function
 *------------------------------------------------------
 */
{
    PyObject *m;
    
#if PY_MAJOR_VERSION >= 3
    m = PyModule_Create(&moduledef);
#else
    m = Py_InitModule("_sidex", _sidex_methods);
#endif
    if (m == NULL)
        INITERROR;

    // create and register exception
    sidexError = PyErr_NewException("sidex.error", NULL, NULL);
    Py_INCREF(sidexError);
    PyModule_AddObject(m, "sidexError", sidexError);

    sidexIndexError = PyErr_NewException("sidex.error", NULL, NULL);
    Py_INCREF(sidexIndexError);
    PyModule_AddObject(m, "sidexIndexError", sidexIndexError);

    // init DateTimeAPI:
    initDateTimeAPI();

#if PY_MAJOR_VERSION >= 3
    return m;
#endif
}
