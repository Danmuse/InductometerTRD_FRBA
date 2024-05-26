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
L MCU_Microchip_ATtiny:ATtiny85-20PU U1
U 1 1 610B5C2F
P 4400 2325
F 0 "U1" H 3871 2371 50  0000 R CNN
F 1 "ATtiny85-20PU" H 3871 2280 50  0000 R CNN
F 2 "Package_DIP:DIP-8_W7.62mm_Socket_LongPads" H 4400 2325 50  0001 C CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/atmel-2586-avr-8-bit-microcontroller-attiny25-attiny45-attiny85_datasheet.pdf" H 4400 2325 50  0001 C CNN
	1    4400 2325
	1    0    0    -1  
$EndComp
$Comp
L Comparator:LM393 U2
U 1 1 610B6246
P 6575 2600
F 0 "U2" H 6675 2875 50  0000 C CNN
F 1 "LM393" H 6675 2775 50  0000 C CNN
F 2 "Package_DIP:DIP-8_W7.62mm_Socket_LongPads" H 6575 2600 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/lm393.pdf" H 6575 2600 50  0001 C CNN
	1    6575 2600
	1    0    0    -1  
$EndComp
$Comp
L Comparator:LM393 U2
U 3 1 610B64ED
P 6575 2600
F 0 "U2" H 6600 2425 50  0000 L CNN
F 1 "LM393" H 6550 2325 50  0000 L CNN
F 2 "Package_DIP:DIP-8_W7.62mm_Socket_LongPads" H 6575 2600 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/lm393.pdf" H 6575 2600 50  0001 C CNN
	3    6575 2600
	1    0    0    -1  
$EndComp
$Comp
L Device:R R1
U 1 1 610B7AB6
P 5600 2225
F 0 "R1" V 5700 2225 50  0000 C CNN
F 1 "220" V 5600 2225 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 5530 2225 50  0001 C CNN
F 3 "~" H 5600 2225 50  0001 C CNN
	1    5600 2225
	0    1    1    0   
$EndComp
$Comp
L Device:C C1
U 1 1 610B8020
P 5800 2725
F 0 "C1" H 5915 2771 50  0000 L CNN
F 1 "1u" H 5915 2680 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D3.8mm_W2.6mm_P2.50mm" H 5838 2575 50  0001 C CNN
F 3 "~" H 5800 2725 50  0001 C CNN
	1    5800 2725
	1    0    0    -1  
$EndComp
$Comp
L Device:LED D1
U 1 1 610B84A5
P 5250 2025
F 0 "D1" H 5243 1770 50  0000 C CNN
F 1 "LED" H 5243 1861 50  0000 C CNN
F 2 "LED_THT:LED_D3.0mm" H 5250 2025 50  0001 C CNN
F 3 "~" H 5250 2025 50  0001 C CNN
	1    5250 2025
	-1   0    0    1   
$EndComp
$Comp
L Connector:Conn_01x04_Male J1
U 1 1 610B8B6C
P 5425 2525
F 0 "J1" H 5600 2700 50  0000 R CNN
F 1 "Display" H 5700 2800 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 5425 2525 50  0001 C CNN
F 3 "~" H 5425 2525 50  0001 C CNN
	1    5425 2525
	-1   0    0    1   
$EndComp
Wire Wire Line
	5800 2875 5800 2950
Wire Wire Line
	5800 2950 6150 2950
Wire Wire Line
	6475 2950 6475 2900
Wire Wire Line
	6275 2700 6150 2700
Wire Wire Line
	6150 2700 6150 2950
Connection ~ 6150 2950
Wire Wire Line
	6150 2950 6475 2950
Wire Wire Line
	6275 2500 5800 2500
Wire Wire Line
	5800 2575 5800 2500
Connection ~ 5800 2500
NoConn ~ 5000 2525
Wire Wire Line
	5225 2525 5075 2525
Wire Wire Line
	5075 2525 5075 1700
Wire Wire Line
	4400 1700 4400 1725
Wire Wire Line
	5000 2025 5100 2025
Wire Wire Line
	4400 2950 4400 2925
Wire Wire Line
	5000 2225 5450 2225
Wire Wire Line
	5750 2225 5800 2225
Wire Wire Line
	5800 2225 5800 2500
Connection ~ 5800 2950
Wire Wire Line
	5225 2625 5075 2625
Wire Wire Line
	5075 2625 5075 2950
Connection ~ 5075 2950
Wire Wire Line
	5075 2950 5800 2950
Wire Wire Line
	4400 2950 5075 2950
Wire Wire Line
	5000 2325 5225 2325
Wire Wire Line
	5000 2425 5225 2425
Wire Wire Line
	4400 1700 5075 1700
Wire Wire Line
	5075 1700 6475 1700
Wire Wire Line
	6475 1700 6475 2300
Connection ~ 5075 1700
Wire Wire Line
	5400 2025 6150 2025
Wire Wire Line
	6150 2025 6150 2700
Connection ~ 6150 2700
Wire Wire Line
	5000 2125 6950 2125
Wire Wire Line
	6950 2125 6950 2600
Wire Wire Line
	6950 2600 6875 2600
Connection ~ 6475 2950
$Comp
L Connector:Screw_Terminal_01x02 J3
U 1 1 610E3B4C
P 7350 3050
F 0 "J3" H 7268 2725 50  0000 C CNN
F 1 "Lx" H 7268 2816 50  0000 C CNN
F 2 "TerminalBlock:TerminalBlock_bornier-2_P5.08mm" H 7350 3050 50  0001 C CNN
F 3 "~" H 7350 3050 50  0001 C CNN
	1    7350 3050
	1    0    0    1   
$EndComp
Wire Wire Line
	5800 2500 5600 2500
Wire Wire Line
	5600 2500 5600 3050
Wire Wire Line
	5600 3050 7150 3050
$Comp
L Device:Battery_Cell BT1
U 1 1 610EF49E
P 7100 2400
F 0 "BT1" H 7218 2496 50  0000 L CNN
F 1 "CR2032" H 7218 2405 50  0000 L CNN
F 2 "Battery:BatteryHolder_Keystone_103_1x20mm" V 7100 2460 50  0001 C CNN
F 3 "~" V 7100 2460 50  0001 C CNN
	1    7100 2400
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_DPDT_x2 SW1
U 1 1 610F1D47
P 6800 1700
F 0 "SW1" H 6800 1985 50  0000 C CNN
F 1 "SW_DPDT_x2" H 6800 1894 50  0000 C CNN
F 2 "Button_Switch_THT:SW_CuK_JS202011AQN_DPDT_Angled" H 6800 1700 50  0001 C CNN
F 3 "~" H 6800 1700 50  0001 C CNN
	1    6800 1700
	1    0    0    -1  
$EndComp
Wire Wire Line
	6475 1700 6600 1700
Connection ~ 6475 1700
Wire Wire Line
	7000 1800 7100 1800
NoConn ~ 7000 1600
Wire Wire Line
	6475 2950 7100 2950
Wire Wire Line
	7100 1800 7100 2200
Wire Wire Line
	7100 2500 7100 2950
Connection ~ 7100 2950
Wire Wire Line
	7100 2950 7150 2950
$EndSCHEMATC