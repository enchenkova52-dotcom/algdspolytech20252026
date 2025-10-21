#include <gtest/gtest.h>

#include "pch.h"
extern "C" {
#include "poly.h"
}
#include <string>

static Poly make_poly(const char* s) {
	Poly p;
	poly_init(&p);
	if (poly_from_string(s, &p) != 0) {
	}
	return p;
}

static std::string to_str(const Poly& p) {
	char* s = poly_to_string(&const_cast<Poly&>(p));
	std::string r = s ? s : std::string("0");
	free(s);
	return r;
}

TEST(ParseTest, SimpleConstant_no1) {
	Poly p = make_poly("5");
	EXPECT_EQ(to_str(p), "5");
	poly_free(&p);
}

TEST(ParseTest, NegativeCoefficients_no2) {
	Poly p = make_poly("-3*x^2+2*x-1");
	EXPECT_EQ(to_str(p), "-1+2*x-3*x^2");
	poly_free(&p);
}

TEST(ParseTest, OmitOneCoefficient_no3) {
	Poly p = make_poly("x^3-x+1");
	EXPECT_EQ(to_str(p), "1-x+x^3");
	poly_free(&p);
}

TEST(AddTest, UnorderedInput_no4) {
	Poly a = make_poly("2*x^2+1");
	Poly b = make_poly("-1+3*x");
	Poly sum;
	poly_add(&a, &b, &sum);
	EXPECT_EQ(to_str(sum), "3*x+2*x^2");
	poly_free(&a);
	poly_free(&b);
	poly_free(&sum);
}

TEST(MulTest, MultiplySimple_no5) {
	Poly a = make_poly("x+1");
	Poly b = make_poly("x-1");
	Poly prod;
	poly_mul(&a, &b, &prod);
	EXPECT_EQ(to_str(prod), "-1+x^2");
	poly_free(&a);
	poly_free(&b);
	poly_free(&prod);
}

TEST(DerivativeTest, DerivativeSimple_no6) {
	Poly a = make_poly("3*x^3+2*x-5");
	Poly d;
	poly_derivative(&a, &d);
	EXPECT_EQ(to_str(d), "2+9*x^2");
	poly_free(&a);
	poly_free(&d);
}

TEST(EdgeTest, ZeroPolynomial_no7) {
	Poly a = make_poly("1-x");
	Poly b = make_poly("x-1");
	Poly sum;
	poly_add(&a, &b, &sum);
	EXPECT_EQ(to_str(sum), "0");
	poly_free(&a);
	poly_free(&b);
	poly_free(&sum);
}

TEST(ParseTest, MediumDegree_fixed_no8) {
	Poly a = make_poly("2*x^10");
	EXPECT_EQ(to_str(a), "2*x^10");
	poly_free(&a);
}

TEST(MulTest, MultiplyToZero_no9) {
	Poly a = make_poly("0");
	Poly b = make_poly("5*x^2+1");
	Poly prod;
	poly_mul(&a, &b, &prod);
	EXPECT_EQ(to_str(prod), "0");
	poly_free(&a);
	poly_free(&b);
	poly_free(&prod);
}

TEST(ComplexTest, CombineLikeTerms_no10) {
	Poly a = make_poly("2*x^2+3*x^2-5");
	EXPECT_EQ(to_str(a), "-5+5*x^2");
	poly_free(&a);
}

TEST(ParseTest, NormalCoefficient_fixed_no11) {
	Poly p = make_poly("100*x^2");
	EXPECT_EQ(to_str(p), "100*x^2");
	poly_free(&p);
}

TEST(ParseTest, WithoutAsterisk_no12) {
	Poly p = make_poly("5x^2+3x+1");
	EXPECT_EQ(to_str(p), "1+3*x+5*x^2");
	poly_free(&p);
}

TEST(DerivativeTest, LinearFunction_no13) {
	Poly a = make_poly("5*x+3");
	Poly d;
	poly_derivative(&a, &d);
	EXPECT_EQ(to_str(d), "5");
	poly_free(&a);
	poly_free(&d);
}

TEST(EdgeTest, SingleX_no14) {
	Poly p = make_poly("x");
	EXPECT_EQ(to_str(p), "x");
	poly_free(&p);
}

TEST(EdgeTest, NegativeX_no15) {
	Poly p = make_poly("-x");
	EXPECT_EQ(to_str(p), "-x");
	poly_free(&p);
}