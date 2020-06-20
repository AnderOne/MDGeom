#define BOOST_TEST_MODULE test_base

#include <boost/test/unit_test.hpp>
#include <geom/base.hpp>


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


BOOST_AUTO_TEST_SUITE(suite_of_vector_tests)

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

//...

BOOST_AUTO_TEST_SUITE(suite_of_basis_tests)

BOOST_AUTO_TEST_CASE(test_constructor, *boost::unit_test::tolerance(MATH_EPSILON)) {

	using namespace GEOM::BASE;

	BOOST_TEST_MESSAGE("Testing basis constructors");

	t_vector<double, 3> v0{0., 0., 0.};
	t_vector<double, 3> v1{1., 0., 0.};
	t_vector<double, 3> v2{0., 1., 0.};
	t_vector<double, 3> v3{0., 0., 1.};
	t_vector<double, 3> v4{4., 3., 2.};

	t_basis<double, 3> b5(v4, 4. * v2 + v3, 3. * v1, v2 + v4);
	BOOST_TEST_VEC(b5.center(), v4);
	for (int i = 0; i < 3; ++ i) {
	for (int j = 0; j < i; ++ j) {
		BOOST_TEST_VAL(b5[i] * b5[j], 0.);
	}
	BOOST_TEST_VAL(b5[i].len(), 1.);
	}

	t_basis<double, 3> b4(v4, 4. * v3, 3. * v1, 2. * v2);
	BOOST_TEST_VEC(b4.center(), v4);
	BOOST_TEST_VEC(b4[0], v3);
	BOOST_TEST_VEC(b4[1], v1);
	BOOST_TEST_VEC(b4[2], v2);

	t_basis<double, 3> b3(v4, v3, v1, v2);
	BOOST_TEST_VEC(b3.center(), v4);
	BOOST_TEST_VEC(b3[0], v3);
	BOOST_TEST_VEC(b3[1], v1);
	BOOST_TEST_VEC(b3[2], v2);

	t_basis<double, 3> b2(v4);
	BOOST_TEST_VEC(b2.center(), v4);
	BOOST_TEST_VEC(b2[0], v1);
	BOOST_TEST_VEC(b2[1], v2);
	BOOST_TEST_VEC(b2[2], v3);

	t_basis<double, 3> b1;
	BOOST_TEST_VEC(b1.center(), v0);
	BOOST_TEST_VEC(b1[0], v1);
	BOOST_TEST_VEC(b1[1], v2);
	BOOST_TEST_VEC(b1[2], v3);

}

//...

BOOST_AUTO_TEST_SUITE_END()
