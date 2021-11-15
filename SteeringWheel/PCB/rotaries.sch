EESchema Schematic File Version 4
EELAYER 30 0
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
L Device:R_Small 1k?
U 1 1 619B6452
P 3300 1800
AR Path="/619B6452" Ref="1k?"  Part="1" 
AR Path="/619ACF8C/619B6452" Ref="1k1"  Part="1" 
F 0 "1k1" V 3496 1800 50  0001 C CNN
F 1 "1k" V 3404 1800 50  0000 C CNN
F 2 "" H 3300 1800 50  0001 C CNN
F 3 "~" H 3300 1800 50  0001 C CNN
	1    3300 1800
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3100 1800 3200 1800
Wire Wire Line
	3400 1850 3100 1900
Wire Wire Line
	3350 1750 3350 1800
$Comp
L Device:R_Small 1k?
U 1 1 619B645B
P 3300 1900
AR Path="/619B645B" Ref="1k?"  Part="1" 
AR Path="/619ACF8C/619B645B" Ref="1k2"  Part="1" 
F 0 "1k2" V 3496 1900 50  0001 C CNN
F 1 "1k" V 3404 1900 50  0000 C CNN
F 2 "" H 3300 1900 50  0001 C CNN
F 3 "~" H 3300 1900 50  0001 C CNN
	1    3300 1900
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3200 1900 3100 1900
Wire Wire Line
	3350 1850 3350 1900
Wire Wire Line
	3400 1950 3100 2000
$Comp
L Device:R_Small 1k?
U 1 1 619B6464
P 3300 2000
AR Path="/619B6464" Ref="1k?"  Part="1" 
AR Path="/619ACF8C/619B6464" Ref="1k3"  Part="1" 
F 0 "1k3" V 3496 2000 50  0001 C CNN
F 1 "1k" V 3404 2000 50  0000 C CNN
F 2 "" H 3300 2000 50  0001 C CNN
F 3 "~" H 3300 2000 50  0001 C CNN
	1    3300 2000
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3100 2000 3200 2000
Wire Wire Line
	3350 1950 3350 2000
Wire Wire Line
	3400 2050 3100 2100
$Comp
L Device:R_Small 1k?
U 1 1 619B646D
P 3300 2100
AR Path="/619B646D" Ref="1k?"  Part="1" 
AR Path="/619ACF8C/619B646D" Ref="1k4"  Part="1" 
F 0 "1k4" V 3496 2100 50  0001 C CNN
F 1 "1k" V 3404 2100 50  0000 C CNN
F 2 "" H 3300 2100 50  0001 C CNN
F 3 "~" H 3300 2100 50  0001 C CNN
	1    3300 2100
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3100 2100 3200 2100
Wire Wire Line
	3350 2050 3350 2100
Wire Wire Line
	3400 2150 3100 2200
$Comp
L Device:R_Small 1k?
U 1 1 619B6476
P 3300 2200
AR Path="/619B6476" Ref="1k?"  Part="1" 
AR Path="/619ACF8C/619B6476" Ref="1k5"  Part="1" 
F 0 "1k5" V 3496 2200 50  0001 C CNN
F 1 "1k" V 3404 2200 50  0000 C CNN
F 2 "" H 3300 2200 50  0001 C CNN
F 3 "~" H 3300 2200 50  0001 C CNN
	1    3300 2200
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3200 2200 3100 2200
Wire Wire Line
	3350 2150 3350 2200
Wire Wire Line
	3100 2300 3400 2250
$Comp
L Device:R_Small 1k?
U 1 1 619B647F
P 3300 2300
AR Path="/619B647F" Ref="1k?"  Part="1" 
AR Path="/619ACF8C/619B647F" Ref="1k6"  Part="1" 
F 0 "1k6" V 3496 2300 50  0001 C CNN
F 1 "1k" V 3404 2300 50  0000 C CNN
F 2 "" H 3300 2300 50  0001 C CNN
F 3 "~" H 3300 2300 50  0001 C CNN
	1    3300 2300
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3100 2300 3200 2300
$Comp
L power:GND #PWR?
U 1 1 619B6486
P 3100 1800
AR Path="/619B6486" Ref="#PWR?"  Part="1" 
AR Path="/619ACF8C/619B6486" Ref="#PWR0116"  Part="1" 
F 0 "#PWR0116" H 3100 1550 50  0001 C CNN
F 1 "GND" H 3105 1627 50  0000 C CNN
F 2 "" H 3100 1800 50  0001 C CNN
F 3 "" H 3100 1800 50  0001 C CNN
	1    3100 1800
	-1   0    0    1   
$EndComp
Wire Wire Line
	3100 2400 3400 2400
Wire Wire Line
	3350 2250 3350 2350
$Comp
L power:+3.3V #PWR?
U 1 1 619B648E
P 3100 2400
AR Path="/619B648E" Ref="#PWR?"  Part="1" 
AR Path="/619ACF8C/619B648E" Ref="#PWR0117"  Part="1" 
F 0 "#PWR0117" H 3100 2250 50  0001 C CNN
F 1 "+3.3V" H 3115 2573 50  0000 C CNN
F 2 "" H 3100 2400 50  0001 C CNN
F 3 "" H 3100 2400 50  0001 C CNN
	1    3100 2400
	-1   0    0    1   
$EndComp
$Comp
L Device:R_Small 1k?
U 1 1 619B6494
P 5250 1800
AR Path="/619B6494" Ref="1k?"  Part="1" 
AR Path="/619ACF8C/619B6494" Ref="1k7"  Part="1" 
F 0 "1k7" V 5446 1800 50  0001 C CNN
F 1 "1k" V 5354 1800 50  0000 C CNN
F 2 "" H 5250 1800 50  0001 C CNN
F 3 "~" H 5250 1800 50  0001 C CNN
	1    5250 1800
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5050 1800 5150 1800
Wire Wire Line
	5350 1850 5050 1900
Wire Wire Line
	5350 1800 5350 1850
$Comp
L Device:R_Small 1k?
U 1 1 619B649D
P 5250 1900
AR Path="/619B649D" Ref="1k?"  Part="1" 
AR Path="/619ACF8C/619B649D" Ref="1k8"  Part="1" 
F 0 "1k8" V 5446 1900 50  0001 C CNN
F 1 "1k" V 5354 1900 50  0000 C CNN
F 2 "" H 5250 1900 50  0001 C CNN
F 3 "~" H 5250 1900 50  0001 C CNN
	1    5250 1900
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5150 1900 5050 1900
Connection ~ 5050 1900
Wire Wire Line
	5350 1900 5350 1950
Wire Wire Line
	5350 1950 5050 2000
$Comp
L Device:R_Small 1k?
U 1 1 619B64A7
P 5250 2000
AR Path="/619B64A7" Ref="1k?"  Part="1" 
AR Path="/619ACF8C/619B64A7" Ref="1k9"  Part="1" 
F 0 "1k9" V 5446 2000 50  0001 C CNN
F 1 "1k" V 5354 2000 50  0000 C CNN
F 2 "" H 5250 2000 50  0001 C CNN
F 3 "~" H 5250 2000 50  0001 C CNN
	1    5250 2000
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5050 2000 5150 2000
Connection ~ 5050 2000
Wire Wire Line
	5350 2000 5350 2050
Wire Wire Line
	5350 2050 5050 2100
$Comp
L Device:R_Small 1k?
U 1 1 619B64B1
P 5250 2100
AR Path="/619B64B1" Ref="1k?"  Part="1" 
AR Path="/619ACF8C/619B64B1" Ref="1k10"  Part="1" 
F 0 "1k10" V 5446 2100 50  0001 C CNN
F 1 "1k" V 5354 2100 50  0000 C CNN
F 2 "" H 5250 2100 50  0001 C CNN
F 3 "~" H 5250 2100 50  0001 C CNN
	1    5250 2100
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5050 2100 5150 2100
Connection ~ 5050 2100
$Comp
L Switch:SW_Rotary10 Uni_RotarySwitch?
U 1 1 619B64B9
P 4650 2400
AR Path="/619B64B9" Ref="Uni_RotarySwitch?"  Part="1" 
AR Path="/619ACF8C/619B64B9" Ref="Uni_RotarySwitch1"  Part="1" 
F 0 "Uni_RotarySwitch1" H 4550 3181 50  0000 C CNN
F 1 "SW_Rotary10" H 4550 3090 50  0000 C CNN
F 2 "" H 4450 3100 50  0001 C CNN
F 3 "http://cdn-reichelt.de/documents/datenblatt/C200/DS-Serie%23LOR.pdf" H 4450 3100 50  0001 C CNN
	1    4650 2400
	1    0    0    -1  
$EndComp
Wire Wire Line
	5350 2100 5350 2150
Wire Wire Line
	5350 2150 5050 2200
$Comp
L Device:R_Small 1k?
U 1 1 619B64C1
P 5250 2200
AR Path="/619B64C1" Ref="1k?"  Part="1" 
AR Path="/619ACF8C/619B64C1" Ref="1k11"  Part="1" 
F 0 "1k11" V 5446 2200 50  0001 C CNN
F 1 "1k" V 5354 2200 50  0000 C CNN
F 2 "" H 5250 2200 50  0001 C CNN
F 3 "~" H 5250 2200 50  0001 C CNN
	1    5250 2200
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5150 2200 5050 2200
Connection ~ 5050 2200
Wire Wire Line
	5350 2200 5350 2250
Wire Wire Line
	5050 2300 5350 2250
$Comp
L Device:R_Small 1k?
U 1 1 619B64CB
P 5250 2300
AR Path="/619B64CB" Ref="1k?"  Part="1" 
AR Path="/619ACF8C/619B64CB" Ref="1k12"  Part="1" 
F 0 "1k12" V 5446 2300 50  0001 C CNN
F 1 "1k" V 5354 2300 50  0000 C CNN
F 2 "" H 5250 2300 50  0001 C CNN
F 3 "~" H 5250 2300 50  0001 C CNN
	1    5250 2300
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5050 2300 5150 2300
Connection ~ 5050 2300
Wire Wire Line
	5350 2300 5350 2350
Wire Wire Line
	5350 2350 5050 2500
$Comp
L Device:R_Small 1k?
U 1 1 619B64D5
P 5250 2500
AR Path="/619B64D5" Ref="1k?"  Part="1" 
AR Path="/619ACF8C/619B64D5" Ref="1k13"  Part="1" 
F 0 "1k13" V 5446 2500 50  0001 C CNN
F 1 "1k" V 5354 2500 50  0000 C CNN
F 2 "" H 5250 2500 50  0001 C CNN
F 3 "~" H 5250 2500 50  0001 C CNN
	1    5250 2500
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5050 2500 5150 2500
Connection ~ 5050 2500
Wire Wire Line
	5050 2600 5350 2550
Wire Wire Line
	5350 2550 5350 2500
$Comp
L Device:R_Small 1k?
U 1 1 619B64DF
P 5250 2600
AR Path="/619B64DF" Ref="1k?"  Part="1" 
AR Path="/619ACF8C/619B64DF" Ref="1k14"  Part="1" 
F 0 "1k14" V 5446 2600 50  0001 C CNN
F 1 "1k" V 5354 2600 50  0000 C CNN
F 2 "" H 5250 2600 50  0001 C CNN
F 3 "~" H 5250 2600 50  0001 C CNN
	1    5250 2600
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5050 2600 5150 2600
Connection ~ 5050 2600
Wire Wire Line
	5350 2600 5350 2650
Wire Wire Line
	5350 2650 5050 2700
$Comp
L Device:R_Small 1k?
U 1 1 619B64E9
P 5250 2700
AR Path="/619B64E9" Ref="1k?"  Part="1" 
AR Path="/619ACF8C/619B64E9" Ref="1k15"  Part="1" 
F 0 "1k15" V 5446 2700 50  0001 C CNN
F 1 "1k" V 5354 2700 50  0000 C CNN
F 2 "" H 5250 2700 50  0001 C CNN
F 3 "~" H 5250 2700 50  0001 C CNN
	1    5250 2700
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5050 2700 5150 2700
Connection ~ 5050 2700
Wire Wire Line
	5350 2700 5350 2750
Wire Wire Line
	5350 2750 5050 2800
$Comp
L power:GND #PWR?
U 1 1 619B64F3
P 5050 1800
AR Path="/619B64F3" Ref="#PWR?"  Part="1" 
AR Path="/619ACF8C/619B64F3" Ref="#PWR0118"  Part="1" 
F 0 "#PWR0118" H 5050 1550 50  0001 C CNN
F 1 "GND" H 5055 1627 50  0000 C CNN
F 2 "" H 5050 1800 50  0001 C CNN
F 3 "" H 5050 1800 50  0001 C CNN
	1    5050 1800
	-1   0    0    1   
$EndComp
Connection ~ 5050 1800
$Comp
L power:+3.3V #PWR?
U 1 1 619B64FA
P 5050 2800
AR Path="/619B64FA" Ref="#PWR?"  Part="1" 
AR Path="/619ACF8C/619B64FA" Ref="#PWR0119"  Part="1" 
F 0 "#PWR0119" H 5050 2650 50  0001 C CNN
F 1 "+3.3V" H 5065 2973 50  0000 C CNN
F 2 "" H 5050 2800 50  0001 C CNN
F 3 "" H 5050 2800 50  0001 C CNN
	1    5050 2800
	-1   0    0    1   
$EndComp
Connection ~ 5050 2800
Connection ~ 3100 2400
Connection ~ 3100 2300
Connection ~ 3100 2200
Connection ~ 3100 2100
Connection ~ 3100 2000
Connection ~ 3100 1900
$Comp
L Switch:SW_Rotary6 ScreenChg_RotarySwitch?
U 1 1 619B6507
P 2700 2500
AR Path="/619B6507" Ref="ScreenChg_RotarySwitch?"  Part="1" 
AR Path="/619ACF8C/619B6507" Ref="ScreenChg_RotarySwitch1"  Part="1" 
F 0 "ScreenChg_RotarySwitch1" H 2600 3281 50  0000 C CNN
F 1 "SW_Rotary6" H 2600 3190 50  0000 C CNN
F 2 "" H 2500 3200 50  0001 C CNN
F 3 "http://cdn-reichelt.de/documents/datenblatt/C200/DS-Serie%23LOR.pdf" H 2500 3200 50  0001 C CNN
	1    2700 2500
	1    0    0    -1  
$EndComp
Text HLabel 2100 2500 0    50   Input ~ 0
ROT_SCR
Text HLabel 4050 2400 0    50   Input ~ 0
ROT_UNI
$EndSCHEMATC
