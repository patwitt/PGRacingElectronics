EESchema Schematic File Version 4
LIBS:PDM-cache
EELAYER 29 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 8
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
P 3125 3575
AR Path="/6193A171" Ref="#PWR?"  Part="1" 
AR Path="/619320AD/6193A171" Ref="#PWR?"  Part="1" 
AR Path="/61914EC1/6193A171" Ref="#PWR031"  Part="1" 
AR Path="/6191C2CA/6193A171" Ref="#PWR015"  Part="1" 
F 0 "#PWR015" H 3125 3425 50  0001 C CNN
F 1 "+12V" H 3140 3748 50  0000 C CNN
F 2 "" H 3125 3575 50  0001 C CNN
F 3 "" H 3125 3575 50  0001 C CNN
	1    3125 3575
	1    0    0    -1  
$EndComp
Wire Wire Line
	3125 3575 3125 3775
$Comp
L Device:Polyfuse_Small F?
U 1 1 6193A178
P 4300 3775
AR Path="/6193A178" Ref="F?"  Part="1" 
AR Path="/619320AD/6193A178" Ref="F?"  Part="1" 
AR Path="/61914EC1/6193A178" Ref="F1"  Part="1" 
AR Path="/6191C2CA/6193A178" Ref="F1"  Part="1" 
F 0 "F1" V 4095 3775 50  0000 C CNN
F 1 "Polyfuse_Small" V 4200 3775 50  0000 C CNN
F 2 "Fuse:Fuse_1206_3216Metric" H 4350 3575 50  0001 L CNN
F 3 "~" H 4300 3775 50  0001 C CNN
	1    4300 3775
	0    1    1    0   
$EndComp
$Comp
L Device:Ferrite_Bead_Small FB?
U 1 1 6193A17E
P 4850 3775
AR Path="/6193A17E" Ref="FB?"  Part="1" 
AR Path="/619320AD/6193A17E" Ref="FB?"  Part="1" 
AR Path="/61914EC1/6193A17E" Ref="FB1"  Part="1" 
AR Path="/6191C2CA/6193A17E" Ref="FB1"  Part="1" 
F 0 "FB1" V 4613 3775 50  0000 C CNN
F 1 "600 @ 600Mhz" V 4704 3775 50  0000 C CNN
F 2 "Inductor_SMD:L_0805_2012Metric" V 4780 3775 50  0001 C CNN
F 3 "~" H 4850 3775 50  0001 C CNN
	1    4850 3775
	0    1    1    0   
$EndComp
Wire Wire Line
	4400 3775 4750 3775
$Comp
L Device:C_Small C?
U 1 1 6193A185
P 5000 4075
AR Path="/6193A185" Ref="C?"  Part="1" 
AR Path="/619320AD/6193A185" Ref="C?"  Part="1" 
AR Path="/61914EC1/6193A185" Ref="C13"  Part="1" 
AR Path="/6191C2CA/6193A185" Ref="C6"  Part="1" 
F 0 "C6" H 5092 4121 50  0000 L CNN
F 1 "10uF" H 5092 4030 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric" H 5000 4075 50  0001 C CNN
F 3 "~" H 5000 4075 50  0001 C CNN
	1    5000 4075
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 6193A18B
P 5000 4525
AR Path="/6193A18B" Ref="#PWR?"  Part="1" 
AR Path="/619320AD/6193A18B" Ref="#PWR?"  Part="1" 
AR Path="/61914EC1/6193A18B" Ref="#PWR033"  Part="1" 
AR Path="/6191C2CA/6193A18B" Ref="#PWR017"  Part="1" 
F 0 "#PWR017" H 5000 4275 50  0001 C CNN
F 1 "GND" H 5005 4352 50  0000 C CNN
F 2 "" H 5000 4525 50  0001 C CNN
F 3 "" H 5000 4525 50  0001 C CNN
	1    5000 4525
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R?
U 1 1 6193A191
P 5450 4375
AR Path="/6193A191" Ref="R?"  Part="1" 
AR Path="/619320AD/6193A191" Ref="R?"  Part="1" 
AR Path="/61914EC1/6193A191" Ref="R5"  Part="1" 
AR Path="/6191C2CA/6193A191" Ref="R7"  Part="1" 
F 0 "R7" H 5509 4421 50  0000 L CNN
F 1 "68k" H 5509 4330 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 5450 4375 50  0001 C CNN
F 3 "~" H 5450 4375 50  0001 C CNN
	1    5450 4375
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 6193A198
P 5450 4525
AR Path="/6193A198" Ref="#PWR?"  Part="1" 
AR Path="/619320AD/6193A198" Ref="#PWR?"  Part="1" 
AR Path="/61914EC1/6193A198" Ref="#PWR035"  Part="1" 
AR Path="/6191C2CA/6193A198" Ref="#PWR018"  Part="1" 
F 0 "#PWR018" H 5450 4275 50  0001 C CNN
F 1 "GND" H 5455 4352 50  0000 C CNN
F 2 "" H 5450 4525 50  0001 C CNN
F 3 "" H 5450 4525 50  0001 C CNN
	1    5450 4525
	1    0    0    -1  
$EndComp
$Comp
L MP2359DT-LF-P:MP2359DT-LF-P U?
U 1 1 6193A1A0
P 5925 3000
AR Path="/6193A1A0" Ref="U?"  Part="1" 
AR Path="/619320AD/6193A1A0" Ref="U?"  Part="1" 
AR Path="/61914EC1/6193A1A0" Ref="U3"  Part="1" 
AR Path="/6191C2CA/6193A1A0" Ref="U5"  Part="1" 
F 0 "U5" H 5925 3325 50  0000 C CNN
F 1 "MP2359" H 5925 3234 50  0000 C CNN
F 2 "MP2359DT-LF-P:SOT95P280X145-6N" H 5925 3300 50  0001 C CNN
F 3 "" H 5925 3300 50  0001 C CNN
	1    5925 3000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 6193A1A6
P 5625 3100
AR Path="/6193A1A6" Ref="#PWR?"  Part="1" 
AR Path="/619320AD/6193A1A6" Ref="#PWR?"  Part="1" 
AR Path="/61914EC1/6193A1A6" Ref="#PWR036"  Part="1" 
AR Path="/6191C2CA/6193A1A6" Ref="#PWR019"  Part="1" 
F 0 "#PWR019" H 5625 2850 50  0001 C CNN
F 1 "GND" H 5630 2927 50  0000 C CNN
F 2 "" H 5625 3100 50  0001 C CNN
F 3 "" H 5625 3100 50  0001 C CNN
	1    5625 3100
	0    1    1    0   
$EndComp
$Comp
L Device:C_Small C?
U 1 1 6193A1AC
P 6950 3500
AR Path="/6193A1AC" Ref="C?"  Part="1" 
AR Path="/619320AD/6193A1AC" Ref="C?"  Part="1" 
AR Path="/61914EC1/6193A1AC" Ref="C14"  Part="1" 
AR Path="/6191C2CA/6193A1AC" Ref="C7"  Part="1" 
F 0 "C7" V 6721 3500 50  0000 C CNN
F 1 "10nF" V 6812 3500 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 6950 3500 50  0001 C CNN
F 3 "~" H 6950 3500 50  0001 C CNN
	1    6950 3500
	0    -1   -1   0   
$EndComp
$Comp
L Device:D_Schottky_Small D?
U 1 1 6193A1B2
P 7850 4275
AR Path="/6193A1B2" Ref="D?"  Part="1" 
AR Path="/619320AD/6193A1B2" Ref="D?"  Part="1" 
AR Path="/61914EC1/6193A1B2" Ref="D2"  Part="1" 
AR Path="/6191C2CA/6193A1B2" Ref="D6"  Part="1" 
F 0 "D6" V 7804 4343 50  0000 L CNN
F 1 "B5819W" V 7895 4343 50  0000 L CNN
F 2 "Diode_SMD:D_SOD-123" V 7850 4275 50  0001 C CNN
F 3 "~" V 7850 4275 50  0001 C CNN
	1    7850 4275
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 6193A1B8
P 7850 4525
AR Path="/6193A1B8" Ref="#PWR?"  Part="1" 
AR Path="/619320AD/6193A1B8" Ref="#PWR?"  Part="1" 
AR Path="/61914EC1/6193A1B8" Ref="#PWR040"  Part="1" 
AR Path="/6191C2CA/6193A1B8" Ref="#PWR022"  Part="1" 
F 0 "#PWR022" H 7850 4275 50  0001 C CNN
F 1 "GND" H 7855 4352 50  0000 C CNN
F 2 "" H 7850 4525 50  0001 C CNN
F 3 "" H 7850 4525 50  0001 C CNN
	1    7850 4525
	1    0    0    -1  
$EndComp
$Comp
L Device:L_Small L?
U 1 1 6193A1BE
P 8275 3875
AR Path="/6193A1BE" Ref="L?"  Part="1" 
AR Path="/619320AD/6193A1BE" Ref="L?"  Part="1" 
AR Path="/61914EC1/6193A1BE" Ref="L2"  Part="1" 
AR Path="/6191C2CA/6193A1BE" Ref="L1"  Part="1" 
F 0 "L1" V 8094 3875 50  0000 C CNN
F 1 "10uH" V 8185 3875 50  0000 C CNN
F 2 "Inductor_SMD:L_7.3x7.3_H4.5" H 8275 3875 50  0001 C CNN
F 3 "~" H 8275 3875 50  0001 C CNN
	1    8275 3875
	0    1    1    0   
$EndComp
$Comp
L Device:C_Small C?
U 1 1 6193A1C6
P 8525 4275
AR Path="/6193A1C6" Ref="C?"  Part="1" 
AR Path="/619320AD/6193A1C6" Ref="C?"  Part="1" 
AR Path="/61914EC1/6193A1C6" Ref="C15"  Part="1" 
AR Path="/6191C2CA/6193A1C6" Ref="C13"  Part="1" 
F 0 "C13" H 8617 4321 50  0000 L CNN
F 1 "10uF" H 8617 4230 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric" H 8525 4275 50  0001 C CNN
F 3 "~" H 8525 4275 50  0001 C CNN
	1    8525 4275
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C?
U 1 1 6193A1CC
P 8875 4275
AR Path="/6193A1CC" Ref="C?"  Part="1" 
AR Path="/619320AD/6193A1CC" Ref="C?"  Part="1" 
AR Path="/61914EC1/6193A1CC" Ref="C16"  Part="1" 
AR Path="/6191C2CA/6193A1CC" Ref="C14"  Part="1" 
F 0 "C14" H 8967 4321 50  0000 L CNN
F 1 "10uF" H 8967 4230 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric" H 8875 4275 50  0001 C CNN
F 3 "~" H 8875 4275 50  0001 C CNN
	1    8875 4275
	1    0    0    -1  
$EndComp
Wire Wire Line
	8875 3875 8875 3775
$Comp
L power:GND #PWR?
U 1 1 6193A1D3
P 8525 4525
AR Path="/6193A1D3" Ref="#PWR?"  Part="1" 
AR Path="/619320AD/6193A1D3" Ref="#PWR?"  Part="1" 
AR Path="/61914EC1/6193A1D3" Ref="#PWR043"  Part="1" 
AR Path="/6191C2CA/6193A1D3" Ref="#PWR025"  Part="1" 
F 0 "#PWR025" H 8525 4275 50  0001 C CNN
F 1 "GND" H 8530 4352 50  0000 C CNN
F 2 "" H 8525 4525 50  0001 C CNN
F 3 "" H 8525 4525 50  0001 C CNN
	1    8525 4525
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 6193A1D9
P 8875 4525
AR Path="/6193A1D9" Ref="#PWR?"  Part="1" 
AR Path="/619320AD/6193A1D9" Ref="#PWR?"  Part="1" 
AR Path="/61914EC1/6193A1D9" Ref="#PWR048"  Part="1" 
AR Path="/6191C2CA/6193A1D9" Ref="#PWR027"  Part="1" 
F 0 "#PWR027" H 8875 4275 50  0001 C CNN
F 1 "GND" H 8880 4352 50  0000 C CNN
F 2 "" H 8875 4525 50  0001 C CNN
F 3 "" H 8875 4525 50  0001 C CNN
	1    8875 4525
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R?
U 1 1 6193A1DF
P 6350 4975
AR Path="/6193A1DF" Ref="R?"  Part="1" 
AR Path="/619320AD/6193A1DF" Ref="R?"  Part="1" 
AR Path="/61914EC1/6193A1DF" Ref="R6"  Part="1" 
AR Path="/6191C2CA/6193A1DF" Ref="R8"  Part="1" 
F 0 "R8" H 6409 5021 50  0000 L CNN
F 1 "47k" H 6409 4930 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 6350 4975 50  0001 C CNN
F 3 "~" H 6350 4975 50  0001 C CNN
	1    6350 4975
	-1   0    0    1   
$EndComp
$Comp
L Device:R_Small R?
U 1 1 6193A1E5
P 6350 5250
AR Path="/6193A1E5" Ref="R?"  Part="1" 
AR Path="/619320AD/6193A1E5" Ref="R?"  Part="1" 
AR Path="/61914EC1/6193A1E5" Ref="R7"  Part="1" 
AR Path="/6191C2CA/6193A1E5" Ref="R9"  Part="1" 
F 0 "R9" H 6409 5296 50  0000 L CNN
F 1 "15k" H 6409 5205 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 6350 5250 50  0001 C CNN
F 3 "~" H 6350 5250 50  0001 C CNN
	1    6350 5250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 6193A1EB
P 6350 5550
AR Path="/6193A1EB" Ref="#PWR?"  Part="1" 
AR Path="/619320AD/6193A1EB" Ref="#PWR?"  Part="1" 
AR Path="/61914EC1/6193A1EB" Ref="#PWR038"  Part="1" 
AR Path="/6191C2CA/6193A1EB" Ref="#PWR021"  Part="1" 
F 0 "#PWR021" H 6350 5300 50  0001 C CNN
F 1 "GND" H 6355 5377 50  0000 C CNN
F 2 "" H 6350 5550 50  0001 C CNN
F 3 "" H 6350 5550 50  0001 C CNN
	1    6350 5550
	1    0    0    -1  
$EndComp
Text Notes 3525 4550 0    50   ~ 0
12V to 3.3V buck-converter
$Comp
L Device:R_Small R?
U 1 1 6193A1F2
P 5450 3925
AR Path="/6193A1F2" Ref="R?"  Part="1" 
AR Path="/619320AD/6193A1F2" Ref="R?"  Part="1" 
AR Path="/61914EC1/6193A1F2" Ref="R4"  Part="1" 
AR Path="/6191C2CA/6193A1F2" Ref="R5"  Part="1" 
F 0 "R5" H 5509 3971 50  0000 L CNN
F 1 "100k" H 5509 3880 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 5450 3925 50  0001 C CNN
F 3 "~" H 5450 3925 50  0001 C CNN
	1    5450 3925
	1    0    0    -1  
$EndComp
Wire Wire Line
	4950 3775 5000 3775
Wire Wire Line
	5000 3775 5000 3975
Wire Wire Line
	5450 4475 5450 4525
Wire Wire Line
	5450 3825 5450 3775
Wire Wire Line
	5450 3775 5000 3775
Connection ~ 5000 3775
Wire Wire Line
	5000 4525 5000 4175
Wire Wire Line
	7850 3875 7850 4175
Wire Wire Line
	8375 3875 8525 3875
Wire Wire Line
	7850 4375 7850 4525
Wire Wire Line
	8525 4175 8525 3875
Connection ~ 8525 3875
Wire Wire Line
	8525 3875 8875 3875
Wire Wire Line
	8525 4375 8525 4525
Wire Wire Line
	8875 4375 8875 4525
Wire Wire Line
	8875 4175 8875 3875
Text HLabel 10200 4675 2    50   Output ~ 0
3.3V
$Comp
L power:+3.3V #PWR026
U 1 1 6193DA4C
P 8875 3775
F 0 "#PWR026" H 8875 3625 50  0001 C CNN
F 1 "+3.3V" H 8890 3948 50  0000 C CNN
F 2 "" H 8875 3775 50  0001 C CNN
F 3 "" H 8875 3775 50  0001 C CNN
	1    8875 3775
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR020
U 1 1 6193F083
P 6350 4875
F 0 "#PWR020" H 6350 4725 50  0001 C CNN
F 1 "+3.3V" H 6365 5048 50  0000 C CNN
F 2 "" H 6350 4875 50  0001 C CNN
F 3 "" H 6350 4875 50  0001 C CNN
	1    6350 4875
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R?
U 1 1 61941203
P 6350 5450
AR Path="/61941203" Ref="R?"  Part="1" 
AR Path="/619320AD/61941203" Ref="R?"  Part="1" 
AR Path="/61914EC1/61941203" Ref="R8"  Part="1" 
AR Path="/6191C2CA/61941203" Ref="R10"  Part="1" 
F 0 "R10" H 6409 5496 50  0000 L CNN
F 1 "270" H 6409 5405 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 6350 5450 50  0001 C CNN
F 3 "~" H 6350 5450 50  0001 C CNN
	1    6350 5450
	1    0    0    -1  
$EndComp
Wire Wire Line
	6350 5075 6350 5100
Wire Wire Line
	6650 5100 6350 5100
Connection ~ 6350 5100
Wire Wire Line
	6350 5100 6350 5150
Text GLabel 6650 5100 2    50   Input ~ 0
BUCK_FB
$Comp
L Device:LED D7
U 1 1 619490BA
P 8250 5200
F 0 "D7" V 8289 5083 50  0000 R CNN
F 1 "LED" V 8198 5083 50  0000 R CNN
F 2 "LED_SMD:LED_0603_1608Metric" H 8250 5200 50  0001 C CNN
F 3 "~" H 8250 5200 50  0001 C CNN
	1    8250 5200
	0    -1   -1   0   
$EndComp
$Comp
L power:+3.3V #PWR023
U 1 1 61949A15
P 8250 5050
F 0 "#PWR023" H 8250 4900 50  0001 C CNN
F 1 "+3.3V" H 8265 5223 50  0000 C CNN
F 2 "" H 8250 5050 50  0001 C CNN
F 3 "" H 8250 5050 50  0001 C CNN
	1    8250 5050
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R?
U 1 1 61949E1B
P 8250 5450
AR Path="/61949E1B" Ref="R?"  Part="1" 
AR Path="/619320AD/61949E1B" Ref="R?"  Part="1" 
AR Path="/61914EC1/61949E1B" Ref="R9"  Part="1" 
AR Path="/6191C2CA/61949E1B" Ref="R14"  Part="1" 
F 0 "R14" H 8309 5496 50  0000 L CNN
F 1 "1k" H 8309 5405 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 8250 5450 50  0001 C CNN
F 3 "~" H 8250 5450 50  0001 C CNN
	1    8250 5450
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 6194A24C
P 8250 5550
AR Path="/6194A24C" Ref="#PWR?"  Part="1" 
AR Path="/619320AD/6194A24C" Ref="#PWR?"  Part="1" 
AR Path="/61914EC1/6194A24C" Ref="#PWR042"  Part="1" 
AR Path="/6191C2CA/6194A24C" Ref="#PWR024"  Part="1" 
F 0 "#PWR024" H 8250 5300 50  0001 C CNN
F 1 "GND" H 8255 5377 50  0000 C CNN
F 2 "" H 8250 5550 50  0001 C CNN
F 3 "" H 8250 5550 50  0001 C CNN
	1    8250 5550
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR028
U 1 1 6194B178
P 9975 4675
F 0 "#PWR028" H 9975 4525 50  0001 C CNN
F 1 "+3.3V" H 9990 4848 50  0000 C CNN
F 2 "" H 9975 4675 50  0001 C CNN
F 3 "" H 9975 4675 50  0001 C CNN
	1    9975 4675
	1    0    0    -1  
$EndComp
Wire Wire Line
	9975 4675 10200 4675
$Comp
L power:+12V #PWR?
U 1 1 6194BCDC
P 9975 4975
AR Path="/6194BCDC" Ref="#PWR?"  Part="1" 
AR Path="/619320AD/6194BCDC" Ref="#PWR?"  Part="1" 
AR Path="/61914EC1/6194BCDC" Ref="#PWR050"  Part="1" 
AR Path="/6191C2CA/6194BCDC" Ref="#PWR029"  Part="1" 
F 0 "#PWR029" H 9975 4825 50  0001 C CNN
F 1 "+12V" H 9990 5148 50  0000 C CNN
F 2 "" H 9975 4975 50  0001 C CNN
F 3 "" H 9975 4975 50  0001 C CNN
	1    9975 4975
	1    0    0    -1  
$EndComp
Text HLabel 10200 4975 2    50   Input ~ 0
12V
Wire Wire Line
	9975 4975 10200 4975
$Comp
L power:GND #PWR?
U 1 1 6194CBE1
P 9975 5225
AR Path="/6194CBE1" Ref="#PWR?"  Part="1" 
AR Path="/619320AD/6194CBE1" Ref="#PWR?"  Part="1" 
AR Path="/61914EC1/6194CBE1" Ref="#PWR051"  Part="1" 
AR Path="/6191C2CA/6194CBE1" Ref="#PWR030"  Part="1" 
F 0 "#PWR030" H 9975 4975 50  0001 C CNN
F 1 "GND" H 9980 5052 50  0000 C CNN
F 2 "" H 9975 5225 50  0001 C CNN
F 3 "" H 9975 5225 50  0001 C CNN
	1    9975 5225
	-1   0    0    1   
$EndComp
Text HLabel 10200 5225 2    50   BiDi ~ 0
GND
Wire Wire Line
	10200 5225 9975 5225
Wire Notes Line
	6050 4650 7075 4650
Wire Notes Line
	7075 4650 7075 5775
Wire Notes Line
	7075 5775 6050 5775
Wire Notes Line
	6050 5775 6050 4650
Text Notes 6425 4975 0    50   ~ 0
Set voltage value\n
Wire Wire Line
	5925 3100 5625 3100
Text GLabel 6925 3100 2    50   Input ~ 0
BUCK_IN
Text GLabel 6925 3200 2    50   Input ~ 0
BUCK_EN
Text GLabel 5925 3200 0    50   Input ~ 0
BUCK_FB
Text GLabel 5925 3000 0    50   Input ~ 0
BUCK_BST
Text GLabel 6925 3000 2    50   Input ~ 0
BUCK_SW
Text GLabel 5575 3775 2    50   Input ~ 0
BUCK_IN
Wire Wire Line
	5450 3775 5575 3775
Connection ~ 5450 3775
Wire Wire Line
	5450 4025 5450 4150
Text GLabel 5575 4150 2    50   Input ~ 0
BUCK_EN
Wire Wire Line
	5575 4150 5450 4150
Connection ~ 5450 4150
Wire Wire Line
	5450 4150 5450 4275
Text GLabel 7725 3875 0    50   Input ~ 0
BUCK_SW
Text GLabel 7050 3500 2    50   Input ~ 0
BUCK_SW
Text GLabel 6850 3500 0    50   Input ~ 0
BUCK_BST
Wire Wire Line
	7725 3875 7850 3875
Wire Wire Line
	8175 3875 7850 3875
Connection ~ 7850 3875
Connection ~ 8875 3875
Text HLabel 10200 4400 2    50   Output ~ 0
5V_ST_LINK
Wire Wire Line
	9975 4400 10200 4400
Text GLabel 9975 4400 0    50   Input ~ 0
5V_ST_LINK
Text GLabel 4100 3125 1    50   Input ~ 0
5V_ST_LINK
$Comp
L AO3401A:AO3401A Q1
U 1 1 6199F078
P 3800 4075
F 0 "Q1" V 4367 4175 50  0000 C CNN
F 1 "AO3401A" V 4276 4175 50  0000 C CNN
F 2 "AO3401A:SOT95P280X125-3N" H 4250 4025 50  0001 L CNN
F 3 "http://www.aosmd.com/pdfs/datasheet/AO3401A.pdf" H 4250 3925 50  0001 L CNN
F 4 "30V P-Channel MOSFET" H 4250 3825 50  0001 L CNN "Description"
F 5 "1.25" H 4250 3725 50  0001 L CNN "Height"
F 6 "Alpha & Omega Semiconductors" H 4250 3625 50  0001 L CNN "Manufacturer_Name"
F 7 "AO3401A" H 4250 3525 50  0001 L CNN "Manufacturer_Part_Number"
F 8 "AO3401A" H 4250 3225 50  0001 L CNN "Arrow Part Number"
F 9 "https://www.arrow.com/en/products/ao3401a/alpha-and-omega-semiconductor?region=nac" H 4250 3125 50  0001 L CNN "Arrow Price/Stock"
	1    3800 4075
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4000 3775 4100 3775
$Comp
L Device:D_Schottky_Small D?
U 1 1 619A1000
P 4100 3350
AR Path="/619A1000" Ref="D?"  Part="1" 
AR Path="/619320AD/619A1000" Ref="D?"  Part="1" 
AR Path="/61914EC1/619A1000" Ref="D?"  Part="1" 
AR Path="/6191C2CA/619A1000" Ref="D11"  Part="1" 
F 0 "D11" V 4054 3418 50  0000 L CNN
F 1 "B5819W" V 4145 3418 50  0000 L CNN
F 2 "Diode_SMD:D_SOD-123" V 4100 3350 50  0001 C CNN
F 3 "~" V 4100 3350 50  0001 C CNN
	1    4100 3350
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4100 3125 4100 3250
Wire Wire Line
	4100 3450 4100 3775
$Comp
L power:GND #PWR?
U 1 1 619A3F94
P 3800 4150
AR Path="/619A3F94" Ref="#PWR?"  Part="1" 
AR Path="/619320AD/619A3F94" Ref="#PWR?"  Part="1" 
AR Path="/61914EC1/619A3F94" Ref="#PWR?"  Part="1" 
AR Path="/6191C2CA/619A3F94" Ref="#PWR0116"  Part="1" 
F 0 "#PWR0116" H 3800 3900 50  0001 C CNN
F 1 "GND" H 3805 3977 50  0000 C CNN
F 2 "" H 3800 4150 50  0001 C CNN
F 3 "" H 3800 4150 50  0001 C CNN
	1    3800 4150
	1    0    0    -1  
$EndComp
Wire Wire Line
	3800 4150 3800 4075
Wire Wire Line
	3400 3775 3125 3775
Connection ~ 4100 3775
Wire Wire Line
	4100 3775 4200 3775
$EndSCHEMATC