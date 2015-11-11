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

## package  TML
#

## @defgroup tmlGroup TML API
#  @ingroup basicAPI
#  @brief Description of TML API functions.

## @ingroup tmlGroup
#  @defgroup coreGeneral General functions
#  @brief General library functions
#

## @ingroup tmlGroup
#  @defgroup coreHandle TMLCore
#  @brief TMLCore functions
#
#  All communications in TML is related to a TMLCore object. At least one
#  TMLCore must exist before any TML communication is possible.

## @ingroup coreHandle
# @defgroup coreManagement Listener management
# @brief TMLCore listener management
#
# A TMLCore listener must be started and initialized to enable incoming traffic.
#

## @ingroup coreHandle
# @defgroup dispatching Profile management
# @brief TMLCore profile
#
# A profile is linked to a TMLCore and diapatches incoming traffic to command handling functions.
#

## @ingroup coreHandle
# @defgroup dataIO Sending commands
# @brief Using the TMLCore to send commands.
#

## @ingroup coreHandle
# @defgroup eventIO Event handling
# @brief Sending of events to multiple destinations.
#
# Sending event messages to multiple peers without a reply.
#

## @ingroup coreHandle
# @defgroup loadbalancedIO Load balancing
# @brief Balance command calls.
#
# For backup and load balancing TML provides an API to call a group of peers instead of a specific peer to
# process a command. If one peer fails the remaining will still handle the command.
#

## @ingroup tmlGroup
#  @defgroup tmlHandle TML commands
#  @brief Creating and handling of TML commands.
#
#  TML commands contain data and header information. The data can be set and read using
#  the SIDEX API. All SIDEX datatypes can be used to be exchanged through a TML command call.
#

## @ingroup tmlHandle
#  @defgroup tmlCmdCallbacks Command message callbacks
#  @brief Callback methods for multiple answers and asynchronous reply handling.
#
#  If commands are called asynchronously or status/progress replies are used, callbacks can
#  be registered for a command to handle this information.
#

## @ingroup tmlHandle
#  @defgroup tmlheader Accessing header information
#  @brief Functions to access command header information.

## @ingroup coreHandle
# @defgroup streamIO Stream communication
# @brief Using TML streams.
#
# The stream API enables TML to transfer files without using another port or protocol.
#

## @ingroup tmlGroup
#  @defgroup tmlCallback TML callback functions
#  @brief Definition of callback functions used in TML API.
#
#  TML API is using various callback functions to support asynchronous and threaded functionality.
#  The names in this module are for reference only. There is no function with this name that can be called.
#  A function with the described interface has to be implemented according to the parameter definition and
#  registered with the corresponding API functions.

## @ingroup  coreGeneral
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
def tml_Core_Get_Version(): pass

## @ingroup coreGeneral
#  @brief   Returns the copyright information.
#
#  The copyright information contains all hints and links concerning the LGPL related
#  libraries used in this SDK.
#
#  @return  [\b str] copyright information
#
#  @see tml_Core_Get_Version()
def tml_Core_Get_Copyright(): pass

## @ingroup  coreGeneral
#  @brief   Get current log value.
# 
#  @param   tmlcore   : [\b int] TML core handle (TML_CORE_HANDLE)
# 
#  @returns  [\b int] debug log value
# 
#  @see tml_Core_Set_LoggingValue()
def tml_Core_Get_LoggingValue(tmlcore): pass

## @ingroup  coreGeneral
#  @brief  Set the debug log value.
#
#  <TABLE>
#   <TR><TD><B>Name       </B></TD><TD><B>Value </B></TD><TD><B>Description  </B></TD></TR>
#   <TR><TD>TML_LOG_OFF                </TD><TD>0x0</TD><TD>no debug log</TD></TR>
#   <TR><TD>TML_LOG_VORTEX_CMD         </TD><TD>0x4</TD><TD>log API commands to vortex library</TD></TR>
#   <TR><TD>TML_LOG_CORE_IO            </TD><TD>0x8</TD><TD>core I/O logging</TD></TR>
#   <TR><TD>TML_LOG_CORE_API           </TD><TD>0x10</TD><TD>log core API</TD></TR>
#   <TR><TD>TML_LOG_VORTEX_FRAMES      </TD><TD>0x20</TD><TD>vortex frame logging</TD></TR>
#   <TR><TD>TML_LOG_VORTEX_CH_POOL     </TD><TD>0x40</TD><TD>vortex channel pool logging</TD></TR>
#   <TR><TD>TML_LOG_VORTEX_MUTEX       </TD><TD>0x80</TD><TD>vortex mutex logging</TD></TR>
#   <TR><TD>TML_LOG_MULTI_SYNC_CMDS    </TD><TD>0x100</TD><TD>vortex multi sync cmds</TD></TR>
#   <TR><TD>TML_LOG_INTERNAL_DISPATCH  </TD><TD>0x800</TD><TD>internal command dispatcher log</TD></TR>
#   <TR><TD>TML_LOG_STREAM_HANDLING    </TD><TD>0x2000</TD><TD>stream handling log</TD></TR>
#   <TR><TD>TML_LOG_EVENT              </TD><TD>0x8000</TD><TD>event logging</TD></TR>
#  </TABLE>
#
#  @param   tmlcore   : [\b int] TML core handle (TML_CORE_HANDLE)
#  @param   iLogValue : [\b int] debug log value
#
#  @see tml_Core_Get_LoggingValue()
def tml_Core_Set_LoggingValue(tmlcore, iLogValue): pass

## @ingroup coreGeneral
#  @brief    Set the license key.
# 
#  Before the sdk functions can be used a license key must be applied.
# 
#  @param    pUserName : [\b str] user name (case sensitive)
#  @param    pPassWord : [\b str] password (case insensitive)
def tml_Core_Set_Password(pUserName, pPassWord): pass

## @ingroup  coreHandle
#  @brief   Open an instance of a TMLCore.
# 
#  @param   iLogValue : [\b int] debug log value
# 
#  @returns [\b int] TML core handle (TML_CORE_HANDLE)
#
# @see tml_Core_Set_LoggingValue()
def tml_Core_Open(iLogValue): pass

## @ingroup  coreHandle
#  @brief  Close and free a TML core.
# 
#  @param   tmlcore   : [\b int] TML core handle (TML_CORE_HANDLE)
#
#  @see tml_Core_Open()
def tml_Core_Close(tmlcore): pass

## @ingroup  coreManagement
#  @brief   Get listener status.
# 
#  @param   tmlcore   : [\b int] TML core handle (TML_CORE_HANDLE)
# 
#  @returns [\b bool] TRUE listener enabled, FALSE listener disabled
# 
#  @see tml_Core_Set_ListenerEnabled()
def tml_Core_Get_ListenerEnabled(tmlcore): pass

## @ingroup  coreManagement
#  @brief   Get the listener interface (IP).
# 
#  @param   tmlcore   : [\b int] TML core handle (TML_CORE_HANDLE)
# 
#  @returns [\b str] listener IP or hostname
# 
#  @see tml_Core_Set_ListenerIP()
def tml_Core_Get_ListenerIP(tmlcore): pass

## @ingroup  coreManagement
#  @brief   Get the listener port number.
# 
#  @param   tmlcore   : [\b int] TML core handle (TML_CORE_HANDLE)
# 
#  @returns [\b str] listener port
#
#  @see tml_Core_Set_ListenerPort() 
def tml_Core_Get_ListenerPort(tmlcore): pass

## @ingroup  coreManagement
#  @brief   Enable or disable the TML listener.
# 
#  @note The interface IP and port have to be configured before enabling the listener.
# 
#  @param   tmlcore   : [\b int] TML core handle (TML_CORE_HANDLE)
#  @param   benable   : [\b bool] TRUE to enable, FALSE to disable the listener
# 
#  @see tml_Core_Get_ListenerEnabled()
def tml_Core_Set_ListenerEnabled(tmlcore, benable): pass

## @ingroup  coreManagement
#  @brief   Set listener interface (IP)
# 
#  The listener can be assigned to a specific network interface or to all network interfaces available ("0.0.0.0) 
#  which is the default.
# 
#  @param   tmlcore   : [\b int] TML core handle (TML_CORE_HANDLE)
#  @param   sip       : [\b str] interface IP or hostname
# 
#  @see tml_Core_Get_ListenerIP()
def tml_Core_Set_ListenerIP(tmlcore, sip): pass

## @ingroup  coreManagement
#  @brief  Set listener port.
# 
#  @param   tmlcore   : [\b int] TML core handle (TML_CORE_HANDLE)
#  @param   sport     : [\b str] port number, 0 = random port number, default 44000
#
#  @see tml_Core_Get_ListenerPort()
def tml_Core_Set_ListenerPort(tmlcore, sport): pass

## @ingroup  dispatching
#  @brief   Get a list of registered profile names.
# 
#  @param   tmlcore   : [\b int] TML core handle (TML_CORE_HANDLE)
#
#  @returns [\b list] a list of registered profiles
#
#  @see tml_Profile_Get_Registered_Count() 
def tml_Profile_Get_Registered(tmlcore): pass

## @ingroup  dispatching
#  @brief   Get the number of registered profiles.
# 
#  @param   tmlcore   : [\b int] TML core handle (TML_CORE_HANDLE)
#
#  @returns [\b int] number of registered profiles
#
#  @see tml_Profile_Get_Registered() 
def tml_Profile_Get_Registered_Count(tmlcore): pass

## @ingroup  dispatching
#  @brief   Check if a profile name is already registered.
# 
#  @param   tmlcore   : [\b int] TML core handle (TML_CORE_HANDLE)
#  @param   sprofile  : [\b str] profile name
#
#  @returns [\b bool] returns true if the profile is already registered
#
#  @see tml_Profile_Register()
def tml_Profile_Get_RegisterState(tmlcore, sprofile): pass

## @ingroup  dispatching
#  @brief   Register a profile.
# 
#  Before any other operation referring to a profile is accepted by the TML core, the profile
#  has to be registered.
# 
#  @param   tmlcore   : [\b int] TML core handle (TML_CORE_HANDLE)
#  @param   sprofile  : [\b str] profile name
#
#  @see tml_Profile_Get_RegisterState() 
def tml_Profile_Register(tmlcore, sprofile): pass

## @ingroup tmlCmdCallbacks
#  @brief Command handler function
#
#  @param   cmd       : [\b int] TML command handle
#  @param pcbdata     : [\b object] registered data
#
#  @see tml_Profile_Register_Cmd() 
def TML_ON_CMD_DISPATCH_CB_FUNC(cmd, pcbdata): pass

## @ingroup  dispatching
#  @brief   Register a command handler function.
# 
#  @param   tmlcore   : [\b int] TML core handle (TML_CORE_HANDLE)
#  @param   sprofile  : [\b str] profile name
#  @param   cmdid     : [\b int] command id
#  @param   pcbfunc   : [\b function] callback function TML_ON_CMD_DISPATCH_CB_FUNC() 
#                                     or None to unregister a previously registered function
#  @param   pcbdata   : [\b object] data to be registered with the function
#
#  @see tml_Profile_Set_OnDeleteCmd(), TML_ON_CMD_DISPATCH_CB_FUNC()
def tml_Profile_Register_Cmd(tmlcore, sprofile, cmdid, pcbfunc, pcbdata): pass


## @ingroup tmlCmdCallbacks
#  @brief General Command handler function called if no special command handler was registered.
#
#  @param cmdid       : [\b int] command id
#  @param cmd         : [\b int] TML command handle
#  @param pcbdata     : [\b object] registered data
#
#  @see tml_Profile_Register_Cmd() 
def TML_ON_CMD_CUSTOM_DISPATCH_CB_FUNC(cmdid, cmd, pcbdata): pass

## @ingroup  dispatching
#  @brief   Register a general command handler.
# 
#  If no command handler was registered for a command id, the profile will call the general command handler
#  if registered. This can be used to implement an own dispatching mechanism to handle incoming commands and events.<br>
#  Read TML_ON_CMD_CUSTOM_DISPATCH_CB_FUNC() for further reference.
#
#  @param   tmlcore   : [\b int] TML core handle (TML_CORE_HANDLE)
#  @param   sprofile  : [\b str] profile name
#  @param   pcbfunc   : [\b function] callback function TML_ON_CMD_CUSTOM_DISPATCH_CB_FUNC() or 
#                                     None to unregister a previously registered function
#  @param   pcbdata   : [\b object] data to be registered with the function
#
#  @see TML_ON_CMD_CUSTOM_DISPATCH_CB_FUNC() 
def tml_Profile_Set_OnCustomDispatch(tmlcore, sprofile, pcbfunc, pcbdata): pass

## @ingroup tmlCmdCallbacks
#  @brief Signal command handler deregistration.
#
#  @param cmdid       : [\b int] command id
#  @param cmdcbdata   : [\b object] custom data registered for the command
#  @param pcbdata     : [\b object] registered data for delete handler
#
#  @see tml_Profile_Register_Cmd() 
def TML_ON_CMD_DELETION_CB_FUNC(cmdid, cmdcbdata, pcbdata): pass

## @ingroup  dispatching
#  @brief   Register a callback to signal command handler deregistration.
# 
#  @param   tmlcore   : [\b int] TML core handle (TML_CORE_HANDLE)
#  @param   sprofile  : [\b str] profile name
#  @param   pcbfunc   : [\b function] callback function TML_ON_CMD_DELETION_CB_FUNC() 
#                                     or None to unregister a previously registered function
#  @param   pcbdata   : [\b object] data to be registered with the function
#
#  @see TML_ON_CMD_DELETION_CB_FUNC(), tml_Profile_Register_Cmd() 
def tml_Profile_Set_OnDeleteCmd(tmlcore, sprofile, pcbfunc, pcbdata): pass

## @ingroup  dispatching
#  @brief   Unregister a profile.
# 
#  All command handlers registered for the profile will be removed from the TML core.
#
#  @param   tmlcore   : [\b int] TML core handle (TML_CORE_HANDLE)
#  @param   sprofile  : [\b str] profile name
#
#  @see tml_Profile_Register()
def tml_Profile_Unregister(tmlcore, sprofile): pass

## @ingroup  dataIO
#  @brief    Send an asynchronous message / command.
# 
#  The call returns after sending the message without waiting for a reply. If a result has to be received or 
#  possible error needs to be handled a callback function has to be registered with tml_Cmd_Register_CommandReady()
#  before the call. 
# 
#  @param   tmlcore   : [\b int] TML core handle (TML_CORE_HANDLE)
#  @param   tmlcmd    : [\b int] TML command handle
#  @param   sprofile  : [\b str] profile name
#  @param   shost     : [\b str] host name or IP address
#  @param   sport     : [\b str] port number
#  @param   itimeout  : [\b int] timeout in milliseconds
# 
#  @see tml_Send_SyncMessage(), tml_Bal_Send_SyncMessage(), tml_Bal_Send_AsyncMessage(), tml_Cmd_Register_CommandReady()
def tml_Send_AsyncMessage(tmlcore, tmlcmd, sprofile, shost, sport, itimeout): pass

## @ingroup  dataIO
#  @brief    Send progress reply.
# 
#  A progress reply resets the timeout at the sender side and allows progress monitoring. To read the progress value
#  at the sender a callback function has to be registered with tml_Cmd_Register_Progress().
# 
#  @param   tmlcmd    : [\b int] TML command handle
#  @param   iprogress : [\b int] progress value (0 - 100)
# 
#  @see tml_Cmd_Register_Progress()
def tml_Send_AsyncProgressReply(tmlcmd, iprogress): pass

## @ingroup  dataIO
#  @brief    Send status reply.
# 
#  A status reply sends back messages to the sender. To read the status information
#  at the sender a callback function has to be registered with tml_Cmd_Register_StatusReply().
# 
#  Status reply types:
#   <TABLE>
#    <TR><TD><B>Name       </B></TD><TD><B>Value </B></TD><TD><B>Description  </B></TD></TR>
#    <TR><TD>TMLCOM_RPY_WARNING       </TD><TD>10</TD><TD>warning message reply</TD></TR>
#    <TR><TD>TMLCOM_RPY_ERROR         </TD><TD>20</TD><TD>error message reply</TD></TR>
#    <TR><TD>TMLCOM_RPY_INFORMATION   </TD><TD>30</TD><TD>information message reply</TD></TR>
#   </TABLE> 
# 
#  @param   tmlcmd    : [\b int] TML command handle
#  @param   itype     : [\b int] status reply type
#  @param   smessage  : [\b str]  status reply message
# 
#  @see tml_Cmd_Register_StatusReply()
def tml_Send_AsyncStatusReply(tmlcmd, itype, smessage): pass

## @ingroup  dataIO
#  @brief    Send a synchronous message / command.
# 
#  Sending a message synchronously means that the call returns after the result of the message call
#  was received or an error occurred.
# 
#  @param   tmlcore   : [\b int] TML core handle (TML_CORE_HANDLE)
#  @param   tmlcmd    : [\b int] TML command handle
#  @param   sprofile  : [\b str] profile name
#  @param   shost     : [\b str] host name or IP address
#  @param   sport     : [\b str] port number
#  @param   itimeout  : [\b int] timeout in milliseconds
# 
#  @see tml_Send_AsyncMessage(), tml_Bal_Send_AsyncMessage(), tml_Bal_Send_SyncMessage()
def tml_Send_SyncMessage(tmlcore, tmlcmd, sprofile, shost, sport, itimeout): pass

## @ingroup  eventIO
#  @brief    Get the maximum event connection fail count.
# 
#  The maximum connection fail count defines after how many unsuccessful connections an event consumer is
#  automatically removed from the list. A value of 0 means it is never removed. The default value is 1.
#  
#  @param   tmlcore   : [\b int] TML core handle (TML_CORE_HANDLE)
# 
#  @returns [\b int] maximum fail count
# 
#  @see tml_Evt_Set_MaxConnectionFailCount()
def tml_Evt_Get_MaxConnectionFailCount(tmlcore): pass

## @ingroup  eventIO
#  @brief    Get the maximum value of queued event messages
# 
#  If the maximum number of queued events for an event consumer is reached, the oldest event will be deleted.
#  The default value is 1000.
#  
#  @param   tmlcore   : [\b int] TML core handle (TML_CORE_HANDLE)
# 
#  @returns [\b int] maximum queue count
#
#  @see tml_Evt_Send_Message(), tml_Evt_Set_MaxQueuedEventMessages(), tml_Evt_Set_OnQueueOverflow()
def tml_Evt_Get_MaxQueuedEventMessages(tmlcore): pass

## @ingroup  eventIO
#  @brief    Get event receiver list.
#
#  The function returns a list of dictionaries containing receiver information.
#   <TABLE>
#    <TR><TD><B>Key       </B></TD><TD><B>Description  </B></TD></TR>
#    <TR><TD>PROFILE          </TD><TD>profile id</TD></TR>
#    <TR><TD>HOST             </TD><TD>host name of the receiver</TD></TR>
#    <TR><TD>PORT             </TD><TD>port number of the receiver</TD></TR>
#   </TABLE> 
#
#  @param   tmlcore   : [\b int] TML core handle (TML_CORE_HANDLE)
#  @param   sprofile  : [\b str] profile name
#
#  @return [\b list] a list of dictionaries containing {'PROFILE':/profile/, 'HOST':/host/, 'PORT':/port/}
#
#  @see tml_Evt_Subscribe_MessageDestination()
def tml_Evt_Get_Subscribed_MessageDestinations(tmlcore, sprofile): pass

## @ingroup  eventIO
#  @brief    Send an event to all registered receivers.
# 
#  @param   tmlcore   : [\b int] TML core handle (TML_CORE_HANDLE)
#  @param   tmlcmd    : [\b int] TML command handle
#  @param   sprofile  : [\b str] profile name
#
#  @see tml_Evt_Subscribe_MessageDestination(), tml_Evt_Set_OnPopulate(), tml_Evt_Get_MaxQueuedEventMessages(), tml_Evt_Set_MaxQueuedEventMessages(), tml_Evt_Set_OnQueueOverflow()
def tml_Evt_Send_Message(tmlcore, tmlcmd, sprofile): pass

## @ingroup  eventIO
#  @brief    Send an event subscription request.
# 
#  With this function a command receiver can subscribe itself to receive event messages.
# 
#  @param   tmlcore     : [\b int] TML core handle (TML_CORE_HANDLE)
#  @param   sprofile    : [\b str] profile name
#  @param   ssourcehost : [\b str] host name / IP addresse of the subscriber
#  @param   ssourceport : [\b str] port number of the subscriber
#  @param   sdesthost   : [\b str] host name / IP address of the event producer
#  @param   sdestport   : [\b str] port number of the event producer
#  @param   itimeout    : [\b int] timeout in milliseconds
# 
#  @see tml_Evt_Send_UnsubscriptionRequest()
def tml_Evt_Send_SubscriptionRequest(tmlcore, sprofile, ssourcehost, ssourceport, sdesthost, sdestport, itimeout): pass

## @ingroup  eventIO
#  @brief    Unregister an event message receiver.
# 
#  @param   tmlcore     : [\b int] TML core handle (TML_CORE_HANDLE)
#  @param   sprofile    : [\b str] profile name
#  @param   ssourcehost : [\b str] host name / IP addresse of the subscriber
#  @param   ssourceport : [\b str] port number of the subscriber
#  @param   sdesthost   : [\b str] host name / IP address of the event producer
#  @param   sdestport   : [\b str] port number of the event producer
#  @param   itimeout    : [\b int] timeout in milliseconds
# 
#  @see tml_Evt_Send_SubscriptionRequest()
def tml_Evt_Send_UnsubscriptionRequest(tmlcore, sprofile, ssourcehost, ssourceport, sdesthost, sdestport, itimeout): pass

## @ingroup  eventIO
#  @brief    Set the maximum event connection fail count.
# 
#  The maximum connection fail count defines after how many unsuccessful connections an event consumer is
#  automatically removed from the list. A value of 0 means it is never removed. The default value is 1.
#  
#  @param   tmlcore   : [\b int] TML core handle (TML_CORE_HANDLE)
#  @param   icount    : [\b int] maximum fail count
# 
#  @see tml_Evt_Get_MaxConnectionFailCount()
def tml_Evt_Set_MaxConnectionFailCount(tmlcore, icount): pass

## @ingroup  eventIO
#  @brief    Set the maximum queued events.
# 
#  If the maximum number of queued events for an event consumer is reached, the oldest event will be deleted.
#  The default value is 1000.
#  
#  @param   tmlcore   : [\b int] TML core handle (TML_CORE_HANDLE)
#  @param   icount    : [\b int] maximum queue count
# 
#  @see tml_Evt_Send_Message(), tml_Evt_Get_MaxQueuedEventMessages(), tml_Evt_Set_OnQueueOverflow()
def tml_Evt_Set_MaxQueuedEventMessages(tmlcore, icount): pass

## @ingroup tmlCallback
#  @brief Signal event error.
#
#  @param   sprofile    : [\b str] profile name
#  @param   shost       : [\b str] host name / IP addresse 
#  @param   sport       : [\b str] port number 
#  @param   cmdid       : [\b int] command id
#  @param   ierrorcode  : [\b int] TML error code
#  @param   pcbdata     : [\b object] registered data
#
#  @see tml_Evt_Set_OnError() 
def TML_ON_EVT_ERROR_CB_FUNC(sprofile, shost, sport, cmdid, ierrorcode, pcbdata): pass

## @ingroup  eventIO
#  @brief    Register event error handler.
# 
#  @param   tmlcore   : [\b int] TML core handle (TML_CORE_HANDLE)
#  @param   sprofile  : [\b str] profile name
#  @param   pcbfunc   : [\b function] callback function TML_ON_EVT_ERROR_CB_FUNC() or None to unregister a previously registered function
#  @param   pcbdata   : [\b object] data to be registered with the function
# 
#  @see TML_ON_EVT_ERROR_CB_FUNC()
def tml_Evt_Set_OnError(tmlcore, sprofile, pcbfunc, pcbdata): pass

## @ingroup tmlCallback
#  @brief Subscription / unsubscription request handler
#
#  @param   bSubscribe  : [\b bool] TRUE = subscribe, FALSE = unsubscribe
#  @param   shost       : [\b str] host name / IP addresse 
#  @param   sport       : [\b str] port number 
#  @param   pcbdata     : [\b object] registered data
#
#  @see tml_Evt_Send_SubscriptionRequest(), tml_Evt_Send_UnsubscriptionRequest(), tml_Evt_Set_OnPeerRegister(),
#       tml_Bal_Send_SubscriptionRequest(), tml_Bal_Send_UnsubscriptionRequest(), tml_Bal_Set_OnPeerRegister()
def TML_ON_PEER_REGISTRATION_CB_FUNC(bSubscribe, shost, sport, pcbdata): pass

## @ingroup  eventIO
#  @brief    Register a function to handle an event subscription request.
# 
#  The handler function is called if an event subscription or unsubscription request is received.
# 
#  @param   tmlcore   : [\b int] TML core handle (TML_CORE_HANDLE)
#  @param   sprofile  : [\b str] profile name
#  @param   pcbfunc   : [\b function] callback function TML_ON_PEER_REGISTRATION_CB_FUNC() or None to unregister a previously registered function
#  @param   pcbdata   : [\b object] data to be registered with the function
# 
#  @see tml_Evt_Send_SubscriptionRequest(), tml_Evt_Send_UnsubscriptionRequest(), TML_ON_PEER_REGISTRATION_CB_FUNC()
def tml_Evt_Set_OnPeerRegister(tmlcore, sprofile, pcbfunc, pcbdata): pass

## @ingroup tmlCallback
#  @brief   Callback function to populate event consumer or command receiver lists.
# 
#  @param   sprofile  : [\b str] profile name
#  @param   pcbdata   : [\b object] data to be registered with the function
# 
#  @see tml_Bal_Set_OnPopulate(), tml_Evt_Set_OnPopulate()
def TML_ON_POPULATE_CB_FUNC(sprofile, pcbdata): pass

## @ingroup  eventIO
#  @brief    Register on populate handler.
# 
#  If no event receiver is registered and an event is added to the queue by calling tml_Evt_Send_Message(),
#  the OnPopulate handler can register or register event receivers again.
# 
#  @param   tmlcore   : [\b int] TML core handle (TML_CORE_HANDLE)
#  @param   sprofile  : [\b str] profile name
#  @param   pcbfunc   : [\b function] callback function TML_ON_POPULATE_CB_FUNC() or None to unregister a previously registered function
#  @param   pcbdata   : [\b object] data to be registered with the function
# 
#  @see  tml_Evt_Send_Message(), TML_ON_POPULATE_CB_FUNC()
def tml_Evt_Set_OnPopulate(tmlcore, sprofile, pcbfunc, pcbdata): pass

## @ingroup tmlCallback
#  @brief   Event queue overflow handler.
# 
#  @param   sprofile  : [\b str] profile name
#  @param   cmdid     : [\b int] command/event id
#  @param   pcbdata   : [\b object] data to be registered with the function
# 
#  @see tml_Evt_Set_OnQueueOverflow()
def TML_ON_EVT_QUEUE_OVERFLOW_CB_FUNC(sprofile, cmdid, pcbdata): pass

## @ingroup  eventIO
#  @brief    Register event queue overflow handler.
# 
#  Events are internally queued before they are sent. If the number of queue entries reaches it's maximum
#  the overflow handler is called.
# 
#  @param   tmlcore   : [\b int] TML core handle (TML_CORE_HANDLE)
#  @param   sprofile  : [\b str] profile name
#  @param   pcbfunc   : [\b function] callback function TML_ON_EVT_QUEUE_OVERFLOW_CB_FUNC() or None to unregister a previously registered function
#  @param   pcbdata   : [\b object] data to be registered with the function
# 
#  @see tml_Evt_Get_MaxQueuedEventMessages(), tml_Evt_Set_MaxQueuedEventMessages(), tml_Evt_Send_Message(), TML_ON_EVT_QUEUE_OVERFLOW_CB_FUNC()
def tml_Evt_Set_OnQueueOverflow(tmlcore, sprofile, pcbfunc, pcbdata): pass

## @ingroup  eventIO
#  @brief    Subscribe event receiver.
# 
#  @param   tmlcore   : [\b int] TML core handle (TML_CORE_HANDLE)
#  @param   sprofile  : [\b str] profile name
#  @param   shost     : [\b str] host name / IP addresse 
#  @param   sport     : [\b str] port number 
# 
#  @see tml_Evt_Get_Subscribed_MessageDestinations(), tml_Evt_Unsubscribe_MessageDestination()
def tml_Evt_Subscribe_MessageDestination(tmlcore, sprofile, shost, sport): pass

## @ingroup  eventIO
#  @brief    Unsubscribe all event receivers.
# 
#  @param   tmlcore   : [\b int] TML core handle (TML_CORE_HANDLE)
#  @param   sprofile  : [\b str] profile name <br>
#                       An empty string or None will unsubscribe all event receivers for any registered profile.
# 
#  @see tml_Evt_Unsubscribe_MessageDestination()
def tml_Evt_Unsubscribe_All_MessageDestinations(tmlcore, sprofile): pass

## @ingroup  eventIO
#  @brief    Unsubscribe event receiver.
# 
#  @param   tmlcore   : [\b int] TML core handle (TML_CORE_HANDLE)
#  @param   sprofile  : [\b str] profile name
#  @param   shost     : [\b str] host name / IP addresse 
#  @param   sport     : [\b str] port number 
# 
#  @see tml_Evt_Subscribe_MessageDestination(), tml_Evt_Unsubscribe_All_MessageDestinations()
def tml_Evt_Unsubscribe_MessageDestination(tmlcore, sprofile, shost, sport): pass

## @ingroup  loadbalancedIO
#  @brief    Get maximum load balancing connection fail count.
# 
#  The maximum connection fail count defines after how many unsuccessful connections a command receiver is
#  automatically removed from the list. A value of 0 means it is never removed. The default value is 1.
#  
#  @param   tmlcore   : [\b int] TML core handle (TML_CORE_HANDLE)
# 
#  @returns [\b int] maximum fail count
# 
#  @see tml_Bal_Set_MaxConnectionFailCount()
def tml_Bal_Get_MaxConnectionFailCount(tmlcore): pass

## @ingroup  loadbalancedIO
#  @brief    Get load balanced command receiver list.
# 
#  The function returns a list of dictionaries containing receiver information.
#   <TABLE>
#    <TR><TD><B>Key       </B></TD><TD><B>Description  </B></TD></TR>
#    <TR><TD>PROFILE          </TD><TD>profile id</TD></TR>
#    <TR><TD>HOST             </TD><TD>host name of the receiver</TD></TR>
#    <TR><TD>PORT             </TD><TD>port number of the receiver</TD></TR>
#   </TABLE> 
#
#  @param   tmlcore   : [\b int] TML core handle (TML_CORE_HANDLE)
#  @param   sprofile  : [\b str] profile name
#
#  @return [\b list] a list of dictionaries containing {'PROFILE':/profile/, 'HOST':/host/, 'PORT':/port/}
# 
#  @see tml_Bal_Subscribe_MessageDestination()
def tml_Bal_Get_Subscribed_MessageDestinations(tmlcore, sprofile): pass

## @ingroup  loadbalancedIO
#  @brief    Send a load balanced asynchronous message / command.
# 
#  This call works like tml_Send_AsyncMessage() but sends the command not to specific peer but to
#  a group of registered command receivers utilizing the TML load balancing.
# 
#  @param   tmlcore   : [\b int] TML core handle (TML_CORE_HANDLE)
#  @param   tmlcmd    : [\b int] TML command handle
#  @param   sprofile  : [\b str] profile name
#  @param   itimeout  : [\b int] timeout in milliseconds
# 
#  @see tml_Send_AsyncMessage(), tml_Bal_Send_SyncMessage()
def tml_Bal_Send_AsyncMessage(tmlcore, tmlcmd, sprofile, itimeout): pass

## @ingroup  loadbalancedIO
#  @brief    Send a load balancing subscription request.
# 
#  With this function a command receiver can subscribe itself to receive load balanced commands.
# 
#  @param   tmlcore     : [\b int] TML core handle (TML_CORE_HANDLE)
#  @param   sprofile    : [\b str] profile name
#  @param   ssourcehost : [\b str] host name / IP addresse of the subscriber
#  @param   ssourceport : [\b str] port number of the subscriber
#  @param   sdesthost   : [\b str] host name / IP address of the event producer
#  @param   sdestport   : [\b str] port number of the event producer
#  @param   itimeout    : [\b int] timeout in milliseconds
# 
#  @see tml_Bal_Send_UnsubscriptionRequest()
def tml_Bal_Send_SubscriptionRequest(tmlcore, sprofile, ssourcehost, ssourceport, sdesthost, sdestport, itimeout): pass

## @ingroup  loadbalancedIO
#  @brief    Send a load balanced synchronous message / command.
# 
#  This call works like tml_Send_SyncMessage() but sends the command not to specific peer but to
#  a group of registered command receivers utilizing the TML load balancing.
# 
#  @param   tmlcore   : [\b int] TML core handle (TML_CORE_HANDLE)
#  @param   tmlcmd    : [\b int] TML command handle
#  @param   sprofile  : [\b str] profile name
#  @param   itimeout  : [\b int] timeout in milliseconds
# 
#  @see tml_Send_SyncMessage(), tml_Bal_Send_AsyncMessage()
def tml_Bal_Send_SyncMessage(tmlcore, tmlcmd, sprofile, itimeout): pass

## @ingroup  loadbalancedIO
#  @brief    Unregister a load balancing command receiver.
# 
#  @param   tmlcore     : [\b int] TML core handle (TML_CORE_HANDLE)
#  @param   sprofile    : [\b str] profile name
#  @param   ssourcehost : [\b str] host name / IP addresse of the subscriber
#  @param   ssourceport : [\b str] port number of the subscriber
#  @param   sdesthost   : [\b str] host name / IP address of the event producer
#  @param   sdestport   : [\b str] port number of the event producer
#  @param   itimeout    : [\b int] timeout in milliseconds
# 
#  @see tml_Bal_Send_SubscriptionRequest()
def tml_Bal_Send_UnsubscriptionRequest(tmlcore, sprofile, ssourcehost, ssourceport, sdesthost, sdestport, itimeout): pass

## @ingroup  loadbalancedIO
#  @brief    Set maximum load balancing connection fail count.
# 
#  The maximum connection fail count defines after how many unsuccessful connections a command receiver is
#  automatically removed from the list. A value of 0 means it is never removed. The default value is 1.
#  
#  @param   tmlcore   : [\b int] TML core handle (TML_CORE_HANDLE)
#  @param   icount    : [\b int] maximum queue count
# 
#  @see tml_Bal_Get_MaxConnectionFailCount()
def tml_Bal_Set_MaxConnectionFailCount(tmlcore, icount): pass


## @ingroup tmlCallback
#  @brief   Load balancing busy request handler.
# 
#  @param tmlcmd      : [\b int] TML command handle (TML_COMMAND_HANDLE)
#  @param pcbdata     : [\b object] registered data
# 
#  @returns TML_SUCCESS or error code
# 
#  @see tml_Bal_Set_OnBusyStatusRequest()
def TML_ON_BAL_BUSYSTATUS_REQ_CB_FUNC(tmlcmd, pcbdata): pass

## @ingroup  loadbalancedIO
#  @brief    Register a busy status request handler.
# 
#  To implement a custom load balancing instead of a round robin the status request handler can
#  collect and return host specifc data. The data is added to the TML command passed to the handler function.
#  On sender side the data is collected and passed to a load balance calculation handler (see tml_Bal_Set_OnCalculation()) that
#  will return the index of the next registered peer to call. If either the busy status request handler or the load balancing
#  calculation handler is not implemented round robin will be used.
# 
#  @param   tmlcore   : [\b int] TML core handle (TML_CORE_HANDLE)
#  @param   sprofile  : [\b str] profile name
#  @param   pcbfunc   : [\b function] callback function TML_ON_BAL_BUSYSTATUS_REQ_CB_FUNC() or None to unregister a previously registered function
#  @param   pcbdata   : [\b object] data to be registered with the function
# 
#  @see TML_ON_BAL_BUSYSTATUS_REQ_CB_FUNC(), tml_Bal_Set_OnCalculation()
def tml_Bal_Set_OnBusyStatusRequest(tmlcore, sprofile, pcbfunc, pcbdata): pass

## @ingroup tmlCallback
#  @brief   Calculate next load balancing command receiver.
# 
#  @param   rcount       : [\b int] number of registered receivers
#  @param   rbusystate   : [\b list]  list of TML command handles (integer) with results from tml_Bal_Set_OnBusyStatusRequest()
#  @param   pcbdata      : [\b object] registered data
# 
#  <TABLE>
#   <TR><TD><B>Name</B></TD><TD><B>Type    </B></TD><TD><B>Description          </B></TD></TR>
#   <TR><TD>result</TD><TD>integer </TD><TD>callback result: <br>
#                                           TML_SUCCESS in case of success<br>
#                                           TML_ERR_INFORMATION_UNDEFINED cannot calculate index (fallback to round robin)<br>
#                                           or any other error code </TD></TR>
#   <TR><TD>nextIndex</TD><TD>integer </TD><TD>next command receiver index</TD></TR>
#  </TABLE>
#
#  @return    [\b tuple]  (result, nextIndex)

#  @see tml_Bal_Set_OnCalculation(), tml_Bal_Send_SyncMessage(), tml_Bal_Send_AsyncMessage()
def TML_ON_BAL_CALCULATION_REQ_CB_FUNC(rcount, rbusystate, pcbdata): pass


## @ingroup  loadbalancedIO
#  @brief    Register load balancing calculation handler.
# 
#  Evaluate load information returned by the status request handlers of registered command receivers to calculate
#  the index of the next peer to receive a load balanced command. 
# 
#  @param   tmlcore   : [\b int] TML core handle (TML_CORE_HANDLE)
#  @param   sprofile  : [\b str] profile name
#  @param   pcbfunc   : [\b function] callback function TML_ON_BAL_CALCULATION_REQ_CB_FUNC() or None to unregister a previously registered function
#  @param   pcbdata   : [\b object] data to be registered with the function
# 
#  @see tml_Bal_Send_SyncMessage(), tml_Bal_Send_AsyncMessage(), tml_Bal_Set_OnBusyStatusRequest(), TML_ON_BAL_CALCULATION_REQ_CB_FUNC() 
def tml_Bal_Set_OnCalculation(tmlcore, sprofile, pcbfunc, pcbdata): pass

## @ingroup  loadbalancedIO
#  @brief    Register a function to handle a load balancing subscription request.
# 
#  The handler function is called if a load balancing subscription or unsubscription request is received.
# 
#  @param   tmlcore   : [\b int] TML core handle (TML_CORE_HANDLE)
#  @param   sprofile  : [\b str] profile name
#  @param   pcbfunc   : [\b function] callback function TML_ON_PEER_REGISTRATION_CB_FUNC() or None to unregister a previously registered function
#  @param   pcbdata   : [\b object] data to be registered with the function
# 
#  @see tml_Bal_Send_SubscriptionRequest(), tml_Bal_Send_UnsubscriptionRequest(), TML_ON_PEER_REGISTRATION_CB_FUNC()
def tml_Bal_Set_OnPeerRegister(tmlcore, sprofile, pcbfunc, pcbdata): pass

## @ingroup  loadbalancedIO
#  @brief    Register on populate handler. 
# 
#  If no load balanced command receiver is registered and a load balanced command is requested by tml_Bal_Send_SyncMessage() or tml_Bal_Send_AsyncMessage(), 
#  the OnPopulate handler can register or reregister load balanced command receiver.
# 
#  @param   tmlcore   : [\b int] TML core handle (TML_CORE_HANDLE)
#  @param   sprofile  : [\b str] profile name
#  @param   pcbfunc   : [\b function] callback function TML_ON_POPULATE_CB_FUNC() or None to unregister a previously registered function
#  @param   pcbdata   : [\b object] data to be registered with the function
# 
#  @see tml_Bal_Send_AsyncMessage(), tml_Bal_Send_SyncMessage(), TML_ON_POPULATE_CB_FUNC() 
def tml_Bal_Set_OnPopulate(tmlcore, sprofile, pcbfunc, pcbdata): pass

## @ingroup  loadbalancedIO
#  @brief    Add a command receiver for load balanced calls.
# 
#  @param   tmlcore   : [\b int] TML core handle (TML_CORE_HANDLE)
#  @param   sprofile  : [\b str] profile name
#  @param   shost     : [\b str] host name / IP addresse 
#  @param   sport     : [\b str] port number 
# 
#  @see tml_Bal_Get_Subscribed_MessageDestinations(), tml_Bal_Unsubscribe_MessageDestination() 
def tml_Bal_Subscribe_MessageDestination(tmlcore, sprofile, shost, sport): pass

## @ingroup  loadbalancedIO
#  @brief    Clear load balanced command receiver list.
# 
#  @param   tmlcore   : [\b int] TML core handle (TML_CORE_HANDLE)
#  @param   sprofile  : [\b str] profile name <br>
#                       An empty string or None will unsubscribe all event receivers for any registered profile.
# 
#  @returns TML_SUCCESS in case of success<br>
#           TML_ERR_MISSING_OBJ invalid core handle
# 
#  @see tml_Bal_Unsubscribe_MessageDestination()
def tml_Bal_Unsubscribe_All_MessageDestinations(tmlcore, sprofile): pass

## @ingroup  loadbalancedIO
#  @brief    Unsubscribe load balanced command receiver.
# 
#  @param   tmlcore   : [\b int] TML core handle (TML_CORE_HANDLE)
#  @param   sprofile  : [\b str] profile name
#  @param   shost     : [\b str] host name / IP addresse 
#  @param   sport     : [\b str] port number 
# 
#  @see tml_Bal_Subscribe_MessageDestination, tml_Bal_Unsubscribe_All_MessageDestinations()
def tml_Bal_Unsubscribe_MessageDestination(tmlcore, sprofile, shost, sport): pass

## @ingroup  streamIO
#  @brief    Close a receiver stream.
# 
#  @param   tmlcore   : [\b int] TML core handle (TML_CORE_HANDLE)
#  @param   iid       : [\b int] stream identifiation.
#  @param   bretainopen : [\b bool] If this value is TRUE the stream can be opened again with a call to tml_RecStream_Open().
# 
#  @see tml_RecStream_Open()
def tml_RecStream_Close(tmlcore, iid, bretainopen): pass

## @ingroup tmlCallback
#  @brief   Download stream processing handler.
# 
#  @param   iid            : [\b int]  stream identifiation
#  @param   pCBDataDld     : [\b object] buffer reception handler data
#  @param   buffer         : [\b str]   data buffer
#  @param   bytesRead      : [\b int]  number of bytes to read
#  @param   totalBytesRead : [\b int]  number of bytes really read
#  @param   streamSize     : [\b int]  size of the stream
# 
#  @returns [\b int]  0 on success / -1 if not operable, / -2 if operation was canceled
# 
#  @see tml_RecStream_DownloadData()
def TML_ON_REC_STRM_DLD_BLOCK_CB_FUNC(iid, pCBDataDld, buffer, bytesRead, totalBytesRead, streamSize): pass

## @ingroup tmlCallback
#  @brief  Download stream finished handler.
# 
#  @param   iid              : [\b int]  stream identifiation
#  @param   errCode          : [\b int]  TML_SUCCESS on success or error code
#  @param   pCBDataDldFinish : [\b object] download finished handler data
# 
#  @see tml_RecStream_DownloadData()
def TML_ON_REC_STRM_DLD_FINISHED_CB_FUNC(iid, errCode, pCBDataDldFinish): pass

## @ingroup  streamIO
#  @brief    Start a full download of a stream.
# 
#  To download a full stream two handler functions are used. One to handle the data buffer by buffer and
#  one to finish the download. Data can be passed for both handlers referring to object instances or records
#  used by the download. Depending on buffer and stream size the handlers are automatically called to
#  handle the download. A call to this function returns after the download is finished or an error occured.
# 
#  @param   tmlcore    : [\b int] TML core handle (TML_CORE_HANDLE)
#  @param   iid        : [\b int] stream identifiation.
#  @param   buffersize : [\b int] size of download buffer
#  @param   pcbfuncdld : [\b function] callback function TML_ON_REC_STRM_DLD_BLOCK_CB_FUNC()
#  @param   pcbdatadld : [\b object] data to be registered with the function
#  @param   pcbfuncfin : [\b function] callback function TML_ON_REC_STRM_DLD_FINISHED_CB_FUNC()
#  @param   pcbdatafin : [\b object] data to be registered with the function
#
#  @see TML_ON_REC_STRM_DLD_BLOCK_CB_FUNC(), TML_ON_REC_STRM_DLD_FINISHED_CB_FUNC(), tml_RecStream_Read()
def tml_RecStream_DownloadData(tmlcore, iid, buffersize, pcbfuncdld, pcbdatadld, pcbfuncfin, pcbdatafin): pass

## @ingroup  streamIO
#  @brief    Get stream position.
# 
#  @param   tmlcore    : [\b int] TML core handle (TML_CORE_HANDLE)
#  @param   iid        : [\b int] stream identifiation.
# 
#  @returns [\b int] stream position
# 
#  @see tml_RecStream_Seek(), tml_SndStream_Register_GetPosition()
def tml_RecStream_GetPosition(tmlcore, iid): pass

## @ingroup  streamIO
#  @brief    Get the size of a stream.
# 
#  @param   tmlcore    : [\b int] TML core handle (TML_CORE_HANDLE)
#  @param   iid        : [\b int] stream identifiation.
# 
#  @returns [\b int] stream size
# 
#  @see tml_SndStream_Register_GetSize()
def tml_RecStream_GetSize(tmlcore, iid): pass

## @ingroup  streamIO
#  @brief    Open a receiver stream.
# 
#  @param   tmlcore    : [\b int] TML core handle (TML_CORE_HANDLE)
#  @param   iid        : [\b int] stream identifiation.
#  @param   sprofile   : [\b str]  profile name
#  @param   shost      : [\b str]  The destination host name / IP number.
#  @param   sport      : [\b str]  The destination Port.
# 
#  @see tml_SndStream_Open(), tml_RecStream_Close()
def tml_RecStream_Open(tmlcore, iid, sprofile, shost, sport): pass

## @ingroup  streamIO
#  @brief    Read data from a stream.
# 
#  @param   tmlcore    : [\b int] TML core handle (TML_CORE_HANDLE)
#  @param   iid        : [\b int] stream identifiation.
#  @param   icount     : [\b int] number of bytes to read
# 
#  @returns [\b str] read buffer
# 
#  @see tml_RecStream_Write(), tml_RecStream_ReadBuffer(), tml_SndStream_Register_Read(), tml_RecStream_DownloadData()
def tml_RecStream_Read(tmlcore, iid, icount): pass

## @ingroup  streamIO
#  @brief    Read data from a stream.
# 
#  This function reads the number of bytes like tml_RecStream_Read(). If the 
#  stream ends before all bytes are read, an error will be raised.
# 
#  @param   tmlcore    : [\b int] TML core handle (TML_CORE_HANDLE)
#  @param   iid        : [\b int] stream identifiation.
#  @param   icount     : [\b int] number of bytes to read
# 
#  @returns [\b str] read buffer
# 
#  @see tml_RecStream_Read(), tml_SndStream_Register_Read()
def tml_RecStream_ReadBuffer(tmlcore, iid, icount): pass

## @ingroup  streamIO
#  @brief    Move to stream position (seek).
# 
#  Values for offset origin are:
#   <TABLE>
#    <TR><TD><B>Name       </B></TD><TD><B>Value</B></TD><TD><B>Description  </B></TD></TR>
#    <TR><TD>soFromBeginning       </TD><TD>0</TD><TD>from start of stream (>0)</TD></TR>
#    <TR><TD>soFromCurrent         </TD><TD>1</TD><TD>from current position</TD></TR>
#    <TR><TD>soFromEnd             </TD><TD>2</TD><TD>from end of stream (<0)</TD></TR>
#   </TABLE> 
# 
#  @param   tmlcore    : [\b int] TML core handle (TML_CORE_HANDLE)
#  @param   iid        : [\b int] stream identifiation.
#  @param   seekpos    : [\b int] position offset
#  @param   origin     : [\b int] offset origin
# 
#  @see tml_SndStream_Register_Seek(), tml_RecStream_GetPosition()
def tml_RecStream_Seek(tmlcore, iid, seekpos, origin): pass

## @ingroup  streamIO
#  @brief    Write data to a stream.
# 
#  @param   tmlcore    : [\b int] TML core handle (TML_CORE_HANDLE)
#  @param   iid        : [\b int] stream identifiation.
#  @param   buffer     : [\b str]  buffer
# 
#  @see tml_RecStream_Read(), tml_SndStream_Register_Write()
def tml_RecStream_Write(tmlcore, iid, buffer): pass

## @ingroup  streamIO
#  @brief    Close a sender stream.
# 
#  @param   tmlcore    : [\b int] TML core handle (TML_CORE_HANDLE)
#  @param   iid        : [\b int] stream identifiation.
# 
#  @see tml_SndStream_Open(), tml_SndStream_Register_Close()
def tml_SndStream_Close(tmlcore, iid): pass

## @ingroup  streamIO
#  @brief    Open a sender stream.
# 
#  Open a stream and aqcuire a clear stream id to be used in all stream operations.
# 
#  @param   tmlcore    : [\b int] TML core handle (TML_CORE_HANDLE)
#  @param   sprofile   : [\b str]  profile name
#  @param   shost      : [\b str]  destination host name or IP address
#  @param   sport      : [\b str]  destination port number
# 
#  @returns  [\b int] stream identifiation
# 
#  @see tml_SndStream_Close()
def tml_SndStream_Open(tmlcore, sprofile, shost, sport): pass

## @ingroup tmlCallback
#  @brief   Stream close handler.
# 
#  @param   iid        : [\b int] stream identifiation.
#  @param   pcbdata    : [\b object] custom data or None
# 
#  @see tml_SndStream_Register_Close() 
def TML_ON_SND_STRM_CLOSE_CB_FUNC(iid, pcbdata): pass

## @ingroup  streamIO
#  @brief    Register close stream handler.
# 
#  @param   tmlcore    : [\b int] TML core handle (TML_CORE_HANDLE)
#  @param   iid        : [\b int] stream identifiation.
#  @param   pcbfunc    : [\b function] callback function TML_ON_SND_STRM_CLOSE_CB_FUNC() or None to unregister a previously registered function
#  @param   pcbdata    : [\b object] data to be registered with the function
# 
#  @see tml_SndStream_Close(), TML_ON_SND_STRM_CLOSE_CB_FUNC()
def tml_SndStream_Register_Close(tmlcore, iid, pcbfunc, pcbdata): pass

## @ingroup  tmlCallback
#  @brief   Get stream position handler.
# 
#  @param   iid        : [\b int] stream identifiation.
#  @param   pcbdata    : [\b object] custom data or None
# 
#  @returns [\b int] current stream position
# 
#  @see tml_SndStream_Register_GetPosition()
def TML_ON_SND_STRM_GET_POSITION_CB_FUNC(iid, pcbdata): pass

## @ingroup  streamIO
#  @brief    Register get position handler.
# 
#  @param   tmlcore    : [\b int] TML core handle (TML_CORE_HANDLE)
#  @param   iid        : [\b int] stream identifiation.
#  @param   pcbfunc    : [\b function] callback function TML_ON_SND_STRM_GET_POSITION_CB_FUNC() or None to unregister a previously registered function
#  @param   pcbdata    : [\b object] data to be registered with the function
# 
#  @see tml_RecStream_GetPosition(), TML_ON_SND_STRM_GET_POSITION_CB_FUNC()
def tml_SndStream_Register_GetPosition(tmlcore, iid, pcbfunc, pcbdata): pass

## @ingroup  tmlCallback
#  @brief   Get stream size handler.
# 
#  @param   iid        : [\b int] stream identifiation.
#  @param   pcbdata    : [\b object] custom data or None
# 
#  @returns [\b int] size of the stream
# 
#  @see tml_SndStream_Register_GetSize()
def TML_ON_SND_STRM_GET_SIZE_CB_FUNC(iid, pcbdata): pass

## @ingroup  streamIO
#  @brief    Register get stream size handler.
# 
#  @param   tmlcore    : [\b int] TML core handle (TML_CORE_HANDLE)
#  @param   iid        : [\b int] stream identifiation.
#  @param   pcbfunc    : [\b function] callback function TML_ON_SND_STRM_GET_SIZE_CB_FUNC() or None to unregister a previously registered function
#  @param   pcbdata    : [\b object] data to be registered with the function
# 
#  @see tml_RecStream_GetSize(), TML_ON_SND_STRM_GET_SIZE_CB_FUNC()
def tml_SndStream_Register_GetSize(tmlcore, iid, pcbfunc, pcbdata): pass

## @ingroup  tmlCallback
#  @brief   Stream error handler.
# 
#  @param   iid        : [\b int] stream identifiation.
#  @param   ierror     : [\b int] error code
#  @param   pcbdata    : [\b object] custom data or None
# 
#  @see tml_SndStream_Register_OnError()
def TML_ON_SND_STRM_COM_FAULT_CB_FUNC(iid, ierror, pcbdata): pass

## @ingroup  streamIO
#  @brief    Register streaming error handler.
# 
#  @param   tmlcore    : [\b int] TML core handle (TML_CORE_HANDLE)
#  @param   iid        : [\b int] stream identifiation.
#  @param   pcbfunc    : [\b function] callback function TML_ON_SND_STRM_COM_FAULT_CB_FUNC() or None to unregister a previously registered function
#  @param   pcbdata    : [\b object] data to be registered with the function
# 
#  @see TML_ON_SND_STRM_COM_FAULT_CB_FUNC(), tml_SndStream_Open()
def tml_SndStream_Register_OnError(tmlcore, iid, pcbfunc, pcbdata): pass

## @ingroup  tmlCallback
#  @brief   Stream read handler.
# 
#  @param   iid        : [\b int]  stream identifiation.
#  @param   pcbdata    : [\b object] custom data or None
#  @param   count      : [\b int]  number of bytes to read
# 
#  <TABLE>
#   <TR><TD><B>Name</B></TD><TD><B>Type    </B></TD><TD><B>Description          </B></TD></TR>
#   <TR><TD>result</TD><TD>boolean </TD><TD>callback result:<br>
#                                           TRUE on success<br>
#                                           FALSE on error</TD></TR>
#   <TR><TD>readBuffer</TD><TD>string </TD><TD> buffer containing the bytes read</TD></TR>
#  </TABLE>
#
#  @return    [\b tuple]  (result, readBuffer)
# 
#  @see tml_SndStream_Register_Read()
def TML_ON_SND_STRM_READ_CB_FUNC(iid, pcbdata, count): pass

## @ingroup  streamIO
#  @brief    Register read stream handler.
# 
#  @param   tmlcore    : [\b int] TML core handle (TML_CORE_HANDLE)
#  @param   iid        : [\b int] stream identifiation.
#  @param   pcbfunc    : [\b function] callback function TML_ON_SND_STRM_READ_CB_FUNC() or None to unregister a previously registered function
#  @param   pcbdata    : [\b object] data to be registered with the function
# 
#  @see tml_RecStream_Read(), TML_ON_SND_STRM_READ_CB_FUNC()
def tml_SndStream_Register_Read(tmlcore, iid, pcbfunc, pcbdata): pass

## @ingroup  tmlCallback
#  @brief   Seek stream handler.
# 
#  @param   iid        : [\b int]  stream identifiation.
#  @param   pcbdata    : [\b object] custom data or None
#  @param seekPosition : [\b int]  seek position
#  @param seekOrigin   : [\b int]  requested seek origin
# 
#  @returns  TRUE on success / FALSE on error
# 
#  @see tml_SndStream_Register_Seek() 
def TML_ON_SND_STRM_SEEK_CB_FUNC(iid, pcbdata, seekPosition, seekOrigin): pass

## @ingroup  streamIO
#  @brief    Register seek handler.
# 
#  @param   tmlcore    : [\b int] TML core handle (TML_CORE_HANDLE)
#  @param   iid        : [\b int] stream identifiation.
#  @param   pcbfunc    : [\b function] callback function TML_ON_SND_STRM_SEEK_CB_FUNC() or None to unregister a previously registered function
#  @param   pcbdata    : [\b object] data to be registered with the function
# 
#  @see tml_RecStream_Seek(), TML_ON_SND_STRM_SEEK_CB_FUNC() 
def tml_SndStream_Register_Seek(tmlcore, iid, pcbfunc, pcbdata): pass

## @ingroup  tmlCallback
#  @brief   Stream write handler.
# 
#  @param   iid        : [\b int]  stream identifiation.
#  @param   buffer     : [\b str]   data buffer
#  @param   count      : [\b int]  number of bytes to write
#  @param   pcbdata    : [\b object] custom data or None
# 
#  @returns  TRUE on success / FALSE on error
# 
#  @see tml_SndStream_Register_Write()
def TML_ON_SND_STRM_WRITE_CB_FUNC(iid, buffer, count, pcbdata): pass

## @ingroup  streamIO
#  @brief    Register write stream handler.
# 
#  @param   tmlcore    : [\b int] TML core handle (TML_CORE_HANDLE)
#  @param   iid        : [\b int] stream identifiation.
#  @param   pcbfunc    : [\b function] callback function TML_ON_SND_STRM_WRITE_CB_FUNC() or None to unregister a previously registered function
#  @param   pcbdata    : [\b object] data to be registered with the function
# 
#  @see tml_RecStream_Write(), TML_ON_SND_STRM_WRITE_CB_FUNC()
def tml_SndStream_Register_Write(tmlcore, iid, pcbfunc, pcbdata): pass

## @ingroup tmlHandle
#  @brief   Aqcuire command data (SIDEX_HANDLE)
# 
#  The TML command has it's own SIDEX document to transport data that has to be locked before 
#  safely accessed and/or changed (see SIDEX documentation) by a specific thread. 
#  The lock has to be removed after accessing the data
#  with tml_Cmd_Release_Sidex_Handle(). 
# 
#  @param   tmlcmd    : [\b int] TML command handle (TML_COMMAND_HANDLE)
# 
#  @returns [\b int] SIDEX handle
# 
#  @see tml_Cmd_Release_Sidex_Handle()
def tml_Cmd_Acquire_Sidex_Handle(tmlcmd): pass

## @ingroup tmlHandle
#  @brief    Create a TML command.
# 
#  @returns [\b int] TML command handle (TML_COMMAND_HANDLE)
# 
#  @see tml_Cmd_Free()
def tml_Cmd_Create(): pass

## @ingroup tmlHandle
#  @brief    Release a TML command.
# 
#  @param   tmlcmd    : [\b int] TML command handle (TML_COMMAND_HANDLE)
# 
#  @see tml_Cmd_Create()
def tml_Cmd_Free(tmlcmd): pass

## @ingroup tmlHandle
#  @brief   Release command data (SIDEX_HANDLE)
# 
#  The command data handle has to be released by the same thread, that aquired the data for access.
#  Trying to release from another thread results in an error.
# 
#  @param   tmlcmd    : [\b int] TML command handle (TML_COMMAND_HANDLE)
# 
#  @see tml_Cmd_Acquire_Sidex_Handle()
def tml_Cmd_Release_Sidex_Handle(tmlcmd): pass

## @ingroup tmlCmdCallbacks
#  @brief   Command ready handler.
# 
#  The command ready handler is called to handle the return value of asynchronous commands.
# 
#  @param   tmlcmd    : [\b int] TML command handle (TML_COMMAND_HANDLE)
#  @param   pcbdata   : [\b object] registered data
# 
#  @see tml_Bal_Send_AsyncMessage(), tml_Send_AsyncMessage(), tml_Cmd_Register_CommandReady()
def TML_ON_COMMAND_READY_CB_FUNC(tmlcmd, pcbdata): pass

## @ingroup tmlHandle
#  @brief   Register command ready handler.
# 
#  The command ready handler is called if an asynchronous command receives its final reply or if an error occurred.
# 
#  @param   tmlcmd    : [\b int] TML command handle (TML_COMMAND_HANDLE)
#  @param   pcbfunc   : [\b function] callback function TML_ON_COMMAND_READY_CB_FUNC() or None to unregister a previously registered function
#  @param   pcbdata   : [\b object] data to be registered with the function
# 
#  @see tml_Send_AsyncMessage(), tml_Bal_Send_AsyncMessage(), TML_ON_COMMAND_READY_CB_FUNC(), tml_Cmd_Registered_CommandReady()
def tml_Cmd_Register_CommandReady(tmlcmd, pcbfunc, pcbdata): pass

## @ingroup tmlCmdCallbacks
#  @brief   Callback function to handle TML progress replies.
# 
#  @param   tmlcmd    : [\b int] TML command handle (TML_COMMAND_HANDLE)
#  @param   pcbdata   : [\b object] registered data
#  @param   iprogress : [\b int] progress value (0-100)
# 
#  @see tml_Send_AsyncProgressReply(), tml_Cmd_Register_Progress()
def TML_ON_PROGRESS_REPLY_CB_FUNC(tmlcmd, pcbdata, iprogress): pass

## @ingroup tmlHandle
#  @brief   Register a progress reply handler.
# 
#  The progress reply handler is called if a progress reply was received for a command.
# 
#  @param   tmlcmd    : [\b int] TML command handle (TML_COMMAND_HANDLE)
#  @param   pcbfunc   : [\b function] callback function TML_ON_PROGRESS_REPLY_CB_FUNC() or None to unregister a previously registered function
#  @param   pcbdata   : [\b object] data to be registered with the function
# 
#  @see tml_Send_AsyncProgressReply(), TML_ON_PROGRESS_REPLY_CB_FUNC(), tml_Cmd_Registered_Progress()
def tml_Cmd_Register_Progress(tmlcmd, pcbfunc, pcbdata): pass

## @ingroup tmlCmdCallbacks
#  @brief   Callback function to handle TML status replies.
# 
#  @param   tmlcmd    : [\b int]  TML command handle (TML_COMMAND_HANDLE)
#  @param   pcbdata   : [\b object] registered data
#  @param   itype     : [\b int]  reply type
#  @param   smsg      : [\b str]   reply message
# 
#  @see tml_Cmd_Register_StatusReply(), tml_Send_AsyncStatusReply()
def TML_ON_STATUS_REPLY_CB_FUNC(tmlcmd, pcbdata, itype, smsg): pass

## @ingroup tmlHandle
#  @brief   Register a status reply handler.
# 
#  The status reply handler is called if a status reply was received for a command.
# 
#  @param   tmlcmd    : [\b int]    TML command handle (TML_COMMAND_HANDLE)
#  @param   pcbfunc   : [\b function] callback function TML_ON_STATUS_REPLY_CB_FUNC() or None to unregister a previously registered function
#  @param   pcbdata   : [\b object]   data to be registered with the function
# 
#  @see tml_Send_AsyncStatusReply(), TML_ON_STATUS_REPLY_CB_FUNC(), tml_Cmd_Registered_StatusReply()
def tml_Cmd_Register_StatusReply(tmlcmd, pcbfunc, pcbdata): pass

## @ingroup tmlHandle
#  @brief   Get registered command ready handler and custom data.
# 
#  @param   tmlcmd    : [\b int]  TML command handle (TML_COMMAND_HANDLE)
# 
#  <TABLE>
#   <TR><TD><B>Name</B></TD><TD><B>Type    </B></TD><TD><B>Description          </B></TD></TR>
#   <TR><TD>function</TD><TD>pyObject </TD><TD>handler function</TD></TR>
#   <TR><TD>data</TD><TD>pyObject </TD><TD>custom data</TD></TR>
#  </TABLE>
#
#  @return    [\b tuple]  (function, data)
# 
#  @see TML_ON_COMMAND_READY_CB_FUNC(), tml_Cmd_Register_CommandReady() 
def tml_Cmd_Registered_CommandReady(tmlcmd): pass

## @ingroup tmlHandle
#  @brief   Get registered progress reply handler and custom data.
# 
#  The progress reply handler is called if a progress reply was received for a command.
# 
#  @param   tmlcmd    : [\b int]  TML command handle (TML_COMMAND_HANDLE)
# 
#  <TABLE>
#   <TR><TD><B>Name</B></TD><TD><B>Type    </B></TD><TD><B>Description          </B></TD></TR>
#   <TR><TD>function</TD><TD>pyObject </TD><TD>handler function</TD></TR>
#   <TR><TD>data</TD><TD>pyObject </TD><TD>custom data</TD></TR>
#  </TABLE>
#
#  @return    [\b tuple]  (function, data)
# 
#  @see tml_Cmd_Register_Progress()
def tml_Cmd_Registered_Progress(tmlcmd): pass

## @ingroup tmlHandle
#  @brief   Get registered status reply handler and custom data.
# 
#  The status reply handler is called if a status reply was received for a command.
# 
#  @param   tmlcmd    : [\b int]  TML command handle (TML_COMMAND_HANDLE)
# 
#  <TABLE>
#   <TR><TD><B>Name</B></TD><TD><B>Type    </B></TD><TD><B>Description          </B></TD></TR>
#   <TR><TD>function</TD><TD>pyObject </TD><TD>handler function</TD></TR>
#   <TR><TD>data</TD><TD>pyObject </TD><TD>custom data</TD></TR>
#  </TABLE>
#
#  @return    [\b tuple]  (function, data)
# 
#  @see tml_Send_AsyncStatusReply(), tml_Cmd_Register_StatusReply()
def tml_Cmd_Registered_StatusReply(tmlcmd): pass

## @ingroup tmlheader
#  @brief Get the command id.
# 
#  @param   tmlcmd    : [\b int]  TML command handle (TML_COMMAND_HANDLE)
# 
#  @returns [\b int] command id
# 
#  @see tml_Cmd_Header_SetCommand()
def tml_Cmd_Header_GetCommand(tmlcmd): pass

## @ingroup tmlheader
#  @brief Get command creation time.
# 
#  @param   tmlcmd    : [\b int]  TML command handle (TML_COMMAND_HANDLE)
#
#  @returns [\b str] command creation time (string)<br>
#              ISO formated: YYYYMMDDhhmmssttt<br>
#              YYYY = year, MM = month, DD= day, hh = hour, mm = minute, ss = second, ttt = miliseconds
def tml_Cmd_Header_GetCreationTime(tmlcmd): pass

## @ingroup tmlheader
#  @brief Get the error code.
# 
#  There are a number of predfined error codes that are set automatically. Besides that a
#  custom error code can be set while processing a command to inform the caller about problems related
#  to implementation specific problems.
# 
#  @param   tmlcmd    : [\b int]  TML command handle (TML_COMMAND_HANDLE)
# 
#  @returns [\b int] TML error code
# 
#  @see tml_Cmd_Header_SetError()
def tml_Cmd_Header_GetError(tmlcmd): pass

## @ingroup tmlheader
#  @brief Get error message.
# 
#  @param   tmlcmd    : [\b int]  TML command handle (TML_COMMAND_HANDLE)
# 
#  @returns [\b str] error message
# 
#  @see tml_Cmd_Header_SetErrorMessage()
def tml_Cmd_Header_GetErrorMessage(tmlcmd): pass

## @ingroup tmlheader
#  @brief Get execution mode.
# 
#   <TABLE>
#    <TR><TD><B>Name       </B></TD><TD><B>Value </B></TD><TD><B>Description  </B></TD></TR>
#    <TR><TD>TMLCOM_MODE_ASYNC   </TD><TD>0</TD><TD>asynchronous command call</TD></TR>
#    <TR><TD>TMLCOM_MODE_SYNC   </TD><TD>1</TD><TD>synchronous command call</TD></TR>
#    <TR><TD>TMLCOM_MODE_EVT   </TD><TD>2</TD><TD>event call</TD></TR>
#   </TABLE>
#  
#  @param   tmlcmd    : [\b int]  TML command handle (TML_COMMAND_HANDLE)
# 
#  @returns [\b int] execution mode
# 
#  @see tml_Cmd_Header_SetMode()
def tml_Cmd_Header_GetMode(tmlcmd): pass

## @ingroup tmlheader
#  @brief Get progress.
# 
#  If a progress is processed, the progress value can be read by this function.
# 
#  @param   tmlcmd    : [\b int]  TML command handle (TML_COMMAND_HANDLE)
# 
#  @returns [\b int] progress [0..100] in percent
# 
#  @see tml_Cmd_Header_SetProgress(), tml_Send_AsyncProgressReply()
def tml_Cmd_Header_GetProgress(tmlcmd): pass

## @ingroup tmlheader
#  @brief Get status reply message.
# 
#  @param   tmlcmd    : [\b int]  TML command handle (TML_COMMAND_HANDLE)
# 
#  @returns [\b str] reply message
# 
#  @see tml_Send_AsyncStatusReply(), tml_Cmd_Header_SetReplyMessage()
def tml_Cmd_Header_GetReplyMessage(tmlcmd): pass

## @ingroup tmlheader
#  @brief Get reply type.
# 
#  If a progress or status reply is received, the command execution state is TMLCOM_CSTATE_PENDING and the
#  reply type can be used to determine the kind of reply.
# 
#   <TABLE>
#    <TR><TD><B>Name       </B></TD><TD><B>Value </B></TD><TD><B>Description  </B></TD></TR>
#    <TR><TD>TMLCOM_RPY_PROGRESS   </TD><TD>0</TD><TD>progress reply</TD></TR>
#    <TR><TD>TMLCOM_RPY_WARNING   </TD><TD>10</TD><TD>warning status reply</TD></TR>
#    <TR><TD>TMLCOM_RPY_ERROR   </TD><TD>20</TD><TD>error status reply</TD></TR>
#    <TR><TD>TMLCOM_RPY_INFORMATION   </TD><TD>30</TD><TD>information status reply</TD></TR>
#    <TR><TD>TMLCOM_CSTATE_UNDEFINED   </TD><TD>-1</TD><TD>undefined reply type</TD></TR>
#   </TABLE>
#  
#  @param   tmlcmd    : [\b int]  TML command handle (TML_COMMAND_HANDLE)
# 
#  @returns [\b int] reply type
# 
#  @see tml_Cmd_Header_SetReplyType(), tml_Send_AsyncStatusReply()
def tml_Cmd_Header_GetReplyType(tmlcmd): pass

## @ingroup tmlheader
#  @brief Get execution state.
# 
#   <TABLE>
#    <TR><TD><B>Name       </B></TD><TD><B>Value </B></TD><TD><B>Description  </B></TD></TR>
#    <TR><TD>TMLCOM_CSTATE_CREATED   </TD><TD>0</TD><TD>initial command state</TD></TR>
#    <TR><TD>TMLCOM_CSTATE_EXECUTED   </TD><TD>1</TD><TD>command successfully executed</TD></TR>
#    <TR><TD>TMLCOM_CSTATE_FAILED   </TD><TD>2</TD><TD>command execution failed</TD></TR>
#    <TR><TD>TMLCOM_CSTATE_PENDING   </TD><TD>3</TD><TD>command execution pending</TD></TR>
#    <TR><TD>TMLCOM_CSTATE_UNDEFINED  </TD><TD>-1</TD><TD>undefined command state</TD></TR>
#   </TABLE>
#  
#  @param   tmlcmd    : [\b int]  TML command handle (TML_COMMAND_HANDLE)
# 
#  @returns [\b int] execution state
# 
#  @see tml_Cmd_Header_SetState()
def tml_Cmd_Header_GetState(tmlcmd): pass

## @ingroup tmlheader
#  @brief Set the command id.
# 
#  A command ID identifies the command and is necessary to process the command a the receiver.
# 
#  @param   tmlcmd    : [\b int]  TML command handle (TML_COMMAND_HANDLE)
#  @param   cmdid     : [\b int]  command id
# 
#  @see tml_Cmd_Header_GetCommand()
def tml_Cmd_Header_SetCommand(tmlcmd, cmdid): pass

## @ingroup tmlheader
#  @brief Set the error code.
# 
#  If a custom error code is set while command processing it will not be overwritten by TML specific standard
#  error codes. 
# 
#  @param   tmlcmd    : [\b int]  TML command handle (TML_COMMAND_HANDLE)
#  @param   ierr      : [\b int]  error code
# 
#  @see tml_Cmd_Header_GetError()
def tml_Cmd_Header_SetError(tmlcmd, ierr): pass

## @ingroup tmlheader
#  @brief Set error message.
# 
#  If a custom error message is set while command processing it will not be overwritten by TML specific standard
#  error messages. 
# 
#  @param   tmlcmd    : [\b int]  TML command handle (TML_COMMAND_HANDLE)
#  @param   serr      : [\b str]   error message
# 
#  @see tml_Cmd_Header_GetErrorMessage()
def tml_Cmd_Header_SetErrorMessage(tmlcmd, serr): pass

## @ingroup tmlheader
#  @brief Set execution mode.
# 
#  It is not recommended to change the execution mode. This values is used internally. Changing the command state
#  might result in unpredictable results.
# 
#  @param   tmlcmd    : [\b int]  TML command handle (TML_COMMAND_HANDLE)
#  @param   imode     : [\b int]  execution mode
# 
#  @see tml_Cmd_Header_GetMode()
def tml_Cmd_Header_SetMode(tmlcmd, imode): pass

## @ingroup tmlheader
#  @brief Set progress.
# 
#  The progress is usually set by tml_Send_AsyncProgressReply(). 
# 
#  @param   tmlcmd    : [\b int]  TML command handle (TML_COMMAND_HANDLE)
#  @param   iprogress : [\b int]  progress value (0-100)
#
#  @see tml_Cmd_Header_GetProgress(), tml_Send_AsyncProgressReply() 
def tml_Cmd_Header_SetProgress(tmlcmd, iprogress): pass

## @ingroup tmlheader
#  @brief Set status reply message.
# 
#  @param   tmlcmd    : [\b int]  TML command handle (TML_COMMAND_HANDLE)
#  @param   srmsg     : [\b str]  reply message
# 
#  @see tml_Send_AsyncStatusReply(), tml_Cmd_Header_GetReplyMessage()
def tml_Cmd_Header_SetReplyMessage(tmlcmd, srmsg): pass

## @ingroup tmlheader
#  @brief Set reply type.
# 
#  It is not recommended to change the reply type. This values is used internally. Changing the reply type
#  might result in unpredictable results.
# 
#  @param   tmlcmd    : [\b int]  TML command handle (TML_COMMAND_HANDLE)
#  @param   irtype    : [\b int]  reply type
# 
#  @see tml_Cmd_Header_GetReplyType()
def tml_Cmd_Header_SetReplyType(tmlcmd, irtype): pass

## @ingroup tmlheader
#  @brief Set execution state.
# 
#  It is not recommended to change the command state. This values is used internally. Changing the command state
#  might result in unpredictable results.
# 
#  @param   tmlcmd    : [\b int]  TML command handle (TML_COMMAND_HANDLE)
#  @param   istate    : [\b int]  execution state
# 
#  @see tml_Cmd_Header_GetState()
def tml_Cmd_Header_SetState(tmlcmd, istate): pass








