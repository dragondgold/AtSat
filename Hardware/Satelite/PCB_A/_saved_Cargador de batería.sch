EESchema Schematic File Version 4
LIBS:PCB_A-cache
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
L SamacSys_Parts:BQ24091DGQT U?
U 1 1 5BAE1912
P 5300 3300
F 0 "U?" H 5950 3565 50  0000 C CNN
F 1 "BQ24091DGQT" H 5950 3474 50  0000 C CNN
F 2 "SamacSys_Parts:SOP50P490X110-11N" H 6450 3400 50  0001 L CNN
F 3 "http://www.ti.com/lit/gpn/bq24091" H 6450 3300 50  0001 L CNN
F 4 "Texas Instruments BQ24091DGQT, Battery Charge Controller, 1000mA, 10-Pin MSOP" H 6450 3200 50  0001 L CNN "Description"
F 5 "Texas Instruments" H 6450 3000 50  0001 L CNN "Manufacturer_Name"
F 6 "BQ24091DGQT" H 6450 2900 50  0001 L CNN "Manufacturer Part Number"
F 7 "8175824P" H 6450 2800 50  0001 L CNN "RS Part Number"
F 8 "http://uk.rs-online.com/web/p/products/8175824P" H 6450 2700 50  0001 L CNN "RS Price/Stock"
F 9 "BQ24091DGQT" H 6450 2600 50  0001 L CNN "Arrow Part Number"
F 10 "https://www.arrow.com/en/products/bq24091dgqt/texas-instruments" H 6450 2500 50  0001 L CNN "Arrow Price/Stock"
	1    5300 3300
	1    0    0    -1  
$EndComp
Text HLabel 4500 3300 0    50   Input ~ 0
VBUS
Wire Wire Line
	4500 3300 5300 3300
Text HLabel 4500 3400 0    50   Input ~ 0
ISET
Wire Wire Line
	4500 3400 5300 3400
$Comp
L power:GND #PWR?
U 1 1 5BAE1A98
P 4950 4100
F 0 "#PWR?" H 4950 3850 50  0001 C CNN
F 1 "GND" H 4955 3927 50  0000 C CNN
F 2 "" H 4950 4100 50  0001 C CNN
F 3 "" H 4950 4100 50  0001 C CNN
	1    4950 4100
	1    0    0    -1  
$EndComp
Wire Wire Line
	4950 4100 4950 3500
Wire Wire Line
	4950 3500 5300 3500
$EndSCHEMATC
