EESchema Schematic File Version 4
LIBS:PCB_C-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 4 4
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
L SamacSys_Parts:BMX055 IC5
U 1 1 5BB71FEC
P 5250 3050
F 0 "IC5" H 5850 3315 50  0000 C CNN
F 1 "BMX055" H 5850 3224 50  0000 C CNN
F 2 "SamacSys_Parts:LGA_PACKAGE_20_PINS" H 6300 3150 50  0001 L CNN
F 3 "http://www.mouser.com/ds/2/621/BST-BMX055-DS000-02-586421.pdf" H 6300 3050 50  0001 L CNN
F 4 "Small versatile 9-axis sensor module" H 6300 2950 50  0001 L CNN "Description"
F 5 "Bosch Sensortec" H 6300 2750 50  0001 L CNN "Manufacturer_Name"
F 6 "BMX055" H 6300 2650 50  0001 L CNN "Manufacturer Part Number"
F 7 "BMX055" H 6300 2350 50  0001 L CNN "Arrow Part Number"
	1    5250 3050
	1    0    0    -1  
$EndComp
Text HLabel 6600 3050 2    50   BiDi ~ 0
SDA
Text HLabel 5050 3850 0    50   Input ~ 0
SCL
Text HLabel 5050 3050 0    50   Output ~ 0
ACC_INT
Text HLabel 5050 3950 0    50   Output ~ 0
MAG_INT
Text HLabel 6500 3350 2    50   Output ~ 0
GYRO_INT
$Comp
L power:GND #PWR073
U 1 1 5BB72101
P 4300 3550
F 0 "#PWR073" H 4300 3300 50  0001 C CNN
F 1 "GND" H 4305 3377 50  0000 C CNN
F 2 "" H 4300 3550 50  0001 C CNN
F 3 "" H 4300 3550 50  0001 C CNN
	1    4300 3550
	1    0    0    -1  
$EndComp
Wire Wire Line
	5050 3050 5250 3050
Wire Wire Line
	4300 3050 4300 3250
Connection ~ 4300 3250
Wire Wire Line
	4300 3250 4300 3300
Wire Wire Line
	4300 3500 4300 3550
Wire Wire Line
	5250 3350 5200 3350
Wire Wire Line
	4650 3350 4650 3550
$Comp
L power:GND #PWR074
U 1 1 5BB7221A
P 4650 3550
F 0 "#PWR074" H 4650 3300 50  0001 C CNN
F 1 "GND" H 4655 3377 50  0000 C CNN
F 2 "" H 4650 3550 50  0001 C CNN
F 3 "" H 4650 3550 50  0001 C CNN
	1    4650 3550
	1    0    0    -1  
$EndComp
Wire Wire Line
	5250 3550 5200 3550
Wire Wire Line
	5200 3550 5200 3350
Connection ~ 5200 3350
Wire Wire Line
	5200 3350 4650 3350
Wire Wire Line
	5250 3650 4950 3650
Wire Wire Line
	4950 3650 4950 3600
Wire Wire Line
	5250 3850 5050 3850
Wire Wire Line
	5050 3950 5250 3950
$Comp
L power:GND #PWR080
U 1 1 5BB7256E
P 7400 4050
F 0 "#PWR080" H 7400 3800 50  0001 C CNN
F 1 "GND" H 7405 3877 50  0000 C CNN
F 2 "" H 7400 4050 50  0001 C CNN
F 3 "" H 7400 4050 50  0001 C CNN
	1    7400 4050
	1    0    0    -1  
$EndComp
Wire Wire Line
	7400 4050 7400 3150
Wire Wire Line
	7050 3050 7050 3250
$Comp
L power:GND #PWR079
U 1 1 5BB72CBF
P 7050 4050
F 0 "#PWR079" H 7050 3800 50  0001 C CNN
F 1 "GND" H 7055 3877 50  0000 C CNN
F 2 "" H 7050 4050 50  0001 C CNN
F 3 "" H 7050 4050 50  0001 C CNN
	1    7050 4050
	1    0    0    -1  
$EndComp
Wire Wire Line
	7050 4050 7050 3450
Wire Wire Line
	6450 3050 6600 3050
Wire Wire Line
	6450 3250 7050 3250
Wire Wire Line
	6500 3350 6450 3350
$Comp
L power:GND #PWR077
U 1 1 5BB7368C
P 6850 4050
F 0 "#PWR077" H 6850 3800 50  0001 C CNN
F 1 "GND" H 6855 3877 50  0000 C CNN
F 2 "" H 6850 4050 50  0001 C CNN
F 3 "" H 6850 4050 50  0001 C CNN
	1    6850 4050
	1    0    0    -1  
$EndComp
Wire Wire Line
	6850 4050 6850 3650
Wire Wire Line
	6850 3650 6450 3650
Wire Wire Line
	6450 3950 6650 3950
Wire Wire Line
	6650 3950 6650 4050
$Comp
L power:GND #PWR076
U 1 1 5BB741F0
P 6650 4050
F 0 "#PWR076" H 6650 3800 50  0001 C CNN
F 1 "GND" H 6655 3877 50  0000 C CNN
F 2 "" H 6650 4050 50  0001 C CNN
F 3 "" H 6650 4050 50  0001 C CNN
	1    6650 4050
	1    0    0    -1  
$EndComp
Wire Wire Line
	4300 3250 5250 3250
$Comp
L Device:C_Small C?
U 1 1 5BCC83DC
P 4300 3400
AR Path="/5BB122CA/5BCC83DC" Ref="C?"  Part="1" 
AR Path="/5BB123AC/5BCC83DC" Ref="C24"  Part="1" 
F 0 "C24" H 4392 3446 50  0000 L CNN
F 1 "100nF" H 4392 3355 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 4300 3400 50  0001 C CNN
F 3 "~" H 4300 3400 50  0001 C CNN
F 4 "GCM155R71C104KA55D" H 4300 3400 50  0001 C CNN "Manufacturer Part Number"
	1    4300 3400
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C?
U 1 1 5BCC861B
P 7050 3350
AR Path="/5BB122CA/5BCC861B" Ref="C?"  Part="1" 
AR Path="/5BB123AC/5BCC861B" Ref="C25"  Part="1" 
F 0 "C25" H 7142 3396 50  0000 L CNN
F 1 "100nF" H 7142 3305 50  0000 L CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 7050 3350 50  0001 C CNN
F 3 "~" H 7050 3350 50  0001 C CNN
F 4 "GCM155R71C104KA55D" H 7050 3350 50  0001 C CNN "Manufacturer Part Number"
	1    7050 3350
	1    0    0    -1  
$EndComp
Wire Wire Line
	6450 3150 7400 3150
Connection ~ 7050 3250
$Comp
L AXTEC_IC:+3V3_Internal #PWR?
U 1 1 5C5DF040
P 4300 3050
AR Path="/5C5DF040" Ref="#PWR?"  Part="1" 
AR Path="/5BB123AC/5C5DF040" Ref="#PWR20"  Part="1" 
F 0 "#PWR20" H 4300 3450 50  0001 C CNN
F 1 "+3V3_Internal" H 4285 3223 50  0000 C CNN
F 2 "" H 4300 3050 50  0001 C CNN
F 3 "" H 4300 3050 50  0001 C CNN
	1    4300 3050
	1    0    0    -1  
$EndComp
$Comp
L AXTEC_IC:+3V3_Internal #PWR?
U 1 1 5C5DF52E
P 7050 3050
AR Path="/5C5DF52E" Ref="#PWR?"  Part="1" 
AR Path="/5BB123AC/5C5DF52E" Ref="#PWR23"  Part="1" 
F 0 "#PWR23" H 7050 3450 50  0001 C CNN
F 1 "+3V3_Internal" H 7035 3223 50  0000 C CNN
F 2 "" H 7050 3050 50  0001 C CNN
F 3 "" H 7050 3050 50  0001 C CNN
	1    7050 3050
	1    0    0    -1  
$EndComp
$Comp
L AXTEC_IC:+3V3_Internal #PWR?
U 1 1 5C5E21C8
P 4950 3600
AR Path="/5C5E21C8" Ref="#PWR?"  Part="1" 
AR Path="/5BB123AC/5C5E21C8" Ref="#PWR25"  Part="1" 
F 0 "#PWR25" H 4950 4000 50  0001 C CNN
F 1 "+3V3_Internal" H 4935 3773 50  0000 C CNN
F 2 "" H 4950 3600 50  0001 C CNN
F 3 "" H 4950 3600 50  0001 C CNN
	1    4950 3600
	1    0    0    -1  
$EndComp
$Bitmap
Pos 8100 6850
Scale 1.000000
Data
89 50 4E 47 0D 0A 1A 0A 00 00 00 0D 49 48 44 52 00 00 02 71 00 00 00 AA 08 06 00 00 00 C2 EB 19 
FD 00 00 00 04 73 42 49 54 08 08 08 08 7C 08 64 88 00 00 00 09 70 48 59 73 00 00 0A F0 00 00 0A 
F0 01 42 AC 34 98 00 00 0A 3D 49 44 41 54 78 9C ED DD ED 71 DC D6 15 06 E0 73 33 FE 2F 77 20 75 
20 75 40 76 20 76 E0 ED C0 EC 40 4C 05 8E 2B 30 54 41 E8 0A B2 25 58 1D 28 1D 44 15 20 3F 08 C6 
CC 8A 30 89 BB 17 1F 07 FB 3C 33 9E B1 A5 C5 E2 8E 21 91 2F 2F DE B3 28 7D DF 07 00 00 B9 FC 6D 
ED 05 00 00 30 9D 10 07 00 90 90 10 07 00 90 90 10 07 00 90 90 10 07 00 90 90 10 07 00 90 90 10 
07 00 90 90 10 07 00 90 90 10 07 00 90 90 10 07 00 90 90 10 07 00 90 90 10 07 00 90 90 10 07 00 
90 90 10 07 00 90 90 10 07 00 90 90 10 07 00 90 90 10 07 00 90 90 10 07 00 90 90 10 07 00 90 90 
10 07 00 90 90 10 07 00 90 90 10 07 00 90 90 10 07 00 90 90 10 07 00 90 90 10 07 00 90 90 10 07 
00 90 90 10 07 00 90 90 10 07 00 90 90 10 07 00 90 90 10 07 00 90 90 10 07 00 90 90 10 07 00 90 
90 10 07 00 90 90 10 07 00 90 90 10 07 00 90 90 10 07 00 90 90 10 07 00 90 90 10 07 00 90 90 10 
07 00 90 90 10 07 00 90 90 10 07 00 90 90 10 07 00 90 90 10 07 00 90 50 89 88 BB B5 17 C1 59 8E 
7D DF 1F A7 1C 50 4A B9 9B 67 29 2C C4 35 BF 3C AE F9 E5 71 CD 2F CF E4 6B FE 43 44 7C 9A 67 2D 
2C E8 38 F1 F5 AE 79 7E C7 89 AF 77 CD F3 3B 4E 7C BD 6B 9E DF 71 E2 EB 5D F3 FC 8E 53 5E EC 76 
2A 00 40 42 42 1C 00 40 42 42 1C 00 40 42 42 1C 00 40 42 42 1C 00 40 42 42 1C 00 40 42 42 1C 00 
40 42 42 1C 00 40 42 42 1C 00 40 42 42 1C 00 40 42 42 1C 00 40 42 42 1C 00 40 42 42 1C 00 40 42 
42 1C 00 40 42 42 1C 00 40 42 42 1C 00 40 42 42 1C 00 40 42 42 1C 00 40 42 42 1C 00 40 42 42 1C 
00 40 42 42 1C 00 40 42 42 1C 00 40 42 42 1C 00 40 42 3F AC BD 80 86 FE 3E F1 F5 D7 11 71 35 C3 
3A 78 F0 39 22 BE 56 1E FB 21 22 3E B6 5B CA 24 DF 22 E2 1F 67 1C 7F 1B 11 6F 1A AD 25 9B A9 7F 
07 F7 E2 3A 2E F7 6B 89 6B 7E 79 5C F3 0D D9 4B 88 FB D2 F7 FD DD 94 03 4A 29 37 B1 C1 0B B2 23 
D7 11 F1 A1 EF FB FF 4C 3D B0 94 F2 63 44 1C 23 E2 7D E3 35 BD C6 6D DF F7 5D CD 81 A5 94 4B 0E 
70 31 F5 EF E0 5E 94 52 EE E2 42 BF 96 B8 E6 97 C7 35 DF 96 BD DC 4E 9D BC 73 D2 F7 FD 7D 44 FC 
7B 86 B5 F0 E0 6D 44 74 35 07 0E C1 EF 10 0F BB 62 4B FA 7C 46 80 FB 10 11 BF B4 5D 0E 00 8C DB 
43 88 FB 16 11 F7 95 C7 76 0D D7 C1 F7 3E 0E BB 53 93 F5 7D FF 47 3C DC 9A 5C CA 97 DA F3 0D 3B 
87 B5 7F 06 01 A0 CA 1E 42 DC 7D CD 2D BB 41 D7 72 21 3C EB 97 61 97 6A B2 61 57 EC 73 DB E5 3C 
EB 5B 44 1C CE FC 73 F4 B6 DD 72 00 E0 65 7B 08 71 D5 25 F4 BE EF BF 46 C4 EF ED 96 C2 88 FB 61 
B7 AA C6 6D 3C EC 92 CD E9 76 D8 F9 9B 6C D8 69 5C 6B 08 03 80 0B 96 3D C4 7D A9 FD E6 FB 44 D7 
62 21 FC A5 2D F7 E3 F4 E0 00 48 29 7B 88 3B E7 A3 20 22 C2 80 C3 82 B6 D8 8F D3 83 03 20 AD CC 
21 EE 9C 81 86 53 5D A3 F7 E1 AF 6D A9 1F A7 07 07 40 6A 99 43 DC 39 03 0D A7 BA 46 EF C3 CB B6 
D2 8F D3 83 03 20 B5 CC 21 EE EC 5B A9 8F 0C 38 2C 6A 0B FD 38 3D 38 00 D2 CB 1A E2 5A 0C 34 9C 
EA 1A BF 1F E3 D6 EC C7 E9 C1 01 B0 0B 59 43 5C B3 5D B8 47 06 1C 16 B7 46 3F 4E 0F 0E 80 DD C8 
F8 EC D4 96 03 0D A7 BA 88 F8 34 D3 7B F3 BD FB 52 4A D5 F3 55 E3 61 37 ED 43 4C 7B BE AA 1E DC 
8C 4A 29 FD DC E7 E8 FB BE 4C 79 FD F0 BC 43 7F A7 37 C2 F5 B8 3C AE F9 BC 32 EE C4 B5 1C 68 38 
D5 CD F4 BE 3C 6F C9 7E 9C 1E 1C 00 BB 92 31 C4 35 BF 95 FA C8 80 C3 2A 96 E8 C7 E9 C1 01 B0 3B 
D9 42 DC 1C 03 0D A7 BA 99 DF 9F EF CD D9 8F D3 83 03 60 97 B2 85 B8 D9 76 E1 1E 19 70 58 CD 5C 
9F 1F A7 07 07 C0 2E 65 0A 71 73 0E 34 9C EA 16 3A 0F 7F 9A A3 1F A7 07 07 C0 6E 65 0A 71 93 07 
1A 4A 29 3F 56 F6 AD BA 8A 63 38 5F CB 7E 9C 1E 1C 00 BB 96 29 C4 D5 DC 4A BD 89 8A 6F E4 06 1C 
56 D5 A2 1F A7 07 07 C0 EE 65 09 71 B5 03 0D B7 11 F1 B6 94 72 53 71 6C 57 71 0C 6D 9C DB 8F 3B 
E8 C1 01 B0 77 59 42 DC E4 5D B8 61 37 E7 F1 83 60 0F 53 8F 37 E0 B0 AA B3 FA 71 C3 B5 9B 4C 0F 
0E 80 4C 32 84 B8 DA 81 86 C3 93 7F FF 58 B9 B3 D3 55 1C 43 1B D5 FD B8 1A 7A 70 00 64 93 21 C4 
D5 3E A1 E1 F0 C2 7F BF 46 57 71 0C ED 54 F7 E3 2A 74 A1 07 07 40 22 19 9E 9D 5A 73 2B F5 10 11 
6F 4E 7E F9 76 EA 7B F5 7D FF B5 94 F2 7B E8 48 AD E9 9C E7 AB BE 8A 1E 1C 2C E6 B8 F6 02 46 78 
B6 27 29 6D 3D C4 D5 0E 34 1C 9E F9 B5 B7 A5 94 EB BE EF 8F 13 DF AB 0B DF E0 D7 F4 D8 8F AB 19 
4E 79 91 1E 1C 2C 67 F8 FA 7B 5C 79 19 DF 29 A5 08 71 A4 B4 F5 DB A9 35 BB 70 EF 22 E2 6A E4 B7 
0F 53 DF CF 80 C3 26 CC D2 8F D3 83 03 20 B3 2D 87 B8 16 03 0D A7 6E 0C 38 A4 35 47 3F AE 0B 3D 
38 00 92 DA 72 88 6B 35 D0 F0 D4 9B A8 BB 2D D7 55 1C 43 7B E7 7C 7E DC FF D1 83 03 20 BB 2D 87 
B8 9A 5B A9 37 F1 F2 CE 8A 27 38 E4 55 FD F9 71 4F E9 C1 01 B0 07 5B 0D 71 2D 07 1A 4E BD 1F 7A 
73 53 75 15 C7 D0 DE C7 61 FA F8 1C 7A 70 00 A4 B7 D5 10 57 B3 0B F7 63 BC FE F6 58 CD 6E 9C 01 
87 6D F8 16 E7 4F B7 09 71 00 A4 B7 C5 10 37 C7 40 C3 39 AF 7D AA AB 3C 8E 76 0E C3 ED ED 6A 7D 
DF DF 46 C4 97 36 CB 01 80 75 6C 31 C4 D5 0E 34 4C D9 5D 7B 53 79 4B AE AB 38 86 76 7E AD 7D 2E 
EA 33 6E E2 E1 07 06 00 48 69 8B 21 AE E6 56 EA 75 4C FF A8 88 C9 53 AA 06 1C 56 F5 65 D8 41 6B 
62 B8 96 87 56 EF 07 00 4B DB 5A 88 9B 73 A0 E1 D4 47 03 0E 69 7C 8B 19 9E D8 30 EC EA FD DA FA 
7D 01 60 09 5B 0B 71 B5 03 0D B5 DF E0 0F 53 0F 30 E0 B0 8A B3 7B 70 63 F4 E3 00 C8 6A 4B 21 AE 
76 A0 E1 26 BE 7F D8 FD 6B 1D 2A 8F EB 2A 8F 63 BA 96 3D B8 31 FA 71 B0 80 52 CA 5D 29 A5 DF DA 
3F 6B FF 7F 81 5A 5B 0A 71 4B 0C 34 9C 7A 3B F4 E9 A6 EA CE 38 27 AF D7 B4 07 37 46 3F 0E 80 8C 
B6 14 E2 6A 1F 76 FF FE CC F3 1E A6 1E 60 C0 61 11 B3 F4 E0 C6 E8 C7 01 90 CD 56 42 5C ED 40 43 
8B 5D 9A 9F 2A 9F C7 D9 35 38 37 E3 66 EB C1 8D D1 8F 03 20 93 AD 84 B8 C9 BB 70 83 43 A3 F3 4F 
7E 1F 03 0E B3 AA EE C1 95 52 AE 4B 29 C7 33 CE AD 1F 07 40 0A 5B 08 71 55 03 0D C3 87 F5 D6 0E 
34 9C 3A 54 1E D7 35 3A 3F 7F AA EE C1 0D 3B AA F7 11 71 55 4A B9 AB 79 0F FD 38 00 B2 D8 42 88 
AB 1D 68 68 D9 97 7A 5F 4A F9 50 71 5C D7 70 0D 9C DF 83 BB 8F 3F 83 FD A7 CA A1 15 FD 38 00 52 
D8 42 88 AB 1D 68 78 ED C3 EE 5F 6B F2 EE 8F 01 87 E6 AA 7B 70 C3 CE DB D5 C9 2F DF 57 F6 1D F5 
E3 00 D8 BC B5 43 DC 92 4F 68 78 C9 8D 01 87 55 9D D5 83 8B 88 4F CF FC D6 9B A8 FB EC C1 47 FA 
71 00 6C D6 DA 21 6E ED 81 86 A7 DE 44 DD F3 54 0D 38 9C AF 45 0F 6E 8C 7E 1C 00 BB B4 66 88 AB 
1D 68 B8 89 E9 0F BB 7F AD 43 E5 71 5D C3 35 5C 9A 96 3D B8 31 FA 71 00 EC CE 9A 21 6E 0B 03 0D 
A7 AE 86 BE DD 54 5D DB 65 5C 94 D6 3D B8 31 FA 71 00 EC CA 0F 2B 9E BB F6 61 F7 3F 4D 3C EC 73 
44 7C 9D F0 FA 77 13 5F 1F 7D DF 7F 2D A5 FC 1E ED 87 2D F6 6E 8E 1E DC 98 C7 7E DC 75 CD F9 E2 
E1 87 87 3F A2 DD C7 DA C0 25 3A AE BD 80 46 AE E3 F5 3F 40 5E BA E3 DA 0B 68 E4 3A 36 78 CD D7 
0A 71 4B 0E 34 74 7D DF 1F 2B 8E 9B 7C 9E 10 E2 A6 98 B3 07 37 E6 AA 94 72 D7 F7 FD DD D4 03 87 
A0 7E 88 88 7F 56 9C 17 88 88 E1 6B F1 71 E5 65 9C 6D E2 5D 80 8B E6 9A CF 6B AD DB A9 B5 03 0D 
B3 3F 0C BD 96 01 87 49 96 E8 C1 8D D1 8F 03 60 17 D6 08 71 B5 03 0D 1F 62 BE 81 86 56 BA B5 17 
90 C4 52 3D B8 31 FA 71 00 A4 B7 46 88 AB 1D 68 D8 EC 2E DC 13 DD DA 0B 48 60 C9 1E DC 18 9F 1F 
07 40 7A 6B 84 B8 DA 81 86 39 A7 52 9B F0 04 87 17 AD D1 83 1B E3 F3 E3 00 48 6D E9 10 57 3B D0 
70 13 79 A6 02 BB B5 17 B0 51 6B F6 E0 C6 E8 C7 01 90 D6 D2 21 6E 77 03 0D A7 0C 38 8C 5A BB 07 
37 46 3F 0E 80 94 96 0C 71 B5 03 0D EF 22 E2 7D EB C5 CC AC 5B 7B 01 1B B3 85 1E DC 18 FD 38 00 
52 5A 32 C4 ED 79 A0 E1 54 B7 F6 02 36 64 4B 3D B8 31 FA 71 00 A4 B3 64 88 DB D2 C3 EE 67 65 C0 
E1 7F B6 D8 83 1B A3 1F 07 40 2A 4B 85 B8 AA 81 86 E1 61 F7 59 06 1A 4E 75 6B 2F 60 03 B6 DA 83 
1B A3 1F 07 40 1A 4B 85 B8 8B D9 85 7B 64 C0 61 D3 3D B8 31 FA 71 00 A4 B1 D4 B3 53 7F 2B A5 FC 
B6 D0 B9 4E FD AB 94 B2 D2 A9 2F DA CF A5 94 9F D7 5E 44 85 AB 52 4A BF F6 22 78 9E 6B B3 2D AE 
C7 E5 71 CD B7 65 AD 67 A7 02 00 70 06 21 0E 00 20 21 21 0E 00 20 21 21 0E 00 20 21 21 0E 00 20 
21 21 0E 00 20 21 21 0E 00 20 21 21 0E 00 20 21 21 0E 00 20 21 21 0E 00 20 21 21 0E 00 20 21 21 
0E 00 20 21 21 0E 00 20 21 21 0E 00 20 21 21 0E 00 20 21 21 0E 00 20 21 21 0E 00 20 21 21 0E 00 
20 21 21 0E 00 20 21 21 0E 00 20 21 21 0E 00 20 21 21 0E 00 20 21 21 0E 00 20 21 21 0E 00 20 A1 
D2 F7 FD DA 6B 00 00 60 22 3B 71 00 00 09 09 71 00 00 09 09 71 00 00 09 09 71 00 00 09 09 71 00 
00 09 09 71 00 00 09 09 71 00 00 09 09 71 00 00 09 09 71 00 00 09 09 71 00 00 09 09 71 00 00 09 
09 71 00 00 09 FD 17 3C 6D 9D 90 5F F2 D3 C3 00 00 00 00 49 45 4E 44 AE 42 60 82 
EndData
$EndBitmap
$EndSCHEMATC
