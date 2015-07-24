#!/usr/bin/python
# -*- coding: utf-8 -*-
#-----------------------------------------------------------
# Capture a Raspberry PI video and provide a  TML Stream
#-----------------------------------------------------------

import time
import picamera
import io
import tml.core
import tml.command
import tml.common


def evt_consumer_registered(profile, binding, subscribe):
    if subscribe:
        print(binding, "registered")
    else:
        print(binding, "unregistered")

try:

    tmlcore = tml.core.TMLCore()
    tmlcore.listener_binding = "0.0.0.0:12345"

    tmlprofile = "urn:tml-software.com:raspberry:camera"

    # register a command and profile
    prf = tmlcore.profiles.add_profile(tmlprofile)
    prf.on_register_event_consumer = evt_consumer_registered

    tmlcore.listener_enabled = True
    cnt = 0
    with picamera.PiCamera() as camera:
        camera.resolution = (320, 240)
        camera.framerate = 30

        # Camera warm-up time
        time.sleep(2)

        start = time.time()
        stream = io.BytesIO()

        for foo in camera.capture_continuous(stream, 'jpeg',
                                             use_video_port=True):
            # send event on key press
            cnt += 1
            cmd = tml.command.TMLCmd(55)
            cmd.data['video']['size'] = stream.tell()
            cmd.data['video']['cnt'] = cnt
            stream.seek(0)
            cmd.data['video']['frame'] = stream.read()
            prf.send_event(cmd)

            print('frame count %s' % cnt)

            if time.time() - start > 20:
                break

            stream.seek(0)
            stream.truncate()
        print('\n')
finally:
    print("finished")
    tmlcore.close()
