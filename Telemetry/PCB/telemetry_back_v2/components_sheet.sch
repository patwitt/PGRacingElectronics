EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 3
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text Notes 2600 3500 0    50   ~ 0
PUSH TO TALK \n
Text GLabel 3000 3750 2    50   Input ~ 0
GND_MIC&ANC
Text GLabel 1900 3850 0    50   Input ~ 0
GND_SPK
Text GLabel 5850 1700 0    50   Input ~ 0
BREAK_PRESSURE_0
$Comp
L telemetria-tyl-rescue:+5V-power-telemetria-tyl-rescue-telemetria-tyl-rescue #PWR?
U 1 1 62F4F237
P 7300 1300
AR Path="/62F4F237" Ref="#PWR?"  Part="1" 
AR Path="/62F2270E/62F4F237" Ref="#PWR0135"  Part="1" 
F 0 "#PWR0135" H 7300 1150 50  0001 C CNN
F 1 "+5V" H 7315 1473 50  0000 C CNN
F 2 "" H 7300 1300 50  0001 C CNN
F 3 "" H 7300 1300 50  0001 C CNN
	1    7300 1300
	1    0    0    -1  
$EndComp
$Comp
L telemetria-tyl-rescue:GND-power-telemetria-tyl-rescue-telemetria-tyl-rescue #PWR?
U 1 1 62F4F23D
P 7300 1600
AR Path="/62F4F23D" Ref="#PWR?"  Part="1" 
AR Path="/62F2270E/62F4F23D" Ref="#PWR0137"  Part="1" 
F 0 "#PWR0137" H 7300 1350 50  0001 C CNN
F 1 "GND" V 7305 1472 50  0000 R CNN
F 2 "" H 7300 1600 50  0001 C CNN
F 3 "" H 7300 1600 50  0001 C CNN
	1    7300 1600
	1    0    0    -1  
$EndComp
$Comp
L telemetria-tyl-rescue:NCX2200GW,125-NCX2200GW_125-telemetria-tyl-rescue-telemetria-tyl-rescue IC?
U 1 1 62F4F249
P 5850 1500
AR Path="/62F4F249" Ref="IC?"  Part="1" 
AR Path="/62F2270E/62F4F249" Ref="IC3"  Part="1" 
F 0 "IC3" H 6350 1765 50  0000 C CNN
F 1 "NCX2200GW,125" H 6350 1674 50  0000 C CNN
F 2 "SamacSys_Parts:SOT65P212X110-5N" H 6700 1600 50  0001 L CNN
F 3 "http://www.nxp.com/docs/en/data-sheet/NCX2200.pdf" H 6700 1500 50  0001 L CNN
F 4 "Analog Comparators Low Voltage Comparator" H 6700 1400 50  0001 L CNN "Description"
F 5 "1.1" H 6700 1300 50  0001 L CNN "Height"
F 6 "771-NCX2200GW125" H 6700 1200 50  0001 L CNN "Mouser Part Number"
F 7 "https://www.mouser.co.uk/ProductDetail/NXP-Semiconductors/NCX2200GW125?qs=j56XEm5BXrQpPSyPxfTy%2Fg%3D%3D" H 6700 1100 50  0001 L CNN "Mouser Price/Stock"
F 8 "NXP" H 6700 1000 50  0001 L CNN "Manufacturer_Name"
F 9 "NCX2200GW,125" H 6700 900 50  0001 L CNN "Manufacturer_Part_Number"
	1    5850 1500
	1    0    0    -1  
$EndComp
$Comp
L telemetria-tyl-rescue:GND-power-telemetria-tyl-rescue-telemetria-tyl-rescue #PWR?
U 1 1 62F4F24F
P 5850 1600
AR Path="/62F4F24F" Ref="#PWR?"  Part="1" 
AR Path="/62F2270E/62F4F24F" Ref="#PWR0138"  Part="1" 
F 0 "#PWR0138" H 5850 1350 50  0001 C CNN
F 1 "GND" V 5855 1472 50  0000 R CNN
F 2 "" H 5850 1600 50  0001 C CNN
F 3 "" H 5850 1600 50  0001 C CNN
	1    5850 1600
	0    1    1    0   
$EndComp
$Comp
L telemetria-tyl-rescue:+5V-power-telemetria-tyl-rescue-telemetria-tyl-rescue #PWR?
U 1 1 62F4F255
P 6850 1600
AR Path="/62F4F255" Ref="#PWR?"  Part="1" 
AR Path="/62F2270E/62F4F255" Ref="#PWR0140"  Part="1" 
F 0 "#PWR0140" H 6850 1450 50  0001 C CNN
F 1 "+5V" H 6865 1773 50  0000 C CNN
F 2 "" H 6850 1600 50  0001 C CNN
F 3 "" H 6850 1600 50  0001 C CNN
	1    6850 1600
	0    1    1    0   
$EndComp
Wire Wire Line
	7150 1450 6850 1450
Wire Wire Line
	6850 1450 6850 1500
$Comp
L telemetria-tyl-rescue:GND-power-telemetria-tyl-rescue-telemetria-tyl-rescue #PWR?
U 1 1 62F4F25D
P 4450 1300
AR Path="/62F4F25D" Ref="#PWR?"  Part="1" 
AR Path="/62F2270E/62F4F25D" Ref="#PWR0150"  Part="1" 
F 0 "#PWR0150" H 4450 1050 50  0001 C CNN
F 1 "GND" V 4455 1172 50  0000 R CNN
F 2 "" H 4450 1300 50  0001 C CNN
F 3 "" H 4450 1300 50  0001 C CNN
	1    4450 1300
	-1   0    0    1   
$EndComp
Text GLabel 4450 1900 3    50   Input ~ 0
LED_GND
$Comp
L telemetria-tyl-rescue:R_POT-Device-telemetria-tyl-rescue-telemetria-tyl-rescue RV?
U 1 1 62F4F264
P 7300 1450
AR Path="/62F4F264" Ref="RV?"  Part="1" 
AR Path="/62F2270E/62F4F264" Ref="RV1"  Part="1" 
F 0 "RV1" H 7230 1404 50  0000 R CNN
F 1 "R_POT" H 7230 1495 50  0000 R CNN
F 2 "SamacSys_Parts:Vishay_pot" H 7300 1450 50  0001 C CNN
F 3 "~" H 7300 1450 50  0001 C CNN
	1    7300 1450
	-1   0    0    1   
$EndComp
$Comp
L telemetria-tyl-rescue:R-Device-telemetria-tyl-rescue-telemetria-tyl-rescue R?
U 1 1 62F4F26A
P 5250 1500
AR Path="/62F4F26A" Ref="R?"  Part="1" 
AR Path="/62F2270E/62F4F26A" Ref="R21"  Part="1" 
F 0 "R21" V 5043 1500 50  0000 C CNN
F 1 "3.3k" V 5134 1500 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" V 5180 1500 50  0001 C CNN
F 3 "~" H 5250 1500 50  0001 C CNN
	1    5250 1500
	0    1    1    0   
$EndComp
Wire Wire Line
	5850 1500 5400 1500
Wire Wire Line
	5100 1500 4750 1500
$Comp
L telemetria-tyl-rescue:R-Device-telemetria-tyl-rescue-telemetria-tyl-rescue R?
U 1 1 62F4F272
P 2850 3750
AR Path="/62F4F272" Ref="R?"  Part="1" 
AR Path="/62F2270E/62F4F272" Ref="R20"  Part="1" 
F 0 "R20" V 2643 3750 50  0000 C CNN
F 1 "1k" V 2734 3750 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" V 2780 3750 50  0001 C CNN
F 3 "~" H 2850 3750 50  0001 C CNN
	1    2850 3750
	0    1    1    0   
$EndComp
$Comp
L telemetria-tyl-rescue:R-Device-telemetria-tyl-rescue-telemetria-tyl-rescue R?
U 1 1 62F4F278
P 1750 3750
AR Path="/62F4F278" Ref="R?"  Part="1" 
AR Path="/62F2270E/62F4F278" Ref="R15"  Part="1" 
F 0 "R15" V 1543 3750 50  0000 C CNN
F 1 "4.7k" V 1634 3750 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" V 1680 3750 50  0001 C CNN
F 3 "~" H 1750 3750 50  0001 C CNN
	1    1750 3750
	0    1    1    0   
$EndComp
Text GLabel 1600 3750 0    50   Input ~ 0
PTT
$Comp
L telemetria-tyl-rescue:BC846BW-BC846BW-telemetria-tyl-rescue-telemetria-tyl-rescue BJT?
U 1 1 62F4F285
P 1900 3750
AR Path="/62F4F285" Ref="BJT?"  Part="1" 
AR Path="/62F2270E/62F4F285" Ref="BJT1"  Part="1" 
F 0 "BJT1" H 2300 4015 50  0000 C CNN
F 1 "BC846BW,115" H 2300 3924 50  0000 C CNN
F 2 "SamacSys_Parts:SOT65P210X110-3N" H 2550 3850 50  0001 L CNN
F 3 "http://www.nxp.com/documents/data_sheet/BC846_SER.pdf" H 2550 3750 50  0001 L CNN
F 4 "Bipolar Transistors - BJT TRANS GP TAPE-7" H 2550 3650 50  0001 L CNN "Description"
F 5 "1.1" H 2550 3550 50  0001 L CNN "Height"
F 6 "771-BC846BW-T/R" H 2550 3450 50  0001 L CNN "Mouser Part Number"
F 7 "https://www.mouser.co.uk/ProductDetail/Nexperia/BC846BW115?qs=me8TqzrmIYXeHaJTKQ9DIQ%3D%3D" H 2550 3350 50  0001 L CNN "Mouser Price/Stock"
F 8 "Nexperia" H 2550 3250 50  0001 L CNN "Manufacturer_Name"
F 9 "BC846BW,115" H 2550 3150 50  0001 L CNN "Manufacturer_Part_Number"
	1    1900 3750
	1    0    0    -1  
$EndComp
$Comp
L telemetria-tyl-rescue:PMV280ENEAR-PMV280ENEAR-telemetria-tyl-rescue-telemetria-tyl-rescue Q?
U 1 1 62F4F291
P 4750 1500
AR Path="/62F4F291" Ref="Q?"  Part="1" 
AR Path="/62F2270E/62F4F291" Ref="Q1"  Part="1" 
F 0 "Q1" H 5179 1554 50  0000 L CNN
F 1 "PMV280ENEAR" H 5179 1645 50  0000 L CNN
F 2 "SamacSys_Parts:SOT95P237X112-3N" H 5200 1450 50  0001 L CNN
F 3 "https://assets.nexperia.com/documents/data-sheet/PMV280ENEA.pdf" H 5200 1350 50  0001 L CNN
F 4 "PMV280ENEAR 3 N-Channel MOSFET, 1 A, 100 V, 3-Pin SOT-23 Nexperia" H 5200 1250 50  0001 L CNN "Description"
F 5 "1.1" H 5200 1150 50  0001 L CNN "Height"
F 6 "771-PMV280ENEAR" H 5200 1050 50  0001 L CNN "Mouser Part Number"
F 7 "https://www.mouser.co.uk/ProductDetail/Nexperia/PMV280ENEAR?qs=%252BEew9%252B0nqrBFzYrVZ6LkLA%3D%3D" H 5200 950 50  0001 L CNN "Mouser Price/Stock"
F 8 "Nexperia" H 5200 850 50  0001 L CNN "Manufacturer_Name"
F 9 "PMV280ENEAR" H 5200 750 50  0001 L CNN "Manufacturer_Part_Number"
	1    4750 1500
	-1   0    0    1   
$EndComp
Connection ~ 2100 2050
Connection ~ 2100 1450
Text GLabel 2400 2050 2    50   Input ~ 0
VSS_IN_2
Text GLabel 1700 2050 0    50   Input ~ 0
TIM3_ETR
Wire Wire Line
	1800 2050 1700 2050
$Comp
L telemetria-tyl-rescue:R-Device-telemetria-tyl-rescue-telemetria-tyl-rescue R?
U 1 1 62F4F29C
P 1950 2050
AR Path="/62F4F29C" Ref="R?"  Part="1" 
AR Path="/62F2270E/62F4F29C" Ref="R17"  Part="1" 
F 0 "R17" V 1743 2050 50  0000 C CNN
F 1 "330" V 1834 2050 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" V 1880 2050 50  0001 C CNN
F 3 "~" H 1950 2050 50  0001 C CNN
	1    1950 2050
	0    1    1    0   
$EndComp
Wire Wire Line
	2100 2050 2100 2200
$Comp
L telemetria-tyl-rescue:GND-power-telemetria-tyl-rescue-telemetria-tyl-rescue #PWR?
U 1 1 62F4F2A3
P 2100 2200
AR Path="/62F4F2A3" Ref="#PWR?"  Part="1" 
AR Path="/62F2270E/62F4F2A3" Ref="#PWR0158"  Part="1" 
F 0 "#PWR0158" H 2100 1950 50  0001 C CNN
F 1 "GND" H 2105 2027 50  0000 C CNN
F 2 "" H 2100 2200 50  0001 C CNN
F 3 "" H 2100 2200 50  0001 C CNN
	1    2100 2200
	1    0    0    -1  
$EndComp
$Comp
L telemetria-tyl-rescue:R-Device-telemetria-tyl-rescue-telemetria-tyl-rescue R?
U 1 1 62F4F2A9
P 2250 2050
AR Path="/62F4F2A9" Ref="R?"  Part="1" 
AR Path="/62F2270E/62F4F2A9" Ref="R19"  Part="1" 
F 0 "R19" V 2043 2050 50  0000 C CNN
F 1 "1k" V 2134 2050 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" V 2180 2050 50  0001 C CNN
F 3 "~" H 2250 2050 50  0001 C CNN
	1    2250 2050
	0    1    1    0   
$EndComp
Wire Wire Line
	1800 1450 1700 1450
$Comp
L telemetria-tyl-rescue:R-Device-telemetria-tyl-rescue-telemetria-tyl-rescue R?
U 1 1 62F4F2B0
P 1950 1450
AR Path="/62F4F2B0" Ref="R?"  Part="1" 
AR Path="/62F2270E/62F4F2B0" Ref="R16"  Part="1" 
F 0 "R16" V 1743 1450 50  0000 C CNN
F 1 "330" V 1834 1450 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" V 1880 1450 50  0001 C CNN
F 3 "~" H 1950 1450 50  0001 C CNN
	1    1950 1450
	0    1    1    0   
$EndComp
Wire Wire Line
	2100 1450 2100 1600
$Comp
L telemetria-tyl-rescue:GND-power-telemetria-tyl-rescue-telemetria-tyl-rescue #PWR?
U 1 1 62F4F2B7
P 2100 1600
AR Path="/62F4F2B7" Ref="#PWR?"  Part="1" 
AR Path="/62F2270E/62F4F2B7" Ref="#PWR0159"  Part="1" 
F 0 "#PWR0159" H 2100 1350 50  0001 C CNN
F 1 "GND" H 2105 1427 50  0000 C CNN
F 2 "" H 2100 1600 50  0001 C CNN
F 3 "" H 2100 1600 50  0001 C CNN
	1    2100 1600
	1    0    0    -1  
$EndComp
$Comp
L telemetria-tyl-rescue:R-Device-telemetria-tyl-rescue-telemetria-tyl-rescue R?
U 1 1 62F4F2BD
P 2250 1450
AR Path="/62F4F2BD" Ref="R?"  Part="1" 
AR Path="/62F2270E/62F4F2BD" Ref="R18"  Part="1" 
F 0 "R18" V 2043 1450 50  0000 C CNN
F 1 "1k" V 2134 1450 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" V 2180 1450 50  0001 C CNN
F 3 "~" H 2250 1450 50  0001 C CNN
	1    2250 1450
	0    1    1    0   
$EndComp
Text GLabel 1700 1450 0    50   Input ~ 0
TIM2_ETR
Text GLabel 2400 1450 2    50   Input ~ 0
VSS_IN_1
Wire Notes Line
	3050 500  3050 2600
Wire Notes Line
	4150 2600 4150 4300
Wire Notes Line
	500  500  500  4300
$Comp
L telemetria-tyl-rescue:R-Device-telemetria-tyl-rescue-telemetria-tyl-rescue R?
U 1 1 62F5E6A4
P 7000 3650
AR Path="/62F5E6A4" Ref="R?"  Part="1" 
AR Path="/62F2270E/62F5E6A4" Ref="R22"  Part="1" 
F 0 "R22" H 7070 3696 50  0000 L CNN
F 1 "120" H 7070 3605 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" V 6930 3650 50  0001 C CNN
F 3 "~" H 7000 3650 50  0001 C CNN
	1    7000 3650
	1    0    0    -1  
$EndComp
$Comp
L telemetria-tyl-rescue:MCP2561-E-SN-Interface_CAN_LIN-telemetria-tyl-rescue-telemetria-tyl-rescue U?
U 1 1 62F5E6AA
P 5850 3700
AR Path="/62F5E6AA" Ref="U?"  Part="1" 
AR Path="/62F2270E/62F5E6AA" Ref="U2"  Part="1" 
F 0 "U2" H 6350 4300 50  0000 C CNN
F 1 "MCP2561-E-SN" H 6350 4150 50  0000 C CNN
F 2 "Package_SO:SOIC-8_3.9x4.9mm_P1.27mm" H 5850 3200 50  0001 C CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/25167A.pdf" H 5850 3700 50  0001 C CNN
	1    5850 3700
	1    0    0    -1  
$EndComp
$Comp
L telemetria-tyl-rescue:GND-power-telemetria-tyl-rescue-telemetria-tyl-rescue #PWR?
U 1 1 62F5E6B0
P 5850 4200
AR Path="/62F5E6B0" Ref="#PWR?"  Part="1" 
AR Path="/62F2270E/62F5E6B0" Ref="#PWR0160"  Part="1" 
F 0 "#PWR0160" H 5850 3950 50  0001 C CNN
F 1 "GND" H 5855 4027 50  0000 C CNN
F 2 "" H 5850 4200 50  0001 C CNN
F 3 "" H 5850 4200 50  0001 C CNN
	1    5850 4200
	1    0    0    -1  
$EndComp
Wire Wire Line
	5850 4050 5850 4100
Connection ~ 5850 4100
Wire Wire Line
	5850 4100 5850 4200
Wire Wire Line
	5850 4100 5100 4100
Wire Wire Line
	5100 4100 5100 3900
Wire Wire Line
	5100 3900 5350 3900
Wire Wire Line
	5850 3300 5850 3000
$Comp
L telemetria-tyl-rescue:+5V-power-telemetria-tyl-rescue-telemetria-tyl-rescue #PWR?
U 1 1 62F5E6BD
P 5850 3000
AR Path="/62F5E6BD" Ref="#PWR?"  Part="1" 
AR Path="/62F2270E/62F5E6BD" Ref="#PWR0162"  Part="1" 
F 0 "#PWR0162" H 5850 2850 50  0001 C CNN
F 1 "+5V" H 5865 3173 50  0000 C CNN
F 2 "" H 5850 3000 50  0001 C CNN
F 3 "" H 5850 3000 50  0001 C CNN
	1    5850 3000
	1    0    0    -1  
$EndComp
Wire Wire Line
	6350 3600 6750 3600
Wire Wire Line
	6750 3600 6750 2700
Wire Wire Line
	6750 2700 7000 2700
Wire Wire Line
	6350 3800 7000 3800
Text GLabel 5350 3600 0    50   Input ~ 0
CAN1_RX
Text GLabel 5350 3500 0    50   Input ~ 0
CAN1_TX
Text GLabel 7000 2700 2    50   Input ~ 0
CAN_H1
Text GLabel 7000 3800 2    50   Input ~ 0
CAN_L1
Wire Wire Line
	7000 2700 7000 3500
Wire Notes Line
	8250 500  8250 4300
Wire Notes Line
	500  4300 8250 4300
Text GLabel 9800 950  2    50   Input ~ 0
RCC_OSC_IN
Text GLabel 9800 1150 2    50   Input ~ 0
RCC_OSC_OUT
Wire Wire Line
	9800 950  9250 950 
Wire Wire Line
	9250 950  9250 1400
Wire Wire Line
	9250 1400 9450 1400
$Comp
L telemetria-tyl-rescue:R_Small-Device-telemetria-tyl-rescue-telemetria-tyl-rescue R?
U 1 1 62FB9F29
P 9750 1300
AR Path="/62FB9F29" Ref="R?"  Part="1" 
AR Path="/62F2270E/62FB9F29" Ref="R23"  Part="1" 
F 0 "R23" H 9809 1346 50  0000 L CNN
F 1 "47" H 9800 1250 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder" H 9750 1300 50  0001 C CNN
F 3 "~" H 9750 1300 50  0001 C CNN
	1    9750 1300
	1    0    0    -1  
$EndComp
Wire Wire Line
	9750 1200 9750 1150
Wire Wire Line
	9750 1150 9800 1150
Wire Wire Line
	9750 1400 9650 1400
$Comp
L telemetria-tyl-rescue:C-Device-telemetria-tyl-rescue-telemetria-tyl-rescue C?
U 1 1 62FB9F32
P 9750 1650
AR Path="/62FB9F32" Ref="C?"  Part="1" 
AR Path="/62F2270E/62FB9F32" Ref="C23"  Part="1" 
F 0 "C23" H 9865 1696 50  0000 L CNN
F 1 "14pF" H 9865 1605 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.18x1.45mm_HandSolder" H 9788 1500 50  0001 C CNN
F 3 "~" H 9750 1650 50  0001 C CNN
	1    9750 1650
	1    0    0    -1  
$EndComp
Wire Wire Line
	9250 1400 9250 1500
Connection ~ 9250 1400
Wire Wire Line
	9750 1400 9750 1500
Connection ~ 9750 1400
Connection ~ 9550 1800
Wire Wire Line
	9550 1800 9750 1800
Wire Wire Line
	9550 1300 9550 1250
$Comp
L telemetria-tyl-rescue:GND-power-telemetria-tyl-rescue-telemetria-tyl-rescue #PWR?
U 1 1 62FB9F3F
P 9550 2000
AR Path="/62FB9F3F" Ref="#PWR?"  Part="1" 
AR Path="/62F2270E/62FB9F3F" Ref="#PWR0163"  Part="1" 
F 0 "#PWR0163" H 9550 1750 50  0001 C CNN
F 1 "GND" H 9555 1827 50  0000 C CNN
F 2 "" H 9550 2000 50  0001 C CNN
F 3 "" H 9550 2000 50  0001 C CNN
	1    9550 2000
	1    0    0    -1  
$EndComp
Wire Wire Line
	9550 1800 9550 2000
Wire Wire Line
	9550 1500 9550 1800
Wire Wire Line
	9550 1250 9400 1250
Wire Wire Line
	9400 1250 9400 1800
Wire Wire Line
	9250 1800 9400 1800
Wire Wire Line
	9400 1800 9550 1800
Connection ~ 9400 1800
$Comp
L telemetria-tyl-rescue:C-Device-telemetria-tyl-rescue-telemetria-tyl-rescue C?
U 1 1 62FB9F4C
P 9250 1650
AR Path="/62FB9F4C" Ref="C?"  Part="1" 
AR Path="/62F2270E/62FB9F4C" Ref="C22"  Part="1" 
F 0 "C22" H 9365 1696 50  0000 L CNN
F 1 "14pF" H 9365 1605 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.18x1.45mm_HandSolder" H 9288 1500 50  0001 C CNN
F 3 "~" H 9250 1650 50  0001 C CNN
	1    9250 1650
	1    0    0    -1  
$EndComp
$Comp
L telemetria-tyl-rescue:Crystal_GND24_Small-Device-telemetria-tyl-rescue-telemetria-tyl-rescue Y?
U 1 1 62FB9F52
P 9550 1400
AR Path="/62FB9F52" Ref="Y?"  Part="1" 
AR Path="/62F2270E/62FB9F52" Ref="Y1"  Part="1" 
F 0 "Y1" H 9500 1600 50  0000 L CNN
F 1 "16Mhz" H 9279 1495 50  0000 L CNN
F 2 "Crystal:Crystal_SMD_3225-4Pin_3.2x2.5mm_HandSoldering" H 9550 1400 50  0001 C CNN
F 3 "~" H 9550 1400 50  0001 C CNN
	1    9550 1400
	1    0    0    -1  
$EndComp
Wire Notes Line
	500  2600 11200 2600
Wire Notes Line
	500  500  11200 500 
Wire Notes Line
	11200 500  11200 2600
Text Notes 1900 3200 0    83   ~ 0
DRUT - RELAY
$EndSCHEMATC
