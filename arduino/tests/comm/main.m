clear all;
close all;

A = load('accel.dat');

g = 9.81;
A = A / 2^15 * 2 * g;

plot3(A(:,1),A(:,2),A(:,3));
pause;

dt = .05;
V = zeros(size(A));
for i = 2:1000
	V(i,:) = V(i-1,:) + dt * A(i,:);
end

R = zeros(size(A));
for i = 2:1000
	R(i,:) = R(i-1,:) + dt * V(i,:);
end

A = V;

%for i=1:1000
	plot3(A(:,1), A(:,2), A(:,3));
%end
