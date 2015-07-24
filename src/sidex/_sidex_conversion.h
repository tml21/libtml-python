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
 
#ifndef _SIDEX_CONVERSION_H
#define _SIDEX_CONVERSION_H

SIDEX_VARIANT _sidex_Variant_New_NoneS(SIDEX_INT32* err);
SIDEX_VARIANT _sidex_Variant_New_BooleanS(PyObject *args, SIDEX_INT32* err);
SIDEX_VARIANT _sidex_Variant_New_FloatS(PyObject *args, SIDEX_INT32* err);
SIDEX_VARIANT _sidex_Variant_New_BinaryS(PyObject *args, SIDEX_INT32* err);
SIDEX_VARIANT _sidex_Variant_New_StringS(PyObject *args, SIDEX_INT32* err);
SIDEX_VARIANT _sidex_Variant_New_UnicodeStringS(PyObject *args, SIDEX_INT32* err, PyObject* errOutputObj);
SIDEX_VARIANT _sidex_Variant_New_IntegerS(PyObject *args, SIDEX_INT32* err);
SIDEX_VARIANT _sidex_Variant_New_DateTimeS(PyObject *args, SIDEX_INT32* err, PyObject* errOutputObj);
SIDEX_VARIANT _sidex_Variant_New_ListS(SIDEX_INT32* err);
SIDEX_VARIANT _sidex_Variant_New_DictS(SIDEX_INT32* err);

SIDEX_VARIANT _PyObjectAsSidexVariantS(PyObject *pvalue, SIDEX_INT32* err, PyObject* errOutputObj);
PyObject*     _SidexVariantDecode(SIDEX_VARIANT sVariant, SIDEX_INT32* err);

int DayOfWeek (int day, int month, int year);
int DayOfYear (int day, int month, int year);

void initDateTimeAPI();

#endif // _SIDEX_CONVERSION_H
