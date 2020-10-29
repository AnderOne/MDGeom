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
		mdim = fscanf(finp, '%i', 2); if (mdim(1) > 3); error('Can''t draw multi-dimensional data!'); end

		%Считываем вершины:
		vnum = fscanf(finp, '%i', 1); vert = fscanf(finp, '%f', [mdim(1), vnum]).';
		if (mdim(1) < 3)
			vert(:, 3) = 0;
		end
		if (mdim(1) < 2)
			vert(:, 2) = 0;
		end

		%Считываем ребра:
		enum = fscanf(finp, '%i', 1); edge = fscanf(finp, '%i', [2, enum]).' + 1;

		%Считываем грани:
		if (mdim(2) > 1)
			fnum = fscanf(finp, '%i', 1);
			for i = 1 : fnum
				face{i} = fscanf(finp, '%i', fscanf(finp, '%i', 1)).' + 1;
			end
		else
			fnum = 0;
		end
		%Считываем тела:
		if (mdim(2) > 2)
			bnum = fscanf(finp, '%i', 1);
			for i = 1 : bnum
				body{i} = fscanf(finp, '%i', fscanf(finp, '%i', 1)).' + 1;
			end
		else
			bnum = 0;
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
			text(x, y, z, s, 'Color', 'b', 'FontSize', 16)
			hold on
		end
		%Нумерация ребер:
		for i = 1 : enum
			s = num2str(i - 1); p = vert(edge(i, :), :);
			x = mean(p(:, 1));
			y = mean(p(:, 2));
			z = mean(p(:, 3));
			text(x, y, z, s, 'Color', 'r', 'FontSize', 14)
			hold on
		end
		%Нумерация вершин:
		for i = 1 : vnum
			s = num2str(i - 1);
			x = vert(i, 1);
			y = vert(i, 2);
			z = vert(i, 3);
			text(x, y, z, s, 'Color', 'g', 'FontSize', 14)
			hold on
		end
end
		title(['step: ', num2str(t), '/', num2str(tnum)]);
		if (mdim(1) < 3); view([0, 90]); end
		if (lim)
			axis([xmin, xmax, ymin, ymax, xmin, zmax]);
		else
			axis equal tight
		end
		grid on
		mov = getframe(gcf);
		if (gif)
			[im, map] = rgb2ind(mov.cdata, 256);
			if (t > tmin)
				imwrite(im, map, 'test.gif', ...
				        'WriteMode', 'Append', ...
				        'DelayTime', 0.1);
			else
				imwrite(im, map, 'test.gif', ...
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
