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
import tml.command
import tml.common
import tml.stream
from tml.constants import *

import types
import weakref


## @defgroup TMLProfileGroup TML Profile
#  @ingroup highLevelClasses
#
#  TML Profile class description and usage.

#  @cond NODOC
def on_cmd_dispatch_handler(hcmd, profile):
    cmd = tml.command.TMLCmd(handle=hcmd)
    handler = profile.get_cmd_handler(cmd.command)
    if isinstance(handler, (types.FunctionType, types.MethodType)):
        try:
            handler(profile, cmd)
            pass
        except Exception as e:
            cmd.error = TML_ERR_COMMON  # an error was raised in handler routine
            cmd.errormsg = str(e)

        finally:
            cmd.release_data()

#  @endcond


#  @cond NODOC
def on_cmd_custom_dispatch_handler(cmdid, hcmd, profile):
    cmd = tml.command.TMLCmd(handle=hcmd)
    handler = profile.on_cmd_custom_dispatch
    if isinstance(handler, (types.FunctionType, types.MethodType)) and (cmd.mode == TMLCOM_MODE_ASYNC):
        try:
            handler(profile, cmdid, cmd)
        except Exception as e:
            cmd.error = TML_ERR_COMMON  # an error was raised in handler routine
            cmd.errormsg = str(e)
        finally:
            cmd.release_data()

#  @endcond


#  @cond NODOC
def on_profile_command_ready_handler(hcommand, tmlprofile):
    cmd = tmlprofile.get_pending_command(hcommand)
    try:
        if isinstance(tmlprofile.on_cmd_ready, (types.FunctionType, types.MethodType)):
            tmlprofile.on_cmd_ready(tmlprofile, cmd)
    finally:
        cmd.release_data()
        tmlprofile.del_pending_command(hcommand)
#  @endcond


#  @cond NODOC
def on_populate_event_consumer_handler(sprofile, profile):
    handler = profile.on_populate_event_consumer
    if isinstance(handler, (types.FunctionType, types.MethodType)):
        handler(profile)
#  @endcond


#  @cond NODOC
def on_populate_call_destinations_handler(sprofile, profile):
    handler = profile.on_populate_call_destinations
    if isinstance(handler, (types.FunctionType, types.MethodType)):
        handler(profile)
#  @endcond


#  @cond NODOC
def on_cmd_progress_handler(hcmd, tmlprofile, iprogress):
    if tmlprofile.has_pending_command(hcmd):
        cmd = tmlprofile.get_pending_command(hcmd)
    else:
        cmd = tml.command.TMLCmd(handle=hcmd)
    handler = tmlprofile.on_progress
    if isinstance(handler, (types.FunctionType, types.MethodType)) and cmd.show_progress:
        try:
            handler(tmlprofile, cmd, iprogress)
        finally:
            cmd.release_data()
#  @endcond


#  @cond NODOC
def on_cmd_status_reply_handler(hcmd, tmlprofile, itype, smessage):
    if tmlprofile.has_pending_command(hcmd):
        cmd = tmlprofile.get_pending_command(hcmd)
    else:
        cmd = tml.command.TMLCmd(handle=hcmd)
    handler = tmlprofile.on_status_reply
    if isinstance(handler, (types.FunctionType, types.MethodType)):
        try:
            handler(tmlprofile, cmd, itype, smessage)
        finally:
            cmd.release_data()
#  @endcond


#  @cond NODOC
def on_event_error_handler(sprofile, shost, sport, cmdid, ierr, tmlprofile):
    handler = tmlprofile.on_event_error
    if isinstance(handler, (types.FunctionType, types.MethodType)):
        handler(tmlprofile, "%s:%s" % (shost, sport), cmdid, ierr)
#  @endcond


#  @cond NODOC
def on_queue_overflow_handler(sprofile, cmdid, tmlprofile):
    handler = tmlprofile.on_queue_overflow
    if isinstance(handler, (types.FunctionType, types.MethodType)):
        handler(tmlprofile, cmdid)
#  @endcond


#  @cond NODOC
def on_register_event_consumer_handler(bsubscribe, shost, sport, tmlprofile):
    handler = tmlprofile.on_register_event_consumer
    if isinstance(handler, (types.FunctionType, types.MethodType)):
        try:
            handler(tmlprofile, "%s:%s" % (shost, sport), bsubscribe)
        except:
            return False
    return True
#  @endcond


#  @cond NODOC
def on_register_call_destination_handler(bsubscribe, shost, sport, tmlprofile):
    handler = tmlprofile.on_register_call_destination
    if isinstance(handler, (types.FunctionType, types.MethodType)):
        try:
            handler(tmlprofile, "%s:%s" % (shost, sport), bsubscribe)
        except:
            return False
    return True
#  @endcond


#  @cond NODOC
def on_busy_status_request_handler(hcmd, tmlprofile):
    err = TML_ERR_INFORMATION_UNDEFINED
    cmd = tml.command.TMLCmd(handle=hcmd)
    handler = tmlprofile.on_busy_status_request
    if isinstance(handler, (types.FunctionType, types.MethodType)):
        try:
            err = handler(tmlprofile, cmd)
        except Exception as e:
            err = TML_ERR_COMMON
            cmd.error = err  # an error was raised in handler routine
            cmd.errormsg = str(e)
    return err
#  @endcond


#  @cond NODOC
def on_balance_calculation_handler(rcount, rbusystate, tmlprofile):
    err = TML_ERR_INFORMATION_UNDEFINED
    cmds = []
    for hcmd in rbusystate:
        cmd = tml.command.TMLCmd(handle=hcmd)
        cmds.append(cmd)
    handler = tmlprofile.on_balance_calculation
    if isinstance(handler, (types.FunctionType, types.MethodType)):
        try:
            err = handler(tmlprofile, cmds, rcount)
        except:
            err = TML_ERR_COMMON
    return err
#  @endcond


## @brief TML Profile
#  @ingroup TMLProfileGroup
#
#  A profile is used to implement TML interfaces.
class TMLProfile(object):
    ## @brief Constructor of TMLProfile.
    #
    #  @param profile :[\b str] profile name (ID)
    #  @param core    :[\b tml.core.TMLCore] TML core instance
    def __init__(self, profile, core):
        self.__profile = profile
        self.__commands = {}
        self.__snd_streams = {}
        self.__rec_streams = {}

        self.__tml_core = core

        self.__on_cmd_custom_dispatch = None
        self.__on_cmd_ready = None
        self.__on_populate_event_consumer = None
        self.__on_populate_call_destinations = None
        self.__on_progress = None
        self.__on_status_reply = None
        self.__on_event_error = None
        self.__on_queue_overflow = None
        self.__on_register_event_consumer = None
        self.__on_register_call_destination = None
        self.__on_busy_status_request = None
        self.__on_balance_calculation = None

        self.__pending_commands = {}

        tml.tml_Profile_Set_OnCustomDispatch(self.__tml_core.core_handle, self.__profile, on_cmd_custom_dispatch_handler, self)
        return

        # ------------ properties ---------------------
        # -- the code here is for documentation only --
        self.on_cmd_custom_dispatch = None
        self.on_cmd_ready = None
        self.on_populate_event_consumer = None
        self.on_populate_call_destinations = None
        self.on_progress = None
        self.on_status_reply = None
        self.on_event_error = None
        self.on_queue_overflow = None
        self.on_register_event_consumer = None
        self.on_register_call_destination = None
        self.on_busy_status_request = None
        self.on_balance_calculation = None
        self.tml_core = None
        self.profile_url = ''
        # ------------ end properties -----------------

    ## @brief Destructor of TMLProfile.
    def __del__(self):
        for lstream in self.__snd_streams.values():
            lstream.close(True)
        self.__snd_streams.clear()

        for lstream in self.__rec_streams.values():
            lstream.close(True)
        self.__rec_streams.clear()


    ## @property profile_url
    #  @brief [\b str] Profile string identifier.
    #
    #  @par Access:
    #    read

    #  @cond NODOC
    @property
    def profile_url(self):
        return self.__profile

    #  @endcond


    ## @property tml_core
    #  @brief [\b tml.core.TMLCore] TML core handling this command.
    #
    #  This property is None until, it was sent through a TML core.
    #
    #  @par Access:
    #    read

    #  @cond NODOC
    @property
    def tml_core(self):
        return self.__tml_core

    #  @endcond

    ## @property on_balance_calculation
    #  @brief [\b function, \b method] a callback function/method
    #
    #  Evaluate load information returned by the status request handlers
    #  of registered command receivers to calculate the index of the next peer to receive a load balanced command.
    #
    #  @par Access:
    #    read/write
    #
    #  @sa ON_BALANCE_CALCULATION(), tml_Bal_Set_OnCalculation()

    #  @cond NODOC
    @property
    def on_balance_calculation(self):
        return self.__on_balance_calculation

    @on_balance_calculation.setter
    def on_balance_calculation(self, value):
        if value is None:
            tml.tml_Bal_Set_OnCalculation(self.__tml_core.core_handle, self.__profile, None, None)
        else:
            tml.tml_Bal_Set_OnCalculation(self.__tml_core.core_handle, self.__profile, None, None)
            tml.tml_Bal_Set_OnCalculation(self.__tml_core.core_handle, self.__profile, on_balance_calculation_handler, self)
        self.__on_balance_calculation = value
    #  @endcond

    ## @property on_busy_status_request
    #  @brief [\b function, \b method] a callback function/method
    #
    #  To implement a custom load balancing instead of a round robin the status request handler can collect and
    #  return host specifc data. The data is added to the TML command passed to the handler function. On sender side
    #  the data is collected and passed to a load balance calculation handler (see on_balance_calculation)
    #  that will return the index of the next registered peer to call. If either the busy status request handler
    #  or the load balancing calculation handler is not implemented round robin will be used.
    #
    #  @par Access:
    #    read/write
    #
    #  @sa ON_BUSY_STATUS_REQUEST(), tml_Bal_Set_OnBusyStatusRequest()

    #  @cond NODOC
    @property
    def on_busy_status_request(self):
        return self.__on_busy_status_request

    @on_busy_status_request.setter
    def on_busy_status_request(self, value):
        if value is None:
            tml.tml_Bal_Set_OnBusyStatusRequest(self.__tml_core.core_handle, self.__profile, None, None)
        else:
            tml.tml_Bal_Set_OnBusyStatusRequest(self.__tml_core.core_handle, self.__profile, None, None)
            tml.tml_Bal_Set_OnBusyStatusRequest(self.__tml_core.core_handle, self.__profile, on_busy_status_request_handler, self)
        self.__on_busy_status_request = value
    #  @endcond

    ## @property on_register_call_destination
    #  @brief [\b function, \b method] a callback function/method
    #
    #  The handler function is called if a load balancing subscription or unsubscription request is received.
    #
    #  @par Access:
    #    read/write
    #
    #  @sa ON_REGISTER_CALL_DESTINATION(), tml_Bal_Set_OnPeerRegister()

    #  @cond NODOC
    @property
    def on_register_call_destination(self):
        return self.__on_register_call_destination

    @on_register_call_destination.setter
    def on_register_call_destination(self, value):
        if value is None:
            tml.tml_Bal_Set_OnPeerRegister(self.__tml_core.core_handle, self.__profile, None, None)
        else:
            tml.tml_Bal_Set_OnPeerRegister(self.__tml_core.core_handle, self.__profile, None, None)
            tml.tml_Bal_Set_OnPeerRegister(self.__tml_core.core_handle, self.__profile, on_register_call_destination_handler, self)
        self.__on_register_call_destination = value
    #  @endcond

    ## @property on_register_event_consumer
    #  @brief [\b function, \b method] a callback function/method
    #
    #  The handler function is called if an event subscription or unsubscription request is received.
    #
    #  @par Access:
    #    read/write
    #
    #  @sa ON_REGISTER_EVENT_CONSUMER(), tml_Evt_Set_OnPeerRegister()

    #  @cond NODOC
    @property
    def on_register_event_consumer(self):
        return self.__on_register_event_consumer

    @on_register_event_consumer.setter
    def on_register_event_consumer(self, value):
        if value is None:
            tml.tml_Evt_Set_OnPeerRegister(self.__tml_core.core_handle, self.__profile, None, None)
        else:
            tml.tml_Evt_Set_OnPeerRegister(self.__tml_core.core_handle, self.__profile, None, None)
            tml.tml_Evt_Set_OnPeerRegister(self.__tml_core.core_handle, self.__profile, on_register_event_consumer_handler, self)
        self.__on_register_event_consumer = value
    #  @endcond

    ## @property on_queue_overflow
    #  @brief [\b function, \b method] a callback function/method
    #
    #  Events are internally queued before they are sent. If the number of queue entries reaches
    #  it's maximum the overflow handler is called.
    #
    #  @par Access:
    #    read/write
    #
    #  @sa ON_QUEUE_OVERFLOW(), tml_Evt_Set_OnQueueOverflow()

    #  @cond NODOC
    @property
    def on_queue_overflow(self):
        return self.__on_queue_overflow

    @on_queue_overflow.setter
    def on_queue_overflow(self, value):
        if value is None:
            tml.tml_Evt_Set_OnQueueOverflow(self.__tml_core.core_handle, self.__profile, None, None)
        else:
            tml.tml_Evt_Set_OnQueueOverflow(self.__tml_core.core_handle, self.__profile, None, None)
            tml.tml_Evt_Set_OnQueueOverflow(self.__tml_core.core_handle, self.__profile, on_queue_overflow_handler, self)
        self.__on_queue_overflow = value
    #  @endcond

    ## @property on_event_error
    #  @brief [\b function, \b method] a callback function/method
    #
    #  The on_event_error callback enables tracking of possible errors on event transmission.
    #
    #  @par Access:
    #    read/write
    #
    #  @sa ON_EVENT_ERROR(), tml_Evt_Set_OnError()

    #  @cond NODOC
    @property
    def on_event_error(self):
        return self.__on_event_error

    @on_event_error.setter
    def on_event_error(self, value):
        if value is None:
            tml.tml_Evt_Set_OnError(self.__tml_core.core_handle, self.__profile, None, None)
        else:
            tml.tml_Evt_Set_OnError(self.__tml_core.core_handle, self.__profile, None, None)
            tml.tml_Evt_Set_OnError(self.__tml_core.core_handle, self.__profile, on_event_error_handler, self)
        self.__on_event_error = value
    #  @endcond

    ## @property on_status_reply
    #  @brief [\b function, \b method] a callback function/method
    #
    #  The on_status_reply event is called if a status reply was received for a pending command.
    #
    #  @par Access:
    #    read/write
    #
    #  @sa ON_STATUS_REPLY(), tml_Cmd_Register_StatusReply()

    #  @cond NODOC
    @property
    def on_status_reply(self):
        return self.__on_status_reply

    @on_status_reply.setter
    def on_status_reply(self, value):
        self.__on_status_reply = value
    #  @endcond


    ## @property on_progress
    #  @brief [\b function, \b method] a callback function/method
    #
    #  The on_progress event is called if a progress change was received for a pending command.
    #
    #  @par Access:
    #    read/write
    #
    #  @sa ON_PROGRESS(), tml_Cmd_Register_Progress()

    #  @cond NODOC
    @property
    def on_progress(self):
        return self.__on_progress

    @on_progress.setter
    def on_progress(self, value):
        self.__on_progress = value
    #  @endcond

    ## @property on_populate_call_destinations
    #  @brief [\b function, \b method] a callback function/method
    #
    #  If no call destination is registered and a call is placed by calling call_sync_balanced or
    #  call_async_balanced, the on_populate_call_destinations handler is called to register known call destinations.
    #
    #  @par Access:
    #    read/write
    #
    #  @sa ON_POPULATE_CALL_DESTINATIONS(), tml_Bal_Set_OnPopulate()

    #  @cond NODOC
    @property
    def on_populate_call_destinations(self):
        return self.__on_populate_call_destinations

    @on_populate_call_destinations.setter
    def on_populate_call_destinations(self, value):
        if value is None:
            tml.tml_Bal_Set_OnPopulate(self.__tml_core.core_handle, self.__profile, None, None)
        else:
            tml.tml_Bal_Set_OnPopulate(self.__tml_core.core_handle, self.__profile, None, None)
            tml.tml_Bal_Set_OnPopulate(self.__tml_core.core_handle, self.__profile, on_populate_call_destinations_handler, self)
        self.__on_populate_call_destinations = value
    #  @endcond


    ## @property on_populate_event_consumer
    #  @brief [\b function, \b method] a callback function/method
    #
    #  If no event receiver is registered and an event is added to the queue by calling send_event,
    #  the on_populate_event_consumer handler is called to register known event receivers.
    #
    #  @par Access:
    #    read/write
    #
    #  @sa ON_POPULATE_EVENT_CONSUMERS(), tml_Evt_Set_OnPopulate()

    #  @cond NODOC
    @property
    def on_populate_event_consumer(self):
        return self.__on_populate_event_consumer

    @on_populate_event_consumer.setter
    def on_populate_event_consumer(self, value):
        if value is None:
            tml.tml_Evt_Set_OnPopulate(self.__tml_core.core_handle, self.__profile, None, None)
        else:
            tml.tml_Evt_Set_OnPopulate(self.__tml_core.core_handle, self.__profile, None, None)
            tml.tml_Evt_Set_OnPopulate(self.__tml_core.core_handle, self.__profile, on_populate_event_consumer_handler, self)
        self.__on_populate_event_consumer = value
    #  @endcond

    ## @property on_cmd_custom_dispatch
    #  @brief [\b function, \b method] a callback routine for commands not registered in the profile
    #
    #  @par Access:
    #    read/write
    #
    #  @sa ON_CUSTOM_DISPATCH(), tml_Profile_Set_OnCustomDispatch()

    #  @cond NODOC
    @property
    def on_cmd_custom_dispatch(self):
        return self.__on_cmd_custom_dispatch

    @on_cmd_custom_dispatch.setter
    def on_cmd_custom_dispatch(self, value):
        if value is None:
            tml.tml_Profile_Set_OnCustomDispatch(self.__tml_core.core_handle, self.__profile, None, None)
        else:
            tml.tml_Profile_Set_OnCustomDispatch(self.__tml_core.core_handle, self.__profile, None, None)
            tml.tml_Profile_Set_OnCustomDispatch(self.__tml_core.core_handle, self.__profile, on_cmd_custom_dispatch_handler, self)
        self.__on_cmd_custom_dispatch = value
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

    ## @brief Register a command handler.
    #
    #  The command handler is a called if the listerner receives a message
    #  with the given cmdid. If no command handler is registered either an error is
    #  returned to the caller or the general command handler is called if registered.
    #
    #  @param cmdid   : [\b int] command id
    #  @param handler : [\b function, \b method] callback function to handle the call
    #
    # @sa on_cmd_custom_dispatch, ON_COMMAND_CALL(), tml_Profile_Register_Cmd()
    def register_cmd(self, cmdid, handler):
        if cmdid in self.__commands:
            raise KeyError("command %s already exists in %s" % (cmdid, self.__profile))
        elif not isinstance(handler, (types.FunctionType, types.MethodType)):
            raise ValueError("handler is not a function or method")
        else:
            self.__commands[cmdid] = handler
            tml.tml_Profile_Register_Cmd(self.__tml_core.core_handle, self.__profile, cmdid, on_cmd_dispatch_handler, self)

    ## @brief Register to receive events from a remote peer.
    #
    #  @param host    : [\b str] host address
    #  @param port    : [\b str] port number
    #  @param timeout : [\b int][\em optional] timeout in milliseconds (default: 5000)
    #
    #  @sa unregister_evt_consumer(), tml_Evt_Send_SubscriptionRequest()
    def register_evt_consumer(self, host, port, timeout=5000):
        lhost, lport = tml.common.split_binding(self.__tml_core.listener_binding)
        tml.tml_Evt_Send_SubscriptionRequest(self.__tml_core.core_handle, self.__profile, lhost, lport, host, port, timeout)

    ## @brief Unregister from remote peer events.
    #
    #  @param host    : [\b str] host address
    #  @param port    : [\b str] port number
    #  @param timeout : [\b int][\em optional] timeout in milliseconds (default: 5000)
    #
    #  @sa unregister_evt_consumer(), tml_Evt_Send_UnsubscriptionRequest()
    def unregister_evt_consumer(self, host, port, timeout=5000):
        lhost, lport = tml.common.split_binding(self.__tml_core.listener_binding)
        tml.tml_Evt_Send_UnsubscriptionRequest(self.__tml_core.core_handle, self.__profile, lhost, lport, host, port, timeout)

    ## @brief Register to receive balanced calls from a remote peer.
    #
    #  @param host    : [\b str] host address
    #  @param port    : [\b str] port number
    #  @param timeout : [\b int][\em optional] timeout in milliseconds (default: 5000)
    #
    #  @sa unregister_bal_receiver(), tml_Bal_Send_SubscriptionRequest()
    def register_bal_receiver(self, host, port, timeout=5000):
        lhost, lport = tml.common.split_binding(self.__tml_core.listener_binding)
        tml.tml_Bal_Send_SubscriptionRequest(self.__tml_core.core_handle, self.__profile, lhost, lport, host, port, timeout)

    ## @brief Unregister a balanced call receiver.
    #
    #  @param host    : [\b str] host address
    #  @param port    : [\b str] port number
    #  @param timeout : [\b int][\em optional] timeout in milliseconds (default: 5000)
    #
    #  @sa register_bal_receiver(), tml_Bal_Send_UnsubscriptionRequest()
    def unregister_bal_receiver(self, host, port, timeout=5000):
        lhost, lport = tml.common.split_binding(self.__tml_core.listener_binding)
        tml.tml_Bal_Send_UnsubscriptionRequest(self.__tml_core.core_handle, self.__profile, lhost, lport, host, port, timeout)

    ## @brief Remove a command handler from the profile.
    #
    #  @param cmdid  : [\b int] command id
    def del_cmd(self, cmdid):
        del self.__commands[cmdid]
        tml.tml_Profile_Register_Cmd(self.__tml_core.core_handle, self.__profile, cmdid, None, None)

    ## @brief Add event consumer to profile.
    #
    #  A list containing event consumer addresses in the form
    #  <host>:<port> or a single string can be passed.
    #
    #  @param abinding: [\b list, \b str] address or list of addresses in the form <host>:<port>
    #  @param rek     : [\b int][\em optional] recursion counter for internal check
    #
    #  @sa tml_Evt_Subscribe_MessageDestination()
    def add_event_consumer(self, abinding, rek=0):
        if rek > 5:
            raise Exception("maximum recursion depth reached %s" % rek)

        if isinstance(abinding, list):
            for lbinding in abinding:
                self.add_event_consumer(lbinding, rek + 1)
        else:
            lhost, lport = tml.common.split_binding(abinding)
            tml.tml_Evt_Subscribe_MessageDestination(self.__tml_core.core_handle, self.__profile, lhost, lport)

    ## @brief Remove event consumer from profile.
    #
    #  A list containing event consumer addresses in the form
    #  <host>:<port> or a single string can be passed.
    #
    #  @param abinding: [\b list, \b str] address or list of addresses in the form <host>:<port>
    #  @param rek     : [\b int][\em optional] recursion counter for internal check
    #
    #  @sa tml_Evt_Unsubscribe_MessageDestination()
    def remove_event_consumer(self, abinding, rek=0):
        if rek > 5:
            raise Exception("maximum recursion depth reached %s" % rek)

        if isinstance(abinding, list):
            for lbinding in abinding:
                self.remove_event_consumer(lbinding, rek + 1)
        else:
            lhost, lport = tml.common.split_binding(abinding)
            tml.tml_Evt_Unsubscribe_MessageDestination(self.__tml_core.core_handle, self.__profile, lhost, lport)

    ## @brief Remove all event consumers from profile.
    #
    #  @sa tml_Evt_Unsubscribe_All_MessageDestinations()
    def clear_event_consumer(self):
        tml.tml_Evt_Unsubscribe_All_MessageDestinations(self.__tml_core.core_handle, self.__profile)

    ## @brief add call destination to profile
    #
    #  Call destinations are peers implementing  command calls of this
    #  profile. Using call destinations instead of calling a peer directly
    #  allows balancing of calls to multiple peers.
    #  A list containing call destination addresses in the form
    #  <host>:<port> or a single string can be passed.
    #
    #  @param abinding: [\b list, \b str] address or list of addresses in the form <host>:<port>
    #  @param rek     : [\b int][\em optional] recursion counter for internal check
    #
    #  @sa tml_Bal_Subscribe_MessageDestination()
    def add_call_destination(self, abinding, rek=0):
        if rek > 5:
            raise Exception("maximum recursion depth reached %s" % rek)

        if isinstance(abinding, list):
            for lbinding in abinding:
                self.add_call_destination(lbinding, rek + 1)
        else:
            lhost, lport = tml.common.split_binding(abinding)
            tml.tml_Bal_Subscribe_MessageDestination(self.__tml_core.core_handle, self.__profile, lhost, lport)

    ## @brief Remove call destination from profile.
    #
    #  Call destinations are peers implementing  command calls of this
    #  profile. Using call destinations instead of calling a peer directly
    #  allows balancing of calls to multiple peers.
    #  A list containing call destination addresses in the form
    #  <host>:<port> or a single string can be passed.
    #
    #  @param abinding: [\b list, \b str] address or list of addresses in the form <host>:<port>
    #  @param rek     : [\b int][\em optional] recursion counter for internal check
    #
    #  @sa tml_Bal_Unsubscribe_MessageDestination()
    def remove_call_destination(self, abinding, rek=0):
        if rek > 5:
            raise Exception("maximum recursion depth reached %s" % rek)

        if isinstance(abinding, list):
            for lbinding in abinding:
                self.remove_call_destination(lbinding, rek + 1)
        else:
            lhost, lport = tml.common.split_binding(abinding)
            tml.tml_Bal_Unsubscribe_MessageDestination(self.__tml_core.core_handle, self.__profile, lhost, lport)

    ## @brief remove all call destinations from profile
    #
    #  @sa tml_Bal_Unsubscribe_All_MessageDestinations()
    def clear_call_destination(self):
        tml.tml_Bal_Unsubscribe_All_MessageDestinations(self.__tml_core.core_handle, self.__profile)

    ## @brief Send a synchronous TML message.
    #
    #  @param host    : [\b str] host address
    #  @param port    : [\b str] port number
    #  @param cmd     : [\b tml.command.TMLCmd] command to send
    #  @param timeout : [\b int][\em optional] timeout in milliseconds (default: 5000)
    #
    #  @sa tml_Send_SyncMessage()
    def call_sync(self, host, port, cmd, timeout=5000):
        if self.on_progress is not None:
            tml.tml_Cmd_Register_Progress(cmd.command_handle, on_cmd_progress_handler, self)
        if self.on_status_reply is not None:
            tml.tml_Cmd_Register_StatusReply(cmd.command_handle, on_cmd_status_reply_handler, self)
        tml.tml_Send_SyncMessage(self.__tml_core.core_handle, cmd.command_handle, self.__profile, host, port, timeout)

    ## @brief Send a asynchronous TML message
    #
    #  @param host    : [\b str] host address
    #  @param port    : [\b str] port number
    #  @param cmd     : [\b tml.command.TMLCmd] command to send
    #  @param timeout : [\b int] [\em optional] timeout in milliseconds (default: 5000)
    #
    #  @sa tml_Send_AsyncMessage()
    def call_async(self, host, port, cmd, timeout=5000):
        self.__pending_commands[cmd.command_handle] = cmd
        if self.on_progress is not None:
            tml.tml_Cmd_Register_Progress(cmd.command_handle, on_cmd_progress_handler, self)
        if self.on_status_reply is not None:
            tml.tml_Cmd_Register_StatusReply(cmd.command_handle, on_cmd_status_reply_handler, self)
        tml.tml_Cmd_Register_CommandReady(cmd.command_handle, on_profile_command_ready_handler, self)
        tml.tml_Send_AsyncMessage(self.__tml_core.core_handle, cmd.command_handle, self.__profile, host, port, timeout)

    ## @brief Send a synchronous TML message to call destinations.
    #
    #  The command is using the next call destination acoording to the
    #  balancing rules.
    #
    #  @param cmd     : [\b tml.command.TMLCmd] command to send
    #  @param timeout : [\b Integer][\em optional] timeout in milliseconds (default: 5000)
    #
    #  @sa tml_Bal_Send_SyncMessage()
    def call_sync_balanced(self, cmd, timeout=5000):
        if self.on_progress is not None:
            tml.tml_Cmd_Register_Progress(cmd.command_handle, on_cmd_progress_handler, self)
        if self.on_status_reply is not None:
            tml.tml_Cmd_Register_StatusReply(cmd.command_handle, on_cmd_status_reply_handler, self)
        tml.tml_Bal_Send_SyncMessage(self.__tml_core.core_handle, cmd.command_handle, self.__profile, timeout)

    ## @brief Send an asynchronous TML message to call destinations.
    #
    #  The command is using the next call steination acoording to the
    #  balancing rules.
    #
    #  @param cmd     : [\b tml.command.TMLCmd] command to send
    #  @param timeout : [\b Integer][\em optional] timeout in milliseconds (default: 5000)
    #
    #  @sa tml_Bal_Send_AsyncMessage()
    def call_async_balanced(self, cmd, timeout=5000):
        self.__pending_commands[cmd.command_handle] = cmd
        if self.on_progress is not None:
            tml.tml_Cmd_Register_Progress(cmd.command_handle, on_cmd_progress_handler, self)
        if self.on_status_reply is not None:
            tml.tml_Cmd_Register_StatusReply(cmd.command_handle, on_cmd_status_reply_handler, self)
        tml.tml_Cmd_Register_CommandReady(cmd.command_handle, on_profile_command_ready_handler, self)
        tml.tml_Bal_Send_AsyncMessage(self.__tml_core.core_handle, cmd.command_handle, self.__profile, timeout)

    ## @brief Send an event to all event consumers registered for this profile.
    #
    #  Events are handled like regular command calls on the receiver peers, but
    #  not return values are expected. The command is added to the sender queue
    #  handled by TML. There is no guarantee that the event will be received by a
    #  specific event consumer.
    #
    #  @param cmd     : [\b tml.command.TMLCmd] command to send
    #
    #  @sa TMLCore.max_queued_events, tml_Evt_Send_Message()
    def send_event(self, cmd):
        tml.tml_Evt_Send_Message(self.__tml_core.core_handle, cmd.command_handle, self.__profile)

    ## @brief Open a sender stream.
    #
    #  The sender stream provides access to stream data. The concrete class
    #  implementation can be customized by implementing a subclass of tml.steam.TMLAbstractSndStream.
    #
    #  @param aclass : [\b tml.stream.TMLAbstractSndStream class] sender stream object class
    #  @param *args  : [\b unpacked \b tuple] variable number of parameters passed to the constructor of the
    #                                      sender stream class
    #
    #  @returns [\b tml.stream.TMLAbstractSndStream] instance of a sender stream.
    #
    #  @sa tml.stream.TMLAbstractSndStream, open_stream_receiver()
    def open_stream_sender(self, aclass, *args):
        if issubclass(aclass, tml.stream.TMLAbstractSndStream):
            lstream = aclass(self, *args)
            self.__snd_streams[lstream.stream_id] = lstream

            return lstream
        else:
            raise TypeError("invalid class type %s" % aclass.__name__)

    ## @brief Open a receiver stream.
    #
    #  A receiver stream object is connected to a sender stream and used to control the
    #  data flow. Passing a special class implementation is optional.
    #
    #  @param iid     : [\b int] unique stream id
    #  @param address : [\b str] <host>:<port> address of the peer providing the stream
    #  @param *args   : [\b unpacked \b tuple] variable number of parameters passed to the constructor of the
    #                                      receiver stream class
    #  @param aclass  : [\b tml.stream.TMLRecStream class][\em optional] receiver stream object class
    #
    #  @returns [\b tml.stream.TMLRecStream] instance of a receiver stream.
    #
    #  @sa open_stream_sender(), tml.stream.TMLRecStream
    def open_stream_receiver(self, iid, address, aclass=tml.stream.TMLRecStream, *args):
        if issubclass(aclass, tml.stream.TMLRecStream):
            lstream = aclass(self, iid, address, *args)
            self.__rec_streams[lstream.stream_id] = lstream
            return lstream
        else:
            raise TypeError("invalid class type %s" % aclass.__name__)

    # --------------------- internal methods -------------------------

# @cond INTERNAL

    ## @brief Get a pending command.
    #
    #  @param command_handle :[\b int] TML command handle
    #
    #  @returns [\b tml.command.TMLCmd] pending command
    def get_pending_command(self, command_handle):
        return self.__pending_commands[command_handle]

    ## @brief Check if a pending command object exists for a handle.
    #
    #  @param command_handle :[\b int] TML command handle
    #
    #  @returns [\b tml.command.TMLCmd] pending command
    def has_pending_command(self, command_handle):
        return command_handle in self.__pending_commands

    ## @brief Delete a pending command.
    #
    #  @param command_handle :[\b int] TML command handle
    def del_pending_command(self, command_handle):
        del self.__pending_commands[command_handle]

    ## @brief Get command handler callback.
    #
    #  @para cmdid : [\b int] command id
    def get_cmd_handler(self, cmdid):
        return self.__commands[cmdid]

    def del_snd_stream(self, iid):
        del self.__snd_streams[iid]

    def del_rec_stream(self, iid):
        del self.__rec_streams[iid]

    def close(self, ashutdown=False):
        if self.tml_core is not None:
            for lstream in self.__snd_streams.values():
                lstream.close(True)
            self.__snd_streams.clear()

            for lstream in self.__rec_streams.values():
                lstream.close(True)
            self.__rec_streams.clear()
            if not ashutdown:
                del self.__tml_core.profiles[self.profile_url]
            self.__tml_core = None

    #  @endcond


## @brief Handle a list of profiles.
#  @ingroup TMLProfileGroup
#
#  The TML profiles are interfaces referenced by a string ID.
#  The TMLProfiles class simplifies management of multiple interface.
class TMLProfiles(object):
    ## @brief constructor of TMLProfiles
    #
    #  @param hcore   :[\b tml.core.TMLCore] TML core
    def __init__(self, core):
        self.__tml_core = weakref.proxy(core)
        #self.__tml_core = core
        self.__profiles = {}
        #self.__tml_core = core

    ## @brief TMLProfiles destructor.
    def __del__(self):
        pass

    ## @brief Add a new profile to the TML core.
    #
    #  @param profile_id :[\b str] profile id to add
    #
    #  @returns [\b TMLProfile] new profile
    def add_profile(self, profile_id):
        if profile_id in self.__profiles:
            raise KeyError("profile %s already exists" % profile_id)
        else:
            tml.tml_Profile_Register(self.__tml_core.core_handle, profile_id)
            self.__profiles[profile_id] = TMLProfile(profile_id, self.__tml_core)
            return self.__profiles[profile_id]

    ## @brief Get a profile by profile id.
    #
    #  @param key : [\b str] profile id
    #
    #  @return [\b TMLProfile] profile object
    #
    #  @exception KeyError profile does not exist
    def __getitem__(self, key):
        return self.__profiles[key]

    ## @brief Unregister a profile
    #
    #  @param key : [\b str] profile id
    #
    #  @exception KeyError profile does not exist
    #
    #  @sa tml_Profile_Unregister()
    def __delitem__(self, key):
        tml.tml_Profile_Unregister(self.__tml_core.core_handle, key)
        self.__profiles[key].close(True)
        del self.__profiles[key]

    ## @brief Get profile count.
    #
    #  @return [\b int] number of registered profiles
    def __len__(self):
        return len(self.__profiles)

    ## @brief Get a list of registered profile ids.
    #
    #  @return [\b list] registered profile ids
    def keys(self):
        return self.__profiles.keys()

    ## @brief Get a list of profile objects.
    #
    #  @return [\b list] registered profiles
    def values(self):
        return self.__profiles.values()

    ## @brief Unregister all profiles.
    #
    #  @sa TMLProfiles.__delitem__()
    def clear(self):
        for profile in self.__profiles.keys():
            tml.tml_Profile_Unregister(self.__tml_core.core_handle, profile)
            self.__profiles[profile].close(True)

        self.__profiles.clear()

    # @cond INTERNAL
    def close(self, ashutdown=False):
        self.__tml_core = None
    #  @endcond


