EESchema Schematic File Version 4
EELAYER 30 0
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
L power:+12V #PWR?
U 1 1 6193A171
P 2225 2650
AR Path="/6193A171" Ref="#PWR?"  Part="1" 
AR Path="/619320AD/6193A171" Ref="#PWR?"  Part="1" 
AR Path="/61914EC1/6193A171" Ref="#PWR031"  Part="1" 
AR Path="/6191C2CA/6193A171" Ref="#PWR015"  Part="1" 
AR Path="/619D8C50/6193A171" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 2225 2500 50  0001 C CNN
F 1 "+12V" H 2240 2823 50  0000 C CNN
F 2 "" H 2225 2650 50  0001 C CNN
F 3 "" H 2225 2650 50  0001 C CNN
	1    2225 2650
	1    0    0    -1  
$EndComp
Wire Wire Line
	2225 2650 2225 2850
$Comp
L Device:Polyfuse_Small F?
U 1 1 6193A178
P 3400 2850
AR Path="/6193A178" Ref="F?"  Part="1" 
AR Path="/619320AD/6193A178" Ref="F?"  Part="1" 
AR Path="/61914EC1/6193A178" Ref="F1"  Part="1" 
AR Path="/6191C2CA/6193A178" Ref="F1"  Part="1" 
AR Path="/619D8C50/6193A178" Ref="F?"  Part="1" 
F 0 "F?" V 3195 2850 50  0000 C CNN
F 1 "Polyfuse_Small" V 3300 2850 50  0000 C CNN
F 2 "Fuse:Fuse_1206_3216Metric" H 3450 2650 50  0001 L CNN
F 3 "~" H 3400 2850 50  0001 C CNN
	1    3400 2850
	0    1    1    0   
$EndComp
$Comp
L Device:Ferrite_Bead_Small FB?
U 1 1 6193A17E
P 3950 2850
AR Path="/6193A17E" Ref="FB?"  Part="1" 
AR Path="/619320AD/6193A17E" Ref="FB?"  Part="1" 
AR Path="/61914EC1/6193A17E" Ref="FB1"  Part="1" 
AR Path="/6191C2CA/6193A17E" Ref="FB1"  Part="1" 
AR Path="/619D8C50/6193A17E" Ref="FB?"  Part="1" 
F 0 "FB?" V 3713 2850 50  0000 C CNN
F 1 "600 @ 600Mhz" V 3804 2850 50  0000 C CNN
F 2 "Inductor_SMD:L_0805_2012Metric" V 3880 2850 50  0001 C CNN
F 3 "~" H 3950 2850 50  0001 C CNN
	1    3950 2850
	0    1    1    0   
$EndComp
Wire Wire Line
	3500 2850 3850 2850
$Comp
L Device:C_Small C?
U 1 1 6193A185
P 4100 3150
AR Path="/6193A185" Ref="C?"  Part="1" 
AR Path="/619320AD/6193A185" Ref="C?"  Part="1" 
AR Path="/61914EC1/6193A185" Ref="C13"  Part="1" 
AR Path="/6191C2CA/6193A185" Ref="C6"  Part="1" 
AR Path="/619D8C50/6193A185" Ref="C?"  Part="1" 
F 0 "C?" H 4192 3196 50  0000 L CNN
F 1 "10uF" H 4192 3105 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric" H 4100 3150 50  0001 C CNN
F 3 "~" H 4100 3150 50  0001 C CNN
	1    4100 3150
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 6193A18B
P 4100 3600
AR Path="/6193A18B" Ref="#PWR?"  Part="1" 
AR Path="/619320AD/6193A18B" Ref="#PWR?"  Part="1" 
AR Path="/61914EC1/6193A18B" Ref="#PWR033"  Part="1" 
AR Path="/6191C2CA/6193A18B" Ref="#PWR017"  Part="1" 
AR Path="/619D8C50/6193A18B" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 4100 3350 50  0001 C CNN
F 1 "GND" H 4105 3427 50  0000 C CNN
F 2 "" H 4100 3600 50  0001 C CNN
F 3 "" H 4100 3600 50  0001 C CNN
	1    4100 3600
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R?
U 1 1 6193A191
P 4550 3450
AR Path="/6193A191" Ref="R?"  Part="1" 
AR Path="/619320AD/6193A191" Ref="R?"  Part="1" 
AR Path="/61914EC1/6193A191" Ref="R5"  Part="1" 
AR Path="/6191C2CA/6193A191" Ref="R7"  Part="1" 
AR Path="/619D8C50/6193A191" Ref="R?"  Part="1" 
F 0 "R?" H 4609 3496 50  0000 L CNN
F 1 "68k" H 4609 3405 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 4550 3450 50  0001 C CNN
F 3 "~" H 4550 3450 50  0001 C CNN
	1    4550 3450
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 6193A198
P 4550 3600
AR Path="/6193A198" Ref="#PWR?"  Part="1" 
AR Path="/619320AD/6193A198" Ref="#PWR?"  Part="1" 
AR Path="/61914EC1/6193A198" Ref="#PWR035"  Part="1" 
AR Path="/6191C2CA/6193A198" Ref="#PWR018"  Part="1" 
AR Path="/619D8C50/6193A198" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 4550 3350 50  0001 C CNN
F 1 "GND" H 4555 3427 50  0000 C CNN
F 2 "" H 4550 3600 50  0001 C CNN
F 3 "" H 4550 3600 50  0001 C CNN
	1    4550 3600
	1    0    0    -1  
$EndComp
$Comp
L MP2359:MP2359DT-LF-P U?
U 1 1 6193A1A0
P 5025 2075
AR Path="/6193A1A0" Ref="U?"  Part="1" 
AR Path="/619320AD/6193A1A0" Ref="U?"  Part="1" 
AR Path="/61914EC1/6193A1A0" Ref="U3"  Part="1" 
AR Path="/6191C2CA/6193A1A0" Ref="U5"  Part="1" 
AR Path="/619D8C50/6193A1A0" Ref="U?"  Part="1" 
F 0 "U?" H 5025 2400 50  0000 C CNN
F 1 "MP2359" H 5025 2309 50  0000 C CNN
F 2 "MP2359DT-LF-P:SOT95P280X145-6N" H 5025 2375 50  0000 C CNN
F 3 "" H 5025 2375 50  0001 C CNN
	1    5025 2075
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 6193A1A6
P 4725 2175
AR Path="/6193A1A6" Ref="#PWR?"  Part="1" 
AR Path="/619320AD/6193A1A6" Ref="#PWR?"  Part="1" 
AR Path="/61914EC1/6193A1A6" Ref="#PWR036"  Part="1" 
AR Path="/6191C2CA/6193A1A6" Ref="#PWR019"  Part="1" 
AR Path="/619D8C50/6193A1A6" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 4725 1925 50  0001 C CNN
F 1 "GND" H 4730 2002 50  0000 C CNN
F 2 "" H 4725 2175 50  0001 C CNN
F 3 "" H 4725 2175 50  0001 C CNN
	1    4725 2175
	0    1    1    0   
$EndComp
$Comp
L Device:C_Small C?
U 1 1 6193A1AC
P 6050 2575
AR Path="/6193A1AC" Ref="C?"  Part="1" 
AR Path="/619320AD/6193A1AC" Ref="C?"  Part="1" 
AR Path="/61914EC1/6193A1AC" Ref="C14"  Part="1" 
AR Path="/6191C2CA/6193A1AC" Ref="C7"  Part="1" 
AR Path="/619D8C50/6193A1AC" Ref="C?"  Part="1" 
F 0 "C?" V 5821 2575 50  0000 C CNN
F 1 "10nF" V 5912 2575 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 6050 2575 50  0001 C CNN
F 3 "~" H 6050 2575 50  0001 C CNN
	1    6050 2575
	0    -1   -1   0   
$EndComp
$Comp
L Device:D_Schottky_Small D?
U 1 1 6193A1B2
P 6950 3350
AR Path="/6193A1B2" Ref="D?"  Part="1" 
AR Path="/619320AD/6193A1B2" Ref="D?"  Part="1" 
AR Path="/61914EC1/6193A1B2" Ref="D2"  Part="1" 
AR Path="/6191C2CA/6193A1B2" Ref="D6"  Part="1" 
AR Path="/619D8C50/6193A1B2" Ref="D?"  Part="1" 
F 0 "D?" V 6904 3418 50  0000 L CNN
F 1 "B5819W" V 6995 3418 50  0000 L CNN
F 2 "Diode_SMD:D_SOD-123" V 6950 3350 50  0001 C CNN
F 3 "~" V 6950 3350 50  0001 C CNN
	1    6950 3350
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 6193A1B8
P 6950 3600
AR Path="/6193A1B8" Ref="#PWR?"  Part="1" 
AR Path="/619320AD/6193A1B8" Ref="#PWR?"  Part="1" 
AR Path="/61914EC1/6193A1B8" Ref="#PWR040"  Part="1" 
AR Path="/6191C2CA/6193A1B8" Ref="#PWR022"  Part="1" 
AR Path="/619D8C50/6193A1B8" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 6950 3350 50  0001 C CNN
F 1 "GND" H 6955 3427 50  0000 C CNN
F 2 "" H 6950 3600 50  0001 C CNN
F 3 "" H 6950 3600 50  0001 C CNN
	1    6950 3600
	1    0    0    -1  
$EndComp
$Comp
L Device:L_Small L?
U 1 1 6193A1BE
P 7375 2950
AR Path="/6193A1BE" Ref="L?"  Part="1" 
AR Path="/619320AD/6193A1BE" Ref="L?"  Part="1" 
AR Path="/61914EC1/6193A1BE" Ref="L2"  Part="1" 
AR Path="/6191C2CA/6193A1BE" Ref="L1"  Part="1" 
AR Path="/619D8C50/6193A1BE" Ref="L?"  Part="1" 
F 0 "L?" V 7194 2950 50  0000 C CNN
F 1 "10uH" V 7285 2950 50  0000 C CNN
F 2 "Inductor_SMD:L_7.3x7.3_H4.5" H 7375 2950 50  0001 C CNN
F 3 "~" H 7375 2950 50  0001 C CNN
	1    7375 2950
	0    1    1    0   
$EndComp
$Comp
L Device:C_Small C?
U 1 1 6193A1C6
P 7625 3350
AR Path="/6193A1C6" Ref="C?"  Part="1" 
AR Path="/619320AD/6193A1C6" Ref="C?"  Part="1" 
AR Path="/61914EC1/6193A1C6" Ref="C15"  Part="1" 
AR Path="/6191C2CA/6193A1C6" Ref="C13"  Part="1" 
AR Path="/619D8C50/6193A1C6" Ref="C?"  Part="1" 
F 0 "C?" H 7717 3396 50  0000 L CNN
F 1 "10uF" H 7717 3305 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric" H 7625 3350 50  0001 C CNN
F 3 "~" H 7625 3350 50  0001 C CNN
	1    7625 3350
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C?
U 1 1 6193A1CC
P 7975 3350
AR Path="/6193A1CC" Ref="C?"  Part="1" 
AR Path="/619320AD/6193A1CC" Ref="C?"  Part="1" 
AR Path="/61914EC1/6193A1CC" Ref="C16"  Part="1" 
AR Path="/6191C2CA/6193A1CC" Ref="C14"  Part="1" 
AR Path="/619D8C50/6193A1CC" Ref="C?"  Part="1" 
F 0 "C?" H 8067 3396 50  0000 L CNN
F 1 "10uF" H 8067 3305 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric" H 7975 3350 50  0001 C CNN
F 3 "~" H 7975 3350 50  0001 C CNN
	1    7975 3350
	1    0    0    -1  
$EndComp
Wire Wire Line
	7975 2950 7975 2850
$Comp
L power:GND #PWR?
U 1 1 6193A1D3
P 7625 3600
AR Path="/6193A1D3" Ref="#PWR?"  Part="1" 
AR Path="/619320AD/6193A1D3" Ref="#PWR?"  Part="1" 
AR Path="/61914EC1/6193A1D3" Ref="#PWR043"  Part="1" 
AR Path="/6191C2CA/6193A1D3" Ref="#PWR025"  Part="1" 
AR Path="/619D8C50/6193A1D3" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 7625 3350 50  0001 C CNN
F 1 "GND" H 7630 3427 50  0000 C CNN
F 2 "" H 7625 3600 50  0001 C CNN
F 3 "" H 7625 3600 50  0001 C CNN
	1    7625 3600
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 6193A1D9
P 7975 3600
AR Path="/6193A1D9" Ref="#PWR?"  Part="1" 
AR Path="/619320AD/6193A1D9" Ref="#PWR?"  Part="1" 
AR Path="/61914EC1/6193A1D9" Ref="#PWR048"  Part="1" 
AR Path="/6191C2CA/6193A1D9" Ref="#PWR027"  Part="1" 
AR Path="/619D8C50/6193A1D9" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 7975 3350 50  0001 C CNN
F 1 "GND" H 7980 3427 50  0000 C CNN
F 2 "" H 7975 3600 50  0001 C CNN
F 3 "" H 7975 3600 50  0001 C CNN
	1    7975 3600
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R?
U 1 1 6193A1DF
P 5450 4050
AR Path="/6193A1DF" Ref="R?"  Part="1" 
AR Path="/619320AD/6193A1DF" Ref="R?"  Part="1" 
AR Path="/61914EC1/6193A1DF" Ref="R6"  Part="1" 
AR Path="/6191C2CA/6193A1DF" Ref="R8"  Part="1" 
AR Path="/619D8C50/6193A1DF" Ref="R?"  Part="1" 
F 0 "R?" H 5509 4096 50  0000 L CNN
F 1 "47k" H 5509 4005 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 5450 4050 50  0001 C CNN
F 3 "~" H 5450 4050 50  0001 C CNN
	1    5450 4050
	-1   0    0    1   
$EndComp
$Comp
L Device:R_Small R?
U 1 1 6193A1E5
P 5450 4325
AR Path="/6193A1E5" Ref="R?"  Part="1" 
AR Path="/619320AD/6193A1E5" Ref="R?"  Part="1" 
AR Path="/61914EC1/6193A1E5" Ref="R7"  Part="1" 
AR Path="/6191C2CA/6193A1E5" Ref="R9"  Part="1" 
AR Path="/619D8C50/6193A1E5" Ref="R?"  Part="1" 
F 0 "R?" H 5509 4371 50  0000 L CNN
F 1 "15k" H 5509 4280 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 5450 4325 50  0001 C CNN
F 3 "~" H 5450 4325 50  0001 C CNN
	1    5450 4325
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 6193A1EB
P 5450 4625
AR Path="/6193A1EB" Ref="#PWR?"  Part="1" 
AR Path="/619320AD/6193A1EB" Ref="#PWR?"  Part="1" 
AR Path="/61914EC1/6193A1EB" Ref="#PWR038"  Part="1" 
AR Path="/6191C2CA/6193A1EB" Ref="#PWR021"  Part="1" 
AR Path="/619D8C50/6193A1EB" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 5450 4375 50  0001 C CNN
F 1 "GND" H 5455 4452 50  0000 C CNN
F 2 "" H 5450 4625 50  0001 C CNN
F 3 "" H 5450 4625 50  0001 C CNN
	1    5450 4625
	1    0    0    -1  
$EndComp
Text Notes 2750 4300 0    50   ~ 0
12V to 3.3V buck-converter
$Comp
L Device:R_Small R?
U 1 1 6193A1F2
P 4550 3000
AR Path="/6193A1F2" Ref="R?"  Part="1" 
AR Path="/619320AD/6193A1F2" Ref="R?"  Part="1" 
AR Path="/61914EC1/6193A1F2" Ref="R4"  Part="1" 
AR Path="/6191C2CA/6193A1F2" Ref="R5"  Part="1" 
AR Path="/619D8C50/6193A1F2" Ref="R?"  Part="1" 
F 0 "R?" H 4609 3046 50  0000 L CNN
F 1 "100k" H 4609 2955 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 4550 3000 50  0001 C CNN
F 3 "~" H 4550 3000 50  0001 C CNN
	1    4550 3000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 6193A1F8
P 2725 2550
AR Path="/6193A1F8" Ref="#PWR?"  Part="1" 
AR Path="/619320AD/6193A1F8" Ref="#PWR?"  Part="1" 
AR Path="/61914EC1/6193A1F8" Ref="#PWR032"  Part="1" 
AR Path="/6191C2CA/6193A1F8" Ref="#PWR016"  Part="1" 
AR Path="/619D8C50/6193A1F8" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 2725 2300 50  0001 C CNN
F 1 "GND" H 2730 2377 50  0000 C CNN
F 2 "" H 2725 2550 50  0001 C CNN
F 3 "" H 2725 2550 50  0001 C CNN
	1    2725 2550
	-1   0    0    1   
$EndComp
Wire Wire Line
	2225 2850 2325 2850
Wire Wire Line
	4050 2850 4100 2850
Wire Wire Line
	4100 2850 4100 3050
Wire Wire Line
	4550 3550 4550 3600
Wire Wire Line
	4550 2900 4550 2850
Wire Wire Line
	4550 2850 4100 2850
Connection ~ 4100 2850
Wire Wire Line
	4100 3600 4100 3250
Wire Wire Line
	6950 2950 6950 3250
Wire Wire Line
	7475 2950 7625 2950
Wire Wire Line
	6950 3450 6950 3600
Wire Wire Line
	7625 3250 7625 2950
Connection ~ 7625 2950
Wire Wire Line
	7625 2950 7975 2950
Wire Wire Line
	7625 3450 7625 3600
Wire Wire Line
	7975 3450 7975 3600
Wire Wire Line
	7975 3250 7975 2950
Text HLabel 9300 3750 2    50   Output ~ 0
3.3V
$Comp
L power:+3.3V #PWR?
U 1 1 6193DA4C
P 7975 2850
F 0 "#PWR?" H 7975 2700 50  0001 C CNN
F 1 "+3.3V" H 7990 3023 50  0000 C CNN
F 2 "" H 7975 2850 50  0001 C CNN
F 3 "" H 7975 2850 50  0001 C CNN
	1    7975 2850
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR?
U 1 1 6193F083
P 5450 3950
F 0 "#PWR?" H 5450 3800 50  0001 C CNN
F 1 "+3.3V" H 5465 4123 50  0000 C CNN
F 2 "" H 5450 3950 50  0001 C CNN
F 3 "" H 5450 3950 50  0001 C CNN
	1    5450 3950
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R?
U 1 1 61941203
P 5450 4525
AR Path="/61941203" Ref="R?"  Part="1" 
AR Path="/619320AD/61941203" Ref="R?"  Part="1" 
AR Path="/61914EC1/61941203" Ref="R8"  Part="1" 
AR Path="/6191C2CA/61941203" Ref="R10"  Part="1" 
AR Path="/619D8C50/61941203" Ref="R?"  Part="1" 
F 0 "R?" H 5509 4571 50  0000 L CNN
F 1 "270" H 5509 4480 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 5450 4525 50  0001 C CNN
F 3 "~" H 5450 4525 50  0001 C CNN
	1    5450 4525
	1    0    0    -1  
$EndComp
Wire Wire Line
	5450 4150 5450 4175
Wire Wire Line
	5750 4175 5450 4175
Connection ~ 5450 4175
Wire Wire Line
	5450 4175 5450 4225
Text GLabel 5750 4175 2    50   Input ~ 0
BUCK_FB
$Comp
L Device:LED D?
U 1 1 619490BA
P 7350 4275
F 0 "D?" V 7389 4158 50  0000 R CNN
F 1 "LED" V 7298 4158 50  0000 R CNN
F 2 "LED_SMD:LED_0603_1608Metric" H 7350 4275 50  0001 C CNN
F 3 "~" H 7350 4275 50  0001 C CNN
	1    7350 4275
	0    -1   -1   0   
$EndComp
$Comp
L power:+3.3V #PWR?
U 1 1 61949A15
P 7350 4125
F 0 "#PWR?" H 7350 3975 50  0001 C CNN
F 1 "+3.3V" H 7365 4298 50  0000 C CNN
F 2 "" H 7350 4125 50  0001 C CNN
F 3 "" H 7350 4125 50  0001 C CNN
	1    7350 4125
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R?
U 1 1 61949E1B
P 7350 4525
AR Path="/61949E1B" Ref="R?"  Part="1" 
AR Path="/619320AD/61949E1B" Ref="R?"  Part="1" 
AR Path="/61914EC1/61949E1B" Ref="R9"  Part="1" 
AR Path="/6191C2CA/61949E1B" Ref="R14"  Part="1" 
AR Path="/619D8C50/61949E1B" Ref="R?"  Part="1" 
F 0 "R?" H 7409 4571 50  0000 L CNN
F 1 "1k" H 7409 4480 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 7350 4525 50  0001 C CNN
F 3 "~" H 7350 4525 50  0001 C CNN
	1    7350 4525
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 6194A24C
P 7350 4625
AR Path="/6194A24C" Ref="#PWR?"  Part="1" 
AR Path="/619320AD/6194A24C" Ref="#PWR?"  Part="1" 
AR Path="/61914EC1/6194A24C" Ref="#PWR042"  Part="1" 
AR Path="/6191C2CA/6194A24C" Ref="#PWR024"  Part="1" 
AR Path="/619D8C50/6194A24C" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 7350 4375 50  0001 C CNN
F 1 "GND" H 7355 4452 50  0000 C CNN
F 2 "" H 7350 4625 50  0001 C CNN
F 3 "" H 7350 4625 50  0001 C CNN
	1    7350 4625
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR?
U 1 1 6194B178
P 9075 3750
F 0 "#PWR?" H 9075 3600 50  0001 C CNN
F 1 "+3.3V" H 9090 3923 50  0000 C CNN
F 2 "" H 9075 3750 50  0001 C CNN
F 3 "" H 9075 3750 50  0001 C CNN
	1    9075 3750
	1    0    0    -1  
$EndComp
Wire Wire Line
	9075 3750 9300 3750
$Comp
L power:+12V #PWR?
U 1 1 6194BCDC
P 9075 4050
AR Path="/6194BCDC" Ref="#PWR?"  Part="1" 
AR Path="/619320AD/6194BCDC" Ref="#PWR?"  Part="1" 
AR Path="/61914EC1/6194BCDC" Ref="#PWR050"  Part="1" 
AR Path="/6191C2CA/6194BCDC" Ref="#PWR029"  Part="1" 
AR Path="/619D8C50/6194BCDC" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 9075 3900 50  0001 C CNN
F 1 "+12V" H 9090 4223 50  0000 C CNN
F 2 "" H 9075 4050 50  0001 C CNN
F 3 "" H 9075 4050 50  0001 C CNN
	1    9075 4050
	1    0    0    -1  
$EndComp
Text HLabel 9300 4050 2    50   Input ~ 0
12V
Wire Wire Line
	9075 4050 9300 4050
$Comp
L power:GND #PWR?
U 1 1 6194CBE1
P 9075 4300
AR Path="/6194CBE1" Ref="#PWR?"  Part="1" 
AR Path="/619320AD/6194CBE1" Ref="#PWR?"  Part="1" 
AR Path="/61914EC1/6194CBE1" Ref="#PWR051"  Part="1" 
AR Path="/6191C2CA/6194CBE1" Ref="#PWR030"  Part="1" 
AR Path="/619D8C50/6194CBE1" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 9075 4050 50  0001 C CNN
F 1 "GND" H 9080 4127 50  0000 C CNN
F 2 "" H 9075 4300 50  0001 C CNN
F 3 "" H 9075 4300 50  0001 C CNN
	1    9075 4300
	-1   0    0    1   
$EndComp
Text HLabel 9300 4300 2    50   BiDi ~ 0
GND
Wire Wire Line
	9300 4300 9075 4300
Wire Notes Line
	5150 3725 6175 3725
Wire Notes Line
	6175 3725 6175 4850
Wire Notes Line
	6175 4850 5150 4850
Wire Notes Line
	5150 4850 5150 3725
Text Notes 5525 4050 0    50   ~ 0
Set voltage value\n
Wire Wire Line
	5025 2175 4725 2175
Text GLabel 6025 2175 2    50   Input ~ 0
BUCK_IN
Text GLabel 6025 2275 2    50   Input ~ 0
BUCK_EN
Text GLabel 5025 2275 0    50   Input ~ 0
BUCK_FB
Text GLabel 5025 2075 0    50   Input ~ 0
BUCK_BST
Text GLabel 6025 2075 2    50   Input ~ 0
BUCK_SW
Text GLabel 4675 2850 2    50   Input ~ 0
BUCK_IN
Wire Wire Line
	4550 2850 4675 2850
Connection ~ 4550 2850
Wire Wire Line
	4550 3100 4550 3225
Text GLabel 4675 3225 2    50   Input ~ 0
BUCK_EN
Wire Wire Line
	4675 3225 4550 3225
Connection ~ 4550 3225
Wire Wire Line
	4550 3225 4550 3350
Text GLabel 6825 2950 0    50   Input ~ 0
BUCK_SW
Text GLabel 6150 2575 2    50   Input ~ 0
BUCK_SW
Text GLabel 5950 2575 0    50   Input ~ 0
BUCK_BST
Wire Wire Line
	6825 2950 6950 2950
Wire Wire Line
	7275 2950 6950 2950
Connection ~ 6950 2950
Wire Wire Line
	2925 2850 3300 2850
$Comp
L A03401A:AO3401A U?
U 1 1 6193A1FE
P 2725 2550
AR Path="/6193A1FE" Ref="U?"  Part="1" 
AR Path="/619320AD/6193A1FE" Ref="U?"  Part="1" 
AR Path="/61914EC1/6193A1FE" Ref="U2"  Part="1" 
AR Path="/6191C2CA/6193A1FE" Ref="U4"  Part="1" 
AR Path="/619D8C50/6193A1FE" Ref="U?"  Part="1" 
F 0 "U?" V 3175 2650 50  0000 C CNN
F 1 "AO3401A" V 3250 2700 50  0000 C CNN
F 2 "AO3401A:SOT95P280X125-3N" H 2725 2550 50  0001 L BNN
F 3 "" H 2725 2550 50  0001 L BNN
	1    2725 2550
	0    -1   1    0   
$EndComp
Connection ~ 7975 2950
$EndSCHEMATC