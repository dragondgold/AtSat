EESchema Schematic File Version 4
LIBS:EstacionTerrena-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 3
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
L Connector:USB_B_Micro J1
U 1 1 5B99984B
P 1150 4150
F 0 "J1" H 1150 4600 50  0000 C CNN
F 1 "USB_B_Micro" H 1205 4526 50  0000 C CNN
F 2 "" H 1300 4100 50  0001 C CNN
F 3 "~" H 1300 4100 50  0001 C CNN
	1    1150 4150
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR01
U 1 1 5B9998B4
P 1100 4650
F 0 "#PWR01" H 1100 4400 50  0001 C CNN
F 1 "GND" H 1105 4477 50  0000 C CNN
F 2 "" H 1100 4650 50  0001 C CNN
F 3 "" H 1100 4650 50  0001 C CNN
	1    1100 4650
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR02
U 1 1 5B9998ED
P 1600 3800
F 0 "#PWR02" H 1600 3650 50  0001 C CNN
F 1 "+5V" H 1615 3973 50  0000 C CNN
F 2 "" H 1600 3800 50  0001 C CNN
F 3 "" H 1600 3800 50  0001 C CNN
	1    1600 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	1450 3950 1600 3950
Wire Wire Line
	1600 3950 1600 3800
Wire Wire Line
	1050 4600 1050 4550
Wire Wire Line
	1150 4550 1150 4600
Wire Wire Line
	1050 4600 1100 4600
Wire Wire Line
	1100 4650 1100 4600
Connection ~ 1100 4600
Wire Wire Line
	1100 4600 1150 4600
$Comp
L AXTEC_IC:MCP2210 U2
U 1 1 5B9999E2
P 8250 4450
F 0 "U2" H 8250 5115 50  0000 C CNN
F 1 "MCP2210" H 8250 5024 50  0000 C CNN
F 2 "" H 8050 4550 50  0001 C CNN
F 3 "" H 8050 4550 50  0001 C CNN
	1    8250 4450
	1    0    0    -1  
$EndComp
Text Label 1650 4150 0    50   ~ 0
D+
Wire Wire Line
	1450 4150 1650 4150
Text Label 1650 4250 0    50   ~ 0
D-
Wire Wire Line
	1450 4250 1650 4250
Text Label 8800 4150 0    50   ~ 0
D+
Text Label 8800 4250 0    50   ~ 0
D-
Wire Wire Line
	8650 4250 8800 4250
Wire Wire Line
	8650 4150 8800 4150
$Comp
L Device:R R1
U 1 1 5B999D14
P 7350 3850
F 0 "R1" H 7420 3896 50  0000 L CNN
F 1 "10K" H 7420 3805 50  0000 L CNN
F 2 "" V 7280 3850 50  0001 C CNN
F 3 "~" H 7350 3850 50  0001 C CNN
	1    7350 3850
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR010
U 1 1 5B999E88
P 7350 3600
F 0 "#PWR010" H 7350 3450 50  0001 C CNN
F 1 "+3V3" H 7365 3773 50  0000 C CNN
F 2 "" H 7350 3600 50  0001 C CNN
F 3 "" H 7350 3600 50  0001 C CNN
	1    7350 3600
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR014
U 1 1 5B999F73
P 9100 4050
F 0 "#PWR014" H 9100 3800 50  0001 C CNN
F 1 "GND" H 9105 3877 50  0000 C CNN
F 2 "" H 9100 4050 50  0001 C CNN
F 3 "" H 9100 4050 50  0001 C CNN
	1    9100 4050
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR015
U 1 1 5B99A093
P 9250 4000
F 0 "#PWR015" H 9250 3850 50  0001 C CNN
F 1 "+3V3" H 9265 4173 50  0000 C CNN
F 2 "" H 9250 4000 50  0001 C CNN
F 3 "" H 9250 4000 50  0001 C CNN
	1    9250 4000
	1    0    0    -1  
$EndComp
Wire Wire Line
	9250 4350 9250 4000
Wire Wire Line
	8650 4350 9250 4350
Wire Wire Line
	8650 4050 9100 4050
Wire Wire Line
	7350 4000 7350 4350
Wire Wire Line
	7350 4350 7850 4350
Wire Wire Line
	7350 3600 7350 3700
$Comp
L power:+3V3 #PWR011
U 1 1 5B99A549
P 7750 3950
F 0 "#PWR011" H 7750 3800 50  0001 C CNN
F 1 "+3V3" H 7765 4123 50  0000 C CNN
F 2 "" H 7750 3950 50  0001 C CNN
F 3 "" H 7750 3950 50  0001 C CNN
	1    7750 3950
	1    0    0    -1  
$EndComp
Wire Wire Line
	7750 3950 7750 4050
Wire Wire Line
	7750 4050 7850 4050
$Comp
L AXTEC_IC:MIC5323-3.3YD5 U1
U 1 1 5B99A6D7
P 4550 4000
F 0 "U1" H 4550 4300 50  0000 C CNN
F 1 "MIC5323-3.3YD5" H 4550 4224 50  0000 C CNN
F 2 "" H 4550 4000 50  0001 C CNN
F 3 "" H 4550 4000 50  0001 C CNN
	1    4550 4000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR05
U 1 1 5B99A833
P 4550 4350
F 0 "#PWR05" H 4550 4100 50  0001 C CNN
F 1 "GND" H 4555 4177 50  0000 C CNN
F 2 "" H 4550 4350 50  0001 C CNN
F 3 "" H 4550 4350 50  0001 C CNN
	1    4550 4350
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C2
U 1 1 5B99A88E
P 5200 4200
F 0 "C2" H 5292 4246 50  0000 L CNN
F 1 "0.1uF" H 5292 4155 50  0000 L CNN
F 2 "" H 5200 4200 50  0001 C CNN
F 3 "~" H 5200 4200 50  0001 C CNN
	1    5200 4200
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C3
U 1 1 5B99A8E2
P 5650 4200
F 0 "C3" H 5742 4246 50  0000 L CNN
F 1 "2.2uF" H 5742 4155 50  0000 L CNN
F 2 "" H 5650 4200 50  0001 C CNN
F 3 "~" H 5650 4200 50  0001 C CNN
	1    5650 4200
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C1
U 1 1 5B99A90A
P 3750 4200
F 0 "C1" H 3842 4246 50  0000 L CNN
F 1 "2.2uF" H 3842 4155 50  0000 L CNN
F 2 "" H 3750 4200 50  0001 C CNN
F 3 "~" H 3750 4200 50  0001 C CNN
	1    3750 4200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR04
U 1 1 5B99A942
P 3750 4350
F 0 "#PWR04" H 3750 4100 50  0001 C CNN
F 1 "GND" H 3755 4177 50  0000 C CNN
F 2 "" H 3750 4350 50  0001 C CNN
F 3 "" H 3750 4350 50  0001 C CNN
	1    3750 4350
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR06
U 1 1 5B99A95B
P 5200 4350
F 0 "#PWR06" H 5200 4100 50  0001 C CNN
F 1 "GND" H 5205 4177 50  0000 C CNN
F 2 "" H 5200 4350 50  0001 C CNN
F 3 "" H 5200 4350 50  0001 C CNN
	1    5200 4350
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR08
U 1 1 5B99A974
P 5650 4350
F 0 "#PWR08" H 5650 4100 50  0001 C CNN
F 1 "GND" H 5655 4177 50  0000 C CNN
F 2 "" H 5650 4350 50  0001 C CNN
F 3 "" H 5650 4350 50  0001 C CNN
	1    5650 4350
	1    0    0    -1  
$EndComp
Wire Wire Line
	3750 4350 3750 4300
Wire Wire Line
	4550 4350 4550 4250
Wire Wire Line
	5200 4300 5200 4350
Wire Wire Line
	4950 4050 5200 4050
Wire Wire Line
	5200 4050 5200 4100
Wire Wire Line
	4950 3950 5650 3950
Wire Wire Line
	3750 4100 3750 3950
Wire Wire Line
	3750 3950 4050 3950
Wire Wire Line
	4150 4050 4050 4050
Wire Wire Line
	4050 4050 4050 3950
Connection ~ 4050 3950
Wire Wire Line
	4050 3950 4150 3950
$Comp
L power:+3V3 #PWR07
U 1 1 5B99BD7B
P 5650 3800
F 0 "#PWR07" H 5650 3650 50  0001 C CNN
F 1 "+3V3" H 5665 3973 50  0000 C CNN
F 2 "" H 5650 3800 50  0001 C CNN
F 3 "" H 5650 3800 50  0001 C CNN
	1    5650 3800
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR03
U 1 1 5B99C23C
P 3750 3800
F 0 "#PWR03" H 3750 3650 50  0001 C CNN
F 1 "+5V" H 3765 3973 50  0000 C CNN
F 2 "" H 3750 3800 50  0001 C CNN
F 3 "" H 3750 3800 50  0001 C CNN
	1    3750 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	3750 3800 3750 3950
Connection ~ 3750 3950
$Comp
L Device:Crystal_GND2 Y1
U 1 1 5B99C800
P 6850 4250
F 0 "Y1" H 6850 4518 50  0000 C CNN
F 1 "12MHz" H 6850 4427 50  0000 C CNN
F 2 "" H 6850 4250 50  0001 C CNN
F 3 "~" H 6850 4250 50  0001 C CNN
	1    6850 4250
	1    0    0    -1  
$EndComp
Wire Wire Line
	7000 4250 7850 4250
Wire Wire Line
	7850 4150 7150 4150
Wire Wire Line
	7150 4150 7150 3850
Wire Wire Line
	7150 3850 6600 3850
Wire Wire Line
	6600 3850 6600 4250
Wire Wire Line
	6600 4250 6700 4250
$Comp
L power:GND #PWR09
U 1 1 5B99D490
P 6850 4550
F 0 "#PWR09" H 6850 4300 50  0001 C CNN
F 1 "GND" H 6855 4377 50  0000 C CNN
F 2 "" H 6850 4550 50  0001 C CNN
F 3 "" H 6850 4550 50  0001 C CNN
	1    6850 4550
	1    0    0    -1  
$EndComp
Wire Wire Line
	6850 4450 6850 4550
Text HLabel 7650 4850 0    50   Output ~ 0
MOSI
Text HLabel 8850 4950 2    50   Output ~ 0
SCK
Text HLabel 8850 4750 2    50   Input ~ 0
MISO
Wire Wire Line
	7650 4850 7850 4850
Wire Wire Line
	8650 4950 8850 4950
Wire Wire Line
	8650 4750 8850 4750
$Comp
L Device:C_Small C5
U 1 1 5B99F697
P 9250 4500
F 0 "C5" H 9342 4546 50  0000 L CNN
F 1 "1uF" H 9342 4455 50  0000 L CNN
F 2 "" H 9250 4500 50  0001 C CNN
F 3 "~" H 9250 4500 50  0001 C CNN
	1    9250 4500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR016
U 1 1 5B99F6F1
P 9250 4650
F 0 "#PWR016" H 9250 4400 50  0001 C CNN
F 1 "GND" H 9255 4477 50  0000 C CNN
F 2 "" H 9250 4650 50  0001 C CNN
F 3 "" H 9250 4650 50  0001 C CNN
	1    9250 4650
	1    0    0    -1  
$EndComp
Wire Wire Line
	9250 4650 9250 4600
Wire Wire Line
	9250 4400 9250 4350
Connection ~ 9250 4350
$Comp
L Device:C_Small C4
U 1 1 5B9A0962
P 8300 3350
F 0 "C4" H 8392 3396 50  0000 L CNN
F 1 "0.1uF" H 8392 3305 50  0000 L CNN
F 2 "" H 8300 3350 50  0001 C CNN
F 3 "~" H 8300 3350 50  0001 C CNN
	1    8300 3350
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR013
U 1 1 5B9A09D3
P 8300 3500
F 0 "#PWR013" H 8300 3250 50  0001 C CNN
F 1 "GND" H 8305 3327 50  0000 C CNN
F 2 "" H 8300 3500 50  0001 C CNN
F 3 "" H 8300 3500 50  0001 C CNN
	1    8300 3500
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR012
U 1 1 5B9A0A7A
P 8300 3200
F 0 "#PWR012" H 8300 3050 50  0001 C CNN
F 1 "+3V3" H 8315 3373 50  0000 C CNN
F 2 "" H 8300 3200 50  0001 C CNN
F 3 "" H 8300 3200 50  0001 C CNN
	1    8300 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	8300 3250 8300 3200
Wire Wire Line
	8300 3450 8300 3500
Wire Wire Line
	5650 4350 5650 4300
Wire Wire Line
	5650 4100 5650 3950
Wire Wire Line
	5650 3800 5650 3950
Connection ~ 5650 3950
$Bitmap
Pos 8300 6800
Scale 1.000000
Data
89 50 4E 47 0D 0A 1A 0A 00 00 00 0D 49 48 44 52 00 00 02 E6 00 00 00 C9 08 02 00 00 00 E0 9E 60 
CE 00 00 00 03 73 42 49 54 08 08 08 DB E1 4F E0 00 00 0A 05 49 44 41 54 78 9C ED DD 5D 72 E2 48 
1A 05 50 7B A2 F6 85 58 19 62 65 88 95 31 0F EA F1 50 96 BB 8A 4C E5 DF 87 CE 79 EA 8E 00 57 86 
64 C8 AB CC 6B E9 F3 F1 78 7C 00 00 8C ED 3F BD 07 00 00 F0 77 22 0B 00 10 80 C8 02 00 04 20 B2 
00 00 01 88 2C 00 40 00 22 0B 00 10 80 C8 02 00 04 20 B2 00 00 01 88 2C 00 40 00 22 0B 00 10 80 
C8 02 00 04 20 B2 00 00 01 88 2C 00 40 00 22 0B 00 10 80 C8 02 00 04 20 B2 00 00 01 88 2C 00 40 
00 22 0B 00 10 80 C8 02 00 04 20 B2 00 00 01 88 2C 00 40 00 22 0B 00 10 80 C8 02 00 04 20 B2 00 
00 01 88 2C 00 40 00 22 0B 00 10 80 C8 02 00 04 20 B2 00 00 01 88 2C 00 40 00 22 0B 00 10 80 C8 
02 00 04 20 B2 00 00 01 88 2C 00 40 00 22 0B 00 10 80 C8 02 00 04 20 B2 00 00 01 88 2C 00 40 00 
22 0B 00 10 80 C8 02 00 04 20 B2 00 00 01 88 2C 00 40 00 22 0B 00 10 80 C8 02 00 04 20 B2 00 00 
01 88 2C 00 40 00 22 0B 00 10 80 C8 02 00 04 20 B2 00 00 01 88 2C 00 40 00 22 0B 00 10 80 C8 02 
00 44 70 B9 5C 7A 0F 81 DF 3C 5E E6 DC 8D E6 F5 73 F7 78 3C 7A 0F 96 DF 5C 2E 17 E7 2E A8 A4 73 
E7 6B 73 34 49 5F 9B 56 59 00 80 00 44 16 00 20 00 91 05 00 08 40 64 01 00 02 10 59 00 80 00 44 
16 00 20 00 91 05 00 08 40 64 01 00 02 10 59 00 80 00 44 16 00 20 00 91 05 00 08 40 64 01 00 02 
10 59 00 80 00 44 16 00 20 00 91 05 00 08 40 64 01 00 02 10 59 00 80 00 44 16 00 20 00 91 05 00 
08 40 64 01 00 02 10 59 00 80 00 44 16 00 20 00 91 05 00 08 40 64 01 00 02 10 59 00 80 00 44 16 
00 20 00 91 05 00 08 40 64 01 00 02 10 59 00 80 00 E2 45 96 D3 E9 F4 78 D9 E5 72 E9 3D DE A8 6E 
B7 DB EB C7 79 75 BB DD 5A 8E 30 E9 37 E1 CB E9 74 6A 39 C8 2E 32 0E 4B 14 6F FF 89 EE 7D 80 2B 
7A FB 73 77 B9 5C 7A 1F E3 8A 7A 1F DD 7F C4 8B 2C D3 34 BD FE E2 79 9E 6B 8D E3 DD 65 1C BA 69 
9A 9A A5 96 D3 E9 B4 2C 4B EA BB E6 79 BE DF EF 15 86 03 40 75 F1 22 4B EA 54 FA F6 D1 BE 92 FB 
FD 9E 94 0E 57 D3 34 B5 59 C6 C8 48 54 CB B2 5C AF D7 0A 63 01 A0 85 60 91 25 23 7F 64 CC BB AC 
EE F7 7B C6 4A C6 B2 2C B5 53 CB ED 76 CB 38 AD E7 F3 B9 C2 58 00 68 24 58 64 19 F9 BA FF 2D 9D 
CF E7 BC FD 97 F2 43 F9 9F D3 E9 94 F7 6B 50 7E 28 00 34 14 29 B2 E4 CD 55 1F A6 AB 7D F2 4A 2D 
95 F6 E3 54 58 00 0E 2B 52 64 C9 4E 1E 4A B8 7B E4 95 5A E6 79 AE B1 B8 A5 C2 02 70 58 91 22 CB 
9E E4 A1 84 BB 47 76 A9 A5 EC 30 54 58 00 8E 2C 4C 64 D9 99 39 EC 0D ED 94 57 6A 29 F8 37 CF 2A 
2C 00 07 17 26 B2 EC 9C 7B 94 70 F7 EB 58 6A 51 61 01 20 46 64 C9 2E DE 3E 73 C1 BD 53 C7 52 8B 
0A 0B 00 31 22 4B 91 B4 A1 84 BB 5F 97 52 8B 0A 0B 00 1F 51 22 4B A9 B4 A1 84 BB 5F E3 52 8B 0A 
0B 00 AB 00 91 A5 60 CE 30 93 15 D1 AC D4 A2 C2 02 C0 97 5F BD 07 F0 77 05 73 C6 5A C2 35 9F ED 
B4 96 5A 52 C3 C4 3C CF CB B2 24 1D 7C 15 96 3C 95 F6 40 A7 69 7A FD C3 58 69 0C C5 FF 72 3E AE 
70 3B DD CE DD 97 65 59 1C 8D 3C A3 47 96 22 C5 DB 67 D3 34 89 2C FB AD A5 96 D4 53 B3 2C CB E7 
E7 E7 8B 2F 56 61 C9 56 2F B4 BD 7E 46 04 C7 DA 1C E1 B8 5C 59 65 1B 7D 63 A8 F8 56 4E B8 4B 93 
61 55 2D B5 A8 B0 00 F0 CD E8 91 A5 46 C2 50 C2 2D A5 52 A9 45 85 05 80 AD A1 23 4B A5 6C E1 5A 
BC 94 4A 77 6A 51 61 01 60 6B E8 C8 52 29 5B B8 13 6E 41 C5 EF D4 A2 C2 02 C0 8F C6 8D 2C C5 8B 
B7 CF 2C B4 14 54 B0 D4 A2 C2 02 C0 BF 19 37 B2 64 FC 35 CA EB 2F 56 C2 2D AB 48 A9 45 85 05 80 
3F 18 37 B2 24 CD 82 CB B2 A4 CE 9A 4A B8 05 15 29 B5 A8 B0 00 F0 07 83 46 96 D4 3C B1 5E 6A 27 
5D A3 DB 4D 28 6B 67 A9 45 85 05 80 3F 1B 34 B2 A4 CE 5E EB D6 40 6A 64 51 C2 2D 2B BB D4 A2 C2 
02 C0 5F 8D 18 59 52 27 B0 AF 0D 85 D4 3D 02 73 5E 71 79 A5 16 15 16 00 FE 6A C4 C8 92 9A 24 9E 
93 8A 12 6E 5F 79 A5 96 54 2A 2C 00 07 34 62 64 49 2D DE 66 BF F7 43 09 B7 82 BC 52 4B 12 15 16 
80 03 1A 2E B2 64 14 6F 9F FF 37 75 B3 C0 DE 50 0D 79 A5 96 17 39 65 00 C7 34 DC 93 9C F3 8A B7 
CF E6 79 7E 7D AD 65 2D E1 6A 45 14 37 CF 73 8D D4 A2 C2 02 AB 11 56 88 97 65 F1 79 A4 A5 B1 22 
4B 76 F1 F6 D9 F5 7A 4D DA 1E 9A A6 C9 A7 AE B8 B5 D4 52 36 B5 A8 B0 C0 97 11 AA 78 2E 21 68 6C 
AC 8D A1 52 77 BC 55 C2 1D 41 F1 52 8B 0A 0B C0 91 8D 15 59 52 8B B7 FF 16 F0 53 67 CA 11 96 58 
DF 52 C1 52 8B 0A 0B C0 C1 0D 14 59 52 73 C3 1F E6 42 37 68 19 47 91 45 2C EB CF 00 0C 14 59 F6 
DC 8E 65 2B B5 CE E2 4E B8 95 EC BF 53 8B 0A 0B 00 1F E3 44 96 22 C5 DB 67 A9 FB 11 16 5A EA D9 
59 6A 51 61 01 E0 63 9C C8 52 AA 78 FB 25 75 9A 54 C2 AD 2A 3B 76 88 92 00 AC 46 89 2C A5 8A B7 
DF 5E 96 34 06 25 DC 7A B2 F7 DD 44 49 00 56 43 44 96 82 C5 DB 67 4A B8 E3 C8 4E 1E 6A 46 00 AC 
86 88 2C 65 8B B7 CF 94 70 47 70 BB DD F6 C4 C1 65 59 9C 17 00 FA 47 96 D4 E2 6D D2 76 8F 12 6E 
77 A9 E7 F7 47 B6 87 00 E8 1F 59 52 E7 B3 A4 D9 2B F5 66 1E A6 C6 B2 4E A7 53 91 5B C9 4D D3 74 
BB DD F6 FF 1C 00 E2 EA 1F 59 52 53 42 ED 14 A2 84 5B 50 C1 08 68 DB 0E E0 E0 3A 47 96 D4 7C 90 
31 05 2A E1 F6 B2 B3 C2 B2 A5 D4 02 70 64 9D 23 4B BD E2 ED B3 A4 BD 09 57 F3 45 14 A9 B0 6C D9 
B9 83 D5 E7 00 DC 96 9A C6 7A 46 96 AA C5 DB 67 A9 F3 9C 85 96 9D 4A 55 58 B6 94 5A 00 0E AB 67 
64 A9 5A BC 7D A6 84 DB 58 D5 03 68 19 0C E0 98 7A 46 96 DA C5 DB 3D FF 96 12 6E B6 E2 15 96 2D 
A5 16 80 03 EA 16 59 1A 14 6F 9F B9 41 4B 1B 95 2A 2C 5B 56 C2 00 8E A6 5B 64 29 FE 1C C4 3F 4B 
7D 4A A2 DD 87 0C 79 15 96 79 9E 3F 3F 3F 53 DF A5 D4 02 70 34 7D 22 4B 46 F1 76 CF AE D0 D7 0F 
49 7A BD 85 96 54 19 2B 1F CB B2 AC 7F 74 90 71 B4 C5 4A 80 43 E9 13 59 1A 2F B1 AC 52 FF 1E CF 
D6 43 92 BC 0A CB F9 7C 5E FF E3 7E BF E7 25 1E A9 05 E0 20 FA 44 96 D4 C9 A9 D4 5F FF 2B E1 56 
92 57 61 F9 F6 96 EB F5 9A B7 AF 94 FA 16 00 22 EA 10 59 1A 17 6F 9F D9 1B AA 21 BB C2 B2 DD EC 
FB 5A 74 79 9D 52 0B C0 41 74 88 2C 5D 76 85 56 4A B8 35 EC A9 B0 6C 29 B5 00 F0 A3 D6 91 A5 4B 
F1 F6 DB 0F 4C 7A BD 85 96 3F DB 59 61 D9 52 6A 01 E0 47 AD 23 4B C7 25 96 95 12 6E 41 45 2A 2C 
5B 4A 2D 00 6C B5 8E 2C BD 8A B7 CF 52 A7 43 25 DC 1F 15 AC B0 6C 29 B5 00 F0 4D D3 C8 92 3A F7 
57 7A B4 9E A7 24 16 51 B6 C2 B2 A5 D4 02 C0 B3 5F 2D FF B1 AA CF 41 AC 77 AB F8 75 22 2C 5B A9 
89 AE 78 85 65 6B 2D B5 A4 06 A3 65 59 A6 69 72 B2 78 7B E1 56 7F 8B 17 13 E3 8A 78 19 5C 63 C7 
23 43 BB C8 92 11 29 92 7E BF A7 69 AA D7 66 30 0B 3E AB 54 61 D9 BA 5E AF D3 34 65 24 DD 8C 7D 
25 88 25 5C 79 EB C5 4D E1 23 C8 F8 5A EB 6E 90 C8 D2 6E 63 A8 EA 12 4B 6D 43 0D A6 AF AA 15 96 
2D A5 16 00 56 ED 22 CB 08 C5 DB 3D C2 2D C3 56 52 BB C2 B2 A5 D4 02 C0 47 B3 C8 32 48 F1 76 8F 
70 EB 78 35 34 A8 B0 6C B9 53 0B 00 1F CD 22 4B E8 5D A1 95 0B F7 66 15 96 2D 77 6A 01 A0 45 64 
A9 5D BC 6D E6 C8 0B 2D 8D 2B 2C 3F FE A8 D4 B7 28 B5 00 BC 93 16 91 E5 0D 96 58 56 C3 0E AC 81 
F6 15 96 6F EE F7 BB 52 0B C0 91 B5 88 2C 19 B7 D6 A8 32 8E 12 8E 59 C2 ED 52 61 D9 4A 7D AA E5 
4A A9 05 E0 3D 54 8F 2C 19 C5 DB 31 77 85 56 07 DC 1B EA 58 61 D9 3A 9F CF 4A 2D 00 C7 54 3D B2 
74 7F 0E 62 59 47 DB 68 E8 5E 61 F9 F1 87 A7 BE 45 A9 05 E0 0D D4 8D 2C 19 17 E8 A3 DD 8E 65 EB 
50 0B 2D DD 2B 2C 5B 4A 2D 00 C7 54 37 B2 BC 4D F1 F6 59 88 41 16 31 48 85 65 4B A9 05 E0 80 EA 
46 96 77 2A DE 3E 3B 42 09 77 A8 0A CB 96 52 0B C0 D1 54 7C 2C E2 E9 74 4A 9D 21 F6 14 20 96 65 
69 36 21 45 89 56 7B 64 3C 66 B2 71 75 7A 9E E7 8C 84 E4 A1 DC D9 5A 7E C4 0E EB 8D 8F F0 DB 7F 
6D FA 80 B4 70 84 05 83 58 1E 2F 73 EE 46 F3 FA B9 7B 3C 1E BD 07 CB 6F 2E 97 8B 73 17 54 D2 B9 
F3 B5 39 9A A4 AF CD 76 8F 45 04 00 C8 26 B2 00 00 01 88 2C 00 40 00 22 0B 00 10 80 C8 02 00 04 
20 B2 00 00 01 88 2C 00 40 00 22 0B 00 10 80 C8 02 00 04 20 B2 00 00 01 88 2C 00 40 00 22 0B 00 
10 80 C8 02 00 04 20 B2 00 00 01 88 2C 00 40 00 22 0B 00 10 80 C8 02 00 04 20 B2 00 00 01 88 2C 
00 40 00 22 0B 00 10 80 C8 02 00 04 20 B2 00 00 01 88 2C 00 40 00 22 0B 00 10 80 C8 02 00 04 20 
B2 00 00 01 88 2C 00 40 00 22 0B 00 10 C0 AF 69 9A 7A 8F 81 4C CE 5D 68 97 CB A5 F7 10 F8 BF A4 
4F 93 73 37 94 A4 73 E7 6B 33 B4 CF C7 E3 D1 7B 0C 00 00 7F 61 63 08 00 08 40 64 01 00 02 10 59 
00 80 00 44 16 00 20 00 91 05 00 08 40 64 01 00 02 10 59 00 80 00 44 16 00 20 00 91 05 00 08 40 
64 01 00 02 10 59 00 80 00 44 16 00 20 00 91 05 00 08 40 64 01 00 02 F8 2F 61 32 11 0C AB 79 ED 
F6 00 00 00 00 49 45 4E 44 AE 42 60 82 
EndData
$EndBitmap
$Comp
L Power_Protection:TPD2E2U06 U4
U 1 1 5B9B2F66
P 2600 4150
F 0 "U4" H 2600 4500 50  0000 L CNN
F 1 "TPD2E2U06" H 2400 4400 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-543" H 1800 3900 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/tpd2e2u06.pdf" H 2400 4350 50  0001 C CNN
	1    2600 4150
	1    0    0    -1  
$EndComp
Text Label 2100 4050 0    50   ~ 0
D-
Text Label 2100 4250 0    50   ~ 0
D+
Wire Wire Line
	2100 4250 2300 4250
Wire Wire Line
	2100 4050 2300 4050
$Comp
L power:GND #PWR022
U 1 1 5B9B4FB0
P 2600 4500
F 0 "#PWR022" H 2600 4250 50  0001 C CNN
F 1 "GND" H 2605 4327 50  0000 C CNN
F 2 "" H 2600 4500 50  0001 C CNN
F 3 "" H 2600 4500 50  0001 C CNN
	1    2600 4500
	1    0    0    -1  
$EndComp
Wire Wire Line
	2600 4500 2600 4450
$Comp
L Device:R R2
U 1 1 5B9AE0AF
P 3750 5300
F 0 "R2" H 3820 5346 50  0000 L CNN
F 1 "100" H 3820 5255 50  0000 L CNN
F 2 "" V 3680 5300 50  0001 C CNN
F 3 "~" H 3750 5300 50  0001 C CNN
	1    3750 5300
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR023
U 1 1 5B9AE12F
P 3750 5100
F 0 "#PWR023" H 3750 4950 50  0001 C CNN
F 1 "+3V3" H 3765 5273 50  0000 C CNN
F 2 "" H 3750 5100 50  0001 C CNN
F 3 "" H 3750 5100 50  0001 C CNN
	1    3750 5100
	1    0    0    -1  
$EndComp
Wire Wire Line
	3750 5100 3750 5150
$Comp
L Device:LED D1
U 1 1 5B9AEC37
P 3750 5650
F 0 "D1" V 3788 5533 50  0000 R CNN
F 1 "LED" V 3697 5533 50  0000 R CNN
F 2 "" H 3750 5650 50  0001 C CNN
F 3 "~" H 3750 5650 50  0001 C CNN
	1    3750 5650
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR024
U 1 1 5B9AECC3
P 3750 5850
F 0 "#PWR024" H 3750 5600 50  0001 C CNN
F 1 "GND" H 3755 5677 50  0000 C CNN
F 2 "" H 3750 5850 50  0001 C CNN
F 3 "" H 3750 5850 50  0001 C CNN
	1    3750 5850
	1    0    0    -1  
$EndComp
Wire Wire Line
	3750 5800 3750 5850
Wire Wire Line
	3750 5500 3750 5450
$Comp
L Device:R R3
U 1 1 5B9B05BA
P 4250 5300
F 0 "R3" H 4320 5346 50  0000 L CNN
F 1 "100" H 4320 5255 50  0000 L CNN
F 2 "" V 4180 5300 50  0001 C CNN
F 3 "~" H 4250 5300 50  0001 C CNN
	1    4250 5300
	1    0    0    -1  
$EndComp
$Comp
L Device:LED D2
U 1 1 5B9B05C8
P 4250 5650
F 0 "D2" V 4288 5533 50  0000 R CNN
F 1 "LED" V 4197 5533 50  0000 R CNN
F 2 "" H 4250 5650 50  0001 C CNN
F 3 "~" H 4250 5650 50  0001 C CNN
	1    4250 5650
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR025
U 1 1 5B9B05CF
P 4250 5850
F 0 "#PWR025" H 4250 5600 50  0001 C CNN
F 1 "GND" H 4255 5677 50  0000 C CNN
F 2 "" H 4250 5850 50  0001 C CNN
F 3 "" H 4250 5850 50  0001 C CNN
	1    4250 5850
	1    0    0    -1  
$EndComp
Wire Wire Line
	4250 5800 4250 5850
Wire Wire Line
	4250 5500 4250 5450
Wire Wire Line
	4250 5000 4250 5150
Text Label 4250 5000 0    50   ~ 0
GP0
Text Label 7550 4450 0    50   ~ 0
GP0
Wire Wire Line
	7850 4450 7550 4450
$EndSCHEMATC
