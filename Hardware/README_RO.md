# ESPbutton
Are patru moduri diferite de functionare:

- alimentat din baterie cu activare semnal sus pe B1-B4
- alimentat tensiune CC 5V cu activare semnal sus pe B1-B4
- alimentat tensiune CC 5V cu activare semnal jos pe B1-B4
- alimentat tensiune CC 5V cu activare semnal jos pe B1-B4 si emulare intrerupator (trimite mesaje atat la inchidere cat si la deschidere contact)

# DS18B20
Pentru a utiliza senzorul de temperatura digital DS18B20 se populeaza IC3 pe fata cablajului si R4 pe spate.

# ESP-01
Nu se populeaza R9 si R10.

# ESP-01S
Nu se populeaza R9 si R10.
Se taie traseul intre pinii CH_PD si RST ai IC1 de pe fata cablajului conform pozei.
<br><img title="ESP-01S mod" src="https://github.com/cctweaker/espbutton/blob/master/Hardware/ESP01-S mod.jpg?raw=true">

# baterie / semnal sus
Nu se populeaza R1 si R2 pe fata cablajului.

# alimentare CC / semnal sus
Nu se populeaza R2 pe fata cablajului si R3, C3, D1, D2 pe spate.

# alimentare CC / semnal jos
Nu se populeaza R2 pe fata cablajului si R3, R5, R6, R7, R8, C3, D1, D2 pe spate.

# Errata
- pe fata cablajului, R5 si R6 situate langa IC2 sunt de fapt R5A si R6A, ambii rezistori cu valoarea 4.7K
- IC2 pe spatele cablajului, langa +5V este U2 (XC6210)

# PCB example
<img title="ESPbutton v1.1 fata" src="https://github.com/cctweaker/espbutton/blob/master/Hardware/ESPbutton v1.1 top example.jpg?raw=true"> <img title="ESPbutton v1.1 verso" src="https://github.com/cctweaker/espbutton/blob/master/Hardware/ESPbutton v1.1 bottom example.jpg?raw=true">