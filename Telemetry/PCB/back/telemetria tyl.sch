EESchema Schematic File Version 4
EELAYER 30 0
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
Text GLabel 4600 850  2    50   Input ~ 0
ADC1_IN0
Text GLabel 4600 950  2    50   Input ~ 0
ADC2_IN1
Text GLabel 4600 1050 2    50   Input ~ 0
TIM2_ETR
Text GLabel 3200 2350 0    50   Input ~ 0
TIM3_ETR
Text GLabel 4600 3550 2    50   Input ~ 0
I2C2_SCL
Text GLabel 4600 3650 2    50   Input ~ 0
12C2_SDA
Text GLabel 4600 2050 2    50   Input ~ 0
USB_OTG_FS_DP
Text GLabel 4600 1950 2    50   Input ~ 0
USB_OTG_FS_DM
Text GLabel 4600 3250 2    50   Input ~ 0
I2C1_SDA
Text GLabel 4600 3150 2    50   Input ~ 0
12C1_SCL
Text GLabel 4600 3350 2    50   Input ~ 0
CAN1_RX
Text GLabel 4600 3450 2    50   Input ~ 0
CAN1_TX
Text GLabel 3200 2050 0    50   Input ~ 0
RCC_OSC_IN
Text GLabel 3200 2150 0    50   Input ~ 0
RCC_OSC_OUT
$Comp
L Device:R R?
U 1 1 623419BA
P 7400 2050
F 0 "R?" H 7470 2096 50  0000 L CNN
F 1 "R" H 7470 2005 50  0000 L CNN
F 2 "" V 7330 2050 50  0001 C CNN
F 3 "~" H 7400 2050 50  0001 C CNN
	1    7400 2050
	1    0    0    -1  
$EndComp
$Comp
L Interface_CAN_LIN:MCP2561-E-SN U?
U 1 1 62343D25
P 6250 2100
F 0 "U?" H 6750 2700 50  0000 C CNN
F 1 "MCP2561-E-SN" H 6750 2550 50  0000 C CNN
F 2 "Package_SO:SOIC-8_3.9x4.9mm_P1.27mm" H 6250 1600 50  0001 C CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/25167A.pdf" H 6250 2100 50  0001 C CNN
	1    6250 2100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 6234506F
P 6250 2600
F 0 "#PWR?" H 6250 2350 50  0001 C CNN
F 1 "GND" H 6255 2427 50  0000 C CNN
F 2 "" H 6250 2600 50  0001 C CNN
F 3 "" H 6250 2600 50  0001 C CNN
	1    6250 2600
	1    0    0    -1  
$EndComp
Wire Wire Line
	6250 2450 6250 2500
Connection ~ 6250 2500
Wire Wire Line
	6250 2500 6250 2600
Wire Wire Line
	6250 2500 5500 2500
Wire Wire Line
	5500 2500 5500 2300
Wire Wire Line
	5500 2300 5750 2300
$Comp
L Device:Jumper JP?
U 1 1 6234744E
P 7400 1400
F 0 "JP?" V 7446 1312 50  0000 R CNN
F 1 "Jumper" V 7355 1312 50  0000 R CNN
F 2 "" H 7400 1400 50  0001 C CNN
F 3 "~" H 7400 1400 50  0001 C CNN
	1    7400 1400
	0    -1   -1   0   
$EndComp
$Comp
L Jumper:Jumper_3_Bridged12 JP?
U 1 1 62348715
P 6250 1250
F 0 "JP?" H 6250 1454 50  0000 C CNN
F 1 "Jumper_3_Bridged12" H 6250 1363 50  0000 C CNN
F 2 "" H 6250 1250 50  0001 C CNN
F 3 "~" H 6250 1250 50  0001 C CNN
	1    6250 1250
	1    0    0    -1  
$EndComp
Wire Wire Line
	6250 1700 6250 1400
$Comp
L power:+3.3V #PWR?
U 1 1 6234E1D3
P 5700 1250
F 0 "#PWR?" H 5700 1100 50  0001 C CNN
F 1 "+3.3V" H 5715 1423 50  0000 C CNN
F 2 "" H 5700 1250 50  0001 C CNN
F 3 "" H 5700 1250 50  0001 C CNN
	1    5700 1250
	1    0    0    -1  
$EndComp
Wire Wire Line
	5700 1250 6000 1250
Connection ~ 6000 1250
Wire Wire Line
	6000 1250 6050 1250
$Comp
L power:+5V #PWR?
U 1 1 6234F920
P 6700 1250
F 0 "#PWR?" H 6700 1100 50  0001 C CNN
F 1 "+5V" H 6715 1423 50  0000 C CNN
F 2 "" H 6700 1250 50  0001 C CNN
F 3 "" H 6700 1250 50  0001 C CNN
	1    6700 1250
	1    0    0    -1  
$EndComp
Wire Wire Line
	6500 1250 6700 1250
Wire Wire Line
	6750 2000 7150 2000
Wire Wire Line
	7150 2000 7150 1100
Wire Wire Line
	7150 1100 7400 1100
Wire Wire Line
	6750 2200 7400 2200
Wire Wire Line
	7400 1900 7400 1700
$Comp
L MCU_ST_STM32F4:STM32F405RGTx U?
U 1 1 62338035
P 3900 2450
F 0 "U?" H 3900 561 50  0000 C CNN
F 1 "STM32F405RGTx" H 3900 470 50  0000 C CNN
F 2 "Package_QFP:LQFP-64_10x10mm_P0.5mm" H 3300 750 50  0001 R CNN
F 3 "http://www.st.com/st-web-ui/static/active/en/resource/technical/document/datasheet/DM00037051.pdf" H 3900 2450 50  0001 C CNN
	1    3900 2450
	1    0    0    -1  
$EndComp
Text GLabel 5750 2000 0    50   Input ~ 0
CAN1_RX
Text GLabel 5750 1900 0    50   Input ~ 0
CAN1_TX
Text GLabel 7400 1100 2    50   Input ~ 0
CAN_H1
Text GLabel 7400 2200 2    50   Input ~ 0
CAN_L1
$Comp
L power:GND #PWR?
U 1 1 6235A7AB
P 8850 1700
F 0 "#PWR?" H 8850 1450 50  0001 C CNN
F 1 "GND" H 8855 1527 50  0000 C CNN
F 2 "" H 8850 1700 50  0001 C CNN
F 3 "" H 8850 1700 50  0001 C CNN
	1    8850 1700
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole_Pad H?
U 1 1 6235B400
P 8750 1600
F 0 "H?" V 8987 1603 50  0000 C CNN
F 1 "MountingHole_Pad" V 8750 2050 50  0000 C CNN
F 2 "" H 8750 1600 50  0001 C CNN
F 3 "~" H 8750 1600 50  0001 C CNN
	1    8750 1600
	0    -1   -1   0   
$EndComp
$Comp
L Mechanical:MountingHole_Pad H?
U 1 1 6235D03A
P 8750 1450
F 0 "H?" V 8987 1453 50  0000 C CNN
F 1 "MountingHole_Pad" V 8750 1900 50  0000 C CNN
F 2 "" H 8750 1450 50  0001 C CNN
F 3 "~" H 8750 1450 50  0001 C CNN
	1    8750 1450
	0    -1   -1   0   
$EndComp
$Comp
L Mechanical:MountingHole_Pad H?
U 1 1 6235DD77
P 8750 1300
F 0 "H?" V 8750 1100 50  0000 C CNN
F 1 "MountingHole_Pad" V 8750 1750 50  0000 C CNN
F 2 "" H 8750 1300 50  0001 C CNN
F 3 "~" H 8750 1300 50  0001 C CNN
	1    8750 1300
	0    -1   -1   0   
$EndComp
$Comp
L Mechanical:MountingHole_Pad H?
U 1 1 6235DD7D
P 8750 1150
F 0 "H?" V 8750 950 50  0000 C CNN
F 1 "MountingHole_Pad" V 8750 1600 50  0000 C CNN
F 2 "" H 8750 1150 50  0001 C CNN
F 3 "~" H 8750 1150 50  0001 C CNN
	1    8750 1150
	0    -1   -1   0   
$EndComp
Wire Wire Line
	8850 1150 8850 1300
Connection ~ 8850 1300
Wire Wire Line
	8850 1300 8850 1450
Connection ~ 8850 1450
Wire Wire Line
	8850 1450 8850 1600
Connection ~ 8850 1600
Wire Wire Line
	8850 1600 8850 1700
$Comp
L power:+12V #PWR?
U 1 1 6235F943
P 10000 1100
F 0 "#PWR?" H 10000 950 50  0001 C CNN
F 1 "+12V" H 10015 1273 50  0000 C CNN
F 2 "" H 10000 1100 50  0001 C CNN
F 3 "" H 10000 1100 50  0001 C CNN
	1    10000 1100
	1    0    0    -1  
$EndComp
$Comp
L Device:Ferrite_Bead FB?
U 1 1 623608A7
P 10400 1100
F 0 "FB?" V 10126 1100 50  0000 C CNN
F 1 "Ferrite_Bead" V 10217 1100 50  0000 C CNN
F 2 "" V 10330 1100 50  0001 C CNN
F 3 "~" H 10400 1100 50  0001 C CNN
	1    10400 1100
	0    1    1    0   
$EndComp
Wire Wire Line
	10000 1100 10250 1100
Wire Wire Line
	10550 1100 10600 1100
$Comp
L power:+12V #PWR?
U 1 1 62364F31
P 10800 1100
F 0 "#PWR?" H 10800 950 50  0001 C CNN
F 1 "+12V" H 10815 1273 50  0000 C CNN
F 2 "" H 10800 1100 50  0001 C CNN
F 3 "" H 10800 1100 50  0001 C CNN
	1    10800 1100
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 623657F4
P 10600 1250
F 0 "C?" H 10715 1296 50  0000 L CNN
F 1 "100nF" H 10715 1205 50  0000 L CNN
F 2 "" H 10638 1100 50  0001 C CNN
F 3 "~" H 10600 1250 50  0001 C CNN
	1    10600 1250
	1    0    0    -1  
$EndComp
Connection ~ 10600 1100
Wire Wire Line
	10600 1100 10800 1100
$Comp
L power:GND #PWR?
U 1 1 62369BB2
P 10600 1650
F 0 "#PWR?" H 10600 1400 50  0001 C CNN
F 1 "GND" H 10605 1477 50  0000 C CNN
F 2 "" H 10600 1650 50  0001 C CNN
F 3 "" H 10600 1650 50  0001 C CNN
	1    10600 1650
	1    0    0    -1  
$EndComp
Wire Wire Line
	10600 1400 10600 1650
$Comp
L power:+12V #PWR?
U 1 1 6236A1B0
P 7250 2700
F 0 "#PWR?" H 7250 2550 50  0001 C CNN
F 1 "+12V" H 7265 2873 50  0000 C CNN
F 2 "" H 7250 2700 50  0001 C CNN
F 3 "" H 7250 2700 50  0001 C CNN
	1    7250 2700
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 6236ABF1
P 7250 2950
F 0 "R?" H 7320 2996 50  0000 L CNN
F 1 "100k" H 7320 2905 50  0000 L CNN
F 2 "" V 7180 2950 50  0001 C CNN
F 3 "~" H 7250 2950 50  0001 C CNN
	1    7250 2950
	1    0    0    -1  
$EndComp
Wire Wire Line
	7250 2700 7250 2800
$Comp
L MP2333HGTL:MP2333HGTL IC?
U 1 1 62339546
P 2800 5350
F 0 "IC?" H 3300 5615 50  0000 C CNN
F 1 "MP2333HGTL" H 3300 5524 50  0000 C CNN
F 2 "SOTFL50P160X60-8N" H 3650 5450 50  0001 L CNN
F 3 "" H 3650 5350 50  0001 L CNN
F 4 "4.2 - 18V Input, 3A, 1.2MHz Synchronous Buck Converter with Power Good and Soft Start in SOT583 (1.6x2mm) Package" H 3650 5250 50  0001 L CNN "Description"
F 5 "0.6" H 3650 5150 50  0001 L CNN "Height"
F 6 "Monolithic Power Systems (MPS)" H 3650 5050 50  0001 L CNN "Manufacturer_Name"
F 7 "MP2333HGTL" H 3650 4950 50  0001 L CNN "Manufacturer_Part_Number"
F 8 "" H 3650 4850 50  0001 L CNN "Mouser Part Number"
F 9 "" H 3650 4750 50  0001 L CNN "Mouser Price/Stock"
F 10 "" H 3650 4650 50  0001 L CNN "Arrow Part Number"
F 11 "" H 3650 4550 50  0001 L CNN "Arrow Price/Stock"
	1    2800 5350
	1    0    0    -1  
$EndComp
$Comp
L MP2359DT-LF-P:MP2359DT-LF-P PS?
U 1 1 6233D311
P 8850 3100
F 0 "PS?" H 9350 3365 50  0000 C CNN
F 1 "MP2359DT-LF-P" H 9350 3274 50  0000 C CNN
F 2 "SOT95P280X145-6N" H 9700 3200 50  0001 L CNN
F 3 "https://ms.componentsearchengine.com/Datasheets/2/MP2359DT-LF-P.pdf" H 9700 3100 50  0001 L CNN
F 4 "MONOLITHIC POWER SYSTEMS (MPS) - MP2359DT-LF-P - DC-DC Switching Buck (Step Down) Regulator, 4.5V-24V in, 810mV to 15V out, 1.2A, 1.4MHz, SOT-23-6" H 9700 3000 50  0001 L CNN "Description"
F 5 "1.45" H 9700 2900 50  0001 L CNN "Height"
F 6 "Monolithic Power Systems (MPS)" H 9700 2800 50  0001 L CNN "Manufacturer_Name"
F 7 "MP2359DT-LF-P" H 9700 2700 50  0001 L CNN "Manufacturer_Part_Number"
F 8 "946-MP2359DTLFP" H 9700 2600 50  0001 L CNN "Mouser Part Number"
F 9 "https://www.mouser.co.uk/ProductDetail/Monolithic-Power-Systems-MPS/MP2359DT-LF-P?qs=rC7bBWoQAAlUuezWIXLG%2Fg%3D%3D" H 9700 2500 50  0001 L CNN "Mouser Price/Stock"
F 10 "" H 9700 2400 50  0001 L CNN "Arrow Part Number"
F 11 "" H 9700 2300 50  0001 L CNN "Arrow Price/Stock"
	1    8850 3100
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 62343FCC
P 8850 3000
F 0 "#PWR?" H 8850 2750 50  0001 C CNN
F 1 "GND" H 8855 2827 50  0000 C CNN
F 2 "" H 8850 3000 50  0001 C CNN
F 3 "" H 8850 3000 50  0001 C CNN
	1    8850 3000
	0    -1   -1   0   
$EndComp
Wire Wire Line
	7250 2800 7700 2800
Wire Wire Line
	7700 2800 7700 3000
Wire Wire Line
	7700 3000 7850 3000
Connection ~ 7250 2800
Wire Wire Line
	7250 3100 7750 3100
Wire Wire Line
	7750 3100 7750 2900
Wire Wire Line
	7750 2900 7850 2900
Wire Wire Line
	8850 2900 8900 2900
Wire Wire Line
	8900 2900 8900 2700
$Comp
L Device:R R?
U 1 1 62353910
P 9050 2700
F 0 "R?" V 8843 2700 50  0000 C CNN
F 1 "R" V 8934 2700 50  0000 C CNN
F 2 "" V 8980 2700 50  0001 C CNN
F 3 "~" H 9050 2700 50  0001 C CNN
	1    9050 2700
	0    1    1    0   
$EndComp
$Comp
L Device:R R?
U 1 1 623559DC
P 8750 2700
F 0 "R?" V 8543 2700 50  0000 C CNN
F 1 "R" V 8634 2700 50  0000 C CNN
F 2 "" V 8680 2700 50  0001 C CNN
F 3 "~" H 8750 2700 50  0001 C CNN
	1    8750 2700
	0    1    1    0   
$EndComp
Wire Wire Line
	8600 2700 8500 2700
Wire Wire Line
	9200 2700 9250 2700
Wire Wire Line
	9250 2700 9250 2650
$Comp
L power:GND #PWR?
U 1 1 62358A57
P 8500 2700
F 0 "#PWR?" H 8500 2450 50  0001 C CNN
F 1 "GND" H 8505 2527 50  0000 C CNN
F 2 "" H 8500 2700 50  0001 C CNN
F 3 "" H 8500 2700 50  0001 C CNN
	1    8500 2700
	0    1    1    0   
$EndComp
$Comp
L power:+3.3V #PWR?
U 1 1 62358F81
P 9250 2650
F 0 "#PWR?" H 9250 2500 50  0001 C CNN
F 1 "+3.3V" H 9265 2823 50  0000 C CNN
F 2 "" H 9250 2650 50  0001 C CNN
F 3 "" H 9250 2650 50  0001 C CNN
	1    9250 2650
	1    0    0    -1  
$EndComp
Wire Wire Line
	7850 3100 7850 3250
Wire Wire Line
	8850 3100 9150 3100
Wire Wire Line
	9150 3100 9250 3100
Connection ~ 9150 3100
$Comp
L Device:C C?
U 1 1 6235C6EC
P 9400 3100
F 0 "C?" V 9148 3100 50  0000 C CNN
F 1 "100nF" V 9239 3100 50  0000 C CNN
F 2 "" H 9438 2950 50  0001 C CNN
F 3 "~" H 9400 3100 50  0001 C CNN
	1    9400 3100
	0    1    1    0   
$EndComp
Wire Wire Line
	9150 2950 9150 3100
Wire Wire Line
	9550 3100 9550 3250
Wire Wire Line
	7850 3250 9550 3250
Wire Wire Line
	9550 3250 9650 3250
Connection ~ 9550 3250
$Comp
L Device:D_Schottky D?
U 1 1 6235F7BA
P 9650 3400
F 0 "D?" V 9604 3480 50  0000 L CNN
F 1 "D_Schottky" V 9695 3480 50  0000 L CNN
F 2 "" H 9650 3400 50  0001 C CNN
F 3 "~" H 9650 3400 50  0001 C CNN
	1    9650 3400
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 62360D9E
P 9650 3550
F 0 "#PWR?" H 9650 3300 50  0001 C CNN
F 1 "GND" H 9655 3377 50  0000 C CNN
F 2 "" H 9650 3550 50  0001 C CNN
F 3 "" H 9650 3550 50  0001 C CNN
	1    9650 3550
	1    0    0    -1  
$EndComp
Wire Wire Line
	9650 3250 9950 3250
Connection ~ 9650 3250
$Comp
L Device:L L?
U 1 1 62362581
P 10100 3250
F 0 "L?" V 10290 3250 50  0000 C CNN
F 1 "10uH" V 10199 3250 50  0000 C CNN
F 2 "" H 10100 3250 50  0001 C CNN
F 3 "~" H 10100 3250 50  0001 C CNN
	1    10100 3250
	0    -1   -1   0   
$EndComp
Wire Wire Line
	10250 3250 10500 3250
Wire Wire Line
	10500 3250 10500 2950
Wire Wire Line
	10500 2950 10150 2950
$Comp
L Diode:1N4148 D?
U 1 1 62364F03
P 10000 2950
F 0 "D?" H 10000 3167 50  0000 C CNN
F 1 "1N4148" H 10000 3076 50  0000 C CNN
F 2 "Diode_THT:D_DO-35_SOD27_P7.62mm_Horizontal" H 10000 2775 50  0001 C CNN
F 3 "https://assets.nexperia.com/documents/data-sheet/1N4148_1N4448.pdf" H 10000 2950 50  0001 C CNN
	1    10000 2950
	1    0    0    -1  
$EndComp
Wire Wire Line
	9850 2950 9150 2950
$Comp
L Device:C C?
U 1 1 62366B01
P 10500 3400
F 0 "C?" H 10615 3446 50  0000 L CNN
F 1 "10uF" H 10615 3355 50  0000 L CNN
F 2 "" H 10538 3250 50  0001 C CNN
F 3 "~" H 10500 3400 50  0001 C CNN
	1    10500 3400
	1    0    0    -1  
$EndComp
Connection ~ 10500 3250
$Comp
L Device:C C?
U 1 1 62367904
P 10850 3400
F 0 "C?" H 10965 3446 50  0000 L CNN
F 1 "10uF" H 10965 3355 50  0000 L CNN
F 2 "" H 10888 3250 50  0001 C CNN
F 3 "~" H 10850 3400 50  0001 C CNN
	1    10850 3400
	1    0    0    -1  
$EndComp
Wire Wire Line
	10500 3250 10850 3250
Wire Wire Line
	10500 3550 10850 3550
Wire Wire Line
	10850 3250 10850 3200
$Comp
L power:+3.3V #PWR?
U 1 1 62369E94
P 10850 3200
F 0 "#PWR?" H 10850 3050 50  0001 C CNN
F 1 "+3.3V" H 10865 3373 50  0000 C CNN
F 2 "" H 10850 3200 50  0001 C CNN
F 3 "" H 10850 3200 50  0001 C CNN
	1    10850 3200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 6236AC04
P 10700 3550
F 0 "#PWR?" H 10700 3300 50  0001 C CNN
F 1 "GND" H 10705 3377 50  0000 C CNN
F 2 "" H 10700 3550 50  0001 C CNN
F 3 "" H 10700 3550 50  0001 C CNN
	1    10700 3550
	1    0    0    -1  
$EndComp
$Comp
L power:+12L #PWR?
U 1 1 6236B52B
P 6000 3750
F 0 "#PWR?" H 6000 3600 50  0001 C CNN
F 1 "+12L" H 6015 3923 50  0000 C CNN
F 2 "" H 6000 3750 50  0001 C CNN
F 3 "" H 6000 3750 50  0001 C CNN
	1    6000 3750
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 6236C5D3
P 6000 3950
F 0 "R?" H 6070 3996 50  0000 L CNN
F 1 "R" H 6070 3905 50  0000 L CNN
F 2 "" V 5930 3950 50  0001 C CNN
F 3 "~" H 6000 3950 50  0001 C CNN
	1    6000 3950
	1    0    0    -1  
$EndComp
Wire Wire Line
	6000 3800 6000 3750
$EndSCHEMATC