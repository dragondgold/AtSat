EESchema Schematic File Version 4
LIBS:PCB_C-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 4
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
L SamacSys_Parts:Si7006-A20-IM1 U6
U 1 1 5BB7134A
P 5100 3150
F 0 "U6" H 5700 3415 50  0000 C CNN
F 1 "Si7006-A20-IM1" H 5700 3324 50  0000 C CNN
F 2 "SamacSys_Parts:SON100P300X300X80-7N" H 6150 3250 50  0001 L CNN
F 3 "https://www.silabs.com/documents/public/data-sheets/Si7006-A20.pdf" H 6150 3150 50  0001 L CNN
F 4 "Si7006 Digital RH temp sensor w filter Silicon Labs Si7006-A20-IM1 Temperature & Humidity Sensor, -40  +125 C, 6-Pin DFN" H 6150 3050 50  0001 L CNN "Description"
F 5 "0.8" H 6150 2950 50  0001 L CNN "Height"
F 6 "Silicon Labs" H 6150 2850 50  0001 L CNN "Manufacturer_Name"
F 7 "Si7006-A20-IM1" H 6150 2750 50  0001 L CNN "Manufacturer Part Number"
F 8 "8652283P" H 6150 2650 50  0001 L CNN "RS Part Number"
F 9 "http://uk.rs-online.com/web/p/products/8652283P" H 6150 2550 50  0001 L CNN "RS Price/Stock"
F 10 "Si7006-A20-IM1" H 6150 2450 50  0001 L CNN "Arrow Part Number"
F 11 "https://www.arrow.com/en/products/si7006-a20-im1/silicon-labs" H 6150 2350 50  0001 L CNN "Arrow Price/Stock"
	1    5100 3150
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C22
U 1 1 5BB71601
P 6900 3450
F 0 "C22" H 6992 3496 50  0000 L CNN
F 1 "100nF" H 6992 3405 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 6900 3450 50  0001 C CNN
F 3 "~" H 6900 3450 50  0001 C CNN
F 4 "EMK107B7104KAHT" H 6900 3450 50  0001 C CNN "Manufacturer Part Number"
	1    6900 3450
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR067
U 1 1 5BB716EE
P 6900 3000
F 0 "#PWR067" H 6900 2850 50  0001 C CNN
F 1 "+3V3" H 6915 3173 50  0000 C CNN
F 2 "" H 6900 3000 50  0001 C CNN
F 3 "" H 6900 3000 50  0001 C CNN
	1    6900 3000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR068
U 1 1 5BB71724
P 6900 3650
F 0 "#PWR068" H 6900 3400 50  0001 C CNN
F 1 "GND" H 6905 3477 50  0000 C CNN
F 2 "" H 6900 3650 50  0001 C CNN
F 3 "" H 6900 3650 50  0001 C CNN
	1    6900 3650
	1    0    0    -1  
$EndComp
Wire Wire Line
	6300 3250 6900 3250
Wire Wire Line
	6900 3250 6900 3000
Wire Wire Line
	6900 3350 6900 3250
Connection ~ 6900 3250
Wire Wire Line
	6900 3650 6900 3550
$Comp
L power:GND #PWR066
U 1 1 5BB717C2
P 4900 3700
F 0 "#PWR066" H 4900 3450 50  0001 C CNN
F 1 "GND" H 4905 3527 50  0000 C CNN
F 2 "" H 4900 3700 50  0001 C CNN
F 3 "" H 4900 3700 50  0001 C CNN
	1    4900 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	4900 3700 4900 3250
Wire Wire Line
	4900 3250 5100 3250
Text HLabel 6450 3150 2    50   Input ~ 0
SCL
Text HLabel 4950 3150 0    50   BiDi ~ 0
SDA
Wire Wire Line
	4950 3150 5100 3150
Wire Wire Line
	6300 3150 6450 3150
$EndSCHEMATC
