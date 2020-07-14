#include <boost/test/unit_test.hpp>
#include <geom/mesh.hpp>

BOOST_AUTO_TEST_SUITE(suite_of_mesh_tests)

template <typename T> static std::vector<std::vector<int>> get_link(const std::vector<T> &item) {
	std::vector<std::vector<int>> link;
	for (int i = 0; i < item.size(); ++ i) for (int k: item[i]) {
		if (link.size() <= k) {
			link.resize(k + 1);
		}
		link[k].push_back(i);
	}
	return link;
}

BOOST_AUTO_TEST_CASE(test_constructor) {

	using namespace GEOM::MESH;

	BOOST_TEST_MESSAGE("Testing mesh constructors");
	{
	std::vector<t_mesh<double, 1, 1>::t_vert> vert{{0}, {1}, {2}};
	std::vector<t_edge> edge{{0, 1}, {1, 2}};

	//Check for cell indices
	t_mesh<double, 1, 1> mesh(vert, edge);
	BOOST_TEST(mesh.cell<1>() == edge);
	BOOST_TEST(mesh.edge() == edge);

	//Check for link indices
	BOOST_TEST(mesh.link<0>() == get_link(mesh.cell<1>()));
	}
	{
	std::vector<t_mesh<double, 3, 2>::t_vert> vert{
	{-1, -1, -1}, {-1, -1, +1}, {-1, +1, -1}, {-1, +1, +1},
	{+1, -1, -1}, {+1, -1, +1}, {+1, +1, -1}, {+1, +1, +1}
	};
	std::vector<t_edge> edge{
	{0, 1}, {0, 2}, {0, 4}, {1, 3}, {1, 5}, {2, 3},
	{2, 6}, {3, 7}, {4, 5}, {4, 6}, {5, 7}, {6, 7},
	{0, 3}, {0, 5}, {0, 6}, {1, 7}, {2, 7}, {4, 7}
	};
	std::vector<t_face> face{
	{6, 11, 16}, {8, 10, 17}, {9, 11, 17},
	{3, 7, 15}, {4, 10, 15}, {5, 7, 16},
	{1, 6, 14}, {2, 8, 13}, {2, 9, 14},
	{0, 3, 12}, {0, 4, 13}, {1, 5, 12}
	};

	//Check for cell indices
	t_mesh<double, 3, 2> mesh(vert, edge, face);
	BOOST_TEST(mesh.cell<1>() == edge);
	BOOST_TEST(mesh.cell<2>() == face);
	BOOST_TEST(mesh.edge() == edge);
	BOOST_TEST(mesh.face() == face);

	//Check for link indices
	BOOST_TEST(mesh.link<0>() == get_link(mesh.cell<1>()));
	BOOST_TEST(mesh.link<1>() == get_link(mesh.cell<2>()));
	}
	{
	std::vector<t_mesh<double, 3, 3>::t_vert> vert{
	{-1, -1, -1}, {-1, -1, +1}, {-1, +1, -1}, {-1, +1, +1},
	{+1, -1, -1}, {+1, -1, +1}, {+1, +1, -1}, {+1, +1, +1}
	};
	std::vector<t_edge> edge{
	{0, 1}, {0, 2}, {0, 4}, {1, 3}, {1, 5}, {2, 3},
	{2, 6}, {3, 7}, {4, 5}, {4, 6}, {5, 7}, {6, 7},
	{0, 3}, {0, 5}, {0, 6}, {1, 7}, {2, 7}, {4, 7}, {0, 7}
	};
	std::vector<t_face> face{
	{6, 11, 16}, { 8, 10, 17}, { 9, 11, 17},
	{3,  7, 15}, { 4, 10, 15}, { 5,  7, 16},
	{1,  6, 14}, { 2,  8, 13}, { 2,  9, 14},
	{0,  3, 12}, { 0,  4, 13}, { 1,  5, 12},
	//...
	{0, 15, 18}, { 1, 16, 18}, { 2, 17, 18},
	{7, 12, 18}, {11, 14, 18}, {10, 13, 18}
	};
	std::vector<t_body> body{
	{0, 6, 13, 16}, {2,  8, 14, 16},
	{1, 7, 14, 17}, {4, 10, 12, 17},
	{3, 9, 12, 15}, {5, 11, 13, 15}
	};

	//Check for cell indices
	t_mesh<double, 3, 3> mesh(vert, edge, face, body);
	BOOST_TEST(mesh.cell<1>() == edge);
	BOOST_TEST(mesh.cell<2>() == face);
	BOOST_TEST(mesh.cell<3>() == body);
	BOOST_TEST(mesh.edge() == edge);
	BOOST_TEST(mesh.face() == face);
	BOOST_TEST(mesh.body() == body);

	//Check for link indices
	BOOST_TEST(mesh.link<0>() == get_link(mesh.cell<1>()));
	BOOST_TEST(mesh.link<1>() == get_link(mesh.cell<2>()));
	BOOST_TEST(mesh.link<2>() == get_link(mesh.cell<3>()));
	}

}

BOOST_AUTO_TEST_SUITE_END()
