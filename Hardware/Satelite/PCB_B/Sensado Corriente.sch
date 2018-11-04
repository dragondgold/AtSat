EESchema Schematic File Version 4
LIBS:PCB_B-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 4 6
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text HLabel 5550 4500 3    50   Output ~ 0
ISENSE
Text HLabel 6450 2500 2    50   Output ~ 0
VOUT
Text HLabel 4700 2500 0    50   Input ~ 0
VIN
$Comp
L Device:R R?
U 1 1 5BB84510
P 5550 2500
AR Path="/5BA17F9C/5BB84510" Ref="R?"  Part="1" 
AR Path="/5BA1A7F9/5BB84510" Ref="R?"  Part="1" 
AR Path="/5BA1A77A/5BB84510" Ref="R?"  Part="1" 
AR Path="/5BB84510" Ref="R?"  Part="1" 
AR Path="/5BB7A164/5BB84510" Ref="R?"  Part="1" 
AR Path="/5BC24746/5BB84510" Ref="R12"  Part="1" 
F 0 "R12" V 5650 2500 50  0000 L CNN
F 1 "51m" V 5450 2500 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 5480 2500 50  0001 C CNN
F 3 "~" H 5550 2500 50  0001 C CNN
F 4 "1%" V 5550 2500 50  0000 C CNN "Tolerance"
F 5 "KRL1632E-C-R051-F-T1" V 5550 2500 50  0001 C CNN "Manufacturer Part Number"
	1    5550 2500
	0    1    1    0   
$EndComp
$Comp
L Amplifier_Current:INA194 U7
U 1 1 5BB84611
P 5550 3900
F 0 "U7" V 5650 3750 50  0000 R CNN
F 1 "INA194" V 5750 3750 50  0000 R CNN
F 2 "Package_TO_SOT_SMD:SOT-23-5" H 5550 3900 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/ina193.pdf" H 5550 3900 50  0001 C CNN
	1    5550 3900
	0    -1   1    0   
$EndComp
Wire Wire Line
	5700 2500 5800 2500
Wire Wire Line
	4700 2500 5300 2500
Wire Wire Line
	5800 2650 5800 2500
Connection ~ 5800 2500
Wire Wire Line
	5800 2500 6450 2500
Wire Wire Line
	5300 2650 5300 2500
Connection ~ 5300 2500
Wire Wire Line
	5300 2500 5400 2500
Wire Wire Line
	5450 3100 5300 3100
Wire Wire Line
	5300 3100 5300 2950
Wire Wire Line
	5650 3100 5800 3100
Wire Wire Line
	5800 3100 5800 2950
Wire Wire Line
	5300 3100 5300 3500
Wire Wire Line
	5300 3500 5450 3500
Wire Wire Line
	5450 3500 5450 3600
Connection ~ 5300 3100
Wire Wire Line
	5800 3100 5800 3500
Wire Wire Line
	5800 3500 5650 3500
Wire Wire Line
	5650 3500 5650 3600
Connection ~ 5800 3100
Text Notes 6100 3200 0    50   ~ 0
Frecuencia de corte dada por:\n\n1/(2*pi*2R*C) = 1.6 kHz\n\nEsto da un error de ganancia del 1% y \nfiltra las frecuencias que no son de\ninterés para el ADC.
$Comp
L power:GND #PWR0145
U 1 1 5BB84C5F
P 6100 4100
F 0 "#PWR0145" H 6100 3850 50  0001 C CNN
F 1 "GND" H 6105 3927 50  0000 C CNN
F 2 "" H 6100 4100 50  0001 C CNN
F 3 "" H 6100 4100 50  0001 C CNN
	1    6100 4100
	1    0    0    -1  
$EndComp
Wire Wire Line
	4750 3900 4750 3800
Wire Wire Line
	4750 3800 5100 3800
Wire Wire Line
	5100 3900 5100 3800
$Comp
L power:GND #PWR0146
U 1 1 5BB85039
P 4750 4100
F 0 "#PWR0146" H 4750 3850 50  0001 C CNN
F 1 "GND" H 4755 3927 50  0000 C CNN
F 2 "" H 4750 4100 50  0001 C CNN
F 3 "" H 4750 4100 50  0001 C CNN
	1    4750 4100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0147
U 1 1 5BB85052
P 5100 4100
F 0 "#PWR0147" H 5100 3850 50  0001 C CNN
F 1 "GND" H 5105 3927 50  0000 C CNN
F 2 "" H 5100 4100 50  0001 C CNN
F 3 "" H 5100 4100 50  0001 C CNN
	1    5100 4100
	1    0    0    -1  
$EndComp
Wire Wire Line
	5550 4200 5550 4500
Wire Wire Line
	6100 4100 6100 3800
Wire Wire Line
	6100 3800 5850 3800
Connection ~ 4750 3800
Wire Wire Line
	4750 3700 4750 3800
Text Notes 4500 2250 0    50   ~ 0
La Rsense fue calculada para una corriente máxima de 1A.\nLa caída de tensión recomendada es de 50 mV a 100 mV.
$Comp
L AXTEC_IC:+3V3_Internal #PWR8
U 1 1 5BD1966D
P 4750 3700
F 0 "#PWR8" H 4750 4100 50  0001 C CNN
F 1 "+3V3_Internal" H 4735 3875 50  0000 C CNN
F 2 "" H 4750 3700 50  0001 C CNN
F 3 "" H 4750 3700 50  0001 C CNN
	1    4750 3700
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 5C69BE91
P 5300 2800
AR Path="/5BC13C37/5C69BE91" Ref="R?"  Part="1" 
AR Path="/5BC14C3C/5C69BE91" Ref="R?"  Part="1" 
AR Path="/5BC24746/5C69BE91" Ref="R11"  Part="1" 
F 0 "R11" H 5230 2709 50  0000 R CNN
F 1 "51R" H 5230 2800 50  0000 R CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 5230 2800 50  0001 C CNN
F 3 "~" H 5300 2800 50  0001 C CNN
F 4 "1%" H 5230 2891 50  0000 R CNN "Tolerance"
F 5 "AC0603FR-0751RL" V 5300 2800 50  0001 C CNN "Manufacturer Part Number"
	1    5300 2800
	-1   0    0    1   
$EndComp
$Comp
L Device:R R?
U 1 1 5C69BE9A
P 5800 2800
AR Path="/5BC13C37/5C69BE9A" Ref="R?"  Part="1" 
AR Path="/5BC14C3C/5C69BE9A" Ref="R?"  Part="1" 
AR Path="/5BC24746/5C69BE9A" Ref="R13"  Part="1" 
F 0 "R13" H 5730 2709 50  0000 R CNN
F 1 "51R" H 5730 2800 50  0000 R CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 5730 2800 50  0001 C CNN
F 3 "~" H 5800 2800 50  0001 C CNN
F 4 "1%" H 5730 2891 50  0000 R CNN "Tolerance"
F 5 "AC0603FR-0751RL" V 5800 2800 50  0001 C CNN "Manufacturer Part Number"
	1    5800 2800
	-1   0    0    1   
$EndComp
$Comp
L Device:C_Small C?
U 1 1 5C69DBEA
P 5550 3100
AR Path="/5BB122CA/5C69DBEA" Ref="C?"  Part="1" 
AR Path="/5BB12371/5C69DBEA" Ref="C?"  Part="1" 
AR Path="/5BB123AC/5C69DBEA" Ref="C?"  Part="1" 
AR Path="/5BB6C3A1/5C69DBEA" Ref="C?"  Part="1" 
AR Path="/5BC13C37/5C69DBEA" Ref="C?"  Part="1" 
AR Path="/5BC14C3C/5C69DBEA" Ref="C?"  Part="1" 
AR Path="/5BC24746/5C69DBEA" Ref="C24"  Part="1" 
F 0 "C24" H 5458 3054 50  0000 R CNN
F 1 "1uF" H 5458 3145 50  0000 R CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 5550 3100 50  0001 C CNN
F 3 "~" H 5550 3100 50  0001 C CNN
F 4 "10V" H 5550 3100 50  0001 C CNN "Voltage"
F 5 "LMK107B7105MA-T" H 5550 3100 50  0001 C CNN "Manufacturer Part Number"
	1    5550 3100
	0    1    -1   0   
$EndComp
$Comp
L Device:C_Small C?
U 1 1 5C69DC90
P 5100 4000
AR Path="/5BB122CA/5C69DC90" Ref="C?"  Part="1" 
AR Path="/5BB12371/5C69DC90" Ref="C?"  Part="1" 
AR Path="/5BB123AC/5C69DC90" Ref="C?"  Part="1" 
AR Path="/5BB6C3A1/5C69DC90" Ref="C?"  Part="1" 
AR Path="/5BC13C37/5C69DC90" Ref="C?"  Part="1" 
AR Path="/5BC14C3C/5C69DC90" Ref="C?"  Part="1" 
AR Path="/5BC24746/5C69DC90" Ref="C23"  Part="1" 
F 0 "C23" H 5008 3954 50  0000 R CNN
F 1 "1uF" H 5008 4045 50  0000 R CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 5100 4000 50  0001 C CNN
F 3 "~" H 5100 4000 50  0001 C CNN
F 4 "10V" H 5100 4000 50  0001 C CNN "Voltage"
F 5 "LMK107B7105MA-T" H 5100 4000 50  0001 C CNN "Manufacturer Part Number"
	1    5100 4000
	-1   0    0    1   
$EndComp
$Comp
L Device:C_Small C?
U 1 1 5C69DD49
P 4750 4000
AR Path="/5BB122CA/5C69DD49" Ref="C?"  Part="1" 
AR Path="/5BB12371/5C69DD49" Ref="C?"  Part="1" 
AR Path="/5BB123AC/5C69DD49" Ref="C?"  Part="1" 
AR Path="/5BB6C3A1/5C69DD49" Ref="C?"  Part="1" 
AR Path="/5BB6C83B/5C69DD49" Ref="C?"  Part="1" 
AR Path="/5BC14C3C/5C69DD49" Ref="C?"  Part="1" 
AR Path="/5C69DD49" Ref="C?"  Part="1" 
AR Path="/5BC13C37/5C69DD49" Ref="C?"  Part="1" 
AR Path="/5BC24746/5C69DD49" Ref="C22"  Part="1" 
F 0 "C22" H 4658 3954 50  0000 R CNN
F 1 "100nF" H 4658 4045 50  0000 R CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 4750 4000 50  0001 C CNN
F 3 "~" H 4750 4000 50  0001 C CNN
F 4 "10V" H 4750 4000 50  0001 C CNN "Voltage"
F 5 "EMK107B7104KAHT" H 4750 4000 50  0001 C CNN "Manufacturer Part Number"
	1    4750 4000
	1    0    0    1   
$EndComp
Connection ~ 5100 3800
Wire Wire Line
	5100 3800 5250 3800
$EndSCHEMATC
