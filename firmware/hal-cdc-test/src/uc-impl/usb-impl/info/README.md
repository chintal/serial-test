Substantial changes from USB Developers Package
===============================================

  - Includes for driverlib changed from `#include "driverlib.h"` to 
    `#include <msp430-driverlib/MSP430F5xx_6xx/driverlib.h>`. This is done to 
    make the library compatible with a cmake based build system acting on 
    pre-compiled, statically linked libraries. Note that this approach has 
    many other downsides, not the least of which is that link-time 
    optimization is far less effective than compile-time optimization.
        
  - `hal.h` includes eliminated. The content of that layer is left to the 
    application, which must ensure that the functions performed by that layer 
    (IO port configuration and Clock systtem initialization) are done by the 
    application code.
      
  - The USB_API code seems to be arranged under the assumption that all code 
    is compiled in one go. Many function calls would result by having multiple 
    libraries linked together after compilation. In order to accomodate this 
    structure (or minimize the changes necessary to accomodate it in the 
    future), the cmake-compatible library defined includes all USB_API and 
    associated source files, including application specific USB_config. 
    Related functions in the driverlib which are used in critical places and 
    are small enough to be inlined without increasing their footprint relative 
    to the cost of the function call have been moved from their C files to the 
    corresponding header files to be inlined.
    
  - The content of USB_app is discarded in favor of a custom implementation in 
    uc-impl/usb_impl.{c/h} and uc-impl/usb_event_handlers.{c/h}, possibly to be 
    based on the content of USB_app/usbConstructs.{c/h} and 
    USB_app/usbEventHandling.c. Similarly, USB_config/UsbIsr.c is moved into 
    uc-impl/usb_handlers.{c/h}, in-line with the pattern for captive interrupt 
    handler functions.
    
  - Deprecated files are nuked wholesale. The API is messy enough as it is.
      - `USB_Common/types.h`
  
  - Large deprecated chunks from other areas of the API are nuked when 
    readily recognizable as unnecessary. 
     - `USB_Common/usb.h`, all the kUSB definitions.
     - `USB_CDC_API/UsbCdc.c`, all the kUSB definitions.
     - ... other similar
     
  - `USB_API` cannot be used as a precompiled library as long as the 
    application is allowed to specify its descriptors within `descriptors.h` 
    and `descriptors.c`, which is used by all the other USBAPI files to 
    control its compilation. Due to this, we therefore assume the integration 
    of `USB_API` into the application code to be unavoidable. As such, 
    `USB_API` and associated code is to reside in `uc-impl`, along with 
    (for now) a standardized set of descriptors. Applications desiring 
    customized descriptors must make the necessary changes to the library 
    in the uC implementation layer.
    
  - Delays in `usb.c` and `usb_irq_handler.c` have been replaced to use 
    standard driverlib functions instead. The `USB_determineFreq()` function 
    in the original USBAPI implementation is eliminated. When the application 
    does not use MCLK frequency scaling (controlled in `application.h` by 
    APP_ENABLE_MCLK_SCALING), the GCC delay intrinsic is used to provide a 
    more size-efficient delay implementation.

Interrupt Handlers
------------------

UsbIsr.c (uc-impl/usb_handlers.c)

  - Removed IAR and TI C Compiler preprocessor support structures around the 
    ISR's attribute definition. This makes the code KDevelop parser friendly.
  
  - `__even_in_range` is a preprocessor definition that apparently does 
    nothing. Usage of this is eliminated. Again, helps KDevelop parse the
    code. 

    
USB_Common
----------

  - Added `stdint.h` include to `usb.h`. 
  
  - There is a strange `__no_init` preprocessor #define in `usb.h` and `usb.c`. 
    This has been replaced with `__no_init_usb` in order to avoid the compiler 
    warnings it produces. Whether or not this is a valid change is not 
    definitively known.
    
  - Got rid of the version string in the executable. While it's nice and such, 
    there really is no way version strings for every included library can 
    reside in final executables. 
  
  - (CONSIDER) Replace `usbdma.c` with standardized interfaces from the HAL.
        
USBCDC
------

Major refactor of the entire USB-API layer for USBCDC. The high level transfer 
control functions of the API are eliminated in favour of providing direct 
access to the application / HAL layer of the USB interface. This is done to 
reduce the overall API depth and enhance memory utilization and throughput. 

The following are the primary assumptions made to reach the conclusion that 
the API should be stripped down : 

  - RAM is more valuable than ease of programming. The original API structure 
    utilized dedicated buffers in USB RAM which are only written to by the 
    API. This means that application data first needs to be packed into the 
    transmittable form in yet another buffer, and copied out. 
     
  - Memcpy, even with DMA, is not something that we want to do all that much. 
    It's slow and blocks the CPU.
        
  - Large pre-prepared transmit buffers are unlikely to ever show up in 
    regular use. It's not like we have that much RAM to start with. 
        
  - USB CDC API should behave along similar lines to the UART API, so that 
    application code can be switched from one to the other. This layer now 
    niether knows nor cares about high level flow control.
        
Application code / HAL is now responsible for the following:

  - When application code wants to write to USBCDC, it should obtain write 
    locks and write the data directly into the appropriate USB endpoint buffer
    using the USBCDC_send API functions. 
    
  - The application / HAL should ensure the transport continues to send by 
    repeated calls to USBCDC_sendTrigger(). 
    
  - TODO The trigger stream may not actually be necessary in the case of 
    USBCDC. USDCDC_commitWriteBuffer() provides a convenient hook to provide
    the trigger at the point where it may be necessary. Howevever, this should 
    be tested.
    
  - In case the application requires transmission of partial packets, 
    USBCDC_sendFlush() should be used. However, note that this will eat into 
    available USBCDC bandwidth, and even more so when zero packet transmission 
    is necessary. Use sparingly.
    
  - When application code wants to read from USBCDC, it should wait until 
    there are sufficient bytes available in the endpoint buffers, and then 
    read using the USBCDC_receive API functions.
    
  - When data in the USB buffer is not needed by the application, it should 
    clear the recieve buffers using USBCDC_rejectData to allow subsequent 
    packets to be recieved. 
        
As a result, the following changes are made: 
        
  - The CDC send complete, recieve complete, data recieved event handlers are 
    eliminated entirely. Most of these events are no longer relevant / exist, 
    since the high-level API is eliminated. If the application needs to 
    monitor for one or more of the deprecated events, it is expected to poll 
    for this information from within a state machine.
  
  - Large swathes of the USBCDC high level transmission API are eliminated. 
  
  - CdcWriteCtrl and CdcReadCtrl are stripped down to hold a single event 
    handler which can be installed by the application / HAL. Addtionally,
    Buffer Control structures are added to simplify access to the buffers 
    from the application side during the construction phase.
    
  - The interrupt handlers are stripped down, and will now only call callbacks 
    (which can be installed by the application / HAL layer) as needed. 
    Low-level buffer control is implemented in the handler and should be 
    sufficient for most applications. A decoupled state-machine based 
    implementation may not need to use additional callbacks. If used, though, 
    these callbacks should be kept small and simple, as they will be called 
    from an interrupt context. The driver level Buffer and Interface control 
    structures can be polled to obtain the necessary information, either 
    directly or using the getInterfaceStatus API functions.
  
  - USBCDC_getInterfaceStatus has been fleshed out with multiple variants, 
    hopefully providing optimal status access interfaces for the application. 
    The associated defines for the return values have been changed.
