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

import sys

#  @cond NODOC
PY3 = sys.version > '3'
PY2 = sys.version < '3'
#  @endcond

# ------------------ command modes ------------------
## @defgroup TMLErrorsConst TML errors
#  @ingroup TMLSIDEXConst
#
#  TML error definitions.

## @brief Return value in case of success.
#  @ingroup TMLErrorsConst
TML_SUCCESS = 0

## @brief TML header contains returned error message.
#  @ingroup TMLErrorsConst
TML_ERR_ERROR_MSG_RECEIVED = 1

## @brief Try to start event handling thread while object destruction in progress.
#  @ingroup TMLErrorsConst
TML_ERR_EVENT_HANDLER_IN_DESTRUCTION = 2

## @brief Stream does not exist.
#  @ingroup TMLErrorsConst
TML_ERR_STREAM_DOES_NOT_EXIST = 11

## @brief Unable to acquire/release critical section.
#  @ingroup TMLErrorsConst
TML_ERR_CRITICAL_SECTION = 12

## @brief Unspecific or custom error.
#  @ingroup TMLErrorsConst
TML_ERR_COMMON = 13

## @brief Connection failed.
#  @ingroup TMLErrorsConst
TML_ERR_CONNECT = 14

## @brief A required object is missing.
#  @ingroup TMLErrorsConst
TML_ERR_MISSING_OBJ = 15

## @brief Missing Information.
#  @ingroup TMLErrorsConst
TML_ERR_INFORMATION_UNDEFINED = 16

## @brief Unknown command execution state.
#  @ingroup TMLErrorsConst
TML_ERR_COMMAND_STATE_UNDEFINED = 17

## @brief Unknown command execution mode.
#  @ingroup TMLErrorsConst
TML_ERR_COMMAND_MODE_UNDEFINED = 18

## @brief Reply type is undefined.
#  @ingroup TMLErrorsConst
TML_ERR_COMMAND_REPLY_TYPE_UNDEFINED = 19

## @brief Progress value is out of range.
#  @ingroup TMLErrorsConst
TML_ERR_COMMAND_PROGRESS_RANGE = 20

## @brief Dispatcher not created or initialized.
#  @ingroup TMLErrorsConst
TML_ERR_DISPATCHER_NOT_CREATED = 21

## @brief 	Dispatcher already exists.
#  @ingroup TMLErrorsConst
TML_ERR_DISPATCHER_ALREADY_EXISTS = 22

## @brief 	No command handler for this command id.
#  @ingroup TMLErrorsConst
TML_ERR_DISPATCHER_CMD_NOT_REGISTERED = 23

## @brief 	Listener not initialized.
#  @ingroup TMLErrorsConst
TML_ERR_LISTENER_NOT_INITIALIZED = 25

## @brief   Listener already started.
#  @ingroup TMLErrorsConst
TML_ERR_LISTENER_ALREADY_STARTED = 27

## @brief 	Communication error at listener.
#  @ingroup TMLErrorsConst
TML_ERR_LISTENER_COMMUNICATION = 28

## @brief 	Sender not initialized.
#  @ingroup TMLErrorsConst
TML_ERR_SENDER_NOT_INITIALIZED = 30

## @brief 	Sender can't register profile.
#  @ingroup TMLErrorsConst
TML_ERR_SENDER_PROFILE_REGISTRATION = 31

## @brief 	Invalid host and/or port address.
#  @ingroup TMLErrorsConst
TML_ERR_SENDER_INVALID_PARAMS = 32

## @brief 	Sender channel not initilialized.
#  @ingroup TMLErrorsConst
TML_ERR_CHANNEL_NOT_INITIALIZED = 33

## @brief 	Communication error at sender.
#  @ingroup TMLErrorsConst
TML_ERR_SENDER_COMMUNICATION = 35

## @brief 	Profile is not supported.
#  @ingroup TMLErrorsConst
TML_ERR_SENDER_PROFILE_NOT_SUPPORTED = 37

## @brief 	Internal hash table error.
#  @ingroup TMLErrorsConst
TML_ERR_HASH = 38

## @brief 	Key is of wrong type.
#  @ingroup TMLErrorsConst
TML_ERR_HASH_WRONG_TYPE = 39

## @brief 	Attribute is not set.
#  @ingroup TMLErrorsConst
TML_ERR_ATTRIBUTE_NOT_SET = 40

## @brief 	Try to change listener property while listener is started.
#  @ingroup TMLErrorsConst
TML_ERR_NOT_OPERABLE_AT_THE_MOMENT = 41

## @brief 	Cannot read TML state.
#  @ingroup TMLErrorsConst
TML_ERR_COM_LAYER_READ_STATE = 42

## @brief 	BEEP frame type error.
#  @ingroup TMLErrorsConst
TML_ERR_COM_LAYER_FRAME_TYPE_IS_ERR = 43

## @brief 	Unexpected BEEP frame type.
#  @ingroup TMLErrorsConst
TML_ERR_COM_LAYER_FRAME_UNEXP_TYPE = 44

## @brief 	Server closed connection.
#  @ingroup TMLErrorsConst
TML_ERR_COM_LAYER_CONNECTION_CLOSE = 45

## @brief 	Initialization error.
#  @ingroup TMLErrorsConst
TML_ERR_INITIALIZATION = 46

## @brief 	Command call timeout.
#  @ingroup TMLErrorsConst
TML_ERR_TIMEOUT = 47

## @brief 	No profiles registered.
#  @ingroup TMLErrorsConst
TML_ERR_NOPROFILES = 48

## @brief 	System out of resources.
#  @ingroup TMLErrorsConst
TML_ERR_SYSTEMRESOURCES = 49

## @brief 	Timeout on waiting for async command execution.
#  @ingroup TMLErrorsConst
TML_ERR_TIMEOUT_ON_WAIT_FOR_ASYNC = 50

## @brief Destination for call not found.
#  @ingroup TMLErrorsConst
TML_ERR_DESTINATION_OBJ_NOT_FOUND = 51

## @brief 	Error in load balance calculation.
#  @ingroup TMLErrorsConst
TML_ERR_LOAD_BALANCE_CALCULATION = 52

## @brief Unable to bind listener address/port.
#  @ingroup TMLErrorsConst
TML_ERR_LISTENER_ADDRESS_BINDING = 53

## @brief System shutdown in progress.
#  @ingroup TMLErrorsConst
TML_ERR_SHUTDOWN = 54

## @brief Callback not initialized.
#  @ingroup TMLErrorsConst
TML_ERR_STREAM_INVALID_CALLBACK = 55

## @brief Invalid stream address parameter.
#  @ingroup TMLErrorsConst
TML_ERR_STREAM_INVALID_ADDRESS = 56

## @brief Invalid stream identification.
#  @ingroup TMLErrorsConst
TML_ERR_STREAM_INVALID_IDENTIFIER = 57

## @brief Stream size not available.
#  @ingroup TMLErrorsConst
TML_ERR_STREAM_SIZE_NOT_AVAILABLE = 58

## @brief Stream read operation failed.
#  @ingroup TMLErrorsConst
TML_ERR_STREAM_READ_FAILED = 59

## @brief 	Stream position is not available.
#  @ingroup TMLErrorsConst
TML_ERR_STREAM_POSITION_NOT_AVAILABLE = 60

## @brief Stream seek operation is not operable.
#  @ingroup TMLErrorsConst
TML_ERR_STREAM_SEEK_NOT_OPERABLE = 61

## @brief 	Stream readBuffer operation reached end of file.
#  @ingroup TMLErrorsConst
TML_ERR_STREAM_READBUFFER_EOF = 62

## @brief 	Stream download error.
#  @ingroup TMLErrorsConst
TML_ERR_STREAM_DOWNLOAD_FAILED = 63

## @brief 	Stream download canceled.
#  @ingroup TMLErrorsConst
TML_ERR_STREAM_DOWNLOAD_CANCELED = 64

## @brief 	Stream download error in write callback.
#  @ingroup TMLErrorsConst
TML_ERR_STREAM_DOWNLOAD_WRITE_FAILED = 65

## @brief Stream is already in use.
#  @ingroup TMLErrorsConst
TML_ERR_STREAM_ALREADY_IN_USE = 66

## @brief 	Retain open error / profile & host & port don't match with the ID.
#  @ingroup TMLErrorsConst
TML_ERR_STREAM_OPEN_ADDRESS_MISMATCH = 67

## @brief 	Stream write failed.
#  @ingroup TMLErrorsConst
TML_ERR_STREAM_WRITE_FAILED = 68

# ------------------ command modes ------------------
## @defgroup TMLCmdModeConst TML command modes
#  @ingroup TMLSIDEXConst
#
#  TML command modes found in header information.
#  @sa tml.command.TMCmd.mode

## @brief Async command call
#  @ingroup TMLCmdModeConst
TMLCOM_MODE_ASYNC = 0

## @brief Sync command call
#  @ingroup TMLCmdModeConst
TMLCOM_MODE_SYNC = 1

## @brief Event call.
#  @ingroup TMLCmdModeConst
TMLCOM_MODE_EVT = 2

# ------------------ status reply types--------------
## @defgroup TMLRpyTypeConst TML status reply types
#  @ingroup TMLSIDEXConst
#
#  Status reply types.
#  @sa tml.command.TMCmd.reply_type

## @brief Warning message reply.
#  @ingroup TMLRpyTypeConst
TMLCOM_RPY_WARNING = 10

## @brief Error message reply.
#  @ingroup TMLRpyTypeConst
TMLCOM_RPY_ERROR = 20

## @brief Information message reply.
#  @ingroup TMLRpyTypeConst
TMLCOM_RPY_INFORMATION = 30

# ------------------ seek stream origins --------------
## @defgroup TMLStreamSeekConst Seek stream origins.
#  @ingroup TMLSIDEXConst
#
#  Seek stream origins.

## @brief from start of stream (>0)
#  @ingroup TMLStreamSeekConst
SO_FROM_BEGINNING = 0

## @brief from current position
#  @ingroup TMLStreamSeekConst
SO_FROM_CURRENT = 1

## @brief from end of stream (<0)
#  @ingroup TMLStreamSeekConst
SO_FROM_END = 2
