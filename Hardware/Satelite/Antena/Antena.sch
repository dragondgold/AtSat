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
L SamacSys_Parts:0915AT43A0026E ANT1
U 1 1 5C464D8D
P 5700 3400
F 0 "ANT1" H 6200 3666 50  0000 C CNN
F 1 "0915AT43A0026E" H 6200 3574 50  0000 C CNN
F 2 "ANTC8020X90N" H 6550 3500 50  0001 L CNN
F 3 "http://www.johansontechnology.com/images/stories/ip/rf-antennas/Antenna_0915AT43A0026_v2.pdf" H 6550 3400 50  0001 L CNN
F 4 "JOHANSON TECHNOLOGY - 0915AT43A0026E - ANTENNA, CERAMIC, 915MHZ" H 6550 3300 50  0001 L CNN "Description"
F 5 "0.9" H 6550 3200 50  0001 L CNN "Height"
F 6 "JOHANSON TECHNOLOGY" H 6550 3100 50  0001 L CNN "Manufacturer_Name"
F 7 "0915AT43A0026E" H 6550 3000 50  0001 L CNN "Manufacturer_Part_Number"
F 8 "" H 6550 2900 50  0001 L CNN "RS Part Number"
F 9 "" H 6550 2800 50  0001 L CNN "RS Price/Stock"
F 10 "609-0915AT43A0026E" H 6550 2700 50  0001 L CNN "Mouser Part Number"
F 11 "https://www.mouser.com/Search/Refine.aspx?Keyword=609-0915AT43A0026E" H 6550 2600 50  0001 L CNN "Mouser Price/Stock"
	1    5700 3400
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x01 J1
U 1 1 5C464F37
P 5150 3400
F 0 "J1" H 5070 3174 50  0000 C CNN
F 1 "Conn_01x01" H 5070 3266 50  0000 C CNN
F 2 "TestPoint:TestPoint_Pad_3.0x3.0mm" H 5150 3400 50  0001 C CNN
F 3 "~" H 5150 3400 50  0001 C CNN
	1    5150 3400
	-1   0    0    1   
$EndComp
Wire Wire Line
	5350 3400 5700 3400
$Comp
L Connector_Generic:Conn_01x01 J2
U 1 1 5C464FB8
P 5150 3800
F 0 "J2" H 5070 3574 50  0000 C CNN
F 1 "Conn_01x01" H 5070 3666 50  0000 C CNN
F 2 "TestPoint:TestPoint_Pad_3.0x3.0mm" H 5150 3800 50  0001 C CNN
F 3 "~" H 5150 3800 50  0001 C CNN
	1    5150 3800
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR01
U 1 1 5C465011
P 5500 3850
F 0 "#PWR01" H 5500 3600 50  0001 C CNN
F 1 "GND" H 5504 3676 50  0000 C CNN
F 2 "" H 5500 3850 50  0001 C CNN
F 3 "" H 5500 3850 50  0001 C CNN
	1    5500 3850
	1    0    0    -1  
$EndComp
Wire Wire Line
	5350 3800 5500 3800
Wire Wire Line
	5500 3800 5500 3850
$Comp
L Mechanical:MountingHole H1
U 1 1 5C4651B3
P 6200 3850
F 0 "H1" H 6300 3896 50  0000 L CNN
F 1 "MountingHole" H 6300 3804 50  0000 L CNN
F 2 "MountingHole:MountingHole_2.2mm_M2" H 6200 3850 50  0001 C CNN
F 3 "~" H 6200 3850 50  0001 C CNN
	1    6200 3850
	1    0    0    -1  
$EndComp
$EndSCHEMATC
