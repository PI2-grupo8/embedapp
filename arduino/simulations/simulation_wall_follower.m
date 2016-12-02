clear all;
close all;

figure('visible', 'off');

dists = load('dists.dat');
states = load('states.dat');

states_str = {};
states_str_front{1} = sprintf('Frente: medio');
states_str_front{2} = sprintf('Frente: longe');
states_str_front{3} = sprintf('Frente: perto');
states_str_front{4} = sprintf('Frente: perto');
states_str_front{5} = sprintf('Frente: medio');
states_str_front{6} = sprintf('Frente: longe');
states_str_front{7} = sprintf('Frente: longe');
states_str_front{8} = sprintf('Frente: medio');
states_str_front{9} = sprintf('Frente: perto');

states_str_back{1} = sprintf('Traseira: medio');
states_str_back{2} = sprintf('Traseira: medio');
states_str_back{3} = sprintf('Traseira: medio');
states_str_back{4} = sprintf('Traseira: perto');
states_str_back{5} = sprintf('Traseira: perto');
states_str_back{6} = sprintf('Traseira: perto');
states_str_back{7} = sprintf('Traseira: longe');
states_str_back{8} = sprintf('Traseira: longe');
states_str_back{9} = sprintf('Traseira: longe');

state_angles = [0 -45 45 30 -45 -60 -30 45 60]*pi/180;

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

front = front/10;
rear = rear/10;

for i=1:size(front,1);
	hold off;
	plot(-[front(i) rear(i)], [1 -1], 'k', 'LineWidth', 3,...
		[0 0], [-2 2], 'r', 'LineWidth', 5,...
		-[1 1], [-2 2], 'b--', 'LineWidth', 5,...
		-[2 2], [-2 2], 'b--', 'LineWidth', 5);

	hold on;
	th = atan2(front(i)-rear(i),2)+pi/2;
	v_x = cos(state_angles(states(i))+th);
	v_y = sin(state_angles(states(i))+th);
	quiver(-front(i), 1, v_x, v_y, 'b', 'LineWidth', 2);

	axis equal;
	axis([-3 2 -2 2]);
	axis('off');
	text(1, 1, states_str_front{states(i)});
	text(1, -1, states_str_back{states(i)});

	print(sprintf('video/%05d.png', i));

	%pause(.01);
end
