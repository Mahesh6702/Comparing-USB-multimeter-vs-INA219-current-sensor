clc
close all;
A = dlmread('currentrelationship.txt');
y = A(:,1);
x = A(:,2);
plot(x,y,'b.');
legend('usb vs sensor');
hold on
title('Current relationship');
ylabel('USB data (Ma)');
xlabel('Raw sensor data');