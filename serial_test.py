# Copyright (c)
# (c) 2015-16 Chintalagiri Shashank, Quazar Technologies Pvt. Ltd.
#
# This file is part of serial-test.
#
# serial-test is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# serial-test is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with serial-test.  If not, see <http://www.gnu.org/licenses/>.

"""
Simple serial communications testing script
"""


import argparse
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


def begin_prbs_test(layer2):
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


def begin_throughput_test(layer2, cmd):
    global recieved_bytes
    global error_bytes
    layer2.write(cmd)
    expected_char = ord('0')
    sync = 0
    print "Waiting for Sync"
    while not sync:
        char = layer2.read()
        if char == cmd:
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
    parser = argparse.ArgumentParser()
    parser.add_argument('-p', '--port', default='/dev/ttyACM1',
                        help="Serial Port",)
    parser.add_argument('-b', '--baudrate', default=256000, type=int,
                        help="Baud Rate. Not applicable to USB CDC links")
    parser.add_argument('test', choices=['baremetal', 'throughput',
                                         'prbs'],
                        default='prbs', help='Type of test to run')
    args = parser.parse_args()
    ser = serial.Serial(args.port, baudrate=args.baudrate, timeout=1)
    ser.flushInput()
    ser.flushOutput()
    i = 0
    drain = ser.read()
    while len(drain) == 1:
        drain = ser.read()
        i += 1
    print "Drained {0} bytes".format(i)
    if args.test == 'prbs':
        begin_prbs_test(ser)
    elif args.test == 'throughput':
        begin_throughput_test(ser, 'a')
    elif args.test == 'baremetal':
        begin_throughput_test(ser, 'c')
    else:
        print "{0} test is not implemented".format(args.test)
