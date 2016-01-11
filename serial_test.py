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

    print (str(datetime.datetime.now()) + '::' + str(throughput) + ' bytes/sec :: BYER :' + str(byer) +
            " :: Total :" + str(total_recieved_bytes) + " :: Errors :" + str(total_error_bytes))


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


def begintest(layer2):
    global recieved_bytes
    global error_bytes
    global prbs
    init_prbs()
    layer2.write('a')
    expected_char = _prbs.lfsr_cGetNextByte(prbs)
    sync = 0
    while not sync:
        char = layer2.read()
        if char == chr(0x01):
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


def init_prbs():
    global prbs
    prbs = _prbs.new_lfsr16_t()
    _prbs.lfsr_vInit(prbs, int(_prbs.LFSR_DEFAULT_SEED), int(_prbs.LFSR_DEFAULT_TAPS))
    return

if __name__ == '__main__':
    ser = serial.Serial('/dev/ttyACM4', timeout=1)
    ser.flushInput()
    ser.flushOutput()
    i = 0
    drain = ser.read()
    drain = []
    while len(drain) == 1:
        drain = ser.read()
        i += 1
    begintest(ser)
