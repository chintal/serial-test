"""
This file is part of serial-test
See the COPYING, README, and INSTALL files for more information
"""


import serial
import threading
import time
import datetime
import _prbs


prbs = None
recieved_bytes = 0
error_bytes = 0
total_recieved_bytes = 0
total_error_bytes = 0
throughput = 0


def dump_status():
    global throughput
    global next_dump
    global total_recieved_bytes
    global total_error_bytes

    if total_recieved_bytes > 0:
        byer = 1.00 * total_error_bytes / total_recieved_bytes
    else:
        byer = 0.00

    print '{0}::{1} bytes/sec :: BYER :{2} :: Total :{3} :: Errors :{4}' \
          ''.format(datetime.datetime.now(), throughput, byer,
                    total_recieved_bytes, total_error_bytes)


def update_status():
    global recieved_bytes
    global error_bytes
    global total_error_bytes
    global total_recieved_bytes
    global throughput
    global next_update

    throughput = recieved_bytes

    total_recieved_bytes += recieved_bytes
    recieved_bytes = 0

    total_error_bytes += error_bytes
    error_bytes = 0

    dump_status()

    next_update += 1
    threading.Timer(next_update - time.time(), update_status).start()


def init_prbs():
    global prbs
    prbs = _prbs.new_lfsr16_t()
    _prbs.lfsr_vInit(prbs, int(_prbs.LFSR_DEFAULT_SEED), int(_prbs.LFSR_DEFAULT_TAPS))
    return


def begin_ber_test(layer2):
    global recieved_bytes
    global error_bytes
    global prbs
    init_prbs()
    layer2.write('b')
    expected_char = _prbs.lfsr_cGetNextByte(prbs)
    sync = 0
    print "Waiting for Sync"
    while not sync:
        char = layer2.read()
        if char == 'b':
            sync = 1
            print "Synced"
    global next_update
    next_update = time.time()
    update_status()
    while 1:
        char = layer2.read()
        if str(format(ord(char), '02x')) != str(format(expected_char, '02x')):
            print "Received: " + str(ord(char)) + " ::Expected : " + str(expected_char)
            error_bytes += 1
        recieved_bytes += 1
        expected_char = _prbs.lfsr_cGetNextByte(prbs)


def begin_throughput_test(layer2):
    global recieved_bytes
    global error_bytes
    layer2.write('a')
    expected_char = ord('0')
    sync = 0
    print "Waiting for Sync"
    while not sync:
        char = layer2.read()
        if char == 'a':
            sync = 1
            print "Synced"
    global next_update
    next_update = time.time()
    update_status()
    while 1:
        char = layer2.read()
        if ord(char) != expected_char:
            error_bytes += 1
        recieved_bytes += 1
        if expected_char is ord('z'):
            expected_char = ord('0')
        else:
            expected_char += 1


if __name__ == '__main__':
    ser = serial.Serial('/dev/ttyACM1', baudrate=256000, timeout=1)
    ser.flushInput()
    ser.flushOutput()
    i = 0
    drain = ser.read()
    while len(drain) == 1:
        drain = ser.read()
        i += 1
    print "Drained {0} bytes".format(i)
    begin_throughput_test(ser)
