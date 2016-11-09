clear all;
close all;

dists = load('dists.dat');
states = load('states.dat');

states_str = {};
states_str{1} = sprintf('Frente: medio\nTraseira: medio');
states_str{2} = sprintf('Frente: longe\nTraseira: medio');
states_str{3} = sprintf('Frente: perto\nTraseira: medio');
states_str{4} = sprintf('Frente: perto\nTraseira: perto');
states_str{5} = sprintf('Frente: medio\nTraseira: perto');
states_str{6} = sprintf('Frente: longe\nTraseira: perto');
states_str{7} = sprintf('Frente: longe\nTraseira: longe');
states_str{8} = sprintf('Frente: medio\nTraseira: longe');
states_str{9} = sprintf('Frente: perto\nTraseira: longe');

valid_dists = dists < 50;
valid_vals = valid_dists(:,1) & valid_dists(:,2);

dists = dists(valid_vals, :);
states = states(valid_vals);

% moving average filter (we can do better than this)
win_size = 10;
mov_avg_win = ones(win_size, 1) / win_size;

% filtering the distance vectors
front = conv(dists(:,1), mov_avg_win);
rear = conv(dists(:,2), mov_avg_win);

for i=1:size(front,1);
	plot(-[front(i) rear(i)], [1 -1], 'k', 'LineWidth', 3,...
		[0 0], [-2 2], 'r', 'LineWidth', 5,...
		-[10 10], [-2 2], 'b--', 'LineWidth', 5,...
		-[20 20], [-2 2], 'b--', 'LineWidth', 5);
	axis([-30 30 -2 2]);
	axis('off');
	text(1, 1, states_str{states(i)});
	pause(.01);
end
