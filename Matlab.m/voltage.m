clc
close all;
A = dlmread('voltagerelationship.txt');
x = A(:,1);
y = A(:,2);
plot(x,y,'b.');
legend('usb vs sensor');
hold on
title('Voltage relationship');
ylabel('USB data (mW)');
xlabel('Raw sensor data');