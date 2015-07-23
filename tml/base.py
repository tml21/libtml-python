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


## @brief Base class for core related objects.
#  @ingroup TMLCoreGroup
#
#  This base class implements the base for classes that require a
#  reference to the TMLCore it is related to.
class TMLCoreRelated(object):

    def __init__(self, core):
        self.__tml_core = core

        return

        # ------------ properties ---------------------
        # -- the code here is for documentation only --
        self.tml_core = None
        # ------------ end properties -----------------

    ## @property tml_core
    #  @brief [\b tml.core.TMLCore] TML core instance
    #
    #  @par Access:
    #    read

    #  @cond NODOC
    @property
    def tml_core(self):
        return self.__tml_core
    #  @endcond


# ## @brief Base class for profile related objects.
#  @ingroup TMLProfileGroup
#
#  This base class implements the base for classes that require a
#  reference to the TMLProfile it is related to.
class TMLProfileRelated(TMLCoreRelated):

    def __init__(self, profile):
        TMLCoreRelated.__init__(self, profile.tml_core)
        self.__tml_profile_url = profile.profile_url

        return

        # ------------ properties ---------------------
        # -- the code here is for documentation only --
        self.tml_profile = None
        # ------------ end properties -----------------

    ## @property tml_profile
    #  @brief [\b tml.profile.TMLProfile] TML profile instance
    #
    #
    #  @par Access:
    #    read

    #  @cond NODOC
    @property
    def tml_profile(self):
        return self.tml_core.profiles[self.__tml_profile_url]
    #  @endcond

