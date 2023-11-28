# Alternative firmware for cheap ($6) 12 volt spot welding batteries based on PIC12F microcontroller (PIC12F683 or PIC12F1822 is supported)

[![leap-splash](./Image/DIY-Spot-Welder-Kit-Portable-12V-Battery-Storage-Spots-Welding-Machine-Printed-Circuit-Board-Welding-Equipment.jpg_Q90.jpg_.jpg?raw=true)](123)
[![leap-splash](./Image/Spot_Welding_PIC12F683-Proteus-8-Professional-Schematic-Capture-28.11.2023-14_20_44.png?raw=true)](123)

The original firmware of the unknown microcontroller has six modes from 5ms to 20ms point delay (if you have wires longer than 30cm, the delay must be longer than 20ms - this is a significant drawback and is why this firmware was written)
The pinout of the original microcontroller in this spot weld is compatible with the PIC12F family of 8 pin microcontrollers.

* Alternative firmware, what is added:
    *  This firmware also has six modes from 5ms to 20ms, but on the 6th mode you can enter the sub-mode in which you can adjust the last value from 20ms (in +3ms steps) to 50ms - this is necessary in case you have a long wire or you want to solder a more current. In the sub-mode the selected mode is saved and in the future (even after power-off the module) it is enough just to switch to the sixth mode.
    *  By default the last chosen modes is not saved (in case the board is power-off), but this option can be enabled when compiling/recompiling the firmware SAVE_LAST_MODE.
    *  Buzzer beeps only when switching mode and only during welding. Unlike the original firmware, the buzzer does beep a certain number of times in proportion to the selected mode (it is annoying), but this option also can be enabled when compiling/recompiling the firmware BEEP_CHANGE_MODE.

Link to this model of spot welder:
https://www.aliexpress.com/item/1005006198373222.html

How to flash this microcontroller without the required proprietary programmer using Arduino:
https://community.alexgyver.ru/threads/arduino-programmator-pic-ov.4279/
