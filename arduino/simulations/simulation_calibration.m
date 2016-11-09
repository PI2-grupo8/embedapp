clear all;
close all;

rel_hum = [60; 30; 5];
higro_val = [50; 100; 1000];

sz = size(higro_val);
higro_val = higro_val + round(10 * rand(sz) - 5);

[a, b] = regression(higro_val, rel_hum);

hum_vals = [5; 10; 15; 20; 25; 30; 40; 50; 60];
sz = size(hum_vals);

fake_vals = a * hum_vals .^ b + round(20 * rand(sz) - 10);

[a, b] = regression(fake_vals, hum_vals);

x = linspace(0, 1023, 1000);
y = a * x .^ b;

plot(fake_vals, hum_vals, 'r^');
hold on; plot(x, y, 'b-');
legend('Pontos de calibração', sprintf('Melhor ajuste: y = %.2f * x^{%.2f}', a, b));
title('Pontos da calibração');
xlabel('Leitura do sensor');
ylabel('Umidade gravimétrica (%)');
axis([0 1023 0 100]); 
