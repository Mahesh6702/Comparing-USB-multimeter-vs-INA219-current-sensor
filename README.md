# Comparing-USB-multimeter-vs-INA219-current-sensor
Using STM32F429 microcontroller current, power consumpitons in low power modes is observed using USB multimeter and compared with current sensor.
Stm32F429 has maximum operating frequency upto 180 MHz.
Intially current is observed in Run mode with different clock frequencies with all peripherals are enabled and disbaled. 

![image](https://user-images.githubusercontent.com/79939325/189425500-2dabe113-2fce-48c3-8453-cdaf3ac83805.png)

In the above image connectivity means SPI, I2C, CAN, ADC's, DAC's etc.
As you can observed current consumpiton is higher in Run mode with higher frequency, So it advisable to run the application in lower frequency. If the application is not needed to run in high frequency.

INA219 sensor provides RAW current, power and voltage data. In the above image, RAW data is compared with USB data.
Basic regression is applied on the test data, to get equation which provide current in mA, along with voltage in volts and power in mW.
![current_relation_with_regression](https://user-images.githubusercontent.com/79939325/200432390-4e135330-aa00-4e74-8ffd-bed0e739af11.jpg)
![power_relation_with_regression](https://user-images.githubusercontent.com/79939325/200432411-480ac3e4-b127-41f1-a8e0-0731e401d4c4.jpg)
Above are the plots how regression is applied on current and power data which is taken from RAW sensor and USB data.
