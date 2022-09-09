# Comparing-USB-multimeter-vs-INA219-current-sensor
Using STM32F429 microcontroller current consumptions in low power modes is observed using USB multimeter and compared with current sensor.
Stm32F429 has maximum operating frequency upto 180 MHz.
Intially current is observed in Run mode with different clock frequencies with all peripherals are enabled and disbaled. 
Clock (MHz)	Modes	Peripherals used	USB		Current sensor	
			Current (mA)	Power 	Current	Power
180	RUN	No	149	758	1938	492
150			139	712	1820	462
100			123	632	1598	405
50			113	576	1474	378
180		Connectivity	162	828	2009	510
150			151	768	1966	498
100			130	666	1653	419
50			117	601	1548	382
180		Ethernet	163	828	2013	511
150			141	717	1856	488
100			125	621	1623	410
50			115	576	1508	371
![image](https://user-images.githubusercontent.com/79939325/189425500-2dabe113-2fce-48c3-8453-cdaf3ac83805.png)


