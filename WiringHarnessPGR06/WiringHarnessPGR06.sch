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
$Comp
L WiringHarnessLib:EMU_Black ECU?
U 1 1 61BFA62D
P 9200 1100
F 0 "ECU?" H 8989 1165 50  0000 C CNN
F 1 "EMU_Black" H 8989 1074 50  0000 C CNN
F 2 "" H 9200 1150 50  0001 C CNN
F 3 "" H 9200 1150 50  0001 C CNN
	1    9200 1100
	1    0    0    -1  
$EndComp
Text GLabel 4525 1375 0    50   Input ~ 0
WBO_6
Text GLabel 4525 1475 0    50   Input ~ 0
WBO_5
Text GLabel 4525 1575 0    50   Input ~ 0
WBO_4
Text GLabel 4525 1675 0    50   Input ~ 0
WBO_3
Text GLabel 4525 1775 0    50   Input ~ 0
WBO_2
Text GLabel 4525 1875 0    50   Input ~ 0
WBO_1
$Comp
L WiringHarnessLib:LSU4.9 LSU?
U 1 1 61BFDCF4
P 5150 1975
F 0 "LSU?" H 4472 1579 50  0000 R CNN
F 1 "LSU4.9" H 4472 1670 50  0000 R CNN
F 2 "" H 5150 1975 50  0001 C CNN
F 3 "" H 5150 1975 50  0001 C CNN
	1    5150 1975
	-1   0    0    1   
$EndComp
Wire Wire Line
	4525 1375 4900 1375
Wire Wire Line
	4525 1475 4900 1475
Wire Wire Line
	4525 1575 4900 1575
Wire Wire Line
	4525 1675 4900 1675
Wire Wire Line
	4525 1775 4900 1775
Wire Wire Line
	4525 1875 4900 1875
Text GLabel 9500 2350 2    50   Input ~ 0
WBO_6
Text GLabel 9500 2800 2    50   Input ~ 0
WBO_2
Text GLabel 9525 2650 2    50   Input ~ 0
WBO_5
Text GLabel 9500 2500 2    50   Input ~ 0
WBO_1
Text GLabel 8400 1750 0    50   Input ~ 0
WBO_3
Text GLabel 4500 2125 0    50   Input ~ 0
WBO_4
$Comp
L Device:Fuse WBO_Fuse
U 1 1 61C06DBA
P 4900 2125
F 0 "WBO_Fuse" V 4725 2000 50  0000 C CNN
F 1 "5A" V 4794 2125 50  0000 C CNN
F 2 "" V 4830 2125 50  0001 C CNN
F 3 "~" H 4900 2125 50  0001 C CNN
	1    4900 2125
	0    1    1    0   
$EndComp
Wire Wire Line
	4500 2125 4750 2125
$Comp
L power:+12V #PWR?
U 1 1 61C076DF
P 5250 2125
F 0 "#PWR?" H 5250 1975 50  0001 C CNN
F 1 "+12V" V 5265 2253 50  0000 L CNN
F 2 "" H 5250 2125 50  0001 C CNN
F 3 "" H 5250 2125 50  0001 C CNN
	1    5250 2125
	0    1    1    0   
$EndComp
Wire Wire Line
	5050 2125 5250 2125
$Comp
L WiringHarnessLib:ACDCREGULATOR REG?
U 1 1 61C0928F
P 5500 2400
F 0 "REG?" H 5500 2515 50  0001 C CNN
F 1 "VOLTAGE_REGULATOR" H 5500 2424 50  0000 C CNN
F 2 "" H 5500 2400 50  0001 C CNN
F 3 "" H 5500 2400 50  0001 C CNN
	1    5500 2400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 61C0A9EA
P 4975 2550
F 0 "#PWR?" H 4975 2300 50  0001 C CNN
F 1 "GND" V 4980 2422 50  0000 R CNN
F 2 "" H 4975 2550 50  0001 C CNN
F 3 "" H 4975 2550 50  0001 C CNN
	1    4975 2550
	0    1    1    0   
$EndComp
Wire Wire Line
	4975 2550 5200 2550
$Comp
L Device:Fuse REG_Fuse
U 1 1 61C0B7BA
P 4550 2650
F 0 "REG_Fuse" V 4353 2650 50  0000 C CNN
F 1 "70A" V 4444 2650 50  0000 C CNN
F 2 "" V 4480 2650 50  0001 C CNN
F 3 "~" H 4550 2650 50  0001 C CNN
	1    4550 2650
	0    1    1    0   
$EndComp
Wire Wire Line
	4700 2650 5200 2650
$Comp
L power:+12V #PWR?
U 1 1 61C0D484
P 4400 2650
F 0 "#PWR?" H 4400 2500 50  0001 C CNN
F 1 "+12V" V 4415 2778 50  0000 L CNN
F 2 "" H 4400 2650 50  0001 C CNN
F 3 "" H 4400 2650 50  0001 C CNN
	1    4400 2650
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5800 2500 6400 2500
Wire Wire Line
	6400 2500 6400 2600
Wire Wire Line
	6400 2600 5800 2600
Wire Wire Line
	5800 2700 6400 2700
Wire Wire Line
	6400 2700 6400 2600
Connection ~ 6400 2600
Text GLabel 8025 2350 0    50   Input ~ 0
INJ#1
Text GLabel 8025 2500 0    50   Input ~ 0
INJ#2
Text GLabel 8025 2650 0    50   Input ~ 0
INJ#3
Wire Wire Line
	8025 2650 8400 2650
Wire Wire Line
	8025 2500 8400 2500
Wire Wire Line
	8025 2350 8400 2350
$Comp
L WiringHarnessLib:PATRYKBOARDSIMPLIFIED PBS?
U 1 1 61C18CE8
P 4975 3125
F 0 "PBS?" H 5075 3190 50  0000 C CNN
F 1 "PATRYKBOARDSIMPLIFIED" H 5075 3099 50  0000 C CNN
F 2 "" H 5075 3125 50  0001 C CNN
F 3 "" H 5075 3125 50  0001 C CNN
	1    4975 3125
	1    0    0    -1  
$EndComp
Text GLabel 4250 3325 0    50   Input ~ 0
INJ#1
Text GLabel 4250 3475 0    50   Input ~ 0
INJ#2
Text GLabel 4250 3625 0    50   Input ~ 0
INJ#3
Wire Wire Line
	4250 3625 4625 3625
Wire Wire Line
	4250 3475 4625 3475
Wire Wire Line
	4250 3325 4625 3325
$Comp
L Device:L INJECTOR#1
U 1 1 61C1A36B
P 5950 3325
F 0 "INJECTOR#1" V 5860 3325 50  0000 C CNN
F 1 "L" V 5860 3325 50  0001 C CNN
F 2 "" H 5950 3325 50  0001 C CNN
F 3 "~" H 5950 3325 50  0001 C CNN
	1    5950 3325
	0    1    1    0   
$EndComp
$Comp
L Device:L INJECTOR#2
U 1 1 61C1B027
P 6300 3475
F 0 "INJECTOR#2" V 6210 3475 50  0000 C CNN
F 1 "L" V 6210 3475 50  0001 C CNN
F 2 "" H 6300 3475 50  0001 C CNN
F 3 "~" H 6300 3475 50  0001 C CNN
	1    6300 3475
	0    1    1    0   
$EndComp
$Comp
L Device:L INJECTOR#3
U 1 1 61C1BCD4
P 6625 3625
F 0 "INJECTOR#3" V 6535 3625 50  0000 C CNN
F 1 "L" V 6535 3625 50  0001 C CNN
F 2 "" H 6625 3625 50  0001 C CNN
F 3 "~" H 6625 3625 50  0001 C CNN
	1    6625 3625
	0    1    1    0   
$EndComp
Wire Wire Line
	5625 3325 5800 3325
Wire Wire Line
	5625 3475 6150 3475
Wire Wire Line
	5625 3625 6475 3625
Wire Wire Line
	6100 3325 7000 3325
Wire Wire Line
	7000 3325 7000 3475
Wire Wire Line
	7000 3625 6775 3625
Wire Wire Line
	6450 3475 7000 3475
Connection ~ 7000 3475
Wire Wire Line
	7000 3475 7000 3625
$Comp
L Device:Fuse INJECTOR_FUSE
U 1 1 61C1F3C7
P 7150 3475
F 0 "INJECTOR_FUSE" V 6953 3475 50  0000 C CNN
F 1 "15A" V 7044 3475 50  0000 C CNN
F 2 "" V 7080 3475 50  0001 C CNN
F 3 "~" H 7150 3475 50  0001 C CNN
	1    7150 3475
	0    1    1    0   
$EndComp
$Comp
L power:+12V #PWR?
U 1 1 61C1FF9A
P 7300 3475
F 0 "#PWR?" H 7300 3325 50  0001 C CNN
F 1 "+12V" V 7315 3603 50  0000 L CNN
F 2 "" H 7300 3475 50  0001 C CNN
F 3 "" H 7300 3475 50  0001 C CNN
	1    7300 3475
	0    1    1    0   
$EndComp
$Comp
L Device:L COIL#1
U 1 1 61C21B37
P 7975 1300
F 0 "COIL#1" V 7885 1300 50  0000 C CNN
F 1 "L" V 7885 1300 50  0001 C CNN
F 2 "" H 7975 1300 50  0001 C CNN
F 3 "~" H 7975 1300 50  0001 C CNN
	1    7975 1300
	0    1    1    0   
$EndComp
$Comp
L Device:L COIL#2
U 1 1 61C228C5
P 7675 1450
F 0 "COIL#2" V 7585 1450 50  0000 C CNN
F 1 "L" V 7585 1450 50  0001 C CNN
F 2 "" H 7675 1450 50  0001 C CNN
F 3 "~" H 7675 1450 50  0001 C CNN
	1    7675 1450
	0    1    1    0   
$EndComp
$Comp
L Device:L COIL#3
U 1 1 61C243A8
P 7375 1600
F 0 "COIL#3" V 7285 1600 50  0000 C CNN
F 1 "L" V 7285 1600 50  0001 C CNN
F 2 "" H 7375 1600 50  0001 C CNN
F 3 "~" H 7375 1600 50  0001 C CNN
	1    7375 1600
	0    1    1    0   
$EndComp
Wire Wire Line
	7525 1600 8400 1600
Wire Wire Line
	7825 1450 8400 1450
Wire Wire Line
	8125 1300 8400 1300
Wire Wire Line
	7825 1300 7225 1300
Wire Wire Line
	7225 1300 7225 1450
Wire Wire Line
	7525 1450 7225 1450
Connection ~ 7225 1450
Wire Wire Line
	7225 1450 7225 1600
$Comp
L Device:Fuse COIL_FUSE
U 1 1 61C2A297
P 7075 1450
F 0 "COIL_FUSE" V 6878 1450 50  0000 C CNN
F 1 "15A" V 6969 1450 50  0000 C CNN
F 2 "" V 7005 1450 50  0001 C CNN
F 3 "~" H 7075 1450 50  0001 C CNN
	1    7075 1450
	0    1    1    0   
$EndComp
$Comp
L power:+12V #PWR?
U 1 1 61C2C7EB
P 6925 1450
F 0 "#PWR?" H 6925 1300 50  0001 C CNN
F 1 "+12V" V 6940 1578 50  0000 L CNN
F 2 "" H 6925 1450 50  0001 C CNN
F 3 "" H 6925 1450 50  0001 C CNN
	1    6925 1450
	0    -1   -1   0   
$EndComp
$Comp
L WiringHarnessLib:BOSCH_DBW DBW?
U 1 1 61C01C2F
P 5075 4275
F 0 "DBW?" H 5453 4016 50  0000 L CNN
F 1 "BOSCH_DBW" H 5453 3925 50  0000 L CNN
F 2 "" H 5075 4275 50  0001 C CNN
F 3 "" H 5075 4275 50  0001 C CNN
	1    5075 4275
	1    0    0    -1  
$EndComp
Wire Wire Line
	4975 4575 4975 4325
Wire Wire Line
	4975 4325 5175 4325
Wire Wire Line
	5175 4325 5175 4175
Wire Wire Line
	4925 4675 4925 4375
Wire Wire Line
	4925 4375 4875 4375
Wire Wire Line
	4875 4375 4875 4175
Wire Wire Line
	5275 4575 5375 4575
Wire Wire Line
	5375 4575 5375 4175
Wire Wire Line
	5125 4575 5125 4375
Wire Wire Line
	5125 4375 5275 4375
Wire Wire Line
	5275 4375 5275 4175
Wire Wire Line
	5225 4675 5150 4675
Wire Wire Line
	5150 4675 5150 4600
Wire Wire Line
	5150 4600 5075 4600
Wire Wire Line
	5075 4600 5075 4175
Wire Wire Line
	5075 4675 5025 4675
Wire Wire Line
	5025 4675 5025 4175
Wire Wire Line
	5025 4175 4975 4175
Wire Wire Line
	8400 1900 7900 1900
Wire Wire Line
	7900 1900 7900 1975
Wire Wire Line
	7900 2050 8400 2050
Wire Wire Line
	7900 1975 7800 1975
Connection ~ 7900 1975
Wire Wire Line
	7900 1975 7900 2050
$Comp
L power:GND #PWR?
U 1 1 61C0AEAB
P 7800 1975
F 0 "#PWR?" H 7800 1725 50  0001 C CNN
F 1 "GND" H 7805 1802 50  0000 C CNN
F 2 "" H 7800 1975 50  0001 C CNN
F 3 "" H 7800 1975 50  0001 C CNN
	1    7800 1975
	1    0    0    -1  
$EndComp
Wire Wire Line
	9500 2200 9725 2200
$Comp
L power:GND #PWR?
U 1 1 61C0D30B
P 9725 2200
F 0 "#PWR?" H 9725 1950 50  0001 C CNN
F 1 "GND" V 9730 2072 50  0000 R CNN
F 2 "" H 9725 2200 50  0001 C CNN
F 3 "" H 9725 2200 50  0001 C CNN
	1    9725 2200
	0    -1   -1   0   
$EndComp
$Comp
L power:+12V #PWR?
U 1 1 61C0E0E8
P 9500 3850
F 0 "#PWR?" H 9500 3700 50  0001 C CNN
F 1 "+12V" H 9515 4023 50  0000 C CNN
F 2 "" H 9500 3850 50  0001 C CNN
F 3 "" H 9500 3850 50  0001 C CNN
	1    9500 3850
	-1   0    0    1   
$EndComp
$Comp
L Device:Thermistor CLT
U 1 1 61C0FE1D
P 10200 2700
F 0 "CLT" H 10250 2700 50  0000 R CNN
F 1 "Thermistor" H 10095 2745 50  0001 R CNN
F 2 "" H 10200 2700 50  0001 C CNN
F 3 "~" H 10200 2700 50  0001 C CNN
	1    10200 2700
	-1   0    0    1   
$EndComp
$Comp
L Device:Thermistor IAT
U 1 1 61C151BD
P 10400 2700
F 0 "IAT" H 10450 2700 50  0000 R CNN
F 1 "Thermistor" H 10295 2745 50  0001 R CNN
F 2 "" H 10400 2700 50  0001 C CNN
F 3 "~" H 10400 2700 50  0001 C CNN
	1    10400 2700
	-1   0    0    1   
$EndComp
$Comp
L Device:Thermistor KN
U 1 1 61C1A3A3
P 10000 2700
F 0 "KN" H 10050 2700 50  0000 R CNN
F 1 "Thermistor" H 9895 2745 50  0001 R CNN
F 2 "" H 10000 2700 50  0001 C CNN
F 3 "~" H 10000 2700 50  0001 C CNN
	1    10000 2700
	-1   0    0    1   
$EndComp
Wire Wire Line
	9500 2950 10000 2950
Wire Wire Line
	10000 2950 10000 2900
Wire Wire Line
	10000 2500 10000 1900
Wire Wire Line
	9500 1900 10000 1900
Wire Wire Line
	9500 3250 10200 3250
Wire Wire Line
	10200 3250 10200 2900
Wire Wire Line
	9500 1750 10200 1750
Wire Wire Line
	10200 1750 10200 2500
Wire Wire Line
	9500 3550 10400 3550
Wire Wire Line
	10400 3550 10400 2900
Wire Wire Line
	9500 1600 10400 1600
Wire Wire Line
	10400 1600 10400 2500
Wire Wire Line
	9500 2050 9725 2050
$Comp
L power:GND #PWR?
U 1 1 61C341C9
P 9725 2050
F 0 "#PWR?" H 9725 1800 50  0001 C CNN
F 1 "GND" V 9730 1922 50  0000 R CNN
F 2 "" H 9725 2050 50  0001 C CNN
F 3 "" H 9725 2050 50  0001 C CNN
	1    9725 2050
	0    -1   -1   0   
$EndComp
$Comp
L WiringHarnessLib:EGTTOCAN EGTTOCAN?
U 1 1 61C36B5B
P 2900 1375
F 0 "EGTTOCAN?" H 2900 1375 50  0001 C CNN
F 1 "EGTTOCAN" H 3000 1348 50  0000 C CNN
F 2 "" H 2900 1375 50  0001 C CNN
F 3 "" H 2900 1375 50  0001 C CNN
	1    2900 1375
	1    0    0    -1  
$EndComp
$Comp
L power:+12V #PWR?
U 1 1 61C37C58
P 3400 1875
F 0 "#PWR?" H 3400 1725 50  0001 C CNN
F 1 "+12V" V 3415 2003 50  0000 L CNN
F 2 "" H 3400 1875 50  0001 C CNN
F 3 "" H 3400 1875 50  0001 C CNN
	1    3400 1875
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 61C386FD
P 3400 1775
F 0 "#PWR?" H 3400 1525 50  0001 C CNN
F 1 "GND" V 3405 1647 50  0000 R CNN
F 2 "" H 3400 1775 50  0001 C CNN
F 3 "" H 3400 1775 50  0001 C CNN
	1    3400 1775
	0    -1   -1   0   
$EndComp
$Comp
L Connector:Conn_01x02_Female EGT#1
U 1 1 61C39A2E
P 1725 1375
F 0 "EGT#1" H 1617 1142 50  0000 C CNN
F 1 "Conn_01x02_Female" H 1617 1141 50  0001 C CNN
F 2 "" H 1725 1375 50  0001 C CNN
F 3 "~" H 1725 1375 50  0001 C CNN
	1    1725 1375
	-1   0    0    1   
$EndComp
$Comp
L Connector:Conn_01x02_Female EGT#2
U 1 1 61C3E4F0
P 1725 1750
F 0 "EGT#2" H 1617 1517 50  0000 C CNN
F 1 "Conn_01x02_Female" H 1617 1516 50  0001 C CNN
F 2 "" H 1725 1750 50  0001 C CNN
F 3 "~" H 1725 1750 50  0001 C CNN
	1    1725 1750
	-1   0    0    1   
$EndComp
$Comp
L Connector:Conn_01x02_Female EGT#3
U 1 1 61C3FF86
P 1725 2125
F 0 "EGT#3" H 1617 1892 50  0000 C CNN
F 1 "Conn_01x02_Female" H 1617 1891 50  0001 C CNN
F 2 "" H 1725 2125 50  0001 C CNN
F 3 "~" H 1725 2125 50  0001 C CNN
	1    1725 2125
	-1   0    0    1   
$EndComp
$Comp
L Connector:Conn_01x02_Female EGT#4
U 1 1 61C41947
P 1750 2450
F 0 "EGT#4" H 1642 2217 50  0000 C CNN
F 1 "Conn_01x02_Female" H 1642 2216 50  0001 C CNN
F 2 "" H 1750 2450 50  0001 C CNN
F 3 "~" H 1750 2450 50  0001 C CNN
	1    1750 2450
	-1   0    0    1   
$EndComp
Wire Wire Line
	1950 2450 2175 2450
Wire Wire Line
	2175 2450 2175 2125
Wire Wire Line
	2175 2125 1925 2125
Wire Wire Line
	2175 2125 2175 1975
Wire Wire Line
	2175 1750 1925 1750
Connection ~ 2175 2125
Wire Wire Line
	1925 1375 2175 1375
Wire Wire Line
	2175 1375 2175 1750
Connection ~ 2175 1750
Wire Wire Line
	2600 1975 2175 1975
Connection ~ 2175 1975
Wire Wire Line
	2175 1975 2175 1750
Wire Wire Line
	1950 2350 2500 2350
Wire Wire Line
	2500 2350 2500 1875
Wire Wire Line
	2500 1875 2600 1875
Wire Wire Line
	1925 2025 2000 2025
Wire Wire Line
	2000 2025 2000 1775
Wire Wire Line
	2000 1775 2600 1775
Wire Wire Line
	2600 1675 1925 1675
Wire Wire Line
	1925 1675 1925 1650
Wire Wire Line
	1925 1275 2600 1275
Wire Wire Line
	2600 1275 2600 1575
Text GLabel 9925 1300 2    50   Input ~ 0
CAN_H
Wire Wire Line
	9500 1300 9925 1300
Text GLabel 9925 1450 2    50   Input ~ 0
CAN_L
Text Label 9675 1300 0    50   ~ 0
violet
Wire Wire Line
	9925 1450 9500 1450
Text Label 9700 1450 0    50   ~ 0
blue
Text GLabel 3400 1575 2    50   Input ~ 0
CAN_H
Text GLabel 3400 1675 2    50   Input ~ 0
CAN_L
Text Label 6025 2700 0    50   ~ 0
black
Text Label 6075 2500 0    50   ~ 0
black
Text Label 6050 2600 0    50   ~ 0
black
Wire Wire Line
	6400 2600 6825 2600
Text Label 6550 2600 0    50   ~ 0
ALTERNATOR
$Comp
L Motor:Motor_DC Starter
U 1 1 61C7893E
P 1950 3250
F 0 "Starter" V 1655 3200 50  0000 C CNN
F 1 "Motor_DC" V 1746 3200 50  0000 C CNN
F 2 "" H 1950 3160 50  0001 C CNN
F 3 "~" H 1950 3160 50  0001 C CNN
	1    1950 3250
	0    1    1    0   
$EndComp
Wire Wire Line
	1650 3250 1650 3525
$Comp
L power:GND #PWR?
U 1 1 61C84EEB
P 1650 3525
F 0 "#PWR?" H 1650 3275 50  0001 C CNN
F 1 "GND" H 1655 3352 50  0000 C CNN
F 2 "" H 1650 3525 50  0001 C CNN
F 3 "" H 1650 3525 50  0001 C CNN
	1    1650 3525
	1    0    0    -1  
$EndComp
$EndSCHEMATC