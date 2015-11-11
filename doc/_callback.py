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

## @defgroup highLevelCallbacks High level class callbacks
#  @ingroup highLevelClasses
#  @brief The API callback functions for TML are simplified when using the high level classes.


## @brief Calculate next peer index for balancing.
#  @ingroup highLevelCallbacks
#
#  @param profile     : [\b tml.profile.TMLProfile]  profile object
#  @param cmd         : [\b list]  list of tml.command.TMLCmd
#  @param rcount      : [\b int]   number of available receivers
#
#  <TABLE>
#   <TR><TD><B>Name</B></TD><TD><B>Type    </B></TD><TD><B>Description          </B></TD></TR>
#   <TR><TD>result</TD><TD>int </TD><TD>callback result: <br>
#                                           tml.constants.TML_SUCCESS in case of success<br>
#                                           tml.constants.TML_ERR_INFORMATION_UNDEFINED cannot calculate index (fallback to round robin)<br>
#                                           or any other error code </TD></TR>
#   <TR><TD>nextIndex</TD><TD>int </TD><TD>next command receiver index</TD></TR>
#  </TABLE>
#
#  @returns [\b tuple] (result, nextindex)
#
#  @sa tml.profile.TMLProfile.on_balance_calculation
def ON_BALANCE_CALCULATION(profile, cmd):
    pass


## @brief Command handler interface.
#  @ingroup highLevelCallbacks
#
#  @param tmlprofile : [\b tml.profile.TMLProfile] sender object
#  @param cmd        : [\b tml.command.TMLCmd] command object
#
#  @sa tml.profile.TMLProfile.register_cmd
def ON_COMMAND_CALL(tmlprofile, cmd):
    pass


## @brief Command ready handler.
#  @ingroup highLevelCallbacks
#
#  @param sender  : [\b tml.profile.TMLProfile, \b tml.core.TMLCore] sender object
#  @param cmd     : [\b tml.command.TMLCmd] command object
#
#  @sa tml.profile.TMLProfile.on_cmd_ready, , tml.core.TMLCore.on_cmd_ready
def ON_COMMAND_READY(sender, cmd):
    pass


## @brief Custom dispatch handler.
#  @ingroup highLevelCallbacks
#
#  @param tmlprofile : [\b tml.profile.TMLProfile] sender object
#  @param cmdid      : [\b int]     cmd id that triggered the command
#  @param cmd        : [\b tml.command.TMLCmd]  command object
#
#  @sa tml.profile.TMLProfile.on_cmd_custom_dispatch
def ON_CUSTOM_DISPATCH(tmlprofile, cmdid, cmd):
    pass


## @brief Populate event consumers.
#  @ingroup highLevelCallbacks
#
#  @param profile     : [\b tml.profile.TMLProfile]  profile object
#
#  @sa tml.profile.TMLProfile.on_populate_event_consumer
def ON_POPULATE_EVENT_CONSUMERS(profile):
    pass


## @brief Populate call destinations.
#  @ingroup highLevelCallbacks
#
#  @param profile     : [\b tml.profile.TMLProfile]  profile object
#
#  @sa tml.profile.TMLProfile.on_populate_call_destinations
def ON_POPULATE_CALL_DESTINATIONS(profile):
    pass


## @brief Handle on Progress.
#  @ingroup highLevelCallbacks
#
#  @param sender : [\b tml.profile.TMLProfile, \b tml.core.TMLCore] sender object
#  @param cmd         : [\b tml.command.TMLCmd]  command object
#  @param progress    : [\b int] progress value in percent
#
#  @sa tml.profile.TMLProfile.on_progress, tml.core.TMLCore.on_progress
def ON_PROGRESS(sender, cmd, progress):
    pass


## @brief Handle status reply.
#  @ingroup highLevelCallbacks
#
#  @param sender : [\b tml.profile.TMLProfile, \b tml.core.TMLCore] sender object
#  @param cmd         : [\b tml.command.TMLCmd]  command object
#  @param itype       : [\b int] @ref TMLRpyTypeConst "status reply type"
#  @param smessage    : [\b str] status reply message
#
#  @sa tml.profile.TMLProfile.on_status_reply, tml.core.TMLCore.on_status_reply
def ON_STATUS_REPLY(sender, cmd, itype, smessage):
    pass


## @brief Track event errors.
#  @ingroup highLevelCallbacks
#
#  @param profile     : [\b tml.profile.TMLProfile]  profile object
#  @param binding     : [\b str]  <host>:<port>
#  @param cmdid       : [\b int] event id
#  @param ierr        : [\b int] error code
#
#  @sa tml.profile.TMLProfile.on_event_error
def ON_EVENT_ERROR(profile, binding, cmdid, ierr):
    pass


## @brief Signal event queue overflow.
#  @ingroup highLevelCallbacks
#
#  @param profile     : [\b tml.profile.TMLProfile]  profile object
#  @param cmdid       : [\b int] event id
#
#  @sa tml.profile.TMLProfile.on_queue_overflow
def ON_QUEUE_OVERFLOW(profile, cmdid):
    pass


## @brief Signal event consumer registration.
#  @ingroup highLevelCallbacks
#
#  @param profile     : [\b tml.profile.TMLProfile]  profile object
#  @param binding     : [\b str]  <host>:<port>
#  @param subscribe   : [\b bool] True or False depending if it is a subscription or unsubscription
#
#  @sa tml.profile.TMLProfile.on_register_event_consumer
def ON_REGISTER_EVENT_CONSUMER(profile, binding, subscribe):
    pass


## @brief Signal call destination registration.
#  @ingroup highLevelCallbacks
#
#  @param profile     : [\b tml.profile.TMLProfile]  profile object
#  @param binding     : [\b str]  <host>:<port>
#  @param subscribe   : [\b bool] True or False depending if it is a subscription or unsubscription
#
#  @sa tml.profile.TMLProfile.on_register_call_destination
def ON_REGISTER_CALL_DESTINATION(profile, binding, subscribe):
    pass


## @brief Handle busy status request.
#  @ingroup highLevelCallbacks
#
#  @param profile     : [\b tml.profile.TMLProfile]  profile object
#  @param cmd         : [\b tml.command.TMLCmd]  command object
#
#  @returns [\b int] tml.constants.TML_SUCCESS or error code
#
#  @sa tml.profile.TMLProfile.on_busy_status_request
def ON_BUSY_STATUS_REQUEST(profile, cmd):
    pass
