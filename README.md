# Nmea2000Monitor
NMEA 2000 monitor built on top of Candle API.
The results are sent over to the server.

# Usage
* Just start the program. It will start automatically.

# Building

1. git submodule update --init --recursive
2. open the project file Nmea2000Monitor.sln in MSVS
3. build

# Development notes
On linux, the device driver is recognized as follows in the dmesg:
```[  246.993509] usb 3-1: new full-speed USB device number 5 using xhci_hcd
[  247.143040] usb 3-1: New USB device found, idVendor=1d50, idProduct=606f, bcdDevice= 0.00
[  247.143053] usb 3-1: New USB device strings: Mfr=1, Product=2, SerialNumber=3
[  247.143059] usb 3-1: Product: canable gs_usb
[  247.143064] usb 3-1: Manufacturer: canable.io
[  247.143068] usb 3-1: SerialNumber: 003000315943570520353938
[  247.144573] gs_usb 3-1:1.0: Configuring for 1 interfaces
```

To setup the CAN interface, run:
```sudo ip link set can0 up type can bitrate 250000```

To send a message from, run:
```cansend can0 993#AAADBEEE```

