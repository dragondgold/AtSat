EESchema Schematic File Version 4
LIBS:PCB_A-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 6
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
L SamacSys_Parts:BQ24091DGQT U2
U 1 1 5BAD7302
P 5250 3550
F 0 "U2" H 5900 3815 50  0000 C CNN
F 1 "BQ24091DGQT" H 5900 3724 50  0000 C CNN
F 2 "SamacSys_Parts:SOP50P490X110-11N" H 6400 3650 50  0001 L CNN
F 3 "http://www.ti.com/lit/gpn/bq24091" H 6400 3550 50  0001 L CNN
F 4 "Texas Instruments BQ24091DGQT, Battery Charge Controller, 1000mA, 10-Pin MSOP" H 6400 3450 50  0001 L CNN "Description"
F 5 "Texas Instruments" H 6400 3250 50  0001 L CNN "Manufacturer_Name"
F 6 "BQ24091DGQT" H 6400 3150 50  0001 L CNN "Manufacturer Part Number"
F 7 "8175824P" H 6400 3050 50  0001 L CNN "RS Part Number"
F 8 "http://uk.rs-online.com/web/p/products/8175824P" H 6400 2950 50  0001 L CNN "RS Price/Stock"
F 9 "BQ24091DGQT" H 6400 2850 50  0001 L CNN "Arrow Part Number"
F 10 "https://www.arrow.com/en/products/bq24091dgqt/texas-instruments" H 6400 2750 50  0001 L CNN "Arrow Price/Stock"
	1    5250 3550
	1    0    0    -1  
$EndComp
Text HLabel 3550 3550 0    50   Input ~ 0
VBUS
Text HLabel 7250 3950 2    50   Input ~ 0
ISET
Text HLabel 7250 3650 2    50   Output ~ 0
VBat+
$Comp
L Device:C_Small C?
U 1 1 5BAD793D
P 3900 4050
AR Path="/5BA17F9C/5BAD793D" Ref="C?"  Part="1" 
AR Path="/5BA1A7F9/5BAD793D" Ref="C?"  Part="1" 
AR Path="/5BA1A77A/5BAD793D" Ref="C1"  Part="1" 
F 0 "C1" H 4050 4050 50  0000 L CNN
F 1 "1uF" H 4000 3950 50  0000 L CNN
F 2 "" H 3900 4050 50  0001 C CNN
F 3 "~" H 3900 4050 50  0001 C CNN
	1    3900 4050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5BAD7944
P 3900 4300
AR Path="/5BA1A7F9/5BAD7944" Ref="#PWR?"  Part="1" 
AR Path="/5BA1A77A/5BAD7944" Ref="#PWR015"  Part="1" 
F 0 "#PWR015" H 3900 4050 50  0001 C CNN
F 1 "GND" H 3905 4127 50  0000 C CNN
F 2 "" H 3900 4300 50  0001 C CNN
F 3 "" H 3900 4300 50  0001 C CNN
	1    3900 4300
	1    0    0    -1  
$EndComp
Wire Wire Line
	3900 4150 3900 4300
Wire Wire Line
	3900 3550 3900 3950
Wire Wire Line
	3900 3550 3550 3550
Connection ~ 3900 3550
Wire Wire Line
	7250 3950 6550 3950
$Comp
L Device:R R?
U 1 1 5BAD7C51
P 4350 4100
AR Path="/5BA17F9C/5BAD7C51" Ref="R?"  Part="1" 
AR Path="/5BA1A7F9/5BAD7C51" Ref="R?"  Part="1" 
AR Path="/5BA1A77A/5BAD7C51" Ref="R4"  Part="1" 
F 0 "R4" V 4450 4100 50  0000 L CNN
F 1 "1k8" V 4250 4100 50  0000 L CNN
F 2 "" V 4280 4100 50  0001 C CNN
F 3 "~" H 4350 4100 50  0001 C CNN
	1    4350 4100
	-1   0    0    1   
$EndComp
Wire Wire Line
	3900 3550 5250 3550
Wire Wire Line
	5250 3650 4350 3650
Wire Wire Line
	4350 3650 4350 3950
$Comp
L power:GND #PWR?
U 1 1 5BAD7E3B
P 4350 4300
AR Path="/5BA1A7F9/5BAD7E3B" Ref="#PWR?"  Part="1" 
AR Path="/5BA1A77A/5BAD7E3B" Ref="#PWR016"  Part="1" 
F 0 "#PWR016" H 4350 4050 50  0001 C CNN
F 1 "GND" H 4355 4127 50  0000 C CNN
F 2 "" H 4350 4300 50  0001 C CNN
F 3 "" H 4350 4300 50  0001 C CNN
	1    4350 4300
	1    0    0    -1  
$EndComp
Wire Wire Line
	4350 4300 4350 4250
Wire Wire Line
	5250 3750 4600 3750
Wire Wire Line
	4600 3750 4600 4300
$Comp
L power:GND #PWR?
U 1 1 5BAD7F3D
P 4600 4300
AR Path="/5BA1A7F9/5BAD7F3D" Ref="#PWR?"  Part="1" 
AR Path="/5BA1A77A/5BAD7F3D" Ref="#PWR017"  Part="1" 
F 0 "#PWR017" H 4600 4050 50  0001 C CNN
F 1 "GND" H 4605 4127 50  0000 C CNN
F 2 "" H 4600 4300 50  0001 C CNN
F 3 "" H 4600 4300 50  0001 C CNN
	1    4600 4300
	1    0    0    -1  
$EndComp
Text Notes 5350 4300 0    50   ~ 0
Corriente de carga a 300mA
$Comp
L Device:R R?
U 1 1 5BAD7FE1
P 4800 4100
AR Path="/5BA17F9C/5BAD7FE1" Ref="R?"  Part="1" 
AR Path="/5BA1A7F9/5BAD7FE1" Ref="R?"  Part="1" 
AR Path="/5BA1A77A/5BAD7FE1" Ref="R5"  Part="1" 
F 0 "R5" V 4900 4100 50  0000 L CNN
F 1 "2k" V 4700 4100 50  0000 L CNN
F 2 "" V 4730 4100 50  0001 C CNN
F 3 "~" H 4800 4100 50  0001 C CNN
	1    4800 4100
	-1   0    0    1   
$EndComp
Wire Wire Line
	4800 4300 4800 4250
Wire Wire Line
	5250 3850 4800 3850
Wire Wire Line
	4800 3850 4800 3950
$Comp
L power:GND #PWR?
U 1 1 5BAD80E4
P 4800 4300
AR Path="/5BA1A7F9/5BAD80E4" Ref="#PWR?"  Part="1" 
AR Path="/5BA1A77A/5BAD80E4" Ref="#PWR018"  Part="1" 
F 0 "#PWR018" H 4800 4050 50  0001 C CNN
F 1 "GND" H 4805 4127 50  0000 C CNN
F 2 "" H 4800 4300 50  0001 C CNN
F 3 "" H 4800 4300 50  0001 C CNN
	1    4800 4300
	1    0    0    -1  
$EndComp
Text Notes 5150 4400 0    50   ~ 0
Corriente de terminación 10% de carga
Text Notes 5150 4500 0    50   ~ 0
Corriente de pre-carga 20% de carga
Text HLabel 7250 3850 2    50   Output ~ 0
CHG
Wire Wire Line
	6550 3850 7250 3850
$Comp
L Device:R R?
U 1 1 5BAD840C
P 7800 4100
AR Path="/5BA17F9C/5BAD840C" Ref="R?"  Part="1" 
AR Path="/5BA1A7F9/5BAD840C" Ref="R?"  Part="1" 
AR Path="/5BA1A77A/5BAD840C" Ref="R6"  Part="1" 
F 0 "R6" V 7900 4100 50  0000 L CNN
F 1 "100k" V 7700 4100 50  0000 L CNN
F 2 "" V 7730 4100 50  0001 C CNN
F 3 "~" H 7800 4100 50  0001 C CNN
	1    7800 4100
	-1   0    0    1   
$EndComp
Wire Wire Line
	7800 4300 7800 4250
$Comp
L power:GND #PWR?
U 1 1 5BAD8415
P 7800 4300
AR Path="/5BA1A7F9/5BAD8415" Ref="#PWR?"  Part="1" 
AR Path="/5BA1A77A/5BAD8415" Ref="#PWR021"  Part="1" 
F 0 "#PWR021" H 7800 4050 50  0001 C CNN
F 1 "GND" H 7805 4127 50  0000 C CNN
F 2 "" H 7800 4300 50  0001 C CNN
F 3 "" H 7800 4300 50  0001 C CNN
	1    7800 4300
	1    0    0    -1  
$EndComp
Wire Wire Line
	6550 3750 7800 3750
Wire Wire Line
	7800 3750 7800 3950
Wire Wire Line
	6550 3650 7000 3650
Text HLabel 7900 3750 2    50   Input ~ 0
CHG_DISABLE
Wire Wire Line
	7800 3750 7900 3750
Connection ~ 7800 3750
$Comp
L power:GND #PWR?
U 1 1 5BAD886E
P 6800 4300
AR Path="/5BA1A7F9/5BAD886E" Ref="#PWR?"  Part="1" 
AR Path="/5BA1A77A/5BAD886E" Ref="#PWR019"  Part="1" 
F 0 "#PWR019" H 6800 4050 50  0001 C CNN
F 1 "GND" H 6805 4127 50  0000 C CNN
F 2 "" H 6800 4300 50  0001 C CNN
F 3 "" H 6800 4300 50  0001 C CNN
	1    6800 4300
	1    0    0    -1  
$EndComp
Wire Wire Line
	6800 3550 6550 3550
Wire Wire Line
	6800 3550 6800 4300
$Comp
L Device:C_Small C?
U 1 1 5BAF1DFA
P 7000 4150
AR Path="/5BA17F9C/5BAF1DFA" Ref="C?"  Part="1" 
AR Path="/5BA1A7F9/5BAF1DFA" Ref="C?"  Part="1" 
AR Path="/5BA1A77A/5BAF1DFA" Ref="C2"  Part="1" 
F 0 "C2" H 7150 4150 50  0000 L CNN
F 1 "1uF" H 7100 4050 50  0000 L CNN
F 2 "" H 7000 4150 50  0001 C CNN
F 3 "~" H 7000 4150 50  0001 C CNN
	1    7000 4150
	1    0    0    -1  
$EndComp
Wire Wire Line
	7000 3650 7000 4050
Connection ~ 7000 3650
Wire Wire Line
	7000 3650 7250 3650
$Comp
L power:GND #PWR?
U 1 1 5BAF219B
P 7000 4300
AR Path="/5BA1A7F9/5BAF219B" Ref="#PWR?"  Part="1" 
AR Path="/5BA1A77A/5BAF219B" Ref="#PWR020"  Part="1" 
F 0 "#PWR020" H 7000 4050 50  0001 C CNN
F 1 "GND" H 7005 4127 50  0000 C CNN
F 2 "" H 7000 4300 50  0001 C CNN
F 3 "" H 7000 4300 50  0001 C CNN
	1    7000 4300
	1    0    0    -1  
$EndComp
Wire Wire Line
	7000 4250 7000 4300
$EndSCHEMATC
