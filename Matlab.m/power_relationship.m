clc
close all;
A = dlmread('powerrelationship.txt');
y = A(:,1);
x = A(:,2);
plot(x,y,'b.');
legend('usb vs sensor');
hold on
title('Power relationship');
ylabel('USB data (mW)');
xlabel('Raw sensor data');