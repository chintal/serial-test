Substantial changes from USB Developers Package
===============================================

  - Includes for driverlib changed from `#include "driverlib.h"` 
    to `#include <msp430-driverlib/MSP430F5xx_6xx/driverlib.h>`. This is 
    done to make the library compatible with a cmake based build system 
    acting on pre-compiled, statically linked libraries. Note that this
    approach has many other downsides, not the least of which is that 
    link-time optimization is far less effective than compile-time 
    optimization.
        
  - `hal.h` includes eliminated. The content of that layer is left to 
    the application, which must ensure that the functions performed by 
    that layer (IO port configuration and Clock systtem initialization)
    are done by the application code.
      
  - The USB_API code seems to be arranged under the assumption that all 
    code is compiled in one go. Many function calls would result by 
    having multiple libraries linked together after compilation. In 
    order to accomodate this structure (or minimize the changes 
    necessary to accomodate it in the future), the cmake-compatible
    library defined includes all USB_API and associated source files, 
    including application specific USB_config. Related functions in 
    the driverlib which used in critical places and are small enough 
    to be inlined without increasing their footprint relative to the 
    cost of the function call have been moved from their C files to 
    the corresponding header files to be inlined.
    
  - The content of USB_app is discarded in favor of a custom implementation
    in uc-impl/usb_impl.{c/h} and uc-impl/usb_event_handlers.{c/h}, possibly 
    to be based on the content of USB_app/usbConstructs.{c/h} and 
    USB_app/usbEventHandling.c. Similarly, USB_config/UsbIsr.c is moved into 
    uc-impl/usb_handlers.{c/h}, in-line with the pattern for captive 
    interrupt handler functions.
    
  - Deprecated files are nuked wholesale. The API is messy enough as it is.
      - `USB_Common/types.h`
  
  - Large deprecated chunks from other areas of the API are nuked when 
    readily recognizable as unnecessary. 
     - `USB_Common/usb.h`, all the kUSB definitions.
     - `USB_CDC_API/UsbCdc.c`, all the kUSB definitions.
     
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
  
  - There is a strange `__no_init` preprocessor #define in `usb.h` and 
    `usb.c`. This has been replaced with `__no_init_usb` in order to 
    avoid the compiler warnings it produces. Whether or not this is a 
    valid change is not definitively known.
    
  - Got rid of the version string in the executable. While it's nice and 
    such, there really is no way version strings for every included library 
    can reside in final executables. 
  
  - (CONSIDER) Replace `usbdma.c` with standardized interfaces from the HAL.
        