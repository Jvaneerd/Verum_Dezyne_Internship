This program is built using https://github.com/sudar/Arduino-Makefile to compile for and upload to an Arduino Uno.
Currently, it is only tested on Linux and MacOS.

To compile and upload this program, there are a few requirements:
- Download Dezyne from http://dezyne.verum.com and run it to install the dzn command line client.
Add the .npm directories that are mentioned to your PATH environment variable
- Follow the instructions on https://github.com/sudar/Arduino-Makefile to install the Arduino-mk package.
- On MacOS, install the Arduino IDE from https://www.arduino.cc
- Configure the following variables in the makefile supplied with this project:
	- PROJECT_DIR: Set to where your project directory is located
	- ARDMK_DIR:
		- on Linux: /usr/share/arduino
		- on MacOS: /usr/local/bin/Arduino-Makefile (when installed with homebrew)
	- ARDUINO_DIR:
		- on Linux: /usr/share/arduino
		- on MacOS: /Applications/Arduino.app/Contents/Java
	- AVR_TOOLS_DIR:
		- on Linux: /usr
        - on MacOS: /usr/local
    - AVRDUDE:
   	    - on Linux: /usr/bin/avrdude
   		- on MacOS: /usr/local/bin/avrdude
   	- MONITOR_PORT:
   		- on Linux: /dev/ttyACM*
   		- on MacOS: /dev/tty.usbmodem*

- This makefile assumes that the folder structure remains untouched:
	- .dzn files are in Dezyne_Models
	- Handwritten files are in src/Handwritten
	- Dezyne library files are in lib/

    If you want to change this structure, look for the *MDLDIR*, *LOCAL_C_SRCS* and *DZN_FILES* variables in the makefile.

- Before the project can be compiled, code must be generated. This is done with `make generate`.
- When code is generated, the generated code will be placed in src/**Generated**. Generating code requires an active internet connection and authentication with Dezyne.
- After the code is generated, `make` will compile the Arduino binary.
- When the project is compiled, `make upload` will upload the binary to your Arduino. Based on user privileges, sudo may be required.
