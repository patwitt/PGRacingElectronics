EESchema Schematic File Version 4
LIBS:Starter-cache
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
L Starter-rescue:BTS50010-1TAE-BTS50010-1TAE-PowerBox-rescue U1
U 1 1 5E7C7F46
P 4550 2400
F 0 "U1" H 4825 3215 50  0000 C CNN
F 1 "BTS50010-1TAE" H 4825 3124 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:TO-263-7_TabPin4" H 4550 2400 50  0001 C CNN
F 3 "" H 4550 2400 50  0001 C CNN
	1    4550 2400
	1    0    0    -1  
$EndComp
Wire Wire Line
	5200 2100 5250 2100
Wire Wire Line
	5200 2200 5250 2200
Wire Wire Line
	5250 2200 5250 2100
Wire Wire Line
	5200 2300 5250 2300
Wire Wire Line
	5250 2300 5250 2200
Connection ~ 5250 2200
NoConn ~ 4450 2300
$Comp
L Mechanical:MountingHole H1
U 1 1 5E81F429
P 1800 950
F 0 "H1" H 1900 996 50  0000 L CNN
F 1 "MountingHole" H 1900 905 50  0000 L CNN
F 2 "MountingHole:MountingHole_2.5mm" H 1800 950 50  0001 C CNN
F 3 "~" H 1800 950 50  0001 C CNN
	1    1800 950 
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H3
U 1 1 5E81FE84
P 1800 1200
F 0 "H3" H 1900 1246 50  0000 L CNN
F 1 "MountingHole" H 1900 1155 50  0000 L CNN
F 2 "MountingHole:MountingHole_2.5mm" H 1800 1200 50  0001 C CNN
F 3 "~" H 1800 1200 50  0001 C CNN
	1    1800 1200
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H4
U 1 1 5E820201
P 1800 1500
F 0 "H4" H 1900 1546 50  0000 L CNN
F 1 "MountingHole" H 1900 1455 50  0000 L CNN
F 2 "MountingHole:MountingHole_2.5mm" H 1800 1500 50  0001 C CNN
F 3 "~" H 1800 1500 50  0001 C CNN
	1    1800 1500
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x01_Male J12
U 1 1 5E826B2D
P 5500 1700
F 0 "J12" H 5472 1632 50  0000 R CNN
F 1 "STARTER SIGNAL" H 5472 1723 50  0000 R CNN
F 2 "MountingHole:MountingHole_2.2mm_M2_DIN965_Pad" H 5500 1700 50  0001 C CNN
F 3 "~" H 5500 1700 50  0001 C CNN
	1    5500 1700
	0    1    1    0   
$EndComp
$Comp
L Connector:Conn_01x01_Male J13
U 1 1 5E82AE14
P 6800 2100
F 0 "J13" H 6772 2032 50  0000 R CNN
F 1 "STARTER" H 6772 2123 50  0000 R CNN
F 2 "MountingHole:MountingHole_5.3mm_M5_Pad" H 6800 2100 50  0001 C CNN
F 3 "~" H 6800 2100 50  0001 C CNN
	1    6800 2100
	-1   0    0    1   
$EndComp
$Comp
L Connector:Conn_01x01_Male J1
U 1 1 5E837101
P 6950 650
F 0 "J1" H 7058 831 50  0000 C CNN
F 1 "12V BAT" H 7058 740 50  0000 C CNN
F 2 "MountingHole:MountingHole_5.3mm_M5_Pad" H 6950 650 50  0001 C CNN
F 3 "~" H 6950 650 50  0001 C CNN
	1    6950 650 
	-1   0    0    1   
$EndComp
$Comp
L Connector:Conn_01x01_Male J16
U 1 1 5E84B2D0
P 2350 2050
F 0 "J16" H 2322 1982 50  0000 R CNN
F 1 "GND" H 2322 2073 50  0000 R CNN
F 2 "MountingHole:MountingHole_2.2mm_M2_DIN965_Pad" H 2350 2050 50  0001 C CNN
F 3 "~" H 2350 2050 50  0001 C CNN
	1    2350 2050
	-1   0    0    1   
$EndComp
Wire Wire Line
	2150 2050 2000 2050
Wire Wire Line
	2000 2050 2000 2150
$Comp
L Device:C COUT1
U 1 1 5F71D31C
P 6400 1450
F 0 "COUT1" H 6515 1496 50  0000 L CNN
F 1 "10nF" H 6515 1405 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 6438 1300 50  0001 C CNN
F 3 "~" H 6400 1450 50  0001 C CNN
	1    6400 1450
	1    0    0    -1  
$EndComp
Connection ~ 5250 2100
$Comp
L power:GND #PWR02
U 1 1 5E84C151
P 2000 2150
F 0 "#PWR02" H 2000 1900 50  0001 C CNN
F 1 "GND" H 2005 1977 50  0000 C CNN
F 2 "" H 2000 2150 50  0001 C CNN
F 3 "" H 2000 2150 50  0001 C CNN
	1    2000 2150
	1    0    0    -1  
$EndComp
$Comp
L Device:R R_GND1
U 1 1 5F73FCC2
P 5350 1900
F 0 "R_GND1" V 5143 1900 50  0000 C CNN
F 1 "10R" V 5234 1900 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 5280 1900 50  0001 C CNN
F 3 "~" H 5350 1900 50  0001 C CNN
	1    5350 1900
	0    1    1    0   
$EndComp
Connection ~ 3650 2100
Wire Wire Line
	3500 2100 3650 2100
$Comp
L Device:R RIN_PD1
U 1 1 5F75D50E
P 3350 2100
F 0 "RIN_PD1" V 3557 2100 50  0000 C CNN
F 1 "22kR" V 3466 2100 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 3280 2100 50  0001 C CNN
F 3 "~" H 3350 2100 50  0001 C CNN
	1    3350 2100
	0    -1   -1   0   
$EndComp
$Comp
L Device:R RIN_PU1
U 1 1 5F756386
P 3650 2250
F 0 "RIN_PU1" H 3580 2204 50  0000 R CNN
F 1 "22k" H 3580 2295 50  0000 R CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 3580 2250 50  0001 C CNN
F 3 "~" H 3650 2250 50  0001 C CNN
	1    3650 2250
	-1   0    0    1   
$EndComp
$Comp
L Device:C CIN1
U 1 1 5F755A6D
P 4050 2250
F 0 "CIN1" H 4165 2296 50  0000 L CNN
F 1 "220nF" H 4165 2205 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 4088 2100 50  0001 C CNN
F 3 "~" H 4050 2250 50  0001 C CNN
	1    4050 2250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR03
U 1 1 5F73F217
P 4950 1250
F 0 "#PWR03" H 4950 1000 50  0001 C CNN
F 1 "GND" H 4955 1077 50  0000 C CNN
F 2 "" H 4950 1250 50  0001 C CNN
F 3 "" H 4950 1250 50  0001 C CNN
	1    4950 1250
	1    0    0    -1  
$EndComp
$Comp
L Device:C C_VS1
U 1 1 5F71CA19
P 4950 800
F 0 "C_VS1" H 5065 846 50  0000 L CNN
F 1 "100nF" H 5065 755 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 4988 650 50  0001 C CNN
F 3 "~" H 4950 800 50  0001 C CNN
	1    4950 800 
	1    0    0    -1  
$EndComp
$Comp
L Device:D_TVS_ALT D1
U 1 1 5F75E37C
P 4600 850
F 0 "D1" V 4550 600 50  0000 L CNN
F 1 "D_TVS_ALT" V 4650 450 50  0000 L CNN
F 2 "Diode_SMD:D_SMA_Handsoldering" H 4600 850 50  0001 C CNN
F 3 "~" H 4600 850 50  0001 C CNN
	1    4600 850 
	0    1    1    0   
$EndComp
$Comp
L Device:R R_GND2
U 1 1 5F75F4FC
P 4800 1150
F 0 "R_GND2" V 4593 1150 50  0000 C CNN
F 1 "1R" V 4684 1150 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 4730 1150 50  0001 C CNN
F 3 "~" H 4800 1150 50  0001 C CNN
	1    4800 1150
	0    1    1    0   
$EndComp
Wire Wire Line
	4600 1000 4600 1150
Wire Wire Line
	4600 1150 4650 1150
Wire Wire Line
	4950 1150 4950 950 
Wire Wire Line
	4950 1150 4950 1250
Connection ~ 4950 1150
$Comp
L Simulation_SPICE:DIODE D2
U 1 1 608A8651
P 5950 1450
F 0 "D2" V 5996 1370 50  0000 R CNN
F 1 "DIODE" V 5905 1370 50  0000 R CNN
F 2 "Diode_SMD:D_SMC" H 5950 1450 50  0001 C CNN
F 3 "~" H 5950 1450 50  0001 C CNN
F 4 "Y" H 5950 1450 50  0001 L CNN "Spice_Netlist_Enabled"
F 5 "V" H 5950 1450 50  0001 L CNN "Spice_Primitive"
	1    5950 1450
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4950 650  5950 650 
Wire Wire Line
	4950 650  4600 650 
Wire Wire Line
	4600 650  4600 700 
Connection ~ 4950 650 
Wire Wire Line
	4600 650  4100 650 
Wire Wire Line
	4100 1900 4450 1900
Connection ~ 4600 650 
$Comp
L power:GND #PWR06
U 1 1 5F76AF0B
P 3650 2400
F 0 "#PWR06" H 3650 2150 50  0001 C CNN
F 1 "GND" H 3655 2227 50  0000 C CNN
F 2 "" H 3650 2400 50  0001 C CNN
F 3 "" H 3650 2400 50  0001 C CNN
	1    3650 2400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0101
U 1 1 608C7462
P 4050 2400
F 0 "#PWR0101" H 4050 2150 50  0001 C CNN
F 1 "GND" H 4055 2227 50  0000 C CNN
F 2 "" H 4050 2400 50  0001 C CNN
F 3 "" H 4050 2400 50  0001 C CNN
	1    4050 2400
	1    0    0    -1  
$EndComp
Wire Wire Line
	5250 2100 5950 2100
$Comp
L Device:R R1
U 1 1 608CB319
P 5950 950
F 0 "R1" H 5880 904 50  0000 R CNN
F 1 "0.22Ohm" H 5880 995 50  0000 R CNN
F 2 "Resistor_SMD:R_2512_6332Metric" V 5880 950 50  0001 C CNN
F 3 "~" H 5950 950 50  0001 C CNN
	1    5950 950 
	-1   0    0    1   
$EndComp
Wire Wire Line
	5950 2100 5950 1600
Connection ~ 5950 2100
Wire Wire Line
	5950 2100 6400 2100
Wire Wire Line
	5950 1300 5950 1100
Wire Wire Line
	5950 800  5950 650 
Connection ~ 5950 650 
Wire Wire Line
	5950 650  6400 650 
Wire Wire Line
	6400 2100 6400 1600
Connection ~ 6400 2100
Wire Wire Line
	6400 2100 6600 2100
Wire Wire Line
	6400 1300 6400 650 
Connection ~ 6400 650 
Wire Wire Line
	6400 650  6750 650 
Wire Wire Line
	4100 650  4100 1550
Connection ~ 4050 2100
Wire Wire Line
	4050 2100 4450 2100
Wire Wire Line
	3650 2100 4050 2100
Wire Wire Line
	4100 1550 3100 1550
Wire Wire Line
	3100 1550 3100 2100
Wire Wire Line
	3100 2100 3200 2100
Connection ~ 4100 1550
Wire Wire Line
	4100 1550 4100 1900
$EndSCHEMATC
