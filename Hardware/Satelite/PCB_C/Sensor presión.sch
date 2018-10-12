EESchema Schematic File Version 4
LIBS:PCB_C-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 4
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
L SamacSys_Parts:MS563702BA03-50 IC?
U 1 1 5BB71B9C
P 5250 3350
F 0 "IC?" H 5750 3615 50  0000 C CNN
F 1 "MS563702BA03-50" H 5750 3524 50  0000 C CNN
F 2 "MS5637_1" H 6100 3450 50  0001 L CNN
F 3 "http://docs-europe.electrocomponents.com/webdocs/142c/0900766b8142cddb.pdf" H 6100 3350 50  0001 L CNN
F 4 "Sensor Pressure 2bar Barometric SMD MS563702BA03-50, Barometric Pressure Sensor, 1200mbar 0 ??? 3.6 V Output, 4-Pin QFN" H 6100 3250 50  0001 L CNN "Description"
F 5 "" H 6100 3150 50  0001 L CNN "Height"
F 6 "TE Connectivity" H 6100 3050 50  0001 L CNN "Manufacturer_Name"
F 7 "MS563702BA03-50" H 6100 2950 50  0001 L CNN "Manufacturer_Part_Number"
F 8 "1704370" H 6100 2850 50  0001 L CNN "RS Part Number"
F 9 "http://uk.rs-online.com/web/p/products/1704370" H 6100 2750 50  0001 L CNN "RS Price/Stock"
F 10 "MS563702BA03-50" H 6100 2650 50  0001 L CNN "Arrow Part Number"
F 11 "https://www.arrow.com/en/products/ms563702ba03-50/te-connectivity" H 6100 2550 50  0001 L CNN "Arrow Price/Stock"
	1    5250 3350
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR?
U 1 1 5BB71C29
P 4800 3250
F 0 "#PWR?" H 4800 3100 50  0001 C CNN
F 1 "+3V3" H 4815 3423 50  0000 C CNN
F 2 "" H 4800 3250 50  0001 C CNN
F 3 "" H 4800 3250 50  0001 C CNN
	1    4800 3250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5BB71C54
P 4800 3800
F 0 "#PWR?" H 4800 3550 50  0001 C CNN
F 1 "GND" H 4805 3627 50  0000 C CNN
F 2 "" H 4800 3800 50  0001 C CNN
F 3 "" H 4800 3800 50  0001 C CNN
	1    4800 3800
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C?
U 1 1 5BB71D0F
P 4800 3700
AR Path="/5BB122CA/5BB71D0F" Ref="C?"  Part="1" 
AR Path="/5BB12371/5BB71D0F" Ref="C?"  Part="1" 
F 0 "C?" H 4708 3654 50  0000 R CNN
F 1 "1uF" H 4708 3745 50  0000 R CNN
F 2 "" H 4800 3700 50  0001 C CNN
F 3 "~" H 4800 3700 50  0001 C CNN
	1    4800 3700
	-1   0    0    1   
$EndComp
Wire Wire Line
	5250 3350 4800 3350
Wire Wire Line
	4800 3350 4800 3600
Wire Wire Line
	4800 3250 4800 3350
Connection ~ 4800 3350
$Comp
L power:GND #PWR?
U 1 1 5BB71DBB
P 6350 3800
F 0 "#PWR?" H 6350 3550 50  0001 C CNN
F 1 "GND" H 6355 3627 50  0000 C CNN
F 2 "" H 6350 3800 50  0001 C CNN
F 3 "" H 6350 3800 50  0001 C CNN
	1    6350 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	6350 3800 6350 3450
Wire Wire Line
	6350 3450 6250 3450
Text HLabel 6500 3350 2    50   Input ~ 0
SCL
Wire Wire Line
	6500 3350 6250 3350
Text HLabel 5050 3450 0    50   BiDi ~ 0
SDA
Wire Wire Line
	5050 3450 5250 3450
$EndSCHEMATC
