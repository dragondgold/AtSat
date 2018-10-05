EESchema Schematic File Version 4
LIBS:PCB_B-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 6
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Sheet
S 1350 1250 500  200 
U 5BB122CA
F0 "Sensor temperatura y humedad" 50
F1 "Sensor temperatura y humedad.sch" 50
$EndSheet
Text HLabel 1850 1400 0    50   Input ~ 0
SCL
Text HLabel 1850 1300 0    50   BiDi ~ 0
SDA
$Sheet
S 1350 1800 500  200 
U 5BB12371
F0 "Sensor presión" 50
F1 "Sensor presión.sch" 50
$EndSheet
Text HLabel 1850 1850 0    50   BiDi ~ 0
SDA
Text HLabel 1850 1950 0    50   Input ~ 0
SCL
$Sheet
S 1350 2350 500  550 
U 5BB123AC
F0 "IMU" 50
F1 "IMU.sch" 50
$EndSheet
Text HLabel 1850 2400 0    50   BiDi ~ 0
SDA
Text HLabel 1850 2500 0    50   Input ~ 0
SCL
Text HLabel 1850 2650 0    50   Output ~ 0
ACC_INT
Text HLabel 1850 2750 0    50   Output ~ 0
MAG_INT
Text HLabel 1850 2850 0    50   Output ~ 0
GYRO_INT
$Comp
L Connector_Generic:Conn_02x02_Counter_Clockwise J?
U 1 1 5BB69B7B
P 10050 950
F 0 "J?" H 10100 1167 50  0000 C CNN
F 1 "Conn_02x02_Counter_Clockwise" H 10100 1076 50  0000 C CNN
F 2 "" H 10050 950 50  0001 C CNN
F 3 "~" H 10050 950 50  0001 C CNN
	1    10050 950 
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_02x02_Counter_Clockwise J?
U 1 1 5BB69BCB
P 10050 1500
F 0 "J?" H 10100 1717 50  0000 C CNN
F 1 "Conn_02x02_Counter_Clockwise" H 10100 1626 50  0000 C CNN
F 2 "" H 10050 1500 50  0001 C CNN
F 3 "~" H 10050 1500 50  0001 C CNN
	1    10050 1500
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_02x02_Counter_Clockwise J?
U 1 1 5BB69BF1
P 10050 2050
F 0 "J?" H 10100 2267 50  0000 C CNN
F 1 "Conn_02x02_Counter_Clockwise" H 10100 2176 50  0000 C CNN
F 2 "" H 10050 2050 50  0001 C CNN
F 3 "~" H 10050 2050 50  0001 C CNN
	1    10050 2050
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_02x02_Counter_Clockwise J?
U 1 1 5BB69C19
P 10050 2600
F 0 "J?" H 10100 2817 50  0000 C CNN
F 1 "Conn_02x02_Counter_Clockwise" H 10100 2726 50  0000 C CNN
F 2 "" H 10050 2600 50  0001 C CNN
F 3 "~" H 10050 2600 50  0001 C CNN
	1    10050 2600
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_02x02_Counter_Clockwise J?
U 1 1 5BB69C47
P 10050 3150
F 0 "J?" H 10100 3367 50  0000 C CNN
F 1 "Conn_02x02_Counter_Clockwise" H 10100 3276 50  0000 C CNN
F 2 "" H 10050 3150 50  0001 C CNN
F 3 "~" H 10050 3150 50  0001 C CNN
	1    10050 3150
	1    0    0    -1  
$EndComp
Wire Wire Line
	9850 950  9300 950 
Wire Wire Line
	9850 1050 9300 1050
Wire Wire Line
	10350 950  10900 950 
Wire Wire Line
	10350 1050 10900 1050
Wire Wire Line
	9850 1500 9300 1500
Wire Wire Line
	10350 1500 10900 1500
Wire Wire Line
	10350 1600 10900 1600
Wire Wire Line
	9300 1600 9850 1600
Wire Wire Line
	9300 2050 9850 2050
Wire Wire Line
	10350 2050 10900 2050
Wire Wire Line
	10350 2150 10900 2150
Wire Wire Line
	10350 2600 10900 2600
Wire Wire Line
	10350 2700 10900 2700
Wire Wire Line
	9850 2600 9300 2600
Wire Wire Line
	9850 2700 9300 2700
Wire Wire Line
	9850 3150 9300 3150
Wire Wire Line
	10350 3150 10900 3150
Wire Wire Line
	10350 3250 10900 3250
Text Label 9400 950  0    50   ~ 0
VBat
Text Label 9400 1050 0    50   ~ 0
VBUS
$Comp
L power:GND #PWR?
U 1 1 5BB6B290
P 10900 1100
F 0 "#PWR?" H 10900 850 50  0001 C CNN
F 1 "GND" H 10905 927 50  0000 C CNN
F 2 "" H 10900 1100 50  0001 C CNN
F 3 "" H 10900 1100 50  0001 C CNN
	1    10900 1100
	1    0    0    -1  
$EndComp
Wire Wire Line
	10900 1100 10900 1050
Connection ~ 10900 1050
Wire Wire Line
	10900 1050 10900 950 
Text Label 9400 1500 0    50   ~ 0
SDO
Text Label 9400 1600 0    50   ~ 0
SDI
Text Label 10550 1600 0    50   ~ 0
SCLK
Text Label 10550 1500 0    50   ~ 0
S_GLOBO
Text Label 9400 2050 0    50   ~ 0
GPS_TX
Text Label 10550 2050 0    50   ~ 0
P2
Text Label 10550 2150 0    50   ~ 0
S_PARACAIDAS
$Comp
L power:+3V3 #PWR?
U 1 1 5BB6B67F
P 9100 2000
F 0 "#PWR?" H 9100 1850 50  0001 C CNN
F 1 "+3V3" H 9115 2173 50  0000 C CNN
F 2 "" H 9100 2000 50  0001 C CNN
F 3 "" H 9100 2000 50  0001 C CNN
	1    9100 2000
	1    0    0    -1  
$EndComp
Wire Wire Line
	9100 2000 9100 2150
Wire Wire Line
	9100 2150 9850 2150
Text Label 9400 2600 0    50   ~ 0
CHG
Text Label 9400 2700 0    50   ~ 0
CHG_DISABLE
Text Label 10550 2600 0    50   ~ 0
ISET
Text Label 10550 2700 0    50   ~ 0
P1
Text Label 9350 3150 0    50   ~ 0
GPOUT
Text Label 10550 3150 0    50   ~ 0
SDA
Text Label 10550 3250 0    50   ~ 0
SCL
$Sheet
S 1300 3350 550  550 
U 5BB6C3A1
F0 "Power 3V3" 50
F1 "Power 3V3.sch" 50
F2 "VBAT" I R 1850 3450 50 
F3 "3V3" O R 1850 3850 50 
F4 "CTRL" I R 1850 3550 50 
F5 "ISENSE" O R 1850 3750 50 
$EndSheet
$Sheet
S 1300 4200 550  550 
U 5BB6C83B
F0 "Power 5V" 50
F1 "Power 5V.sch" 50
F2 "VBAT" I R 1850 4300 50 
F3 "CTRL" I R 1850 4400 50 
F4 "3V3" I R 1850 4700 50 
F5 "ISENSE" O R 1850 4600 50 
$EndSheet
Wire Wire Line
	9850 3250 9300 3250
Text Label 9350 3250 0    50   ~ 0
VBat_I_SENSE
$Comp
L MCU_ST_STM32F4:STM32F401RETx U?
U 1 1 5BB6D45B
P 5650 3000
F 0 "U?" H 5600 4878 50  0000 C CNN
F 1 "STM32F401RETx" H 5600 4787 50  0000 C CNN
F 2 "Package_QFP:LQFP-64_10x10mm_P0.5mm" H 5050 1300 50  0001 R CNN
F 3 "http://www.st.com/st-web-ui/static/active/en/resource/technical/document/datasheet/DM00102166.pdf" H 5650 3000 50  0001 C CNN
	1    5650 3000
	1    0    0    -1  
$EndComp
Wire Wire Line
	1850 1300 2200 1300
Wire Wire Line
	1850 1400 2300 1400
Wire Wire Line
	1850 1850 2200 1850
Wire Wire Line
	2200 1850 2200 1300
Connection ~ 2200 1300
Wire Wire Line
	2200 1300 3050 1300
Wire Wire Line
	1850 1950 2300 1950
Wire Wire Line
	2300 1950 2300 1400
Connection ~ 2300 1400
Wire Wire Line
	2300 1400 3050 1400
Wire Wire Line
	1850 2400 2200 2400
Wire Wire Line
	2200 2400 2200 1850
Connection ~ 2200 1850
Wire Wire Line
	1850 2500 2300 2500
Wire Wire Line
	2300 2500 2300 1950
Connection ~ 2300 1950
Text Label 2850 1300 0    50   ~ 0
SDA
Text Label 2850 1400 0    50   ~ 0
SCL
$Bitmap
Pos 8100 6800
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
