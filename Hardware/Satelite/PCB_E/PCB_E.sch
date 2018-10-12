EESchema Schematic File Version 4
LIBS:PCB_E-cache
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
L power:GND #PWR08
U 1 1 5BEB7E4E
P 9700 4550
F 0 "#PWR08" H 9700 4300 50  0001 C CNN
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
L power:GND #PWR09
U 1 1 5BEDCD68
P 9700 5150
F 0 "#PWR09" H 9700 4900 50  0001 C CNN
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
L power:GND #PWR010
U 1 1 5BEEF57C
P 9700 5750
F 0 "#PWR010" H 9700 5500 50  0001 C CNN
F 1 "GND" H 9704 5576 50  0000 C CNN
F 2 "" H 9700 5750 50  0001 C CNN
F 3 "" H 9700 5750 50  0001 C CNN
	1    9700 5750
	1    0    0    -1  
$EndComp
Wire Wire Line
	9700 5700 9700 5750
$Comp
L power:+BATT #PWR013
U 1 1 5BC7EDA4
P 10150 2600
F 0 "#PWR013" H 10150 2450 50  0001 C CNN
F 1 "+BATT" H 10164 2775 50  0000 C CNN
F 2 "" H 10150 2600 50  0001 C CNN
F 3 "" H 10150 2600 50  0001 C CNN
	1    10150 2600
	1    0    0    -1  
$EndComp
Wire Wire Line
	7650 2750 7200 2750
Wire Wire Line
	7650 2850 7200 2850
Wire Wire Line
	7200 3150 7650 3150
Wire Wire Line
	7650 3250 7200 3250
Wire Wire Line
	8050 2750 8500 2750
Wire Wire Line
	8500 2850 8050 2850
Wire Wire Line
	7200 2950 7650 2950
Wire Wire Line
	7650 3050 7200 3050
Text Notes 7700 2300 0    118  ~ 0
Conector de expansión
Wire Notes Line
	7050 2100 10550 2100
Wire Notes Line
	7050 3750 7050 2100
Wire Notes Line
	10550 3750 7050 3750
Wire Notes Line
	10550 2100 10550 3750
Text Label 7250 2750 0    50   ~ 0
IO0
Text Label 7250 2850 0    50   ~ 0
IO1
Text Label 7250 2950 0    50   ~ 0
IO2
Text Label 7250 3050 0    50   ~ 0
IO3
Text Label 7250 3150 0    50   ~ 0
IO4
Text Label 7250 3250 0    50   ~ 0
IO5
Text Label 8100 2750 0    50   ~ 0
IO6
$Comp
L Connector_Generic:Conn_01x06 J14
U 1 1 5BC7EDBF
P 7850 2950
F 0 "J14" H 7450 2550 50  0000 L CNN
F 1 "Conn_01x06" H 7450 2450 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x06_P2.54mm_Vertical" H 7850 2950 50  0001 C CNN
F 3 "https://www.mouser.com/datasheet/2/527/ssw_sm-1370305.pdf" H 7850 2950 50  0001 C CNN
F 4 "SSW-106-03-T-S" H 7850 2950 50  0001 C CNN "Manufacturer Part Number"
	1    7850 2950
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x06 J16
U 1 1 5BC7EDC7
P 8700 2950
F 0 "J16" H 8300 2550 50  0000 L CNN
F 1 "Conn_01x06" H 8300 2450 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x06_P2.54mm_Vertical" H 8700 2950 50  0001 C CNN
F 3 "https://www.mouser.com/datasheet/2/527/ssw_sm-1370305.pdf" H 8700 2950 50  0001 C CNN
F 4 "SSW-106-03-T-S" H 8700 2950 50  0001 C CNN "Manufacturer Part Number"
	1    8700 2950
	1    0    0    -1  
$EndComp
Wire Wire Line
	8500 2950 8050 2950
Wire Wire Line
	8050 3050 8500 3050
Wire Wire Line
	8050 3150 8500 3150
Wire Wire Line
	8050 3250 8500 3250
Wire Wire Line
	8900 2750 9350 2750
Wire Wire Line
	9350 2850 8900 2850
$Comp
L Connector_Generic:Conn_01x06 J17
U 1 1 5BC7EDD5
P 9550 2950
F 0 "J17" H 9150 2550 50  0000 L CNN
F 1 "Conn_01x06" H 9150 2450 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x06_P2.54mm_Vertical" H 9550 2950 50  0001 C CNN
F 3 "https://www.mouser.com/datasheet/2/527/ssw_sm-1370305.pdf" H 9550 2950 50  0001 C CNN
F 4 "SSW-106-03-T-S" H 9550 2950 50  0001 C CNN "Manufacturer Part Number"
	1    9550 2950
	1    0    0    -1  
$EndComp
Wire Wire Line
	9350 2950 8900 2950
Wire Wire Line
	8900 3050 9350 3050
Wire Wire Line
	8900 3150 9350 3150
Wire Wire Line
	8900 3250 9350 3250
Text Label 8950 2750 0    50   ~ 0
MOSI
Text Label 8950 2850 0    50   ~ 0
MISO
Text Label 8950 2950 0    50   ~ 0
SCK
Text Label 8950 3050 0    50   ~ 0
SS
Text Label 8950 3150 0    50   ~ 0
SCL
Text Label 8950 3250 0    50   ~ 0
SDA
$Comp
L Connector_Generic:Conn_01x06 J18
U 1 1 5BC7EDE7
P 10400 2950
F 0 "J18" H 10000 2550 50  0000 L CNN
F 1 "Conn_01x06" H 10000 2450 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x06_P2.54mm_Vertical" H 10400 2950 50  0001 C CNN
F 3 "https://www.mouser.com/datasheet/2/527/ssw_sm-1370305.pdf" H 10400 2950 50  0001 C CNN
F 4 "SSW-106-03-T-S" H 10400 2950 50  0001 C CNN "Manufacturer Part Number"
	1    10400 2950
	1    0    0    -1  
$EndComp
Wire Wire Line
	10150 2600 10150 2750
Wire Wire Line
	10150 2750 10200 2750
$Comp
L power:+3V3 #PWR011
U 1 1 5BC7EDF0
P 9900 2600
F 0 "#PWR011" H 9900 2450 50  0001 C CNN
F 1 "+3V3" H 9914 2775 50  0000 C CNN
F 2 "" H 9900 2600 50  0001 C CNN
F 3 "" H 9900 2600 50  0001 C CNN
	1    9900 2600
	1    0    0    -1  
$EndComp
Wire Wire Line
	9900 2600 9900 2850
Wire Wire Line
	9900 2850 10200 2850
$Comp
L power:+5V #PWR07
U 1 1 5BC7EDF8
P 9700 2600
F 0 "#PWR07" H 9700 2450 50  0001 C CNN
F 1 "+5V" H 9714 2775 50  0000 C CNN
F 2 "" H 9700 2600 50  0001 C CNN
F 3 "" H 9700 2600 50  0001 C CNN
	1    9700 2600
	1    0    0    -1  
$EndComp
Wire Wire Line
	9700 2600 9700 2950
Wire Wire Line
	9700 2950 10200 2950
$Comp
L power:GND #PWR012
U 1 1 5BC7EE00
P 9900 3450
F 0 "#PWR012" H 9900 3200 50  0001 C CNN
F 1 "GND" H 9904 3276 50  0000 C CNN
F 2 "" H 9900 3450 50  0001 C CNN
F 3 "" H 9900 3450 50  0001 C CNN
	1    9900 3450
	1    0    0    -1  
$EndComp
Wire Wire Line
	10200 3050 10150 3050
Wire Wire Line
	9900 3050 9900 3450
Wire Wire Line
	10200 3150 10150 3150
Wire Wire Line
	10150 3150 10150 3050
Connection ~ 10150 3050
Wire Wire Line
	10150 3050 9900 3050
Wire Wire Line
	10200 3250 10150 3250
Wire Wire Line
	10150 3250 10150 3150
Connection ~ 10150 3150
Text Label 8100 2850 0    50   ~ 0
IO7
Text Label 8100 2950 0    50   ~ 0
AD0
Text Label 8100 3050 0    50   ~ 0
AD1
Text Label 8100 3150 0    50   ~ 0
AD2
Text Label 8100 3250 0    50   ~ 0
AD3
$Comp
L Connector_Generic:Conn_02x02_Counter_Clockwise J1
U 1 1 5BC9D944
P 1200 2300
F 0 "J1" H 1250 2517 50  0000 C CNN
F 1 "Conn_02x02_Counter_Clockwise" H 1250 2426 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_2x02_P2.54mm_Vertical" H 1200 2300 50  0001 C CNN
F 3 "~" H 1200 2300 50  0001 C CNN
	1    1200 2300
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR01
U 1 1 5BC9D9FC
P 900 2450
F 0 "#PWR01" H 900 2200 50  0001 C CNN
F 1 "GND" H 904 2276 50  0000 C CNN
F 2 "" H 900 2450 50  0001 C CNN
F 3 "" H 900 2450 50  0001 C CNN
	1    900  2450
	1    0    0    -1  
$EndComp
Wire Wire Line
	900  2450 900  2400
Wire Wire Line
	900  2300 1000 2300
Wire Wire Line
	1000 2400 900  2400
Connection ~ 900  2400
Wire Wire Line
	900  2400 900  2300
$Comp
L power:+BATT #PWR04
U 1 1 5BCA6C50
P 1950 2300
F 0 "#PWR04" H 1950 2150 50  0001 C CNN
F 1 "+BATT" H 1964 2475 50  0000 C CNN
F 2 "" H 1950 2300 50  0001 C CNN
F 3 "" H 1950 2300 50  0001 C CNN
	1    1950 2300
	1    0    0    -1  
$EndComp
Wire Wire Line
	1500 2300 1600 2300
Wire Wire Line
	1500 2400 1600 2400
Wire Wire Line
	1600 2400 1600 2300
Connection ~ 1600 2300
Wire Wire Line
	1600 2300 1950 2300
$Comp
L Connector_Generic:Conn_02x02_Counter_Clockwise J2
U 1 1 5BCB56D1
P 1200 2900
F 0 "J2" H 1250 3117 50  0000 C CNN
F 1 "Conn_02x02_Counter_Clockwise" H 1250 3026 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_2x02_P2.54mm_Vertical" H 1200 2900 50  0001 C CNN
F 3 "~" H 1200 2900 50  0001 C CNN
	1    1200 2900
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR02
U 1 1 5BCB56D8
P 900 3050
F 0 "#PWR02" H 900 2800 50  0001 C CNN
F 1 "GND" H 904 2876 50  0000 C CNN
F 2 "" H 900 3050 50  0001 C CNN
F 3 "" H 900 3050 50  0001 C CNN
	1    900  3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	900  3050 900  3000
Wire Wire Line
	900  2900 1000 2900
Wire Wire Line
	1000 3000 900  3000
Connection ~ 900  3000
Wire Wire Line
	900  3000 900  2900
Wire Wire Line
	1500 2900 1600 2900
Wire Wire Line
	1500 3000 1600 3000
Wire Wire Line
	1600 3000 1600 2900
Connection ~ 1600 2900
Wire Wire Line
	1600 2900 1950 2900
$Comp
L Connector_Generic:Conn_02x02_Counter_Clockwise J3
U 1 1 5BCBB2D4
P 1200 3500
F 0 "J3" H 1250 3717 50  0000 C CNN
F 1 "Conn_02x02_Counter_Clockwise" H 1250 3626 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_2x02_P2.54mm_Vertical" H 1200 3500 50  0001 C CNN
F 3 "~" H 1200 3500 50  0001 C CNN
	1    1200 3500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR03
U 1 1 5BCBB2DB
P 900 3650
F 0 "#PWR03" H 900 3400 50  0001 C CNN
F 1 "GND" H 904 3476 50  0000 C CNN
F 2 "" H 900 3650 50  0001 C CNN
F 3 "" H 900 3650 50  0001 C CNN
	1    900  3650
	1    0    0    -1  
$EndComp
Wire Wire Line
	900  3650 900  3600
Wire Wire Line
	900  3500 1000 3500
Wire Wire Line
	1000 3600 900  3600
Connection ~ 900  3600
Wire Wire Line
	900  3600 900  3500
Wire Wire Line
	1500 3500 1600 3500
Wire Wire Line
	1500 3600 1600 3600
Wire Wire Line
	1600 3600 1600 3500
Connection ~ 1600 3500
Wire Wire Line
	1600 3500 1950 3500
$Comp
L power:+5V #PWR05
U 1 1 5BCC1B51
P 1950 2900
F 0 "#PWR05" H 1950 2750 50  0001 C CNN
F 1 "+5V" H 1964 3075 50  0000 C CNN
F 2 "" H 1950 2900 50  0001 C CNN
F 3 "" H 1950 2900 50  0001 C CNN
	1    1950 2900
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR06
U 1 1 5BCC1B80
P 1950 3500
F 0 "#PWR06" H 1950 3350 50  0001 C CNN
F 1 "+3V3" H 1964 3675 50  0000 C CNN
F 2 "" H 1950 3500 50  0001 C CNN
F 3 "" H 1950 3500 50  0001 C CNN
	1    1950 3500
	1    0    0    -1  
$EndComp
Wire Wire Line
	7050 3850 6600 3850
Wire Wire Line
	7050 3950 6600 3950
Wire Wire Line
	6600 4250 7050 4250
Wire Wire Line
	7050 4350 6600 4350
Wire Wire Line
	6600 4050 7050 4050
Wire Wire Line
	7050 4150 6600 4150
Text Label 6650 3850 0    50   ~ 0
IO0
Text Label 6650 3950 0    50   ~ 0
IO1
Text Label 6650 4050 0    50   ~ 0
IO2
Text Label 6650 4150 0    50   ~ 0
IO3
Text Label 6650 4250 0    50   ~ 0
IO4
Text Label 6650 4350 0    50   ~ 0
IO5
Wire Wire Line
	7850 4400 8300 4400
Wire Wire Line
	8300 4500 7850 4500
Text Label 7900 4400 0    50   ~ 0
IO6
Wire Wire Line
	8300 4600 7850 4600
Wire Wire Line
	7850 4700 8300 4700
Wire Wire Line
	7850 4800 8300 4800
Wire Wire Line
	7850 4900 8300 4900
Text Label 7900 4500 0    50   ~ 0
IO7
Text Label 7900 4600 0    50   ~ 0
AD0
Text Label 7900 4700 0    50   ~ 0
AD1
Text Label 7900 4800 0    50   ~ 0
AD2
Text Label 7900 4900 0    50   ~ 0
AD3
Wire Wire Line
	6600 5200 7050 5200
Wire Wire Line
	7050 5300 6600 5300
Wire Wire Line
	7050 5400 6600 5400
Wire Wire Line
	6600 5500 7050 5500
Wire Wire Line
	6600 5600 7050 5600
Wire Wire Line
	6600 5700 7050 5700
Text Label 6650 5200 0    50   ~ 0
MOSI
Text Label 6650 5300 0    50   ~ 0
MISO
Text Label 6650 5400 0    50   ~ 0
SCK
Text Label 6650 5500 0    50   ~ 0
SS
Text Label 6650 5600 0    50   ~ 0
SCL
Text Label 6650 5700 0    50   ~ 0
SDA
$Comp
L Connector_Generic:Conn_01x06 J13
U 1 1 5BCDFAAC
P 7250 5400
F 0 "J13" H 6850 5000 50  0000 L CNN
F 1 "Conn_01x06" H 6850 4900 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x06_P2.54mm_Vertical" H 7250 5400 50  0001 C CNN
F 3 "https://www.mouser.com/datasheet/2/527/ssw_sm-1370305.pdf" H 7250 5400 50  0001 C CNN
F 4 "SSW-106-03-T-S" H 7250 5400 50  0001 C CNN "Manufacturer Part Number"
	1    7250 5400
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x06 J12
U 1 1 5BCE857F
P 7250 4050
F 0 "J12" H 6850 3650 50  0000 L CNN
F 1 "Conn_01x06" H 6850 3550 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x06_P2.54mm_Vertical" H 7250 4050 50  0001 C CNN
F 3 "https://www.mouser.com/datasheet/2/527/ssw_sm-1370305.pdf" H 7250 4050 50  0001 C CNN
F 4 "SSW-106-03-T-S" H 7250 4050 50  0001 C CNN "Manufacturer Part Number"
	1    7250 4050
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x06 J15
U 1 1 5BCE85D3
P 8500 4600
F 0 "J15" H 8100 4200 50  0000 L CNN
F 1 "Conn_01x06" H 8100 4100 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x06_P2.54mm_Vertical" H 8500 4600 50  0001 C CNN
F 3 "https://www.mouser.com/datasheet/2/527/ssw_sm-1370305.pdf" H 8500 4600 50  0001 C CNN
F 4 "SSW-106-03-T-S" H 8500 4600 50  0001 C CNN "Manufacturer Part Number"
	1    8500 4600
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_02x04_Odd_Even J4
U 1 1 5BCEAA33
P 3150 2600
F 0 "J4" H 3200 2917 50  0000 C CNN
F 1 "Conn_02x04_Odd_Even" H 3200 2826 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_2x04_P2.54mm_Vertical" H 3150 2600 50  0001 C CNN
F 3 "~" H 3150 2600 50  0001 C CNN
	1    3150 2600
	1    0    0    -1  
$EndComp
Wire Wire Line
	2950 2500 2950 2600
Connection ~ 2950 2600
Wire Wire Line
	2950 2600 2950 2700
Connection ~ 2950 2700
Wire Wire Line
	2950 2700 2950 2800
Wire Wire Line
	3450 2500 3450 2600
Connection ~ 3450 2600
Wire Wire Line
	3450 2600 3450 2700
Connection ~ 3450 2700
Wire Wire Line
	3450 2700 3450 2800
$Comp
L Connector_Generic:Conn_02x04_Odd_Even J8
U 1 1 5BCEED62
P 3950 2600
F 0 "J8" H 4000 2917 50  0000 C CNN
F 1 "Conn_02x04_Odd_Even" H 4000 2826 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_2x04_P2.54mm_Vertical" H 3950 2600 50  0001 C CNN
F 3 "~" H 3950 2600 50  0001 C CNN
	1    3950 2600
	1    0    0    -1  
$EndComp
Wire Wire Line
	3750 2500 3750 2600
Connection ~ 3750 2600
Wire Wire Line
	3750 2600 3750 2700
Connection ~ 3750 2700
Wire Wire Line
	3750 2700 3750 2800
Wire Wire Line
	4250 2500 4250 2600
Connection ~ 4250 2600
Wire Wire Line
	4250 2600 4250 2700
Connection ~ 4250 2700
Wire Wire Line
	4250 2700 4250 2800
$Comp
L Connector_Generic:Conn_02x04_Odd_Even J5
U 1 1 5BCF37F5
P 3150 3200
F 0 "J5" H 3200 3517 50  0000 C CNN
F 1 "Conn_02x04_Odd_Even" H 3200 3426 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_2x04_P2.54mm_Vertical" H 3150 3200 50  0001 C CNN
F 3 "~" H 3150 3200 50  0001 C CNN
	1    3150 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	2950 3100 2950 3200
Connection ~ 2950 3200
Wire Wire Line
	2950 3200 2950 3300
Connection ~ 2950 3300
Wire Wire Line
	2950 3300 2950 3400
Wire Wire Line
	3450 3100 3450 3200
Connection ~ 3450 3200
Wire Wire Line
	3450 3200 3450 3300
Connection ~ 3450 3300
Wire Wire Line
	3450 3300 3450 3400
$Comp
L Connector_Generic:Conn_02x04_Odd_Even J9
U 1 1 5BCF3806
P 3950 3200
F 0 "J9" H 4000 3517 50  0000 C CNN
F 1 "Conn_02x04_Odd_Even" H 4000 3426 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_2x04_P2.54mm_Vertical" H 3950 3200 50  0001 C CNN
F 3 "~" H 3950 3200 50  0001 C CNN
	1    3950 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	3750 3100 3750 3200
Connection ~ 3750 3200
Wire Wire Line
	3750 3200 3750 3300
Connection ~ 3750 3300
Wire Wire Line
	3750 3300 3750 3400
Wire Wire Line
	4250 3100 4250 3200
Connection ~ 4250 3200
Wire Wire Line
	4250 3200 4250 3300
Connection ~ 4250 3300
Wire Wire Line
	4250 3300 4250 3400
$Comp
L Connector_Generic:Conn_02x04_Odd_Even J6
U 1 1 5BCF648A
P 3150 3750
F 0 "J6" H 3200 4067 50  0000 C CNN
F 1 "Conn_02x04_Odd_Even" H 3200 3976 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_2x04_P2.54mm_Vertical" H 3150 3750 50  0001 C CNN
F 3 "~" H 3150 3750 50  0001 C CNN
	1    3150 3750
	1    0    0    -1  
$EndComp
Wire Wire Line
	2950 3650 2950 3750
Connection ~ 2950 3750
Wire Wire Line
	2950 3750 2950 3850
Connection ~ 2950 3850
Wire Wire Line
	2950 3850 2950 3950
Wire Wire Line
	3450 3650 3450 3750
Connection ~ 3450 3750
Wire Wire Line
	3450 3750 3450 3850
Connection ~ 3450 3850
Wire Wire Line
	3450 3850 3450 3950
$Comp
L Connector_Generic:Conn_02x04_Odd_Even J10
U 1 1 5BCF649B
P 3950 3750
F 0 "J10" H 4000 4067 50  0000 C CNN
F 1 "Conn_02x04_Odd_Even" H 4000 3976 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_2x04_P2.54mm_Vertical" H 3950 3750 50  0001 C CNN
F 3 "~" H 3950 3750 50  0001 C CNN
	1    3950 3750
	1    0    0    -1  
$EndComp
Wire Wire Line
	3750 3650 3750 3750
Connection ~ 3750 3750
Wire Wire Line
	3750 3750 3750 3850
Connection ~ 3750 3850
Wire Wire Line
	3750 3850 3750 3950
Wire Wire Line
	4250 3650 4250 3750
Connection ~ 4250 3750
Wire Wire Line
	4250 3750 4250 3850
Connection ~ 4250 3850
Wire Wire Line
	4250 3850 4250 3950
$Comp
L Connector_Generic:Conn_02x04_Odd_Even J7
U 1 1 5BCF64AC
P 3150 4350
F 0 "J7" H 3200 4667 50  0000 C CNN
F 1 "Conn_02x04_Odd_Even" H 3200 4576 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_2x04_P2.54mm_Vertical" H 3150 4350 50  0001 C CNN
F 3 "~" H 3150 4350 50  0001 C CNN
	1    3150 4350
	1    0    0    -1  
$EndComp
Wire Wire Line
	2950 4250 2950 4350
Connection ~ 2950 4350
Wire Wire Line
	2950 4350 2950 4450
Connection ~ 2950 4450
Wire Wire Line
	2950 4450 2950 4550
Wire Wire Line
	3450 4250 3450 4350
Connection ~ 3450 4350
Wire Wire Line
	3450 4350 3450 4450
Connection ~ 3450 4450
Wire Wire Line
	3450 4450 3450 4550
$Comp
L Connector_Generic:Conn_02x04_Odd_Even J11
U 1 1 5BCF64BD
P 3950 4350
F 0 "J11" H 4000 4667 50  0000 C CNN
F 1 "Conn_02x04_Odd_Even" H 4000 4576 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_2x04_P2.54mm_Vertical" H 3950 4350 50  0001 C CNN
F 3 "~" H 3950 4350 50  0001 C CNN
	1    3950 4350
	1    0    0    -1  
$EndComp
Wire Wire Line
	3750 4250 3750 4350
Connection ~ 3750 4350
Wire Wire Line
	3750 4350 3750 4450
Connection ~ 3750 4450
Wire Wire Line
	3750 4450 3750 4550
Wire Wire Line
	4250 4250 4250 4350
Connection ~ 4250 4350
Wire Wire Line
	4250 4350 4250 4450
Connection ~ 4250 4450
Wire Wire Line
	4250 4450 4250 4550
$EndSCHEMATC
