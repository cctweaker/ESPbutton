# ESPbutton
It has 4 main ways to work:

- battery powered with high level activation on B1-B4
- DC powered with high level activation on B1-B4
- DC powered with low level activation on B1-B4
- DC powered with low level activation on B1-B4 but emulating a rocker switch

# DS18B20
To use the DS18B20 temperature sensor, please populate IC3 and R4.

# ESP-01
Do not populate R9 and R10.

# ESP-01S
Do not populate R9 and R10.
Break away connection on top of PCB between CH_PD and RST on IC1 pins.
<br><img title="ESP-01S mod" src="https://github.com/cctweaker/espbutton/blob/master/Hardware/ESP01-S mod.jpg?raw=true">

# battery power / high
Do not populate R1 and R2 on top of the PCB.

# DC power / high
Do not populate R2 on top of the PCB and R3, C3, D1, D2 on the bottom.

# DC power / low
Do not populate R2 on top of the PCB and R3, R5, R6, R7, R8, C3, D1, D2 on the bottom.

# Errata
- R5 and R6 next to IC2 on top of the PCB should read R5A and R6A, both 4.7K
- IC2 on the bottom of the PCB next to +5V pad should read U2 and is the XC6210

# PCB example
<img title="ESPbutton v1.1 TOP" src="https://github.com/cctweaker/espbutton/blob/master/Hardware/ESPbutton v1.1 top example.jpg?raw=true"> <img title="ESPbutton v1.1 BOTTOM" src="https://github.com/cctweaker/espbutton/blob/master/Hardware/ESPbutton v1.1 bottom example.jpg?raw=true">