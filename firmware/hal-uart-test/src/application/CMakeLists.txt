
FIND_PACKAGE("prbs" REQUIRED)

SET(application_SRCS main.c)
LIST(APPEND deps "hal-uc-core hal-uc-gpio hal-uc-uart prbs")
ADD_PLATFORM_EXECUTABLE(firmware ${deps} ${application_SRCS})
