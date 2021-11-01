EESchema Schematic File Version 4
LIBS:PDM-cache
EELAYER 29 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
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
L PDM-rescue:BTS50010-1TAE-BTS50010-1TAE-PowerBox-rescue U1
U 1 1 5E7C7F46
P 4050 1600
F 0 "U1" H 4325 2415 50  0000 C CNN
F 1 "BTS50010-1TAE" H 4325 2324 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:TO-263-7_TabPin4" H 4050 1600 50  0001 C CNN
F 3 "" H 4050 1600 50  0001 C CNN
	1    4050 1600
	1    0    0    -1  
$EndComp
$Comp
L PDM-rescue:BTS50010-1TAE-BTS50010-1TAE-PowerBox-rescue U2
U 1 1 5E7C8E27
P 4050 2650
F 0 "U2" H 4325 3465 50  0000 C CNN
F 1 "BTS50010-1TAE" H 4325 3374 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:TO-263-7_TabPin4" H 4050 2650 50  0001 C CNN
F 3 "" H 4050 2650 50  0001 C CNN
	1    4050 2650
	1    0    0    -1  
$EndComp
$Comp
L PDM-rescue:BTS50010-1TAE-BTS50010-1TAE-PowerBox-rescue U3
U 1 1 5E7C95F3
P 4050 3750
F 0 "U3" H 4325 4565 50  0000 C CNN
F 1 "BTS50010-1TAE" H 4325 4474 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:TO-263-7_TabPin4" H 4050 3750 50  0001 C CNN
F 3 "" H 4050 3750 50  0001 C CNN
	1    4050 3750
	1    0    0    -1  
$EndComp
Wire Wire Line
	4700 1300 4750 1300
Wire Wire Line
	4700 1400 4750 1400
Wire Wire Line
	4750 1400 4750 1300
Wire Wire Line
	4700 1500 4750 1500
Wire Wire Line
	4750 1500 4750 1400
Connection ~ 4750 1400
Wire Wire Line
	4700 2350 4750 2350
Wire Wire Line
	4700 2450 4750 2450
Wire Wire Line
	4750 2450 4750 2350
Wire Wire Line
	4700 2550 4750 2550
Wire Wire Line
	4750 2550 4750 2450
Connection ~ 4750 2450
Wire Wire Line
	4700 3550 4750 3550
Wire Wire Line
	4750 3550 4750 3450
Wire Wire Line
	4700 3650 4750 3650
Wire Wire Line
	4750 3650 4750 3550
Connection ~ 4750 3550
NoConn ~ 3950 1500
NoConn ~ 3950 2550
NoConn ~ 3950 3650
$Comp
L Mechanical:MountingHole H1
U 1 1 5E81F429
P 8500 1200
F 0 "H1" H 8600 1246 50  0000 L CNN
F 1 "MountingHole" H 8600 1155 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_DIN965_Pad" H 8500 1200 50  0001 C CNN
F 3 "~" H 8500 1200 50  0001 C CNN
	1    8500 1200
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H2
U 1 1 5E81FE46
P 8500 1450
F 0 "H2" H 8600 1496 50  0000 L CNN
F 1 "MountingHole" H 8600 1405 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_DIN965_Pad" H 8500 1450 50  0001 C CNN
F 3 "~" H 8500 1450 50  0001 C CNN
	1    8500 1450
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H3
U 1 1 5E81FE84
P 8500 1700
F 0 "H3" H 8600 1746 50  0000 L CNN
F 1 "MountingHole" H 8600 1655 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_DIN965_Pad" H 8500 1700 50  0001 C CNN
F 3 "~" H 8500 1700 50  0001 C CNN
	1    8500 1700
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H4
U 1 1 5E820201
P 8500 2000
F 0 "H4" H 8600 2046 50  0000 L CNN
F 1 "MountingHole" H 8600 1955 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_DIN965_Pad" H 8500 2000 50  0001 C CNN
F 3 "~" H 8500 2000 50  0001 C CNN
	1    8500 2000
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x01_Male 12V1
U 1 1 5E82AE14
P 5750 1300
F 0 "12V1" H 5722 1232 50  0000 R CNN
F 1 "85 K1 (starter)" H 5722 1323 50  0000 R CNN
F 2 "MountingHole:MountingHole_2.5mm_Pad_Via" H 5750 1300 50  0001 C CNN
F 3 "~" H 5750 1300 50  0001 C CNN
	1    5750 1300
	-1   0    0    1   
$EndComp
$Comp
L Connector:Conn_01x01_Male FANS1
U 1 1 5E82B442
P 5750 2350
F 0 "FANS1" H 5722 2282 50  0000 R CNN
F 1 "FANS" H 5722 2373 50  0000 R CNN
F 2 "MountingHole:MountingHole_2.5mm_Pad_Via" H 5750 2350 50  0001 C CNN
F 3 "~" H 5750 2350 50  0001 C CNN
	1    5750 2350
	-1   0    0    1   
$EndComp
$Comp
L Connector:Conn_01x01_Male F.PUMP1
U 1 1 5E82BC52
P 5700 3450
F 0 "F.PUMP1" H 5672 3382 50  0000 R CNN
F 1 "FUEL PUMP" H 5672 3473 50  0000 R CNN
F 2 "MountingHole:MountingHole_2.5mm_Pad_Via" H 5700 3450 50  0001 C CNN
F 3 "~" H 5700 3450 50  0001 C CNN
	1    5700 3450
	-1   0    0    1   
$EndComp
$Comp
L Connector:Conn_01x01_Male J1
U 1 1 5E837101
P 2200 900
F 0 "J1" H 2308 1081 50  0000 C CNN
F 1 "12V BAT" H 2308 990 50  0000 C CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_Pad_Via" H 2200 900 50  0001 C CNN
F 3 "~" H 2200 900 50  0001 C CNN
	1    2200 900 
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x01_Male J16
U 1 1 5E84B2D0
P 10250 2150
F 0 "J16" H 10222 2082 50  0000 R CNN
F 1 "GND" H 10222 2173 50  0000 R CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_Pad_Via" H 10250 2150 50  0001 C CNN
F 3 "~" H 10250 2150 50  0001 C CNN
	1    10250 2150
	-1   0    0    1   
$EndComp
Wire Wire Line
	10050 2150 9900 2150
Wire Wire Line
	9900 2150 9900 2250
$Comp
L Device:C C5
U 1 1 5F71D31C
P 5050 1450
F 0 "C5" H 5165 1496 50  0000 L CNN
F 1 "10nF" H 5165 1405 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 5088 1300 50  0001 C CNN
F 3 "~" H 5050 1450 50  0001 C CNN
	1    5050 1450
	1    0    0    -1  
$EndComp
Connection ~ 4750 1300
$Comp
L power:GND #PWR02
U 1 1 5E84C151
P 9900 2250
F 0 "#PWR02" H 9900 2000 50  0001 C CNN
F 1 "GND" H 9905 2077 50  0000 C CNN
F 2 "" H 9900 2250 50  0001 C CNN
F 3 "" H 9900 2250 50  0001 C CNN
	1    9900 2250
	1    0    0    -1  
$EndComp
Wire Wire Line
	4750 1300 5050 1300
Wire Wire Line
	5050 1300 5550 1300
Connection ~ 5050 1300
Wire Wire Line
	3950 900  3950 1100
Connection ~ 2850 900 
$Comp
L power:GND #PWR03
U 1 1 5F73F217
P 2850 1500
F 0 "#PWR03" H 2850 1250 50  0001 C CNN
F 1 "GND" H 2855 1327 50  0000 C CNN
F 2 "" H 2850 1500 50  0001 C CNN
F 3 "" H 2850 1500 50  0001 C CNN
	1    2850 1500
	1    0    0    -1  
$EndComp
$Comp
L Device:C C2
U 1 1 5F71CA19
P 2850 1050
F 0 "C2" H 2965 1096 50  0000 L CNN
F 1 "100nF" H 2965 1005 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 2888 900 50  0001 C CNN
F 3 "~" H 2850 1050 50  0001 C CNN
	1    2850 1050
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x01_Male J4
U 1 1 5E82BF12
P 5700 4350
F 0 "J4" H 5672 4282 50  0000 R CNN
F 1 "BACK LIGHT" H 5672 4373 50  0000 R CNN
F 2 "MountingHole:MountingHole_2.2mm_M2_DIN965_Pad" H 5700 4350 50  0001 C CNN
F 3 "~" H 5700 4350 50  0001 C CNN
	1    5700 4350
	-1   0    0    1   
$EndComp
$Comp
L Connector:Conn_01x01_Male J11
U 1 1 5E82D17F
P 5700 6450
F 0 "J11" H 5672 6382 50  0000 R CNN
F 1 "EMU" H 5672 6473 50  0000 R CNN
F 2 "MountingHole:MountingHole_2.2mm_M2_DIN965_Pad" H 5700 6450 50  0001 C CNN
F 3 "~" H 5700 6450 50  0001 C CNN
	1    5700 6450
	-1   0    0    1   
$EndComp
$Comp
L Connector:Conn_01x01_Male J10
U 1 1 5E82CEF7
P 5700 6150
F 0 "J10" H 5672 6082 50  0000 R CNN
F 1 "RASPBERRY PI" H 5672 6173 50  0000 R CNN
F 2 "MountingHole:MountingHole_2.2mm_M2_DIN965_Pad" H 5700 6150 50  0001 C CNN
F 3 "~" H 5700 6150 50  0001 C CNN
	1    5700 6150
	-1   0    0    1   
$EndComp
$Comp
L Connector:Conn_01x01_Male J9
U 1 1 5E82CB69
P 5700 5850
F 0 "J9" H 5672 5782 50  0000 R CNN
F 1 "STM BOX" H 5672 5873 50  0000 R CNN
F 2 "MountingHole:MountingHole_2.2mm_M2_DIN965_Pad" H 5700 5850 50  0001 C CNN
F 3 "~" H 5700 5850 50  0001 C CNN
	1    5700 5850
	-1   0    0    1   
$EndComp
$Comp
L Connector:Conn_01x01_Male J8
U 1 1 5E82C973
P 5700 5550
F 0 "J8" H 5672 5482 50  0000 R CNN
F 1 "RACE TCS" H 5672 5573 50  0000 R CNN
F 2 "MountingHole:MountingHole_2.2mm_M2_DIN965_Pad" H 5700 5550 50  0001 C CNN
F 3 "~" H 5700 5550 50  0001 C CNN
	1    5700 5550
	-1   0    0    1   
$EndComp
$Comp
L Connector:Conn_01x01_Male J7
U 1 1 5E82C782
P 5700 5250
F 0 "J7" H 5672 5182 50  0000 R CNN
F 1 "LSU 4.9" H 5672 5273 50  0000 R CNN
F 2 "MountingHole:MountingHole_2.2mm_M2_DIN965_Pad" H 5700 5250 50  0001 C CNN
F 3 "~" H 5700 5250 50  0001 C CNN
	1    5700 5250
	-1   0    0    1   
$EndComp
$Comp
L Connector:Conn_01x01_Male J6
U 1 1 5E82C582
P 5700 4950
F 0 "J6" H 5672 4882 50  0000 R CNN
F 1 "INJECTORS" H 5672 4973 50  0000 R CNN
F 2 "MountingHole:MountingHole_2.2mm_M2_DIN965_Pad" H 5700 4950 50  0001 C CNN
F 3 "~" H 5700 4950 50  0001 C CNN
	1    5700 4950
	-1   0    0    1   
$EndComp
$Comp
L Connector:Conn_01x01_Male J5
U 1 1 5E82C272
P 5700 4650
F 0 "J5" H 5672 4582 50  0000 R CNN
F 1 "COILS" H 5672 4673 50  0000 R CNN
F 2 "MountingHole:MountingHole_2.2mm_M2_DIN965_Pad" H 5700 4650 50  0001 C CNN
F 3 "~" H 5700 4650 50  0001 C CNN
	1    5700 4650
	-1   0    0    1   
$EndComp
Wire Wire Line
	3400 4350 3400 4650
Wire Wire Line
	4150 4350 3400 4350
Wire Wire Line
	3400 4650 3400 4950
Connection ~ 3400 4650
Wire Wire Line
	4150 4650 3400 4650
Wire Wire Line
	3400 4950 3400 5250
Connection ~ 3400 4950
Wire Wire Line
	4150 4950 3400 4950
Wire Wire Line
	3400 5250 3400 5550
Connection ~ 3400 5250
Wire Wire Line
	4150 5250 3400 5250
Wire Wire Line
	3400 5550 3400 5850
Connection ~ 3400 5550
Wire Wire Line
	4150 5550 3400 5550
Connection ~ 3400 5850
Wire Wire Line
	4150 5850 3400 5850
Connection ~ 3400 6150
Wire Wire Line
	4150 6150 3400 6150
Connection ~ 3400 6450
Wire Wire Line
	3400 6450 4150 6450
Wire Wire Line
	3400 6150 3400 6450
Wire Wire Line
	1900 6450 1900 6550
Wire Wire Line
	2000 6450 1900 6450
$Comp
L power:GND #PWR01
U 1 1 5E7EEEBF
P 1900 6550
F 0 "#PWR01" H 1900 6300 50  0001 C CNN
F 1 "GND" H 1905 6377 50  0000 C CNN
F 2 "" H 1900 6550 50  0001 C CNN
F 3 "" H 1900 6550 50  0001 C CNN
	1    1900 6550
	1    0    0    -1  
$EndComp
Wire Wire Line
	2650 6450 2300 6450
Wire Wire Line
	3400 6450 2950 6450
Wire Wire Line
	4450 6450 5500 6450
Wire Wire Line
	4450 6150 5500 6150
Wire Wire Line
	4450 5850 5500 5850
Wire Wire Line
	4450 5550 5500 5550
Wire Wire Line
	4450 5250 5500 5250
Wire Wire Line
	4450 4950 5500 4950
Wire Wire Line
	4450 4650 5500 4650
Wire Wire Line
	5500 4350 4450 4350
$Comp
L Device:Fuse F9
U 1 1 5E7DDF06
P 4300 6450
F 0 "F9" V 4103 6450 50  0000 C CNN
F 1 "3" V 4194 6450 50  0000 C CNN
F 2 "Fuse:Fuse_1812_4532Metric_Pad1.30x3.40mm_HandSolder" V 4230 6450 50  0001 C CNN
F 3 "~" H 4300 6450 50  0001 C CNN
	1    4300 6450
	0    1    1    0   
$EndComp
$Comp
L Device:Fuse F8
U 1 1 5E7DDF00
P 4300 6150
F 0 "F8" V 4103 6150 50  0000 C CNN
F 1 "5" V 4194 6150 50  0000 C CNN
F 2 "Fuse:Fuse_1812_4532Metric_Pad1.30x3.40mm_HandSolder" V 4230 6150 50  0001 C CNN
F 3 "~" H 4300 6150 50  0001 C CNN
	1    4300 6150
	0    1    1    0   
$EndComp
$Comp
L Diode:1N4448 D1
U 1 1 5E7D9320
P 3400 6000
F 0 "D1" V 3446 5921 50  0000 R CNN
F 1 "D" V 3355 5921 50  0000 R CNN
F 2 "Diode_SMD:D_SMC_Handsoldering" H 3400 6000 50  0001 C CNN
F 3 "~" H 3400 6000 50  0001 C CNN
	1    3400 6000
	0    -1   -1   0   
$EndComp
$Comp
L Device:C C1
U 1 1 5E7D853B
P 2150 6450
F 0 "C1" V 1898 6450 50  0000 C CNN
F 1 "0.2F" V 1989 6450 50  0000 C CNN
F 2 "Kondensator:Kondensator" H 2188 6300 50  0001 C CNN
F 3 "~" H 2150 6450 50  0001 C CNN
	1    2150 6450
	0    1    1    0   
$EndComp
$Comp
L Device:R R1
U 1 1 5E7D7D8D
P 2800 6450
F 0 "R1" V 2593 6450 50  0000 C CNN
F 1 "0.1" V 2684 6450 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 2730 6450 50  0001 C CNN
F 3 "~" H 2800 6450 50  0001 C CNN
	1    2800 6450
	0    1    1    0   
$EndComp
$Comp
L Device:Fuse F7
U 1 1 5E7CBEB7
P 4300 5850
F 0 "F7" V 4103 5850 50  0000 C CNN
F 1 "7.5" V 4194 5850 50  0000 C CNN
F 2 "Fuse:Fuse_1812_4532Metric_Pad1.30x3.40mm_HandSolder" V 4230 5850 50  0001 C CNN
F 3 "~" H 4300 5850 50  0001 C CNN
	1    4300 5850
	0    1    1    0   
$EndComp
$Comp
L Device:Fuse F6
U 1 1 5E7CB988
P 4300 5550
F 0 "F6" V 4103 5550 50  0000 C CNN
F 1 "Fuse" V 4194 5550 50  0000 C CNN
F 2 "Fuse:Fuse_1812_4532Metric_Pad1.30x3.40mm_HandSolder" V 4230 5550 50  0001 C CNN
F 3 "~" H 4300 5550 50  0001 C CNN
	1    4300 5550
	0    1    1    0   
$EndComp
$Comp
L Device:Fuse F5
U 1 1 5E7CB578
P 4300 5250
F 0 "F5" V 4103 5250 50  0000 C CNN
F 1 "3" V 4194 5250 50  0000 C CNN
F 2 "Fuse:Fuse_1812_4532Metric_Pad1.30x3.40mm_HandSolder" V 4230 5250 50  0001 C CNN
F 3 "~" H 4300 5250 50  0001 C CNN
	1    4300 5250
	0    1    1    0   
$EndComp
$Comp
L Device:Fuse F4
U 1 1 5E7CB00D
P 4300 4950
F 0 "F4" V 4103 4950 50  0000 C CNN
F 1 "15" V 4194 4950 50  0000 C CNN
F 2 "Fuse:Fuse_1812_4532Metric_Pad1.30x3.40mm_HandSolder" V 4230 4950 50  0001 C CNN
F 3 "~" H 4300 4950 50  0001 C CNN
	1    4300 4950
	0    1    1    0   
$EndComp
$Comp
L Device:Fuse F3
U 1 1 5E7CAE1B
P 4300 4650
F 0 "F3" V 4103 4650 50  0000 C CNN
F 1 "15" V 4194 4650 50  0000 C CNN
F 2 "Fuse:Fuse_1812_4532Metric_Pad1.30x3.40mm_HandSolder" V 4230 4650 50  0001 C CNN
F 3 "~" H 4300 4650 50  0001 C CNN
	1    4300 4650
	0    1    1    0   
$EndComp
$Comp
L Device:Fuse F2
U 1 1 5E7CAA9B
P 4300 4350
F 0 "F2" V 4103 4350 50  0000 C CNN
F 1 "2" V 4194 4350 50  0000 C CNN
F 2 "Fuse:Fuse_1812_4532Metric_Pad1.30x3.40mm_HandSolder" V 4230 4350 50  0001 C CNN
F 3 "~" H 4300 4350 50  0001 C CNN
	1    4300 4350
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR04
U 1 1 5F798B9C
P 2950 2700
F 0 "#PWR04" H 2950 2450 50  0001 C CNN
F 1 "GND" H 2955 2527 50  0000 C CNN
F 2 "" H 2950 2700 50  0001 C CNN
F 3 "" H 2950 2700 50  0001 C CNN
	1    2950 2700
	1    0    0    -1  
$EndComp
$Comp
L Device:C C3
U 1 1 5F798BA3
P 2950 2300
F 0 "C3" H 3065 2346 50  0000 L CNN
F 1 "100nF" H 3065 2255 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 2988 2150 50  0001 C CNN
F 3 "~" H 2950 2300 50  0001 C CNN
	1    2950 2300
	1    0    0    -1  
$EndComp
Wire Wire Line
	2600 2150 2950 2150
Connection ~ 2950 2150
$Comp
L power:GND #PWR05
U 1 1 5F7B5D4C
P 2950 3800
F 0 "#PWR05" H 2950 3550 50  0001 C CNN
F 1 "GND" H 2955 3627 50  0000 C CNN
F 2 "" H 2950 3800 50  0001 C CNN
F 3 "" H 2950 3800 50  0001 C CNN
	1    2950 3800
	1    0    0    -1  
$EndComp
$Comp
L Device:C C4
U 1 1 5F7B5D52
P 2950 3400
F 0 "C4" H 3065 3446 50  0000 L CNN
F 1 "100nF" H 3065 3355 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 2988 3250 50  0001 C CNN
F 3 "~" H 2950 3400 50  0001 C CNN
	1    2950 3400
	1    0    0    -1  
$EndComp
Connection ~ 2950 3250
$Comp
L Device:C C10
U 1 1 5F7C3B08
P 5050 2500
F 0 "C10" H 5165 2546 50  0000 L CNN
F 1 "10nF" H 5165 2455 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 5088 2350 50  0001 C CNN
F 3 "~" H 5050 2500 50  0001 C CNN
	1    5050 2500
	1    0    0    -1  
$EndComp
Wire Wire Line
	4750 2350 5050 2350
Wire Wire Line
	5050 2350 5550 2350
Connection ~ 5050 2350
$Comp
L power:GND #PWR011
U 1 1 5F7C3B19
P 5050 2650
F 0 "#PWR011" H 5050 2400 50  0001 C CNN
F 1 "GND" H 5055 2477 50  0000 C CNN
F 2 "" H 5050 2650 50  0001 C CNN
F 3 "" H 5050 2650 50  0001 C CNN
	1    5050 2650
	1    0    0    -1  
$EndComp
$Comp
L Device:C C9
U 1 1 5F7CA8AE
P 5000 3600
F 0 "C9" H 5115 3646 50  0000 L CNN
F 1 "10nF" H 5115 3555 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 5038 3450 50  0001 C CNN
F 3 "~" H 5000 3600 50  0001 C CNN
	1    5000 3600
	1    0    0    -1  
$EndComp
Wire Wire Line
	5000 3450 5500 3450
Connection ~ 5000 3450
$Comp
L power:GND #PWR09
U 1 1 5F7CA8BF
P 5000 3750
F 0 "#PWR09" H 5000 3500 50  0001 C CNN
F 1 "GND" H 5005 3577 50  0000 C CNN
F 2 "" H 5000 3750 50  0001 C CNN
F 3 "" H 5000 3750 50  0001 C CNN
	1    5000 3750
	1    0    0    -1  
$EndComp
Wire Wire Line
	4700 3450 4750 3450
Connection ~ 4750 3450
Wire Wire Line
	4750 3450 5000 3450
$Comp
L Device:D_TVS_ALT D2
U 1 1 5F75E37C
P 2500 1100
F 0 "D2" V 2450 850 50  0000 L CNN
F 1 "D_TVS_ALT" V 2550 700 50  0000 L CNN
F 2 "Diode_SMD:D_SMA_Handsoldering" H 2500 1100 50  0001 C CNN
F 3 "~" H 2500 1100 50  0001 C CNN
	1    2500 1100
	0    1    1    0   
$EndComp
$Comp
L Device:R R11
U 1 1 5F75F4FC
P 2700 1400
F 0 "R11" V 2493 1400 50  0000 C CNN
F 1 "1R" V 2584 1400 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 2630 1400 50  0001 C CNN
F 3 "~" H 2700 1400 50  0001 C CNN
	1    2700 1400
	0    1    1    0   
$EndComp
Wire Wire Line
	2500 950  2500 900 
Wire Wire Line
	2500 900  2850 900 
Wire Wire Line
	2500 900  2400 900 
Connection ~ 2500 900 
Wire Wire Line
	2500 1250 2500 1400
Wire Wire Line
	2500 1400 2550 1400
Wire Wire Line
	2850 1400 2850 1200
Wire Wire Line
	2850 1400 2850 1500
Connection ~ 2850 1400
$Comp
L Device:D_TVS_ALT D3
U 1 1 5F773F05
P 2600 2300
F 0 "D3" V 2550 2150 50  0000 L CNN
F 1 "D_TVS_ALT" V 2650 1850 50  0000 L CNN
F 2 "Diode_SMD:D_SMA_Handsoldering" H 2600 2300 50  0001 C CNN
F 3 "~" H 2600 2300 50  0001 C CNN
	1    2600 2300
	0    1    1    0   
$EndComp
$Comp
L Device:R R12
U 1 1 5F773F0B
P 2800 2600
F 0 "R12" V 2593 2600 50  0000 C CNN
F 1 "1R" V 2684 2600 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 2730 2600 50  0001 C CNN
F 3 "~" H 2800 2600 50  0001 C CNN
	1    2800 2600
	0    1    1    0   
$EndComp
Wire Wire Line
	2600 2450 2600 2600
Wire Wire Line
	2600 2600 2650 2600
Wire Wire Line
	2950 2450 2950 2600
Wire Wire Line
	2950 2700 2950 2600
Connection ~ 2950 2600
$Comp
L Device:D_TVS_ALT D4
U 1 1 5F78D97F
P 2600 3450
F 0 "D4" V 2550 3300 50  0000 L CNN
F 1 "D_TVS_ALT" V 2650 3000 50  0000 L CNN
F 2 "Diode_SMD:D_SMA_Handsoldering" H 2600 3450 50  0001 C CNN
F 3 "~" H 2600 3450 50  0001 C CNN
	1    2600 3450
	0    1    1    0   
$EndComp
$Comp
L Device:R R13
U 1 1 5F78D985
P 2800 3700
F 0 "R13" V 2593 3700 50  0000 C CNN
F 1 "1R" V 2684 3700 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 2730 3700 50  0001 C CNN
F 3 "~" H 2800 3700 50  0001 C CNN
	1    2800 3700
	0    1    1    0   
$EndComp
Wire Wire Line
	2600 3700 2650 3700
Wire Wire Line
	2950 3550 2950 3700
Wire Wire Line
	2950 3700 2950 3800
Connection ~ 2950 3700
Wire Wire Line
	2600 3300 2600 3250
Connection ~ 2600 3250
Wire Wire Line
	2600 3250 2950 3250
Wire Wire Line
	2600 3700 2600 3600
Wire Wire Line
	2950 3250 3350 3250
Wire Wire Line
	3350 3450 3350 3250
Wire Wire Line
	2950 2150 3350 2150
$Comp
L power:GND #PWR0101
U 1 1 60A96359
P 5550 1100
F 0 "#PWR0101" H 5550 850 50  0001 C CNN
F 1 "GND" H 5555 927 50  0000 C CNN
F 2 "" H 5550 1100 50  0001 C CNN
F 3 "" H 5550 1100 50  0001 C CNN
	1    5550 1100
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4700 1100 5550 1100
Wire Wire Line
	4700 2150 5550 2150
$Comp
L power:GND #PWR0102
U 1 1 60B00421
P 5050 1600
F 0 "#PWR0102" H 5050 1350 50  0001 C CNN
F 1 "GND" H 5055 1427 50  0000 C CNN
F 2 "" H 5050 1600 50  0001 C CNN
F 3 "" H 5050 1600 50  0001 C CNN
	1    5050 1600
	1    0    0    -1  
$EndComp
Wire Wire Line
	2850 900  3350 900 
Text GLabel 3350 1000 3    50   Input ~ 0
12V_BAT
Wire Wire Line
	3350 1000 3350 900 
Text GLabel 3700 1300 0    50   Input ~ 0
IN_1
Text GLabel 3350 2350 3    50   Input ~ 0
12V_OUT1
Text GLabel 3350 3450 3    50   Input ~ 0
12V_OUT2
Wire Wire Line
	3350 2150 3350 2350
Wire Wire Line
	3950 900  3350 900 
Connection ~ 3350 900 
Wire Wire Line
	2600 2150 2600 1750
Wire Wire Line
	2600 1750 4750 1750
Wire Wire Line
	4750 1750 4750 1500
Connection ~ 2600 2150
Connection ~ 4750 1500
Wire Wire Line
	4700 3250 5500 3250
Text GLabel 3700 2350 0    50   Input ~ 0
IN_2
Wire Wire Line
	3950 2150 3350 2150
Connection ~ 3350 2150
Wire Wire Line
	3350 3250 3950 3250
Connection ~ 3350 3250
Wire Wire Line
	2100 3250 2100 1750
Wire Wire Line
	2100 1750 2600 1750
Wire Wire Line
	2100 3250 2600 3250
Connection ~ 2600 1750
Text GLabel 3700 3450 0    50   Input ~ 0
IN_3
Wire Wire Line
	2100 3250 2100 4350
Wire Wire Line
	2100 4350 3400 4350
Connection ~ 2100 3250
Connection ~ 3400 4350
$Comp
L power:GND #PWR0106
U 1 1 60BB3260
P 5550 2150
F 0 "#PWR0106" H 5550 1900 50  0001 C CNN
F 1 "GND" H 5555 1977 50  0000 C CNN
F 2 "" H 5550 2150 50  0001 C CNN
F 3 "" H 5550 2150 50  0001 C CNN
	1    5550 2150
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR0107
U 1 1 60BB39FA
P 5500 3250
F 0 "#PWR0107" H 5500 3000 50  0001 C CNN
F 1 "GND" H 5505 3077 50  0000 C CNN
F 2 "" H 5500 3250 50  0001 C CNN
F 3 "" H 5500 3250 50  0001 C CNN
	1    5500 3250
	0    -1   -1   0   
$EndComp
$Comp
L Regulator_Linear:LM2937xMP U4
U 1 1 6174E72E
P 7450 2050
F 0 "U4" H 7450 2292 50  0000 C CNN
F 1 "LM2937xMP" H 7450 2201 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-223-3_TabPin2" H 7450 2275 50  0001 C CIN
F 3 "http://www.ti.com/lit/ds/symlink/lm2937.pdf" H 7450 2000 50  0001 C CNN
	1    7450 2050
	1    0    0    -1  
$EndComp
Text GLabel 6950 2050 0    50   Input ~ 0
12V_BAT
$Comp
L Device:C_Small C6
U 1 1 6174FF26
P 7100 2150
F 0 "C6" H 6900 2150 50  0000 L CNN
F 1 "0.1uF" H 6850 2050 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 7100 2150 50  0001 C CNN
F 3 "~" H 7100 2150 50  0001 C CNN
	1    7100 2150
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C7
U 1 1 61750E9F
P 7850 2150
F 0 "C7" H 7942 2196 50  0000 L CNN
F 1 "10uF" H 7942 2105 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 7850 2150 50  0001 C CNN
F 3 "~" H 7850 2150 50  0001 C CNN
	1    7850 2150
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0104
U 1 1 6175134E
P 7450 2350
F 0 "#PWR0104" H 7450 2100 50  0001 C CNN
F 1 "GND" H 7455 2177 50  0000 C CNN
F 2 "" H 7450 2350 50  0001 C CNN
F 3 "" H 7450 2350 50  0001 C CNN
	1    7450 2350
	1    0    0    -1  
$EndComp
Wire Wire Line
	7100 2250 7100 2350
Wire Wire Line
	7100 2350 7450 2350
Connection ~ 7450 2350
Wire Wire Line
	7450 2350 7850 2350
Wire Wire Line
	7850 2350 7850 2250
Wire Wire Line
	7100 2050 7150 2050
Wire Wire Line
	7750 2050 7850 2050
$Comp
L power:+3.3V #PWR0105
U 1 1 61776A92
P 7850 2050
F 0 "#PWR0105" H 7850 1900 50  0001 C CNN
F 1 "+3.3V" H 7865 2223 50  0000 C CNN
F 2 "" H 7850 2050 50  0001 C CNN
F 3 "" H 7850 2050 50  0001 C CNN
	1    7850 2050
	1    0    0    -1  
$EndComp
Connection ~ 7850 2050
Wire Wire Line
	6950 2050 7100 2050
Connection ~ 7100 2050
$Comp
L SN74LVC1GU04DBVT:SN74LVC1GU04DBVT IC1
U 1 1 61790D75
P 7400 3100
F 0 "IC1" H 7950 3365 50  0000 C CNN
F 1 "SN74LVC1GU04DBVT" H 7950 3274 50  0000 C CNN
F 2 "SN74LVC1GU04DBVT:SOT95P280X145-5N" H 8350 3200 50  0001 L CNN
F 3 "http://www.ti.com/lit/ds/symlink/sn74lvc1gu04.pdf" H 8350 3100 50  0001 L CNN
F 4 "74LVC Single Unbuffered Inverter SOT23 Texas Instruments SN74LVC1GU04DBVT CMOS Inverter, 1.65  5.5 V SOT-23, 5-Pin" H 8350 3000 50  0001 L CNN "Description"
F 5 "1.45" H 8350 2900 50  0001 L CNN "Height"
F 6 "Texas Instruments" H 8350 2800 50  0001 L CNN "Manufacturer_Name"
F 7 "SN74LVC1GU04DBVT" H 8350 2700 50  0001 L CNN "Manufacturer_Part_Number"
F 8 "595-SN74LVC1GU04DBVT" H 8350 2600 50  0001 L CNN "Mouser Part Number"
F 9 "https://www.mouser.co.uk/ProductDetail/Texas-Instruments/SN74LVC1GU04DBVT/?qs=dT9u2OTAaVXa%252BrelilPp1g%3D%3D" H 8350 2500 50  0001 L CNN "Mouser Price/Stock"
F 10 "SN74LVC1GU04DBVT" H 8350 2400 50  0001 L CNN "Arrow Part Number"
F 11 "https://www.arrow.com/en/products/sn74lvc1gu04dbvt/texas-instruments?region=nac" H 8350 2300 50  0001 L CNN "Arrow Price/Stock"
	1    7400 3100
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0108
U 1 1 61791BFF
P 8650 3200
F 0 "#PWR0108" H 8650 3050 50  0001 C CNN
F 1 "+3.3V" H 8665 3373 50  0000 C CNN
F 2 "" H 8650 3200 50  0001 C CNN
F 3 "" H 8650 3200 50  0001 C CNN
	1    8650 3200
	-1   0    0    1   
$EndComp
Wire Wire Line
	8650 3200 8500 3200
$Comp
L power:GND #PWR0109
U 1 1 617972FC
P 7400 3300
F 0 "#PWR0109" H 7400 3050 50  0001 C CNN
F 1 "GND" H 7405 3127 50  0000 C CNN
F 2 "" H 7400 3300 50  0001 C CNN
F 3 "" H 7400 3300 50  0001 C CNN
	1    7400 3300
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0110
U 1 1 61797C2B
P 7150 2850
F 0 "#PWR0110" H 7150 2700 50  0001 C CNN
F 1 "+3.3V" H 7165 3023 50  0000 C CNN
F 2 "" H 7150 2850 50  0001 C CNN
F 3 "" H 7150 2850 50  0001 C CNN
	1    7150 2850
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R6
U 1 1 61798589
P 7150 3050
F 0 "R6" H 6950 3100 50  0000 L CNN
F 1 "100" H 6900 3000 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" H 7150 3050 50  0001 C CNN
F 3 "~" H 7150 3050 50  0001 C CNN
	1    7150 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	7150 2850 7150 2950
$Comp
L Connector:Conn_01x01_Male J13
U 1 1 617A163F
P 6950 3350
F 0 "J13" H 6922 3282 50  0000 R CNN
F 1 "SAFETY SYSTEM" H 6922 3373 50  0000 R CNN
F 2 "MountingHole:MountingHole_2.2mm_M2_DIN965_Pad" H 6950 3350 50  0001 C CNN
F 3 "~" H 6950 3350 50  0001 C CNN
	1    6950 3350
	1    0    0    -1  
$EndComp
Text GLabel 8500 3100 2    50   Input ~ 0
IN_1
Wire Wire Line
	7150 3150 7150 3200
Wire Wire Line
	7400 3200 7150 3200
Connection ~ 7150 3200
Wire Wire Line
	7150 3200 7150 3350
Text GLabel 7600 3900 0    50   Input ~ 0
12V_OUT1
$Comp
L Device:R_Small R4
U 1 1 617C64C3
P 8000 3900
F 0 "R4" H 7800 3950 50  0000 L CNN
F 1 "10k" H 7750 3850 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" H 8000 3900 50  0001 C CNN
F 3 "~" H 8000 3900 50  0001 C CNN
	1    8000 3900
	0    1    1    0   
$EndComp
$Comp
L Connector:Conn_01x01_Male J3
U 1 1 617C6D4B
P 7900 4150
F 0 "J3" H 7872 4082 50  0000 R CNN
F 1 "EMU PIN 13 GN (AUX2)" H 7872 4173 50  0000 R CNN
F 2 "MountingHole:MountingHole_2.2mm_M2_DIN965_Pad" H 7900 4150 50  0001 C CNN
F 3 "~" H 7900 4150 50  0001 C CNN
	1    7900 4150
	1    0    0    -1  
$EndComp
Wire Wire Line
	7600 3900 7900 3900
Text GLabel 8250 4000 2    50   Input ~ 0
IN_2
Wire Wire Line
	8100 4150 8100 4000
Wire Wire Line
	8250 4000 8100 4000
Connection ~ 8100 4000
Wire Wire Line
	8100 4000 8100 3900
Text GLabel 7500 4600 0    50   Input ~ 0
12V_OUT2
$Comp
L Connector:Conn_01x01_Male J2
U 1 1 617DC9D7
P 7850 4800
F 0 "J2" H 7822 4732 50  0000 R CNN
F 1 "EMU PIN 21 GN(AUX1)" H 7822 4823 50  0000 R CNN
F 2 "MountingHole:MountingHole_2.2mm_M2_DIN965_Pad" H 7850 4800 50  0001 C CNN
F 3 "~" H 7850 4800 50  0001 C CNN
	1    7850 4800
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R3
U 1 1 617DD1EF
P 7950 4600
F 0 "R3" H 7750 4650 50  0000 L CNN
F 1 "10k" H 7700 4550 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" H 7950 4600 50  0001 C CNN
F 3 "~" H 7950 4600 50  0001 C CNN
	1    7950 4600
	0    1    1    0   
$EndComp
Wire Wire Line
	7500 4600 7850 4600
Wire Wire Line
	8050 4800 8050 4700
Text GLabel 8200 4700 2    50   Input ~ 0
IN_3
Wire Wire Line
	8050 4700 8200 4700
Connection ~ 8050 4700
Wire Wire Line
	8050 4700 8050 4600
$Comp
L Device:C_Small C12
U 1 1 6186E6D9
P 3800 3650
F 0 "C12" H 3550 3700 50  0000 L CNN
F 1 "150nF" H 3500 3600 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 3800 3650 50  0001 C CNN
F 3 "~" H 3800 3650 50  0001 C CNN
	1    3800 3650
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0103
U 1 1 61877BCF
P 3800 3750
F 0 "#PWR0103" H 3800 3500 50  0001 C CNN
F 1 "GND" H 3805 3577 50  0000 C CNN
F 2 "" H 3800 3750 50  0001 C CNN
F 3 "" H 3800 3750 50  0001 C CNN
	1    3800 3750
	1    0    0    -1  
$EndComp
Wire Wire Line
	3700 3450 3800 3450
Wire Wire Line
	3800 3550 3800 3450
Connection ~ 3800 3450
Wire Wire Line
	3800 3450 3950 3450
$Comp
L Device:C_Small C11
U 1 1 618808BD
P 3800 2550
F 0 "C11" H 3550 2600 50  0000 L CNN
F 1 "150nF" H 3500 2500 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 3800 2550 50  0001 C CNN
F 3 "~" H 3800 2550 50  0001 C CNN
	1    3800 2550
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0111
U 1 1 618808C3
P 3800 2650
F 0 "#PWR0111" H 3800 2400 50  0001 C CNN
F 1 "GND" H 3805 2477 50  0000 C CNN
F 2 "" H 3800 2650 50  0001 C CNN
F 3 "" H 3800 2650 50  0001 C CNN
	1    3800 2650
	1    0    0    -1  
$EndComp
Wire Wire Line
	3800 2450 3800 2350
Wire Wire Line
	3700 2350 3800 2350
Connection ~ 3800 2350
Wire Wire Line
	3800 2350 3950 2350
$Comp
L Device:C_Small C8
U 1 1 6188CDCB
P 3800 1500
F 0 "C8" H 3550 1550 50  0000 L CNN
F 1 "150nF" H 3500 1450 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 3800 1500 50  0001 C CNN
F 3 "~" H 3800 1500 50  0001 C CNN
	1    3800 1500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0112
U 1 1 6188CDD1
P 3800 1600
F 0 "#PWR0112" H 3800 1350 50  0001 C CNN
F 1 "GND" H 3805 1427 50  0000 C CNN
F 2 "" H 3800 1600 50  0001 C CNN
F 3 "" H 3800 1600 50  0001 C CNN
	1    3800 1600
	1    0    0    -1  
$EndComp
Wire Wire Line
	3800 1400 3800 1300
Wire Wire Line
	3700 1300 3800 1300
Connection ~ 3800 1300
Wire Wire Line
	3800 1300 3950 1300
$Comp
L power:+3.3V #PWR06
U 1 1 618A7CAC
P 9300 4000
F 0 "#PWR06" H 9300 3850 50  0001 C CNN
F 1 "+3.3V" H 9315 4173 50  0000 C CNN
F 2 "" H 9300 4000 50  0001 C CNN
F 3 "" H 9300 4000 50  0001 C CNN
	1    9300 4000
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R2
U 1 1 618A8157
P 9300 4200
F 0 "R2" H 9100 4250 50  0000 L CNN
F 1 "100" H 9050 4150 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" H 9300 4200 50  0001 C CNN
F 3 "~" H 9300 4200 50  0001 C CNN
	1    9300 4200
	-1   0    0    1   
$EndComp
Wire Wire Line
	9300 4100 9300 4000
Text GLabel 9300 4900 3    50   Input ~ 0
BRAKE_SWITCH
Text GLabel 9850 4450 2    50   Input ~ 0
BSPD_IN
$Comp
L Device:D_Schottky D5
U 1 1 618AE34E
P 9300 4750
F 0 "D5" V 9346 4671 50  0000 R CNN
F 1 "D_Schottky" V 9255 4671 50  0000 R CNN
F 2 "Diode_SMD:D_SOD-123" H 9300 4750 50  0001 C CNN
F 3 "~" H 9300 4750 50  0001 C CNN
	1    9300 4750
	0    -1   -1   0   
$EndComp
Wire Wire Line
	9300 4300 9300 4450
Wire Wire Line
	9850 4450 9300 4450
Connection ~ 9300 4450
Wire Wire Line
	9300 4450 9300 4600
$Comp
L Connector:Conn_01x02_Male J12
U 1 1 618BD9D7
P 9850 5300
F 0 "J12" H 9958 5481 50  0000 C CNN
F 1 "Conn_01x02_Male" H 9958 5390 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 9850 5300 50  0001 C CNN
F 3 "~" H 9850 5300 50  0001 C CNN
	1    9850 5300
	1    0    0    -1  
$EndComp
Text GLabel 10050 5300 2    50   Input ~ 0
BSPD_IN
Text GLabel 10050 5400 2    50   Input ~ 0
BRAKE_SWITCH
$EndSCHEMATC
