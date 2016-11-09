function [a, b] = regression(x, y);

x = x(:);
y = y(:);

sz = size(x);

A = [ones(sz) log(x)];
b = log(y);

th = A \ b;

a = exp(th(1));
b = th(2);
