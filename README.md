# curro32
Playing with ESP32 and Bittle


# Getting tensorflow lite on ESP32
Tutorials:
https://www.survivingwithandroid.com/run-tensorflow-lite-esp32-platformio/
https://github.com/atomic14/tensorflow-lite-esp32



# TODO

## Switch MPU library to something maintained
We are using Jeff Rowberg's MPU6050 library.
Unfortunately it fails to compile because it requires a macro
"BUFFER_LENGTH" be defined in Wire.h, which won't be there for all boards.
The issue is discussed in the link below. The easiest way to move on is to modify the
library with 
#define BUFFER_LENGTH 1024
https://forum.arduino.cc/t/samd21-and-i2cdevlib/488628/2

another conflict:

https://github.com/jrowberg/i2cdevlib/issues/346
