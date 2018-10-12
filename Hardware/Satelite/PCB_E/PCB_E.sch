EESchema Schematic File Version 4
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Connector_Generic:Conn_02x02_Counter_Clockwise J20
U 1 1 5BC13D98
P 7900 1650
F 0 "J20" H 7950 1868 50  0000 C CNN
F 1 "Conn_02x02_Counter_Clockwise" H 7950 1776 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x02_P2.54mm_Vertical" H 7900 1650 50  0001 C CNN
F 3 "https://www.mouser.com/datasheet/2/527/ssq_th-1370157.pdf" H 7900 1650 50  0001 C CNN
F 4 "SSQ-102-03-T-D" H 7900 1650 50  0001 C CNN "Manufacturer Part Number"
	1    7900 1650
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR014
U 1 1 5BC13D9F
P 10400 1550
F 0 "#PWR014" H 10400 1400 50  0001 C CNN
F 1 "+3V3" H 10414 1725 50  0000 C CNN
F 2 "" H 10400 1550 50  0001 C CNN
F 3 "" H 10400 1550 50  0001 C CNN
	1    10400 1550
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR07
U 1 1 5BC13DA5
P 9000 1550
F 0 "#PWR07" H 9000 1400 50  0001 C CNN
F 1 "+3V3" H 9014 1725 50  0000 C CNN
F 2 "" H 9000 1550 50  0001 C CNN
F 3 "" H 9000 1550 50  0001 C CNN
	1    9000 1550
	1    0    0    -1  
$EndComp
Wire Wire Line
	9950 1650 10400 1650
Wire Wire Line
	10400 1650 10400 1550
Wire Wire Line
	9450 1650 9000 1650
Wire Wire Line
	9000 1650 9000 1550
$Comp
L power:GND #PWR09
U 1 1 5BC13DAF
P 9350 1750
F 0 "#PWR09" H 9350 1500 50  0001 C CNN
F 1 "GND" H 9354 1576 50  0000 C CNN
F 2 "" H 9350 1750 50  0001 C CNN
F 3 "" H 9350 1750 50  0001 C CNN
	1    9350 1750
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR012
U 1 1 5BC13DB5
P 10050 1750
F 0 "#PWR012" H 10050 1500 50  0001 C CNN
F 1 "GND" H 10054 1576 50  0000 C CNN
F 2 "" H 10050 1750 50  0001 C CNN
F 3 "" H 10050 1750 50  0001 C CNN
	1    10050 1750
	1    0    0    -1  
$EndComp
Wire Wire Line
	9950 1750 10050 1750
Wire Wire Line
	9350 1750 9450 1750
Wire Wire Line
	10400 2100 10400 2200
Wire Wire Line
	10400 2200 9950 2200
$Comp
L power:+BATT #PWR016
U 1 1 5BC13DBF
P 10650 2100
F 0 "#PWR016" H 10650 1950 50  0001 C CNN
F 1 "+BATT" H 10664 2275 50  0000 C CNN
F 2 "" H 10650 2100 50  0001 C CNN
F 3 "" H 10650 2100 50  0001 C CNN
	1    10650 2100
	1    0    0    -1  
$EndComp
Wire Wire Line
	10650 2100 10650 2300
Wire Wire Line
	10650 2300 9950 2300
$Comp
L power:+BATT #PWR015
U 1 1 5BC13DC7
P 10400 2100
F 0 "#PWR015" H 10400 1950 50  0001 C CNN
F 1 "+BATT" H 10414 2275 50  0000 C CNN
F 2 "" H 10400 2100 50  0001 C CNN
F 3 "" H 10400 2100 50  0001 C CNN
	1    10400 2100
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR08
U 1 1 5BC13DCD
P 9000 2100
F 0 "#PWR08" H 9000 1950 50  0001 C CNN
F 1 "+5V" H 9014 2275 50  0000 C CNN
F 2 "" H 9000 2100 50  0001 C CNN
F 3 "" H 9000 2100 50  0001 C CNN
	1    9000 2100
	1    0    0    -1  
$EndComp
Wire Wire Line
	9000 2100 9000 2200
Wire Wire Line
	9000 2200 9450 2200
$Comp
L power:GND #PWR010
U 1 1 5BC13DD5
P 9350 2300
F 0 "#PWR010" H 9350 2050 50  0001 C CNN
F 1 "GND" H 9354 2126 50  0000 C CNN
F 2 "" H 9350 2300 50  0001 C CNN
F 3 "" H 9350 2300 50  0001 C CNN
	1    9350 2300
	1    0    0    -1  
$EndComp
Wire Wire Line
	9450 2300 9350 2300
Wire Wire Line
	7700 1650 7250 1650
Text Label 9050 2750 0    50   ~ 0
MOSI
Text Label 9050 2850 0    50   ~ 0
MISO
Text Label 10150 2750 0    50   ~ 0
SCLK
Text Label 10250 2850 0    50   ~ 0
CS
Wire Wire Line
	9950 3300 10450 3300
Text Label 10250 3300 0    50   ~ 0
SDA
Wire Wire Line
	9450 3300 9000 3300
Text Label 9050 3300 0    50   ~ 0
SCL
$Comp
L power:GND #PWR011
U 1 1 5BC13DE8
P 9350 3450
F 0 "#PWR011" H 9350 3200 50  0001 C CNN
F 1 "GND" H 9354 3276 50  0000 C CNN
F 2 "" H 9350 3450 50  0001 C CNN
F 3 "" H 9350 3450 50  0001 C CNN
	1    9350 3450
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR013
U 1 1 5BC13DEE
P 10050 3450
F 0 "#PWR013" H 10050 3200 50  0001 C CNN
F 1 "GND" H 10054 3276 50  0000 C CNN
F 2 "" H 10050 3450 50  0001 C CNN
F 3 "" H 10050 3450 50  0001 C CNN
	1    10050 3450
	1    0    0    -1  
$EndComp
Wire Wire Line
	10050 3450 10050 3400
Wire Wire Line
	10050 3400 9950 3400
Wire Wire Line
	9350 3450 9350 3400
Wire Wire Line
	9350 3400 9450 3400
Wire Wire Line
	8650 2550 8200 2550
Wire Wire Line
	7250 2650 7700 2650
Wire Wire Line
	8200 2650 8650 2650
Wire Wire Line
	7250 2550 7700 2550
Text Label 7300 2550 0    50   ~ 0
AD0
Text Label 7300 2650 0    50   ~ 0
AD1
Text Label 8450 2650 0    50   ~ 0
AD2
Text Label 8450 2550 0    50   ~ 0
AD3
Wire Wire Line
	7700 1750 7250 1750
Wire Wire Line
	7250 2100 7700 2100
Wire Wire Line
	7700 2200 7250 2200
Wire Wire Line
	8200 2100 8650 2100
Wire Wire Line
	8650 2200 8200 2200
Wire Wire Line
	8200 1750 8650 1750
Wire Wire Line
	8650 1650 8200 1650
Text Label 7300 1650 0    50   ~ 0
IO0
Text Label 7300 1750 0    50   ~ 0
IO1
Text Label 8450 1750 0    50   ~ 0
IO2
Text Label 8450 1650 0    50   ~ 0
IO3
Text Label 7300 2100 0    50   ~ 0
IO4
Text Label 7300 2200 0    50   ~ 0
IO5
Text Label 8450 2200 0    50   ~ 0
IO6
Text Label 8450 2100 0    50   ~ 0
IO7
Text Notes 7850 1000 0    118  ~ 0
Conector de expansión
Wire Notes Line
	7150 700  10950 700 
Wire Notes Line
	10950 700  10950 3750
Wire Notes Line
	10950 3750 7150 3750
Wire Notes Line
	7150 3750 7150 700 
$Comp
L Connector_Generic:Conn_02x02_Counter_Clockwise J21
U 1 1 5BC13E16
P 7900 2100
F 0 "J21" H 7950 2318 50  0000 C CNN
F 1 "Conn_02x02_Counter_Clockwise" H 7950 2226 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x02_P2.54mm_Vertical" H 7900 2100 50  0001 C CNN
F 3 "https://www.mouser.com/datasheet/2/527/ssq_th-1370157.pdf" H 7900 2100 50  0001 C CNN
F 4 "SSQ-102-03-T-D" H 7900 2100 50  0001 C CNN "Manufacturer Part Number"
	1    7900 2100
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_02x02_Counter_Clockwise J22
U 1 1 5BC13E1E
P 7900 2550
F 0 "J22" H 7950 2768 50  0000 C CNN
F 1 "Conn_02x02_Counter_Clockwise" H 7950 2676 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x02_P2.54mm_Vertical" H 7900 2550 50  0001 C CNN
F 3 "https://www.mouser.com/datasheet/2/527/ssq_th-1370157.pdf" H 7900 2550 50  0001 C CNN
F 4 "SSQ-102-03-T-D" H 7900 2550 50  0001 C CNN "Manufacturer Part Number"
	1    7900 2550
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_02x02_Counter_Clockwise J23
U 1 1 5BC13E26
P 9650 1650
F 0 "J23" H 9700 1868 50  0000 C CNN
F 1 "Conn_02x02_Counter_Clockwise" H 9700 1776 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x02_P2.54mm_Vertical" H 9650 1650 50  0001 C CNN
F 3 "https://www.mouser.com/datasheet/2/527/ssq_th-1370157.pdf" H 9650 1650 50  0001 C CNN
F 4 "SSQ-102-03-T-D" H 9650 1650 50  0001 C CNN "Manufacturer Part Number"
	1    9650 1650
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_02x02_Counter_Clockwise J24
U 1 1 5BC13E2E
P 9650 2200
F 0 "J24" H 9700 2418 50  0000 C CNN
F 1 "Conn_02x02_Counter_Clockwise" H 9700 2326 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x02_P2.54mm_Vertical" H 9650 2200 50  0001 C CNN
F 3 "https://www.mouser.com/datasheet/2/527/ssq_th-1370157.pdf" H 9650 2200 50  0001 C CNN
F 4 "SSQ-102-03-T-D" H 9650 2200 50  0001 C CNN "Manufacturer Part Number"
	1    9650 2200
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_02x02_Counter_Clockwise J26
U 1 1 5BC13E3E
P 9650 3300
F 0 "J26" H 9700 3518 50  0000 C CNN
F 1 "Conn_02x02_Counter_Clockwise" H 9700 3426 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x02_P2.54mm_Vertical" H 9650 3300 50  0001 C CNN
F 3 "https://www.mouser.com/datasheet/2/527/ssq_th-1370157.pdf" H 9650 3300 50  0001 C CNN
F 4 "SSQ-102-03-T-D" H 9650 3300 50  0001 C CNN "Manufacturer Part Number"
	1    9650 3300
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_02x05_Counter_Clockwise J4
U 1 1 5BC18F9C
P 2750 2150
F 0 "J4" V 2754 2430 50  0000 L CNN
F 1 "Conn_02x05_Counter_Clockwise" V 2845 2430 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_2x05_P2.54mm_Vertical" H 2750 2150 50  0001 C CNN
F 3 "~" H 2750 2150 50  0001 C CNN
	1    2750 2150
	0    1    1    0   
$EndComp
$Comp
L Connector_Generic:Conn_02x05_Counter_Clockwise J5
U 1 1 5BC1900E
P 2750 2900
F 0 "J5" V 2754 3180 50  0000 L CNN
F 1 "Conn_02x05_Counter_Clockwise" V 2845 3180 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_2x05_P2.54mm_Vertical" H 2750 2900 50  0001 C CNN
F 3 "~" H 2750 2900 50  0001 C CNN
	1    2750 2900
	0    1    1    0   
$EndComp
$Comp
L Connector_Generic:Conn_02x14_Odd_Even J1
U 1 1 5BC1EE67
P 1200 4000
F 0 "J1" H 1250 4817 50  0000 C CNN
F 1 "Conn_02x14_Odd_Even" H 1250 4726 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x14_P2.54mm_Vertical" H 1200 4000 50  0001 C CNN
F 3 "~" H 1200 4000 50  0001 C CNN
	1    1200 4000
	1    0    0    -1  
$EndComp
Wire Wire Line
	2550 2450 2650 2450
Connection ~ 2650 2450
Wire Wire Line
	2650 2450 2750 2450
Connection ~ 2750 2450
Wire Wire Line
	2750 2450 2850 2450
Connection ~ 2850 2450
Wire Wire Line
	2850 2450 2950 2450
Connection ~ 2950 2450
Wire Wire Line
	2950 2450 3100 2450
Wire Wire Line
	2550 1950 2650 1950
Connection ~ 2650 1950
Wire Wire Line
	2650 1950 2750 1950
Connection ~ 2750 1950
Wire Wire Line
	2750 1950 2850 1950
Connection ~ 2850 1950
Wire Wire Line
	2850 1950 2950 1950
Connection ~ 2950 1950
Wire Wire Line
	2950 1950 3100 1950
Wire Wire Line
	2550 3200 2650 3200
Connection ~ 2650 3200
Wire Wire Line
	2650 3200 2750 3200
Connection ~ 2750 3200
Wire Wire Line
	2750 3200 2850 3200
Connection ~ 2850 3200
Wire Wire Line
	2850 3200 2950 3200
Connection ~ 2950 3200
Wire Wire Line
	2950 3200 3100 3200
Wire Wire Line
	2550 2700 2650 2700
Connection ~ 2650 2700
Wire Wire Line
	2650 2700 2750 2700
Connection ~ 2750 2700
Wire Wire Line
	2750 2700 2850 2700
Connection ~ 2850 2700
Wire Wire Line
	2850 2700 2950 2700
Connection ~ 2950 2700
Wire Wire Line
	2950 2700 3100 2700
$Comp
L Connector_Generic:Conn_02x05_Counter_Clockwise J6
U 1 1 5BC2FF18
P 2750 3550
F 0 "J6" V 2754 3830 50  0000 L CNN
F 1 "Conn_02x05_Counter_Clockwise" V 2845 3830 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_2x05_P2.54mm_Vertical" H 2750 3550 50  0001 C CNN
F 3 "~" H 2750 3550 50  0001 C CNN
	1    2750 3550
	0    1    1    0   
$EndComp
Wire Wire Line
	2550 3850 2650 3850
Connection ~ 2650 3850
Wire Wire Line
	2650 3850 2750 3850
Connection ~ 2750 3850
Wire Wire Line
	2750 3850 2850 3850
Connection ~ 2850 3850
Wire Wire Line
	2850 3850 2950 3850
Connection ~ 2950 3850
Wire Wire Line
	2950 3850 3100 3850
Wire Wire Line
	2550 3350 2650 3350
Connection ~ 2650 3350
Wire Wire Line
	2650 3350 2750 3350
Connection ~ 2750 3350
Wire Wire Line
	2750 3350 2850 3350
Connection ~ 2850 3350
Wire Wire Line
	2850 3350 2950 3350
Connection ~ 2950 3350
Wire Wire Line
	2950 3350 3100 3350
$Comp
L Connector_Generic:Conn_02x05_Counter_Clockwise J7
U 1 1 5BC32303
P 2750 4250
F 0 "J7" V 2754 4530 50  0000 L CNN
F 1 "Conn_02x05_Counter_Clockwise" V 2845 4530 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_2x05_P2.54mm_Vertical" H 2750 4250 50  0001 C CNN
F 3 "~" H 2750 4250 50  0001 C CNN
	1    2750 4250
	0    1    1    0   
$EndComp
$Comp
L Connector_Generic:Conn_02x05_Counter_Clockwise J8
U 1 1 5BC3230A
P 2750 4950
F 0 "J8" V 2754 5230 50  0000 L CNN
F 1 "Conn_02x05_Counter_Clockwise" V 2845 5230 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_2x05_P2.54mm_Vertical" H 2750 4950 50  0001 C CNN
F 3 "~" H 2750 4950 50  0001 C CNN
	1    2750 4950
	0    1    1    0   
$EndComp
Wire Wire Line
	2550 4550 2650 4550
Connection ~ 2650 4550
Wire Wire Line
	2650 4550 2750 4550
Connection ~ 2750 4550
Wire Wire Line
	2750 4550 2850 4550
Connection ~ 2850 4550
Wire Wire Line
	2850 4550 2950 4550
Connection ~ 2950 4550
Wire Wire Line
	2950 4550 3100 4550
Wire Wire Line
	2550 4050 2650 4050
Connection ~ 2650 4050
Wire Wire Line
	2650 4050 2750 4050
Connection ~ 2750 4050
Wire Wire Line
	2750 4050 2850 4050
Connection ~ 2850 4050
Wire Wire Line
	2850 4050 2950 4050
Connection ~ 2950 4050
Wire Wire Line
	2950 4050 3100 4050
Wire Wire Line
	2550 5250 2650 5250
Connection ~ 2650 5250
Wire Wire Line
	2650 5250 2750 5250
Connection ~ 2750 5250
Wire Wire Line
	2750 5250 2850 5250
Connection ~ 2850 5250
Wire Wire Line
	2850 5250 2950 5250
Connection ~ 2950 5250
Wire Wire Line
	2950 5250 3100 5250
Wire Wire Line
	2550 4750 2650 4750
Connection ~ 2650 4750
Wire Wire Line
	2650 4750 2750 4750
Connection ~ 2750 4750
Wire Wire Line
	2750 4750 2850 4750
Connection ~ 2850 4750
Wire Wire Line
	2850 4750 2950 4750
Connection ~ 2950 4750
Wire Wire Line
	2950 4750 3100 4750
$Comp
L Connector_Generic:Conn_02x05_Counter_Clockwise J9
U 1 1 5BC32335
P 2750 5600
F 0 "J9" V 2754 5880 50  0000 L CNN
F 1 "Conn_02x05_Counter_Clockwise" V 2845 5880 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_2x05_P2.54mm_Vertical" H 2750 5600 50  0001 C CNN
F 3 "~" H 2750 5600 50  0001 C CNN
	1    2750 5600
	0    1    1    0   
$EndComp
Wire Wire Line
	2550 5900 2650 5900
Connection ~ 2650 5900
Wire Wire Line
	2650 5900 2750 5900
Connection ~ 2750 5900
Wire Wire Line
	2750 5900 2850 5900
Connection ~ 2850 5900
Wire Wire Line
	2850 5900 2950 5900
Connection ~ 2950 5900
Wire Wire Line
	2950 5900 3100 5900
Wire Wire Line
	2550 5400 2650 5400
Connection ~ 2650 5400
Wire Wire Line
	2650 5400 2750 5400
Connection ~ 2750 5400
Wire Wire Line
	2750 5400 2850 5400
Connection ~ 2850 5400
Wire Wire Line
	2850 5400 2950 5400
Connection ~ 2950 5400
Wire Wire Line
	2950 5400 3100 5400
Wire Wire Line
	1000 3400 1000 3500
Connection ~ 1000 3500
Wire Wire Line
	1000 3500 1000 3600
Connection ~ 1000 3600
Wire Wire Line
	1000 3600 1000 3700
Connection ~ 1000 3700
Wire Wire Line
	1000 3700 1000 3800
Connection ~ 1000 3800
Wire Wire Line
	1000 3800 1000 3900
Connection ~ 1000 3900
Wire Wire Line
	1000 3900 1000 4000
Connection ~ 1000 4000
Wire Wire Line
	1000 4000 1000 4100
Connection ~ 1000 4100
Wire Wire Line
	1000 4100 1000 4200
Connection ~ 1000 4200
Wire Wire Line
	1000 4200 1000 4300
Connection ~ 1000 4300
Wire Wire Line
	1000 4300 1000 4400
Connection ~ 1000 4400
Wire Wire Line
	1000 4400 1000 4500
Connection ~ 1000 4500
Wire Wire Line
	1000 4500 1000 4600
Connection ~ 1000 4600
Wire Wire Line
	1000 4600 1000 4700
Connection ~ 1000 4700
Wire Wire Line
	1000 4700 1000 4850
Wire Wire Line
	1500 3400 1500 3500
Connection ~ 1500 3500
Wire Wire Line
	1500 3500 1500 3600
Connection ~ 1500 3600
Wire Wire Line
	1500 3600 1500 3700
Connection ~ 1500 3700
Wire Wire Line
	1500 3700 1500 3800
Connection ~ 1500 3800
Wire Wire Line
	1500 3800 1500 3900
Connection ~ 1500 3900
Wire Wire Line
	1500 3900 1500 4000
Connection ~ 1500 4000
Wire Wire Line
	1500 4000 1500 4100
Connection ~ 1500 4100
Wire Wire Line
	1500 4100 1500 4200
Connection ~ 1500 4200
Wire Wire Line
	1500 4200 1500 4300
Connection ~ 1500 4300
Wire Wire Line
	1500 4300 1500 4400
Connection ~ 1500 4400
Wire Wire Line
	1500 4400 1500 4500
Connection ~ 1500 4500
Wire Wire Line
	1500 4500 1500 4600
Connection ~ 1500 4600
Wire Wire Line
	1500 4600 1500 4700
Connection ~ 1500 4700
Wire Wire Line
	1500 4700 1500 4850
$Comp
L power:GND #PWR01
U 1 1 5BC40F4B
P 1000 4850
F 0 "#PWR01" H 1000 4600 50  0001 C CNN
F 1 "GND" H 1004 4676 50  0000 C CNN
F 2 "" H 1000 4850 50  0001 C CNN
F 3 "" H 1000 4850 50  0001 C CNN
	1    1000 4850
	1    0    0    -1  
$EndComp
Wire Wire Line
	1500 4850 1650 4850
$Comp
L Connector_Generic:Conn_02x05_Counter_Clockwise J10
U 1 1 5BC4C547
P 4600 2150
F 0 "J10" V 4604 2430 50  0000 L CNN
F 1 "Conn_02x05_Counter_Clockwise" V 4695 2430 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_2x05_P2.54mm_Vertical" H 4600 2150 50  0001 C CNN
F 3 "~" H 4600 2150 50  0001 C CNN
	1    4600 2150
	0    1    1    0   
$EndComp
$Comp
L Connector_Generic:Conn_02x05_Counter_Clockwise J11
U 1 1 5BC4C54E
P 4600 2900
F 0 "J11" V 4604 3180 50  0000 L CNN
F 1 "Conn_02x05_Counter_Clockwise" V 4695 3180 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_2x05_P2.54mm_Vertical" H 4600 2900 50  0001 C CNN
F 3 "~" H 4600 2900 50  0001 C CNN
	1    4600 2900
	0    1    1    0   
$EndComp
$Comp
L Connector_Generic:Conn_02x14_Odd_Even J3
U 1 1 5BC4C555
P 1250 1900
F 0 "J3" H 1300 2717 50  0000 C CNN
F 1 "Conn_02x14_Odd_Even" H 1300 2626 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x14_P2.54mm_Vertical" H 1250 1900 50  0001 C CNN
F 3 "~" H 1250 1900 50  0001 C CNN
	1    1250 1900
	1    0    0    -1  
$EndComp
Wire Wire Line
	4400 2450 4500 2450
Connection ~ 4500 2450
Wire Wire Line
	4500 2450 4600 2450
Connection ~ 4600 2450
Wire Wire Line
	4600 2450 4700 2450
Connection ~ 4700 2450
Wire Wire Line
	4700 2450 4800 2450
Connection ~ 4800 2450
Wire Wire Line
	4800 2450 4950 2450
Wire Wire Line
	4400 1950 4500 1950
Connection ~ 4500 1950
Wire Wire Line
	4500 1950 4600 1950
Connection ~ 4600 1950
Wire Wire Line
	4600 1950 4700 1950
Connection ~ 4700 1950
Wire Wire Line
	4700 1950 4800 1950
Connection ~ 4800 1950
Wire Wire Line
	4800 1950 4950 1950
Wire Wire Line
	4400 3200 4500 3200
Connection ~ 4500 3200
Wire Wire Line
	4500 3200 4600 3200
Connection ~ 4600 3200
Wire Wire Line
	4600 3200 4700 3200
Connection ~ 4700 3200
Wire Wire Line
	4700 3200 4800 3200
Connection ~ 4800 3200
Wire Wire Line
	4800 3200 4950 3200
Wire Wire Line
	4400 2700 4500 2700
Connection ~ 4500 2700
Wire Wire Line
	4500 2700 4600 2700
Connection ~ 4600 2700
Wire Wire Line
	4600 2700 4700 2700
Connection ~ 4700 2700
Wire Wire Line
	4700 2700 4800 2700
Connection ~ 4800 2700
Wire Wire Line
	4800 2700 4950 2700
$Comp
L Connector_Generic:Conn_02x05_Counter_Clockwise J12
U 1 1 5BC4C580
P 4600 3550
F 0 "J12" V 4604 3830 50  0000 L CNN
F 1 "Conn_02x05_Counter_Clockwise" V 4695 3830 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_2x05_P2.54mm_Vertical" H 4600 3550 50  0001 C CNN
F 3 "~" H 4600 3550 50  0001 C CNN
	1    4600 3550
	0    1    1    0   
$EndComp
Wire Wire Line
	4400 3850 4500 3850
Connection ~ 4500 3850
Wire Wire Line
	4500 3850 4600 3850
Connection ~ 4600 3850
Wire Wire Line
	4600 3850 4700 3850
Connection ~ 4700 3850
Wire Wire Line
	4700 3850 4800 3850
Connection ~ 4800 3850
Wire Wire Line
	4800 3850 4950 3850
Wire Wire Line
	4400 3350 4500 3350
Connection ~ 4500 3350
Wire Wire Line
	4500 3350 4600 3350
Connection ~ 4600 3350
Wire Wire Line
	4600 3350 4700 3350
Connection ~ 4700 3350
Wire Wire Line
	4700 3350 4800 3350
Connection ~ 4800 3350
Wire Wire Line
	4800 3350 4950 3350
$Comp
L Connector_Generic:Conn_02x05_Counter_Clockwise J13
U 1 1 5BC4C599
P 4600 4250
F 0 "J13" V 4604 4530 50  0000 L CNN
F 1 "Conn_02x05_Counter_Clockwise" V 4695 4530 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_2x05_P2.54mm_Vertical" H 4600 4250 50  0001 C CNN
F 3 "~" H 4600 4250 50  0001 C CNN
	1    4600 4250
	0    1    1    0   
$EndComp
$Comp
L Connector_Generic:Conn_02x05_Counter_Clockwise J14
U 1 1 5BC4C5A0
P 4600 4950
F 0 "J14" V 4604 5230 50  0000 L CNN
F 1 "Conn_02x05_Counter_Clockwise" V 4695 5230 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_2x05_P2.54mm_Vertical" H 4600 4950 50  0001 C CNN
F 3 "~" H 4600 4950 50  0001 C CNN
	1    4600 4950
	0    1    1    0   
$EndComp
Wire Wire Line
	4400 4550 4500 4550
Connection ~ 4500 4550
Wire Wire Line
	4500 4550 4600 4550
Connection ~ 4600 4550
Wire Wire Line
	4600 4550 4700 4550
Connection ~ 4700 4550
Wire Wire Line
	4700 4550 4800 4550
Connection ~ 4800 4550
Wire Wire Line
	4800 4550 4950 4550
Wire Wire Line
	4400 4050 4500 4050
Connection ~ 4500 4050
Wire Wire Line
	4500 4050 4600 4050
Connection ~ 4600 4050
Wire Wire Line
	4600 4050 4700 4050
Connection ~ 4700 4050
Wire Wire Line
	4700 4050 4800 4050
Connection ~ 4800 4050
Wire Wire Line
	4800 4050 4950 4050
Wire Wire Line
	4400 5250 4500 5250
Connection ~ 4500 5250
Wire Wire Line
	4500 5250 4600 5250
Connection ~ 4600 5250
Wire Wire Line
	4600 5250 4700 5250
Connection ~ 4700 5250
Wire Wire Line
	4700 5250 4800 5250
Connection ~ 4800 5250
Wire Wire Line
	4800 5250 4950 5250
Wire Wire Line
	4400 4750 4500 4750
Connection ~ 4500 4750
Wire Wire Line
	4500 4750 4600 4750
Connection ~ 4600 4750
Wire Wire Line
	4600 4750 4700 4750
Connection ~ 4700 4750
Wire Wire Line
	4700 4750 4800 4750
Connection ~ 4800 4750
Wire Wire Line
	4800 4750 4950 4750
$Comp
L Connector_Generic:Conn_02x05_Counter_Clockwise J15
U 1 1 5BC4C5CB
P 4600 5600
F 0 "J15" V 4604 5880 50  0000 L CNN
F 1 "Conn_02x05_Counter_Clockwise" V 4695 5880 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_2x05_P2.54mm_Vertical" H 4600 5600 50  0001 C CNN
F 3 "~" H 4600 5600 50  0001 C CNN
	1    4600 5600
	0    1    1    0   
$EndComp
Wire Wire Line
	4400 5900 4500 5900
Connection ~ 4500 5900
Wire Wire Line
	4500 5900 4600 5900
Connection ~ 4600 5900
Wire Wire Line
	4600 5900 4700 5900
Connection ~ 4700 5900
Wire Wire Line
	4700 5900 4800 5900
Connection ~ 4800 5900
Wire Wire Line
	4800 5900 4950 5900
Wire Wire Line
	4400 5400 4500 5400
Connection ~ 4500 5400
Wire Wire Line
	4500 5400 4600 5400
Connection ~ 4600 5400
Wire Wire Line
	4600 5400 4700 5400
Connection ~ 4700 5400
Wire Wire Line
	4700 5400 4800 5400
Connection ~ 4800 5400
Wire Wire Line
	4800 5400 4950 5400
Wire Wire Line
	1050 1300 1050 1400
Connection ~ 1050 1400
Wire Wire Line
	1050 1400 1050 1500
Connection ~ 1050 1500
Wire Wire Line
	1050 1500 1050 1600
Connection ~ 1050 1600
Wire Wire Line
	1050 1600 1050 1700
Connection ~ 1050 1700
Wire Wire Line
	1050 1700 1050 1800
Connection ~ 1050 1800
Wire Wire Line
	1050 1800 1050 1900
Connection ~ 1050 1900
Wire Wire Line
	1050 1900 1050 2000
Connection ~ 1050 2000
Wire Wire Line
	1050 2000 1050 2100
Connection ~ 1050 2100
Wire Wire Line
	1050 2100 1050 2200
Connection ~ 1050 2200
Wire Wire Line
	1050 2200 1050 2300
Connection ~ 1050 2300
Wire Wire Line
	1050 2300 1050 2400
Connection ~ 1050 2400
Wire Wire Line
	1050 2400 1050 2500
Connection ~ 1050 2500
Wire Wire Line
	1050 2500 1050 2600
Connection ~ 1050 2600
Wire Wire Line
	1050 2600 1050 2750
Wire Wire Line
	1550 1300 1550 1400
Connection ~ 1550 1400
Wire Wire Line
	1550 1400 1550 1500
Connection ~ 1550 1500
Wire Wire Line
	1550 1500 1550 1600
Connection ~ 1550 1600
Wire Wire Line
	1550 1600 1550 1700
Connection ~ 1550 1700
Wire Wire Line
	1550 1700 1550 1800
Connection ~ 1550 1800
Wire Wire Line
	1550 1800 1550 1900
Connection ~ 1550 1900
Wire Wire Line
	1550 1900 1550 2000
Connection ~ 1550 2000
Wire Wire Line
	1550 2000 1550 2100
Connection ~ 1550 2100
Wire Wire Line
	1550 2100 1550 2200
Connection ~ 1550 2200
Wire Wire Line
	1550 2200 1550 2300
Connection ~ 1550 2300
Wire Wire Line
	1550 2300 1550 2400
Connection ~ 1550 2400
Wire Wire Line
	1550 2400 1550 2500
Connection ~ 1550 2500
Wire Wire Line
	1550 2500 1550 2600
Connection ~ 1550 2600
Wire Wire Line
	1550 2600 1550 2750
$Comp
L power:GND #PWR03
U 1 1 5BC4C61A
P 1050 2750
F 0 "#PWR03" H 1050 2500 50  0001 C CNN
F 1 "GND" H 1054 2576 50  0000 C CNN
F 2 "" H 1050 2750 50  0001 C CNN
F 3 "" H 1050 2750 50  0001 C CNN
	1    1050 2750
	1    0    0    -1  
$EndComp
Wire Wire Line
	1550 2750 1700 2750
$Comp
L Connector_Generic:Conn_02x14_Odd_Even J2
U 1 1 5BC5AEA2
P 1200 6200
F 0 "J2" H 1250 7017 50  0000 C CNN
F 1 "Conn_02x14_Odd_Even" H 1250 6926 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x14_P2.54mm_Vertical" H 1200 6200 50  0001 C CNN
F 3 "~" H 1200 6200 50  0001 C CNN
	1    1200 6200
	1    0    0    -1  
$EndComp
Wire Wire Line
	1000 5600 1000 5700
Connection ~ 1000 5700
Wire Wire Line
	1000 5700 1000 5800
Connection ~ 1000 5800
Wire Wire Line
	1000 5800 1000 5900
Connection ~ 1000 5900
Wire Wire Line
	1000 5900 1000 6000
Connection ~ 1000 6000
Wire Wire Line
	1000 6000 1000 6100
Connection ~ 1000 6100
Wire Wire Line
	1000 6100 1000 6200
Connection ~ 1000 6200
Wire Wire Line
	1000 6200 1000 6300
Connection ~ 1000 6300
Wire Wire Line
	1000 6300 1000 6400
Connection ~ 1000 6400
Wire Wire Line
	1000 6400 1000 6500
Connection ~ 1000 6500
Wire Wire Line
	1000 6500 1000 6600
Connection ~ 1000 6600
Wire Wire Line
	1000 6600 1000 6700
Connection ~ 1000 6700
Wire Wire Line
	1000 6700 1000 6800
Connection ~ 1000 6800
Wire Wire Line
	1000 6800 1000 6900
Connection ~ 1000 6900
Wire Wire Line
	1000 6900 1000 7050
Wire Wire Line
	1500 5600 1500 5700
Connection ~ 1500 5700
Wire Wire Line
	1500 5700 1500 5800
Connection ~ 1500 5800
Wire Wire Line
	1500 5800 1500 5900
Connection ~ 1500 5900
Wire Wire Line
	1500 5900 1500 6000
Connection ~ 1500 6000
Wire Wire Line
	1500 6000 1500 6100
Connection ~ 1500 6100
Wire Wire Line
	1500 6100 1500 6200
Connection ~ 1500 6200
Wire Wire Line
	1500 6200 1500 6300
Connection ~ 1500 6300
Wire Wire Line
	1500 6300 1500 6400
Connection ~ 1500 6400
Wire Wire Line
	1500 6400 1500 6500
Connection ~ 1500 6500
Wire Wire Line
	1500 6500 1500 6600
Connection ~ 1500 6600
Wire Wire Line
	1500 6600 1500 6700
Connection ~ 1500 6700
Wire Wire Line
	1500 6700 1500 6800
Connection ~ 1500 6800
Wire Wire Line
	1500 6800 1500 6900
Connection ~ 1500 6900
Wire Wire Line
	1500 6900 1500 7050
$Comp
L power:GND #PWR02
U 1 1 5BC5AEDF
P 1000 7050
F 0 "#PWR02" H 1000 6800 50  0001 C CNN
F 1 "GND" H 1004 6876 50  0000 C CNN
F 2 "" H 1000 7050 50  0001 C CNN
F 3 "" H 1000 7050 50  0001 C CNN
	1    1000 7050
	1    0    0    -1  
$EndComp
Wire Wire Line
	1500 7050 1650 7050
$Comp
L power:+3V3 #PWR05
U 1 1 5BC7BA26
P 1650 7050
F 0 "#PWR05" H 1650 6900 50  0001 C CNN
F 1 "+3V3" H 1664 7225 50  0000 C CNN
F 2 "" H 1650 7050 50  0001 C CNN
F 3 "" H 1650 7050 50  0001 C CNN
	1    1650 7050
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR04
U 1 1 5BC7BEB4
P 1650 4850
F 0 "#PWR04" H 1650 4700 50  0001 C CNN
F 1 "+5V" H 1664 5025 50  0000 C CNN
F 2 "" H 1650 4850 50  0001 C CNN
F 3 "" H 1650 4850 50  0001 C CNN
	1    1650 4850
	1    0    0    -1  
$EndComp
$Comp
L power:+BATT #PWR06
U 1 1 5BC7C24B
P 1700 2750
F 0 "#PWR06" H 1700 2600 50  0001 C CNN
F 1 "+BATT" H 1714 2925 50  0000 C CNN
F 2 "" H 1700 2750 50  0001 C CNN
F 3 "" H 1700 2750 50  0001 C CNN
	1    1700 2750
	1    0    0    -1  
$EndComp
Text Label 7150 4100 0    50   ~ 0
IO0
Text Label 7150 4200 0    50   ~ 0
IO1
Text Label 7150 4300 0    50   ~ 0
IO2
Text Label 7150 4400 0    50   ~ 0
IO3
Text Label 7150 4500 0    50   ~ 0
IO4
Text Label 7150 4600 0    50   ~ 0
IO5
Text Label 7150 4700 0    50   ~ 0
IO6
$Comp
L Connector_Generic:Conn_01x08 J16
U 1 1 5BC8DCEC
P 7500 4400
F 0 "J16" H 7580 4392 50  0000 L CNN
F 1 "Conn_01x08" H 7580 4301 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x08_P2.54mm_Vertical" H 7500 4400 50  0001 C CNN
F 3 "~" H 7500 4400 50  0001 C CNN
	1    7500 4400
	1    0    0    -1  
$EndComp
Wire Wire Line
	7300 4100 7150 4100
Wire Wire Line
	7300 4200 7150 4200
Wire Wire Line
	7300 4300 7150 4300
Wire Wire Line
	7300 4400 7150 4400
Wire Wire Line
	7300 4500 7150 4500
Wire Wire Line
	7300 4600 7150 4600
Wire Wire Line
	7300 4700 7150 4700
Text Label 7150 4800 0    50   ~ 0
IO7
Wire Wire Line
	7150 4800 7300 4800
$Comp
L Connector_Generic:Conn_01x04 J17
U 1 1 5BD1524E
P 7500 5150
F 0 "J17" H 7580 5142 50  0000 L CNN
F 1 "Conn_01x04" H 7580 5051 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 7500 5150 50  0001 C CNN
F 3 "~" H 7500 5150 50  0001 C CNN
	1    7500 5150
	1    0    0    -1  
$EndComp
Text Label 7150 5050 0    50   ~ 0
AD0
Wire Wire Line
	7300 5050 7150 5050
Text Label 7150 5150 0    50   ~ 0
AD1
Text Label 7150 5250 0    50   ~ 0
AD2
Text Label 7150 5350 0    50   ~ 0
AD3
Wire Wire Line
	7300 5150 7150 5150
Wire Wire Line
	7300 5250 7150 5250
Wire Wire Line
	7300 5350 7150 5350
$Comp
L Connector_Generic:Conn_01x04 J18
U 1 1 5BD5C306
P 7500 5650
F 0 "J18" H 7580 5642 50  0000 L CNN
F 1 "Conn_01x04" H 7580 5551 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 7500 5650 50  0001 C CNN
F 3 "~" H 7500 5650 50  0001 C CNN
	1    7500 5650
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_02x02_Counter_Clockwise J25
U 1 1 5BC13E36
P 9650 2750
F 0 "J25" H 9700 2968 50  0000 C CNN
F 1 "Conn_02x02_Counter_Clockwise" H 9700 2876 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x02_P2.54mm_Vertical" H 9650 2750 50  0001 C CNN
F 3 "https://www.mouser.com/datasheet/2/527/ssq_th-1370157.pdf" H 9650 2750 50  0001 C CNN
F 4 "SSQ-102-03-T-D" H 9650 2750 50  0001 C CNN "Manufacturer Part Number"
	1    9650 2750
	1    0    0    -1  
$EndComp
Wire Wire Line
	9450 2750 9000 2750
Wire Wire Line
	9950 2850 10400 2850
Wire Wire Line
	9950 2750 10400 2750
Wire Wire Line
	9450 2850 9000 2850
Text Label 7150 5550 0    50   ~ 0
MOSI
Text Label 7150 5650 0    50   ~ 0
MISO
Text Label 7150 5750 0    50   ~ 0
CS
Text Label 7150 5850 0    50   ~ 0
SCLK
Wire Wire Line
	7300 5850 7150 5850
Wire Wire Line
	7300 5750 7150 5750
Wire Wire Line
	7300 5650 7150 5650
Wire Wire Line
	7300 5550 7150 5550
$Comp
L Connector_Generic:Conn_01x02 J19
U 1 1 5BE33C18
P 7500 6050
F 0 "J19" H 7580 6042 50  0000 L CNN
F 1 "Conn_01x02" H 7580 5951 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 7500 6050 50  0001 C CNN
F 3 "~" H 7500 6050 50  0001 C CNN
	1    7500 6050
	1    0    0    -1  
$EndComp
Text Label 7150 6150 0    50   ~ 0
SCL
Text Label 7150 6050 0    50   ~ 0
SDA
Wire Wire Line
	7300 6050 7150 6050
Wire Wire Line
	7300 6150 7150 6150
$Comp
L Mechanical:MountingHole_Pad MH1
U 1 1 5BEB7BF7
P 9700 4400
F 0 "MH1" H 9800 4451 50  0000 L CNN
F 1 "MountingHole_Pad" H 9800 4360 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_Pad" H 9700 4400 50  0001 C CNN
F 3 "~" H 9700 4400 50  0001 C CNN
	1    9700 4400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR017
U 1 1 5BEB7E4E
P 9700 4550
F 0 "#PWR017" H 9700 4300 50  0001 C CNN
F 1 "GND" H 9704 4376 50  0000 C CNN
F 2 "" H 9700 4550 50  0001 C CNN
F 3 "" H 9700 4550 50  0001 C CNN
	1    9700 4550
	1    0    0    -1  
$EndComp
Wire Wire Line
	9700 4500 9700 4550
$Comp
L Mechanical:MountingHole_Pad MH2
U 1 1 5BEDCD61
P 9700 5000
F 0 "MH2" H 9800 5051 50  0000 L CNN
F 1 "MountingHole_Pad" H 9800 4960 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_Pad" H 9700 5000 50  0001 C CNN
F 3 "~" H 9700 5000 50  0001 C CNN
	1    9700 5000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR018
U 1 1 5BEDCD68
P 9700 5150
F 0 "#PWR018" H 9700 4900 50  0001 C CNN
F 1 "GND" H 9704 4976 50  0000 C CNN
F 2 "" H 9700 5150 50  0001 C CNN
F 3 "" H 9700 5150 50  0001 C CNN
	1    9700 5150
	1    0    0    -1  
$EndComp
Wire Wire Line
	9700 5100 9700 5150
$Comp
L Mechanical:MountingHole_Pad MH3
U 1 1 5BEEF575
P 9700 5600
F 0 "MH3" H 9800 5651 50  0000 L CNN
F 1 "MountingHole_Pad" H 9800 5560 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_Pad" H 9700 5600 50  0001 C CNN
F 3 "~" H 9700 5600 50  0001 C CNN
	1    9700 5600
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR019
U 1 1 5BEEF57C
P 9700 5750
F 0 "#PWR019" H 9700 5500 50  0001 C CNN
F 1 "GND" H 9704 5576 50  0000 C CNN
F 2 "" H 9700 5750 50  0001 C CNN
F 3 "" H 9700 5750 50  0001 C CNN
	1    9700 5750
	1    0    0    -1  
$EndComp
Wire Wire Line
	9700 5700 9700 5750
$EndSCHEMATC
