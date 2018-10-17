EESchema Schematic File Version 4
LIBS:PCB_B-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 5 6
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
L SamacSys_Parts:BQ24091DGQT U8
U 1 1 5BAD7302
P 5250 3550
F 0 "U8" H 5900 3815 50  0000 C CNN
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
VIN
Text HLabel 7250 3950 2    50   Input ~ 0
ISET
Text HLabel 7250 3650 2    50   BiDi ~ 0
VBAT
$Comp
L power:GND #PWR?
U 1 1 5BAD7944
P 3900 4300
AR Path="/5BA1A7F9/5BAD7944" Ref="#PWR?"  Part="1" 
AR Path="/5BA1A77A/5BAD7944" Ref="#PWR?"  Part="1" 
AR Path="/5BC300BF/5BAD7944" Ref="#PWR0148"  Part="1" 
F 0 "#PWR0148" H 3900 4050 50  0001 C CNN
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
AR Path="/5BA1A77A/5BAD7E3B" Ref="#PWR?"  Part="1" 
AR Path="/5BC300BF/5BAD7E3B" Ref="#PWR0149"  Part="1" 
F 0 "#PWR0149" H 4350 4050 50  0001 C CNN
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
AR Path="/5BA1A77A/5BAD7F3D" Ref="#PWR?"  Part="1" 
AR Path="/5BC300BF/5BAD7F3D" Ref="#PWR0150"  Part="1" 
F 0 "#PWR0150" H 4600 4050 50  0001 C CNN
F 1 "GND" H 4605 4127 50  0000 C CNN
F 2 "" H 4600 4300 50  0001 C CNN
F 3 "" H 4600 4300 50  0001 C CNN
	1    4600 4300
	1    0    0    -1  
$EndComp
Text Notes 5350 4300 0    50   ~ 0
Corriente de carga a 300mA
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
AR Path="/5BA1A77A/5BAD80E4" Ref="#PWR?"  Part="1" 
AR Path="/5BC300BF/5BAD80E4" Ref="#PWR0151"  Part="1" 
F 0 "#PWR0151" H 4800 4050 50  0001 C CNN
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
AR Path="/5BA1A77A/5BAD840C" Ref="R?"  Part="1" 
AR Path="/5BC300BF/5BAD840C" Ref="R16"  Part="1" 
F 0 "R16" V 7900 4100 50  0000 L CNN
F 1 "100k" V 7700 4100 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 7730 4100 50  0001 C CNN
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
AR Path="/5BA1A77A/5BAD8415" Ref="#PWR?"  Part="1" 
AR Path="/5BC300BF/5BAD8415" Ref="#PWR0152"  Part="1" 
F 0 "#PWR0152" H 7800 4050 50  0001 C CNN
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
AR Path="/5BA1A77A/5BAD886E" Ref="#PWR?"  Part="1" 
AR Path="/5BC300BF/5BAD886E" Ref="#PWR0153"  Part="1" 
F 0 "#PWR0153" H 6800 4050 50  0001 C CNN
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
AR Path="/5BA1A77A/5BAF219B" Ref="#PWR?"  Part="1" 
AR Path="/5BC300BF/5BAF219B" Ref="#PWR0154"  Part="1" 
F 0 "#PWR0154" H 7000 4050 50  0001 C CNN
F 1 "GND" H 7005 4127 50  0000 C CNN
F 2 "" H 7000 4300 50  0001 C CNN
F 3 "" H 7000 4300 50  0001 C CNN
	1    7000 4300
	1    0    0    -1  
$EndComp
Wire Wire Line
	7000 4250 7000 4300
$Comp
L Device:C_Small C?
U 1 1 5C6A0401
P 3900 4050
AR Path="/5BB122CA/5C6A0401" Ref="C?"  Part="1" 
AR Path="/5BB12371/5C6A0401" Ref="C?"  Part="1" 
AR Path="/5BB123AC/5C6A0401" Ref="C?"  Part="1" 
AR Path="/5BB6C3A1/5C6A0401" Ref="C?"  Part="1" 
AR Path="/5BC13C37/5C6A0401" Ref="C?"  Part="1" 
AR Path="/5BC14C3C/5C6A0401" Ref="C?"  Part="1" 
AR Path="/5BC300BF/5C6A0401" Ref="C25"  Part="1" 
F 0 "C25" H 3808 4004 50  0000 R CNN
F 1 "1uF" H 3808 4095 50  0000 R CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 3900 4050 50  0001 C CNN
F 3 "~" H 3900 4050 50  0001 C CNN
F 4 "10V" H 3900 4050 50  0001 C CNN "Voltage"
F 5 "LMK107B7105MA-T" H 3900 4050 50  0001 C CNN "Manufacturer Part Number"
	1    3900 4050
	-1   0    0    1   
$EndComp
$Comp
L Device:C_Small C?
U 1 1 5C6A063E
P 7000 4150
AR Path="/5BB122CA/5C6A063E" Ref="C?"  Part="1" 
AR Path="/5BB12371/5C6A063E" Ref="C?"  Part="1" 
AR Path="/5BB123AC/5C6A063E" Ref="C?"  Part="1" 
AR Path="/5BB6C3A1/5C6A063E" Ref="C?"  Part="1" 
AR Path="/5BC13C37/5C6A063E" Ref="C?"  Part="1" 
AR Path="/5BC14C3C/5C6A063E" Ref="C?"  Part="1" 
AR Path="/5BC300BF/5C6A063E" Ref="C26"  Part="1" 
F 0 "C26" H 6908 4104 50  0000 R CNN
F 1 "1uF" H 6908 4195 50  0000 R CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 7000 4150 50  0001 C CNN
F 3 "~" H 7000 4150 50  0001 C CNN
F 4 "10V" H 7000 4150 50  0001 C CNN "Voltage"
F 5 "LMK107B7105MA-T" H 7000 4150 50  0001 C CNN "Manufacturer Part Number"
	1    7000 4150
	-1   0    0    1   
$EndComp
$Comp
L Device:R R?
U 1 1 5C6A0A52
P 4800 4100
AR Path="/5BC13C37/5C6A0A52" Ref="R?"  Part="1" 
AR Path="/5BC14C3C/5C6A0A52" Ref="R?"  Part="1" 
AR Path="/5BC300BF/5C6A0A52" Ref="R15"  Part="1" 
F 0 "R15" H 4730 4008 50  0000 R CNN
F 1 "2k" H 4730 4100 50  0000 R CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 4730 4100 50  0001 C CNN
F 3 "~" H 4800 4100 50  0001 C CNN
F 4 "1%" H 4730 4192 50  0000 R CNN "Tolerance"
F 5 "RC0603FR-102KL" V 4800 4100 50  0001 C CNN "Manufacturer Part Number"
	1    4800 4100
	-1   0    0    1   
$EndComp
$Comp
L Device:R R?
U 1 1 5C6A0C34
P 4350 4100
AR Path="/5BC13C37/5C6A0C34" Ref="R?"  Part="1" 
AR Path="/5BC14C3C/5C6A0C34" Ref="R?"  Part="1" 
AR Path="/5BC300BF/5C6A0C34" Ref="R14"  Part="1" 
F 0 "R14" H 4280 4008 50  0000 R CNN
F 1 "1k8" H 4280 4100 50  0000 R CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 4280 4100 50  0001 C CNN
F 3 "~" H 4350 4100 50  0001 C CNN
F 4 "1%" H 4280 4192 50  0000 R CNN "Tolerance"
F 5 "RC0603FR-071K8L" V 4350 4100 50  0001 C CNN "Manufacturer Part Number"
	1    4350 4100
	-1   0    0    1   
$EndComp
$EndSCHEMATC
