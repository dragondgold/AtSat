EESchema Schematic File Version 4
LIBS:PCB_B-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 6 6
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
L Battery_Management:BQ27441DRZR-G1A U?
U 1 1 5BB27F17
P 5900 3700
F 0 "U?" H 5900 4367 50  0000 C CNN
F 1 "BQ27441DRZR-G1A" H 5900 4276 50  0000 C CNN
F 2 "Package_SON:Texas_S-PDSO-N12" H 6150 3150 50  0001 L CNN
F 3 "http://www.ti.com/lit/ds/symlink/bq27441-g1.pdf" H 6100 3900 50  0001 C CNN
	1    5900 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	5900 4650 5900 4350
$Comp
L power:GND #PWR?
U 1 1 5BB28170
P 5900 4650
AR Path="/5BA1A7F9/5BB28170" Ref="#PWR?"  Part="1" 
AR Path="/5BA1A77A/5BB28170" Ref="#PWR?"  Part="1" 
AR Path="/5BB1631B/5BB28170" Ref="#PWR?"  Part="1" 
AR Path="/5BC28DAC/5BB28170" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 5900 4400 50  0001 C CNN
F 1 "GND" H 5905 4477 50  0000 C CNN
F 2 "" H 5900 4650 50  0001 C CNN
F 3 "" H 5900 4650 50  0001 C CNN
	1    5900 4650
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 5BB5738A
P 5050 4350
AR Path="/5BA17F9C/5BB5738A" Ref="R?"  Part="1" 
AR Path="/5BA1A7F9/5BB5738A" Ref="R?"  Part="1" 
AR Path="/5BA1A77A/5BB5738A" Ref="R?"  Part="1" 
AR Path="/5BB5738A" Ref="R?"  Part="1" 
AR Path="/5BB1631B/5BB5738A" Ref="R?"  Part="1" 
AR Path="/5BC28DAC/5BB5738A" Ref="R?"  Part="1" 
F 0 "R?" V 5150 4350 50  0000 L CNN
F 1 "10k" V 4950 4350 50  0000 L CNN
F 2 "" V 4980 4350 50  0001 C CNN
F 3 "~" H 5050 4350 50  0001 C CNN
	1    5050 4350
	1    0    0    -1  
$EndComp
Wire Wire Line
	5300 4100 5050 4100
Wire Wire Line
	5050 4100 5050 4200
$Comp
L power:GND #PWR?
U 1 1 5BB573CA
P 5050 4650
AR Path="/5BA1A7F9/5BB573CA" Ref="#PWR?"  Part="1" 
AR Path="/5BA1A77A/5BB573CA" Ref="#PWR?"  Part="1" 
AR Path="/5BB1631B/5BB573CA" Ref="#PWR?"  Part="1" 
AR Path="/5BC28DAC/5BB573CA" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 5050 4400 50  0001 C CNN
F 1 "GND" H 5055 4477 50  0000 C CNN
F 2 "" H 5050 4650 50  0001 C CNN
F 3 "" H 5050 4650 50  0001 C CNN
	1    5050 4650
	1    0    0    -1  
$EndComp
Wire Wire Line
	5050 4650 5050 4500
$Comp
L Device:R R?
U 1 1 5BB57404
P 5050 2950
AR Path="/5BA17F9C/5BB57404" Ref="R?"  Part="1" 
AR Path="/5BA1A7F9/5BB57404" Ref="R?"  Part="1" 
AR Path="/5BA1A77A/5BB57404" Ref="R?"  Part="1" 
AR Path="/5BB57404" Ref="R?"  Part="1" 
AR Path="/5BB1631B/5BB57404" Ref="R?"  Part="1" 
AR Path="/5BC28DAC/5BB57404" Ref="R?"  Part="1" 
F 0 "R?" V 5150 2950 50  0000 L CNN
F 1 "10k" V 4950 2950 50  0000 L CNN
F 2 "" V 4980 2950 50  0001 C CNN
F 3 "~" H 5050 2950 50  0001 C CNN
	1    5050 2950
	1    0    0    -1  
$EndComp
Wire Wire Line
	5300 3500 4750 3500
Wire Wire Line
	4200 3600 5050 3600
Text HLabel 4200 3500 0    50   BiDi ~ 0
SDA
Text HLabel 4200 3600 0    50   Input ~ 0
SCL
Text HLabel 3550 5050 0    50   BiDi ~ 0
VBAT
$Comp
L Device:R R?
U 1 1 5BB59022
P 4750 2950
AR Path="/5BA17F9C/5BB59022" Ref="R?"  Part="1" 
AR Path="/5BA1A7F9/5BB59022" Ref="R?"  Part="1" 
AR Path="/5BA1A77A/5BB59022" Ref="R?"  Part="1" 
AR Path="/5BB59022" Ref="R?"  Part="1" 
AR Path="/5BB1631B/5BB59022" Ref="R?"  Part="1" 
AR Path="/5BC28DAC/5BB59022" Ref="R?"  Part="1" 
F 0 "R?" V 4850 2950 50  0000 L CNN
F 1 "10k" V 4650 2950 50  0000 L CNN
F 2 "" V 4680 2950 50  0001 C CNN
F 3 "~" H 4750 2950 50  0001 C CNN
	1    4750 2950
	1    0    0    -1  
$EndComp
Wire Wire Line
	4750 3100 4750 3500
Connection ~ 4750 3500
Wire Wire Line
	4750 3500 4200 3500
Wire Wire Line
	5050 3100 5050 3600
Connection ~ 5050 3600
Wire Wire Line
	5050 3600 5300 3600
$Comp
L Device:R R?
U 1 1 5BB59286
P 7300 3450
AR Path="/5BA17F9C/5BB59286" Ref="R?"  Part="1" 
AR Path="/5BA1A7F9/5BB59286" Ref="R?"  Part="1" 
AR Path="/5BA1A77A/5BB59286" Ref="R?"  Part="1" 
AR Path="/5BB59286" Ref="R?"  Part="1" 
AR Path="/5BB1631B/5BB59286" Ref="R?"  Part="1" 
AR Path="/5BC28DAC/5BB59286" Ref="R?"  Part="1" 
F 0 "R?" V 7400 3450 50  0000 L CNN
F 1 "10k" V 7200 3450 50  0000 L CNN
F 2 "" V 7230 3450 50  0001 C CNN
F 3 "~" H 7300 3450 50  0001 C CNN
	1    7300 3450
	1    0    0    -1  
$EndComp
Wire Wire Line
	7300 3600 7300 4150
Wire Wire Line
	7300 3150 7300 3300
$Comp
L Device:C_Small C?
U 1 1 5BB5956D
P 6800 3550
AR Path="/5BA17F9C/5BB5956D" Ref="C?"  Part="1" 
AR Path="/5BA1A7F9/5BB5956D" Ref="C?"  Part="1" 
AR Path="/5BA1A77A/5BB5956D" Ref="C?"  Part="1" 
AR Path="/5BB5956D" Ref="C?"  Part="1" 
AR Path="/5BB1631B/5BB5956D" Ref="C?"  Part="1" 
AR Path="/5BC28DAC/5BB5956D" Ref="C?"  Part="1" 
F 0 "C?" H 6950 3550 50  0000 L CNN
F 1 "0.47uF" H 6900 3450 50  0000 L CNN
F 2 "" H 6800 3550 50  0001 C CNN
F 3 "~" H 6800 3550 50  0001 C CNN
	1    6800 3550
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5BB59DD4
P 6800 3650
AR Path="/5BA1A7F9/5BB59DD4" Ref="#PWR?"  Part="1" 
AR Path="/5BA1A77A/5BB59DD4" Ref="#PWR?"  Part="1" 
AR Path="/5BB1631B/5BB59DD4" Ref="#PWR?"  Part="1" 
AR Path="/5BC28DAC/5BB59DD4" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 6800 3400 50  0001 C CNN
F 1 "GND" H 6805 3477 50  0000 C CNN
F 2 "" H 6800 3650 50  0001 C CNN
F 3 "" H 6800 3650 50  0001 C CNN
	1    6800 3650
	1    0    0    -1  
$EndComp
Wire Wire Line
	7300 4150 7650 4150
Text HLabel 7650 4150 2    50   Output ~ 0
GPOUT
Wire Wire Line
	5800 4300 5800 4350
Wire Wire Line
	5800 4350 5900 4350
Connection ~ 5900 4350
Wire Wire Line
	5900 4350 5900 4300
$Comp
L Device:R R?
U 1 1 5BB5A45B
P 4450 5050
AR Path="/5BA17F9C/5BB5A45B" Ref="R?"  Part="1" 
AR Path="/5BA1A7F9/5BB5A45B" Ref="R?"  Part="1" 
AR Path="/5BA1A77A/5BB5A45B" Ref="R?"  Part="1" 
AR Path="/5BB5A45B" Ref="R?"  Part="1" 
AR Path="/5BB1631B/5BB5A45B" Ref="R?"  Part="1" 
AR Path="/5BC28DAC/5BB5A45B" Ref="R?"  Part="1" 
F 0 "R?" V 4550 5050 50  0000 L CNN
F 1 "10m" V 4350 5050 50  0000 L CNN
F 2 "" V 4380 5050 50  0001 C CNN
F 3 "~" H 4450 5050 50  0001 C CNN
F 4 "1%" V 4450 5050 50  0000 C CNN "Tolerance"
	1    4450 5050
	0    1    1    0   
$EndComp
Wire Wire Line
	4300 5050 4200 5050
Wire Wire Line
	3800 5050 3800 3300
Wire Wire Line
	3800 3300 5300 3300
Wire Wire Line
	5300 3900 4200 3900
Wire Wire Line
	4200 3900 4200 5050
Connection ~ 4200 5050
Wire Wire Line
	4200 5050 3800 5050
Wire Wire Line
	4600 5050 4700 5050
Wire Wire Line
	4700 5050 4700 3800
Wire Wire Line
	4700 3800 5300 3800
Wire Wire Line
	4700 5050 5350 5050
Connection ~ 4700 5050
Wire Wire Line
	3550 5050 3800 5050
Connection ~ 3800 5050
Text HLabel 5350 5050 2    50   BiDi ~ 0
SYS
$Comp
L AXTEC_IC:+3V3_Internal #PWR?
U 1 1 5BD19F65
P 7300 3150
AR Path="/5BD19F65" Ref="#PWR?"  Part="1" 
AR Path="/5BC28DAC/5BD19F65" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 7300 3550 50  0001 C CNN
F 1 "+3V3_Internal" H 7285 3325 50  0000 C CNN
F 2 "" H 7300 3150 50  0001 C CNN
F 3 "" H 7300 3150 50  0001 C CNN
	1    7300 3150
	1    0    0    -1  
$EndComp
$Comp
L AXTEC_IC:+3V3_Internal #PWR?
U 1 1 5BD1A8BA
P 4900 2600
AR Path="/5BD1A8BA" Ref="#PWR?"  Part="1" 
AR Path="/5BC28DAC/5BD1A8BA" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 4900 3000 50  0001 C CNN
F 1 "+3V3_Internal" H 4885 2775 50  0000 C CNN
F 2 "" H 4900 2600 50  0001 C CNN
F 3 "" H 4900 2600 50  0001 C CNN
	1    4900 2600
	1    0    0    -1  
$EndComp
Wire Wire Line
	4750 2650 4900 2650
Wire Wire Line
	5050 2650 5050 2800
Wire Wire Line
	4750 2650 4750 2800
Wire Wire Line
	4900 2600 4900 2650
Connection ~ 4900 2650
Wire Wire Line
	4900 2650 5050 2650
Wire Wire Line
	6800 3400 6800 3450
Wire Wire Line
	6500 3400 6800 3400
Wire Wire Line
	7300 4150 6600 4150
Wire Wire Line
	6600 4150 6600 3700
Wire Wire Line
	6600 3700 6500 3700
Connection ~ 7300 4150
$EndSCHEMATC
