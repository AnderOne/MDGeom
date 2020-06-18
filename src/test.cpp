#define BOOST_TEST_MODULE test_base

#include <boost/test/unit_test.hpp>
#include <geom/base.hpp>

BOOST_AUTO_TEST_SUITE(suite_of_vector_tests)

#define BOOST_TEST_VEC(_v1_expr, _v2_expr) {\
const auto _v1 = _v1_expr, _v2 = _v2_expr;\
for (auto it1 = _v1.begin(), it2 = _v2.begin(); it1 != _v1.end(); ++ it1, ++ it2) {\
BOOST_TEST(*it1 == *it2);\
}\
}

#define BOOST_TEST_VAL(_v1_expr, _v2_expr) {\
const auto _v1 = _v1_expr, _v2 = _v2_expr;\
BOOST_TEST(_v1 == _v2);\
}

BOOST_AUTO_TEST_CASE(test_constructor, *boost::unit_test::tolerance(MATH_EPSILON)) {

	using namespace GEOM::BASE;

	BOOST_TEST_MESSAGE("Testing vector constructors");

	t_vector<double, 3> v1 = {1., 2, 3};
	BOOST_TEST_VAL(v1[0], 1.);
	BOOST_TEST_VAL(v1[1], 2.);
	BOOST_TEST_VAL(v1[2], 3.);

	t_vector<double, 3> v2 = 4.;
	BOOST_TEST_VAL(v2[0], 4.);
	BOOST_TEST_VAL(v2[1], 4.);
	BOOST_TEST_VAL(v2[2], 4.);

	t_vector<double, 3> v3 = v1;
	BOOST_TEST_VAL(
	v3[0], v1[0]);
	BOOST_TEST_VAL(
	v3[1], v1[1]);
	BOOST_TEST_VAL(
	v3[2], v1[2]);

}

BOOST_AUTO_TEST_CASE(test_compare, *boost::unit_test::tolerance(MATH_EPSILON)) {

	using namespace GEOM::BASE;

	BOOST_TEST_MESSAGE("Testing vector comparators");

	t_vector<double, 3> v1 = {1., 2, 3};
	t_vector<double, 3> v2 = {1., 2, 4};
	t_vector<double, 3> v3 = v1;

	BOOST_REQUIRE(!(v1 == v2));
	BOOST_REQUIRE(v1 != v2);
	BOOST_REQUIRE(!(v1 != v3));
	BOOST_REQUIRE(v1 == v3);

}

BOOST_AUTO_TEST_CASE(test_math, *boost::unit_test::tolerance(MATH_EPSILON)) {

	using namespace GEOM::BASE;

	BOOST_TEST_MESSAGE("Testing vector operations");

	t_vector<double, 3> v1 = {1., 2, 3};
	t_vector<double, 3> v2 = {2., 4, 6};
	t_vector<double, 3> v3 = {2., 3, 4};
	t_vector<double, 3> v4 = {3., 6, 9};

	BOOST_TEST_VEC(2. * v1, v2);
	BOOST_TEST_VEC(v1 * 2., v2);
	BOOST_TEST_VEC(v1, v2 / 2.);
	BOOST_TEST_VEC(v1 + 1., v3);
	BOOST_TEST_VEC(v1, v3 - 1.);
	BOOST_TEST_VEC(v1 + v2, v4);
	BOOST_TEST_VEC(v1, v4 - v2);
	BOOST_TEST_VAL(
	v1 * v3, v1[0] * v3[0] + v1[1] * v3[1] + v1[2] * v3[2]
	);
	BOOST_TEST_VAL(
	v1 * v1, v1.len2()
	);

}

//...

BOOST_AUTO_TEST_SUITE_END()
