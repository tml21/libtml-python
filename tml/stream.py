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

import tml.base
import tml.common
import tml.constants

import sidex.variant


## @defgroup TMLStreamGroup TML Stream
#  @ingroup highLevelClasses
#
#  Classes to send and receive streams with TML.


## @brief Receiver stream object.
#  @ingroup TMLStreamGroup
#
#  The receiver stream is connected to a sender stream on another peer.
class TMLRecStream(tml.base.TMLProfileRelated):

    def __init__(self, profile, streamid, address):
        tml.base.TMLProfileRelated.__init__(self, profile)
        self.__stream_id = sidex.variant.sdx_to_py(streamid)
        host, port = tml.common.split_binding(address)

        tml.tml_RecStream_Open(self.tml_core.core_handle, self.__stream_id, self.tml_profile.profile_url, host, port)

        return

        # ------------ properties ---------------------
        # -- the code here is for documentation only --
        self.stream_id = None
        self.size = -1
        self.position = 0
        # ------------ end properties -----------------

    def __del__(self):
        self.close()

    ## @property stream_id
    #  @brief [\b int] unique stream id.
    #
    #  @par Access:
    #    read

    #  @cond NODOC
    @property
    def stream_id(self):
        return self.__stream_id
    #  @endcond

    ## @property size
    #  @brief [\b int] Stream size in bytes.
    #
    #  @par Access:
    #    read
    #
    #  @sa tml_RecStream_GetSize()

    #  @cond NODOC
    @property
    def size(self):
        return tml.tml_RecStream_GetSize(self.tml_core.core_handle, self.__stream_id)
    #  @endcond

    ## @property position
    #  @brief [\b int] Stream position.
    #
    #  @par Access:
    #    read
    #
    #  @sa tml_RecStream_GetPosition()

    #  @cond NODOC
    @property
    def position(self):
        return tml.tml_RecStream_GetPosition(self.tml_core.core_handle, self.__stream_id)
    #  @endcond

    ## @brief Move the cursor to another stream position.
    #
    #  @param seekpos : [\b int] position offset
    #  @param origin  : [\b int] @ref TMLStreamSeekConst "offset origin"
    #
    #  @sa tml_RecStream_Seek()
    def seek(self, seekpos, origin):
        tml.tml_RecStream_Seek(self.tml_core.core_handle, self.__stream_id, seekpos, origin)

    ## @brief Read a buffer from the stream.
    #
    #  If the stream ends before then number of bytes are read, only the remaining
    #  bytes are returned in the result.
    #
    #  @param rbytes   : [\b int] number of bytes to read
    #
    #  @returns [\b str] bytes from the stream
    #
    #  @sa tml_RecStream_Read()
    def read(self, rbytes):
        return tml.tml_RecStream_Read(self.tml_core.core_handle, self.__stream_id, rbytes)

    ## @brief Write a buffer to a stream.
    #
    #  @param wbuffer   : [\b str] bytes to write
    #
    #  @sa tml_RecStream_Write()
    def write(self, wbuffer):
        tml.tml_RecStream_Write(self.tml_core.core_handle, self.__stream_id, wbuffer)

    ## @brief Close the stream.
    #
    #  @param ashutdown : [\b bool][\em optional] used to indicate internal close on shutdown
    #
    #  @sa tml_RecStream_Close()
    def close(self, ashutdown=False):
        if self.__stream_id is not None:
            tml.tml_RecStream_Close(self.tml_core.core_handle, self.__stream_id, False)
            if not ashutdown:
                self.tml_profile.del_rec_stream(self.__stream_id)
            self.__stream_id = None


#  @cond NODOC
def on_snd_stream_get_size(iid, tmlstream):
    assert isinstance(tmlstream, TMLAbstractSndStream), "tmlstream is not a TMLAbstractSndStream descendant"
    return tmlstream.on_get_size()
#  @endcond


#  @cond NODOC
def on_snd_stream_get_position(iid, tmlstream):
    assert isinstance(tmlstream, TMLAbstractSndStream), "tmlstream is not a TMLAbstractSndStream descendant"
    return tmlstream.on_get_position()
#  @endcond


#  @cond NODOC
def on_snd_stream_error(iid, ierr, tmlstream):
    assert isinstance(tmlstream, TMLAbstractSndStream), "tmlstream is not a TMLAbstractSndStream descendant"
    tmlstream.on_error(ierr)
#  @endcond


#  @cond NODOC
def on_snd_stream_read(iid, tmlstream, bcount):
    assert isinstance(tmlstream, TMLAbstractSndStream), "tmlstream is not a TMLAbstractSndStream descendant"
    return tmlstream.on_read(bcount)
#  @endcond


#  @cond NODOC
def on_snd_stream_write(iid, buffer, bcount, tmlstream):
    assert isinstance(tmlstream, TMLAbstractSndStream), "tmlstream is not a TMLAbstractSndStream descendant"
    return tmlstream.on_write(buffer, bcount)
#  @endcond


#  @cond NODOC
def on_snd_stream_seek(iid, tmlstream, seekpos, seekorigin):
    assert isinstance(tmlstream, TMLAbstractSndStream), "tmlstream is not a TMLAbstractSndStream descendant"
    return tmlstream.on_seek(seekpos, seekorigin)
#  @endcond


#  @cond NODOC
def on_snd_stream_close(iid, tmlstream):
    assert isinstance(tmlstream, TMLAbstractSndStream), "tmlstream is not a TMLAbstractSndStream descendant"
    return tmlstream.close()
#  @endcond


## @brief Sender stream object.
#  @ingroup TMLStreamGroup
#
#  A sender stream provides a data stream to another peer to connect with
#  a TMLRecStream for accessing the data.
#
#  This is an abstract base class and can be used to implement concrete stream handling.
#  As an example of a concrete class implementation you can refer to TMLFileStreamSnd.
class TMLAbstractSndStream(tml.base.TMLProfileRelated):

    def __init__(self, profile):
        tml.base.TMLProfileRelated.__init__(self, profile)

        self.__stream_id = tml.tml_SndStream_Open(self.tml_core.core_handle, self.tml_profile.profile_url, "127.0.0.1", "4242")

        tml.tml_SndStream_Register_GetSize(self.tml_core.core_handle, self.__stream_id, on_snd_stream_get_size, self)
        tml.tml_SndStream_Register_Close(self.tml_core.core_handle, self.__stream_id, on_snd_stream_close, self)
        tml.tml_SndStream_Register_GetPosition(self.tml_core.core_handle, self.__stream_id, on_snd_stream_get_position, self)
        tml.tml_SndStream_Register_OnError(self.tml_core.core_handle, self.__stream_id, on_snd_stream_error, self)

        tml.tml_SndStream_Register_Read(self.tml_core.core_handle, self.__stream_id, on_snd_stream_read, self)
        tml.tml_SndStream_Register_Write(self.tml_core.core_handle, self.__stream_id, on_snd_stream_write, self)
        tml.tml_SndStream_Register_Seek(self.tml_core.core_handle, self.__stream_id, on_snd_stream_seek, self)
        return

        # ------------ properties ---------------------
        # -- the code here is for documentation only --
        self.stream_id = None
        # ------------ end properties -----------------

    def __del__(self):
        self.close()

    ## @property stream_id
    #  @brief [\b int] unique stream id.
    #
    #
    #  @par Access:
    #    read

    #  @cond NODOC
    @property
    def stream_id(self):
        return self.__stream_id
    #  @endcond

    ## @brief Close the stream.
    #
    #  Do not override this method. Use on_close() instead to add own code.
    #
    #  @param ashutdown : [\b bool][\em optional] used to indicate internal close on shutdown
    #
    #  @sa tml_SndStream_Close()
    def close(self, ashutdown=False):
        if self.__stream_id is not None:
            tml.tml_SndStream_Register_GetSize(self.tml_core.core_handle, self.__stream_id, None, None)
            tml.tml_SndStream_Register_Close(self.tml_core.core_handle, self.__stream_id, None, None)
            tml.tml_SndStream_Register_GetPosition(self.tml_core.core_handle, self.__stream_id, None, None)
            tml.tml_SndStream_Register_OnError(self.tml_core.core_handle, self.__stream_id, None, None)
            tml.tml_SndStream_Register_Read(self.tml_core.core_handle, self.__stream_id, None, None)
            tml.tml_SndStream_Register_Write(self.tml_core.core_handle, self.__stream_id, None, None)
            tml.tml_SndStream_Register_Seek(self.tml_core.core_handle, self.__stream_id, None, None)

            tml.tml_SndStream_Close(self.tml_core.core_handle, self.__stream_id)
            if not ashutdown:
                self.tml_profile.del_snd_stream(self.__stream_id)
            self.__stream_id = None

            self.on_close()

    ## @brief Get stream size event.
    #
    #  Abstract function to be derived and implemented in concrete stream classes.
    #
    #  @returns [\b int] stream size in byte
    def on_get_size(self):
        assert False, 'Pure virtual function should be derived'

    ## @brief Get stream position event.
    #
    #  Abstract function to be derived and implemented in concrete stream classes.
    #
    #  @returns [\b int] stream position
    def on_get_position(self):
        assert False, 'Pure virtual function should be derived'

    ## @brief On error event.
    #
    #  Abstract function to be derived and implemented in concrete stream classes.
    #
    #  @param ierr : [\b int] error code
    def on_error(self, ierr):
        assert False, 'Pure virtual function should be derived'

    ## @brief On seek event.
    #
    #  Abstract function to be derived and implemented in concrete stream classes.
    #
    #  @param soffset : [\b int] seek offset in bytes
    #  @param sorigin : [\b int] @ref TMLStreamSeekConst "offset origin"
    #
    #  @returns [\b bool] True = success
    def on_seek(self, soffset, sorigin):
        assert False, 'Pure virtual function should be derived'

    ## @brief On write event.
    #
    #  Abstract function to be derived and implemented in concrete stream classes.
    #
    #  @param buffer  : [\b str] buffer to write
    #  @param bcount  : [\b int] number of bytes to write
    #
    #  @returns [\b bool] True = success
    def on_write(self, buffer, bcount):
        assert False, 'Pure virtual function should be derived'

    ## @brief On read event.
    #
    #  Abstract function to be derived and implemented in concrete stream classes.
    #
    #  @param bcount  : [\b int] number of bytes to read
    #
    #  @returns [(\b bool, \b str)] (<result>, <buffer>) - result = True on success
    def on_read(self, bcount):
        assert False, 'Pure virtual function should be derived'

    ## @brief Perform cleanup code on close stream.
    #
    #  In own stream implementations this method can be overridden to run
    #  cleanup code befor closing the stream.
    def on_close(self):
        pass


## @brief File stream sender.
#  @ingroup TMLStreamGroup
#
#  This class implements a file stream sender. A file path can be passed
#  to be transferred to a receiver peer.
class TMLFileStreamSnd(TMLAbstractSndStream):

    def __init__(self, profile, path):
        TMLAbstractSndStream.__init__(self, profile)
        self.__filepath = path
        self.__file = open(self.__filepath, 'rb')
        self.__file.seek(0, 2)  # move to end of file
        self.__size = self.__file.tell()
        self.__file.seek(0, 0)  # move to start of file

    ## @brief Get file stream size.
    #
    #  @returns [\b int] stream size in byte
    def on_get_size(self):
        return self.__size

    ## @brief Get stream position event.
    #
    #  @returns [\b int] stream position
    def on_get_position(self):
        return self.__file.tell()

    ## @brief On error event.
    #
    #  @param ierr : [\b int] error code
    def on_error(self, ierr):
        pass

    ## @brief On seek event.
    #
    #  @param soffset : [\b int] seek offset in bytes
    #  @param sorigin : [\b int] @ref TMLStreamSeekConst "offset origin"
    #
    #  @returns [\b bool] True = success
    def on_seek(self, soffset, sorigin):
        try:
            self.__file.seek(soffset, sorigin)
        except:
            return False
        return True

    ## @brief On write event.
    #
    #  @param buffer  : [\b str] buffer to write
    #  @param bcount  : [\b int] number of bytes to write
    #
    #  @returns [\b bool] True = success
    def on_write(self, buffer, bcount):
        assert False, 'Pure virtual function should be derived'

    ## @brief On read event.
    #
    #  @param bcount  : [\b int] number of bytes to read
    #
    #  @returns [(\b bool, \b str)] (<result>, <buffer>) - result = True on success
    def on_read(self, bcount):
        try:
            sbuffer = self.__file.read(bcount)
            return True, sbuffer
        except Exception as e:
            return False, None

    ## @brief Perform cleanup code on close stream.
    #
    #  In own stream implementations this method can be overridden to run
    #  cleanup code befor closing the stream.
    def on_close(self):
        self.__file.close()


## @brief File receiver stream object.
#  @ingroup TMLStreamGroup
#
#  The standard TMLRecStream is extended by a download function to save the file.
class TMLFileStreamRec(TMLRecStream):

    ## @brief Save stream to file.
    #
    #  @param afilename : [\b str] file name
    #  @param chunksize : [\b int][\em optional] buffer size
    def save_file(self, afilename, chunksize=1024):
        sfile = open(afilename, 'wb')

        chunks = int(self.size/chunksize)+1
        assert chunks*chunksize >= self.size, "download calculation invalid"

        self.seek(0, tml.constants.SO_FROM_BEGINNING)
        for c in range(0, chunks):
            buffer = self.read(chunksize)
            sfile.write(buffer)

        sfile.close()