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
#  For professionel support contact us:
#
#    wobe-systems GmbH
#    support@libtml.org
#-----------------------------------------------------------

from sidex.constants import *
import tml
import sidex.document
import sidex.variant

## @defgroup TMLCmdGroup TML Command
#  @ingroup highLevelClasses
#  @brief Commands, events and data exchange.
#
#  The TMLCommand contains status information and carries the data of a command or event call.
#  All parameters are accessable through a data property. If the command is used to call a remote
#  function changes will be passed back to the caller.


## @brief TML Command Object.
#  @ingroup TMLCmdGroup
#
#  The TMLCmd object provides an interface to a TML command
#  and its data.
class TMLCmd(object):

    ## @brief TMLCmd constructor.
    #
    #  Passing a handle results in a wrapper around it.
    #  The command handle will not be freed on object destruction
    #  because it is not owned by the object.
    #  If not handle is passed the object will create a new one that
    #  is owned and freed on object destruction.
    #
    #  @param aId     : [\b int] [\em optional] Command Id
    #  @param aHandle : [\b int] [\em optional] handle of an existing command
    #
    #  @sa tml_Cmd_Create()
    def __init__(self, cmdid=None, handle=None):
        if (PY3 and handle and isinstance(handle, int)) or \
           (PY2 and handle and isinstance(handle, (long, int))):
            self.__command_handle = handle
            self.__thisown = False
        else:
            self.__command_handle = tml.tml_Cmd_Create()
            self.__thisown = True
        if cmdid:
            self.command = cmdid

        self.__sdoc = None  # sidex document
        self.__progress = 0  # last progress value
        self.__show_progress = True
        self.__tml_core = None

        return

        # ------------ properties ---------------------
        # -- the code here is for documentation only --
        self.data = None
        self.command = None
        self.error = None
        self.errormsg = None
        self.progress = None
        self.command_handle = None
        self.thisown = True
        self.creation_time = None
        self.show_progress = True
        self.mode = None
        self.reply_type = 0
        self.reply_message = ''
        self.tml_core = None
        # ------------ end properties -----------------

    ## @brief Free command handle and release data.
    #
    #  If the command handle is owned by the object it will be freed here.
    #  The data needs to be released to allow other threads to access it.
    #
    #  @sa tml_Cmd_Free
    def __del__(self):
        self.release_data()
        if self.__thisown:
            tml.tml_Cmd_Free(self.__command_handle)

    ## @property tml_core
    #  @brief [\b tml.core.TMLCore] TML core handling this command.
    #
    #  This property is None until, it was sent through a TML core.
    #
    #  @par Access:
    #    read/write

    #  @cond NODOC
    @property
    def tml_core(self):
        return self.__tml_core

    @tml_core.setter
    def tml_core(self, value):
            self.__tml_core = value
    #  @endcond

    ## @property reply_message
    #  @brief [\b int] status reply message
    #
    #  @par Access:
    #    read
    #
    #  @sa tml_Cmd_Header_GetReplyMessage()

    #  @cond NODOC
    @property
    def reply_type(self):
        return tml.tml_Cmd_Header_GetReplyMessage(self.__command_handle)
    #  @endcond

    ## @property reply_type
    #  @brief [\b int] status reply type
    #
    #  @par Access:
    #    read
    #
    #  @sa tml_Cmd_Header_GetReplyType()

    #  @cond NODOC
    @property
    def reply_type(self):
        return tml.tml_Cmd_Header_GetReplyType(self.__command_handle)
    #  @endcond

    ## @property mode
    #  @brief [\b int] command mode
    #
    #  @par Access:
    #    read
    #
    #  @sa tml_Cmd_Header_GetMode()

    #  @cond NODOC
    @property
    def mode(self):
        return tml.tml_Cmd_Header_GetMode(self.__command_handle)
    #  @endcond

    ## @property show_progress
    #  @brief [\b bool] call progress event
    #
    #  Assigning false to show_progress will block the tml.profile.TMLProfile.on_progress event.
    #
    #  @par Access:
    #    read/write
    #
    #  @sa tml.profile.TMLProfile.on_progress

    #  @cond NODOC
    @property
    def show_progress(self):
        return self.__show_progress

    @show_progress.setter
    def progress(self, value):
            self.__show_progress = value
    #  @endcond

    ## @property creation_time
    #  @brief [\b sidex.variant.SDXDateTime] command creation time
    #
    #  @par Access:
    #    read
    #
    #  @sa tml_Cmd_Header_GetCreationTime()

    #  @cond NODOC
    @property
    def creation_time(self):
        stime = tml.tml_Cmd_Header_GetCreationTime(self.__command_handle)
        return sidex.variant.SDXDateTime(value=stime)
    #  @endcond

    ## @property data
    #  @brief [\b sidex.document.SDXDocument] A SIDEX Document representing the command data
    #
    #  @par Access:
    #    read

    #  @cond NODOC
    @property
    def data(self):
        return self.aquire_data()
    #  @endcond

    ## @property command
    #  @brief [\b int] command id of the command
    #
    #  @par Access:
    #    read/write
    #
    #  @sa tml_Cmd_Header_GetCommand(), tml_Cmd_Header_SetCommand()

    #  @cond NODOC
    @property
    def command(self):
        return tml.tml_Cmd_Header_GetCommand(self.__command_handle)

    @command.setter
    def command(self, value):
        tml.tml_Cmd_Header_SetCommand(self.__command_handle, value)
    #  @endcond

    ## @property error
    #  @brief [\b int] The error number
    #
    #  @par Access:
    #    read/write
    #
    #  @sa tml_Cmd_Header_GetError(), tml_Cmd_Header_SetError()

    #  @cond NODOC
    @property
    def error(self):
        return tml.tml_Cmd_Header_GetError(self.__command_handle)

    @error.setter
    def error(self, value):
        tml.tml_Cmd_Header_SetError(self.__command_handle, value)
    #  @endcond

    ## @property errormsg
    #  @brief [\b str] The error message
    #
    #  @par Access:
    #    read/write
    #
    #  @sa tml_Cmd_Header_GetErrorMessage(), tml_Cmd_Header_SetErrorMessage()

    #  @cond NODOC
    @property
    def errormsg(self):
        return tml.tml_Cmd_Header_GetErrorMessage(self.__command_handle)

    @errormsg.setter
    def errormsg(self, value):
        tml.tml_Cmd_Header_SetErrorMessage(self.__command_handle, value)
    #  @endcond

    ## @property progress
    #  @brief [\b int] progress between 0-100
    #
    #  @par Access:
    #    read/write
    #
    #  @sa tml_Send_AsyncProgressReply()

    #  @cond NODOC
    @property
    def progress(self):
        return self.__progress

    @progress.setter
    def progress(self, value):
        if self.__progress != value:
            tml.tml_Send_AsyncProgressReply(self.__command_handle, value)
            self.__progress = value
    #  @endcond

    ## @property command_handle
    #  @brief [\b int] TML command handle
    #
    #  @par Access:
    #    read

    #  @cond NODOC
    @property
    def command_handle(self):
        return self.__command_handle
    #  @endcond

    ## @property thisown
    #  @brief [\b bool] if True the handle is freed on object destruction
    #
    #  Normally the command handle is freed on object destruction. If the TMLCmd
    #  is used to access a TML command handle from that was not created by the
    #  TMLCmd object constructor, thisown is False. Sometimes it makes sense
    #  to free the TMLCmd instance but leave the TML command handle untouched.
    #  In this case False can be assigned to the thisown property to prevent the
    #  handle from beeing freed.
    #
    #  @par Access:
    #    read/write

    #  @cond NODOC
    @property
    def thisown(self):
        return self.__thisown

    @thisown.setter
    def thisown(self, value):
        self.__thisown = value
    #  @endcond

    ##  @brief Aquire the SIDEX handle of the command data and return a SIDEX document.
    #
    #   If the data property is read, aquire_data will be autmatically called. After using
    #   the data a call to release data is required.
    #
    #   @note This a mechanism to protect the data from changes by another thread while
    #   used. If the call to release_data is missing, the system can be blocked.
    #
    #   Using data in callback functions does not require a release_data call. This is
    #   handled by the calling class. The data will also be released by the TMLCmd destructor.
    #
    #   @returns [sidex.document.SDXDocument] SIDEX document to access the data
    #
    #   @sa release_data, tml_Cmd_Acquire_Sidex_Handle()
    def aquire_data(self):
        if self.__sdoc is None:
            self.__sdoc = sidex.document.SDXDocument(tml.tml_Cmd_Acquire_Sidex_Handle(self.__command_handle))
        return self.__sdoc

    ##  @brief Release the handle of the SIDEX document representing the command data.
    #
    #   @sa aquire_data, tml_Cmd_Release_Sidex_Handle()
    def release_data(self):
        if self.__sdoc is not None:
            tml.tml_Cmd_Release_Sidex_Handle(self.__command_handle)
            self.__sdoc = None

    ## @brief Send a status reply to the caller.
    #
    #  @param rtype : [\b int] status reply type
    #  @param rmsg  : [\b rmsg] status reply message
    #
    #  @sa tml.core.on_status_reply, tml.profile.on_status_reply, tml_Send_AsyncStatusReply()
    def send_status_reply(self, rtype, rmsg):
        tml.tml_Send_AsyncStatusReply(self.__command_handle, rtype, rmsg)
