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

import tml
import types
import tml.profile
import tml.command
import tml.common
from tml.constants import *

## @defgroup TMLCoreGroup TML Core
#  @ingroup highLevelClasses
#
#  The TML core provides a high level interface to TCP/IP bindings, synchronous and asynchronous calls,
#  and lister management. Profiles a added and managed through a TML core object as well.


#  @cond NODOC
def on_core_command_ready_handler(command_handle, tmlcore):
    cmd = tmlcore.get_pending_command(command_handle)
    try:
        if isinstance(tmlcore.on_cmd_ready, (types.FunctionType, types.MethodType)) and (cmd.mode == TMLCOM_MODE_ASYNC):
            tmlcore.on_cmd_ready(tmlcore, cmd)
    finally:
        cmd.release_data()
        tmlcore.del_pending_command(command_handle)
#  @endcond


#  @cond NODOC
def on_core_command_progress_handler(command_handle, tmlcore, progress):
    if tmlcore.has_pending_command(command_handle):
        cmd = tmlcore.get_pending_command(command_handle)
    else:
        cmd = tml.command.TMLCmd(handle=command_handle)
        cmd.tml_core = tmlcore
    try:
        if isinstance(tmlcore.on_progress, (types.FunctionType, types.MethodType)) and cmd.show_progress:
            tmlcore.on_progress(tmlcore, cmd, progress)
    finally:
        cmd.release_data()
#  @endcond


#  @cond NODOC
def on_core_command_statusreply_handler(command_handle, tmlcore, itype, smsg):
    cmd = tml.command.TMLCmd(handle=command_handle)
    cmd.tml_core = tmlcore
    try:
        if isinstance(tmlcore.on_status_reply, (types.FunctionType, types.MethodType)):
            tmlcore.on_status_reply(tmlcore, cmd, itype, smsg)
    finally:
        cmd.release_data()
#  @endcond


## @brief TML Core Object
#  @ingroup TMLCoreGroup
#
#  The TMLCore manages all communication of a peer using a single TCP/IP port. Normally one TMLCore instance
#  per process is enough. Multiple cores are possible, but normally not necessary.
class TMLCore(object):
    ## @brief Constructor of TMLCore
    #
    #  The constructor opens a TML core. The listener will remain
    #  disabled.
    def __init__(self):

        # private
        self.__core_handle = tml.tml_Core_Open(0)

        self.__on_cmd_ready = None
        self.__on_progress = None
        self.__on_status_reply = None

        self.__profiles = None
        self.__pending_commands = {}

        self.__closed = False

        # public

        return

        # ------------ properties ---------------------
        # -- the code here is for documentation only --
        self.core_handle = None
        self.profiles = {}
        self.listener_binding = "0.0.0.0:44000"
        self.listener_enabled = False
        self.max_queued_events = 1000
        self.max_bal_fail_count = 1

        self.on_cmd_ready = None
        self.on_progress = None
        self.on_status_reply = None
        # ------------ end properties -----------------

    ## @brief Shutdown and close the TML core
    #
    #  The destructor disables the listener and removes all
    #  profiles from the core. Finally the core itself is closed.
    def __del__(self):
        self.close()
        tml.tml_Core_Close(self.__core_handle)

    ## @brief Close TMLCore and cleanup depending objects.
    #
    #  Python 2 causes an Exception if close() is not called before
    #  object Destruction.
    def close(self):
        if self.__closed:
            return
        self.__closed = True
        if self.listener_enabled:
            self.listener_enabled = False
        if self.profiles is not None:
            self.__profiles.clear()
            self.__profiles.close(True)
            self.__profiles = None

    ## @property listener_binding
    #  @brief [\b str] <host>:<port>
    #
    #  Host can be an ip address, hostname or 0.0.0.0 for all interfaces.
    #
    #  @par Access:
    #    read/write

    #  @cond NODOC
    @property
    def listener_binding(self):
        lhost = tml.tml_Core_Get_ListenerIP(self.__core_handle)
        lport = tml.tml_Core_Get_ListenerPort(self.__core_handle)
        return "%s:%s" % (lhost, lport)

    @listener_binding.setter
    def listener_binding(self, value):
        lhost, lport = tml.common.split_binding(value)
        tml.tml_Core_Set_ListenerIP(self.__core_handle, lhost)
        tml.tml_Core_Set_ListenerPort(self.__core_handle, lport)
    #  @endcond

    ## @property listener_enabled
    #  @brief [\b bool] True if the TML listener is started
    #
    #  The listener is started by assigning True to this property.
    #
    #  @par Access:
    #    read/write

    #  @cond NODOC
    @property
    def listener_enabled(self):
        return tml.tml_Core_Get_ListenerEnabled(self.__core_handle)

    @listener_enabled.setter
    def listener_enabled(self, value):
        tml.tml_Core_Set_ListenerEnabled(self.__core_handle, value)
    #  @endcond

    ## @property on_cmd_ready
    #  @brief [\b function, \b method] a callback routine for command ready handling
    #
    #  @par Access:
    #    read/write
    #
    #  @sa ON_COMMAND_READY()

    #  @cond NODOC
    @property
    def on_cmd_ready(self):
        return self.__on_cmd_ready

    @on_cmd_ready.setter
    def on_cmd_ready(self, value):
        self.__on_cmd_ready = value
    #  @endcond

    ## @property core_handle
    #  @brief [\b int] TML core handle
    #
    #  @par Access:
    #    read

    #  @cond NODOC
    @property
    def core_handle(self):
        return self.__core_handle
    #  @endcond

    ## @property profiles
    #  @brief [\b tml.profile.TMLProfiles] access interfaces
    #
    #  @par Access:
    #    read

    #  @cond NODOC
    @property
    def profiles(self):
        if self.__profiles is None:
            self.__profiles = tml.profile.TMLProfiles(self)
        return self.__profiles
    #  @endcond

    ## @property max_queued_events
    #  @brief [\b int] maximum queue size for sending events
    #
    #  If the maximum number of queued events for an event consumer
    #  is reached, the oldest event will be deleted. The default value is 1000.
    #
    #  @par Access:
    #    read/write

    #  @cond NODOC
    @property
    def max_queued_events(self):
        return tml.tml_Evt_Get_MaxQueuedEventMessages(self.__core_handle)

    @max_queued_events.setter
    def max_queued_events(self, value):
        tml.tml_Evt_Set_MaxQueuedEventMessages(self.__core_handle, value)
    #  @endcond

    ## @property max_bal_fail_count
    #  @brief [\b int] maximum load balancing connection fail count
    #
    #  The maximum connection fail count defines after how many unsuccessful
    #  connections a command receiver is automatically removed from the list.
    #  A value of 0 means it is never removed. The default value is 1.
    #
    #  @par Access:
    #    read/write

    #  @cond NODOC
    @property
    def max_bal_fail_count(self):
        return tml.tml_Bal_Get_MaxConnectionFailCount(self.__core_handle)

    @max_bal_fail_count.setter
    def max_bal_fail_count(self, value):
        tml.tml_Bal_Set_MaxConnectionFailCount(self.__core_handle, value)
    #  @endcond

    ## @property on_progress
    #  @brief [\b function, \b method] a callback routine to handle progress
    #
    #  This event is called if a pending command is sending a progress back to the caller.
    #
    #  @par Access:
    #    read/write
    #
    #  @sa ON_PROGRESS()

    #  @cond NODOC
    @property
    def on_progress(self):
        return self.__on_progress

    @on_progress.setter
    def on_progress(self, value):
        self.__on_progress = value
    #  @endcond

    ## @property on_status_reply
    #  @brief [\b function, \b method]  status reply handler
    #
    #  The status reply handler is called if a status reply was received for a command.
    #
    #  @param tmlcore  : [\b TMLCore] the tml core object calling the event
    #  @param cmd      : [\b TMLCmd]  the command that signaled ready
    #  @param itype    : [\b int]     @ref TMLRpyTypeConst "reply type"
    #  @param smsg     : [\b str]     reply message
    #
    #  @par Access:
    #    read/write
    #
    #  @sa ON_STATUS_REPLY()

    #  @cond NODOC
    @property
    def on_status_reply(self):
        return self.__on_status_reply

    @on_status_reply.setter
    def on_status_reply(self, value):
        self.__on_status_reply = value
    #  @endcond

    ## @brief Send a synchronous TML message
    #
    #  @param host    : [\b str] host address
    #  @param port    : [\b str] port number
    #  @param profile : [\b str] profile name
    #  @param cmd     : [\b TMLCmd] command to send
    #  @param timeout : [\b int][\em optional] timeout in milliseconds (default: 5000)
    def call_sync(self, host, port, profile, cmd, timeout=5000):
        cmd.tml_core = self
        self.__pending_commands[cmd.command_handle] = cmd
        tml.tml_Cmd_Register_CommandReady(cmd.command_handle, on_core_command_ready_handler, self)

        if self.on_progress is not None:
            tml.tml_Cmd_Register_Progress(cmd.command_handle, on_core_command_progress_handler, self)
        if self.on_status_reply is not None:
            tml.tml_Cmd_Register_StatusReply(cmd.command_handle, on_core_command_statusreply_handler, self)

        tml.tml_Send_SyncMessage(self.__core_handle, cmd.command_handle, profile, host, port, timeout)

    ## @brief Send a asynchronous TML message
    #
    #  @param host    : [\b str] host address
    #  @param port    : [\b str] port number
    #  @param profile : [\b str] profile name
    #  @param cmd     : [\b TMLCmd] command to send
    #  @param timeout : [\b int] [\em optional] timeout in milliseconds (default: 5000)
    def call_async(self, host, port, profile, cmd, timeout=5000):
        cmd.tml_core = self
        self.__pending_commands[cmd.command_handle] = cmd
        tml.tml_Cmd_Register_CommandReady(cmd.command_handle, on_core_command_ready_handler, self)

        if self.on_progress is not None:
            tml.tml_Cmd_Register_Progress(cmd.command_handle, on_core_command_progress_handler, self)
        if self.on_status_reply is not None:
            tml.tml_Cmd_Register_StatusReply(cmd.command_handle, on_core_command_statusreply_handler, self)

        tml.tml_Send_AsyncMessage(self.__core_handle, cmd.command_handle, profile, host, port, timeout)

    # --------------------- internal methods -------------------------

    # @cond INTERNAL

    ## @brief get a pending command
    #
    #  @param command_handle :[\b int] TML command handle
    #
    #  @returns [\b TMLCmd] pending command
    def get_pending_command(self, command_handle):
        return self.__pending_commands[command_handle]

    ## @brief delete a pending command
    #
    #  @param command_handle :[\b int] TML command handle
    def del_pending_command(self, command_handle):
        del self.__pending_commands[command_handle]

    ## @brief Check if a pending command object exists for a handle.
    #
    #  @param command_handle :[\b int] TML command handle
    #
    #  @returns [\b tml.command.TMLCmd] pending command
    def has_pending_command(self, command_handle):
        return command_handle in self.__pending_commands

    def get_core(self):
        return self

    # @endcond

