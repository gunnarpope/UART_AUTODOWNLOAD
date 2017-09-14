# UART_Autodownload
* Code to automate the download of data from a microcontroller into a python array, where it can then be processed, plotted, or sent to web.

by: Gunnar Pope <gunnar.c.pope.th@dartmouth.edu>  
and  
Caroline Vanacore <caroline.r.vanacore.17@dartmouth.edu>  

# Setup
* Open Code Compose Studio
  * Set the workspace directory as UART_AUTODOWNLOAD/CCS_Projects/
* Open the project UART_AUTODOWNLOAD and debug the main.c file to run the program

# Launchboard Setup
* See CCS_Projects/UART_AUTODOWNLOAD/main.c for the pinout to connect the UART cables
* P2.6 -> Orange
* P2.5 -> Yellow
* GND MSP430 -> GND UART cable
* UART baud rate = 115200

# Open a serial connection  


### Open the terminal on a mac and locate the connected USBserial device
```
~$ ls /dev/ | grep 'tty.usbserial'
tty.usbserial-FTXOGKV5
```

### Open a serial connection using the screen program (you may have to download this package) and set the baud rate to 115200 bps
```
~$ screen /dev/tty.usbserial-FTXOGKV5 115200
```

### Now, you should be able to tx and rx bytes from the MSP430 in the terminal

# To close the serial session:  
Type Ctrl-a, then Ctrl-\, and press 'y' for yes


## Use version Python 3.x for all code on laptop
* See python's pySerial package for reading from serial terminal
* https://pythonhosted.org/pyserial/



