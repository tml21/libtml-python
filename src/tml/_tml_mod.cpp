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
#include <sidex.h>
#include <tmlCore.h>
#include <Python.h>
#include <structmember.h>
#include <iostream>
#include "_py_unicode.h"

using namespace std;


/////////////////////////////////////////////////////////////////////////////////////
// Datastructure contains the parameter for callback request:
struct PythonCallbackData{
  PyObject*       pCBFunc;
  PyObject*       pCBData;
};


/////////////////////////////////////////////////////////////////////////////////////
// Datastructure contains the parameter for RecStream- Download request:
struct PythonReceiverStrmCallbackData{
  PyObject*       pCB_Rec_Strm_DLD_Block_Func;
  PyObject*       pCB_Rec_Strm_DLD_Block_Data;
  PyObject*       pCB_Rec_Strm_DLD_Finish_Func;
  PyObject*       pCB_Rec_Strm_DLD_Finish_Data;
  TML_INT64       iID;
};


/////////////////////////////////////////////////////////////////////////////////////
// Datastructure contains the parameter for SndStream- Download request:
struct PythonSenderStrmCallbackData{
  PyObject*  pCB_OnClose_Func;
  PyObject*  pCB_OnClose_Data;
  PyObject*  pCB_OnGetPosition_Func;
  PyObject*  pCB_OnGetPosition_Data;
  PyObject*  pCB_OnGetSize_Func;
  PyObject*  pCB_OnGetSize_Data;
  PyObject*  pCB_OnError_Func;
  PyObject*  pCB_OnError_Data;
  PyObject*  pCB_OnRead_Func;
  PyObject*  pCB_OnRead_Data;
  PyObject*  pCB_OnSeek_Func;
  PyObject*  pCB_OnSeek_Data;
  PyObject*  pCB_OnWrite_Func;
  PyObject*  pCB_OnWrite_Data;
  TML_INT64  iID;
};


/*
 *************************************************************************************
 *                                 command dispatch dictionary
 *************************************************************************************
 */
static SIDEX_VARIANT m_customDispatchDict         = SIDEX_HANDLE_TYPE_NULL;
static SIDEX_VARIANT m_cmdDispatchDict            = SIDEX_HANDLE_TYPE_NULL;
static SIDEX_VARIANT m_deleteCmdDict              = SIDEX_HANDLE_TYPE_NULL;
static SIDEX_VARIANT m_balOnBusyStatusRequestDict = SIDEX_HANDLE_TYPE_NULL;
static SIDEX_VARIANT m_balOnCalculationDict       = SIDEX_HANDLE_TYPE_NULL;
static SIDEX_VARIANT m_balOnPeerRegisterDict      = SIDEX_HANDLE_TYPE_NULL;
static SIDEX_VARIANT m_balOnPopulateDict          = SIDEX_HANDLE_TYPE_NULL;
static SIDEX_VARIANT m_evtOnErrorDict             = SIDEX_HANDLE_TYPE_NULL;
static SIDEX_VARIANT m_evtOnPeerRegisterDict      = SIDEX_HANDLE_TYPE_NULL;
static SIDEX_VARIANT m_evtOnPopulateDict          = SIDEX_HANDLE_TYPE_NULL;
static SIDEX_VARIANT m_evtOnQueueOverflowDict     = SIDEX_HANDLE_TYPE_NULL;
static SIDEX_VARIANT m_receiverStrmDict           = SIDEX_HANDLE_TYPE_NULL;
static SIDEX_VARIANT m_senderStrmDict             = SIDEX_HANDLE_TYPE_NULL;

/*
 *************************************************************************************
 *                                 exception definition
 *************************************************************************************
 */
static PyObject *tmlError;
static PyObject *tmlIndexError;


/*
 *----------------------------------------------------------------
 *      Helper method to print error / exception to sterr
 *----------------------------------------------------------------
 */
void stderrPrintErr(TML_INT32 err, char * sIdentifier){

  if (TML_SUCCESS != err){
    PySys_WriteStderr("Error:%d in %s\n", err, sIdentifier);
  }
  else{
    if (PyErr_Occurred()) {
      PyErr_Print();
    }
  }
};


/*
 *----------------------------------------------------------------
 *      Helper method to store dispatch callback infos
 *----------------------------------------------------------------
 */
static TML_INT32 getCommandDispatchCallbackData(TML_TCHAR* pProfile, TML_COMMAND_ID_TYPE iCmdIdentifier,
                                                    PyObject* pCBFunc, PyObject* pCBData,
                                                    PythonCallbackData** commandDispatchCallbackData){
    TML_INT32             err = TML_SUCCESS;
    SIDEX_VARIANT profileDict = SIDEX_HANDLE_TYPE_NULL;
    SIDEX_VARIANT vData       = SIDEX_HANDLE_TYPE_NULL;
    err = sidex_Variant_Dict_Get(m_cmdDispatchDict, pProfile, &profileDict);
    if (TML_SUCCESS != err){
      profileDict = sidex_Variant_New_Dict();
      err = sidex_Variant_Dict_Set(m_cmdDispatchDict, pProfile, profileDict);
    }
    if (TML_SUCCESS == err){

      wchar_t* sCmdIdentifier = new wchar_t[128];
#ifdef LINUX
      swprintf (sCmdIdentifier, 128, L"%ld", iCmdIdentifier);
#else // LINUX
      swprintf_s (sCmdIdentifier, 128, L"%ld", iCmdIdentifier);
#endif // LINUX
      SIDEX_INT64 iValue = 0;
      err = sidex_Variant_Dict_Get(profileDict, sCmdIdentifier, &vData);
      if (TML_SUCCESS == err){
        err = sidex_Variant_As_Integer(vData, &iValue);
        if (TML_SUCCESS == err){
          *commandDispatchCallbackData = (PythonCallbackData*)iValue;
          Py_XDECREF((*commandDispatchCallbackData)->pCBFunc);
          Py_XDECREF((*commandDispatchCallbackData)->pCBData);
        }
      }
      else{
        *commandDispatchCallbackData = new PythonCallbackData;
        iValue = (SIDEX_INT64) *commandDispatchCallbackData;
        vData = sidex_Variant_New_Integer(iValue);
        err = sidex_Variant_Dict_Set(profileDict, sCmdIdentifier, vData);
      }
      if (TML_SUCCESS == err){
        Py_XINCREF(pCBFunc);
        Py_XINCREF(pCBData);
        (*commandDispatchCallbackData)->pCBFunc = pCBFunc;
        (*commandDispatchCallbackData)->pCBData = pCBData;
      }
      delete sCmdIdentifier;
    }
  return err;
};


/*
 *----------------------------------------------------------------
 *      Helper method to store custom dispatch callback infos into a dict
 *----------------------------------------------------------------
 */
static TML_INT32 getCallbackDataFromDict(TML_TCHAR* pProfile,
                                                    PyObject* pCBFunc, PyObject* pCBData,
                                                    PythonCallbackData** callbackData, SIDEX_VARIANT dict){
  TML_INT32             err = TML_SUCCESS;
  SIDEX_VARIANT vData       = SIDEX_HANDLE_TYPE_NULL;
  err = sidex_Variant_Dict_Get(dict, pProfile, &vData);
  SIDEX_INT64 iValue;
  if (TML_SUCCESS != err){
    *callbackData = new PythonCallbackData;
    iValue = (SIDEX_INT64) *callbackData;
    vData = sidex_Variant_New_Integer(iValue);
    err = sidex_Variant_Dict_Set(dict, pProfile, vData);
  }
  else{
    err = sidex_Variant_As_Integer(vData, &iValue);
    *callbackData = (PythonCallbackData*)iValue;
    Py_XDECREF((*callbackData)->pCBFunc);
    Py_XDECREF((*callbackData)->pCBData);
  }
  if (TML_SUCCESS == err){
    Py_XINCREF(pCBFunc);
    Py_XINCREF(pCBData);
    (*callbackData)->pCBFunc = pCBFunc;
    (*callbackData)->pCBData = pCBData;
  }
  return err;
};


/*
 *----------------------------------------------------------------
 *      Helper method to store receiver stream callback infos into a dict
 *----------------------------------------------------------------
 */
static TML_INT32 getRecStrmCallbackDataFromDict(TML_INT64 iID,
                                                PyObject* pCB_Rec_Strm_DLD_Block_Func, PyObject* pCB_Rec_Strm_DLD_Block_Data,
                                                PyObject* pCB_Rec_Strm_DLD_Finish_Func, PyObject* pCB_Rec_Strm_DLD_Finish_Data,
                                                PythonReceiverStrmCallbackData** callbackData, SIDEX_VARIANT dict){
  TML_INT32     err    = TML_SUCCESS;
  SIDEX_VARIANT vData  = SIDEX_HANDLE_TYPE_NULL;

  char sKey[128];
#ifdef LINUX
  sprintf (sKey, "%" PRId64, iID);
#else // LINUX
  #if _MSC_VER > 1500
    sprintf_s (sKey, 128, "%" PRId64, iID);
  #else
    sprintf (sKey, "%" PRId64, iID);
  #endif
#endif // LINUX

  err = sidex_Variant_Dict_Get_A(dict, sKey, &vData);
  SIDEX_INT64 iValue;
  if (TML_SUCCESS != err){
    *callbackData = new PythonReceiverStrmCallbackData;
    iValue = (SIDEX_INT64) *callbackData;
    vData = sidex_Variant_New_Integer(iValue);
    err = sidex_Variant_Dict_Set_A(dict, sKey, vData);
  }
  else{
    err = sidex_Variant_As_Integer(vData, &iValue);
    *callbackData = (PythonReceiverStrmCallbackData*)iValue;
    Py_XDECREF((*callbackData)->pCB_Rec_Strm_DLD_Block_Func);
    Py_XDECREF((*callbackData)->pCB_Rec_Strm_DLD_Block_Data);
    Py_XDECREF((*callbackData)->pCB_Rec_Strm_DLD_Finish_Func);
    Py_XDECREF((*callbackData)->pCB_Rec_Strm_DLD_Finish_Data);
  }
  if (TML_SUCCESS == err){
    Py_XINCREF(pCB_Rec_Strm_DLD_Block_Func);
    Py_XINCREF(pCB_Rec_Strm_DLD_Block_Data);
    Py_XINCREF(pCB_Rec_Strm_DLD_Finish_Func);
    Py_XINCREF(pCB_Rec_Strm_DLD_Finish_Data);
    (*callbackData)->pCB_Rec_Strm_DLD_Block_Func  = pCB_Rec_Strm_DLD_Block_Func;
    (*callbackData)->pCB_Rec_Strm_DLD_Block_Data  = pCB_Rec_Strm_DLD_Block_Data;
    (*callbackData)->pCB_Rec_Strm_DLD_Finish_Func = pCB_Rec_Strm_DLD_Finish_Func;
    (*callbackData)->pCB_Rec_Strm_DLD_Finish_Data = pCB_Rec_Strm_DLD_Finish_Data;
  }
  return err;
};


/*
 *----------------------------------------------------------------
 *      Helper method to store sender stream callback infos into a dict
 *----------------------------------------------------------------
 */
static TML_INT32 getSndStrmCallbackDataFromDict(TML_INT64 iID,
                                                    bool bOnClose, PyObject* pCB_OnClose_Func,       PyObject* pCB_OnClose_Data,
                                                    bool bOnGetPosition, PyObject* pCB_OnGetPosition_Func, PyObject* pCB_OnGetPosition_Data,
                                                    bool bOnGetSize, PyObject* pCB_OnGetSize_Func,     PyObject* pCB_OnGetSize_Data,
                                                    bool bOnError, PyObject* pCB_OnError_Func,       PyObject* pCB_OnError_Data,
                                                    bool bOnRead, PyObject* pCB_OnRead_Func,        PyObject* pCB_OnRead_Data,
                                                    bool bOnSeek, PyObject* pCB_OnSeek_Func,        PyObject* pCB_OnSeek_Data,
                                                    bool bOnWrite, PyObject* pCB_OnWrite_Func,       PyObject* pCB_OnWrite_Data,
                                                    PythonSenderStrmCallbackData** callbackData, SIDEX_VARIANT dict){
  TML_INT32     err    = TML_SUCCESS;
  SIDEX_VARIANT vData  = SIDEX_HANDLE_TYPE_NULL;

  char sKey[128];
#ifdef LINUX
  sprintf (sKey, "%" PRId64, iID);
#else // LINUX
  #if _MSC_VER > 1500
    sprintf_s (sKey, 128, "%" PRId64, iID);
  #else
    sprintf (sKey, "%" PRId64, iID);
  #endif
#endif // LINUX

  err = sidex_Variant_Dict_Get_A(dict, sKey, &vData);
  SIDEX_INT64 iValue;
  if (TML_SUCCESS != err){
    *callbackData = new PythonSenderStrmCallbackData;
    (*callbackData)->pCB_OnClose_Func       = NULL;
    (*callbackData)->pCB_OnClose_Data       = NULL;
    (*callbackData)->pCB_OnGetPosition_Func = NULL;
    (*callbackData)->pCB_OnGetPosition_Data = NULL;
    (*callbackData)->pCB_OnGetSize_Func     = NULL;
    (*callbackData)->pCB_OnGetSize_Data     = NULL;
    (*callbackData)->pCB_OnError_Func       = NULL;
    (*callbackData)->pCB_OnError_Data       = NULL;
    (*callbackData)->pCB_OnRead_Func        = NULL;
    (*callbackData)->pCB_OnRead_Data        = NULL;
    (*callbackData)->pCB_OnSeek_Func        = NULL;
    (*callbackData)->pCB_OnSeek_Data        = NULL;
    (*callbackData)->pCB_OnWrite_Func       = NULL;
    (*callbackData)->pCB_OnWrite_Data       = NULL;
    (*callbackData)->iID                    = iID;

    iValue = (SIDEX_INT64) *callbackData;
    vData = sidex_Variant_New_Integer(iValue);
    err = sidex_Variant_Dict_Set_A(dict, sKey, vData);
  }
  else{
    err = sidex_Variant_As_Integer(vData, &iValue);
    *callbackData = (PythonSenderStrmCallbackData*)iValue;
    if (bOnClose && NULL != (*callbackData)->pCB_OnClose_Func){
        Py_XDECREF((*callbackData)->pCB_OnClose_Func);
        Py_XDECREF((*callbackData)->pCB_OnClose_Data);
    }
    if (bOnGetPosition && NULL != (*callbackData)->pCB_OnGetPosition_Func){
        Py_XDECREF((*callbackData)->pCB_OnGetPosition_Func);
        Py_XDECREF((*callbackData)->pCB_OnGetPosition_Data);
    }
    if (bOnGetSize && NULL != (*callbackData)->pCB_OnGetSize_Func){
        Py_XDECREF((*callbackData)->pCB_OnGetSize_Func);
        Py_XDECREF((*callbackData)->pCB_OnGetSize_Data);
    }
    if (bOnError && NULL != (*callbackData)->pCB_OnError_Func){
        Py_XDECREF((*callbackData)->pCB_OnError_Func);
        Py_XDECREF((*callbackData)->pCB_OnError_Data);
    }
    if (bOnRead && NULL != (*callbackData)->pCB_OnRead_Func){
        Py_XDECREF((*callbackData)->pCB_OnRead_Func);
        Py_XDECREF((*callbackData)->pCB_OnRead_Data);
    }
    if (bOnSeek && NULL != (*callbackData)->pCB_OnSeek_Func){
        Py_XDECREF((*callbackData)->pCB_OnSeek_Func);
        Py_XDECREF((*callbackData)->pCB_OnSeek_Data);
    }
    if (bOnWrite && NULL != (*callbackData)->pCB_OnWrite_Func){
        Py_XDECREF((*callbackData)->pCB_OnWrite_Func);
        Py_XDECREF((*callbackData)->pCB_OnWrite_Data);
    }
  }
  if (TML_SUCCESS == err){
    if (bOnClose){
        Py_XINCREF(pCB_OnClose_Func);
        Py_XINCREF(pCB_OnClose_Data);
        (*callbackData)->pCB_OnClose_Func = pCB_OnClose_Func;
        (*callbackData)->pCB_OnClose_Data = pCB_OnClose_Data;
    }
    if (bOnGetPosition){
        Py_XINCREF(pCB_OnGetPosition_Func);
        Py_XINCREF(pCB_OnGetPosition_Data);
        (*callbackData)->pCB_OnGetPosition_Func = pCB_OnGetPosition_Func;
        (*callbackData)->pCB_OnGetPosition_Data = pCB_OnGetPosition_Data;
    }
    if (bOnGetSize){
        Py_XINCREF(pCB_OnGetSize_Func);
        Py_XINCREF(pCB_OnGetSize_Data);
        (*callbackData)->pCB_OnGetSize_Func = pCB_OnGetSize_Func;
        (*callbackData)->pCB_OnGetSize_Data = pCB_OnGetSize_Data;
    }
    if (bOnError){
        Py_XINCREF(pCB_OnError_Func);
        Py_XINCREF(pCB_OnError_Data);
        (*callbackData)->pCB_OnError_Func = pCB_OnError_Func;
        (*callbackData)->pCB_OnError_Data = pCB_OnError_Data;
    }
    if (bOnRead){
        Py_XINCREF(pCB_OnRead_Func);
        Py_XINCREF(pCB_OnRead_Data);
        (*callbackData)->pCB_OnRead_Func = pCB_OnRead_Func;
        (*callbackData)->pCB_OnRead_Data = pCB_OnRead_Data;
    }
    if (bOnSeek){
        Py_XINCREF(pCB_OnSeek_Func);
        Py_XINCREF(pCB_OnSeek_Data);
        (*callbackData)->pCB_OnSeek_Func = pCB_OnSeek_Func;
        (*callbackData)->pCB_OnSeek_Data = pCB_OnSeek_Data;
    }
    if (bOnWrite){
        Py_XINCREF(pCB_OnWrite_Func);
        Py_XINCREF(pCB_OnWrite_Data);
        (*callbackData)->pCB_OnWrite_Func = pCB_OnWrite_Func;
        (*callbackData)->pCB_OnWrite_Data = pCB_OnWrite_Data;
    }
  }
  return err;
}


/*
 *----------------------------------------------------------------
 *      Helper method to release dispatch callback infos
 *----------------------------------------------------------------
 */
static void releaseCommandDispatchCallbackData(bool bReleaseDict){
  TML_INT32 err = TML_SUCCESS;
  err = sidex_Variant_Dict_First(m_cmdDispatchDict);
  while (TML_SUCCESS == err){
    SIDEX_VARIANT profileDict = SIDEX_HANDLE_TYPE_NULL;
    wchar_t*      sProfile;
    err = sidex_Variant_Dict_Next(m_cmdDispatchDict, &sProfile, &profileDict);
    if (TML_SUCCESS == err){
      err = sidex_Variant_Dict_First(profileDict);
      while (TML_SUCCESS == err){
        wchar_t*      sCmd;
        SIDEX_VARIANT vCmd = SIDEX_HANDLE_TYPE_NULL;
        err = sidex_Variant_Dict_Next(profileDict, &sCmd, &vCmd);
        if (TML_SUCCESS == err){
          SIDEX_INT64 iValue = 0;
          err = sidex_Variant_As_Integer(vCmd, &iValue);
          if (TML_SUCCESS == err){
              PythonCallbackData* commandDispatchCallbackData = (PythonCallbackData*)iValue;
              Py_XDECREF((commandDispatchCallbackData)->pCBFunc);
              Py_XDECREF((commandDispatchCallbackData)->pCBData);
              delete (commandDispatchCallbackData);
              sidex_Variant_DecRef(vCmd);
          }
        }
      }
      sidex_Variant_Dict_Clear(profileDict);
      sidex_Variant_DecRef(profileDict);
      err = TML_SUCCESS;
    }
  }
  sidex_Variant_Dict_Clear(m_cmdDispatchDict);
  if (bReleaseDict){
    sidex_Variant_DecRef(m_cmdDispatchDict);
  }
  return;
};


/*
 *----------------------------------------------------------------
 *      Helper method to release dispatch callback infos for a profile
 *----------------------------------------------------------------
 */
static void releaseCommandDispatchCallbackData4Profile(TML_TCHAR* pProfile){
  TML_INT32 err = TML_SUCCESS;
  SIDEX_VARIANT profileDict = SIDEX_HANDLE_TYPE_NULL;
  err = sidex_Variant_Dict_Get(m_cmdDispatchDict, pProfile, &profileDict);
  if (TML_SUCCESS == err){
    err = sidex_Variant_Dict_First(profileDict);
    if (TML_SUCCESS == err){
      do{
        wchar_t*      sCmd;
        SIDEX_VARIANT vCmd = SIDEX_HANDLE_TYPE_NULL;
        err = sidex_Variant_Dict_Next(profileDict, &sCmd, &vCmd);
        if (TML_SUCCESS == err){
          SIDEX_INT64 iValue = 0;
          err = sidex_Variant_As_Integer(vCmd, &iValue);
          if (TML_SUCCESS == err){
            PythonCallbackData* commandDispatchCallbackData = (PythonCallbackData*)iValue;
            Py_XDECREF((commandDispatchCallbackData)->pCBFunc);
            Py_XDECREF((commandDispatchCallbackData)->pCBData);
            delete (commandDispatchCallbackData);
            sidex_Variant_DecRef(vCmd);
          }
        }
      }while (TML_SUCCESS == err);
      sidex_Variant_Dict_Clear(profileDict);
      sidex_Variant_DecRef(profileDict);
    }
    sidex_Variant_Dict_Delete (m_cmdDispatchDict, pProfile);
  }
  return;
};


/*
 *----------------------------------------------------------------
 *      Helper method to release callback infos
 *----------------------------------------------------------------
 */
static void releaseCallbackDataDict(SIDEX_VARIANT dict, bool bReleaseDict){
  TML_INT32 err = TML_SUCCESS;
  err = sidex_Variant_Dict_First(dict);
  while (TML_SUCCESS == err){
    wchar_t*      sProfile;
    SIDEX_VARIANT vCmd = SIDEX_HANDLE_TYPE_NULL;
    err = sidex_Variant_Dict_Next(dict, &sProfile, &vCmd);
    if (TML_SUCCESS == err){
      SIDEX_INT64 iValue = 0;
      err = sidex_Variant_As_Integer(vCmd, &iValue);
      if (TML_SUCCESS == err){
        PythonCallbackData* callbackData = (PythonCallbackData*)iValue;
        Py_XDECREF((callbackData)->pCBFunc);
        Py_XDECREF((callbackData)->pCBData);
        delete (callbackData);
        sidex_Variant_DecRef(vCmd);
      }
    }
  }
  sidex_Variant_Dict_Clear(dict);
  if (bReleaseDict){
    sidex_Variant_DecRef(dict);
  }
  return;
};

/*
 *----------------------------------------------------------------
 *      Helper method to release callback infos for a profile
 *----------------------------------------------------------------
 */
static void releaseCallbackDataDict4Profile(SIDEX_VARIANT dict, TML_TCHAR* pProfile){
  TML_INT32 err = TML_SUCCESS;

  SIDEX_VARIANT vData       = SIDEX_HANDLE_TYPE_NULL;
  err = sidex_Variant_Dict_Get(dict, pProfile, &vData);
  if (TML_SUCCESS == err){
    SIDEX_INT64 iValue;
    err = sidex_Variant_As_Integer(vData, &iValue);
    if (TML_SUCCESS == err){
      PythonCallbackData* callbackData = (PythonCallbackData*)iValue;
      Py_XDECREF(callbackData->pCBFunc);
      Py_XDECREF(callbackData->pCBData);
      delete (callbackData);
      sidex_Variant_DecRef(vData);
    }
    sidex_Variant_Dict_Delete (dict, pProfile);
  }
  return;
};


/*
 *----------------------------------------------------------------
 *      Helper method to release receiver stream callback infos for a special iID
 *----------------------------------------------------------------
 */
static void removeRecStrmCallbackDataFromDict(SIDEX_VARIANT dict, TML_INT64 iID){
  TML_INT32 err = TML_SUCCESS;

  char sKey[128];
#ifdef LINUX
  sprintf (sKey, "%" PRId64, iID);
#else // LINUX
  #if _MSC_VER > 1500
    sprintf_s (sKey, 128, "%" PRId64, iID);
  #else
    sprintf (sKey, "%" PRId64, iID);
  #endif
#endif // LINUX

  SIDEX_VARIANT vCmd = SIDEX_HANDLE_TYPE_NULL;
  err = sidex_Variant_Dict_Get_A(dict, sKey, &vCmd);
  if (TML_SUCCESS == err){
    SIDEX_INT64 iValue = 0;
    err = sidex_Variant_As_Integer(vCmd, &iValue);
    if (TML_SUCCESS == err){
      PythonReceiverStrmCallbackData* callbackData = (PythonReceiverStrmCallbackData*)iValue;
      Py_XDECREF(callbackData->pCB_Rec_Strm_DLD_Block_Func);
      Py_XDECREF(callbackData->pCB_Rec_Strm_DLD_Block_Data);
      Py_XDECREF(callbackData->pCB_Rec_Strm_DLD_Finish_Func);
      Py_XDECREF(callbackData->pCB_Rec_Strm_DLD_Finish_Data);
      delete (callbackData);
      sidex_Variant_DecRef(vCmd);
    }
    err = sidex_Variant_Dict_Delete_A(dict, sKey);
  }
  return;
};


/*
 *----------------------------------------------------------------
 *      Helper method to release receiver stream callback infos
 *----------------------------------------------------------------
 */
static void removeAllRecStrmCallbackDataFromDict(SIDEX_VARIANT dict, bool bReleaseDict){
  TML_INT32 err = TML_SUCCESS;
  err = sidex_Variant_Dict_First(dict);
  while (TML_SUCCESS == err){
    wchar_t*      sProfile;
    SIDEX_VARIANT vCmd = SIDEX_HANDLE_TYPE_NULL;
    err = sidex_Variant_Dict_Next(dict, &sProfile, &vCmd);
    if (TML_SUCCESS == err){
      SIDEX_INT64 iValue = 0;
      err = sidex_Variant_As_Integer(vCmd, &iValue);
      if (TML_SUCCESS == err){
        PythonReceiverStrmCallbackData* callbackData = (PythonReceiverStrmCallbackData*)iValue;
        Py_XDECREF(callbackData->pCB_Rec_Strm_DLD_Block_Func);
        Py_XDECREF(callbackData->pCB_Rec_Strm_DLD_Block_Data);
        Py_XDECREF(callbackData->pCB_Rec_Strm_DLD_Finish_Func);
        Py_XDECREF(callbackData->pCB_Rec_Strm_DLD_Finish_Data);
        delete (callbackData);
        sidex_Variant_DecRef(vCmd);
      }
    }
  }
  sidex_Variant_Dict_Clear(dict);
  if (bReleaseDict){
    sidex_Variant_DecRef(dict);
  }
  return;
};


/*
 *----------------------------------------------------------------
 *      Helper method to release sender stream callback infos for a special iID
 *----------------------------------------------------------------
 */
static void removeSndStrmCallbackDataFromDict(SIDEX_VARIANT dict, TML_INT64 iID){
  TML_INT32 err = TML_SUCCESS;

  char sKey[128];
#ifdef LINUX
  sprintf (sKey, "%" PRId64, iID);
#else // LINUX
  #if _MSC_VER > 1500
    sprintf_s (sKey, 128, "%" PRId64, iID);
  #else
    sprintf (sKey, "%" PRId64, iID);
  #endif
#endif // LINUX

  SIDEX_VARIANT vCmd = SIDEX_HANDLE_TYPE_NULL;
  err = sidex_Variant_Dict_Get_A(dict, sKey, &vCmd);
  if (TML_SUCCESS == err){
    SIDEX_INT64 iValue = 0;
    err = sidex_Variant_As_Integer(vCmd, &iValue);
    if (TML_SUCCESS == err){
      PythonSenderStrmCallbackData* callbackData = (PythonSenderStrmCallbackData*)iValue;
      if (NULL != callbackData->pCB_OnClose_Func){
          Py_XDECREF(callbackData->pCB_OnClose_Func);
          Py_XDECREF(callbackData->pCB_OnClose_Data);
      }
      if (NULL != callbackData->pCB_OnGetPosition_Func){
          Py_XDECREF(callbackData->pCB_OnGetPosition_Func);
          Py_XDECREF(callbackData->pCB_OnGetPosition_Data);
      }
      if (NULL != callbackData->pCB_OnGetSize_Func){
          Py_XDECREF(callbackData->pCB_OnGetSize_Func);
          Py_XDECREF(callbackData->pCB_OnGetSize_Data);
      }
      if (NULL != callbackData->pCB_OnError_Func){
          Py_XDECREF(callbackData->pCB_OnError_Func);
          Py_XDECREF(callbackData->pCB_OnError_Data);
      }
      if (NULL != callbackData->pCB_OnRead_Func){
          Py_XDECREF(callbackData->pCB_OnRead_Func);
          Py_XDECREF(callbackData->pCB_OnRead_Data);
      }
      if (NULL != callbackData->pCB_OnSeek_Func){
          Py_XDECREF(callbackData->pCB_OnSeek_Func);
          Py_XDECREF(callbackData->pCB_OnSeek_Data);
      }
      if (NULL != callbackData->pCB_OnWrite_Func){
          Py_XDECREF(callbackData->pCB_OnWrite_Func);
          Py_XDECREF(callbackData->pCB_OnWrite_Data);
      }
      delete (callbackData);
      sidex_Variant_DecRef(vCmd);
    }
    err = sidex_Variant_Dict_Delete_A(dict, sKey);
  }
  return;
}


/*
 *----------------------------------------------------------------
 *      Helper method to release sender stream callback infos
 *----------------------------------------------------------------
 */
static void removeAllSndStrmCallbackDataFromDict(SIDEX_VARIANT dict, bool bReleaseDict){
  TML_INT32 err = TML_SUCCESS;
  err = sidex_Variant_Dict_First(dict);
  while (TML_SUCCESS == err){
    wchar_t*      sProfile;
    SIDEX_VARIANT vCmd = SIDEX_HANDLE_TYPE_NULL;
    err = sidex_Variant_Dict_Next(dict, &sProfile, &vCmd);
    if (TML_SUCCESS == err){
      SIDEX_INT64 iValue = 0;
      err = sidex_Variant_As_Integer(vCmd, &iValue);
      if (TML_SUCCESS == err){
        PythonSenderStrmCallbackData* callbackData = (PythonSenderStrmCallbackData*)iValue;
        if (NULL != callbackData->pCB_OnClose_Func){
            Py_XDECREF(callbackData->pCB_OnClose_Func);
            Py_XDECREF(callbackData->pCB_OnClose_Data);
        }
        if (NULL != callbackData->pCB_OnGetPosition_Func){
            Py_XDECREF(callbackData->pCB_OnGetPosition_Func);
            Py_XDECREF(callbackData->pCB_OnGetPosition_Data);
        }
        if (NULL != callbackData->pCB_OnGetSize_Func){
            Py_XDECREF(callbackData->pCB_OnGetSize_Func);
            Py_XDECREF(callbackData->pCB_OnGetSize_Data);
        }
        if (NULL != callbackData->pCB_OnError_Func){
            Py_XDECREF(callbackData->pCB_OnError_Func);
            Py_XDECREF(callbackData->pCB_OnError_Data);
        }
        if (NULL != callbackData->pCB_OnRead_Func){
            Py_XDECREF(callbackData->pCB_OnRead_Func);
            Py_XDECREF(callbackData->pCB_OnRead_Data);
        }
        if (NULL != callbackData->pCB_OnSeek_Func){
            Py_XDECREF(callbackData->pCB_OnSeek_Func);
            Py_XDECREF(callbackData->pCB_OnSeek_Data);
        }
        if (NULL != callbackData->pCB_OnWrite_Func){
            Py_XDECREF(callbackData->pCB_OnWrite_Func);
            Py_XDECREF(callbackData->pCB_OnWrite_Data);
        }
        delete (callbackData);
        sidex_Variant_DecRef(vCmd);
      }
    }
  }
  sidex_Variant_Dict_Clear(dict);
  if (bReleaseDict){
    sidex_Variant_DecRef(dict);
  }
  return;
}


/*
 *************************************************************************************
 *                                  function definitions
 *************************************************************************************
 */

/**
 * @brief    Callback method for async progress
 */
void FUNC_C_DECL onCoreProgressCallback(TML_COMMAND_HANDLE cmd, TML_POINTER data, SIDEX_INT32 iProgress){
  TML_INT32   err = TML_SUCCESS;
  // Calling Python out of a non- python created thread
  PyGILState_STATE gstate = PyGILState_UNLOCKED;
  try {
    gstate = PyGILState_Ensure();

    PythonCallbackData* callbackData = (PythonCallbackData*) data;

    PyObject *pythonCBFunc = callbackData->pCBFunc;
    /* Time to call the callback */
    if (PyCallable_Check(pythonCBFunc)) {
      PyObject* arglist = Py_BuildValue("(LOl)", cmd, callbackData->pCBData, iProgress);
      PyObject* result = PyEval_CallObject(pythonCBFunc, arglist);
      Py_DECREF(arglist);
      Py_DECREF(result);
    }
  }
  catch (...) {
    PyErr_Print();
    err = TML_ERR_COMMON;
  }
  stderrPrintErr(err, (char*)"onCoreProgressCallback request");

  // Release calling Python out of a non- python created thread
  // this call must be located after catch(..) to make sure it is called in case of
  // any exception to avoid blocking the GIL
  PyGILState_Release(gstate);
}


/**
 * @brief    Callback method for async status reply
 */
void FUNC_C_DECL onCoreStatusReplyCallback(TML_COMMAND_HANDLE cmd, TML_POINTER data, TML_INT32 iType, TML_CTSTR *sMsg)
{
  TML_INT32   err = TML_SUCCESS;
  // Calling Python out of a non- python created thread
  PyGILState_STATE gstate = PyGILState_UNLOCKED;
  try {
    gstate = PyGILState_Ensure();

    PythonCallbackData* callbackData = (PythonCallbackData*) data;

    PyObject *pythonCBFunc = callbackData->pCBFunc;
    /* Time to call the callback */
    if (PyCallable_Check(pythonCBFunc)) {
      PyObject* pyMsg = utf32_utf16_decode((TML_TCHAR*)sMsg, -1);
      PyObject* arglist = Py_BuildValue("(LOlO)", cmd, callbackData->pCBData, iType, pyMsg);
      Py_XDECREF(pyMsg);
      PyObject* result = PyEval_CallObject(pythonCBFunc, arglist);
      Py_DECREF(arglist);
      Py_DECREF(result);
    }
  }
  catch (...) {
    PyErr_Print();
    err = TML_ERR_COMMON;
  }
  stderrPrintErr(err, (char*)"onCoreStatusReplyCallback request");

  // Release calling Python out of a non- python created thread
  // this call must be located after catch(..) to make sure it is called in case of
  // any exception to avoid blocking the GIL
  PyGILState_Release(gstate);
}


/**
 * @brief    Callback method for async command ready
 */
void FUNC_C_DECL onCoreCommandReadyCallback(TML_COMMAND_HANDLE cmd, TML_POINTER data)
{
  TML_INT32   err = TML_SUCCESS;
  // Calling Python out of a non- python created thread
  PyGILState_STATE gstate = PyGILState_UNLOCKED;
  try {
    gstate = PyGILState_Ensure();

    PythonCallbackData* callbackData = (PythonCallbackData*) data;

    PyObject *pythonCBFunc = callbackData->pCBFunc;
    /* Time to call the callback */
    if (PyCallable_Check(pythonCBFunc)) {
      PyObject* arglist = Py_BuildValue("(LO)", cmd, callbackData->pCBData);
      PyObject* result = PyEval_CallObject(pythonCBFunc, arglist);
      Py_DECREF(arglist);
      Py_DECREF(result);
    }
  }
  catch (...) {
    PyErr_Print();
    err = TML_ERR_COMMON;
  }
  stderrPrintErr(err, (char*)"onCoreCommandReadyCallback request");

  // Release calling Python out of a non- python created thread
  // this call must be located after catch(..) to make sure it is called in case of
  // any exception to avoid blocking the GIL
  PyGILState_Release(gstate);
}


/**
 * @brief    Callback method for command dispatch
 */
void FUNC_C_DECL onCoreCustomDispatchCallback(TML_COMMAND_ID_TYPE iCMD, TML_COMMAND_HANDLE cmd, TML_POINTER data)
{
 TML_INT32   err = TML_SUCCESS;
  // Calling Python out of a non- python created thread
  PyGILState_STATE gstate = PyGILState_UNLOCKED;
  try {
    gstate = PyGILState_Ensure();

    PythonCallbackData* callbackData = (PythonCallbackData*) data;

    PyObject *pythonCBFunc = callbackData->pCBFunc;
    /* Time to call the callback */
    if (PyCallable_Check(pythonCBFunc)) {
      PyObject* arglist = Py_BuildValue("(kLO)", iCMD, cmd, callbackData->pCBData);
      PyObject* result = PyEval_CallObject(pythonCBFunc, arglist);
      Py_DECREF(arglist);
      Py_DECREF(result);
    }
  }
  catch (...) {
    PyErr_Print();
    err = TML_ERR_COMMON;
  }
  stderrPrintErr(err, (char*)"onCoreCustomDispatchCallback request");

  // Release calling Python out of a non- python created thread
  // this call must be located after catch(..) to make sure it is called in case of
  // any exception to avoid blocking the GIL
  PyGILState_Release(gstate);
}


/**
 * @brief    Callback method for delete cmd
 */
void FUNC_C_DECL onCoreDeleteCmdCallback(TML_COMMAND_ID_TYPE iCMD, TML_POINTER pCmdData, TML_POINTER data)
{
  TML_INT32   err = TML_SUCCESS;
  // Calling Python out of a non- python created thread
  PyGILState_STATE gstate = PyGILState_UNLOCKED;
  try {
    gstate = PyGILState_Ensure();

    PythonCallbackData* callbackData = (PythonCallbackData*) data;

    PyObject *pythonCBFunc = callbackData->pCBFunc;
    /* Time to call the callback */
    if (PyCallable_Check(pythonCBFunc)) {
      PyObject* arglist = Py_BuildValue("(kOO)", iCMD, pCmdData, callbackData->pCBData);
      PyObject* result = PyEval_CallObject(pythonCBFunc, arglist);
      Py_DECREF(arglist);
      Py_DECREF(result);
    }
  }
  catch (...) {
    PyErr_Print();
    err = TML_ERR_COMMON;
  }
  stderrPrintErr(err, (char*)"onCoreDeleteCmdCallback request");

  // Release calling Python out of a non- python created thread
  // this call must be located after catch(..) to make sure it is called in case of
  // any exception to avoid blocking the GIL
  PyGILState_Release(gstate);
}


/**
 * @brief    Callback method for command dispatch
 */
void FUNC_C_DECL onCoreCommandDispatchCallback(TML_COMMAND_HANDLE cmd, TML_POINTER data)
{
  TML_INT32   err = TML_SUCCESS;
  // Calling Python out of a non- python created thread
  PyGILState_STATE gstate = PyGILState_UNLOCKED;
  try {
    gstate = PyGILState_Ensure();

    PythonCallbackData* callbackData = (PythonCallbackData*) data;

    PyObject *pythonCBFunc = callbackData->pCBFunc;
    /* Time to call the callback */
    if (PyCallable_Check(pythonCBFunc)) {
      PyObject* arglist = Py_BuildValue("(LO)", cmd, callbackData->pCBData);
      PyObject* result = PyEval_CallObject(pythonCBFunc, arglist);
      Py_DECREF(arglist);
      Py_DECREF(result);
      }
  }
  catch (...) {
    PyErr_Print();
    err = TML_ERR_COMMON;
  }
  stderrPrintErr(err, (char*)"onCoreCommandDispatchCallback request");

  // Release calling Python out of a non- python created thread
  // this call must be located after catch(..) to make sure it is called in case of
  // any exception to avoid blocking the GIL
  PyGILState_Release(gstate);
}


/**
 * @brief    Callback method for balanced busy status request
 */
TML_INT32 FUNC_C_DECL onBalBusyStatusRequestCallback(TML_COMMAND_HANDLE cmd, TML_POINTER data)
{
 TML_INT32   err = TML_SUCCESS;
  // Calling Python out of a non- python created thread
  PyGILState_STATE gstate = PyGILState_UNLOCKED;
  try {
    gstate = PyGILState_Ensure();

    PythonCallbackData* callbackData = (PythonCallbackData*) data;

    PyObject *pythonCBFunc = callbackData->pCBFunc;
    /* Time to call the callback */
    if (PyCallable_Check(pythonCBFunc)) {
      PyObject* arglist = Py_BuildValue("(LO)", cmd, callbackData->pCBData);
      PyObject* result = PyEval_CallObject(pythonCBFunc, arglist);
      if (!PyArg_Parse(result, "l", &err)){
        err = TML_ERR_COMMON;
      }
      Py_DECREF(arglist);
      Py_DECREF(result);
    }
  }
  catch (...) {
    PyErr_Print();
    err = TML_ERR_COMMON;
  }
  stderrPrintErr(err, (char*)"onBalBusyStatusRequestCallback request");

  // Release calling Python out of a non- python created thread
  // this call must be located after catch(..) to make sure it is called in case of
  // any exception to avoid blocking the GIL
  PyGILState_Release(gstate);
  return err;
}


/**
 * @brief    Callback method for balanced calculation request
 */
TML_INT32 FUNC_C_DECL onBalCalculationCallback(TML_INT32 iCountOfDestinations, TML_COMMAND_HANDLE* listenerBusyStateArray, TML_POINTER data, TML_INT32 *iNextListenerIndex)
{
  TML_INT32   err = TML_SUCCESS;
  TML_INT64   iTemp64_1; // Umgehung von Speicherzugriffsverletzung bei Python 3.4 unter LINUX x64
  TML_INT64   iTemp64_2; // Umgehung von Speicherzugriffsverletzung bei Python 3.4 unter LINUX x64

  // Calling Python out of a non- python created thread
  PyGILState_STATE gstate = PyGILState_UNLOCKED;
  try {
    gstate = PyGILState_Ensure();

    // I Have to create a Python- List:
    PyObject* cmdItem ;
    PyObject*  cmdList = PyList_New(0);
    for (int i = 0; i < iCountOfDestinations; ++i){
      cmdItem = Py_BuildValue("L", listenerBusyStateArray[i]);
      PyList_Append(cmdList, cmdItem);
      Py_DECREF(cmdItem);
    }

    PythonCallbackData* callbackData = (PythonCallbackData*) data;

    PyObject *pythonCBFunc = callbackData->pCBFunc;
    /* Time to call the callback */
    if (PyCallable_Check(pythonCBFunc)) {
      PyObject* arglist = Py_BuildValue("(kOO)", iCountOfDestinations, cmdList, callbackData->pCBData);
      PyObject* result = PyEval_CallObject(pythonCBFunc, arglist);
      if (!PyArg_ParseTuple(result, "ll", &iTemp64_1, &iTemp64_2)){
        err = TML_ERR_COMMON;
      }
      err                 = (TML_INT32)iTemp64_1;
      *iNextListenerIndex = (TML_INT32)iTemp64_2;

      Py_DECREF(arglist);
      Py_DECREF(result);
    }
    else{
      // No Callback - (fallback to round robin)
      err = TML_ERR_INFORMATION_UNDEFINED;
    }
    Py_DECREF(cmdList);
  }
  catch (...) {
    PyErr_Print();
    err = TML_ERR_COMMON;
  }
  if (TML_ERR_INFORMATION_UNDEFINED != err){
    // fallback to round robin is ok
    stderrPrintErr(err, (char*)"onBalCalculationCallback request");
  }

  // Release calling Python out of a non- python created thread
  // this call must be located after catch(..) to make sure it is called in case of
  // any exception to avoid blocking the GIL
  PyGILState_Release(gstate);
  return err;
}


/**
 * @brief    Callback method for peer registration requests
 */
TML_BOOL FUNC_C_DECL onPeerRegisterCallback(TML_BOOL bSubscribe, TML_CTSTR *sHost, TML_CTSTR *sPort, TML_POINTER data)
{
 TML_INT32   err = TML_SUCCESS;
 TML_BOOL    bRet = TML_TRUE;

  // Calling Python out of a non- python created thread
  PyGILState_STATE gstate = PyGILState_UNLOCKED;
  try {
    gstate = PyGILState_Ensure();

    PythonCallbackData* callbackData = (PythonCallbackData*) data;

    PyObject *pythonCBFunc = callbackData->pCBFunc;
    /* Time to call the callback */
    if (PyCallable_Check(pythonCBFunc)) {
      PyObject* pyHost = utf32_utf16_decode((TML_TCHAR*)sHost, -1);
      PyObject* pyPort = utf32_utf16_decode((TML_TCHAR*)sPort, -1);
      PyObject* arglist = Py_BuildValue("(OOOO)", bSubscribe? Py_True : Py_False, pyHost, pyPort, callbackData->pCBData);
      Py_XDECREF(pyHost);
      Py_XDECREF(pyPort);
      PyObject* result = PyEval_CallObject(pythonCBFunc, arglist);

      PyObject* pBool;
      if (!PyArg_Parse(result, "O", &pBool)){
        err = TML_ERR_COMMON;
      }
      else{
        if (true == PyBool_Check(pBool)){
          if (!PyObject_IsTrue(pBool)){
            bRet = TML_FALSE;
          }
        }
        else{
          err = TML_ERR_COMMON;
        }
      }
      Py_DECREF(arglist);
      Py_DECREF(result);
    }
  }
  catch (...) {
    PyErr_Print();
    err = TML_ERR_COMMON;
  }
  stderrPrintErr(err, (char*)"onPeerRegisterCallback request");

  // Release calling Python out of a non- python created thread
  // this call must be located after catch(..) to make sure it is called in case of
  // any exception to avoid blocking the GIL
  PyGILState_Release(gstate);
  return bRet;
}


/**
 * @brief    Callback method for population requests
 */
TML_INT32 FUNC_C_DECL onPopulateCallback(TML_CTSTR *sProfile, TML_POINTER data)
{
 TML_INT32   err = TML_SUCCESS;
  // Calling Python out of a non- python created thread
  PyGILState_STATE gstate = PyGILState_UNLOCKED;
  try {
    gstate = PyGILState_Ensure();

    PythonCallbackData* callbackData = (PythonCallbackData*) data;

    PyObject *pythonCBFunc = callbackData->pCBFunc;
    /* Time to call the callback */
    if (PyCallable_Check(pythonCBFunc)) {
      PyObject* pyProfile = utf32_utf16_decode((TML_TCHAR*)sProfile, -1);
      PyObject* arglist = Py_BuildValue("(OO)", pyProfile, callbackData->pCBData);
      Py_XDECREF(pyProfile);
      PyObject* result = PyEval_CallObject(pythonCBFunc, arglist);
      if (!PyArg_Parse(result, "l", &err)){
        err = TML_ERR_COMMON;
      }
      Py_DECREF(arglist);
      Py_DECREF(result);
    }
  }
  catch (...) {
    PyErr_Print();
    err = TML_ERR_COMMON;
  }
  stderrPrintErr(err, (char*)"onPopulateCallback request");

  // Release calling Python out of a non- python created thread
  // this call must be located after catch(..) to make sure it is called in case of
  // any exception to avoid blocking the GIL
  PyGILState_Release(gstate);
  return err;
}




/**
 * @brief    Callback method for peer registration requests
 */
void FUNC_C_DECL onEvtErrorCallback(TML_CTSTR*  sProfile, TML_CTSTR *sHost, TML_CTSTR *sPort, TML_COMMAND_ID_TYPE iCMD, TML_INT32 iErrorCode, TML_POINTER data)
{
 TML_INT32   err = TML_SUCCESS;

  // Calling Python out of a non- python created thread
  PyGILState_STATE gstate = PyGILState_UNLOCKED;
  try {
    gstate = PyGILState_Ensure();

    PythonCallbackData* callbackData = (PythonCallbackData*) data;

    PyObject *pythonCBFunc = callbackData->pCBFunc;
    /* Time to call the callback */
    if (PyCallable_Check(pythonCBFunc)) {
      PyObject* pyProfile = utf32_utf16_decode((TML_TCHAR*)sProfile, -1);
      PyObject* pyHost    = utf32_utf16_decode((TML_TCHAR*)sHost, -1);
      PyObject* pyPort    = utf32_utf16_decode((TML_TCHAR*)sPort, -1);
      PyObject* arglist = Py_BuildValue("(OOOkkO)", pyProfile, pyHost, pyPort, iCMD, iErrorCode, callbackData->pCBData);
      Py_XDECREF(pyProfile);
      Py_XDECREF(pyHost);
      Py_XDECREF(pyPort);
      PyObject* result = PyEval_CallObject(pythonCBFunc, arglist);
      Py_DECREF(arglist);
      Py_DECREF(result);
    }
  }
  catch (...) {
    PyErr_Print();
    err = TML_ERR_COMMON;
  }
  stderrPrintErr(err, (char*)"onEvtErrorCallback request");

  // Release calling Python out of a non- python created thread
  // this call must be located after catch(..) to make sure it is called in case of
  // any exception to avoid blocking the GIL
  PyGILState_Release(gstate);
}


/**
 * @brief    Callback method for population requests
 */
void FUNC_C_DECL onEvtQueueOverflowCallback(TML_CTSTR*  sProfile, TML_COMMAND_ID_TYPE iCMD, TML_POINTER data)
{
 TML_INT32   err = TML_SUCCESS;
  // Calling Python out of a non- python created thread
  PyGILState_STATE gstate = PyGILState_UNLOCKED;
  try {
    gstate = PyGILState_Ensure();

    PythonCallbackData* callbackData = (PythonCallbackData*) data;

    PyObject *pythonCBFunc = callbackData->pCBFunc;
    /* Time to call the callback */
    if (PyCallable_Check(pythonCBFunc)) {
      PyObject* pyProfile = utf32_utf16_decode((TML_TCHAR*)sProfile, -1);
      PyObject* arglist = Py_BuildValue("(OkO)", pyProfile, iCMD, callbackData->pCBData);
      Py_XDECREF(pyProfile);
      PyObject* result = PyEval_CallObject(pythonCBFunc, arglist);
      Py_DECREF(arglist);
      Py_DECREF(result);
    }
  }
  catch (...) {
    PyErr_Print();
    err = TML_ERR_COMMON;
  }
  stderrPrintErr(err, (char*)"onEvtQueueOverflowCallback request");

  // Release calling Python out of a non- python created thread
  // this call must be located after catch(..) to make sure it is called in case of
  // any exception to avoid blocking the GIL
  PyGILState_Release(gstate);
}


/**
 * @brief    Callback method for recStrm download block
 */
TML_INT32 FUNC_C_DECL onRecStrmDldBlockCallback(TML_STREAM_ID_TYPE iID, TML_POINTER pCBDataDld, TML_POINTER buffer, TML_INT32 bytesRead, TML_INT64 totalBytesRead, TML_INT64 streamSize)
{
 TML_INT32   err = TML_SUCCESS;
 TML_INT32   resValue = -1;
  // Calling Python out of a non- python created thread
  PyGILState_STATE gstate = PyGILState_UNLOCKED;
  try {
    gstate = PyGILState_Ensure();
    PythonReceiverStrmCallbackData* callbackData = (PythonReceiverStrmCallbackData*) pCBDataDld;

    PyObject *pythonCBFunc = callbackData->pCB_Rec_Strm_DLD_Block_Func;
    // Time to call the callback
    if (PyCallable_Check(pythonCBFunc)) {
      // The buffer data is a "binary" char array / so in python #s
#if PY_MAJOR_VERSION < 3
      PyObject* arglist = Py_BuildValue("(LOs#lLL)", iID, callbackData->pCB_Rec_Strm_DLD_Block_Data, buffer, bytesRead, bytesRead, totalBytesRead, streamSize);
#else
      PyObject* arglist = Py_BuildValue("(LOy#lLL)", iID, callbackData->pCB_Rec_Strm_DLD_Block_Data, buffer, bytesRead, bytesRead, totalBytesRead, streamSize);
#endif
      PyObject* result = PyEval_CallObject(pythonCBFunc, arglist);
      if (!PyArg_Parse(result, "l", &resValue)){
        err = TML_ERR_COMMON;
      }
      Py_DECREF(arglist);
      Py_DECREF(result);
    }
  }
  catch (...) {
    PyErr_Print();
    err = TML_ERR_COMMON;
  }
  stderrPrintErr(err, (char*)"onRecStrmDLdBlockCallback request");

  // Release calling Python out of a non- python created thread
  // this call must be located after catch(..) to make sure it is called in case of
  // any exception to avoid blocking the GIL
  PyGILState_Release(gstate);
  if (err != TML_SUCCESS){
    resValue = -1;
  }
  return resValue;
}


/**
 * @brief    Callback method for recStrm download finish
 */
void FUNC_C_DECL onRecStrmDldFinishedCallback(TML_STREAM_ID_TYPE iID, TML_INT32 errCode, TML_POINTER pCBDataDldFinish)
{
 TML_INT32   err = TML_SUCCESS;
  // Calling Python out of a non- python created thread
  PyGILState_STATE gstate = PyGILState_UNLOCKED;
  try {
    gstate = PyGILState_Ensure();
    PythonReceiverStrmCallbackData* callbackData = (PythonReceiverStrmCallbackData*) pCBDataDldFinish;

    PyObject *pythonCBFunc = callbackData->pCB_Rec_Strm_DLD_Finish_Func;
    // Time to call the callback
    if (PyCallable_Check(pythonCBFunc)) {
      PyObject* arglist = Py_BuildValue("(LlO)", iID, errCode, callbackData->pCB_Rec_Strm_DLD_Finish_Data);
      PyObject* result = PyEval_CallObject(pythonCBFunc, arglist);
      Py_DECREF(arglist);
      Py_DECREF(result);
    }
  }
  catch (...) {
    PyErr_Print();
    err = TML_ERR_COMMON;
  }
  stderrPrintErr(err, (char*)"onRecStrmDldFinishedCallback request");

  // Release calling Python out of a non- python created thread
  // this call must be located after catch(..) to make sure it is called in case of
  // any exception to avoid blocking the GIL
  PyGILState_Release(gstate);
}


/**
 * @brief    Callback method for sndStrm close
 */
void FUNC_C_DECL onSndStrmRegisterCloseCallback(TML_STREAM_ID_TYPE iID, TML_POINTER data)
{
 TML_INT32   err = TML_SUCCESS;
  // Calling Python out of a non- python created thread
  PyGILState_STATE gstate = PyGILState_UNLOCKED;
  try {
    gstate = PyGILState_Ensure();

    PythonSenderStrmCallbackData* callbackData = (PythonSenderStrmCallbackData*) data;

    PyObject *pythonCBFunc = callbackData->pCB_OnClose_Func;
    /* Time to call the callback */
    if (PyCallable_Check(pythonCBFunc)) {
      PyObject* arglist = Py_BuildValue("(LO)", iID, callbackData->pCB_OnClose_Data);
      PyObject* result = PyEval_CallObject(pythonCBFunc, arglist);
      Py_DECREF(arglist);
      Py_DECREF(result);
    }
  }
  catch (...) {
    PyErr_Print();
    err = TML_ERR_COMMON;
  }
  stderrPrintErr(err, (char*)"onSndStrmRegisterCloseCallback request");

  // Release calling Python out of a non- python created thread
  // this call must be located after catch(..) to make sure it is called in case of
  // any exception to avoid blocking the GIL
  PyGILState_Release(gstate);
}


/**
 * @brief    Callback method for sndStrm getPosition
 */
TML_INT64 FUNC_C_DECL onSndStrmRegisterGetPositionCallback(TML_STREAM_ID_TYPE iID, TML_POINTER data)
{
 TML_INT32   err = TML_SUCCESS;
 TML_INT64   positionValue = -1;
  // Calling Python out of a non- python created thread
  PyGILState_STATE gstate = PyGILState_UNLOCKED;
  try {
    gstate = PyGILState_Ensure();

    PythonSenderStrmCallbackData* callbackData = (PythonSenderStrmCallbackData*) data;

    PyObject *pythonCBFunc = callbackData->pCB_OnGetPosition_Func;
    /* Time to call the callback */
    if (PyCallable_Check(pythonCBFunc)) {
      PyObject* arglist = Py_BuildValue("(LO)", iID, callbackData->pCB_OnGetPosition_Data);
      PyObject* result = PyEval_CallObject(pythonCBFunc, arglist);
      if (!PyArg_Parse(result, "L", &positionValue)){
        err = TML_ERR_COMMON;
      }
      Py_DECREF(arglist);
      Py_DECREF(result);
    }
  }
  catch (...) {
    PyErr_Print();
    err = TML_ERR_COMMON;
  }
  stderrPrintErr(err, (char*)"onSndStrmRegisterGetPositionCallback request");

  // Release calling Python out of a non- python created thread
  // this call must be located after catch(..) to make sure it is called in case of
  // any exception to avoid blocking the GIL
  PyGILState_Release(gstate);
  if (err != TML_SUCCESS){
    positionValue = -1;
  }
  return positionValue;
}


/**
 * @brief    Callback method for sndStrm getSize
 */
TML_INT64 FUNC_C_DECL onSndStrmRegisterGetSizeCallback(TML_STREAM_ID_TYPE iID, TML_POINTER data)
{
 TML_INT32   err = TML_SUCCESS;
 TML_INT64   sizeValue = -1;
  // Calling Python out of a non- python created thread
  PyGILState_STATE gstate = PyGILState_UNLOCKED;
  try {
    gstate = PyGILState_Ensure();

    PythonSenderStrmCallbackData* callbackData = (PythonSenderStrmCallbackData*) data;

    PyObject *pythonCBFunc = callbackData->pCB_OnGetSize_Func;
    /* Time to call the callback */
    if (PyCallable_Check(pythonCBFunc)) {
      PyObject* arglist = Py_BuildValue("(LO)", iID, callbackData->pCB_OnGetSize_Data);
      PyObject* result = PyEval_CallObject(pythonCBFunc, arglist);
      if (!PyArg_Parse(result, "L", &sizeValue)){
        err = TML_ERR_COMMON;
      }
      Py_DECREF(arglist);
      Py_DECREF(result);
    }
  }
  catch (...) {
    PyErr_Print();
    err = TML_ERR_COMMON;
  }
  stderrPrintErr(err, (char*)"onSndStrmRegisterGetSizeCallback request");

  // Release calling Python out of a non- python created thread
  // this call must be located after catch(..) to make sure it is called in case of
  // any exception to avoid blocking the GIL
  PyGILState_Release(gstate);
  if (err != TML_SUCCESS){
    sizeValue = -1;
  }
  return sizeValue;
}


/**
 * @brief    Callback method for sndStrm onError
 */
void FUNC_C_DECL onSndStrmRegisterOnErrorCallback(TML_STREAM_ID_TYPE iID, TML_INT32 iError, TML_POINTER data)
{
 TML_INT32   err = TML_SUCCESS;
  // Calling Python out of a non- python created thread
  PyGILState_STATE gstate = PyGILState_UNLOCKED;
  try {
    gstate = PyGILState_Ensure();

    PythonSenderStrmCallbackData* callbackData = (PythonSenderStrmCallbackData*) data;

    PyObject *pythonCBFunc = callbackData->pCB_OnError_Func;
    /* Time to call the callback */
    if (PyCallable_Check(pythonCBFunc)) {
      PyObject* arglist = Py_BuildValue("(LlO)", iID, iError, callbackData->pCB_OnError_Data);
      PyObject* result = PyEval_CallObject(pythonCBFunc, arglist);
      Py_DECREF(arglist);
      Py_DECREF(result);
    }
  }
  catch (...) {
    PyErr_Print();
    err = TML_ERR_COMMON;
  }
  stderrPrintErr(err, (char*)"onSndStrmOnErrorCallback request");

  // Release calling Python out of a non- python created thread
  // this call must be located after catch(..) to make sure it is called in case of
  // any exception to avoid blocking the GIL
  PyGILState_Release(gstate);
}


/**
 * @brief    Callback method for sndStrm read
 */
TML_INT32 FUNC_C_DECL onSndStrmRegisterReadCallback(TML_STREAM_ID_TYPE iID, TML_POINTER data, TML_POINTER buffer, TML_INT32 count, TML_INT32 *bytesRead)
{
 TML_INT32   err = TML_SUCCESS;
 PyObject*   resState = Py_False;
 char*       resBuffer = NULL;
 TML_INT32   resBytesRead = 0;
  // Calling Python out of a non- python created thread
  PyGILState_STATE gstate = PyGILState_UNLOCKED;
  try {
    gstate = PyGILState_Ensure();

    PythonSenderStrmCallbackData* callbackData = (PythonSenderStrmCallbackData*) data;

    PyObject *pythonCBFunc = callbackData->pCB_OnRead_Func;
    /* Time to call the callback */
    if (PyCallable_Check(pythonCBFunc)) {
      PyObject* arglist = Py_BuildValue("(LOl)", iID, callbackData->pCB_OnRead_Data, count);
      PyObject* result = PyEval_CallObject(pythonCBFunc, arglist);
      PyObject* pyBuffer;
      if (!PyArg_ParseTuple(result, "OO", &resState, &pyBuffer)){
        err = TML_ERR_COMMON;
      }
      else{
        if (Py_True == resState){
          PyObject* tuple = Py_BuildValue("(O)", pyBuffer);
#if PY_MAJOR_VERSION < 3
          if (!PyArg_ParseTuple(tuple, "s#", &resBuffer, &resBytesRead)){
#else
          if (!PyArg_ParseTuple(tuple, "y#", &resBuffer, &resBytesRead)){
#endif
            err = TML_ERR_COMMON;
          }
          else{
            *bytesRead = resBytesRead;
            for (TML_INT32 i = 0; i < resBytesRead; ++i){
              ((char*)buffer)[i] = resBuffer[i];
            }
          }
          Py_XDECREF(tuple);
        }
      }

      Py_DECREF(arglist);
      Py_DECREF(result);
    }
  }
  catch (...) {
    PyErr_Print();
    err = TML_ERR_COMMON;
  }
  stderrPrintErr(err, (char*)"onSndStrmRegisterReadCallback request");

  // Release calling Python out of a non- python created thread
  // this call must be located after catch(..) to make sure it is called in case of
  // any exception to avoid blocking the GIL
  PyGILState_Release(gstate);
  if (err != TML_SUCCESS){
    resState = Py_False;
  }
  return (resState == Py_True) ? 0 : -1;
}


/**
 * @brief    Callback method for sndStrm seek
 */
TML_INT32 FUNC_C_DECL onSndStrmRegisterSeekCallback(TML_STREAM_ID_TYPE iID, TML_POINTER data, TML_INT64 seekPosition, tmlSeekOrigin seekOrigin)
{
 TML_INT32   err = TML_SUCCESS;
 PyObject*   resState = Py_False;
  // Calling Python out of a non- python created thread
  PyGILState_STATE gstate = PyGILState_UNLOCKED;
  try {
    gstate = PyGILState_Ensure();

    PythonSenderStrmCallbackData* callbackData = (PythonSenderStrmCallbackData*) data;

    PyObject *pythonCBFunc = callbackData->pCB_OnSeek_Func;
    /* Time to call the callback */
    if (PyCallable_Check(pythonCBFunc)) {
      PyObject* arglist = Py_BuildValue("(LOLl)", iID, callbackData->pCB_OnSeek_Data, seekPosition, seekOrigin);
      PyObject* result = PyEval_CallObject(pythonCBFunc, arglist);
      if (!PyArg_Parse(result, "O", &resState)){
        err = TML_ERR_COMMON;
      }
      Py_DECREF(arglist);
      Py_DECREF(result);
    }
  }
  catch (...) {
    PyErr_Print();
    err = TML_ERR_COMMON;
  }
  stderrPrintErr(err, (char*)"onSndStrmRegisterSeekCallback request");

  // Release calling Python out of a non- python created thread
  // this call must be located after catch(..) to make sure it is called in case of
  // any exception to avoid blocking the GIL
  PyGILState_Release(gstate);
  if (err != TML_SUCCESS){
    resState = Py_False;
  }
  return (resState == Py_True) ? 0 : -1;
}


/**
 * @brief    Callback method for sndStrm write
 */
TML_INT32 FUNC_C_DECL onSndStrmRegisterWriteCallback(TML_STREAM_ID_TYPE iID, TML_POINTER buffer, TML_INT32 count, TML_POINTER data)
{
 TML_INT32   err = TML_SUCCESS;
 PyObject*   resState = Py_False;
  // Calling Python out of a non- python created thread
  PyGILState_STATE gstate = PyGILState_UNLOCKED;
  try {
    gstate = PyGILState_Ensure();

    PythonSenderStrmCallbackData* callbackData = (PythonSenderStrmCallbackData*) data;

    PyObject *pythonCBFunc = callbackData->pCB_OnWrite_Func;
    /* Time to call the callback */
    if (PyCallable_Check(pythonCBFunc)) {
      // Binary char string is represented by 
      // "s#", buffer, count
#if PY_MAJOR_VERSION < 3
      PyObject* arglist = Py_BuildValue("(Ls#O)", iID, buffer, count, callbackData->pCB_OnWrite_Data);
#else
      PyObject* arglist = Py_BuildValue("(Ly#O)", iID, buffer, count, callbackData->pCB_OnWrite_Data);
#endif
      PyObject* result = PyEval_CallObject(pythonCBFunc, arglist);
      if (!PyArg_Parse(result, "O", &resState)){
        err = TML_ERR_COMMON;
      }
      Py_DECREF(arglist);
      Py_DECREF(result);
    }
  }
  catch (...) {
    PyErr_Print();
    err = TML_ERR_COMMON;
  }
  stderrPrintErr(err, (char*)"onSndStrmRegisterWriteCallback request");

  // Release calling Python out of a non- python created thread
  // this call must be located after catch(..) to make sure it is called in case of
  // any exception to avoid blocking the GIL
  PyGILState_Release(gstate);
  if (err != TML_SUCCESS){
    resState = Py_False;
  }
  return (resState == Py_True) ? 0 : -1;
}


/*
 *------------------------------------------------------
 *               tml_Core_Get_Version
 *------------------------------------------------------
 */
static PyObject * _tml_Core_Get_Version(PyObject *self)
{
    TML_INT32   apiVer = 0;
    TML_INT32   libVer  = 0;
    TML_TCHAR*  libStr;


    TML_INT32   err = TML_SUCCESS;

    Py_BEGIN_ALLOW_THREADS;
    try {
        tml_Core_Get_Version(&apiVer, &libVer, &libStr);
    }
    catch (...) {
        err = TML_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Core_Get_Version failed (%d)",err);
            return NULL;
    }
    
    PyObject* pyLibStr = utf32_utf16_decode(libStr, -1);
    PyObject* result = Py_BuildValue("(l,l,O)", apiVer, libVer, pyLibStr);
    Py_XDECREF(pyLibStr);

    return result;
}


/*
 *------------------------------------------------------
 *               tml_Core_Get_Copyright
 *------------------------------------------------------
 */
static PyObject * _tml_Core_Get_Copyright(PyObject *self, PyObject *args)
{
    TML_TCHAR*  sresult;
    TML_INT32   slength;

    TML_INT32    err = TML_SUCCESS;

    Py_BEGIN_ALLOW_THREADS;
    try {
        tml_Core_Get_Copyright(&sresult, &slength);
    }
    catch (...) {
        err = TML_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Core_Get_Copyright failed (%d)",err);
            return NULL;
    }

    PyObject* pyCopyright = utf32_utf16_decode(sresult, slength);
    PyObject* pRetVal = Py_BuildValue("O#",pyCopyright, slength);
    Py_XDECREF(pyCopyright);

    return pRetVal;
}

/*
 *------------------------------------------------------
 *               tml_Core_Set_Password
 *------------------------------------------------------
 */
static PyObject * _tml_Core_Set_Password(PyObject *self, PyObject *args)
{
    PyObject*   py_UserName16;
    PyObject*   py_PassWord16;
    TML_TCHAR*  pUserName;
    TML_TCHAR*  pPassWord;
    PyObject*   py_UserName;
    PyObject*   py_PassWord;
    
    TML_INT32   err = TML_SUCCESS;

    if (!PyArg_ParseTuple(args, "OO", &py_UserName, &py_PassWord))
        return NULL;
    if ( _unicodeConversion(py_UserName, &pUserName, &py_UserName16, tmlError) ) return NULL;
    if ( _unicodeConversion(py_PassWord, &pPassWord, &py_PassWord16, tmlError) ) return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = tml_Core_Set_Password(pUserName, pPassWord);
    }
    catch (...) {
        err = TML_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    unicodeFree(py_UserName16, pUserName);
    unicodeFree(py_PassWord16, pPassWord);

    if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Core_Set_Password failed (%d)",err);
            return NULL;
    }

    Py_RETURN_NONE;
}

/*
 *------------------------------------------------------
 *               tml_Core_Set_LoggingValue
 *------------------------------------------------------
 */
static PyObject * _tml_Core_Set_LoggingValue(PyObject *self, PyObject *args)
{
    TML_CORE_HANDLE  shandle;
    TML_INT32        iLogValue;
    TML_INT64        iTemp64; // Umgehung von Speicherzugriffsverletzung bei Python 3.4 unter LINUX x64

    TML_INT32   err = TML_SUCCESS;

    if (!PyArg_ParseTuple(args, "Ll", &shandle, &iTemp64))
        return NULL;
    iLogValue = (TML_INT32)iTemp64;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = tml_Core_Set_LoggingValue(shandle, iLogValue);
    }
    catch (...) {
        err = TML_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Core_Set_LoggingValue failed (%d)",err);
            return NULL;
    }

    Py_RETURN_NONE;
}


/*
 *------------------------------------------------------
 *               tml_Core_Get_LoggingValue
 *------------------------------------------------------
 */
static PyObject * _tml_Core_Get_LoggingValue(PyObject *self, PyObject *args)
{
    TML_CORE_HANDLE  shandle;
    TML_INT32 iLogValue = 0;

    TML_INT32   err = TML_SUCCESS;

    if (!PyArg_ParseTuple(args, "L", &shandle))
        return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = tml_Core_Get_LoggingValue(shandle, &iLogValue);
    }
    catch (...) {
        err = TML_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Core_Get_LoggingValue failed (%d)",err);
            return NULL;
    }

    return Py_BuildValue("l",iLogValue);
}

/*
 *------------------------------------------------------
 *               tml_Core_Open
 *------------------------------------------------------
 */
static PyObject * _tml_Core_Open(PyObject *self, PyObject *args)
{
    TML_INT32        iLogValue;
    TML_INT64        iTemp64; // Umgehung von Speicherzugriffsverletzung bei Python 3.4 unter LINUX x64
    TML_CORE_HANDLE  shandle;

    TML_INT32   err = TML_SUCCESS;

    if (!PyArg_ParseTuple(args, "l", &iTemp64))
        return NULL;
    iLogValue = (TML_INT32)iTemp64;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = tml_Core_Open(&shandle, iLogValue);
    }
    catch (...) {
        err = TML_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Core_Open failed (%d)",err);
            return NULL;
    }

    m_cmdDispatchDict            = sidex_Variant_New_Dict();
    m_customDispatchDict         = sidex_Variant_New_Dict();
    m_deleteCmdDict              = sidex_Variant_New_Dict();
    m_balOnBusyStatusRequestDict = sidex_Variant_New_Dict();
    m_balOnCalculationDict       = sidex_Variant_New_Dict();
    m_balOnPeerRegisterDict      = sidex_Variant_New_Dict();
    m_balOnPopulateDict          = sidex_Variant_New_Dict();
    m_evtOnErrorDict             = sidex_Variant_New_Dict();
    m_evtOnPeerRegisterDict      = sidex_Variant_New_Dict();
    m_evtOnPopulateDict          = sidex_Variant_New_Dict();
    m_evtOnQueueOverflowDict     = sidex_Variant_New_Dict();
    m_receiverStrmDict           = sidex_Variant_New_Dict();
    m_senderStrmDict             = sidex_Variant_New_Dict();

    return Py_BuildValue("L",shandle);
}

/*
 *------------------------------------------------------
 *               tml_Core_Close
 *------------------------------------------------------
 */
static PyObject * _tml_Core_Close(PyObject *self, PyObject *args)
{
    TML_CORE_HANDLE  shandle;

    TML_INT32   err = TML_SUCCESS;

    if (!PyArg_ParseTuple(args, "L", &shandle))
        return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = tml_Core_Close(&shandle);
    }
    catch (...) {
        err = TML_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Core_Close failed (%d)",err);
            return NULL;
    }
    releaseCommandDispatchCallbackData(true);
    releaseCallbackDataDict(m_customDispatchDict, true);
    releaseCallbackDataDict(m_deleteCmdDict, true);
    releaseCallbackDataDict(m_balOnBusyStatusRequestDict, true);
    releaseCallbackDataDict(m_balOnCalculationDict, true);
    releaseCallbackDataDict(m_balOnPeerRegisterDict, true);
    releaseCallbackDataDict(m_balOnPopulateDict, true);
    releaseCallbackDataDict(m_evtOnErrorDict, true);
    releaseCallbackDataDict(m_evtOnPeerRegisterDict, true);
    releaseCallbackDataDict(m_evtOnPopulateDict, true);
    releaseCallbackDataDict(m_evtOnQueueOverflowDict, true);
    removeAllRecStrmCallbackDataFromDict(m_receiverStrmDict, true);
    removeAllSndStrmCallbackDataFromDict(m_senderStrmDict, true);
    Py_RETURN_NONE;
}

/*
 *------------------------------------------------------
 *               tml_Core_GeneralDeregistration
 *------------------------------------------------------
 */
static PyObject * _tml_Core_GeneralDeregistration(PyObject *self, PyObject *args)
{
    TML_CORE_HANDLE  shandle;

    TML_INT32   err = TML_SUCCESS;

    if (!PyArg_ParseTuple(args, "L", &shandle))
        return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = tml_Core_GeneralDeregistration(shandle);
    }
    catch (...) {
        err = TML_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Core_GeneralDeregistration failed (%d)",err);
            return NULL;
    }
    releaseCommandDispatchCallbackData(false);
    releaseCallbackDataDict(m_customDispatchDict, false);
    releaseCallbackDataDict(m_deleteCmdDict, false);
    releaseCallbackDataDict(m_balOnBusyStatusRequestDict, false);
    releaseCallbackDataDict(m_balOnCalculationDict, false);
    releaseCallbackDataDict(m_balOnPeerRegisterDict, false);
    releaseCallbackDataDict(m_balOnPopulateDict, false);
    releaseCallbackDataDict(m_evtOnErrorDict, false);
    releaseCallbackDataDict(m_evtOnPeerRegisterDict, false);
    releaseCallbackDataDict(m_evtOnPopulateDict, false);
    releaseCallbackDataDict(m_evtOnQueueOverflowDict, false);
    removeAllRecStrmCallbackDataFromDict(m_receiverStrmDict, false);
    removeAllSndStrmCallbackDataFromDict(m_senderStrmDict, false);
    Py_RETURN_NONE;
}

/*
 *------------------------------------------------------
 *               tml_Cmd_Create
 *------------------------------------------------------
 */
static PyObject * _tml_Cmd_Create(PyObject *self)
{
    TML_COMMAND_HANDLE  shandle;

    TML_INT32   err = TML_SUCCESS;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = tml_Cmd_Create(&shandle);
    }
    catch (...) {
        err = TML_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Cmd_Create failed (%d)",err);
            return NULL;
    }

    return Py_BuildValue("L",shandle);
}

/*
 *------------------------------------------------------
 *               tml_Cmd_Free
 *------------------------------------------------------
 */
static PyObject * _tml_Cmd_Free(PyObject *self, PyObject *args)
{
    TML_COMMAND_HANDLE  shandle;

    TML_INT32   err = TML_SUCCESS;

    if (!PyArg_ParseTuple(args, "L", &shandle))
        return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        PythonCallbackData*           progressCallbackData = NULL;
        TML_ON_PROGRESS_REPLY_CB_FUNC pCBFuncProgress;
        PythonCallbackData*           statusCallbackData   = NULL;
        TML_ON_STATUS_REPLY_CB_FUNC   pCBFuncStatus;
        PythonCallbackData*           commandReadyCallbackData = NULL;
        TML_ON_COMMAND_READY_CB_FUNC pCBFuncCommandReady;
        TML_POINTER pCBData;

        err = tml_Cmd_Registered_Progress (shandle, &pCBFuncProgress, &pCBData);
        if (NULL != pCBData){
          progressCallbackData = (PythonCallbackData*) pCBData;
          Py_XDECREF(progressCallbackData->pCBFunc);
          Py_XDECREF(progressCallbackData->pCBData);
        }
        err = tml_Cmd_Registered_StatusReply (shandle, &pCBFuncStatus, &pCBData);
        if (NULL != pCBData){
          statusCallbackData = (PythonCallbackData*) pCBData;
          Py_XDECREF(statusCallbackData->pCBFunc);
          Py_XDECREF(statusCallbackData->pCBData);
        }
        err = tml_Cmd_Registered_CommandReady (shandle, &pCBFuncCommandReady, &pCBData);
        if (NULL != pCBData){
          commandReadyCallbackData = (PythonCallbackData*) pCBData;
          Py_XDECREF(commandReadyCallbackData->pCBFunc);
          Py_XDECREF(commandReadyCallbackData->pCBData);
        }
        err = tml_Cmd_Free(&shandle);
        if (NULL != progressCallbackData){
          delete progressCallbackData;
        }
        if (NULL != statusCallbackData){
          delete statusCallbackData;
        }
        if (NULL != commandReadyCallbackData){
          delete commandReadyCallbackData;
        }
    }
    catch (...) {
        err = TML_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Cmd_Free failed (%d)",err);
            return NULL;
    }

    Py_RETURN_NONE;
}

/*
 *------------------------------------------------------
 *               tml_Cmd_Acquire_Sidex_Handle
 *------------------------------------------------------
 */
static PyObject * _tml_Cmd_Acquire_Sidex_Handle(PyObject *self, PyObject *args)
{
    TML_COMMAND_HANDLE  cmdHandle;
    SIDEX_HANDLE        shandle; 

    TML_INT32   err = TML_SUCCESS;

    if (!PyArg_ParseTuple(args, "L", &cmdHandle))
        return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = tml_Cmd_Acquire_Sidex_Handle(cmdHandle, &shandle);
    }
    catch (...) {
        err = TML_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Cmd_Acquire_Sidex_Handle failed (%d)",err);
            return NULL;
    }

    return Py_BuildValue("L",shandle);
}

/*
 *------------------------------------------------------
 *               tml_Cmd_Release_Sidex_Handle
 *------------------------------------------------------
 */
static PyObject * _tml_Cmd_Release_Sidex_Handle(PyObject *self, PyObject *args)
{
    TML_COMMAND_HANDLE  cmdHandle;

    TML_INT32   err = TML_SUCCESS;

    if (!PyArg_ParseTuple(args, "L", &cmdHandle))
        return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = tml_Cmd_Release_Sidex_Handle(cmdHandle);
    }
    catch (...) {
        err = TML_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Cmd_Release_Sidex_Handle failed (%d)",err);
            return NULL;
    }

    Py_RETURN_NONE;
}

/*
 *------------------------------------------------------
 *               tml_Cmd_Header_SetCommand 
 *------------------------------------------------------
 */
static PyObject * _tml_Cmd_Header_SetCommand (PyObject *self, PyObject *args)
{
    TML_COMMAND_HANDLE  cmdHandle;
    TML_UINT64          iTemp64u; // Umgehung von Speicherzugriffsverletzung bei Python 3.4 unter LINUX x64
    TML_COMMAND_ID_TYPE iCmdIdentifier;

    TML_INT32   err = TML_SUCCESS;

    if (!PyArg_ParseTuple(args, "Lk", &cmdHandle, &iTemp64u))
        return NULL;
    iCmdIdentifier = (TML_COMMAND_ID_TYPE)iTemp64u;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = tml_Cmd_Header_SetCommand (cmdHandle, iCmdIdentifier);
    }
    catch (...) {
        err = TML_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Cmd_Header_SetCommand  failed (%d)",err);
            return NULL;
    }

    Py_RETURN_NONE;
}

/*
 *------------------------------------------------------
 *               tml_Cmd_Header_GetCommand 
 *------------------------------------------------------
 */
static PyObject * _tml_Cmd_Header_GetCommand (PyObject *self, PyObject *args)
{
    TML_COMMAND_HANDLE  cmdHandle;
    TML_COMMAND_ID_TYPE iCmdIdentifier;

    TML_INT32   err = TML_SUCCESS;

    if (!PyArg_ParseTuple(args, "L", &cmdHandle))
        return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = tml_Cmd_Header_GetCommand (cmdHandle, &iCmdIdentifier);
    }
    catch (...) {
        err = TML_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Cmd_Header_GetCommand  failed (%d)",err);
            return NULL;
    }

    return Py_BuildValue("k",iCmdIdentifier);
}

/*
 *------------------------------------------------------
 *               tml_Cmd_Header_GetCreationTime 
 *------------------------------------------------------
 */
static PyObject * _tml_Cmd_Header_GetCreationTime (PyObject *self, PyObject *args)
{
    TML_COMMAND_HANDLE  cmdHandle;
    TML_TCHAR*          sTime;
    PyObject *          result;

    TML_INT32   err = TML_SUCCESS;

    if (!PyArg_ParseTuple(args, "L", &cmdHandle))
        return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = tml_Cmd_Header_GetCreationTime (cmdHandle, &sTime);
    }
    catch (...) {
        err = TML_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Cmd_Header_GetCreationTime  failed (%d)",err);
            return NULL;
    }

    result = utf32_utf16_decode(sTime, -1);
    sidex_Free_ReadString((wchar_t*)sTime);
    return result;
}

/*
 *------------------------------------------------------
 *               tml_Cmd_Header_SetError 
 *------------------------------------------------------
 */
static PyObject * _tml_Cmd_Header_SetError (PyObject *self, PyObject *args)
{
    TML_COMMAND_HANDLE  cmdHandle;
    TML_INT64           iTemp64; // Umgehung von Speicherzugriffsverletzung bei Python 3.4 unter LINUX x64
    TML_COMMAND_ID_TYPE iErrorCode;

    TML_INT32   err = TML_SUCCESS;

    if (!PyArg_ParseTuple(args, "Ll", &cmdHandle, &iTemp64))
        return NULL;
    iErrorCode = (TML_COMMAND_ID_TYPE) iTemp64;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = tml_Cmd_Header_SetError (cmdHandle, iErrorCode);
    }
    catch (...) {
        err = TML_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Cmd_Header_SetError  failed (%d)",err);
            return NULL;
    }

    Py_RETURN_NONE;
}

/*
 *------------------------------------------------------
 *               tml_Cmd_Header_GetError 
 *------------------------------------------------------
 */
static PyObject * _tml_Cmd_Header_GetError (PyObject *self, PyObject *args)
{
    TML_COMMAND_HANDLE  cmdHandle;
    TML_INT32 iErrorCode;

    TML_INT32   err = TML_SUCCESS;

    if (!PyArg_ParseTuple(args, "L", &cmdHandle))
        return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = tml_Cmd_Header_GetError (cmdHandle, &iErrorCode);
    }
    catch (...) {
        err = TML_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Cmd_Header_GetError  failed (%d)",err);
            return NULL;
    }

    return Py_BuildValue("l",iErrorCode);
}

/*
 *------------------------------------------------------
 *               tml_Cmd_Header_SetErrorMessage 
 *------------------------------------------------------
 */
static PyObject * _tml_Cmd_Header_SetErrorMessage (PyObject *self, PyObject *args)
{
    TML_COMMAND_HANDLE  cmdHandle;
    PyObject*           py_ErrorMessage16;
    TML_TCHAR*          pErrorMessage;
    PyObject*           py_ErrorMessage;

    TML_INT32   err = TML_SUCCESS;

    if (!PyArg_ParseTuple(args, "LO", &cmdHandle, &py_ErrorMessage))
        return NULL;
    if ( _unicodeConversion(py_ErrorMessage, &pErrorMessage, &py_ErrorMessage16, tmlError) ) return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        TML_INT32 iSize;
        iSize = (TML_INT32) wcslen(pErrorMessage);

        err = tml_Cmd_Header_SetErrorMessage (cmdHandle, pErrorMessage, iSize);
    }
    catch (...) {
        err = TML_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    unicodeFree(py_ErrorMessage16, pErrorMessage);

    if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Cmd_Header_SetErrorMessage  failed (%d)",err);
            return NULL;
    }

    Py_RETURN_NONE;
}

/*
 *------------------------------------------------------
 *               tml_Cmd_Header_GetErrorMessage 
 *------------------------------------------------------
 */
static PyObject * _tml_Cmd_Header_GetErrorMessage (PyObject *self, PyObject *args)
{
    TML_COMMAND_HANDLE  cmdHandle;
    TML_TCHAR* sErrorMessage;
    PyObject *    result;

    TML_INT32   err = TML_SUCCESS;

    if (!PyArg_ParseTuple(args, "L", &cmdHandle))
        return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        SIDEX_INT32 iSize;
        err = tml_Cmd_Header_GetErrorMessage (cmdHandle, &sErrorMessage, &iSize);
    }
    catch (...) {
        err = TML_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Cmd_Header_GetErrorMessage  failed (%d)",err);
            return NULL;
    }

    result = utf32_utf16_decode(sErrorMessage, -1);
    sidex_Free_ReadString((wchar_t*)sErrorMessage);
    return result;
}

/*
 *------------------------------------------------------
 *               tml_Cmd_Header_SetMode 
 *------------------------------------------------------
 */
static PyObject * _tml_Cmd_Header_SetMode (PyObject *self, PyObject *args)
{
    TML_COMMAND_HANDLE  cmdHandle;
    TML_INT32           mode;
    TML_INT64          iTemp64; // Umgehung von Speicherzugriffsverletzung bei Python 3.4 unter LINUX x64

    TML_INT32   err = TML_SUCCESS;

   if (!PyArg_ParseTuple(args, "Ll", &cmdHandle, &iTemp64))
        return NULL;
   mode = (TML_INT32) iTemp64;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = tml_Cmd_Header_SetMode (cmdHandle, mode);
    }
    catch (...) {
        err = TML_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Cmd_Header_SetMode  failed (%d)",err);
            return NULL;
    }

    Py_RETURN_NONE;
}

/*
 *------------------------------------------------------
 *               tml_Cmd_Header_GetMode 
 *------------------------------------------------------
 */
static PyObject * _tml_Cmd_Header_GetMode (PyObject *self, PyObject *args)
{
    TML_COMMAND_HANDLE  cmdHandle;
    TML_INT32           mode;

    TML_INT32   err = TML_SUCCESS;

    if (!PyArg_ParseTuple(args, "L", &cmdHandle))
        return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = tml_Cmd_Header_GetMode (cmdHandle, &mode);
    }
    catch (...) {
        err = TML_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Cmd_Header_GetMode  failed (%d)",err);
            return NULL;
    }

    return Py_BuildValue("l",mode);
}

/*
 *------------------------------------------------------
 *               tml_Cmd_Header_SetProgress 
 *------------------------------------------------------
 */
static PyObject * _tml_Cmd_Header_SetProgress (PyObject *self, PyObject *args)
{
    TML_COMMAND_HANDLE  cmdHandle;
    TML_INT32           progress;
    TML_INT64           iTemp64; // Umgehung von Speicherzugriffsverletzung bei Python 3.4 unter LINUX x64

    TML_INT32   err = TML_SUCCESS;

    if (!PyArg_ParseTuple(args, "Ll", &cmdHandle, &iTemp64))
        return NULL;
    progress = (TML_INT32)iTemp64;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = tml_Cmd_Header_SetProgress (cmdHandle, progress);
    }
    catch (...) {
        err = TML_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Cmd_Header_SetProgress  failed (%d)",err);
            return NULL;
    }

    Py_RETURN_NONE;
}

/*
 *------------------------------------------------------
 *               tml_Cmd_Header_GetProgress 
 *------------------------------------------------------
 */
static PyObject * _tml_Cmd_Header_GetProgress (PyObject *self, PyObject *args)
{
    TML_COMMAND_HANDLE  cmdHandle;
    TML_INT32           progress;

    TML_INT32   err = TML_SUCCESS;

    if (!PyArg_ParseTuple(args, "L", &cmdHandle))
        return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = tml_Cmd_Header_GetProgress (cmdHandle, &progress);
    }
    catch (...) {
        err = TML_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Cmd_Header_GetProgress  failed (%d)",err);
            return NULL;
    }

    return Py_BuildValue("l",progress);
}

/*
 *------------------------------------------------------
 *               tml_Cmd_Header_SetReplyMessage 
 *------------------------------------------------------
 */
static PyObject * _tml_Cmd_Header_SetReplyMessage (PyObject *self, PyObject *args)
{
    TML_COMMAND_HANDLE  cmdHandle;
    PyObject*           py_ReplyMessage16;
    TML_TCHAR*          sReplyMessage;
    PyObject*           py_ReplyMessage;

    TML_INT32   err = TML_SUCCESS;

    if (!PyArg_ParseTuple(args, "LO", &cmdHandle, &py_ReplyMessage))
        return NULL;
    if ( _unicodeConversion(py_ReplyMessage, &sReplyMessage, &py_ReplyMessage16, tmlError) ) return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        TML_INT32 iSize;
        iSize = (TML_INT32) wcslen(sReplyMessage);

        err = tml_Cmd_Header_SetReplyMessage (cmdHandle, sReplyMessage, iSize);
    }
    catch (...) {
        err = TML_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    unicodeFree(py_ReplyMessage16, sReplyMessage);

    if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Cmd_Header_SetReplyMessage  failed (%d)",err);
            return NULL;
    }

    Py_RETURN_NONE;
}

/*
 *------------------------------------------------------
 *               tml_Cmd_Header_GetReplyMessage 
 *------------------------------------------------------
 */
static PyObject * _tml_Cmd_Header_GetReplyMessage (PyObject *self, PyObject *args)
{
    TML_COMMAND_HANDLE  cmdHandle;
    TML_TCHAR*          sReplyMessage;
    PyObject *          result;

    TML_INT32   err = TML_SUCCESS;

    if (!PyArg_ParseTuple(args, "L", &cmdHandle))
        return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        SIDEX_INT32 iSize;
        err = tml_Cmd_Header_GetReplyMessage (cmdHandle, &sReplyMessage, &iSize);
    }
    catch (...) {
        err = TML_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Cmd_Header_GetReplyMessage  failed (%d)",err);
            return NULL;
    }

    result = utf32_utf16_decode(sReplyMessage, -1);

    sidex_Free_ReadString((wchar_t*)sReplyMessage);
    return result;
}

/*
 *------------------------------------------------------
 *               tml_Cmd_Header_SetReplyType 
 *------------------------------------------------------
 */
static PyObject * _tml_Cmd_Header_SetReplyType (PyObject *self, PyObject *args)
{
    TML_COMMAND_HANDLE  cmdHandle;
    TML_INT32           type;
    TML_INT64           iTemp64; // Umgehung von Speicherzugriffsverletzung bei Python 3.4 unter LINUX x64

    TML_INT32   err = TML_SUCCESS;

    if (!PyArg_ParseTuple(args, "Ll", &cmdHandle, &iTemp64))
        return NULL;
    type = (TML_INT32)iTemp64;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = tml_Cmd_Header_SetReplyType (cmdHandle, type);
    }
    catch (...) {
        err = TML_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Cmd_Header_SetReplyType  failed (%d)",err);
            return NULL;
    }

    Py_RETURN_NONE;
}

/*
 *------------------------------------------------------
 *               tml_Cmd_Header_GetReplyType 
 *------------------------------------------------------
 */
static PyObject * _tml_Cmd_Header_GetReplyType (PyObject *self, PyObject *args)
{
    TML_COMMAND_HANDLE  cmdHandle;
    TML_INT32           type;

    TML_INT32   err = TML_SUCCESS;

    if (!PyArg_ParseTuple(args, "L", &cmdHandle))
        return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = tml_Cmd_Header_GetReplyType (cmdHandle, &type);
    }
    catch (...) {
        err = TML_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Cmd_Header_GetReplyType  failed (%d)",err);
            return NULL;
    }

    return Py_BuildValue("l",type);
}

/*
 *------------------------------------------------------
 *               tml_Cmd_Header_SetState 
 *------------------------------------------------------
 */
static PyObject * _tml_Cmd_Header_SetState (PyObject *self, PyObject *args)
{
    TML_COMMAND_HANDLE  cmdHandle;
    TML_INT32           state;
    TML_INT64           iTemp64; // Umgehung von Speicherzugriffsverletzung bei Python 3.4 unter LINUX x64

    TML_INT32   err = TML_SUCCESS;

    if (!PyArg_ParseTuple(args, "Ll", &cmdHandle, &iTemp64))
        return NULL;
    state = (TML_INT32)iTemp64;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = tml_Cmd_Header_SetState (cmdHandle, state);
    }
    catch (...) {
        err = TML_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Cmd_Header_SetState  failed (%d)",err);
            return NULL;
    }

    Py_RETURN_NONE;
}

/*
 *------------------------------------------------------
 *               tml_Cmd_Header_GetState 
 *------------------------------------------------------
 */
static PyObject * _tml_Cmd_Header_GetState (PyObject *self, PyObject *args)
{
    TML_COMMAND_HANDLE  cmdHandle;
    TML_INT32           state;

    TML_INT32   err = TML_SUCCESS;

    if (!PyArg_ParseTuple(args, "L", &cmdHandle))
        return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = tml_Cmd_Header_GetState (cmdHandle, &state);
    }
    catch (...) {
        err = TML_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Cmd_Header_GetState  failed (%d)",err);
            return NULL;
    }

    return Py_BuildValue("l",state);
}

/*
 *------------------------------------------------------
 *               tml_Send_SyncMessage
 *------------------------------------------------------
 */
static PyObject * _tml_Send_SyncMessage(PyObject *self, PyObject *args)
{
    TML_CORE_HANDLE    shandle;
    TML_COMMAND_HANDLE cmdHandle;
    PyObject*          py_Profile16;
    TML_TCHAR*         sProfile;
    PyObject*          py_Profile;
    PyObject*          py_Host16;
    TML_TCHAR*         sHost;
    PyObject*          py_Host;
    PyObject*          py_Port16;
    TML_TCHAR*         sPort;
    PyObject*          py_Port;
    TML_UINT32         iTimeout;
    TML_UINT64         iTemp64u; // Umgehung von Speicherzugriffsverletzung bei Python 3.4 unter LINUX x64

    TML_INT32   err = TML_SUCCESS;

    if (!PyArg_ParseTuple(args, "LLOOOk", &shandle, &cmdHandle, &py_Profile, &py_Host, &py_Port, &iTemp64u))
        return NULL;
    iTimeout = (TML_UINT32)iTemp64u;
    if ( _unicodeConversion(py_Profile, &sProfile, &py_Profile16, tmlError) ) return NULL;
    if ( _unicodeConversion(py_Host, &sHost, &py_Host16, tmlError) ) return NULL;
    if ( _unicodeConversion(py_Port, &sPort, &py_Port16, tmlError) ) return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = tml_Send_SyncMessage(shandle, cmdHandle, sProfile, sHost, sPort, iTimeout);
    }
    catch (...) {
        err = TML_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    unicodeFree(py_Profile16, sProfile);
    unicodeFree(py_Host16, sHost);
    unicodeFree(py_Port16, sPort);

    if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Send_SyncMessage failed (%d)",err);
            return NULL;
    }

    Py_RETURN_NONE;
}

/*
 *------------------------------------------------------
 *               tml_Send_AsyncMessage
 *------------------------------------------------------
 */
static PyObject * _tml_Send_AsyncMessage(PyObject *self, PyObject *args)
{
    TML_CORE_HANDLE    shandle;
    TML_COMMAND_HANDLE cmdHandle;
    PyObject*          py_Profile16;
    TML_TCHAR*         sProfile;
    PyObject*          py_Profile;
    PyObject*          py_Host16;
    TML_TCHAR*         sHost;
    PyObject*          py_Host;
    PyObject*          py_Port16;
    TML_TCHAR*         sPort;
    PyObject*          py_Port;
    TML_UINT32         iTimeout;
    TML_UINT64         iTemp64u; // Umgehung von Speicherzugriffsverletzung bei Python 3.4 unter LINUX x64

    TML_INT32   err = TML_SUCCESS;

    if (!PyArg_ParseTuple(args, "LLOOOk", &shandle, &cmdHandle, &py_Profile, &py_Host, &py_Port, &iTemp64u))
        return NULL;
    iTimeout = (TML_UINT32)iTemp64u;
    if ( _unicodeConversion(py_Profile, &sProfile, &py_Profile16, tmlError) ) return NULL;
    if ( _unicodeConversion(py_Host, &sHost, &py_Host16, tmlError) ) return NULL;
    if ( _unicodeConversion(py_Port, &sPort, &py_Port16, tmlError) ) return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = tml_Send_AsyncMessage(shandle, cmdHandle, sProfile, sHost, sPort, iTimeout);
    }
    catch (...) {
        err = TML_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    unicodeFree(py_Profile16, sProfile);
    unicodeFree(py_Host16, sHost);
    unicodeFree(py_Port16, sPort);

    if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Send_AsyncMessage failed (%d)",err);
            return NULL;
    }

    Py_RETURN_NONE;
}

/*
 *------------------------------------------------------
 *               tml_Cmd_Register_Progress
 *------------------------------------------------------
 */
static PyObject * _tml_Cmd_Register_Progress(PyObject *self, PyObject *args)
{
    TML_COMMAND_HANDLE   cmdHandle;
    PyObject*            pCBFunc;
    PyObject*            pCBData;
    TML_INT32            err = TML_SUCCESS;

    if (PyArg_ParseTuple(args, "LOO", &cmdHandle, &pCBFunc, &pCBData)) {
        if (!PyCallable_Check(pCBFunc) && pCBFunc != Py_None) {
            PyErr_SetString(PyExc_TypeError, "parameter must be a callable function");
            return NULL;
        }
        if (!PyFunction_Check(pCBFunc) && pCBFunc != Py_None) {
            PyErr_SetString(PyExc_TypeError, "parameter must be a function");
            return NULL;
        }
        Py_BEGIN_ALLOW_THREADS;
        try {
            PythonCallbackData*           progressCallbackData = NULL;
            TML_ON_PROGRESS_REPLY_CB_FUNC pCBFuncProgress;
            TML_POINTER                   pCBDataProgress;

            err = tml_Cmd_Registered_Progress (cmdHandle, &pCBFuncProgress, &pCBDataProgress);
            if (TML_SUCCESS == err){
                if (NULL != pCBDataProgress){
                    progressCallbackData = (PythonCallbackData*) pCBDataProgress;
                    Py_XDECREF(progressCallbackData->pCBFunc);
                    Py_XDECREF(progressCallbackData->pCBData);
                }
                else{
                    progressCallbackData = new PythonCallbackData;
                }
                Py_XINCREF(pCBFunc);
                Py_XINCREF(pCBData);
                progressCallbackData->pCBFunc = pCBFunc;
                progressCallbackData->pCBData = pCBData;

                err = tml_Cmd_Register_Progress(cmdHandle, onCoreProgressCallback, progressCallbackData);
            }
        }
        catch (...) {
            err = TML_ERR_COMMON;
        }
        Py_END_ALLOW_THREADS;
    
        if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Cmd_Register_Progress failed (%d)",err);
            return NULL;
        }

    }
    Py_RETURN_NONE;
}


/*
 *------------------------------------------------------
 *               tml_Cmd_Registered_Progress
 *------------------------------------------------------
 */
static PyObject * _tml_Cmd_Registered_Progress(PyObject *self, PyObject *args)
{
    TML_COMMAND_HANDLE            cmdHandle;
    TML_ON_PROGRESS_REPLY_CB_FUNC pCBFuncProgressIntern = NULL;
    TML_POINTER                   pCBDataProgressIntern = NULL;
    PyObject*                     pCBFuncProgress = NULL;
    PyObject*                     pCBDataProgress = NULL;
    TML_INT32                     err = TML_SUCCESS;

    if (PyArg_ParseTuple(args, "L", &cmdHandle)) {
        Py_BEGIN_ALLOW_THREADS;
        try {
            err = tml_Cmd_Registered_Progress (cmdHandle, &pCBFuncProgressIntern, &pCBDataProgressIntern);
            if (TML_SUCCESS == err){
                if (NULL != pCBDataProgressIntern){
                    PythonCallbackData* progressCallbackData = (PythonCallbackData*) pCBDataProgressIntern;
                    pCBFuncProgress = progressCallbackData->pCBFunc;
                    pCBDataProgress = progressCallbackData->pCBData;
                }
            }
        }
        catch (...) {
            err = TML_ERR_COMMON;
        }
        Py_END_ALLOW_THREADS;
    
        if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Cmd_Registered_Progress failed (%d)",err);
            return NULL;
        }

    }

    if (NULL == pCBFuncProgress){
        Py_INCREF(Py_None);
        pCBFuncProgress = Py_None;
    }
    if (NULL == pCBDataProgress){
        Py_INCREF(Py_None);
        pCBDataProgress = Py_None;
    }
    return Py_BuildValue("OO",pCBFuncProgress, pCBDataProgress);
}

/*
 *------------------------------------------------------
 *               tml_Cmd_Register_StatusReply
 *------------------------------------------------------
 */
static PyObject * _tml_Cmd_Register_StatusReply(PyObject *self, PyObject *args)
{
    TML_COMMAND_HANDLE   cmdHandle;
    PyObject*            pCBFunc;
    PyObject*            pCBData;
    TML_INT32            err = TML_SUCCESS;

    if (PyArg_ParseTuple(args, "LOO", &cmdHandle, &pCBFunc, &pCBData)) {
        if (!PyCallable_Check(pCBFunc) && pCBFunc != Py_None) {
            PyErr_SetString(PyExc_TypeError, "parameter must be a callable function");
            return NULL;
        }
        if (!PyFunction_Check(pCBFunc) && pCBFunc != Py_None) {
            PyErr_SetString(PyExc_TypeError, "parameter must be a function");
            return NULL;
        }
        Py_BEGIN_ALLOW_THREADS;
        try {
            PythonCallbackData*           statusReplyCallbackData = NULL;
            TML_ON_STATUS_REPLY_CB_FUNC   pCBFuncStatusReply;
            TML_POINTER                   pCBDataStatusReply;

            err = tml_Cmd_Registered_StatusReply (cmdHandle, &pCBFuncStatusReply, &pCBDataStatusReply);
            if (TML_SUCCESS == err){
                if (NULL != pCBDataStatusReply){
                    statusReplyCallbackData = (PythonCallbackData*) pCBDataStatusReply;
                    Py_XDECREF(statusReplyCallbackData->pCBFunc);
                    Py_XDECREF(statusReplyCallbackData->pCBData);
                }
                else{
                    statusReplyCallbackData = new PythonCallbackData;
                }
                Py_XINCREF(pCBFunc);
                Py_XINCREF(pCBData);
                statusReplyCallbackData->pCBFunc = pCBFunc;
                statusReplyCallbackData->pCBData = pCBData;

                err = tml_Cmd_Register_StatusReply(cmdHandle, onCoreStatusReplyCallback, statusReplyCallbackData);
            }
        }
        catch (...) {
            err = TML_ERR_COMMON;
        }
        Py_END_ALLOW_THREADS;
    
        if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Cmd_Register_StatusReply failed (%d)",err);
            return NULL;
        }

    }
    Py_RETURN_NONE;
}

/*
 *------------------------------------------------------
 *               tml_Cmd_Registered_StatusReply
 *------------------------------------------------------
 */
static PyObject * _tml_Cmd_Registered_StatusReply(PyObject *self, PyObject *args)
{
    TML_COMMAND_HANDLE            cmdHandle;
    TML_ON_STATUS_REPLY_CB_FUNC   pCBFuncStatusReplyIntern = NULL;
    TML_POINTER                   pCBDataStatusReplyIntern = NULL;
    PyObject*                     pCBFuncStatusReply = NULL;
    PyObject*                     pCBDataStatusReply = NULL;
    TML_INT32                     err = TML_SUCCESS;

    if (PyArg_ParseTuple(args, "L", &cmdHandle)) {
        Py_BEGIN_ALLOW_THREADS;
        try {
            err = tml_Cmd_Registered_StatusReply (cmdHandle, &pCBFuncStatusReplyIntern, &pCBDataStatusReplyIntern);
            if (TML_SUCCESS == err){
                if (NULL != pCBDataStatusReplyIntern){
                    PythonCallbackData* statusReplyCallbackData = (PythonCallbackData*) pCBDataStatusReplyIntern;
                    pCBFuncStatusReply = statusReplyCallbackData->pCBFunc;
                    pCBDataStatusReply = statusReplyCallbackData->pCBData;
                }
            }
        }
        catch (...) {
            err = TML_ERR_COMMON;
        }
        Py_END_ALLOW_THREADS;
    
        if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Cmd_Registered_StatusReply failed (%d)",err);
            return NULL;
        }

    }

    if (NULL == pCBFuncStatusReply){
        Py_INCREF(Py_None);
        pCBFuncStatusReply = Py_None;
    }
    if (NULL == pCBDataStatusReply){
        Py_INCREF(Py_None);
        pCBDataStatusReply = Py_None;
    }
    return Py_BuildValue("OO",pCBFuncStatusReply, pCBDataStatusReply);
}


/*
 *------------------------------------------------------
 *               tml_Cmd_Register_CommandReady
 *------------------------------------------------------
 */
static PyObject * _tml_Cmd_Register_CommandReady(PyObject *self, PyObject *args)
{
    TML_COMMAND_HANDLE   cmdHandle;
    PyObject*            pCBFunc;
    PyObject*            pCBData;
    TML_INT32            err = TML_SUCCESS;

    if (PyArg_ParseTuple(args, "LOO", &cmdHandle, &pCBFunc, &pCBData)) {
        if (!PyCallable_Check(pCBFunc) && pCBFunc != Py_None) {
            PyErr_SetString(PyExc_TypeError, "parameter must be a callable function");
            return NULL;
        }
        if (!PyFunction_Check(pCBFunc) && pCBFunc != Py_None) {
            PyErr_SetString(PyExc_TypeError, "parameter must be a function");
            return NULL;
        }
        Py_BEGIN_ALLOW_THREADS;
        try {
            PythonCallbackData*           commandReadyCallbackData = NULL;
            TML_ON_COMMAND_READY_CB_FUNC  pCBFuncCommandReady;
            TML_POINTER                   pCBDataCommandReady;

            err = tml_Cmd_Registered_CommandReady (cmdHandle, &pCBFuncCommandReady, &pCBDataCommandReady);
            if (TML_SUCCESS == err){
                if (NULL != pCBDataCommandReady){
                    commandReadyCallbackData = (PythonCallbackData*) pCBDataCommandReady;
                    Py_XDECREF(commandReadyCallbackData->pCBFunc);
                    Py_XDECREF(commandReadyCallbackData->pCBData);
                }
                else{
                    commandReadyCallbackData = new PythonCallbackData;
                }
                Py_XINCREF(pCBFunc);
                Py_XINCREF(pCBData);
                commandReadyCallbackData->pCBFunc = pCBFunc;
                commandReadyCallbackData->pCBData = pCBData;

                err = tml_Cmd_Register_CommandReady(cmdHandle, onCoreCommandReadyCallback, commandReadyCallbackData);
            }
        }
        catch (...) {
            err = TML_ERR_COMMON;
        }
        Py_END_ALLOW_THREADS;
    
        if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Cmd_Register_commandReady failed (%d)",err);
            return NULL;
        }

    }
    Py_RETURN_NONE;
}

/*
 *------------------------------------------------------
 *               tml_Cmd_Registered_CommandReady
 *------------------------------------------------------
 */
static PyObject * _tml_Cmd_Registered_CommandReady(PyObject *self, PyObject *args)
{
    TML_COMMAND_HANDLE            cmdHandle;
    TML_ON_COMMAND_READY_CB_FUNC  pCBFuncCommandReadyIntern = NULL;
    TML_POINTER                   pCBDataCommandReadyIntern = NULL;
    PyObject*                     pCBFuncCommandReady = NULL;
    PyObject*                     pCBDataCommandReady = NULL;
    TML_INT32                     err = TML_SUCCESS;

    if (PyArg_ParseTuple(args, "L", &cmdHandle)) {
        Py_BEGIN_ALLOW_THREADS;
        try {
            err = tml_Cmd_Registered_CommandReady (cmdHandle, &pCBFuncCommandReadyIntern, &pCBDataCommandReadyIntern);
            if (TML_SUCCESS == err){
                if (NULL != pCBDataCommandReadyIntern){
                    PythonCallbackData* commandReadyCallbackData = (PythonCallbackData*) pCBDataCommandReadyIntern;
                    pCBFuncCommandReady = commandReadyCallbackData->pCBFunc;
                    pCBDataCommandReady = commandReadyCallbackData->pCBData;
                }
            }
        }
        catch (...) {
            err = TML_ERR_COMMON;
        }
        Py_END_ALLOW_THREADS;
    
        if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Cmd_Registered_commandReady failed (%d)",err);
            return NULL;
        }

    }

    if (NULL == pCBFuncCommandReady){
        Py_INCREF(Py_None);
        pCBFuncCommandReady = Py_None;
    }
    if (NULL == pCBDataCommandReady){
        Py_INCREF(Py_None);
        pCBDataCommandReady = Py_None;
    }
    return Py_BuildValue("OO",pCBFuncCommandReady, pCBDataCommandReady);
}

/*
 *------------------------------------------------------
 *               tml_Send_AsyncProgressReply
 *------------------------------------------------------
 */
static PyObject * _tml_Send_AsyncProgressReply(PyObject *self, PyObject *args)
{
    TML_COMMAND_HANDLE  cmdHandle;
    TML_UINT32          iProgress;
    TML_UINT64          iTemp64u; // Umgehung von Speicherzugriffsverletzung bei Python 3.4 unter LINUX x64

    TML_INT32   err = TML_SUCCESS;

    if (!PyArg_ParseTuple(args, "Lk", &cmdHandle, &iTemp64u))
        return NULL;
    iProgress = (TML_UINT32)iTemp64u;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = tml_Send_AsyncProgressReply(cmdHandle, iProgress);
    }
    catch (...) {
        err = TML_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Send_AsyncProgressReply failed (%d)",err);
            return NULL;
    }

    Py_RETURN_NONE;
}

/*
 *------------------------------------------------------
 *               tml_Send_AsyncStatusReply
 *------------------------------------------------------
 */
static PyObject * _tml_Send_AsyncStatusReply(PyObject *self, PyObject *args)
{


    TML_COMMAND_HANDLE cmdHandle;
    TML_UINT32         iType;
    TML_UINT64         iTemp64u; // Umgehung von Speicherzugriffsverletzung bei Python 3.4 unter LINUX x64
    PyObject*          py_Message16;
    SIDEX_TCHAR*       sMessage;
    PyObject*          py_Message;

    TML_INT32   err = TML_SUCCESS;

    if (!PyArg_ParseTuple(args, "LkO", &cmdHandle, &iTemp64u, &py_Message))
        return NULL;
    iType = (TML_UINT32)iTemp64u;
    if ( _unicodeConversion(py_Message, &sMessage, &py_Message16, tmlError) ) return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = tml_Send_AsyncStatusReply(cmdHandle, iType, sMessage);
    }
    catch (...) {
        err = TML_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    unicodeFree(py_Message16, sMessage);

    if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Send_AsyncStatusReply failed (%d)",err);
            return NULL;
    }

    Py_RETURN_NONE;
}

/*
 *------------------------------------------------------
 *               tml_Core_Set_ListenerEnabled
 *------------------------------------------------------
 */
static PyObject * _tml_Core_Set_ListenerEnabled(PyObject *self, PyObject *args)
{
    TML_CORE_HANDLE  shandle;
    PyObject         * val;

    TML_INT32   err = TML_SUCCESS;

    if (!PyArg_ParseTuple(args, "LO", &shandle, &val))
        return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = tml_Core_Set_ListenerEnabled(shandle, PyObject_IsTrue(val));
    }
    catch (...) {
        err = TML_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Core_Set_ListenerEnabled failed (%d)",err);
            return NULL;
    }

    Py_RETURN_NONE;
}


/*
 *------------------------------------------------------
 *               tml_Core_Get_ListenerEnabled
 *------------------------------------------------------
 */
static PyObject * _tml_Core_Get_ListenerEnabled(PyObject *self, PyObject *args)
{
    TML_CORE_HANDLE  shandle;
    TML_BOOL         rval;

    TML_INT32   err = TML_SUCCESS;

    if (!PyArg_ParseTuple(args, "L", &shandle))
        return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = tml_Core_Get_ListenerEnabled(shandle, &rval);
    }
    catch (...) {
        err = TML_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Core_Get_ListenerEnabled failed (%d)",err);
            return NULL;
    }

    if (SIDEX_TRUE == rval)
        Py_RETURN_TRUE;
    else
        Py_RETURN_FALSE;
}


/*
 *------------------------------------------------------
 *               tml_Core_Set_ListenerIP
 *------------------------------------------------------
 */
static PyObject * _tml_Core_Set_ListenerIP(PyObject *self, PyObject *args)
{
    TML_CORE_HANDLE  shandle;
    PyObject*        py_IP16;
    SIDEX_TCHAR*     pIP;
    PyObject*        py_IP;

    TML_INT32   err = TML_SUCCESS;

    if (!PyArg_ParseTuple(args, "LO", &shandle, &py_IP))
        return NULL;
    if ( _unicodeConversion(py_IP, &pIP, &py_IP16, tmlError) ) return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = tml_Core_Set_ListenerIP(shandle, pIP);
    }
    catch (...) {
        err = TML_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    unicodeFree(py_IP16, pIP);
    
    if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Core_Set_ListenerIP failed (%d)",err);
            return NULL;
    }

    Py_RETURN_NONE;
}


/*
 *------------------------------------------------------
 *               tml_Core_Get_ListenerIP
 *------------------------------------------------------
 */
static PyObject * _tml_Core_Get_ListenerIP(PyObject *self, PyObject *args)
{
    TML_CORE_HANDLE  shandle;
    TML_TCHAR*       pIP;
    PyObject *       result;

    TML_INT32   err = TML_SUCCESS;

    if (!PyArg_ParseTuple(args, "L", &shandle))
        return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = tml_Core_Get_ListenerIP(shandle, &pIP);
    }
    catch (...) {
        err = TML_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Core_Get_ListenerIP failed (%d)",err);
            return NULL;
    }

    result = utf32_utf16_decode(pIP, -1);

    return result;
}


/*
 *------------------------------------------------------
 *               tml_Core_Set_ListenerPort
 *------------------------------------------------------
 */
static PyObject * _tml_Core_Set_ListenerPort(PyObject *self, PyObject *args)
{
    TML_CORE_HANDLE  shandle;
    PyObject*        py_Port16;
    SIDEX_TCHAR*     pPort;
    PyObject*        py_Port;

    TML_INT32   err = TML_SUCCESS;

    if (!PyArg_ParseTuple(args, "LO", &shandle, &py_Port))
        return NULL;
    if ( _unicodeConversion(py_Port, &pPort, &py_Port16, tmlError) ) return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = tml_Core_Set_ListenerPort(shandle, pPort);
    }
    catch (...) {
        err = TML_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    unicodeFree(py_Port16, pPort);

    if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Core_Set_ListenerPort failed (%d)",err);
            return NULL;
    }

    Py_RETURN_NONE;
}


/*
 *------------------------------------------------------
 *               tml_Core_Get_ListenerPort
 *------------------------------------------------------
 */
static PyObject * _tml_Core_Get_ListenerPort(PyObject *self, PyObject *args)
{
    TML_CORE_HANDLE  shandle;
    TML_TCHAR*       pPort;
    PyObject *       result;

    TML_INT32   err = TML_SUCCESS;

    if (!PyArg_ParseTuple(args, "L", &shandle))
        return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = tml_Core_Get_ListenerPort(shandle, &pPort);
    }
    catch (...) {
        err = TML_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Core_Get_ListenerPort failed (%d)",err);
            return NULL;
    }

    result = utf32_utf16_decode(pPort, -1);

    return result;
}


/*
 *------------------------------------------------------
 *               tml_Profile_Get_Registered
 *------------------------------------------------------
 */
static PyObject * _tml_Profile_Get_Registered(PyObject *self, PyObject *args)
{
    TML_CORE_HANDLE  shandle;
    PyObject*        pyList = PyList_New(0);

    TML_INT32        err = TML_SUCCESS;

    if (!PyArg_ParseTuple(args, "L", &shandle))
        return NULL;

    TML_INT32        vCount = 0;
    SIDEX_VARIANT    sProfiles = SIDEX_HANDLE_TYPE_NULL;
    Py_BEGIN_ALLOW_THREADS;
    try {
      /////////////////////////////////////////
      // Get the number of registered profiles:
      err = tml_Profile_Get_Registered_Count (shandle, &vCount);
      if (err == TML_SUCCESS && vCount) {
        /////////////////////////////////////////
        // Get the registered profiles:
        err = tml_Profile_Get_Registered(shandle, &sProfiles);
      }
    }
    catch (...) {
        err = TML_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err == TML_SUCCESS) {
      if (SIDEX_HANDLE_TYPE_NULL != sProfiles){
        for (TML_INT32 i = 0; i< vCount && err == TML_SUCCESS; ++i){
          SIDEX_VARIANT sItem;
          /////////////////////////////////////////
          // Get profile name:
          err = sidex_Variant_List_Get (sProfiles, i, &sItem);
          if (err == TML_SUCCESS) {
            wchar_t* sTmpProfile;
            TML_INT32 iLength;
            err = sidex_Variant_As_String_X(sItem, &sTmpProfile, &iLength);
            if (err == TML_SUCCESS) {
              /////////////////////////////////////////
              // Add profile name to pyList:
              PyObject* listItem = utf32_utf16_decode(sTmpProfile, -1);
              PyList_Append(pyList, listItem);
              Py_XDECREF(listItem);
            }
          }
        }
        sidex_Variant_DecRef(sProfiles);
      }
    }
    else{
      PyErr_Format(tmlError, "tml_Profile_Get_Registered failed (%d)",err);
      return NULL;
    }
    return pyList;
}


/*
 *------------------------------------------------------
 *               tml_Profile_Get_Registered_Count
 *------------------------------------------------------
 */
static PyObject * _tml_Profile_Get_Registered_Count(PyObject *self, PyObject *args)
{
    TML_CORE_HANDLE  shandle;
    TML_INT32        ssize;

    TML_INT32   err = TML_SUCCESS;

    if (!PyArg_ParseTuple(args, "L", &shandle))
        return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = tml_Profile_Get_Registered_Count(shandle, &ssize);
    }
    catch (...) {
        err = TML_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Profile_Get_Registered_Count failed (%d)",err);
            return NULL;
    }

    return Py_BuildValue("l",ssize);
}


/*
 *------------------------------------------------------
 *               tml_Profile_Get_RegisterState
 *------------------------------------------------------
 */
static PyObject * _tml_Profile_Get_RegisterState(PyObject *self, PyObject *args)
{
    TML_CORE_HANDLE  shandle;
    PyObject*        py_Profile16;
    SIDEX_TCHAR*     pProfile;
    PyObject*        py_Profile;
    TML_BOOL         rval;

    TML_INT32   err = TML_SUCCESS;

    if (!PyArg_ParseTuple(args, "LO", &shandle, &py_Profile))
        return NULL;
    if ( _unicodeConversion(py_Profile, &pProfile, &py_Profile16, tmlError) ) return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = tml_Profile_Get_RegisterState(shandle, pProfile, &rval);
    }
    catch (...) {
        err = TML_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    unicodeFree(py_Profile16, pProfile);

    if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Profile_Get_RegisterState failed (%d)",err);
            return NULL;
    }

    if (SIDEX_TRUE == rval)
        Py_RETURN_TRUE;
    else
        Py_RETURN_FALSE;
}


/*
 *------------------------------------------------------
 *               tml_Profile_Register
 *------------------------------------------------------
 */
static PyObject * _tml_Profile_Register(PyObject *self, PyObject *args)
{
    TML_CORE_HANDLE  shandle;
    PyObject*        py_Profile16;
    SIDEX_TCHAR*     pProfile;
    PyObject*        py_Profile;

    TML_INT32   err = TML_SUCCESS;

    if (!PyArg_ParseTuple(args, "LO", &shandle, &py_Profile))
        return NULL;
    if ( _unicodeConversion(py_Profile, &pProfile, &py_Profile16, tmlError) ) return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = tml_Profile_Register(shandle, pProfile);
    }
    catch (...) {
        err = TML_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    unicodeFree(py_Profile16, pProfile);

    if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Profile_Register failed (%d)",err);
            return NULL;
    }

    Py_RETURN_NONE;
}


/*
 *------------------------------------------------------
 *               tml_Profile_Register_Cmd
 *------------------------------------------------------
 */
static PyObject * _tml_Profile_Register_Cmd(PyObject *self, PyObject *args)
{
    TML_CORE_HANDLE      shandle;
    PyObject*            py_Profile16;
    SIDEX_TCHAR*         pProfile;
    PyObject*            py_Profile;
    TML_COMMAND_ID_TYPE  iCmdIdentifier;
    TML_UINT64           iTemp64u; // Umgehung von Speicherzugriffsverletzung bei Python 3.4 unter LINUX x64
    PyObject*            pCBFunc;
    PyObject*            pCBData;
    TML_INT32            err = TML_SUCCESS;

    if (PyArg_ParseTuple(args, "LOkOO", &shandle, &py_Profile, &iTemp64u, &pCBFunc, &pCBData)) {
        iCmdIdentifier = (TML_COMMAND_ID_TYPE)iTemp64u;
        if (!PyCallable_Check(pCBFunc) && pCBFunc != Py_None) {
            PyErr_SetString(PyExc_TypeError, "parameter must be a callable function");
            return NULL;
        }
        if (!PyFunction_Check(pCBFunc) && pCBFunc != Py_None) {
            PyErr_SetString(PyExc_TypeError, "parameter must be a function");
            return NULL;
        }
        if ( _unicodeConversion(py_Profile, &pProfile, &py_Profile16, tmlError) ) return NULL;
        PythonCallbackData* commandDispatchCallbackData = NULL;
        err = getCommandDispatchCallbackData(pProfile, iCmdIdentifier, pCBFunc, pCBData, &commandDispatchCallbackData);
        if (TML_SUCCESS == err){
          Py_BEGIN_ALLOW_THREADS;
          try {
            err = tml_Profile_Register_Cmd(shandle, pProfile, iCmdIdentifier, onCoreCommandDispatchCallback, commandDispatchCallbackData);
          }
          catch (...) {
            err = TML_ERR_COMMON;
          }
          Py_END_ALLOW_THREADS;
        }

        unicodeFree(py_Profile16, pProfile);

        if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Profile_Register_Cmd failed (%d)",err);
            return NULL;
        }
    }
    Py_RETURN_NONE;
}


/*
 *------------------------------------------------------
 *               tml_Profile_Set_OnCustomDispatch
 *------------------------------------------------------
 */
static PyObject * _tml_Profile_Set_OnCustomDispatch(PyObject *self, PyObject *args)
{
    TML_CORE_HANDLE      shandle;
    PyObject*            py_Profile16;
    SIDEX_TCHAR*         pProfile;
    PyObject*            py_Profile;
    PyObject*            pCBFunc;
    PyObject*            pCBData;
    TML_INT32            err = TML_SUCCESS;

    if (PyArg_ParseTuple(args, "LOOO", &shandle, &py_Profile, &pCBFunc, &pCBData)) {
        if (!PyCallable_Check(pCBFunc) && pCBFunc != Py_None) {
            PyErr_SetString(PyExc_TypeError, "parameter must be a callable function");
            return NULL;
        }
        if (!PyFunction_Check(pCBFunc) && pCBFunc != Py_None) {
            PyErr_SetString(PyExc_TypeError, "parameter must be a function");
            return NULL;
        }
        if ( _unicodeConversion(py_Profile, &pProfile, &py_Profile16, tmlError) ) return NULL;
        PythonCallbackData* customDispatchCallbackData = NULL;
        err = getCallbackDataFromDict(pProfile, pCBFunc, pCBData, &customDispatchCallbackData, m_customDispatchDict);
        if (TML_SUCCESS == err){
          Py_BEGIN_ALLOW_THREADS;
          try {
            err = tml_Profile_Set_OnCustomDispatch(shandle, pProfile, onCoreCustomDispatchCallback, customDispatchCallbackData);
          }
          catch (...) {
            err = TML_ERR_COMMON;
          }
          Py_END_ALLOW_THREADS;
        }
    
        unicodeFree(py_Profile16, pProfile);

        if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Profile_Set_OnCustomDispatch failed (%d)",err);
            return NULL;
        }
    }
    Py_RETURN_NONE;
}


/*
 *------------------------------------------------------
 *               tml_Profile_Set_OnDeleteCmd
 *------------------------------------------------------
 */
static PyObject * _tml_Profile_Set_OnDeleteCmd(PyObject *self, PyObject *args)
{
    TML_CORE_HANDLE      shandle;
    PyObject*            py_Profile16;
    SIDEX_TCHAR*         pProfile;
    PyObject*            py_Profile;
    PyObject*            pCBFunc;
    PyObject*            pCBData;
    TML_INT32            err = TML_SUCCESS;

    if (PyArg_ParseTuple(args, "LOOO", &shandle, &py_Profile, &pCBFunc, &pCBData)) {
        if (!PyCallable_Check(pCBFunc) && pCBFunc != Py_None) {
            PyErr_SetString(PyExc_TypeError, "parameter must be a callable function");
            return NULL;
        }
        if (!PyFunction_Check(pCBFunc) && pCBFunc != Py_None) {
            PyErr_SetString(PyExc_TypeError, "parameter must be a function");
            return NULL;
        }
        if ( _unicodeConversion(py_Profile, &pProfile, &py_Profile16, tmlError) ) return NULL;
        PythonCallbackData* deleteCmdCallbackData = NULL;
        err = getCallbackDataFromDict(pProfile, pCBFunc, pCBData, &deleteCmdCallbackData, m_deleteCmdDict);
        if (TML_SUCCESS == err){
          Py_BEGIN_ALLOW_THREADS;
          try {
            err = tml_Profile_Set_OnDeleteCmd(shandle, pProfile, onCoreDeleteCmdCallback, deleteCmdCallbackData);
          }
          catch (...) {
            err = TML_ERR_COMMON;
          }
          Py_END_ALLOW_THREADS;
        }
        unicodeFree(py_Profile16, pProfile);

        if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Profile_Set_OnDeleteCmd failed (%d)",err);
            return NULL;
        }
    }
    Py_RETURN_NONE;
}


/*
 *------------------------------------------------------
 *               tml_Profile_Unregister
 *------------------------------------------------------
 */
static PyObject * _tml_Profile_Unregister(PyObject *self, PyObject *args)
{
    TML_CORE_HANDLE  shandle;
    PyObject*        py_Profile16;
    SIDEX_TCHAR*     pProfile;
    PyObject*        py_Profile;

    TML_INT32   err = TML_SUCCESS;

    if (!PyArg_ParseTuple(args, "LO", &shandle, &py_Profile))
        return NULL;
    if ( _unicodeConversion(py_Profile, &pProfile, &py_Profile16, tmlError) ) return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        ////////////////////////////
        // deregister all callbacks:
        // m_customDispatchDict         
        err = tml_Profile_Set_OnCustomDispatch(shandle, pProfile, NULL, NULL);

        if (err == TML_SUCCESS) err = tml_Profile_Set_OnDeleteCmd(shandle, pProfile, NULL, NULL);
        // m_balOnBusyStatusRequestDict 
        if (err == TML_SUCCESS) err = tml_Bal_Set_OnBusyStatusRequest(shandle, pProfile, NULL, NULL);
        // m_balOnCalculationDict       
        if (err == TML_SUCCESS) err = tml_Bal_Set_OnCalculation(shandle, pProfile, NULL, NULL);
        // m_balOnPeerRegisterDict      
        if (err == TML_SUCCESS) err =  tml_Bal_Set_OnPeerRegister(shandle, pProfile, NULL, NULL);
        // m_balOnPopulateDict          
        if (err == TML_SUCCESS) err = tml_Bal_Set_OnPopulate(shandle, pProfile, NULL, NULL);
        // m_evtOnErrorDict             
        if (err == TML_SUCCESS) err =  tml_Evt_Set_OnError(shandle, pProfile, NULL, NULL);
        // m_evtOnPeerRegisterDict      
        if (err == TML_SUCCESS) err = tml_Evt_Set_OnPeerRegister(shandle, pProfile, NULL, NULL);
        // m_evtOnPopulateDict          
        if (err == TML_SUCCESS) err =  tml_Evt_Set_OnPopulate(shandle, pProfile, NULL, NULL);
        // m_evtOnQueueOverflowDict     
        if (err == TML_SUCCESS) err =  tml_Evt_Set_OnQueueOverflow(shandle, pProfile, NULL, NULL);
        // Now unregister:
        if (err == TML_SUCCESS) err = tml_Profile_Unregister(shandle, pProfile);
    }
    catch (...) {
        err = TML_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    ///////////////////////////////////////////////////////////////
    // All registered callbacks must be deregistered (refcounting):
    // m_deleteCmdDict will be done in releaseCommandDispatchCallbackData4Profile:
    releaseCommandDispatchCallbackData4Profile(pProfile);

    releaseCallbackDataDict4Profile(m_customDispatchDict, pProfile);
    releaseCallbackDataDict4Profile(m_deleteCmdDict, pProfile);
    releaseCallbackDataDict4Profile(m_balOnBusyStatusRequestDict, pProfile);
    releaseCallbackDataDict4Profile(m_balOnCalculationDict, pProfile);
    releaseCallbackDataDict4Profile(m_balOnPeerRegisterDict, pProfile);
    releaseCallbackDataDict4Profile(m_balOnPopulateDict, pProfile);
    releaseCallbackDataDict4Profile(m_evtOnErrorDict, pProfile);
    releaseCallbackDataDict4Profile(m_evtOnPeerRegisterDict, pProfile);
    releaseCallbackDataDict4Profile(m_evtOnPopulateDict, pProfile);
    releaseCallbackDataDict4Profile(m_evtOnQueueOverflowDict, pProfile);

    unicodeFree(py_Profile16, pProfile);

    if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Profile_Unregister failed (%d)",err);
            return NULL;
    }

    Py_RETURN_NONE;
}


/*
 *------------------------------------------------------
 *               tml_Bal_Get_MaxConnectionFailCount
 *------------------------------------------------------
 */
static PyObject * _tml_Bal_Get_MaxConnectionFailCount(PyObject *self, PyObject *args)
{
    TML_CORE_HANDLE  shandle;
    TML_UINT32       iCount;

    TML_INT32   err = TML_SUCCESS;

    if (!PyArg_ParseTuple(args, "L", &shandle))
        return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = tml_Bal_Get_MaxConnectionFailCount(shandle, &iCount);
    }
    catch (...) {
        err = TML_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Bal_Get_MaxConnectionFailCount failed (%d)",err);
            return NULL;
    }

    return Py_BuildValue("k",iCount);
}


/*
 *------------------------------------------------------
 *               tml_Bal_Get_Subscribed_MessageDestinations
 *------------------------------------------------------
 */
static PyObject * _tml_Bal_Get_Subscribed_MessageDestinations(PyObject *self, PyObject *args)
{
    TML_CORE_HANDLE  shandle;
    PyObject*        py_Profile16;
    SIDEX_TCHAR*     sProfile;
    PyObject*        py_Profile;
    PyObject*        pyList = PyList_New(0);

    TML_INT32   err = TML_SUCCESS;

    if (!PyArg_ParseTuple(args, "LO", &shandle, &py_Profile))
        return NULL;
    if ( _unicodeConversion(py_Profile, &sProfile, &py_Profile16, tmlError) ) return NULL;

    TML_INT32        vColumns = 0;
    TML_INT32        vRows = 0;
    SIDEX_VARIANT    sSubscriptions = SIDEX_HANDLE_TYPE_NULL;
    Py_BEGIN_ALLOW_THREADS;
    try {
        err = tml_Bal_Get_Subscribed_MessageDestinations(shandle, sProfile, &sSubscriptions);
    }
    catch (...) {
        err = TML_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    unicodeFree(py_Profile16, sProfile);

    if (err == TML_SUCCESS) {
      if (SIDEX_HANDLE_TYPE_NULL != sSubscriptions){
        err = sidex_Variant_Table_Columns (sSubscriptions, &vColumns);
        if (err == TML_SUCCESS) {
          err = sidex_Variant_Table_Rows (sSubscriptions, &vRows);
          if (err == TML_SUCCESS) {
            for (TML_INT32 i = 0; i< vRows && err == TML_SUCCESS; ++i){
              PyObject* pyDict = PyDict_New();
              for (TML_INT32 j = 0; j< vColumns && err == TML_SUCCESS; ++j){
                wchar_t* colName;
                err = sidex_Variant_Table_GetColumnName (sSubscriptions, j, &colName);
                if (err == TML_SUCCESS) {
                    SIDEX_VARIANT vCell;
                    err = sidex_Variant_Table_GetField_X(sSubscriptions, i, colName, &vCell);
                    if (err == TML_SUCCESS) {
                      wchar_t* sTmpCell;
                      TML_INT32 iLength;
                      err = sidex_Variant_As_String_X(vCell, &sTmpCell, &iLength);
                      if (err == TML_SUCCESS) {
                        PyObject* key = utf32_utf16_decode((TML_TCHAR*)colName, -1);
                        PyObject* val = utf32_utf16_decode((TML_TCHAR*)sTmpCell, -1);
                        PyDict_SetItem(pyDict, key, val);
                        Py_XDECREF(key);
                        Py_XDECREF(val);
                      }
                    }
                }
              }
              PyList_Append(pyList, pyDict);
              Py_XDECREF(pyDict);
            }
          }
        }
        sidex_Variant_DecRef(sSubscriptions);
      }
    }
    else{
      PyErr_Format(tmlError, "tml_Bal_Get_Subscribed_MessageDestinations failed (%d)",err);
      return NULL;
    }
    return pyList;
}


/*
 *------------------------------------------------------
 *               tml_Bal_Send_AsyncMessage
 *------------------------------------------------------
 */
static PyObject * _tml_Bal_Send_AsyncMessage(PyObject *self, PyObject *args)
{
    TML_CORE_HANDLE    shandle;
    TML_COMMAND_HANDLE cmdHandle;
    PyObject*          py_Profile16;
    SIDEX_TCHAR*       sProfile;
    PyObject*          py_Profile;
    TML_UINT32         iTimeout;
    TML_UINT64         iTemp64u; // Umgehung von Speicherzugriffsverletzung bei Python 3.4 unter LINUX x64

    TML_INT32   err = TML_SUCCESS;

    if (!PyArg_ParseTuple(args, "LLOk", &shandle, &cmdHandle, &py_Profile, &iTemp64u))
        return NULL;
    iTimeout = (TML_UINT32)iTemp64u;
    if ( _unicodeConversion(py_Profile, &sProfile, &py_Profile16, tmlError) ) return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = tml_Bal_Send_AsyncMessage(shandle, cmdHandle, sProfile, iTimeout);
    }
    catch (...) {
        err = TML_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    unicodeFree(py_Profile16, sProfile);

    if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Bal_Send_AsyncMessage failed (%d)",err);
            return NULL;
    }

    Py_RETURN_NONE;
}


/*
 *------------------------------------------------------
 *               tml_Bal_Send_SubscriptionRequest
 *------------------------------------------------------
 */
static PyObject * _tml_Bal_Send_SubscriptionRequest(PyObject *self, PyObject *args)
{
    TML_CORE_HANDLE  shandle;
    PyObject*        py_Profile16;
    TML_TCHAR*       sProfile;
    PyObject*        py_Profile;
    PyObject*        py_SourceHost16;
    TML_TCHAR*       sSourceHost;
    PyObject*        py_SourceHost;
    PyObject*        py_SourcePort16;
    TML_TCHAR*       sSourcePort;
    PyObject*        py_SourcePort;
    PyObject*        py_DestHost16;
    TML_TCHAR*       sDestHost;
    PyObject*        py_DestHost;
    PyObject*        py_DestPort16;
    TML_TCHAR*       sDestPort;
    PyObject*        py_DestPort;
    TML_UINT32       iTimeout;
    TML_UINT64       iTemp64u; // Umgehung von Speicherzugriffsverletzung bei Python 3.4 unter LINUX x64

    TML_INT32   err = TML_SUCCESS;

    if (!PyArg_ParseTuple(args, "LOOOOOk", &shandle, &py_Profile, &py_SourceHost, &py_SourcePort, &py_DestHost, &py_DestPort, &iTemp64u))
        return NULL;
    iTimeout = (TML_UINT32)iTemp64u;
    if ( _unicodeConversion(py_Profile, &sProfile, &py_Profile16, tmlError) ) return NULL;
    if ( _unicodeConversion(py_SourceHost, &sSourceHost, &py_SourceHost16, tmlError) ) return NULL;
    if ( _unicodeConversion(py_SourcePort, &sSourcePort, &py_SourcePort16, tmlError) ) return NULL;
    if ( _unicodeConversion(py_DestHost, &sDestHost, &py_DestHost16, tmlError) ) return NULL;
    if ( _unicodeConversion(py_DestPort, &sDestPort, &py_DestPort16, tmlError) ) return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = tml_Bal_Send_SubscriptionRequest(shandle, sProfile, sSourceHost, sSourcePort, sDestHost, sDestPort, iTimeout);
    }
    catch (...) {
        err = TML_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    unicodeFree(py_Profile16, sProfile);
    unicodeFree(py_SourceHost16, sSourceHost);
    unicodeFree(py_SourcePort16, sSourcePort);
    unicodeFree(py_DestHost16, sDestHost);
    unicodeFree(py_DestPort16, sDestPort);

    if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Bal_Send_SubscriptionRequest failed (%d)",err);
            return NULL;
    }

    Py_RETURN_NONE;
}


/*
 *------------------------------------------------------
 *               tml_Bal_Send_SyncMessage
 *------------------------------------------------------
 */
static PyObject * _tml_Bal_Send_SyncMessage(PyObject *self, PyObject *args)
{
    TML_CORE_HANDLE    shandle;
    TML_COMMAND_HANDLE cmdHandle;
    PyObject*          py_Profile16;
    SIDEX_TCHAR*       sProfile;
    PyObject*          py_Profile;
    TML_UINT32         iTimeout;
    TML_UINT64         iTemp64u; // Umgehung von Speicherzugriffsverletzung bei Python 3.4 unter LINUX x64

    TML_INT32   err = TML_SUCCESS;

    if (!PyArg_ParseTuple(args, "LLOk", &shandle, &cmdHandle, &py_Profile, &iTemp64u))
        return NULL;
    iTimeout = (TML_UINT32)iTemp64u;
    if ( _unicodeConversion(py_Profile, &sProfile, &py_Profile16, tmlError) ) return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = tml_Bal_Send_SyncMessage(shandle, cmdHandle, sProfile, iTimeout);
    }
    catch (...) {
        err = TML_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    unicodeFree(py_Profile16, sProfile);

    if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Bal_Send_SyncMessage failed (%d)",err);
            return NULL;
    }

    Py_RETURN_NONE;
}


/*
 *------------------------------------------------------
 *               tml_Bal_Send_UnsubscriptionRequest
 *------------------------------------------------------
 */
static PyObject * _tml_Bal_Send_UnsubscriptionRequest(PyObject *self, PyObject *args)
{
    TML_CORE_HANDLE  shandle;
    PyObject*        py_Profile16;
    TML_TCHAR*       sProfile;
    PyObject*        py_Profile;
    PyObject*        py_SourceHost16;
    TML_TCHAR*       sSourceHost;
    PyObject*        py_SourceHost;
    PyObject*        py_SourcePort16;
    TML_TCHAR*       sSourcePort;
    PyObject*        py_SourcePort;
    PyObject*        py_DestHost16;
    TML_TCHAR*       sDestHost;
    PyObject*        py_DestHost;
    PyObject*        py_DestPort16;
    TML_TCHAR*       sDestPort;
    PyObject*        py_DestPort;
    TML_UINT32       iTimeout;
    TML_UINT64       iTemp64u; // Umgehung von Speicherzugriffsverletzung bei Python 3.4 unter LINUX x64

    TML_INT32   err = TML_SUCCESS;

    if (!PyArg_ParseTuple(args, "LOOOOOk", &shandle, &py_Profile, &py_SourceHost, &py_SourcePort, &py_DestHost, &py_DestPort, &iTemp64u))
        return NULL;
    iTimeout = (TML_UINT32)iTemp64u;
    if ( _unicodeConversion(py_Profile, &sProfile, &py_Profile16, tmlError) ) return NULL;
    if ( _unicodeConversion(py_SourceHost, &sSourceHost, &py_SourceHost16, tmlError) ) return NULL;
    if ( _unicodeConversion(py_SourcePort, &sSourcePort, &py_SourcePort16, tmlError) ) return NULL;
    if ( _unicodeConversion(py_DestHost, &sDestHost, &py_DestHost16, tmlError) ) return NULL;
    if ( _unicodeConversion(py_DestPort, &sDestPort, &py_DestPort16, tmlError) ) return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = tml_Bal_Send_UnsubscriptionRequest(shandle, sProfile, sSourceHost, sSourcePort, sDestHost, sDestPort, iTimeout);
    }
    catch (...) {
        err = TML_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    unicodeFree(py_Profile16, sProfile);
    unicodeFree(py_SourceHost16, sSourceHost);
    unicodeFree(py_SourcePort16, sSourcePort);
    unicodeFree(py_DestHost16, sDestHost);
    unicodeFree(py_DestPort16, sDestPort);
    
    if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Bal_Send_UnsubscriptionRequest failed (%d)",err);
            return NULL;
    }

    Py_RETURN_NONE;
}


/*
 *------------------------------------------------------
 *               tml_Bal_Set_MaxConnectionFailCount
 *------------------------------------------------------
 */
static PyObject * _tml_Bal_Set_MaxConnectionFailCount(PyObject *self, PyObject *args)
{
    TML_CORE_HANDLE  shandle;
    TML_UINT32       iCount;
    TML_UINT64       iTemp64u; // Umgehung von Speicherzugriffsverletzung bei Python 3.4 unter LINUX x64

    TML_INT32   err = TML_SUCCESS;

    if (!PyArg_ParseTuple(args, "Lk", &shandle, &iTemp64u))
        return NULL;
    iCount = (TML_UINT32)iTemp64u;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = tml_Bal_Set_MaxConnectionFailCount(shandle, iCount);
    }
    catch (...) {
        err = TML_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Bal_Set_MaxConnectionFailCount failed (%d)",err);
            return NULL;
    }

    return Py_BuildValue("k",iCount);
}


/*
 *------------------------------------------------------
 *               tml_Bal_Subscribe_MessageDestination
 *------------------------------------------------------
 */
static PyObject * _tml_Bal_Subscribe_MessageDestination(PyObject *self, PyObject *args)
{
    TML_CORE_HANDLE    shandle;
    PyObject*          py_Profile16;
    TML_TCHAR*         sProfile;
    PyObject*          py_Profile;
    PyObject*          py_Host16;
    TML_TCHAR*         sHost;
    PyObject*          py_Host;
    PyObject*          py_Port16;
    TML_TCHAR*         sPort;
    PyObject*          py_Port;

    TML_INT32   err = TML_SUCCESS;

    if (!PyArg_ParseTuple(args, "LOOO", &shandle, &py_Profile, &py_Host, &py_Port))
        return NULL;
    if ( _unicodeConversion(py_Profile, &sProfile, &py_Profile16, tmlError) ) return NULL;
    if ( _unicodeConversion(py_Host, &sHost, &py_Host16, tmlError) ) return NULL;
    if ( _unicodeConversion(py_Port, &sPort, &py_Port16, tmlError) ) return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = tml_Bal_Subscribe_MessageDestination(shandle, sProfile, sHost, sPort);
    }
    catch (...) {
        err = TML_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;

    unicodeFree(py_Profile16, sProfile);
    unicodeFree(py_Host16, sHost);
    unicodeFree(py_Port16, sPort);
    
    if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Bal_Subscribe_MessageDestination failed (%d)",err);
            return NULL;
    }

    Py_RETURN_NONE;
}


/*
 *------------------------------------------------------
 *               tml_Bal_Unsubscribe_All_MessageDestinations
 *------------------------------------------------------
 */
static PyObject * _tml_Bal_Unsubscribe_All_MessageDestinations(PyObject *self, PyObject *args)
{
    TML_CORE_HANDLE  shandle;
    PyObject*        py_Profile16 = NULL;
    SIDEX_TCHAR*     sProfile;
    PyObject*        py_Profile;
    bool             objIsNull = false;

    TML_INT32   err = TML_SUCCESS;

    if (!PyArg_ParseTuple(args, "LO", &shandle, &py_Profile)){
      return NULL;
    }
    else{
      if (Py_None == py_Profile){
        sProfile = (TML_TCHAR*)L"";
        objIsNull = true;
      }
      else{
        if ( _unicodeConversion(py_Profile, &sProfile, &py_Profile16, tmlError) ) return NULL;
      }
    }

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = tml_Bal_Unsubscribe_All_MessageDestinations(shandle, sProfile);
    }
    catch (...) {
        err = TML_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (!objIsNull){
      unicodeFree(py_Profile16, sProfile);
    }

    if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Bal_Unsubscribe_All_MessageDestinations failed (%d)",err);
            return NULL;
    }

    Py_RETURN_NONE;
}


/*
 *------------------------------------------------------
 *               tml_Bal_Unsubscribe_MessageDestination
 *------------------------------------------------------
 */
static PyObject * _tml_Bal_Unsubscribe_MessageDestination(PyObject *self, PyObject *args)
{
    TML_CORE_HANDLE  shandle;
    PyObject*          py_Profile16;
    TML_TCHAR*         sProfile;
    PyObject*          py_Profile;
    PyObject*          py_Host16;
    TML_TCHAR*         sHost;
    PyObject*          py_Host;
    PyObject*          py_Port16;
    TML_TCHAR*         sPort;
    PyObject*          py_Port;

    TML_INT32   err = TML_SUCCESS;

    if (!PyArg_ParseTuple(args, "LOOO", &shandle, &py_Profile, &py_Host, &py_Port))
        return NULL;
    if ( _unicodeConversion(py_Profile, &sProfile, &py_Profile16, tmlError) ) return NULL;
    if ( _unicodeConversion(py_Host, &sHost, &py_Host16, tmlError) ) return NULL;
    if ( _unicodeConversion(py_Port, &sPort, &py_Port16, tmlError) ) return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = tml_Bal_Unsubscribe_MessageDestination(shandle, sProfile, sHost, sPort);
    }
    catch (...) {
        err = TML_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    unicodeFree(py_Profile16, sProfile);
    unicodeFree(py_Host16, sHost);
    unicodeFree(py_Port16, sPort);

    if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Bal_Unsubscribe_MessageDestination failed (%d)",err);
            return NULL;
    }

    Py_RETURN_NONE;
}


/*
 *------------------------------------------------------
 *               tml_Bal_Set_OnBusyStatusRequest
 *------------------------------------------------------
 */
static PyObject * _tml_Bal_Set_OnBusyStatusRequest(PyObject *self, PyObject *args)
{
    TML_CORE_HANDLE      shandle;
    PyObject*            py_Profile16;
    SIDEX_TCHAR*         pProfile;
    PyObject*            py_Profile;
    PyObject*            pCBFunc;
    PyObject*            pCBData;
    TML_INT32            err = TML_SUCCESS;

    if (PyArg_ParseTuple(args, "LOOO", &shandle, &py_Profile, &pCBFunc, &pCBData)) {
        if (!PyCallable_Check(pCBFunc) && pCBFunc != Py_None) {
            PyErr_SetString(PyExc_TypeError, "parameter must be a callable function");
            return NULL;
        }
        if (!PyFunction_Check(pCBFunc) && pCBFunc != Py_None) {
            PyErr_SetString(PyExc_TypeError, "parameter must be a function");
            return NULL;
        }
        if ( _unicodeConversion(py_Profile, &pProfile, &py_Profile16, tmlError) ) return NULL;
        PythonCallbackData* customDispatchCallbackData = NULL;
        err = getCallbackDataFromDict(pProfile, pCBFunc, pCBData, &customDispatchCallbackData, m_balOnBusyStatusRequestDict);
        if (TML_SUCCESS == err){
          Py_BEGIN_ALLOW_THREADS;
          try {
            err = tml_Bal_Set_OnBusyStatusRequest(shandle, pProfile, onBalBusyStatusRequestCallback, customDispatchCallbackData);
          }
          catch (...) {
            err = TML_ERR_COMMON;
          }
          Py_END_ALLOW_THREADS;
        }

        unicodeFree(py_Profile16, pProfile);

        if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Bal_Set_OnBusyStatusRequest failed (%d)",err);
            return NULL;
        }
    }
    Py_RETURN_NONE;
}


/*
 *------------------------------------------------------
 *               tml_Bal_Set_OnCalculation
 *------------------------------------------------------
 */
static PyObject * _tml_Bal_Set_OnCalculation(PyObject *self, PyObject *args)
{
    TML_CORE_HANDLE      shandle;
    PyObject*            py_Profile16;
    SIDEX_TCHAR*         pProfile;
    PyObject*            py_Profile;
    PyObject*            pCBFunc;
    PyObject*            pCBData;
    TML_INT32            err = TML_SUCCESS;

    if (PyArg_ParseTuple(args, "LOOO", &shandle, &py_Profile, &pCBFunc, &pCBData)) {
        if (!PyCallable_Check(pCBFunc) && pCBFunc != Py_None) {
            PyErr_SetString(PyExc_TypeError, "parameter must be a callable function");
            return NULL;
        }
        if (!PyFunction_Check(pCBFunc) && pCBFunc != Py_None) {
            PyErr_SetString(PyExc_TypeError, "parameter must be a function");
            return NULL;
        }
        if ( _unicodeConversion(py_Profile, &pProfile, &py_Profile16, tmlError) ) return NULL;
        PythonCallbackData* customDispatchCallbackData = NULL;
        err = getCallbackDataFromDict(pProfile, pCBFunc, pCBData, &customDispatchCallbackData, m_balOnCalculationDict);
        if (TML_SUCCESS == err){
          Py_BEGIN_ALLOW_THREADS;
          try {
            err = tml_Bal_Set_OnCalculation(shandle, pProfile, onBalCalculationCallback, customDispatchCallbackData);
          }
          catch (...) {
            err = TML_ERR_COMMON;
          }
          Py_END_ALLOW_THREADS;
        }

        unicodeFree(py_Profile16, pProfile);

        if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Bal_Set_OnCalculation failed (%d)",err);
            return NULL;
        }
    }
    Py_RETURN_NONE;
}


/*
 *------------------------------------------------------
 *               tml_Bal_Set_OnPeerRegister
 *------------------------------------------------------
 */
static PyObject * _tml_Bal_Set_OnPeerRegister(PyObject *self, PyObject *args)
{
    TML_CORE_HANDLE      shandle;
    PyObject*            py_Profile16;
    SIDEX_TCHAR*         pProfile;
    PyObject*            py_Profile;
    PyObject*            pCBFunc;
    PyObject*            pCBData;
    TML_INT32            err = TML_SUCCESS;

    if (PyArg_ParseTuple(args, "LOOO", &shandle, &py_Profile, &pCBFunc, &pCBData)) {
        if (!PyCallable_Check(pCBFunc) && pCBFunc != Py_None) {
            PyErr_SetString(PyExc_TypeError, "parameter must be a callable function");
            return NULL;
        }
        if (!PyFunction_Check(pCBFunc) && pCBFunc != Py_None) {
            PyErr_SetString(PyExc_TypeError, "parameter must be a function");
            return NULL;
        }
        if ( _unicodeConversion(py_Profile, &pProfile, &py_Profile16, tmlError) ) return NULL;
        PythonCallbackData* customDispatchCallbackData = NULL;
        err = getCallbackDataFromDict(pProfile, pCBFunc, pCBData, &customDispatchCallbackData, m_balOnPeerRegisterDict);
        if (TML_SUCCESS == err){
          Py_BEGIN_ALLOW_THREADS;
          try {
            if (PyCallable_Check(pCBFunc)) {
              err = tml_Bal_Set_OnPeerRegister(shandle, pProfile, onPeerRegisterCallback, customDispatchCallbackData);
            }
            else{
              // It's not callable / reset the callback method:
              err = tml_Bal_Set_OnPeerRegister(shandle, pProfile, NULL, NULL);
            }
          }
          catch (...) {
            err = TML_ERR_COMMON;
          }
          Py_END_ALLOW_THREADS;
        }

        unicodeFree(py_Profile16, pProfile);

        if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Bal_Set_OnPeerRegister failed (%d)",err);
            return NULL;
        }
    }
    Py_RETURN_NONE;
}


/*
 *------------------------------------------------------
 *               tml_Bal_Set_OnPopulate
 *------------------------------------------------------
 */
static PyObject * _tml_Bal_Set_OnPopulate(PyObject *self, PyObject *args)
{
    TML_CORE_HANDLE      shandle;
    PyObject*            py_Profile16;
    SIDEX_TCHAR*         pProfile;
    PyObject*            py_Profile;
    PyObject*            pCBFunc;
    PyObject*            pCBData;
    TML_INT32            err = TML_SUCCESS;

    if (PyArg_ParseTuple(args, "LOOO", &shandle, &py_Profile, &pCBFunc, &pCBData)) {
        if (!PyCallable_Check(pCBFunc) && pCBFunc != Py_None) {
            PyErr_SetString(PyExc_TypeError, "parameter must be a callable function");
            return NULL;
        }
        if (!PyFunction_Check(pCBFunc) && pCBFunc != Py_None) {
            PyErr_SetString(PyExc_TypeError, "parameter must be a function");
            return NULL;
        }
        if ( _unicodeConversion(py_Profile, &pProfile, &py_Profile16, tmlError) ) return NULL;
        PythonCallbackData* customDispatchCallbackData = NULL;
        err = getCallbackDataFromDict(pProfile, pCBFunc, pCBData, &customDispatchCallbackData, m_balOnPopulateDict);
        if (TML_SUCCESS == err){
          Py_BEGIN_ALLOW_THREADS;
          try {
            err = tml_Bal_Set_OnPopulate(shandle, pProfile, onPopulateCallback, customDispatchCallbackData);
          }
          catch (...) {
            err = TML_ERR_COMMON;
          }
          Py_END_ALLOW_THREADS;
        }

        unicodeFree(py_Profile16, pProfile);

        if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Bal_Set_OnPopulate failed (%d)",err);
            return NULL;
        }
    }
    Py_RETURN_NONE;
}


/*
 *------------------------------------------------------
 *               tml_Evt_Get_MaxQueuedEventMessages
 *------------------------------------------------------
 */
static PyObject * _tml_Evt_Get_MaxQueuedEventMessages(PyObject *self, PyObject *args)
{
    TML_CORE_HANDLE  shandle;
    TML_UINT32       iCount;

    TML_INT32   err = TML_SUCCESS;

    if (!PyArg_ParseTuple(args, "L", &shandle))
        return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = tml_Evt_Get_MaxQueuedEventMessages(shandle, &iCount);
    }
    catch (...) {
        err = TML_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Evt_Get_MaxQueuedEventMessages failed (%d)",err);
            return NULL;
    }

    return Py_BuildValue("k",iCount);
}


/*
 *------------------------------------------------------
 *               tml_Evt_Get_MaxConnectionFailCount
 *------------------------------------------------------
 */
static PyObject * _tml_Evt_Get_MaxConnectionFailCount(PyObject *self, PyObject *args)
{
    TML_CORE_HANDLE  shandle;
    TML_UINT32       iCount;

    TML_INT32   err = TML_SUCCESS;

    if (!PyArg_ParseTuple(args, "L", &shandle))
        return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = tml_Evt_Get_MaxConnectionFailCount(shandle, &iCount);
    }
    catch (...) {
        err = TML_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Evt_Get_MaxConnectionFailCount failed (%d)",err);
            return NULL;
    }

    return Py_BuildValue("k",iCount);
}


/*
 *------------------------------------------------------
 *               tml_Evt_Get_Subscribed_MessageDestinations
 *------------------------------------------------------
 */
static PyObject * _tml_Evt_Get_Subscribed_MessageDestinations(PyObject *self, PyObject *args)
{
    TML_CORE_HANDLE  shandle;
    PyObject*        py_Profile16;
    SIDEX_TCHAR*     sProfile;
    PyObject*        py_Profile;
    PyObject*        pyList = PyList_New(0);

    TML_INT32   err = TML_SUCCESS;

    if (!PyArg_ParseTuple(args, "LO", &shandle, &py_Profile))
        return NULL;
    if ( _unicodeConversion(py_Profile, &sProfile, &py_Profile16, tmlError) ) return NULL;

    TML_INT32        vColumns = 0;
    TML_INT32        vRows = 0;
    SIDEX_VARIANT    sSubscriptions = SIDEX_HANDLE_TYPE_NULL;
    Py_BEGIN_ALLOW_THREADS;
    try {
      err = tml_Evt_Get_Subscribed_MessageDestinations(shandle, sProfile, &sSubscriptions);
    }
    catch (...) {
        err = TML_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    unicodeFree(py_Profile16, sProfile);

    if (err == TML_SUCCESS) {
      if (SIDEX_HANDLE_TYPE_NULL != sSubscriptions){
        err = sidex_Variant_Table_Columns (sSubscriptions, &vColumns);
        if (err == TML_SUCCESS) {
          err = sidex_Variant_Table_Rows (sSubscriptions, &vRows);
          if (err == TML_SUCCESS) {
            for (TML_INT32 i = 0; i< vRows && err == TML_SUCCESS; ++i){
              PyObject* pyDict = PyDict_New();
              for (TML_INT32 j = 0; j< vColumns && err == TML_SUCCESS; ++j){
                wchar_t* colName;
                err = sidex_Variant_Table_GetColumnName (sSubscriptions, j, &colName);
                if (err == TML_SUCCESS) {
                    SIDEX_VARIANT vCell;
                    err = sidex_Variant_Table_GetField_X(sSubscriptions, i, colName, &vCell);
                    if (err == TML_SUCCESS) {
                      wchar_t* sTmpCell;
                      TML_INT32 iLength;
                      err = sidex_Variant_As_String_X(vCell, &sTmpCell, &iLength);
                      if (err == TML_SUCCESS) {
                        PyObject* key = utf32_utf16_decode((TML_TCHAR*)colName, -1);
                        PyObject* val = utf32_utf16_decode((TML_TCHAR*)sTmpCell, -1);
                        PyDict_SetItem(pyDict, key, val);
                        Py_XDECREF(key);
                        Py_XDECREF(val);
                      }
                    }
                }
              }
              PyList_Append(pyList, pyDict);
              Py_XDECREF(pyDict);
            }
          }
        }
        sidex_Variant_DecRef(sSubscriptions);
      }
    }
    else{
      PyErr_Format(tmlError, "tml_Evt_Get_Subscribed_MessageDestinations failed (%d)",err);
      return NULL;
    }
    return pyList;
}


/*
 *------------------------------------------------------
 *               tml_Evt_Send_Message
 *------------------------------------------------------
 */
static PyObject * _tml_Evt_Send_Message(PyObject *self, PyObject *args)
{
    TML_CORE_HANDLE     shandle;
    TML_COMMAND_HANDLE  cmdHandle;
    PyObject*           py_Profile16;
    SIDEX_TCHAR*        sProfile;
    PyObject*           py_Profile;

    TML_INT32   err = TML_SUCCESS;

    if (!PyArg_ParseTuple(args, "LLO", &shandle, &cmdHandle, &py_Profile))
        return NULL;
    if ( _unicodeConversion(py_Profile, &sProfile, &py_Profile16, tmlError) ) return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = tml_Evt_Send_Message(shandle, cmdHandle, sProfile);
    }
    catch (...) {
        err = TML_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    unicodeFree(py_Profile16, sProfile);
    
    if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Evt_Send_Message failed (%d)",err);
            return NULL;
    }

    Py_RETURN_NONE;
}


/*
 *------------------------------------------------------
 *               tml_Evt_Send_SubscriptionRequest
 *------------------------------------------------------
 */
static PyObject * _tml_Evt_Send_SubscriptionRequest(PyObject *self, PyObject *args)
{
    TML_CORE_HANDLE  shandle;
    PyObject*        py_Profile16;
    TML_TCHAR*       sProfile;
    PyObject*        py_Profile;
    PyObject*        py_SourceHost16;
    TML_TCHAR*       sSourceHost;
    PyObject*        py_SourceHost;
    PyObject*        py_SourcePort16;
    TML_TCHAR*       sSourcePort;
    PyObject*        py_SourcePort;
    PyObject*        py_DestHost16;
    TML_TCHAR*       sDestHost;
    PyObject*        py_DestHost;
    PyObject*        py_DestPort16;
    TML_TCHAR*       sDestPort;
    PyObject*        py_DestPort;
    TML_UINT32       iTimeout;
    TML_UINT64       iTemp64u; // Umgehung von Speicherzugriffsverletzung bei Python 3.4 unter LINUX x64

    TML_INT32   err = TML_SUCCESS;

    if (!PyArg_ParseTuple(args, "LOOOOOk", &shandle, &py_Profile, &py_SourceHost, &py_SourcePort, &py_DestHost, &py_DestPort, &iTemp64u))
        return NULL;
    iTimeout = (TML_UINT32)iTemp64u;
    if ( _unicodeConversion(py_Profile, &sProfile, &py_Profile16, tmlError) ) return NULL;
    if ( _unicodeConversion(py_SourceHost, &sSourceHost, &py_SourceHost16, tmlError) ) return NULL;
    if ( _unicodeConversion(py_SourcePort, &sSourcePort, &py_SourcePort16, tmlError) ) return NULL;
    if ( _unicodeConversion(py_DestHost, &sDestHost, &py_DestHost16, tmlError) ) return NULL;
    if ( _unicodeConversion(py_DestPort, &sDestPort, &py_DestPort16, tmlError) ) return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = tml_Evt_Send_SubscriptionRequest(shandle, sProfile, sSourceHost, sSourcePort, sDestHost, sDestPort, iTimeout);
    }
    catch (...) {
        err = TML_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    unicodeFree(py_Profile16, sProfile);
    unicodeFree(py_SourceHost16, sSourceHost);
    unicodeFree(py_SourcePort16, sSourcePort);
    unicodeFree(py_DestHost16, sDestHost);
    unicodeFree(py_DestPort16, sDestPort);
    
    if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Evt_Send_SubscriptionRequest failed (%d)",err);
            return NULL;
    }

    Py_RETURN_NONE;
}


/*
 *------------------------------------------------------
 *               tml_Evt_Send_UnsubscriptionRequest
 *------------------------------------------------------
 */
static PyObject * _tml_Evt_Send_UnsubscriptionRequest(PyObject *self, PyObject *args)
{
    TML_CORE_HANDLE  shandle;
    PyObject*        py_Profile16;
    TML_TCHAR*       sProfile;
    PyObject*        py_Profile;
    PyObject*        py_SourceHost16;
    TML_TCHAR*       sSourceHost;
    PyObject*        py_SourceHost;
    PyObject*        py_SourcePort16;
    TML_TCHAR*       sSourcePort;
    PyObject*        py_SourcePort;
    PyObject*        py_DestHost16;
    TML_TCHAR*       sDestHost;
    PyObject*        py_DestHost;
    PyObject*        py_DestPort16;
    TML_TCHAR*       sDestPort;
    PyObject*        py_DestPort;
    TML_UINT32       iTimeout;
    TML_UINT64       iTemp64u; // Umgehung von Speicherzugriffsverletzung bei Python 3.4 unter LINUX x64

    TML_INT32   err = TML_SUCCESS;

    if (!PyArg_ParseTuple(args, "LOOOOOk", &shandle, &py_Profile, &py_SourceHost, &py_SourcePort, &py_DestHost, &py_DestPort, &iTemp64u))
        return NULL;
    iTimeout = (TML_UINT32)iTemp64u;
    if ( _unicodeConversion(py_Profile, &sProfile, &py_Profile16, tmlError) ) return NULL;
    if ( _unicodeConversion(py_SourceHost, &sSourceHost, &py_SourceHost16, tmlError) ) return NULL;
    if ( _unicodeConversion(py_SourcePort, &sSourcePort, &py_SourcePort16, tmlError) ) return NULL;
    if ( _unicodeConversion(py_DestHost, &sDestHost, &py_DestHost16, tmlError) ) return NULL;
    if ( _unicodeConversion(py_DestPort, &sDestPort, &py_DestPort16, tmlError) ) return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = tml_Evt_Send_UnsubscriptionRequest(shandle, sProfile, sSourceHost, sSourcePort, sDestHost, sDestPort, iTimeout);
    }
    catch (...) {
        err = TML_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    unicodeFree(py_Profile16, sProfile);
    unicodeFree(py_SourceHost16, sSourceHost);
    unicodeFree(py_SourcePort16, sSourcePort);
    unicodeFree(py_DestHost16, sDestHost);
    unicodeFree(py_DestPort16, sDestPort);
    
    if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Evt_Send_UnsubscriptionRequest failed (%d)",err);
            return NULL;
    }

    Py_RETURN_NONE;
}


/*
 *------------------------------------------------------
 *               tml_Evt_Set_MaxConnectionFailCount
 *------------------------------------------------------
 */
static PyObject * _tml_Evt_Set_MaxConnectionFailCount(PyObject *self, PyObject *args)
{
    TML_CORE_HANDLE  shandle;
    TML_UINT32       iCount;
    TML_UINT64       iTemp64u; // Umgehung von Speicherzugriffsverletzung bei Python 3.4 unter LINUX x64

    TML_INT32   err = TML_SUCCESS;

    if (!PyArg_ParseTuple(args, "Lk", &shandle, &iTemp64u))
        return NULL;
    iCount = (TML_UINT32)iTemp64u;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = tml_Evt_Set_MaxConnectionFailCount(shandle, iCount);
    }
    catch (...) {
        err = TML_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Evt_Set_MaxConnectionFailCount failed (%d)",err);
            return NULL;
    }

    return Py_BuildValue("k",iCount);
}


/*
 *------------------------------------------------------
 *               tml_Evt_Set_MaxQueuedEventMessages
 *------------------------------------------------------
 */
static PyObject * _tml_Evt_Set_MaxQueuedEventMessages(PyObject *self, PyObject *args)
{
    TML_CORE_HANDLE  shandle;
    TML_UINT32       iCount;
    TML_UINT64       iTemp64u; // Umgehung von Speicherzugriffsverletzung bei Python 3.4 unter LINUX x64

    TML_INT32   err = TML_SUCCESS;

    if (!PyArg_ParseTuple(args, "Lk", &shandle, &iTemp64u))
        return NULL;
    iCount = (TML_UINT32)iTemp64u;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = tml_Evt_Set_MaxQueuedEventMessages(shandle, iCount);
    }
    catch (...) {
        err = TML_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Evt_Set_MaxQueuedEventMessages failed (%d)",err);
            return NULL;
    }

    return Py_BuildValue("k",iCount);
}


/*
 *------------------------------------------------------
 *               tml_Evt_Set_OnError
 *------------------------------------------------------
 */
static PyObject * _tml_Evt_Set_OnError(PyObject *self, PyObject *args)
{
    TML_CORE_HANDLE      shandle;
    PyObject*            py_Profile16;
    SIDEX_TCHAR*         pProfile;
    PyObject*            py_Profile;
    PyObject*            pCBFunc;
    PyObject*            pCBData;
    TML_INT32            err = TML_SUCCESS;

    if (PyArg_ParseTuple(args, "LOOO", &shandle, &py_Profile, &pCBFunc, &pCBData)) {
        if (!PyCallable_Check(pCBFunc) && pCBFunc != Py_None) {
            PyErr_SetString(PyExc_TypeError, "parameter must be a callable function");
            return NULL;
        }
        if (!PyFunction_Check(pCBFunc) && pCBFunc != Py_None) {
            PyErr_SetString(PyExc_TypeError, "parameter must be a function");
            return NULL;
        }
        if ( _unicodeConversion(py_Profile, &pProfile, &py_Profile16, tmlError) ) return NULL;
        PythonCallbackData* customDispatchCallbackData = NULL;
        err = getCallbackDataFromDict(pProfile, pCBFunc, pCBData, &customDispatchCallbackData, m_evtOnErrorDict);
        if (TML_SUCCESS == err){
          Py_BEGIN_ALLOW_THREADS;
          try {
            err = tml_Evt_Set_OnError(shandle, pProfile, onEvtErrorCallback, customDispatchCallbackData);
          }
          catch (...) {
            err = TML_ERR_COMMON;
          }
          Py_END_ALLOW_THREADS;
        }

        unicodeFree(py_Profile16, pProfile);

        if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Evt_Set_OnError failed (%d)",err);
            return NULL;
        }
    }
    Py_RETURN_NONE;
}


/*
 *------------------------------------------------------
 *               tml_Evt_Set_OnPeerRegister
 *------------------------------------------------------
 */
static PyObject * _tml_Evt_Set_OnPeerRegister(PyObject *self, PyObject *args)
{
    TML_CORE_HANDLE      shandle;
    PyObject*            py_Profile16;
    SIDEX_TCHAR*         pProfile;
    PyObject*            py_Profile;
    PyObject*            pCBFunc;
    PyObject*            pCBData;
    TML_INT32            err = TML_SUCCESS;

    if (PyArg_ParseTuple(args, "LOOO", &shandle, &py_Profile, &pCBFunc, &pCBData)) {
        if (!PyCallable_Check(pCBFunc) && pCBFunc != Py_None) {
            PyErr_SetString(PyExc_TypeError, "parameter must be a callable function");
            return NULL;
        }
        if (!PyFunction_Check(pCBFunc) && pCBFunc != Py_None) {
            PyErr_SetString(PyExc_TypeError, "parameter must be a function");
            return NULL;
        }
        if ( _unicodeConversion(py_Profile, &pProfile, &py_Profile16, tmlError) ) return NULL;
        PythonCallbackData* customDispatchCallbackData = NULL;
        err = getCallbackDataFromDict(pProfile, pCBFunc, pCBData, &customDispatchCallbackData, m_evtOnPeerRegisterDict);
        if (TML_SUCCESS == err){
          Py_BEGIN_ALLOW_THREADS;
          try {
            if (PyCallable_Check(pCBFunc)) {
              err = tml_Evt_Set_OnPeerRegister(shandle, pProfile, onPeerRegisterCallback, customDispatchCallbackData);
            }
            else{
              // It's not callable / reset the callback method:
              err = tml_Evt_Set_OnPeerRegister(shandle, pProfile, NULL, NULL);
            }
          }
          catch (...) {
            err = TML_ERR_COMMON;
          }
          Py_END_ALLOW_THREADS;
        }

        unicodeFree(py_Profile16, pProfile);

        if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Evt_Set_OnPeerRegister failed (%d)",err);
            return NULL;
        }
    }
    Py_RETURN_NONE;
}


/*
 *------------------------------------------------------
 *               tml_Evt_Set_OnPopulate
 *------------------------------------------------------
 */
static PyObject * _tml_Evt_Set_OnPopulate(PyObject *self, PyObject *args)
{
    TML_CORE_HANDLE      shandle;
    PyObject*            py_Profile16;
    SIDEX_TCHAR*         pProfile;
    PyObject*            py_Profile;
    PyObject*            pCBFunc;
    PyObject*            pCBData;
    TML_INT32            err = TML_SUCCESS;

    if (PyArg_ParseTuple(args, "LOOO", &shandle, &py_Profile, &pCBFunc, &pCBData)) {
        if (!PyCallable_Check(pCBFunc) && pCBFunc != Py_None) {
            PyErr_SetString(PyExc_TypeError, "parameter must be a callable function");
            return NULL;
        }
        if (!PyFunction_Check(pCBFunc) && pCBFunc != Py_None) {
            PyErr_SetString(PyExc_TypeError, "parameter must be a function");
            return NULL;
        }
        if ( _unicodeConversion(py_Profile, &pProfile, &py_Profile16, tmlError) ) return NULL;
        PythonCallbackData* customDispatchCallbackData = NULL;
        err = getCallbackDataFromDict(pProfile, pCBFunc, pCBData, &customDispatchCallbackData, m_evtOnPopulateDict);
        if (TML_SUCCESS == err){
          Py_BEGIN_ALLOW_THREADS;
          try {
            err = tml_Evt_Set_OnPopulate(shandle, pProfile, onPopulateCallback, customDispatchCallbackData);
          }
          catch (...) {
            err = TML_ERR_COMMON;
          }
          Py_END_ALLOW_THREADS;
        }

        unicodeFree(py_Profile16, pProfile);

        if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Evt_Set_OnPopulate failed (%d)",err);
            return NULL;
        }
    }
    Py_RETURN_NONE;
}


/*
 *------------------------------------------------------
 *               tml_Evt_Set_OnQueueOverflow
 *------------------------------------------------------
 */
static PyObject * _tml_Evt_Set_OnQueueOverflow(PyObject *self, PyObject *args)
{
    TML_CORE_HANDLE      shandle;
    PyObject*            py_Profile16;
    SIDEX_TCHAR*         pProfile;
    PyObject*            py_Profile;
    PyObject*            pCBFunc;
    PyObject*            pCBData;
    TML_INT32            err = TML_SUCCESS;

    if (PyArg_ParseTuple(args, "LOOO", &shandle, &py_Profile, &pCBFunc, &pCBData)) {
        if (!PyCallable_Check(pCBFunc) && pCBFunc != Py_None) {
            PyErr_SetString(PyExc_TypeError, "parameter must be a callable function");
            return NULL;
        }
        if (!PyFunction_Check(pCBFunc) && pCBFunc != Py_None) {
            PyErr_SetString(PyExc_TypeError, "parameter must be a function");
            return NULL;
        }
        if ( _unicodeConversion(py_Profile, &pProfile, &py_Profile16, tmlError) ) return NULL;
        PythonCallbackData* customDispatchCallbackData = NULL;
        err = getCallbackDataFromDict(pProfile, pCBFunc, pCBData, &customDispatchCallbackData, m_evtOnQueueOverflowDict);
        if (TML_SUCCESS == err){
          Py_BEGIN_ALLOW_THREADS;
          try {
            err = tml_Evt_Set_OnQueueOverflow(shandle, pProfile, onEvtQueueOverflowCallback, customDispatchCallbackData);
          }
          catch (...) {
            err = TML_ERR_COMMON;
          }
          Py_END_ALLOW_THREADS;
        }

        unicodeFree(py_Profile16, pProfile);

        if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Evt_Set_OnQueueOverflow failed (%d)",err);
            return NULL;
        }
    }
    Py_RETURN_NONE;
}


/*
 *------------------------------------------------------
 *               tml_Evt_Subscribe_MessageDestination
 *------------------------------------------------------
 */
static PyObject * _tml_Evt_Subscribe_MessageDestination(PyObject *self, PyObject *args)
{
    TML_CORE_HANDLE  shandle;
    PyObject*          py_Profile16;
    TML_TCHAR*         sProfile;
    PyObject*          py_Profile;
    PyObject*          py_Host16;
    TML_TCHAR*         sHost;
    PyObject*          py_Host;
    PyObject*          py_Port16;
    TML_TCHAR*         sPort;
    PyObject*          py_Port;

    TML_INT32   err = TML_SUCCESS;

    if (!PyArg_ParseTuple(args, "LOOO", &shandle, &py_Profile, &py_Host, &py_Port))
        return NULL;
    if ( _unicodeConversion(py_Profile, &sProfile, &py_Profile16, tmlError) ) return NULL;
    if ( _unicodeConversion(py_Host, &sHost, &py_Host16, tmlError) ) return NULL;
    if ( _unicodeConversion(py_Port, &sPort, &py_Port16, tmlError) ) return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = tml_Evt_Subscribe_MessageDestination(shandle, sProfile, sHost, sPort);
    }
    catch (...) {
        err = TML_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;

    unicodeFree(py_Profile16, sProfile);
    unicodeFree(py_Host16, sHost);
    unicodeFree(py_Port16, sPort);
    
    if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Evt_Subscribe_MessageDestination failed (%d)",err);
            return NULL;
    }

    Py_RETURN_NONE;
}


/*
 *------------------------------------------------------
 *               tml_Evt_Unsubscribe_All_MessageDestinations
 *------------------------------------------------------
 */
static PyObject * _tml_Evt_Unsubscribe_All_MessageDestinations(PyObject *self, PyObject *args)
{
    TML_CORE_HANDLE  shandle;
    PyObject*        py_Profile16 = NULL;
    SIDEX_TCHAR*     sProfile;
    PyObject*        py_Profile;
    bool             objIsNull = false;

    TML_INT32   err = TML_SUCCESS;

    if (!PyArg_ParseTuple(args, "LO", &shandle, &py_Profile)){
      return NULL;
    }
    else{
      if (Py_None == py_Profile){
        sProfile = (TML_TCHAR*)L"";
        objIsNull = true;
      }
      else{
        if ( _unicodeConversion(py_Profile, &sProfile, &py_Profile16, tmlError) ) return NULL;
      }
    }

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = tml_Evt_Unsubscribe_All_MessageDestinations(shandle, sProfile);
    }
    catch (...) {
        err = TML_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (!objIsNull){
      unicodeFree(py_Profile16, sProfile);
    }

    if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Evt_Unsubscribe_All_MessageDestinations failed (%d)",err);
            return NULL;
    }

    Py_RETURN_NONE;
}


/*
 *------------------------------------------------------
 *               tml_Evt_Unsubscribe_MessageDestination
 *------------------------------------------------------
 */
static PyObject * _tml_Evt_Unsubscribe_MessageDestination(PyObject *self, PyObject *args)
{
    TML_CORE_HANDLE shandle;
    PyObject*       py_Profile16;
    TML_TCHAR*      sProfile;
    PyObject*       py_Profile;
    PyObject*       py_Host16;
    TML_TCHAR*      sHost;
    PyObject*       py_Host;
    PyObject*       py_Port16;
    TML_TCHAR*      sPort;
    PyObject*       py_Port;

    TML_INT32   err = TML_SUCCESS;

    if (!PyArg_ParseTuple(args, "LOOO", &shandle, &py_Profile, &py_Host, &py_Port))
        return NULL;
    if ( _unicodeConversion(py_Profile, &sProfile, &py_Profile16, tmlError) ) return NULL;
    if ( _unicodeConversion(py_Host, &sHost, &py_Host16, tmlError) ) return NULL;
    if ( _unicodeConversion(py_Port, &sPort, &py_Port16, tmlError) ) return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = tml_Evt_Unsubscribe_MessageDestination(shandle, sProfile, sHost, sPort);
    }
    catch (...) {
        err = TML_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    unicodeFree(py_Profile16, sProfile);
    unicodeFree(py_Host16, sHost);
    unicodeFree(py_Port16, sPort);

    if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_Evt_Unsubscribe_MessageDestination failed (%d)",err);
            return NULL;
    }

    Py_RETURN_NONE;
}


/*
 *------------------------------------------------------
 *               tml_RecStream_Close
 *------------------------------------------------------
 */
static PyObject * _tml_RecStream_Close(PyObject *self, PyObject *args)
{
    TML_CORE_HANDLE  shandle;
    TML_INT64        iID;
    PyObject*        bRetainOpen;

    TML_INT32   err = TML_SUCCESS;

    if (!PyArg_ParseTuple(args, "LLO", &shandle, &iID, &bRetainOpen))
        return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
      err = tml_RecStream_Close(shandle, iID, PyObject_IsTrue(bRetainOpen));
    }
    catch (...) {
      err = TML_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;

    // Remove the entry out of the dictionary:
    removeRecStrmCallbackDataFromDict(m_receiverStrmDict, iID);
    
    if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_RecStream_Close failed (%d)",err);
            return NULL;
    }
    Py_RETURN_NONE;
}


/*
 *------------------------------------------------------
 *               tml_RecStream_DownloadData
 *------------------------------------------------------
 */
static PyObject * _tml_RecStream_DownloadData(PyObject *self, PyObject *args)
{
    TML_CORE_HANDLE  shandle;
    TML_INT64        iID;
    TML_UINT32       iBufferSize;
    TML_UINT64       iTemp64u; // Umgehung von Speicherzugriffsverletzung bei Python 3.4 unter LINUX x64
    PyObject*        pCB_Rec_Strm_DLD_Block_Func;
    PyObject*        pCB_Rec_Strm_DLD_Block_Data;
    PyObject*        pCB_Rec_Strm_DLD_Finish_Func;
    PyObject*        pCB_Rec_Strm_DLD_Finish_Data;

    TML_INT32   err = TML_SUCCESS;

   if (PyArg_ParseTuple(args, "LLkOOOO", &shandle, &iID, &iTemp64u, 
              &pCB_Rec_Strm_DLD_Block_Func, &pCB_Rec_Strm_DLD_Block_Data,
              &pCB_Rec_Strm_DLD_Finish_Func, &pCB_Rec_Strm_DLD_Finish_Data)){
        iBufferSize = (TML_UINT32)iTemp64u;
        if (!PyCallable_Check(pCB_Rec_Strm_DLD_Block_Func) && pCB_Rec_Strm_DLD_Block_Func != Py_None) {
            PyErr_SetString(PyExc_TypeError, "parameter must be a callable function");
            return NULL;
        }
        if (!PyFunction_Check(pCB_Rec_Strm_DLD_Block_Func) && pCB_Rec_Strm_DLD_Block_Func != Py_None) {
            PyErr_SetString(PyExc_TypeError, "parameter must be a function");
            return NULL;
        }
        if (!PyCallable_Check(pCB_Rec_Strm_DLD_Finish_Func) && pCB_Rec_Strm_DLD_Finish_Func != Py_None) {
            PyErr_SetString(PyExc_TypeError, "parameter must be a callable function");
            return NULL;
        }
        if (!PyFunction_Check(pCB_Rec_Strm_DLD_Finish_Func) && pCB_Rec_Strm_DLD_Finish_Func != Py_None) {
            PyErr_SetString(PyExc_TypeError, "parameter must be a function");
            return NULL;
        }
        PythonReceiverStrmCallbackData* callbackData = NULL;
        err = getRecStrmCallbackDataFromDict(iID, 
                                              pCB_Rec_Strm_DLD_Block_Func, pCB_Rec_Strm_DLD_Block_Data,
                                              pCB_Rec_Strm_DLD_Finish_Func, pCB_Rec_Strm_DLD_Finish_Data, 
                                              &callbackData, m_receiverStrmDict);
        if (TML_SUCCESS == err){
          Py_BEGIN_ALLOW_THREADS;
          try {
            err = tml_RecStream_DownloadData(shandle, iID, iBufferSize, onRecStrmDldBlockCallback, callbackData,
                                                                        onRecStrmDldFinishedCallback, callbackData);
          }
          catch (...) {
            err = TML_ERR_COMMON;
          }
          Py_END_ALLOW_THREADS;
        }
        if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_RecStream_DownloadData failed (%d)",err);
            return NULL;
        }
    }
    Py_RETURN_NONE;
}


/*
 *------------------------------------------------------
 *               tml_RecStream_GetPosition
 *------------------------------------------------------
 */
static PyObject * _tml_RecStream_GetPosition(PyObject *self, PyObject *args)
{
    TML_CORE_HANDLE  shandle;
    TML_INT64        iID;
    TML_INT64        iPosition;

    TML_INT32   err = TML_SUCCESS;

    if (!PyArg_ParseTuple(args, "LL", &shandle, &iID))
        return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = tml_RecStream_GetPosition(shandle, iID, &iPosition);
    }
    catch (...) {
        err = TML_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_RecStream_GetPosition failed (%d)",err);
            return NULL;
    }
    return Py_BuildValue("L",iPosition);
}


/*
 *------------------------------------------------------
 *               tml_RecStream_GetSize
 *------------------------------------------------------
 */
static PyObject * _tml_RecStream_GetSize(PyObject *self, PyObject *args)
{
    TML_CORE_HANDLE  shandle;
    TML_INT64        iID;
    TML_INT64        iSize;

    TML_INT32   err = TML_SUCCESS;

    if (!PyArg_ParseTuple(args, "LL", &shandle, &iID))
        return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = tml_RecStream_GetSize(shandle, iID, &iSize);
    }
    catch (...) {
        err = TML_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_RecStream_GetSize failed (%d)",err);
            return NULL;
    }
    return Py_BuildValue("L",iSize);
}


/*
 *------------------------------------------------------
 *               tml_RecStream_Open
 *------------------------------------------------------
 */
static PyObject * _tml_RecStream_Open(PyObject *self, PyObject *args)
{
    TML_CORE_HANDLE shandle;
    TML_INT64       iID;
    PyObject*       py_Profile16;
    TML_TCHAR*      sProfile;
    PyObject*       py_Profile;
    PyObject*       py_Host16;
    TML_TCHAR*      sHost;
    PyObject*       py_Host;
    PyObject*       py_Port16;
    TML_TCHAR*      sPort;
    PyObject*       py_Port;

    TML_INT32   err = TML_SUCCESS;

    if (!PyArg_ParseTuple(args, "LLOOO", &shandle, &iID, &py_Profile, &py_Host, &py_Port))
        return NULL;
    if ( _unicodeConversion(py_Profile, &sProfile, &py_Profile16, tmlError) ) return NULL;
    if ( _unicodeConversion(py_Host, &sHost, &py_Host16, tmlError) ) return NULL;
    if ( _unicodeConversion(py_Port, &sPort, &py_Port16, tmlError) ) return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = tml_RecStream_Open(shandle, iID, sProfile, sHost, sPort);
    }
    catch (...) {
        err = TML_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    unicodeFree(py_Profile16, sProfile);
    unicodeFree(py_Host16, sHost);
    unicodeFree(py_Port16, sPort);

    if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_RecStream_Open failed (%d)",err);
            return NULL;
    }
    Py_RETURN_NONE;
}


/*
 *------------------------------------------------------
 *               tml_RecStream_Read
 *------------------------------------------------------
 */
static PyObject * _tml_RecStream_Read(PyObject *self, PyObject *args)
{
    TML_CORE_HANDLE  shandle;
    TML_INT64        iID;
    TML_INT32        iCount;
    TML_INT64        iTemp64; // Umgehung von Speicherzugriffsverletzung bei Python 3.4 unter LINUX x64
    TML_INT32        bytesRead = 0;
    PyObject         *retValue;

    TML_INT32   err = TML_SUCCESS;

    if (!PyArg_ParseTuple(args, "LLl", &shandle, &iID, &iTemp64))
        return NULL;
    iCount = (TML_INT32)iTemp64;
    char* buffer = new char[iCount+1];

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = tml_RecStream_Read(shandle, iID, buffer, iCount, &bytesRead);
    }
    catch (...) {
        err = TML_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_RecStream_Read failed (%d)",err);
            delete buffer;
            return NULL;
    }
    else{
#if PY_MAJOR_VERSION < 3
            retValue = Py_BuildValue("s#", buffer, bytesRead);
#else
            retValue = Py_BuildValue("y#", buffer, bytesRead);
#endif
            delete buffer;
    }
    return retValue;
}


/*
 *------------------------------------------------------
 *               tml_RecStream_ReadBuffer
 *------------------------------------------------------
 */
static PyObject * _tml_RecStream_ReadBuffer(PyObject *self, PyObject *args)
{
    TML_CORE_HANDLE  shandle;
    TML_INT64        iID;
    TML_INT32        iCount;
    TML_INT64        iTemp64; // Umgehung von Speicherzugriffsverletzung bei Python 3.4 unter LINUX x64
    PyObject         *retValue;

    TML_INT32   err = TML_SUCCESS;

    if (!PyArg_ParseTuple(args, "LLl", &shandle, &iID, &iTemp64))
        return NULL;
    iCount = (TML_INT32)iTemp64;
    char* buffer = new char[iCount+1];

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = tml_RecStream_ReadBuffer(shandle, iID, buffer, iCount);
    }
    catch (...) {
        err = TML_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_RecStream_ReadBuffer failed (%d)",err);
            delete buffer;
            return NULL;
    }
    else{
      // Must use "s#" instead of "s" because we must transport binary data too 
#if PY_MAJOR_VERSION < 3
      retValue = Py_BuildValue("s#",buffer, iCount);
#else
      retValue = Py_BuildValue("y#",buffer, iCount);
#endif
    }
    return retValue;
}


/*
 *------------------------------------------------------
 *               tml_RecStream_Seek
 *------------------------------------------------------
 */
static PyObject * _tml_RecStream_Seek(PyObject *self, PyObject *args)
{
    TML_CORE_HANDLE  shandle;
    TML_INT64        iID;
    TML_INT64        iSeekPos;
    tmlSeekOrigin    iOrigin;
    TML_INT64        iTemp64; // Umgehung von Speicherzugriffsverletzung bei Python 3.4 unter LINUX x64

    TML_INT32   err = TML_SUCCESS;

    if (!PyArg_ParseTuple(args, "LLLl", &shandle, &iID, &iSeekPos, &iTemp64))
        return NULL;
    iOrigin = (tmlSeekOrigin)iTemp64;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = tml_RecStream_Seek(shandle, iID, iSeekPos, iOrigin);
    }
    catch (...) {
        err = TML_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_RecStream_Seek failed (%d)",err);
            return NULL;
    }
    Py_RETURN_NONE;
}


/*
 *------------------------------------------------------
 *               tml_RecStream_Write
 *------------------------------------------------------
 */
static PyObject * _tml_RecStream_Write(PyObject *self, PyObject *args)
{
    TML_CORE_HANDLE  shandle;
    TML_INT64        iID;
    TML_POINTER      buffer;
    TML_INT32        count;

    TML_INT32   err = TML_SUCCESS;

#if PY_MAJOR_VERSION < 3
     if (!PyArg_ParseTuple(args, "LLs#", &shandle, &iID, &buffer, &count))
#else
     if (!PyArg_ParseTuple(args, "LLy#", &shandle, &iID, &buffer, &count))
#endif
        return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = tml_RecStream_Write(shandle, iID, buffer, count);
    }
    catch (...) {
        err = TML_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_RecStream_Write failed (%d)",err);
            return NULL;
    }
    Py_RETURN_NONE;
}


/*
 *------------------------------------------------------
 *                tml_SndStream_Close
 *------------------------------------------------------
 */
static PyObject * _tml_SndStream_Close(PyObject *self, PyObject *args)
{
    TML_CORE_HANDLE  shandle;
    TML_INT64        iID;

    TML_INT32   err = TML_SUCCESS;

    if (!PyArg_ParseTuple(args, "LL", &shandle, &iID))
        return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
      err = tml_SndStream_Register_GetSize(shandle, iID, NULL, NULL);
      if (err == TML_SUCCESS) err = tml_SndStream_Register_Close(shandle, iID, NULL, NULL);
      if (err == TML_SUCCESS) err = tml_SndStream_Register_GetPosition(shandle, iID, NULL, NULL);
      if (err == TML_SUCCESS) err = tml_SndStream_Register_OnError(shandle, iID, NULL, NULL);
      if (err == TML_SUCCESS) err = tml_SndStream_Register_Read(shandle, iID, NULL, NULL);
      if (err == TML_SUCCESS) err = tml_SndStream_Register_Write(shandle, iID, NULL, NULL);
      if (err == TML_SUCCESS) err = tml_SndStream_Register_Seek(shandle, iID, NULL, NULL);
      // Now close the stream:
      if (err == TML_SUCCESS) err =  tml_SndStream_Close(shandle, iID);
    }
    catch (...) {
      err = TML_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;

    ///////////////////////////////////////////////////////////////
    // Remove the entry out of the dictionary:
    // All registered callbacks must be deregistered (refcounting):
    removeSndStrmCallbackDataFromDict(m_senderStrmDict, iID);
    
    if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, " tml_SndStream_Close failed (%d)",err);
            return NULL;
    }
    Py_RETURN_NONE;
}


/*
 *------------------------------------------------------
 *               tml_SndStream_Open
 *------------------------------------------------------
 */
static PyObject * _tml_SndStream_Open(PyObject *self, PyObject *args)
{
    TML_CORE_HANDLE shandle;
    TML_INT64       iID;
    PyObject*       py_Profile16;
    TML_TCHAR*      sProfile;
    PyObject*       py_Profile;
    PyObject*       py_Host16;
    TML_TCHAR*      sHost;
    PyObject*       py_Host;
    PyObject*       py_Port16;
    TML_TCHAR*      sPort;
    PyObject*       py_Port;

    TML_INT32   err = TML_SUCCESS;

    if (!PyArg_ParseTuple(args, "LOOO", &shandle, &py_Profile, &py_Host, &py_Port))
        return NULL;
    if ( _unicodeConversion(py_Profile, &sProfile, &py_Profile16, tmlError) ) return NULL;
    if ( _unicodeConversion(py_Host, &sHost, &py_Host16, tmlError) ) return NULL;
    if ( _unicodeConversion(py_Port, &sPort, &py_Port16, tmlError) ) return NULL;

    Py_BEGIN_ALLOW_THREADS;
    try {
        err = tml_SndStream_Open(shandle, &iID, sProfile, sHost, sPort);
    }
    catch (...) {
        err = TML_ERR_COMMON;
    }
    Py_END_ALLOW_THREADS;
    
    unicodeFree(py_Profile16, sProfile);
    unicodeFree(py_Host16, sHost);
    unicodeFree(py_Port16, sPort);

    if (err != TML_SUCCESS) {
            PyErr_Format(tmlError, "tml_RecStream_Open tml_SndStream_Open (%d)",err);
            return NULL;
    }

    return Py_BuildValue("L",iID);
}


/*
 *------------------------------------------------------
 *               tml_SndStream_Register_Close
 *------------------------------------------------------
 */
static PyObject * _tml_SndStream_Register_Close(PyObject *self, PyObject *args)
{
    TML_CORE_HANDLE  shandle;
    TML_INT64        iID;
    PyObject*        pCBFunc;
    PyObject*        pCBData;

    TML_INT32   err = TML_SUCCESS;

    if (PyArg_ParseTuple(args, "LLOO", &shandle, &iID, &pCBFunc, &pCBData)){
        if (!PyCallable_Check(pCBFunc) && pCBFunc != Py_None) {
            PyErr_SetString(PyExc_TypeError, "parameter must be a callable function");
            return NULL;
        }
        if (!PyFunction_Check(pCBFunc) && pCBFunc != Py_None) {
            PyErr_SetString(PyExc_TypeError, "parameter must be a function");
            return NULL;
        }
        PythonSenderStrmCallbackData* callbackData = NULL;
        err = getSndStrmCallbackDataFromDict(iID, 
              true, pCBFunc, pCBData,
              false, NULL, NULL,
              false, NULL, NULL,
              false, NULL, NULL,
              false, NULL, NULL,
              false, NULL, NULL,
              false, NULL, NULL,
              &callbackData, m_senderStrmDict);
        if (err == TML_SUCCESS) {
          Py_BEGIN_ALLOW_THREADS;
          try {
            err = tml_SndStream_Register_Close(shandle, iID, onSndStrmRegisterCloseCallback, callbackData);
          }
          catch (...) {
              err = TML_ERR_COMMON;
          }
          Py_END_ALLOW_THREADS;
        }
    
        if (err != TML_SUCCESS) {
                PyErr_Format(tmlError, "tml_SndStream_Register_Close failed (%d)",err);
                return NULL;
        }
    }
    Py_RETURN_NONE;
}


/*
 *------------------------------------------------------
 *               tml_SndStream_Register_GetPosition
 *------------------------------------------------------
 */
static PyObject * _tml_SndStream_Register_GetPosition(PyObject *self, PyObject *args)
{
    TML_CORE_HANDLE  shandle;
    TML_INT64        iID;
    PyObject*        pCBFunc;
    PyObject*        pCBData;

    TML_INT32   err = TML_SUCCESS;

    if (PyArg_ParseTuple(args, "LLOO", &shandle, &iID, &pCBFunc, &pCBData)){
        if (!PyCallable_Check(pCBFunc) && pCBFunc != Py_None) {
            PyErr_SetString(PyExc_TypeError, "parameter must be a callable function");
            return NULL;
        }
        if (!PyFunction_Check(pCBFunc) && pCBFunc != Py_None) {
            PyErr_SetString(PyExc_TypeError, "parameter must be a function");
            return NULL;
        }
        PythonSenderStrmCallbackData* callbackData = NULL;
        err = getSndStrmCallbackDataFromDict(iID, 
              false, NULL, NULL,
              true, pCBFunc, pCBData,
              false, NULL, NULL,
              false, NULL, NULL,
              false, NULL, NULL,
              false, NULL, NULL,
              false, NULL, NULL,
              &callbackData, m_senderStrmDict);
        if (err == TML_SUCCESS) {
          Py_BEGIN_ALLOW_THREADS;
          try {
            err = tml_SndStream_Register_GetPosition(shandle, iID, onSndStrmRegisterGetPositionCallback, callbackData);
          }
          catch (...) {
              err = TML_ERR_COMMON;
          }
          Py_END_ALLOW_THREADS;
        }
        if (err != TML_SUCCESS) {
                PyErr_Format(tmlError, "tml_SndStream_Register_GetPosition failed (%d)",err);
                return NULL;
        }
    }
    Py_RETURN_NONE;
}


/*
 *------------------------------------------------------
 *               tml_SndStream_Register_GetSize
 *------------------------------------------------------
 */
static PyObject * _tml_SndStream_Register_GetSize(PyObject *self, PyObject *args)
{
    TML_CORE_HANDLE  shandle;
    TML_INT64        iID;
    PyObject*        pCBFunc;
    PyObject*        pCBData;

    TML_INT32   err = TML_SUCCESS;

    if (PyArg_ParseTuple(args, "LLOO", &shandle, &iID, &pCBFunc, &pCBData)){
        if (!PyCallable_Check(pCBFunc) && pCBFunc != Py_None) {
            PyErr_SetString(PyExc_TypeError, "parameter must be a callable function");
            return NULL;
        }
        if (!PyFunction_Check(pCBFunc) && pCBFunc != Py_None) {
            PyErr_SetString(PyExc_TypeError, "parameter must be a function");
            return NULL;
        }
        PythonSenderStrmCallbackData* callbackData = NULL;
        err = getSndStrmCallbackDataFromDict(iID, 
              false, NULL, NULL,
              false, NULL, NULL,
              true, pCBFunc, pCBData,
              false, NULL, NULL,
              false, NULL, NULL,
              false, NULL, NULL,
              false, NULL, NULL,
              &callbackData, m_senderStrmDict);
        if (err == TML_SUCCESS) {
          Py_BEGIN_ALLOW_THREADS;
          try {
            err = tml_SndStream_Register_GetSize(shandle, iID, onSndStrmRegisterGetSizeCallback, callbackData);
          }
          catch (...) {
              err = TML_ERR_COMMON;
          }
          Py_END_ALLOW_THREADS;
        }
        if (err != TML_SUCCESS) {
                PyErr_Format(tmlError, "tml_SndStream_Register_GetSize failed (%d)",err);
                return NULL;
        }
    }
    Py_RETURN_NONE;
}


/*
 *------------------------------------------------------
 *               tml_SndStream_Register_OnError
 *------------------------------------------------------
 */
static PyObject * _tml_SndStream_Register_OnError(PyObject *self, PyObject *args)
{
    TML_CORE_HANDLE  shandle;
    TML_INT64        iID;
    PyObject*        pCBFunc;
    PyObject*        pCBData;

    TML_INT32   err = TML_SUCCESS;

    if (PyArg_ParseTuple(args, "LLOO", &shandle, &iID, &pCBFunc, &pCBData)){
        if (!PyCallable_Check(pCBFunc) && pCBFunc != Py_None) {
            PyErr_SetString(PyExc_TypeError, "parameter must be a callable function");
            return NULL;
        }
        if (!PyFunction_Check(pCBFunc) && pCBFunc != Py_None) {
            PyErr_SetString(PyExc_TypeError, "parameter must be a function");
            return NULL;
        }
        PythonSenderStrmCallbackData* callbackData = NULL;
        err = getSndStrmCallbackDataFromDict(iID, 
              false, NULL, NULL,
              false, NULL, NULL,
              false, NULL, NULL,
              true, pCBFunc, pCBData,
              false, NULL, NULL,
              false, NULL, NULL,
              false, NULL, NULL,
              &callbackData, m_senderStrmDict);
        if (err == TML_SUCCESS) {
          Py_BEGIN_ALLOW_THREADS;
          try {
            err = tml_SndStream_Register_OnError(shandle, iID, onSndStrmRegisterOnErrorCallback, callbackData);
          }
          catch (...) {
              err = TML_ERR_COMMON;
          }
          Py_END_ALLOW_THREADS;
        }
        if (err != TML_SUCCESS) {
                PyErr_Format(tmlError, "tml_SndStream_Register_OnError failed (%d)",err);
                return NULL;
        }
    }
    Py_RETURN_NONE;
}


/*
 *------------------------------------------------------
 *               tml_SndStream_Register_Read
 *------------------------------------------------------
 */
static PyObject * _tml_SndStream_Register_Read(PyObject *self, PyObject *args)
{
    TML_CORE_HANDLE  shandle;
    TML_INT64        iID;
    PyObject*        pCBFunc;
    PyObject*        pCBData;

    TML_INT32   err = TML_SUCCESS;

    if (PyArg_ParseTuple(args, "LLOO", &shandle, &iID, &pCBFunc, &pCBData)){
        if (!PyCallable_Check(pCBFunc) && pCBFunc != Py_None) {
            PyErr_SetString(PyExc_TypeError, "parameter must be a callable function");
            return NULL;
        }
        if (!PyFunction_Check(pCBFunc) && pCBFunc != Py_None) {
            PyErr_SetString(PyExc_TypeError, "parameter must be a function");
            return NULL;
        }
        PythonSenderStrmCallbackData* callbackData = NULL;
        err = getSndStrmCallbackDataFromDict(iID, 
              false, NULL, NULL,
              false, NULL, NULL,
              false, NULL, NULL,
              false, NULL, NULL,
              true, pCBFunc, pCBData,
              false, NULL, NULL,
              false, NULL, NULL,
              &callbackData, m_senderStrmDict);
        if (err == TML_SUCCESS) {
          Py_BEGIN_ALLOW_THREADS;
          try {
            err = tml_SndStream_Register_Read(shandle, iID, onSndStrmRegisterReadCallback, callbackData);
          }
          catch (...) {
              err = TML_ERR_COMMON;
          }
          Py_END_ALLOW_THREADS;
        }
        if (err != TML_SUCCESS) {
                PyErr_Format(tmlError, "tml_SndStream_Register_Read failed (%d)",err);
                return NULL;
        }
    }
    Py_RETURN_NONE;
}


/*
 *------------------------------------------------------
 *               tml_SndStream_Register_Seek
 *------------------------------------------------------
 */
static PyObject * _tml_SndStream_Register_Seek(PyObject *self, PyObject *args)
{
    TML_CORE_HANDLE  shandle;
    TML_INT64        iID;
    PyObject*        pCBFunc;
    PyObject*        pCBData;

    TML_INT32   err = TML_SUCCESS;

    if (PyArg_ParseTuple(args, "LLOO", &shandle, &iID, &pCBFunc, &pCBData)){
        if (!PyCallable_Check(pCBFunc) && pCBFunc != Py_None) {
            PyErr_SetString(PyExc_TypeError, "parameter must be a callable function");
            return NULL;
        }
        if (!PyFunction_Check(pCBFunc) && pCBFunc != Py_None) {
            PyErr_SetString(PyExc_TypeError, "parameter must be a function");
            return NULL;
        }
        PythonSenderStrmCallbackData* callbackData = NULL;
        err = getSndStrmCallbackDataFromDict(iID, 
              false, NULL, NULL,
              false, NULL, NULL,
              false, NULL, NULL,
              false, NULL, NULL,
              false, NULL, NULL,
              true, pCBFunc, pCBData,
              false, NULL, NULL,
              &callbackData, m_senderStrmDict);
        if (err == TML_SUCCESS) {
          Py_BEGIN_ALLOW_THREADS;
          try {
            err = tml_SndStream_Register_Seek(shandle, iID, onSndStrmRegisterSeekCallback, callbackData);
          }
          catch (...) {
              err = TML_ERR_COMMON;
          }
          Py_END_ALLOW_THREADS;
        }
        if (err != TML_SUCCESS) {
                PyErr_Format(tmlError, "tml_SndStream_Register_Seek failed (%d)",err);
                return NULL;
        }
    }
    Py_RETURN_NONE;
}


/*
 *------------------------------------------------------
 *               tml_SndStream_Register_Write
 *------------------------------------------------------
 */
static PyObject * _tml_SndStream_Register_Write(PyObject *self, PyObject *args)
{
    TML_CORE_HANDLE  shandle;
    TML_INT64        iID;
    PyObject*        pCBFunc;
    PyObject*        pCBData;

    TML_INT32   err = TML_SUCCESS;

    if (PyArg_ParseTuple(args, "LLOO", &shandle, &iID, &pCBFunc, &pCBData)){
        if (!PyCallable_Check(pCBFunc) && pCBFunc != Py_None) {
            PyErr_SetString(PyExc_TypeError, "parameter must be callable function");
            return NULL;
        }
        if (!PyFunction_Check(pCBFunc) && pCBFunc != Py_None) {
            PyErr_SetString(PyExc_TypeError, "parameter must be a function");
            return NULL;
        }
        PythonSenderStrmCallbackData* callbackData = NULL;
        err = getSndStrmCallbackDataFromDict(iID, 
              false, NULL, NULL,
              false, NULL, NULL,
              false, NULL, NULL,
              false, NULL, NULL,
              false, NULL, NULL,
              false, NULL, NULL,
              true, pCBFunc, pCBData,
              &callbackData, m_senderStrmDict);
        if (err == TML_SUCCESS) {
          Py_BEGIN_ALLOW_THREADS;
          try {
            err = tml_SndStream_Register_Write(shandle, iID, onSndStrmRegisterWriteCallback, callbackData);
          }
          catch (...) {
              err = TML_ERR_COMMON;
          }
          Py_END_ALLOW_THREADS;
        }
        if (err != TML_SUCCESS) {
                PyErr_Format(tmlError, "tml_SndStream_Register_Write failed (%d)",err);
                return NULL;
        }
    }
    Py_RETURN_NONE;
}


/*
 *************************************************************************************
 *                                  function defintion table
 *************************************************************************************
 */
static PyMethodDef _tml_methods[] = {
    // General functions:
    {"tml_Core_Get_Copyright",  _tml_Core_Get_Copyright, METH_VARARGS,"tml_Core_Get_Copyright() -> <value>"},
    {"tml_Core_Get_LoggingValue",  _tml_Core_Get_LoggingValue, METH_VARARGS,"tml_Core_Get_LoggingValue(<tmlHandle>) -> <iLogValue>"},
    {"tml_Core_Get_Version",  (PyCFunction)_tml_Core_Get_Version, METH_NOARGS,"tml_Core_Get_Version() -> (<api version>,<lib version>,<lib version string>)"},
    {"tml_Core_Set_LoggingValue",  _tml_Core_Set_LoggingValue, METH_VARARGS,"tml_Core_Set_LoggingValue(<tmlHandle>, <iLogValue>)"},
    {"tml_Core_Set_Password",  _tml_Core_Set_Password, METH_VARARGS,"tml_Core_Set_Password(<name>,<password>)"},


    // TMLCore:
    {"tml_Core_Open",  _tml_Core_Open, METH_VARARGS,"tml_Core_Open(<iLogValue>) -> tml handle"},
    {"tml_Core_GeneralDeregistration",  _tml_Core_GeneralDeregistration, METH_VARARGS,"tml_Core_GeneralDeregistration(<tmlHandle>)"},
    {"tml_Core_Close",  _tml_Core_Close, METH_VARARGS,"tml_Core_Close(<tmlHandle>)"},


    // Listener management
    {"tml_Core_Get_ListenerEnabled",  _tml_Core_Get_ListenerEnabled, METH_VARARGS,"tml_Core_Get_ListenerEnabled(<tmlHandle>) -> <bEnable>"},
    {"tml_Core_Get_ListenerIP",  _tml_Core_Get_ListenerIP, METH_VARARGS,"tml_Core_Get_ListenerIP(<tmlHandle>) -> <sIP>"},
    {"tml_Core_Get_ListenerPort",  _tml_Core_Get_ListenerPort, METH_VARARGS,"tml_Core_Get_ListenerPort(<tmlHandle>) -> <sPort>"},
    {"tml_Core_Set_ListenerEnabled",  _tml_Core_Set_ListenerEnabled, METH_VARARGS,"tml_Core_Set_ListenerEnabled(<tmlHandle>, <bEnable>)"},
    {"tml_Core_Set_ListenerIP",  _tml_Core_Set_ListenerIP, METH_VARARGS,"tml_Core_Set_ListenerIP(<tmlHandle>, <sIP>)"},
    {"tml_Core_Set_ListenerPort",  _tml_Core_Set_ListenerPort, METH_VARARGS,"tml_Core_Set_ListenerPort(<tmlHandle>, <sPort>)"},


    // Profile management
    {"tml_Profile_Get_Registered",  _tml_Profile_Get_Registered, METH_VARARGS,"tml_Profile_Get_Registered(<tmlHandle>) -> <sProfiles>"},
    {"tml_Profile_Get_Registered_Count",  _tml_Profile_Get_Registered_Count, METH_VARARGS,"tml_Profile_Get_Registered_Count(<tmlHandle>) -> <iCount>"},
    {"tml_Profile_Get_RegisterState",  _tml_Profile_Get_RegisterState, METH_VARARGS,"tml_Profile_Get_RegisterState(<tmlHandle>, <sProfile>) -> <bRegisterred>"},
    {"tml_Profile_Register",  _tml_Profile_Register, METH_VARARGS,"tml_Profile_Register(<tmlHandle>, <sProfile>)"},
    {"tml_Profile_Register_Cmd",  _tml_Profile_Register_Cmd, METH_VARARGS,"tml_Profile_Register_Cmd(<tmlHandle>, <sProfile>, <cmdIdentifier>, <pCBFunc>, <pCBData>)"},
    {"tml_Profile_Set_OnCustomDispatch",  _tml_Profile_Set_OnCustomDispatch, METH_VARARGS,"tml_Profile_Set_OnCustomDispatch(<tmlHandle>, <sProfile>, <pCBFunc>, <pCBData>)"},
    {"tml_Profile_Set_OnDeleteCmd",  _tml_Profile_Set_OnDeleteCmd, METH_VARARGS,"tml_Profile_Set_OnDeleteCmd(<tmlHandle>, <sProfile>, <pCBFunc>, <pCBData>)"},
    {"tml_Profile_Unregister",  _tml_Profile_Unregister, METH_VARARGS,"tml_Profile_Unregister(<tmlHandle>, <sProfile>)"},


    // Sending commands
    {"tml_Send_AsyncMessage",  _tml_Send_AsyncMessage, METH_VARARGS,"tml_Send_AsyncMessage(<tmlHandle>, <cmdHandle>, <sProfile>, <sHost>, <sPort>, <iTimeout>)"},
    {"tml_Send_AsyncProgressReply",  _tml_Send_AsyncProgressReply, METH_VARARGS,"tml_Send_AsyncProgressReply(<cmdHandle>, <iProgress>)"},
    {"tml_Send_AsyncStatusReply",  _tml_Send_AsyncStatusReply, METH_VARARGS,"tml_Send_AsyncStatusReply(<cmdHandle>, <iType>, <sMessage>)"},
    {"tml_Send_SyncMessage",  _tml_Send_SyncMessage, METH_VARARGS,"tml_Send_SyncMessage(<tmlHandle>, <cmdHandle>, <sProfile>, <sHost>, <sPort>, <iTimeout>)"},

    // Event handling
    {"tml_Evt_Get_MaxConnectionFailCount",  _tml_Evt_Get_MaxConnectionFailCount, METH_VARARGS,"tml_Evt_Get_MaxConnectionFailCount(<tmlHandle>) -> <iCount>"},
    {"tml_Evt_Get_MaxQueuedEventMessages",  _tml_Evt_Get_MaxQueuedEventMessages, METH_VARARGS,"tml_Evt_Get_MaxQueuedEventMessages(<tmlHandle>) -> <iCount>"},
    {"tml_Evt_Get_Subscribed_MessageDestinations",  _tml_Evt_Get_Subscribed_MessageDestinations, METH_VARARGS,"tml_Evt_Get_Subscribed_MessageDestinations(<tmlHandle>, <sProfile>) -> <sSubscriptions>"},
    {"tml_Evt_Send_Message",  _tml_Evt_Send_Message, METH_VARARGS,"tml_Evt_Send_Message(<tmlHandle>, <cmdHandle>, <sProfile>)"},
    {"tml_Evt_Send_SubscriptionRequest",  _tml_Evt_Send_SubscriptionRequest, METH_VARARGS,"tml_Evt_Send_SubscriptionRequest(<tmlHandle>, <sProfile>, <sSourceHost>, <sSourcePort>, <sDestHost>, <sDestPort>, <iTimeout>)"},
    {"tml_Evt_Send_UnsubscriptionRequest",  _tml_Evt_Send_UnsubscriptionRequest, METH_VARARGS,"tml_Evt_Send_UnsubscriptionRequest(<tmlHandle>, <sProfile>, <sSourceHost>, <sSourcePort>, <sDestHost>, <sDestPort>, <iTimeout>)"},
    {"tml_Evt_Set_MaxConnectionFailCount",  _tml_Evt_Set_MaxConnectionFailCount, METH_VARARGS,"tml_Evt_Set_MaxConnectionFailCount(<tmlHandle>, <iCount>)"},
    {"tml_Evt_Set_MaxQueuedEventMessages",  _tml_Evt_Set_MaxQueuedEventMessages, METH_VARARGS,"tml_Evt_Set_MaxQueuedEventMessages(<tmlHandle>, <iCount>)"},
    {"tml_Evt_Set_OnError",  _tml_Evt_Set_OnError, METH_VARARGS,"tml_Evt_Set_OnError(<tmlHandle>, <sProfile>, <pCBFunc>, <pCBData>)"},
    {"tml_Evt_Set_OnPeerRegister",  _tml_Evt_Set_OnPeerRegister, METH_VARARGS,"tml_Evt_Set_OnPeerRegister(<tmlHandle>, <sProfile>, <pCBFunc>, <pCBData>)"},
    {"tml_Evt_Set_OnPopulate",  _tml_Evt_Set_OnPopulate, METH_VARARGS,"tml_Evt_Set_OnPopulate(<tmlHandle>, <sProfile>, <pCBFunc>, <pCBData>)"},
    {"tml_Evt_Set_OnQueueOverflow",  _tml_Evt_Set_OnQueueOverflow, METH_VARARGS,"tml_Evt_Set_OnQueueOverflow(<tmlHandle>, <sProfile>, <pCBFunc>, <pCBData>)"},
    {"tml_Evt_Subscribe_MessageDestination",  _tml_Evt_Subscribe_MessageDestination, METH_VARARGS,"tml_Evt_Subscribe_MessageDestination(<tmlHandle>, <sProfile>, <sHost>, <sPort>)"},
    {"tml_Evt_Unsubscribe_All_MessageDestinations",  _tml_Evt_Unsubscribe_All_MessageDestinations, METH_VARARGS,"tml_Evt_Unsubscribe_All_MessageDestinations(<tmlHandle>, <sProfile>)"},
    {"tml_Evt_Unsubscribe_MessageDestination",  _tml_Evt_Unsubscribe_MessageDestination, METH_VARARGS,"tml_Evt_Unsubscribe_MessageDestination(<tmlHandle>, <sProfile>, <sHost>, <sPort>)"},


    // Load balancing
    {"tml_Bal_Get_MaxConnectionFailCount",  _tml_Bal_Get_MaxConnectionFailCount, METH_VARARGS,"tml_Bal_Get_MaxConnectionFailCount(<tmlHandle>) -> <iCount>"},
    {"tml_Bal_Get_Subscribed_MessageDestinations",  _tml_Bal_Get_Subscribed_MessageDestinations, METH_VARARGS,"tml_Bal_Get_Subscribed_MessageDestinations(<tmlHandle>, <sProfile>) -> <sSubscriptions>"},
    {"tml_Bal_Send_AsyncMessage",  _tml_Bal_Send_AsyncMessage, METH_VARARGS,"tml_Bal_Send_AsyncMessage(<tmlHandle>, <cmdHandle>, <sProfile>, <iTimeout>)"},
    {"tml_Bal_Send_SubscriptionRequest",  _tml_Bal_Send_SubscriptionRequest, METH_VARARGS,"tml_Bal_Send_SubscriptionRequest(<tmlHandle>, <sProfile>, <sSourceHost>, <sSourcePort>, <sDestHost>, <sDestPort>, <iTimeout>)"},
    {"tml_Bal_Send_SyncMessage",  _tml_Bal_Send_SyncMessage, METH_VARARGS,"tml_Bal_Send_SyncMessage(<tmlHandle>, <cmdHandle>, <sProfile>, <iTimeout>)"},
    {"tml_Bal_Send_UnsubscriptionRequest",  _tml_Bal_Send_UnsubscriptionRequest, METH_VARARGS,"tml_Bal_Send_UnsubscriptionRequest(<tmlHandle>, <sProfile>, <sSourceHost>, <sSourcePort>, <sDestHost>, <sDestPort>, <iTimeout>)"},
    {"tml_Bal_Set_MaxConnectionFailCount",  _tml_Bal_Set_MaxConnectionFailCount, METH_VARARGS,"tml_Bal_Set_MaxConnectionFailCount(<tmlHandle>, <iCount>)"},
    {"tml_Bal_Set_OnBusyStatusRequest",  _tml_Bal_Set_OnBusyStatusRequest, METH_VARARGS,"tml_Bal_Set_OnBusyStatusRequest(<tmlHandle>, <sProfile>, <pCBFunc>, <pCBData>)"},
    {"tml_Bal_Set_OnCalculation",  _tml_Bal_Set_OnCalculation, METH_VARARGS,"tml_Bal_Set_OnCalculation(<tmlHandle>, <sProfile>, <pCBFunc>, <pCBData>)"},
    {"tml_Bal_Set_OnPeerRegister",  _tml_Bal_Set_OnPeerRegister, METH_VARARGS,"tml_Bal_Set_OnPeerRegister(<tmlHandle>, <sProfile>, <pCBFunc>, <pCBData>)"},
    {"tml_Bal_Set_OnPopulate",  _tml_Bal_Set_OnPopulate, METH_VARARGS,"tml_Bal_Set_OnPopulate(<tmlHandle>, <sProfile>, <pCBFunc>, <pCBData>)"},
    {"tml_Bal_Subscribe_MessageDestination",  _tml_Bal_Subscribe_MessageDestination, METH_VARARGS,"tml_Bal_Subscribe_MessageDestination(<tmlHandle>, <sProfile>, <sHost>, <sPort>)"},
    {"tml_Bal_Unsubscribe_All_MessageDestinations",  _tml_Bal_Unsubscribe_All_MessageDestinations, METH_VARARGS,"tml_Bal_Unsubscribe_All_MessageDestinations(<tmlHandle>, <sProfile>)"},
    {"tml_Bal_Unsubscribe_MessageDestination",  _tml_Bal_Unsubscribe_MessageDestination, METH_VARARGS,"tml_Bal_Unsubscribe_MessageDestination(<tmlHandle>, <sProfile>, <sHost>, <sPort>)"},


    //Stream communication
    {"tml_RecStream_Close",  _tml_RecStream_Close, METH_VARARGS,"tml_RecStream_Close(<tmlHandle>, <iID>, <bRetainOpen>)"},
    {"tml_RecStream_DownloadData",  _tml_RecStream_DownloadData, METH_VARARGS,"tml_RecStream_DownloadData(<tmlHandle>, <iID>, <buffersize>, <pCBFuncDld>, <pCBDataDld>, <pCBFuncDldFinish>, <pCBDataDldFinish>)"},
    {"tml_RecStream_GetPosition",  _tml_RecStream_GetPosition, METH_VARARGS,"tml_RecStream_GetPosition(<tmlHandle>, <iID>) -> <iPosition>"},
    {"tml_RecStream_GetSize",  _tml_RecStream_GetSize, METH_VARARGS,"tml_RecStream_GetSize(<tmlHandle>, <iID>) -> <iSize>"},
    {"tml_RecStream_Open",  _tml_RecStream_Open, METH_VARARGS,"tml_RecStream_Open(<tmlHandle>, <iID>, <sProfile>, <sHost>, <sPort>)"},
    {"tml_RecStream_Read",  _tml_RecStream_Read, METH_VARARGS,"tml_RecStream_Read(<tmlHandle>, <iID>, <count>) -> <readBuffer>, <bytesRead>"},
    {"tml_RecStream_ReadBuffer",  _tml_RecStream_ReadBuffer, METH_VARARGS,"tml_RecStream_ReadBuffer(<tmlHandle>, <iID>, <count>) -> <readBuffer>"},
    {"tml_RecStream_Seek",  _tml_RecStream_Seek, METH_VARARGS,"tml_RecStream_Seek(<tmlHandle>, <iID>, <seekPos>, <origin>)"},
    {"tml_RecStream_Write",  _tml_RecStream_Write, METH_VARARGS,"tml_RecStream_Write(<tmlHandle>, <iID>, <buffer>, <count>)"},
    {"tml_SndStream_Close",  _tml_SndStream_Close, METH_VARARGS,"tml_SndStream_Close(<tmlHandle>, <iID>)"},
    {"tml_SndStream_Open",  _tml_SndStream_Open, METH_VARARGS,"tml_SndStream_Open(<tmlHandle>, <sProfile>, <sHost>, <sPort>) -> iID"},
    {"tml_SndStream_Register_Close",  _tml_SndStream_Register_Close, METH_VARARGS,"tml_SndStream_Register_Close(<tmlHandle>, <iID>, <pCBFunc>, <pCBData>)"},
    {"tml_SndStream_Register_GetPosition",  _tml_SndStream_Register_GetPosition, METH_VARARGS,"tml_SndStream_Register_GetPosition(<tmlHandle>, <iID>, <pCBFunc>, <pCBData>)"},
    {"tml_SndStream_Register_GetSize",  _tml_SndStream_Register_GetSize, METH_VARARGS,"tml_SndStream_Register_GetSize(<tmlHandle>, <iID>, <pCBFunc>, <pCBData>)"},
    {"tml_SndStream_Register_OnError",  _tml_SndStream_Register_OnError, METH_VARARGS,"tml_SndStream_Register_OnError(<tmlHandle>, <iID>, <pCBFunc>, <pCBData>)"},
    {"tml_SndStream_Register_Read",  _tml_SndStream_Register_Read, METH_VARARGS,"tml_SndStream_Register_Read(<tmlHandle>, <iID>, <pCBFunc>, <pCBData>)"},
    {"tml_SndStream_Register_Seek",  _tml_SndStream_Register_Seek, METH_VARARGS,"tml_SndStream_Register_Seek(<tmlHandle>, <iID>, <pCBFunc>, <pCBData>)"},
    {"tml_SndStream_Register_Write",  _tml_SndStream_Register_Write, METH_VARARGS,"tml_SndStream_Register_Write(<tmlHandle>, <iID>, <pCBFunc>, <pCBData>)"},


    // TML commands
    {"tml_Cmd_Acquire_Sidex_Handle",  _tml_Cmd_Acquire_Sidex_Handle, METH_VARARGS,"tml_Cmd_Acquire_Sidex_Handle(<cmdHandle>) -> sidex handle"},
    {"tml_Cmd_Create",  (PyCFunction)_tml_Cmd_Create, METH_NOARGS,"tml_Cmd_Create() -> cmd handle"},
    {"tml_Cmd_Free",  _tml_Cmd_Free, METH_VARARGS,"tml_Cmd_Free(<cmdHandle>)"},
    {"tml_Cmd_Release_Sidex_Handle",  _tml_Cmd_Release_Sidex_Handle, METH_VARARGS,"tml_Cmd_Release_Sidex_Handle(<cmdHandle>)"},


    // Command message callbacks
    {"tml_Cmd_Register_CommandReady",  _tml_Cmd_Register_CommandReady, METH_VARARGS,"tml_Cmd_Register_CommandReady(<cmdHandle>, <pCBFunc>, <pCBData>)"},
    {"tml_Cmd_Register_Progress",  _tml_Cmd_Register_Progress, METH_VARARGS,"tml_Cmd_Register_Progress(<cmdHandle>, <pCBFunc>, <pCBData>)"},
    {"tml_Cmd_Register_StatusReply",  _tml_Cmd_Register_StatusReply, METH_VARARGS,"tml_Cmd_Register_StatusReply(<cmdHandle>, <pCBFunc>, <pCBData>)"},
    {"tml_Cmd_Registered_CommandReady",  _tml_Cmd_Registered_CommandReady, METH_VARARGS,"tml_Cmd_Registered_CommandReady(<cmdHandle>) -> (<pCBFunc>, <pCBData>)"},
    {"tml_Cmd_Registered_Progress",  _tml_Cmd_Registered_Progress, METH_VARARGS,"tml_Cmd_Registered_Progress(<cmdHandle>) -> (<pCBFunc>, <pCBData>)"},
    {"tml_Cmd_Registered_StatusReply",  _tml_Cmd_Registered_StatusReply, METH_VARARGS,"tml_Cmd_Registered_StatusReply(<cmdHandle>) -> (<pCBFunc>, <pCBData>)"},


    // Accessing header information
    {"tml_Cmd_Header_GetCommand",  _tml_Cmd_Header_GetCommand , METH_VARARGS,"tml_Cmd_Header_GetCommand (<cmdHandle>) -> command identifier"},
    {"tml_Cmd_Header_GetCreationTime",  _tml_Cmd_Header_GetCreationTime , METH_VARARGS,"tml_Cmd_Header_GetCreationTime (<cmdHandle>) -> time"},
    {"tml_Cmd_Header_GetError",  _tml_Cmd_Header_GetError , METH_VARARGS,"tml_Cmd_Header_GetError (<cmdHandle>) -> error code"},
    {"tml_Cmd_Header_GetErrorMessage",  _tml_Cmd_Header_GetErrorMessage , METH_VARARGS,"tml_Cmd_Header_GetErrorMessage (<cmdHandle>) -> sErrorMessage"},
    {"tml_Cmd_Header_GetMode",  _tml_Cmd_Header_GetMode , METH_VARARGS,"tml_Cmd_Header_GetMode (<cmdHandle>) -> mode"},
    {"tml_Cmd_Header_GetProgress",  _tml_Cmd_Header_GetProgress , METH_VARARGS,"tml_Cmd_Header_GetProgress (<cmdHandle>) -> progress"},
    {"tml_Cmd_Header_GetReplyMessage",  _tml_Cmd_Header_GetReplyMessage , METH_VARARGS,"tml_Cmd_Header_GetReplyMessage (<cmdHandle>) -> sReplyMessage"},
    {"tml_Cmd_Header_GetReplyType",  _tml_Cmd_Header_GetReplyType , METH_VARARGS,"tml_Cmd_Header_GetReplyType (<cmdHandle>) -> type"},
    {"tml_Cmd_Header_GetState",  _tml_Cmd_Header_GetState , METH_VARARGS,"tml_Cmd_Header_GetState (<cmdHandle>) -> state"},
    {"tml_Cmd_Header_SetCommand",  _tml_Cmd_Header_SetCommand , METH_VARARGS,"tml_Cmd_Header_SetCommand (<cmdHandle>, <cmdIdentifier>)"},
    {"tml_Cmd_Header_SetError",  _tml_Cmd_Header_SetError , METH_VARARGS,"tml_Cmd_Header_SetError (<cmdHandle>, <iErrorCode>)"},
    {"tml_Cmd_Header_SetErrorMessage",  _tml_Cmd_Header_SetErrorMessage , METH_VARARGS,"tml_Cmd_Header_SetErrorMessage (<cmdHandle>, <sErrorMessage>)"},
    {"tml_Cmd_Header_SetMode",  _tml_Cmd_Header_SetMode , METH_VARARGS,"tml_Cmd_Header_SetMode (<cmdHandle>, <mode>)"},
    {"tml_Cmd_Header_SetProgress",  _tml_Cmd_Header_SetProgress , METH_VARARGS,"tml_Cmd_Header_SetProgress (<cmdHandle>, <progress>)"},
    {"tml_Cmd_Header_SetReplyMessage",  _tml_Cmd_Header_SetReplyMessage , METH_VARARGS,"tml_Cmd_Header_SetReplyMessage (<cmdHandle>, <sReplyMessage>)"},
    {"tml_Cmd_Header_SetReplyType",  _tml_Cmd_Header_SetReplyType , METH_VARARGS,"tml_Cmd_Header_SetReplyType (<cmdHandle>, <type>)"},
    {"tml_Cmd_Header_SetState",  _tml_Cmd_Header_SetState , METH_VARARGS,"tml_Cmd_Header_SetState (<cmdHandle>, <state>)"},


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

static int _tml_traverse(PyObject *m, visitproc visit, void *arg) {
    Py_VISIT(GETSTATE(m)->error);
    return 0;
}

static int _py3tml_clear(PyObject *m) {
    Py_CLEAR(GETSTATE(m)->error);
    return 0;
}

static struct PyModuleDef moduledef = {
        PyModuleDef_HEAD_INIT,
        "_tml",
        NULL,
        sizeof(struct module_state),
        _tml_methods,
        NULL,
        _tml_traverse,
        _py3tml_clear,
        NULL
};
#define INITERROR return NULL
PyMODINIT_FUNC PyInit__tml(void)

#else
#define INITERROR return
PyMODINIT_FUNC init_tml(void)
#endif/*
 *------------------------------------------------------
 *               module init function
 *------------------------------------------------------
 */
{
    PyObject *m;
    PyEval_InitThreads();

#if PY_MAJOR_VERSION >= 3
    m = PyModule_Create(&moduledef);
#else
    m = Py_InitModule("_tml", _tml_methods);
#endif
    if (m == NULL)
        INITERROR;

    // create and register exception
    tmlError = PyErr_NewException((char*)"tml.error", NULL, NULL);
    Py_INCREF(tmlError);
    PyModule_AddObject(m, "tmlError", tmlError);

    tmlIndexError = PyErr_NewException((char*)"tml.error", NULL, NULL);
    Py_INCREF(tmlIndexError);
    PyModule_AddObject(m, "tmlIndexError", tmlIndexError);

#if PY_MAJOR_VERSION >= 3
    return m;
#endif
}
