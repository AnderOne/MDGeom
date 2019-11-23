function test(fname, xmin, xmax, ymin, ymax, zmin, zmax, tmin, tmax, gif)

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

		vnum = fscanf(finp, '%i', 1);
		vert = fscanf(finp, '%f', [3, vnum]).';
		enum = fscanf(finp, '%i', 1);
		edge = fscanf(finp, '%i', [2, enum]).';
		edge = edge + 1;
		fnum = fscanf(finp, '%i', 1);
		for i = 1 : fnum
			n = fscanf(finp, '%i', 1);
			e = fscanf(finp, '%i', n) + 1;
			v = edge(e(1), 2);
			for j = 2 : n
				for k = j : n
					a = edge(e(k), 1);
					b = edge(e(k), 2);
					if (a == v)
						c = e(j); e(j) = e(k); e(k) = c;
						v = b;
						break
					end
					if (b == v)
						c = e(j); e(j) = e(k); e(k) = c;
						v = a;
						break
					end
				end
			end
			face(i).e = e;
		end

		if (t < tmin) || (t > tmax); continue; end

		if (fnum == 0); clf(); end

		for i = 1 : fnum
			a = [edge(face(i).e(1), 1)];
			p = [];
			for e = face(i).e(:).'
				b = edge(e, :);
				if b(1) ~= a
					c = b(1);
					b(1) = b(2);
					b(2) = c;
				end
				p = [p, b];
				a = b(2);
			end
			p = vert([p(:); p(1)], :);
			fill3(p(:, 1), p(:, 2), p(:, 3), 'b', 'FaceAlpha', 0.85)
			hold on
			plot3(p(:, 1), p(:, 2), p(:, 3), 'LineWidth', 2, 'Color', 'r')
		end

		title(['time: ', num2str(t), '/', num2str(tnum)]);
		if (lim)
			axis([xmin, xmax, ymin, ymax, xmin, zmax]);
		end
		mov = getframe();
		if (gif)
			[im, map] = rgb2ind(mov.cdata, 256);
			if (t > tmin)
				imwrite(im, map, 'test.gif', 'DelayTime', 0.1, ...
				        'WriteMode', 'Append');
			else
				imwrite(im, map, 'test.gif', ...
				        'DelayTime', 0.1);
			end
		end
		pause(0.1);
		hold off
	end

	fclose(finp);

end
