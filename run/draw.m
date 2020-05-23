function draw(fname, xmin, xmax, ymin, ymax, zmin, zmax, tmin, tmax, gif)

	finp = fopen(fname, 'rt'); tnum = fscanf(finp, '%i', 1);

	lim = (exist('xmin') && exist('xmax') && ...
	       exist('ymin') && exist('ymax') && ...
	       exist('zmin') && exist('zmax'));
	
	if (~exist('tmax'))
		tmax = tnum;
	end
	if (~exist('tmin'))
		tmin = 1;
	end
	if (~exist('gif'))
		gif = false;
	end
	
	for t = 1 : tnum

		vnum = fscanf(finp, '%i', 1); vert = fscanf(finp, '%f', [3, vnum]).';
		enum = fscanf(finp, '%i', 1); edge = fscanf(finp, '%i', [2, enum]).';
		edge = edge + 1;
		fnum = fscanf(finp, '%i', 1); face = fscanf(finp, '%i', [3, fnum]).';
		face = face + 1;

		if (t < tmin) || (t > tmax); continue; end

		if (fnum == 0); clf(); end

		for i = 1 : fnum
			p = unique(edge(face(i, :), :));
			p = vert([p(:); p(1)], :);
			fill3(p(:, 1), p(:, 2), p(:, 3), 'b', 'FaceAlpha', 0.85)
			hold on
			plot3(p(:, 1), p(:, 2), p(:, 3), 'r', 'LineWidth', 2)
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

end
