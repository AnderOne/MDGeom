function draw(fname, xmin, xmax, ymin, ymax, zmin, zmax, tmin, tmax, gif)

	finp = fopen(fname, 'rt'); tnum = fscanf(finp, '%i', 1);

	lim = (exist('xmin') && exist('xmax') && ...
	       exist('ymin') && exist('ymax') && ...
	       exist('zmin') && exist('zmax'));
	
	if (~exist('tmax')); tmax = tnum; end
	if (~exist('tmin')); tmin = 1; end
	if (~exist('gif'))
		gif = false;
	end
	
	for t = 1 : tnum

		%Считываем сетку из файла:
		vnum = fscanf(finp, '%i', 1); vert = fscanf(finp, '%f', [3, vnum]).';
		enum = fscanf(finp, '%i', 1); edge = fscanf(finp, '%i', [2, enum]).';
		edge = edge + 1;
		fnum = fscanf(finp, '%i', 1);
		for i = 1 : fnum
			face{i} = fscanf(finp, '%i', fscanf(finp, '%i', 1)).';
			face{i} = face{i} + 1;
		end

		if (t < tmin) || (t > tmax); continue; end

		if (fnum == 0); clf(); end

		%Отрисовка граней:
		for i = 1 : fnum
			p = vert(vert_list_by_edge(face{i}, edge), :);
			fill3(p(:, 1), p(:, 2), p(:, 3), 'b', 'FaceAlpha', 0.45)
			hold on
		end
		%Отрисовка ребер:
		for i = 1 : enum
			p = vert(edge(i, :), :);
			plot3(p(:, 1), p(:, 2), p(:, 3), 'r', 'LineWidth', 2)
			hold on
		end
if (false)
		%Нумерация граней:
		for i = 1 : fnum
			p = vert(vert_list_by_edge(face{i}, edge), :);
			s = num2str(i - 1);
			x = mean(p(:, 1));
			y = mean(p(:, 2));
			z = mean(p(:, 3));
			text(x, y, z, s, 'Color', 'k', 'FontSize', 16)
			hold on
		end
		%Нумерация ребер:
		for i = 1 : enum
			s = num2str(i - 1); p = vert(edge(i, :), :);
			x = mean(p(:, 1));
			y = mean(p(:, 2));
			z = mean(p(:, 3));
			text(x, y, z, s, 'Color', 'm', 'FontSize', 12)
			hold on
		end
end
		title(['time: ', num2str(t), '/', num2str(tnum)]);
		if (lim)
			axis([xmin, xmax, ymin, ymax, xmin, zmax]);
		else
			axis equal tight
		end
		mov = getframe();
		if (gif)
			[im, map] = rgb2ind(mov.cdata, 256);
			if (t > tmin)
				imwrite(im, map, 'output.gif', ...
				        'WriteMode', 'Append', ...
				        'DelayTime', 0.1);
			else
				imwrite(im, map, 'output.gif', ...
				        'DelayTime', 0.1);
			end
		end
		pause(0.1);
		hold off
	end

	fclose(finp);
	hold off

end

function list = vert_list_by_edge(list, edge)

	used = zeros(length(list), 1);
	head = edge(list, 1);
	tail = edge(list, 2);
	list = [head(1), tail(1)];
	used(1) = 1;
	find = true;
	while (find)
		find = false;
		for i = 2 : length(used)
			if (~used(i))
				if (head(i) == list(end))
					list = [list, tail(i)];
					used(i) = 1;
					find = true;
					continue
				end
				if (tail(i) == list(end))
					list = [list, head(i)];
					used(i) = 1;
					find = true;
					continue
				end
			end
		end
	end

end
