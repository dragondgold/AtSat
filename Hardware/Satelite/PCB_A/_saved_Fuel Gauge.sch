EESchema Schematic File Version 4
LIBS:PCB_A-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 5 5
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
L Battery_Management:BQ27441DRZR-G1A U?
U 1 1 5BB27F17
P 5900 3700
F 0 "U?" H 5900 4367 50  0000 C CNN
F 1 "BQ27441DRZR-G1A" H 5900 4276 50  0000 C CNN
F 2 "Package_SON:Texas_S-PDSO-N12" H 6150 3150 50  0001 L CNN
F 3 "http://www.ti.com/lit/ds/symlink/bq27441-g1.pdf" H 6100 3900 50  0001 C CNN
	1    5900 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	5900 4350 5900 4300
$Comp
L power:GND #PWR?
U 1 1 5BB28170
P 5900 4350
AR Path="/5BA1A7F9/5BB28170" Ref="#PWR?"  Part="1" 
AR Path="/5BA1A77A/5BB28170" Ref="#PWR?"  Part="1" 
AR Path="/5BB1631B/5BB28170" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 5900 4100 50  0001 C CNN
F 1 "GND" H 5905 4177 50  0000 C CNN
F 2 "" H 5900 4350 50  0001 C CNN
F 3 "" H 5900 4350 50  0001 C CNN
	1    5900 4350
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR?
U 1 1 5BB29A49
P 7050 2600
F 0 "#PWR?" H 7050 2450 50  0001 C CNN
F 1 "+3V3" H 7065 2773 50  0000 C CNN
F 2 "" H 7050 2600 50  0001 C CNN
F 3 "" H 7050 2600 50  0001 C CNN
	1    7050 2600
	1    0    0    -1  
$EndComp
$EndSCHEMATC
