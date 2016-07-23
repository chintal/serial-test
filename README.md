
# Script for Basic Serial Communications Testing #


This script is intended for basic testing of serial links to embedded
systems over RS-232/UART/USB-CDC.

## Running the Test ##

$ python serial-test.py -h

### Baremetal Test ###

Test for actual hardware link thoughput by elimination all possible
firmware bottlenecks.

### Throughput Test ###

Similar to the Baremetal Test, but measure effective throughput using
whatever firmware architecture exists for link management, including
whatever buffering method exists. This does not include any higher
level protocol overhead.

### PRBS Test ###

Similar to the throughput test, but uses a PRBS instead of a simply
incrementing byte value. While this is theoretically a 'better' test,
in reality it likely does not make much difference for regular UART
links. The actual generation of the PRBS, however, does take finite
time and has been found to be a limiting step on the firmware side,
making it unsuitable for testing throughput capability.

### Protocol Tests ###

(NOT YET IMPLEMENTED)

Test for throughput including higher level protocol overhead. This is
not currently implemented.


## PRBS Compile and Usage ##

(PRBS no longer bundled here. Doc TBD)

This script requires a simple C PRBS library to generate pseudo random
bitstreams. A version of the library of the library is bundled here for
convenience. The same library in a form compatible with `cmake` and for
static linkage with embedded code can be found at
https://github.com/chintal/ebs-lib-prbs . If that library is to be used
instead, all references to `_prbs` in the script should be changed to
`prbs` instead, and the host build of ebs-lib-prbs should be used to
install the library to your python environment.

The bundled `prbs` library allows the generation of pseudo random bitstream
for testing. This library must be compiled and converted to an importable
python library using swig in order to function.

### Compile Commands ###

    $ cd src
    $ swig -python prbs.i
    $ gcc -c prbs.c prbs_wrap.c -I/usr/include/python2.7 -fPIC
    $ ld -shared prbs.o prbs_wrap.o -o ../_prbs.so

### Basic Usage Example ###

    >>> import _prbs
    >>> prbs = _prbs.lfsr16_t()
    >>> _prbs.lfsr_vInit(prbs, int(_prbs.LFSR_DEFAULT_SEED), int(_prbs.LFSR_DEFAULT_TAPS))
    >>> while(1):
    ...     print _prbs.lfsr_cGetNextByte(prbs)


## Firmware Preparation ##

TBD
