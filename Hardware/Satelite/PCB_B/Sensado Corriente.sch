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
OUT
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
AR Path="/5BC24746/5BB84510" Ref="R?"  Part="1" 
F 0 "R?" V 5650 2500 50  0000 L CNN
F 1 "51m" V 5450 2500 50  0000 L CNN
F 2 "" V 5480 2500 50  0001 C CNN
F 3 "~" H 5550 2500 50  0001 C CNN
F 4 "1%" V 5550 2500 50  0000 C CNN "Tolerance"
F 5 "KRL1632E-C-R051-F-T1" V 5550 2500 50  0001 C CNN "Manufacturer Part Number"
	1    5550 2500
	0    1    1    0   
$EndComp
$Comp
L Amplifier_Current:INA194 U?
U 1 1 5BB84611
P 5550 3900
F 0 "U?" V 5650 3750 50  0000 R CNN
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
$Comp
L Device:R R?
U 1 1 5BB846FE
P 5300 2800
AR Path="/5BA17F9C/5BB846FE" Ref="R?"  Part="1" 
AR Path="/5BA1A7F9/5BB846FE" Ref="R?"  Part="1" 
AR Path="/5BA1A77A/5BB846FE" Ref="R?"  Part="1" 
AR Path="/5BB846FE" Ref="R?"  Part="1" 
AR Path="/5BB7A164/5BB846FE" Ref="R?"  Part="1" 
AR Path="/5BC24746/5BB846FE" Ref="R?"  Part="1" 
F 0 "R?" V 5400 2800 50  0000 L CNN
F 1 "50R" V 5200 2800 50  0000 L CNN
F 2 "" V 5230 2800 50  0001 C CNN
F 3 "~" H 5300 2800 50  0001 C CNN
F 4 "1%" V 5300 2800 50  0000 C CNN "Tolerance"
	1    5300 2800
	-1   0    0    1   
$EndComp
$Comp
L Device:R R?
U 1 1 5BB847E2
P 5800 2800
AR Path="/5BA17F9C/5BB847E2" Ref="R?"  Part="1" 
AR Path="/5BA1A7F9/5BB847E2" Ref="R?"  Part="1" 
AR Path="/5BA1A77A/5BB847E2" Ref="R?"  Part="1" 
AR Path="/5BB847E2" Ref="R?"  Part="1" 
AR Path="/5BB7A164/5BB847E2" Ref="R?"  Part="1" 
AR Path="/5BC24746/5BB847E2" Ref="R?"  Part="1" 
F 0 "R?" V 5900 2800 50  0000 L CNN
F 1 "50R" V 5700 2800 50  0000 L CNN
F 2 "" V 5730 2800 50  0001 C CNN
F 3 "~" H 5800 2800 50  0001 C CNN
F 4 "1%" V 5800 2800 50  0000 C CNN "Tolerance"
	1    5800 2800
	-1   0    0    1   
$EndComp
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
$Comp
L Device:C_Small C?
U 1 1 5BB848CB
P 5550 3100
AR Path="/5BA17F9C/5BB848CB" Ref="C?"  Part="1" 
AR Path="/5BA1A7F9/5BB848CB" Ref="C?"  Part="1" 
AR Path="/5BA1A77A/5BB848CB" Ref="C?"  Part="1" 
AR Path="/5BB848CB" Ref="C?"  Part="1" 
AR Path="/5BB7A164/5BB848CB" Ref="C?"  Part="1" 
AR Path="/5BC24746/5BB848CB" Ref="C?"  Part="1" 
F 0 "C?" H 5700 3100 50  0000 L CNN
F 1 "1uF" H 5650 3000 50  0000 L CNN
F 2 "" H 5550 3100 50  0001 C CNN
F 3 "~" H 5550 3100 50  0001 C CNN
	1    5550 3100
	0    1    1    0   
$EndComp
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
L power:GND #PWR?
U 1 1 5BB84C5F
P 6100 4100
F 0 "#PWR?" H 6100 3850 50  0001 C CNN
F 1 "GND" H 6105 3927 50  0000 C CNN
F 2 "" H 6100 4100 50  0001 C CNN
F 3 "" H 6100 4100 50  0001 C CNN
	1    6100 4100
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C?
U 1 1 5BB84CBB
P 5150 4000
AR Path="/5BA17F9C/5BB84CBB" Ref="C?"  Part="1" 
AR Path="/5BA1A7F9/5BB84CBB" Ref="C?"  Part="1" 
AR Path="/5BA1A77A/5BB84CBB" Ref="C?"  Part="1" 
AR Path="/5BB84CBB" Ref="C?"  Part="1" 
AR Path="/5BB7A164/5BB84CBB" Ref="C?"  Part="1" 
AR Path="/5BC24746/5BB84CBB" Ref="C?"  Part="1" 
F 0 "C?" H 5300 4000 50  0000 L CNN
F 1 "1uF" H 5250 3900 50  0000 L CNN
F 2 "" H 5150 4000 50  0001 C CNN
F 3 "~" H 5150 4000 50  0001 C CNN
	1    5150 4000
	-1   0    0    1   
$EndComp
$Comp
L Device:C_Small C?
U 1 1 5BB84D2D
P 4750 4000
AR Path="/5BA17F9C/5BB84D2D" Ref="C?"  Part="1" 
AR Path="/5BA1A7F9/5BB84D2D" Ref="C?"  Part="1" 
AR Path="/5BA1A77A/5BB84D2D" Ref="C?"  Part="1" 
AR Path="/5BB84D2D" Ref="C?"  Part="1" 
AR Path="/5BB7A164/5BB84D2D" Ref="C?"  Part="1" 
AR Path="/5BC24746/5BB84D2D" Ref="C?"  Part="1" 
F 0 "C?" H 4900 4000 50  0000 L CNN
F 1 "100n" H 4850 3900 50  0000 L CNN
F 2 "" H 4750 4000 50  0001 C CNN
F 3 "~" H 4750 4000 50  0001 C CNN
	1    4750 4000
	-1   0    0    1   
$EndComp
Wire Wire Line
	4750 3900 4750 3800
Wire Wire Line
	4750 3800 5150 3800
Wire Wire Line
	5150 3900 5150 3800
Connection ~ 5150 3800
Wire Wire Line
	5150 3800 5250 3800
$Comp
L power:GND #PWR?
U 1 1 5BB85039
P 4750 4100
F 0 "#PWR?" H 4750 3850 50  0001 C CNN
F 1 "GND" H 4755 3927 50  0000 C CNN
F 2 "" H 4750 4100 50  0001 C CNN
F 3 "" H 4750 4100 50  0001 C CNN
	1    4750 4100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5BB85052
P 5150 4100
F 0 "#PWR?" H 5150 3850 50  0001 C CNN
F 1 "GND" H 5155 3927 50  0000 C CNN
F 2 "" H 5150 4100 50  0001 C CNN
F 3 "" H 5150 4100 50  0001 C CNN
	1    5150 4100
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
L AXTEC_IC:+3V3_Internal #PWR?
U 1 1 5BD1966D
P 4750 3700
F 0 "#PWR?" H 4750 4100 50  0001 C CNN
F 1 "+3V3_Internal" H 4735 3875 50  0000 C CNN
F 2 "" H 4750 3700 50  0001 C CNN
F 3 "" H 4750 3700 50  0001 C CNN
	1    4750 3700
	1    0    0    -1  
$EndComp
$EndSCHEMATC
