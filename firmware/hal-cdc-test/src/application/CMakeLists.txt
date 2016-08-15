
FIND_PACKAGE("prbs" REQUIRED)

SET(application_SRCS main.c)
LIST(APPEND deps "hal-uc-core hal-uc-gpio hal-uc-uart prbs msp430-usbapi")

SET(EXTRA_LINKER_FLAGS "-T ${CMAKE_SOURCE_DIR}/uc-impl/usb-impl/USB_API/msp430USB.ld")
ADD_PLATFORM_EXECUTABLE(firmware ${deps} ${application_SRCS})
