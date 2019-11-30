#pragma once

#include "base.hpp"
#include <valarray>
#include <vector>
#include <array>

namespace GEOM {

using namespace BASE;

namespace MESH {

template <typename T, unsigned N> using t_vert = t_vector<T, N>;

template <unsigned N>
//using t_item = std::array<int, N + 1>;
struct t_item {
	template <typename ... TT> t_item(TT ... args): ind{args ...} {}

	std::ostream &print(std::ostream &out) const {
		out << ind.size(); for (int i: ind) out << "\t" << i;
		return out;
	}

	const auto begin() const { return std::begin(ind); }

	const auto end() const { return std::end(ind); }

	size_t size() const { return ind.size(); }

	const auto &operator[](int i) const {
		return ind[i];
	}
	auto &operator[](int i) {
		return ind[i];
	}
private:
	std::valarray<int> ind;
};

template <> struct t_item<1> {
	template <typename ... TT> t_item(TT ... args): ind{args ...} {}

	std::ostream &print(std::ostream &out) const {
		return (out << ind[0] << "\t" << ind[1]);
	}

	const auto begin() const { return std::begin(ind); }

	const auto end() const { return std::end(ind); }

	size_t size() const { return ind.size(); }

	const auto &operator[](int i) const {
		return ind[i];
	}
	auto &operator[](int i) {
		return ind[i];
	}
private:
	std::array<int, 2> ind;
};

typedef t_item<3> t_cell;
typedef t_item<2> t_face;
typedef t_item<1> t_edge;

template <unsigned N>
struct t_grid;

template <unsigned N, unsigned M>
struct t_hand {

	static_assert((N > 0) && (M > 0) && (M < N));

	template <typename ... TT>
	static t_grid<N> gen(const std::vector<t_item<M>> &item, TT && ... args) {
		t_grid<N> grid = std::move(t_hand<N, M + 1>::
		          gen(std::forward<TT>(args) ...));
		t_hand<N, M>::get(grid) = item;
		return grid;
	}
	template <typename ... TT>
	static t_grid<N> gen(std::vector<t_item<M>> &&item, TT && ... args) {
		t_grid<N> grid = std::move(t_hand<N, M + 1>::
		          gen(std::forward<TT>(args) ...));
		t_hand<N, M>::get(grid) =
		        std::move(item);
		return grid;
	}

	static const std::vector<t_item<M>> &get(const t_grid<N> &grid) {
		return t_hand<N-1, M>::get(grid.GRID);
	}
	static std::vector<t_item<M>> &get(t_grid<N> &grid) {
		return t_hand<N-1, M>::get(grid.GRID);
	}
};

template <unsigned N>
struct t_hand<N, N> {

	static t_grid<N> gen(const std::vector<t_item<N>> &item) {
		t_grid<N> grid; grid.ITEM = item; return grid;
	}
	static t_grid<N> gen(std::vector<t_item<N>> &&item) {
		t_grid<N> grid; grid.ITEM = std::move(item);
		return grid;
	}

	static const std::vector<t_item<N>> &get(const t_grid<N> &grid) {
		return grid.ITEM;
	}
	static std::vector<t_item<N>> &get(t_grid<N> &grid) {
		return grid.ITEM;
	}
};

template <unsigned N>
struct t_grid {

	std::ostream &print(std::ostream &out) const {
		GRID.print(out);
		out << ITEM.size() << "\n";
		for (const auto &i : ITEM) {
			i.print(out);
			out << "\n";
		}
		out << "\n";
		return out;
	}

	template <unsigned M>
	const std::vector<t_item<M>> &get() const {
		return t_hand<N, M>::get(*this);
	}
	template <unsigned M>
	std::vector<t_item<M>> &get() {
		return t_hand<N, M>::get(*this);
	}

	std::vector<t_item<N>> ITEM;
	t_grid<N-1> GRID;
};

template <> struct t_grid<1> {

	std::ostream &print(std::ostream &out) const {
		out << ITEM.size() << "\n";
		for (const auto &i : ITEM) {
			i.print(out);
			out << "\n";
		}
		out << "\n";
		return out;
	}

	std::vector<t_item<1>> ITEM;
};

template <typename T, unsigned N>
struct t_mesh {

	typedef MESH::t_vert<T, N>
	t_vert;

	template <typename ... TT> t_mesh(const std::vector<t_vert> &vert, TT && ... args):
	                           VERT(vert),
	                           GRID(std::move(t_hand<N-1, 1>::gen(
	                           std::forward<TT>(args) ...
	                           ))) {}

	template <typename ... TT> t_mesh(std::vector<t_vert> &&vert, TT && ... args):
	                           VERT(std::move(vert)),
	                           GRID(std::move(t_hand<N-1, 1>::gen(
	                           std::forward<TT>(args) ...
	                           ))) {}

	t_mesh() {}

	//Data transform:
	void rot(const t_vector<T, N> &center, int xaxis, int yaxis, T angle) {
		for (auto &v : VERT) v = v.rot(center, xaxis, yaxis, angle);
	}
	void rot(const t_basis<T, N> &basis, int xaxis, int yaxis, T angle) {
		for (auto &v : VERT) v = v.rot(basis, xaxis, yaxis, angle);
	}
	void rot(int xaxis, int yaxis, T angle) {
		for (auto &v : VERT) v = v.rot(xaxis, yaxis, angle);
	}
	void mov(const t_vector<T, N> &dir) {
		for (auto &v : VERT) v = v.mov(dir);
	}

	//Data access:
	template <unsigned M> const std::vector<t_item<M>> &item() const {
		return GRID.template get<M>();
	}
	const std::vector<t_cell> &cell() const { return item<3>(); }
	const std::vector<t_face> &face() const { return item<2>(); }
	const std::vector<t_edge> &edge() const { return item<1>(); }
	const std::vector<t_vert> &vert() const { return VERT; }

	template <unsigned M> const t_item<M> &item(int i) const {
		return GRID.template get<M>()[i];
	}
	const t_cell &cell(int i) const { return item<3>(i); }
	const t_face &face(int i) const { return item<2>(i); }
	const t_edge &edge(int i) const { return item<1>(i); }
	const t_vert &vert(int i) const { return VERT[i]; }

	std::ostream &print(std::ostream &out) const {
		out << VERT.size() << "\n";
		for (const auto &v : VERT) {
			out << v << "\n";
		}
		out << "\n";
		GRID.print(out);
		return out;
	}

	//...
	template <typename ... F, unsigned M, typename = typename std::enable_if<
	         ((sizeof ... (F) == 0) && (N == 4) && (M == 3))
	         >::type>
	t_mesh<T, M> cross(const t_basis<T, N, M> &slice) const {

		//Check for vertices positions:
		t_vector<T, N> norm = slice.normal(), top = slice.center();
		std::vector<T> proj(vert().size());
		std::vector<typename t_mesh<T, 3>::t_vert> VERT;
		std::vector<t_edge> EDGE;
		std::vector<t_face> FACE;
		for (int vi = 0; vi < vert().size(); ++ vi) {
			proj[vi] = (vert(vi) - top) * norm;
		}

		constexpr int null = -1;
		std::vector<int> VMAP(vert().size(), null);
		std::vector<int> EMAP(edge().size(), null);
		std::vector<int> NODE(edge().size(), null);
		std::vector<int> USED(face().size(), 0);

		//Check for cells:
		for (int ci = 0; ci < cell().size(); ++ ci) {

			const auto &_cell = cell(ci);
			std::vector<int> PATH;

			//Check for faces:
			for (int fi : _cell) {

				const auto &_face = face(fi);
				int line[2];
				int fp = 0;
				int fc = 0;

				//Check for edges:
				for (int ei : _face) {

					const auto &_edge = edge(ei);
					int a = _edge[0];
					int b = _edge[1];
					const auto &va = vert(a);
					const auto &vb = vert(b);
					int ec = 0;

					//Check for vertices:
					if (proj[a] * proj[b] < 0) {
						if (NODE[ei] == null) {
							const auto &ab = vb - va;
							const auto &t = - proj[a] / ab.dot(norm);
							const auto &c = slice.put(va + t * ab);
							//Add new point:
							NODE[ei] = VERT.size();
							VERT.push_back(c);
						}
						line[fp ++] = NODE[ei];
					}
					if (proj[a] == 0) {
						if (VMAP[a] == null) {
							VMAP[a] = VERT.size();
							VERT.push_back(slice.put(va));
						}
						++ ec;
					}
					if (proj[b] == 0) {
						if (VMAP[b] == null) {
							VMAP[b] = VERT.size();
							VERT.push_back(slice.put(vb));
						}
						++ ec;
					}

					//Add new edge:
					if (ec == 2) {
						if (EMAP[ei] == null) {
							EMAP[ei] = EDGE.size();
							EDGE.push_back({VMAP[a], VMAP[b]});
						}
						++ fc;
					}
					//...
				}

				if ((fc > 1) && !USED[fi]) {
					std::valarray<int> val(_face.size());
					for (int i = 0; i < _face.size(); ++ i) {
						val[i] = EMAP[_face[i]];
					}
					FACE.push_back(val);
					USED[fi] = 1;
					continue;
				}
				if (fp > 0) {
					assert(fp == 2);
					PATH.push_back(EDGE.size());
					EDGE.push_back({line[0], line[1]});
				}
			}
			//...
			if (PATH.size() != 0) {
				std::valarray<int> val(PATH.size());
				for (int i = 0; i < PATH.size(); ++ i) {
					val[i] = PATH[i];
				}
				FACE.push_back(val);
			}
		}

		return t_mesh<T, M> (
		std::move(VERT),
		std::move(EDGE),
		std::move(FACE)
		);
	}

protected:
	template <unsigned M> void set(const std::vector<t_item<M>> &it) {
		GRID.template get<M>() = it;
	}
	template <unsigned M> void set(std::vector<t_item<M>> &&it) {
		GRID.template get<M>() = std::move(it);
	}
	void set(const std::vector<t_vert> &vt) {
		VERT = vt;
	}
	void set(std::vector<t_vert> &&vt) {
		VERT = std::move(vt);
	}

private:
	std::vector<t_vert> VERT;
	t_grid<N-1> GRID;
};

//...

template <typename T, unsigned N>
std::ostream &operator<<(std::ostream &out, const t_mesh<T, N> &mesh) {
	return mesh.print(out);
}

//...

}//MESH

//...

typedef MESH::t_mesh<double, 4>
t_mesh_4d;
typedef MESH::t_mesh<double, 3>
t_mesh_3d;

//...

}//GEOM
