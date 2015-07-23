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

#ifndef LINUX
#endif
#include <wchar.h>
#include <sidex.h>
#include <Python.h>

/*
 *------------------------------------------------------
 *  In case of Python 2.7 there is a special behaviour for
 * strings on LINUX & OSX platforms
 *------------------------------------------------------
 */
#ifdef LINUX
  #if PY_MAJOR_VERSION < 3
    #if PY_MINOR_VERSION == 7
        #define LINUX_X64
    #endif // PY_MINOR_VERSION
  #endif // PY_MAJOR_VERSION
#endif //LINUX

/*
 *------------------------------------------------------
 *               Helper method
 *------------------------------------------------------
 */
PyObject* utf32_utf16_decode(SIDEX_TCHAR* seq, int sLength) {
  PyObject* result;

  #ifdef LINUX_X64
    if (0 > sLength){
      sLength = (SIDEX_INT32)wcslen(seq);
    }
    result = PyUnicode_FromWideChar(seq, sLength);
  #else //LINUX_X64
    result = Py_BuildValue("u", seq);
  #endif //LINUX_X64
  return result;
}

/*
 *------------------------------------------------------
 *               Helper method
 *------------------------------------------------------
 */
void unicodeFree(PyObject* unicodeObj, SIDEX_TCHAR* sUnicode){
  if (NULL!= unicodeObj){
    //printf ("unicodeFree / NEED PY_DECREF \n");
    Py_XDECREF(unicodeObj);
  }
#ifdef LINUX_X64
  if (NULL != sUnicode)
  //printf ("unicodeFree / Delete UTF32\n");
    delete (sUnicode);
#endif //LINUX_X64
}

/*
 *------------------------------------------------------
 *               Helper method
 *------------------------------------------------------
 */
bool utf16_utf32_decode(PyUnicodeObject* py_UObj, Py_ssize_t sLength, wchar_t** sRetVal) {
  bool      bFail = false;
  wchar_t*  sRet  = NULL;

  #ifdef LINUX_X64
    sRet = new wchar_t[sLength+1];
#if PY_MAJOR_VERSION < 3
    Py_ssize_t lRet = PyUnicode_AsWideChar(py_UObj, sRet, sLength+1);
#else // PY_MAJOR_VERSION
  #if PY_MINOR_VERSION < 2
    Py_ssize_t lRet = PyUnicode_AsWideChar(py_UObj, sRet, sLength+1);
  #else // PY_MINOR_VERSION
    Py_ssize_t lRet = PyUnicode_AsWideChar((PyObject*)py_UObj, sRet, sLength+1);
  #endif // PY_MINOR_VERSION
#endif // PY_MAJOR_VERSION
    if (lRet != sLength){
      bFail = true;
      delete sRet;
      sRet = NULL;
    }
  #else //LINUX_X64
    int iLength;
    PyObject* tuple = Py_BuildValue("(O)", py_UObj);
    if (!PyArg_ParseTuple(tuple, "u#", &sRet, &iLength)){
      bFail = true;
      sRet = NULL;
    }
    Py_XDECREF(tuple);
  #endif //LINUX_X64
  if (!bFail)
    *sRetVal = sRet;
  return bFail;
}

/*
 *------------------------------------------------------
 *               Helper method
 *------------------------------------------------------
 */
bool _unicodeConversion(PyObject* py_Obj, SIDEX_TCHAR** u_ret, PyObject** py_ObjRes, PyObject* errOutputObj = NULL)
{
  bool       bFail = false;
  PyObject*  py_Obj16 = NULL;
  int        length;

  *py_ObjRes = NULL;

  bool bUnicode = true;
  bUnicode = (true == PyUnicode_Check(py_Obj));
#if PY_MAJOR_VERSION < 3 // On Python 3 we only have Unicode:
  if (!bUnicode){
    if (false == PyString_Check(py_Obj)){
      // Not unicode and not a string:
      if (NULL != errOutputObj){
        PyErr_Format(errOutputObj, "Unicode / String parameter expected");
      }
      return true;
    }
  }
#else
  if (!bUnicode){
    // Not unicode:
    if (NULL != errOutputObj){
      PyErr_Format(errOutputObj, "Unicode parameter expected");
    }
    return true;
  }
#endif
  if (!bUnicode){
    ///////////////////////////////////////
    // The char sting case:
    char* s_ret;
    PyObject* tuple = Py_BuildValue("(O)", py_Obj);
    if (!PyArg_ParseTuple(tuple, "s#", &s_ret, &length))
      bFail = true;
    Py_XDECREF(tuple);
    ////////////////////////////////////////////
    // First generate 16- Bit Unicode:
    if (!bFail){
#if PY_MAJOR_VERSION < 3 // On Python 3 we only have Unicode:
  #if PY_MINOR_VERSION < 6
        PyObject* py_strObj = PyString_FromStringAndSize((const char*) s_ret, length);
        py_Obj16 = PyUnicode_FromObject(py_strObj);
        Py_XDECREF(py_strObj);
  #else
        py_Obj16 = PyUnicode_FromStringAndSize((const char*) s_ret, length);
  #endif
#else
      py_Obj16 = PyUnicode_FromStringAndSize((const char*) s_ret, length);
#endif
      *py_ObjRes = py_Obj16;
    }
  }
  else{
    py_Obj16 = py_Obj;
    length = (int)PyUnicode_GET_SIZE(py_Obj16);
  }
  ////////////////////////////////////////////
  // Maybe we run on a LINUX- x64 /generate 16- Bit Unicode:
  if (!bFail){
    bFail =  utf16_utf32_decode((PyUnicodeObject*)py_Obj16, (Py_ssize_t)length, u_ret);
    //fwprintf(stderr, L"---   %ls ---\n", *u_ret);
  }

  return bFail;
}

