EESchema Schematic File Version 4
LIBS:PCB_B-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 6 6
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
L Device:C_Small C?
U 1 1 5BBB58E1
P 3100 4350
AR Path="/5BB122CA/5BBB58E1" Ref="C?"  Part="1" 
AR Path="/5BB12371/5BBB58E1" Ref="C?"  Part="1" 
AR Path="/5BB123AC/5BBB58E1" Ref="C?"  Part="1" 
AR Path="/5BB6C3A1/5BBB58E1" Ref="C?"  Part="1" 
AR Path="/5BB6C83B/5BBB58E1" Ref="C?"  Part="1" 
F 0 "C?" H 3008 4304 50  0000 R CNN
F 1 "10uF" H 3008 4395 50  0000 R CNN
F 2 "" H 3100 4350 50  0001 C CNN
F 3 "~" H 3100 4350 50  0001 C CNN
F 4 "10V" H 3100 4350 50  0001 C CNN "Voltage"
	1    3100 4350
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5BBB58EA
P 3100 4600
AR Path="/5BB6C3A1/5BBB58EA" Ref="#PWR?"  Part="1" 
AR Path="/5BB6C83B/5BBB58EA" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 3100 4350 50  0001 C CNN
F 1 "GND" H 3105 4427 50  0000 C CNN
F 2 "" H 3100 4600 50  0001 C CNN
F 3 "" H 3100 4600 50  0001 C CNN
	1    3100 4600
	1    0    0    -1  
$EndComp
Wire Wire Line
	3100 4450 3100 4600
$Comp
L Device:C_Small C?
U 1 1 5BBB58F9
P 6750 4600
AR Path="/5BB122CA/5BBB58F9" Ref="C?"  Part="1" 
AR Path="/5BB12371/5BBB58F9" Ref="C?"  Part="1" 
AR Path="/5BB123AC/5BBB58F9" Ref="C?"  Part="1" 
AR Path="/5BB6C3A1/5BBB58F9" Ref="C?"  Part="1" 
AR Path="/5BB6C83B/5BBB58F9" Ref="C?"  Part="1" 
F 0 "C?" H 6658 4554 50  0000 R CNN
F 1 "10uF" H 6658 4645 50  0000 R CNN
F 2 "" H 6750 4600 50  0001 C CNN
F 3 "~" H 6750 4600 50  0001 C CNN
F 4 "10V" H 6750 4600 50  0001 C CNN "Voltage"
	1    6750 4600
	-1   0    0    1   
$EndComp
$Comp
L Device:L_Core_Iron L?
U 1 1 5BBB5901
P 3500 4250
AR Path="/5BB6C3A1/5BBB5901" Ref="L?"  Part="1" 
AR Path="/5BB6C83B/5BBB5901" Ref="L?"  Part="1" 
F 0 "L?" H 3459 4204 50  0000 R CNN
F 1 "4.7uH" H 3459 4295 50  0000 R CNN
F 2 "~" H 3500 4250 50  0001 C CNN
F 3 "~" H 3500 4250 50  0001 C CNN
F 4 "SRN3010TA-4R7M" V 3500 4250 50  0001 C CNN "Manufacturer Part Number"
	1    3500 4250
	-1   0    0    1   
$EndComp
Wire Wire Line
	6750 4400 6750 4500
$Comp
L power:GND #PWR?
U 1 1 5BBB590A
P 6750 4900
AR Path="/5BB6C3A1/5BBB590A" Ref="#PWR?"  Part="1" 
AR Path="/5BB6C83B/5BBB590A" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 6750 4650 50  0001 C CNN
F 1 "GND" H 6755 4727 50  0000 C CNN
F 2 "" H 6750 4900 50  0001 C CNN
F 3 "" H 6750 4900 50  0001 C CNN
	1    6750 4900
	1    0    0    -1  
$EndComp
Wire Wire Line
	6750 4700 6750 4900
Text HLabel 2950 4100 0    50   Input ~ 0
VBAT
Text HLabel 7000 4400 2    50   Output ~ 0
5V
Wire Wire Line
	3100 4100 2950 4100
Connection ~ 3100 4100
Text HLabel 5400 4100 2    50   Input ~ 0
CTRL
Wire Wire Line
	6500 4600 6500 4400
Connection ~ 6500 4400
Wire Wire Line
	6500 4400 6750 4400
Wire Wire Line
	7000 4400 6750 4400
Connection ~ 6750 4400
$Comp
L Amplifier_Current:INA194 U?
U 1 1 5BBB591E
P 6250 2600
AR Path="/5BB6C3A1/5BBB591E" Ref="U?"  Part="1" 
AR Path="/5BB6C83B/5BBB591E" Ref="U?"  Part="1" 
F 0 "U?" V 6296 2259 50  0000 R CNN
F 1 "INA194" V 6205 2259 50  0000 R CNN
F 2 "Package_TO_SOT_SMD:SOT-23-5" H 6250 2600 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/ina193.pdf" H 6250 2600 50  0001 C CNN
	1    6250 2600
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R?
U 1 1 5BBB5927
P 6250 4400
AR Path="/5BB6C3A1/5BBB5927" Ref="R?"  Part="1" 
AR Path="/5BB6C83B/5BBB5927" Ref="R?"  Part="1" 
F 0 "R?" V 5953 4400 50  0000 C CNN
F 1 "200m" V 6044 4400 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 6180 4400 50  0001 C CNN
F 3 "~" H 6250 4400 50  0001 C CNN
F 4 "1%" V 6135 4400 50  0000 C CNN "Tolerance"
F 5 "KRL1632E-C-R200-F-T5" V 6250 4400 50  0001 C CNN "Manufacturer Part Number"
	1    6250 4400
	0    1    1    0   
$EndComp
Wire Wire Line
	6400 4400 6500 4400
$Comp
L Device:R R?
U 1 1 5BBB5931
P 6000 3800
AR Path="/5BB6C3A1/5BBB5931" Ref="R?"  Part="1" 
AR Path="/5BB6C83B/5BBB5931" Ref="R?"  Part="1" 
F 0 "R?" H 5930 3709 50  0000 R CNN
F 1 "50R" H 5930 3800 50  0000 R CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 5930 3800 50  0001 C CNN
F 3 "~" H 6000 3800 50  0001 C CNN
F 4 "1%" H 5930 3891 50  0000 R CNN "Tolerance"
F 5 "KRL1632E-M-R120-F-T5" V 6000 3800 50  0001 C CNN "Manufacturer Part Number"
	1    6000 3800
	-1   0    0    1   
$EndComp
$Comp
L Device:R R?
U 1 1 5BBB593A
P 6500 3800
AR Path="/5BB6C3A1/5BBB593A" Ref="R?"  Part="1" 
AR Path="/5BB6C83B/5BBB593A" Ref="R?"  Part="1" 
F 0 "R?" H 6430 3709 50  0000 R CNN
F 1 "50R" H 6430 3800 50  0000 R CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 6430 3800 50  0001 C CNN
F 3 "~" H 6500 3800 50  0001 C CNN
F 4 "1%" H 6430 3891 50  0000 R CNN "Tolerance"
F 5 "KRL1632E-M-R120-F-T5" V 6500 3800 50  0001 C CNN "Manufacturer Part Number"
	1    6500 3800
	-1   0    0    1   
$EndComp
Wire Wire Line
	6000 3950 6000 4400
Connection ~ 6000 4400
Wire Wire Line
	6000 4400 6100 4400
Wire Wire Line
	6500 3950 6500 4400
$Comp
L Device:C_Small C?
U 1 1 5BBB5946
P 6250 3300
AR Path="/5BB122CA/5BBB5946" Ref="C?"  Part="1" 
AR Path="/5BB12371/5BBB5946" Ref="C?"  Part="1" 
AR Path="/5BB123AC/5BBB5946" Ref="C?"  Part="1" 
AR Path="/5BB6C3A1/5BBB5946" Ref="C?"  Part="1" 
AR Path="/5BB6C83B/5BBB5946" Ref="C?"  Part="1" 
F 0 "C?" H 6158 3254 50  0000 R CNN
F 1 "1uF" H 6158 3345 50  0000 R CNN
F 2 "" H 6250 3300 50  0001 C CNN
F 3 "~" H 6250 3300 50  0001 C CNN
F 4 "10V" H 6250 3300 50  0001 C CNN "Voltage"
	1    6250 3300
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6350 3300 6500 3300
Wire Wire Line
	6500 3300 6500 3650
Wire Wire Line
	6150 3300 6000 3300
Wire Wire Line
	6000 3300 6000 3650
Wire Wire Line
	6000 3300 6000 3000
Wire Wire Line
	6000 3000 6150 3000
Wire Wire Line
	6150 3000 6150 2900
Connection ~ 6000 3300
Wire Wire Line
	6350 2900 6350 3000
Wire Wire Line
	6350 3000 6500 3000
Wire Wire Line
	6500 3000 6500 3300
Connection ~ 6500 3300
$Comp
L AXTEC_IC:+3V3_Internal #PWR?
U 1 1 5BBB5959
P 5100 2600
AR Path="/5BB6C3A1/5BBB5959" Ref="#PWR?"  Part="1" 
AR Path="/5BB6C83B/5BBB5959" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 5100 3000 50  0001 C CNN
F 1 "+3V3_Internal" H 5085 2773 50  0000 C CNN
F 2 "" H 5100 2600 50  0001 C CNN
F 3 "" H 5100 2600 50  0001 C CNN
	1    5100 2600
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C?
U 1 1 5BBB5960
P 5600 2900
AR Path="/5BB122CA/5BBB5960" Ref="C?"  Part="1" 
AR Path="/5BB12371/5BBB5960" Ref="C?"  Part="1" 
AR Path="/5BB123AC/5BBB5960" Ref="C?"  Part="1" 
AR Path="/5BB6C3A1/5BBB5960" Ref="C?"  Part="1" 
AR Path="/5BB6C83B/5BBB5960" Ref="C?"  Part="1" 
F 0 "C?" H 5508 2854 50  0000 R CNN
F 1 "1uF" H 5508 2945 50  0000 R CNN
F 2 "" H 5600 2900 50  0001 C CNN
F 3 "~" H 5600 2900 50  0001 C CNN
F 4 "10V" H 5600 2900 50  0001 C CNN "Voltage"
	1    5600 2900
	-1   0    0    1   
$EndComp
$Comp
L Device:C_Small C?
U 1 1 5BBB5968
P 5250 2900
AR Path="/5BB122CA/5BBB5968" Ref="C?"  Part="1" 
AR Path="/5BB12371/5BBB5968" Ref="C?"  Part="1" 
AR Path="/5BB123AC/5BBB5968" Ref="C?"  Part="1" 
AR Path="/5BB6C3A1/5BBB5968" Ref="C?"  Part="1" 
AR Path="/5BB6C83B/5BBB5968" Ref="C?"  Part="1" 
F 0 "C?" H 5158 2854 50  0000 R CNN
F 1 "100n" H 5158 2945 50  0000 R CNN
F 2 "" H 5250 2900 50  0001 C CNN
F 3 "~" H 5250 2900 50  0001 C CNN
F 4 "10V" H 5250 2900 50  0001 C CNN "Voltage"
	1    5250 2900
	-1   0    0    1   
$EndComp
Wire Wire Line
	5100 2600 5100 2700
Wire Wire Line
	5100 2700 5250 2700
Wire Wire Line
	5600 2800 5600 2700
Connection ~ 5600 2700
Wire Wire Line
	5600 2700 5950 2700
Wire Wire Line
	5250 2800 5250 2700
Connection ~ 5250 2700
Wire Wire Line
	5250 2700 5600 2700
$Comp
L power:GND #PWR?
U 1 1 5BBB5977
P 5250 3000
AR Path="/5BB6C3A1/5BBB5977" Ref="#PWR?"  Part="1" 
AR Path="/5BB6C83B/5BBB5977" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 5250 2750 50  0001 C CNN
F 1 "GND" H 5255 2827 50  0000 C CNN
F 2 "" H 5250 3000 50  0001 C CNN
F 3 "" H 5250 3000 50  0001 C CNN
	1    5250 3000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5BBB597D
P 5600 3000
AR Path="/5BB6C3A1/5BBB597D" Ref="#PWR?"  Part="1" 
AR Path="/5BB6C83B/5BBB597D" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 5600 2750 50  0001 C CNN
F 1 "GND" H 5605 2827 50  0000 C CNN
F 2 "" H 5600 3000 50  0001 C CNN
F 3 "" H 5600 3000 50  0001 C CNN
	1    5600 3000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5BBB5983
P 6800 2950
AR Path="/5BB6C3A1/5BBB5983" Ref="#PWR?"  Part="1" 
AR Path="/5BB6C83B/5BBB5983" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 6800 2700 50  0001 C CNN
F 1 "GND" H 6805 2777 50  0000 C CNN
F 2 "" H 6800 2950 50  0001 C CNN
F 3 "" H 6800 2950 50  0001 C CNN
	1    6800 2950
	1    0    0    -1  
$EndComp
Wire Wire Line
	6800 2950 6800 2700
Wire Wire Line
	6800 2700 6550 2700
Text HLabel 6500 2050 2    50   Output ~ 0
ISENSE
Wire Wire Line
	6500 2050 6250 2050
Wire Wire Line
	6250 2050 6250 2300
$Comp
L SamacSys_Parts:MCP1642B-50I_MS IC?
U 1 1 5BBB622D
P 5150 4100
F 0 "IC?" H 5700 4365 50  0000 C CNN
F 1 "MCP1642B-50I_MS" H 5700 4274 50  0000 C CNN
F 2 "SOP65P490X110-8N" H 6100 4200 50  0001 L CNN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/20005253A.pdf" H 6100 4100 50  0001 L CNN
F 4 "Switching Voltage Regulators Step-up DC/DC Reg 1 MHz, 1.6A, 5Vout" H 6100 4000 50  0001 L CNN "Description"
F 5 "1.1" H 6100 3900 50  0001 L CNN "Height"
F 6 "Microchip" H 6100 3800 50  0001 L CNN "Manufacturer_Name"
F 7 "MCP1642B-50I/MS" H 6100 3700 50  0001 L CNN "Manufacturer_Part_Number"
F 8 "" H 6100 3600 50  0001 L CNN "RS Part Number"
F 9 "" H 6100 3500 50  0001 L CNN "RS Price/Stock"
F 10 "70567327" H 6100 3400 50  0001 L CNN "Allied_Number"
F 11 "https://www.alliedelec.com/microchip-technology-inc-mcp1642b-50i-ms/70567327/" H 6100 3300 50  0001 L CNN "Allied Price/Stock"
F 12 "MCP1642B-50I/MS" H 6100 3200 50  0001 L CNN "Arrow Part Number"
F 13 "https://www.arrow.com/en/products/mcp1642b-50ims/microchip-technology" H 6100 3100 50  0001 L CNN "Arrow Price/Stock"
	1    5150 4100
	-1   0    0    -1  
$EndComp
Wire Wire Line
	3100 4100 3500 4100
Wire Wire Line
	3100 4100 3100 4250
$Comp
L power:GND #PWR?
U 1 1 5BBB7A9F
P 3850 4250
AR Path="/5BB6C3A1/5BBB7A9F" Ref="#PWR?"  Part="1" 
AR Path="/5BB6C83B/5BBB7A9F" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 3850 4000 50  0001 C CNN
F 1 "GND" H 3855 4077 50  0000 C CNN
F 2 "" H 3850 4250 50  0001 C CNN
F 3 "" H 3850 4250 50  0001 C CNN
	1    3850 4250
	1    0    0    -1  
$EndComp
Wire Wire Line
	4050 4200 4000 4200
Wire Wire Line
	3850 4200 3850 4250
Wire Wire Line
	4050 4300 4000 4300
Wire Wire Line
	4000 4300 4000 4200
Connection ~ 4000 4200
Wire Wire Line
	4000 4200 3850 4200
Wire Wire Line
	5400 4100 5150 4100
Wire Wire Line
	4050 4400 4000 4400
Wire Wire Line
	4000 4400 4000 4550
Wire Wire Line
	4000 4550 3500 4550
Wire Wire Line
	3500 4550 3500 4400
Wire Wire Line
	5150 4400 6000 4400
Connection ~ 3500 4100
Wire Wire Line
	3500 4100 4050 4100
$EndSCHEMATC
