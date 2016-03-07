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
#include <Windows.h>
#endif
#include <wchar.h>
#include <sidex.h>
#include <Python.h>
#include <datetime.h>
#include "_py_unicode.h"

bool bLog = false;

/*
 *------------------------------------------------------
 *               _sidex_Variant_New_NoneS
 *------------------------------------------------------
 */
SIDEX_VARIANT _sidex_Variant_New_NoneS(SIDEX_INT32* err)
{
    SIDEX_VARIANT svariant = SIDEX_HANDLE_TYPE_NULL;

    *err = SIDEX_SUCCESS;

    Py_BEGIN_ALLOW_THREADS;
    try {
        svariant = sidex_Variant_New_None();
    }
    catch (...) {
        *err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    return svariant;
}

/*
 *------------------------------------------------------
 *               _sidex_Variant_New_BooleanS
 *------------------------------------------------------
 */
SIDEX_VARIANT _sidex_Variant_New_BooleanS(PyObject *args, SIDEX_INT32* err)
{
    SIDEX_VARIANT svariant = SIDEX_HANDLE_TYPE_NULL;
    PyObject *    pvalue;

    *err = SIDEX_SUCCESS;

    if (!PyArg_ParseTuple(args, "O", &pvalue))
        return svariant;

    Py_BEGIN_ALLOW_THREADS;
    try {
        if (PyObject_IsTrue(pvalue))
            svariant = sidex_Variant_New_Boolean(SIDEX_TRUE);
        else
            svariant = sidex_Variant_New_Boolean(SIDEX_FALSE);
    }
    catch (...) {
        *err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    return svariant;
}

/*
 *------------------------------------------------------
 *               sidex_Variant_New_Binary
 *------------------------------------------------------
 */
SIDEX_VARIANT _sidex_Variant_New_BinaryS(PyObject *args, SIDEX_INT32* err)
{
    SIDEX_VARIANT svariant = SIDEX_HANDLE_TYPE_NULL;
    char   *pvalue;
    SIDEX_INT32 plength;

    *err = SIDEX_SUCCESS;

    if (!PyArg_ParseTuple(args, "s#", &pvalue, &plength))
        return svariant;

    Py_BEGIN_ALLOW_THREADS;
    try {
        *err = sidex_Variant_New_Binary((unsigned char*)pvalue, plength, &svariant);
    }
    catch (...) {
        *err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    return svariant;
}

/*
 *------------------------------------------------------
 *               _sidex_Variant_New_FloatS
 *------------------------------------------------------
 */
SIDEX_VARIANT _sidex_Variant_New_FloatS(PyObject *args, SIDEX_INT32* err)
{
    SIDEX_VARIANT svariant = SIDEX_HANDLE_TYPE_NULL;
    SIDEX_DOUBLE  pvalue;

    *err = SIDEX_SUCCESS;

    if (!PyArg_ParseTuple(args, "d", &pvalue))
        return svariant;

    Py_BEGIN_ALLOW_THREADS;
    try {
        svariant = sidex_Variant_New_Float(pvalue);
    }
    catch (...) {
        *err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    return svariant;
}

/*
 *------------------------------------------------------
 *               _sidex_Variant_New_StringS
 *------------------------------------------------------
 */
SIDEX_VARIANT _sidex_Variant_New_StringS(PyObject *args, SIDEX_INT32* err)
{
    SIDEX_VARIANT svariant = SIDEX_HANDLE_TYPE_NULL;
    char*   pvalue;

    *err = SIDEX_SUCCESS;

    if (!PyArg_ParseTuple(args, "s", &pvalue))
        return svariant;

    Py_BEGIN_ALLOW_THREADS;
    try {
        *err = sidex_Variant_New_String_A(pvalue, &svariant);
    }
    catch (...) {
        *err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    return svariant;
}

/*
 *------------------------------------------------------
 *               _sidex_Variant_New_UnicodeStringS
 *------------------------------------------------------
 */
SIDEX_VARIANT _sidex_Variant_New_UnicodeStringS(PyObject *args, SIDEX_INT32* err, PyObject* errOutputObj)
{
    SIDEX_VARIANT svariant = SIDEX_HANDLE_TYPE_NULL;
    PyObject*    py_pvalue16;
    SIDEX_TCHAR* pvalue;
    PyObject*    py_pvalue;

    *err = SIDEX_SUCCESS;
    if (!PyArg_ParseTuple(args, "O", &py_pvalue))
        return svariant;
    if ( _unicodeConversion(py_pvalue, &pvalue, &py_pvalue16, errOutputObj) ) return svariant;

    Py_BEGIN_ALLOW_THREADS;
    try {
        *err = sidex_Variant_New_String(pvalue, &svariant);
    }
    catch (...) {
        *err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    unicodeFree(py_pvalue16, pvalue);

    return svariant;
}

/*
 *------------------------------------------------------
 *               _sidex_Variant_New_IntegerS
 *------------------------------------------------------
 */
SIDEX_VARIANT _sidex_Variant_New_IntegerS(PyObject *args, SIDEX_INT32* err)
{
    SIDEX_VARIANT svariant = SIDEX_HANDLE_TYPE_NULL;
    SIDEX_INT64   pvalue;

    *err = SIDEX_SUCCESS;

    if (!PyArg_ParseTuple(args, "K", &pvalue))
        return svariant;

    Py_BEGIN_ALLOW_THREADS;
    try {
        svariant = sidex_Variant_New_Integer(pvalue);
    }
    catch (...) {
        *err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    return svariant;
}

/*
 *------------------------------------------------------
 *               sidex_Variant_New_DateTimeS
 *------------------------------------------------------
 */
SIDEX_VARIANT _sidex_Variant_New_DateTimeS(PyObject *args, SIDEX_INT32* err, PyObject* errOutputObj)
{
    SIDEX_VARIANT svariant = SIDEX_HANDLE_TYPE_NULL;

    *err = SIDEX_SUCCESS;

    PyObject* value;
    if (!PyArg_ParseTuple(args, "O", &value))
        return svariant;

    SIDEX_TCHAR dateTimeStringTemp[64];

    if (true == PyDate_Check(value)){
      int year         = PyDateTime_GET_YEAR(value);
      int month        = PyDateTime_GET_MONTH(value);
      int day          = PyDateTime_GET_DAY(value);
      int hours        = PyDateTime_DATE_GET_HOUR(value);
      int minutes      = PyDateTime_DATE_GET_MINUTE(value);
      int seconds      = PyDateTime_DATE_GET_SECOND(value);
      int milliseconds = PyDateTime_DATE_GET_MICROSECOND(value);

      #ifdef LINUX
        swprintf (dateTimeStringTemp, 64, L"%04d-%02d-%02d %02d:%02d:%02d:%03d", year, month, day, hours, minutes, seconds, milliseconds);
      #else // LINUX
        swprintf_s (dateTimeStringTemp, 64, L"%04d-%02d-%02d %02d:%02d:%02d:%03d", year, month, day, hours, minutes, seconds, milliseconds);
      #endif // LINUX
    }
    else{
      SIDEX_INT64 year64         = 0;// Umgehung von Speicherzugriffsverletzung bei Python 3.4 unter LINUX x64
      SIDEX_INT64 month64        = 0;// Umgehung von Speicherzugriffsverletzung bei Python 3.4 unter LINUX x64
      SIDEX_INT64 day64          = 0;// Umgehung von Speicherzugriffsverletzung bei Python 3.4 unter LINUX x64
      SIDEX_INT64 hours64        = 0;// Umgehung von Speicherzugriffsverletzung bei Python 3.4 unter LINUX x64
      SIDEX_INT64 minutes64      = 0;// Umgehung von Speicherzugriffsverletzung bei Python 3.4 unter LINUX x64
      SIDEX_INT64 seconds64      = 0;// Umgehung von Speicherzugriffsverletzung bei Python 3.4 unter LINUX x64
      SIDEX_INT64 milliseconds64 = 0;// Umgehung von Speicherzugriffsverletzung bei Python 3.4 unter LINUX x64
      SIDEX_INT64 tuple7         = 0;// Umgehung von Speicherzugriffsverletzung bei Python 3.4 unter LINUX x64
      SIDEX_INT64 tuple8         = 0;// Umgehung von Speicherzugriffsverletzung bei Python 3.4 unter LINUX x64
      SIDEX_INT64 tuple9         = 0;// Umgehung von Speicherzugriffsverletzung bei Python 3.4 unter LINUX x64

      if (!PyArg_ParseTuple(value, "iiiiiiiii", &year64, &month64, &day64, &hours64, &minutes64, &seconds64, &tuple7, &tuple8, &tuple9)){
          return svariant;
      }
      #ifdef LINUX
        swprintf (dateTimeStringTemp, 64, L"%04lld-%02lld-%02lld %02lld:%02lld:%02lld:%03lld", year64, month64, day64, hours64, minutes64, seconds64, milliseconds64);
      #else // LINUX
        swprintf_s (dateTimeStringTemp, 64, L"%04lld-%02lld-%02lld %02lld:%02lld:%02lld:%03lld", year64, month64, day64, hours64, minutes64, seconds64, milliseconds64);
      #endif // LINUX
    }
    Py_BEGIN_ALLOW_THREADS;
    try {
        *err = sidex_Variant_New_DateTime(dateTimeStringTemp, &svariant);
    }
    catch (...) {
        *err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    return svariant;
}

/*
 *------------------------------------------------------
 *               _sidex_Variant_New_ListS
 *------------------------------------------------------
 */
SIDEX_VARIANT _sidex_Variant_New_ListS(SIDEX_INT32* err)
{
    SIDEX_VARIANT svariant = SIDEX_HANDLE_TYPE_NULL;
    *err = SIDEX_SUCCESS;

    Py_BEGIN_ALLOW_THREADS;
    try {
        svariant = sidex_Variant_New_List();
    }
    catch (...) {
        *err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    return svariant;
}

/*
 *------------------------------------------------------
 *               sidex_Variant_New_DictS
 *------------------------------------------------------
 */
SIDEX_VARIANT _sidex_Variant_New_DictS(SIDEX_INT32* err)
{
    SIDEX_VARIANT svariant = SIDEX_HANDLE_TYPE_NULL;
    *err = SIDEX_SUCCESS;

    Py_BEGIN_ALLOW_THREADS;
    try {
        svariant = sidex_Variant_New_Dict();
    }
    catch (...) {
        *err = SIDEX_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    return svariant;
}

/*
 *------------------------------------------------------
 *               _PyObjectAsSidexVariantS
 *------------------------------------------------------
 */
SIDEX_VARIANT _PyObjectAsSidexVariantS(PyObject *pValue, SIDEX_INT32* err, PyObject* errOutputObj) {
    SIDEX_VARIANT retValue = SIDEX_HANDLE_TYPE_NULL;
    
    *err = SIDEX_SUCCESS;
    if (bLog)printf ("## _PyObjectAsSidexVariantS ##\n");

    if (Py_None == pValue){
      retValue = _sidex_Variant_New_NoneS(err);
      if (bLog)printf ("## None ##\n");
    }
#if PY_MAJOR_VERSION == 2
  #if PY_MINOR_VERSION >= 6
    else{ if (true == PyBytes_Check(pValue)){
      PyObject* tuple = Py_BuildValue("(O)",pValue);
      retValue = _sidex_Variant_New_BinaryS(tuple, err);
      Py_XDECREF(tuple);
      if (bLog)printf ("## A Binary ##\n");
    }
  #endif
#endif
#if PY_MAJOR_VERSION > 2
    else{ if (true == PyBytes_Check(pValue)){
      PyObject* tuple = Py_BuildValue("(O)",pValue);
      retValue = _sidex_Variant_New_BinaryS(tuple, err);
      Py_XDECREF(tuple);
      if (bLog)printf ("## A Binary ##\n");
    }
#endif
    else{ if (true == PyBool_Check(pValue)){
      PyObject* tuple = Py_BuildValue("(O)",pValue);
      retValue = _sidex_Variant_New_BooleanS(tuple, err);
      Py_XDECREF(tuple);
      if (bLog)printf ("## A Boolean ##\n");
    }
    else{ if (true == PyFloat_Check(pValue)){
      PyObject* tuple = Py_BuildValue("(O)",pValue);
      retValue = _sidex_Variant_New_FloatS(tuple, err);
      Py_XDECREF(tuple);
      if (bLog)printf ("## A Float ##\n");
    }
#if PY_MAJOR_VERSION < 3 // On Python 3 we only have Unicode:
    else { if (true == PyString_Check(pValue)){
      PyObject* tuple = Py_BuildValue("(O)",pValue);
      retValue = _sidex_Variant_New_StringS(tuple, err);
      Py_XDECREF(tuple);
      if (bLog)printf ("## A String ##\n");
    }
#endif
    else { if (true == PyUnicode_Check(pValue)){
      PyObject* tuple = Py_BuildValue("(O)",pValue);
      retValue = _sidex_Variant_New_UnicodeStringS(tuple, err, errOutputObj);
      Py_XDECREF(tuple);
      if (bLog)printf ("## A Unicode String ##\n");
    }
#if PY_MAJOR_VERSION < 3 // On Python 3 we only have Long:
    else{ if (true == PyInt_Check(pValue)){
      PyObject* tuple = Py_BuildValue("(O)",pValue);
      retValue = _sidex_Variant_New_IntegerS(tuple, err);
      Py_XDECREF(tuple);
      if (bLog)printf ("## An Integer ##\n");
    }
#endif
    else{ if (true == PyLong_Check(pValue)){
      PyObject* tuple = Py_BuildValue("(O)",pValue);
      retValue = _sidex_Variant_New_IntegerS(tuple, err);
      Py_XDECREF(tuple);
      if (bLog)printf ("## A Long ##\n");
    }
    else{ if (true == PyList_Check(pValue)){
      retValue = _sidex_Variant_New_ListS(err);
      ssize_t iListSize = PyList_Size(pValue);
      for (ssize_t i = 0; (i < iListSize) && (SIDEX_SUCCESS == *err); ++i){
        PyObject* item = PyList_GetItem(pValue, i);
        SIDEX_VARIANT vItem = _PyObjectAsSidexVariantS(item, err, errOutputObj);
        if (SIDEX_SUCCESS == *err && SIDEX_HANDLE_TYPE_NULL != vItem){
          SIDEX_INT32 iPos;
          *err = sidex_Variant_List_Append(retValue, vItem, &iPos);
          sidex_Variant_DecRef(vItem);
        }
      }
      if (bLog)printf ("## A List ##\n");
    }
    else{ if (true == PyDict_Check(pValue)){
      retValue = _sidex_Variant_New_DictS(err);
      ssize_t iDictSize = PyDict_Size(pValue);
      PyObject* pyKeys = PyDict_Keys(pValue);
      for (ssize_t i = 0; (i < iDictSize) && (SIDEX_SUCCESS == *err); ++i){
        PyObject* pKey = PyList_GetItem(pyKeys, i);
        PyObject* item = PyDict_GetItem(pValue, pKey);
        if (true == PyUnicode_Check(pKey)){
          //// UNICODE :
          PyObject*    py_sKey16;
          SIDEX_TCHAR* sKey;
          PyObject*    py_sKey;
          PyObject* tuple = Py_BuildValue("(O)",pKey);
          if (!PyArg_ParseTuple(tuple, "O", &py_sKey)){
            *err = SIDEX_ERR_COMMON;
          }
          else{
            SIDEX_VARIANT vItem = _PyObjectAsSidexVariantS(item, err, errOutputObj);
            if (SIDEX_SUCCESS == *err && SIDEX_HANDLE_TYPE_NULL != vItem){
              if ( _unicodeConversion(py_sKey, &sKey, &py_sKey16, errOutputObj) ) return SIDEX_HANDLE_TYPE_NULL;
              *err = sidex_Variant_Dict_Set(retValue, sKey, vItem);
              sidex_Variant_DecRef(vItem);
              unicodeFree(py_sKey16, sKey);
            }
          }
          Py_XDECREF(tuple);
        }
#if PY_MAJOR_VERSION < 3
        else if (true == PyString_Check(pKey)){
          //// String :
          char*  sKey;
          PyObject* tuple = Py_BuildValue("(O)",pKey);
          if (!PyArg_ParseTuple(tuple, "s", &sKey)){
            *err = SIDEX_ERR_COMMON;
          }
          else{
            SIDEX_VARIANT vItem = _PyObjectAsSidexVariantS(item, err, errOutputObj);
            if (SIDEX_SUCCESS == *err && SIDEX_HANDLE_TYPE_NULL != vItem){
              *err = sidex_Variant_Dict_Set_A(retValue, sKey, vItem);
              sidex_Variant_DecRef(vItem);
            }
          }
          Py_XDECREF(tuple);
        }
#endif
        else {
          *err = SIDEX_ERR_COMMON;
        }
      }
      Py_XDECREF(pyKeys);
      if (bLog)printf ("## A Dict ##\n");
    }
    else{ if (true == PyDate_Check(pValue)){
      PyObject* tuple = Py_BuildValue("(O)", pValue);
      retValue = _sidex_Variant_New_DateTimeS(tuple, err, errOutputObj);
      Py_XDECREF(tuple);
      if (bLog)printf ("## A Date Time ##\n");
    }
    else{ if (true == PyTuple_Check(pValue)){
      PyObject* tuple = Py_BuildValue("(O)", pValue);
      retValue = _sidex_Variant_New_DateTimeS(tuple, err, errOutputObj);
      Py_XDECREF(tuple);
      if (bLog)printf ("## A Tuple / Date Time ##\n");
    }
    else{
      printf ("## Type Error ##\n");
      *err = SIDEX_ERR_COMMON;
    }
    }
    }
    }
    }
    }
#if PY_MAJOR_VERSION < 3
    }
#endif
    }
#if PY_MAJOR_VERSION < 3
    }
#endif
    }
    }
#if PY_MAJOR_VERSION > 2
    }
#endif
#if PY_MAJOR_VERSION == 2
  #if PY_MINOR_VERSION >= 6
    }
  #endif
#endif

    if (SIDEX_SUCCESS != *err && SIDEX_HANDLE_TYPE_NULL != retValue){
      sidex_Variant_DecRef(retValue);
      retValue = SIDEX_HANDLE_TYPE_NULL;
    }
    return retValue;
}

///////////////////////////////////////////////////////////////////////////
// See http://en.wikipedia.org/wiki/Determination_of_the_day_of_the_week
// Zeller's algorithm to calc day of week
int DayOfWeek (int day, int month, int year){

  int PADDING[7] = {5, 6, 0, 1, 2, 3, 4};

  if (1 == month){
    year -= 1;
    month = 13;
  }
  if (2 == month){
    year -= 1;
    month = 14;
  }
  int y = year % 100;
  int c = year / 100;
  int d = day;
  int m = month;

  int iVal1 = 13 * (m+1);
  int iVal2 = iVal1 / 5;
  int iVal3 = y / 4;
  int iVal4 = c / 4;

  int dayOfWeek = (d + iVal2 + y + iVal3 + iVal4 - 2*c) % 7;

  //printf ("dayOfWeek = %d\n", PADDING[dayOfWeek]);
  return PADDING[dayOfWeek];
}

///////////////////////////////////////////////////////////////////////////
// Calc day of week
int DayOfYear (int day, int month, int year){

  int PADDING[12] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};

  int leap = 0;
  if (month >= 3){  // the leap year affects only march and higher (in this function)
    if (0 == (year % 4)){
      if (0 == (year % 100)){
        if (0 == (year % 400)){
          leap = 1;
        }
      }
    }
  }
  //printf ("leap = %d\n", leap);
  int dayOfYear = PADDING[month-1] + day + leap;
  //printf ("dayOfYear = %d\n", dayOfYear);
  return dayOfYear;
}

PyObject*_SidexVariantDecode(SIDEX_VARIANT sVariant, SIDEX_INT32* err){
  PyObject* retValue = NULL;
    
  *err = SIDEX_SUCCESS;
  if (bLog)printf ("######### _SidexVariantDecode #########\n");
  SIDEX_DATA_TYPE type = sidex_Variant_GetType (sVariant);
  const char* sRet;
  switch (type){
    case SIDEX_DATA_TYPE_NONE:
                          sRet = "TYPE_NONE";
                          Py_INCREF(Py_None);
                          retValue = Py_None;
                          break;
    case SIDEX_DATA_TYPE_BOOLEAN:
                          SIDEX_BOOL bValue;
                          *err = sidex_Variant_As_Boolean (sVariant, &bValue);
                          if (SIDEX_SUCCESS == *err){
                            if (SIDEX_TRUE == bValue){
                              Py_INCREF(Py_True);
                              retValue = Py_True;
                            }
                            else{
                              Py_INCREF(Py_False);
                              retValue = Py_False;
                            }
                          }
                          sRet = "TYPE_BOOLEAN";
                          break;
    case SIDEX_DATA_TYPE_INTEGER:
                          SIDEX_INT64 iValue;
                          *err = sidex_Variant_As_Integer (sVariant, &iValue);
                          if (SIDEX_SUCCESS == *err){
                            retValue =  Py_BuildValue("L", iValue);
                          }
                          sRet = "TYPE_INTEGER";
                          break;
    case SIDEX_DATA_TYPE_FLOAT:
                          SIDEX_DOUBLE fValue;
                          *err = sidex_Variant_As_Float (sVariant, &fValue);
                          if (SIDEX_SUCCESS == *err){
                            retValue =  Py_BuildValue("d", fValue);
                          }
                          sRet = "TYPE_FLOAT";
                          break;
    case SIDEX_DATA_TYPE_DATETIME:
                          SIDEX_TCHAR* sDateTimeValue;
                          *err = sidex_Variant_As_DateTime (sVariant, &sDateTimeValue);
                          if (SIDEX_SUCCESS == *err){
                            int iYear;
                            int iMonth;
                            int iDay;
                            int iHour;
                            int iMinute;
                            int iSecond = 0;
                            int iMillisecond = 0;
                            int weekday = 0;
                            int yearday = 0;
                            int isDstTime   = -1;// Unknown
                            //int isDstTime   = 0;// W. Europe Standard Time - UTC
                            //int isDstTime = 1;// W. Europe Daylight Time - DST

                            #ifdef LINUX
                              swscanf(sDateTimeValue, L"%4d-%2d-%2d %2d:%2d:%2d:%3d", &iYear, &iMonth, &iDay, &iHour, &iMinute, &iSecond, &iMillisecond);
                            #else // LINUX
                              swscanf_s(sDateTimeValue, L"%4d-%2d-%2d %2d:%2d:%2d:%3d", &iYear, &iMonth, &iDay, &iHour, &iMinute, &iSecond, &iMillisecond);
                            #endif // LINUX


                            weekday = DayOfWeek (iDay, iMonth, iYear);
                            yearday = DayOfYear(iDay, iMonth, iYear);

                            retValue =  Py_BuildValue("(iiiiiiiii)", iYear, iMonth, iDay, iHour, iMinute, iSecond, weekday, yearday, isDstTime);
                          }
                          sRet = "TYPE_DATETIME";
                          break;
    case SIDEX_DATA_TYPE_STRING:
                          SIDEX_TCHAR* sValueTemp;
                          SIDEX_INT32 iLength;
                          *err = sidex_Variant_As_String (sVariant, &sValueTemp, &iLength);
                          if (SIDEX_SUCCESS == *err){
                            retValue = utf32_utf16_decode(sValueTemp, iLength);
                          }
                          sRet = "TYPE_STRING";
                          break;
#if PY_MAJOR_VERSION == 2
  #if PY_MINOR_VERSION >= 6
    case SIDEX_DATA_TYPE_BINARY:
                          unsigned char* sBinValueTemp;
                          SIDEX_INT32 iBinLength;
                          *err = sidex_Variant_As_Binary (sVariant, &sBinValueTemp, &iBinLength);
                          if (SIDEX_SUCCESS == *err){
                            retValue = PyBytes_FromStringAndSize((const char*)sBinValueTemp, iBinLength);
                          }
                          sRet = "TYPE_BINARY";
                          break;
  #endif
#endif
#if PY_MAJOR_VERSION > 2
    case SIDEX_DATA_TYPE_BINARY:
                          unsigned char* sBinValueTemp;
                          SIDEX_INT32 iBinLength;
                          *err = sidex_Variant_As_Binary (sVariant, &sBinValueTemp, &iBinLength);
                          if (SIDEX_SUCCESS == *err){
                            retValue = PyBytes_FromStringAndSize((const char*)sBinValueTemp, iBinLength);
                          }
                          sRet = "TYPE_BINARY";
                          break;
#endif
    case SIDEX_DATA_TYPE_LIST:
                          sRet = "TYPE_LIST";
                          SIDEX_INT32 iListSize;
                          *err = sidex_Variant_List_Size (sVariant, &iListSize);
                          if (SIDEX_SUCCESS == *err){
                            retValue = PyList_New(iListSize);
                            for (SIDEX_INT32 index = 0; index < iListSize && SIDEX_SUCCESS == *err; ++index){
                              SIDEX_VARIANT sListItem;
                              *err = sidex_Variant_List_Get (sVariant, index, &sListItem);
                              if (SIDEX_SUCCESS == *err){
                                PyObject* pyListItem = NULL;
                                pyListItem = _SidexVariantDecode(sListItem, err);
                                if (SIDEX_SUCCESS == *err){
                                  int iPyErr = PyList_SetItem(retValue, index, pyListItem);
                                  if (0 > iPyErr){
                                    *err = SIDEX_ERR_COMMON;
                                  }
                                }
                              }
                            }
                          }
                          break;
    case SIDEX_DATA_TYPE_DICT:
                          sRet = "TYPE_DICT";
                          SIDEX_INT32 iDictSize;
                          *err = sidex_Variant_Dict_Size (sVariant, &iDictSize);
                          if (SIDEX_SUCCESS == *err){
                            retValue = PyDict_New();
                            *err = sidex_Variant_Dict_First(sVariant);
                            for (SIDEX_INT32 index = 0; index < iDictSize && SIDEX_SUCCESS == *err; ++index){
                              SIDEX_TCHAR* sKeyTemp;
                              SIDEX_VARIANT sDictItem;
                              *err = sidex_Variant_Dict_Next(sVariant, &sKeyTemp, &sDictItem);
                              if (SIDEX_SUCCESS == *err){
                                PyObject* pyDictItem = NULL;
                                pyDictItem = _SidexVariantDecode(sDictItem, err);
                                if (SIDEX_SUCCESS == *err){
                                  PyObject* pyKeyValue = utf32_utf16_decode(sKeyTemp, -1);
                                  int iPyErr = PyDict_SetItem(retValue, pyKeyValue, pyDictItem);
                                  if (0 > iPyErr){
                                    *err = SIDEX_ERR_COMMON;
                                  }
                                  Py_XDECREF(pyKeyValue);
                                }
                              }
                            }
                          }
                          break;
    case SIDEX_DATA_TYPE_TABLE:
                          sRet = "TYPE_TABLE";
                          SIDEX_INT32 iColumns;
                          *err = sidex_Variant_Table_Columns (sVariant, &iColumns);
                          if (SIDEX_SUCCESS == *err){
                            SIDEX_INT32 iRows;
                            *err = sidex_Variant_Table_Rows (sVariant, &iRows);
                            retValue = PyList_New(iRows);

                            if (SIDEX_SUCCESS == *err && iRows && iColumns){
                              SIDEX_VARIANT sVarColumnNames;
                              *err = sidex_Variant_Table_ColumnNames (sVariant, &sVarColumnNames);
                              if (SIDEX_SUCCESS == *err){
                                for (SIDEX_INT32 rowIndex = 0; rowIndex < iRows && SIDEX_SUCCESS == *err; ++rowIndex){
                                  PyObject* pyRow = PyDict_New();
                                  for (SIDEX_INT32 columnIndex = 0; columnIndex < iColumns && SIDEX_SUCCESS == *err; ++columnIndex){
                                    SIDEX_VARIANT sVarColumnName;
                                    *err = sidex_Variant_List_Get (sVarColumnNames, columnIndex, &sVarColumnName);
                                    if (SIDEX_SUCCESS == *err){
                                      PyObject* pyColumnName = NULL;
                                      pyColumnName = _SidexVariantDecode(sVarColumnName, err);
                                      if (SIDEX_SUCCESS == *err){
                                        SIDEX_TCHAR* sColumnNameTemp;
                                        SIDEX_INT32 isColLength;
                                        sidex_Variant_As_String (sVarColumnName, &sColumnNameTemp, &isColLength);
                                        SIDEX_VARIANT sVarCell;
                                        *err = sidex_Variant_Table_GetField (sVariant, rowIndex, sColumnNameTemp, &sVarCell);
                                        if (SIDEX_SUCCESS == *err){
                                          PyObject* pyKeyValue = utf32_utf16_decode(sColumnNameTemp, -1);
                                          PyObject* pyDictItem = NULL;
                                          pyDictItem = _SidexVariantDecode(sVarCell, err);
                                          if (SIDEX_SUCCESS == *err){
                                            int iPyErr = PyDict_SetItem(pyRow, pyKeyValue, pyDictItem);
                                            if (0 > iPyErr){
                                              *err = SIDEX_ERR_COMMON;
                                            }
                                          }
                                          Py_XDECREF(pyKeyValue);
                                        }
                                      }
                                    }
                                  }
                                  int iPyErr = PyList_SetItem(retValue, rowIndex, pyRow);
                                  if (0 > iPyErr){
                                    *err = SIDEX_ERR_COMMON;
                                  }
                                }
                                sidex_Variant_DecRef(sVarColumnNames);
                              }
                            }
                          }
                          break;
    case SIDEX_DATA_TYPE_UNKNOWN:
    default:
                          sRet = "TYPE_UNKNOWN";
                          *err = SIDEX_ERR_COMMON;
                          break;
  }
  if (bLog)printf ("Type = %s\n", sRet);
  return retValue;
}


/*
 *------------------------------------------------------
 *               initDateTimeAPI
 *------------------------------------------------------
 */
void initDateTimeAPI() {
#if PY_MAJOR_VERSION >= 3
  #if PY_MINOR_VERSION >= 1
      PyDateTimeAPI = (PyDateTime_CAPI*) PyCapsule_Import("datetime.datetime_CAPI", 0);
  #else
      PyDateTimeAPI = (PyDateTime_CAPI*) PyCObject_Import("datetime", "datetime_CAPI");
  #endif
#else
    PyDateTimeAPI = (PyDateTime_CAPI*) PyCObject_Import("datetime", "datetime_CAPI");
#endif
}

