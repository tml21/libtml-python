#!/usr/bin/python
# -*- coding: utf-8 -*-
#-----------------------------------------------------------
# TML show video demo with Open CV
# This script requires Open CV for Python
# http://opencv.org/
#-----------------------------------------------------------

import numpy as np
import cv2
import tml.common
import tml.core
import random
import sidex.variant


def frame_evt_handler(tmlprofile, cmd):
    frame = cmd.data['video']['frame']
    print('frame count %s' % cmd.data['video']['cnt'])

    # Load a color image in gray scale
    img_array = np.asarray(bytearray(sidex.variant.sdx_to_py(frame)), dtype=np.uint8)
    img = cv2.imdecode(img_array, 1)
    cv2.imshow('image', img)


def main():
    try:
        tmlcore = tml.core.TMLCore()

        tmlprofile = "urn:tml-software.com:raspberry:camera"

        # event producer address
        paddress = "raspi:12345"
        host, port = tml.common.split_binding(paddress)

        # event consumer address
        laddress = "myhost:%s" % random.randint(55000, 60000)

        print("start liestener on ", laddress)
        tmlcore.listener_binding = laddress
        tmlcore.listener_enabled = True

        # register event handler
        prf = tmlcore.profiles.add_profile(tmlprofile)
        tmlcore.profiles[tmlprofile].register_cmd(55, frame_evt_handler)

        cv2.namedWindow('image', cv2.WINDOW_NORMAL)

        print(host, port)
        prf.register_evt_consumer(host, port)

        # wait ...
        cv2.waitKey(0)
    finally:
        cv2.destroyAllWindows()
        prf.unregister_evt_consumer(host, port)
        tmlcore.close()

if __name__ == '__main__':
    main()
