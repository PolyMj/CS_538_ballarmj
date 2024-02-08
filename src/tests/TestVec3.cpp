#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <typeinfo>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include "doctest/doctest.h"
#include "Vector.hpp"
#include "Settings.hpp"
using namespace std;

#define EPS_F 1e-5
#define EPS_D 1e-12

inline bool close(float a, float b) {
    if(!(abs(a - b) <= EPS_F)) {
        cout << "ERROR: " << a << " vs. " << b << endl;
        return false;
    }
    return true;
}

inline bool really_close(double a, double b) {
    if(!(abs(a - b) <= EPS_D)) {
        //cout << std::setprecision (15);
        cout << "ERROR: " << a << " vs. " << b << endl;
        return false;
    }
    return true;
}

#ifdef ENABLE_TESTING

TEST_CASE("VecLength") {
    SUBCASE("float") { CHECK(typeid(float) == typeid(potato::VecLength<float>::type));}  
    SUBCASE("int") { CHECK(typeid(float) == typeid(potato::VecLength<int>::type));}  
    SUBCASE("unsigned char") { CHECK(typeid(float) == typeid(potato::VecLength<unsigned char>::type));}  
    SUBCASE("double") { CHECK(typeid(double) == typeid(potato::VecLength<double>::type));}  
}

TEST_CASE("Vec3: Constructors") {
    SUBCASE("Default") {
        potato::Vec3<float> f;
        potato::Vec3<int> i;

        CHECK(typeid(float) == typeid(f.x));
        CHECK(typeid(float) == typeid(f.y));
        CHECK(typeid(float) == typeid(f.z));

        CHECK(typeid(int) == typeid(i.x));
        CHECK(typeid(int) == typeid(i.y));
        CHECK(typeid(int) == typeid(i.z));  

        CHECK(close(f.x, 0.0));
        CHECK(close(f.y, 0.0));
        CHECK(close(f.z, 0.0));

        CHECK(i.x == 0);
        CHECK(i.y == 0);
        CHECK(i.z == 0);        
    }

    SUBCASE("Arguments") {
        potato::Vec3<float> f(1,2,3);
        potato::Vec3<int> i(4,5,6);

        CHECK(typeid(float) == typeid(f.x));
        CHECK(typeid(float) == typeid(f.y));
        CHECK(typeid(float) == typeid(f.z));

        CHECK(typeid(int) == typeid(i.x));
        CHECK(typeid(int) == typeid(i.y));
        CHECK(typeid(int) == typeid(i.z));  

        CHECK(close(f.x, 1.0));
        CHECK(close(f.y, 2.0));
        CHECK(close(f.z, 3.0));

        CHECK(i.x == 4);
        CHECK(i.y == 5);
        CHECK(i.z == 6);        
    }

    SUBCASE("Conversion") {
        potato::Vec3<float> f(1.2f,-3.4f,5.6f);
        potato::Vec3<int> i(4,5,6);

        potato::Vec3<float> i2f = i;
        potato::Vec3<int> f2i = f;

        CHECK(close(i2f.x, 4.0));
        CHECK(close(i2f.y, 5.0));
        CHECK(close(i2f.z, 6.0));

        CHECK(f2i.x == 1);
        CHECK(f2i.y == -3);
        CHECK(f2i.z == 5);        
    }
}

TEST_CASE("Vec3: Union Data") {
    potato::Vec3<float> f(1,2,3);

    CHECK(close(f.r, 1.0));
    CHECK(close(f.g, 2.0));
    CHECK(close(f.b, 3.0));

    CHECK(close(f.s, 1.0));
    CHECK(close(f.t, 2.0));
    CHECK(close(f.p, 3.0));

    f.x = 4;
    f.y = 5;
    f.z = 6;

    CHECK(close(f.r, 4.0));
    CHECK(close(f.g, 5.0));
    CHECK(close(f.b, 6.0));

    CHECK(close(f.s, 4.0));
    CHECK(close(f.t, 5.0));
    CHECK(close(f.p, 6.0));
}

TEST_CASE("Vec3: Addition") {
    potato::Vec3<float> a(1.2f,4.8f,0.0f);
    potato::Vec3<float> b(-2.7f,3.9f,2.3f);
    potato::Vec3<int> c(-3,4,1);
    potato::Vec3<int> d(9,-2,6);

    CHECK(typeid(potato::Vec3<float>) == typeid(a+b));
    CHECK(typeid(potato::Vec3<float>) == typeid(b+a));
    CHECK(typeid(potato::Vec3<int>) == typeid(c+d));
    CHECK(typeid(potato::Vec3<int>) == typeid(d+c));

    CHECK(typeid(potato::Vec3<float>) == typeid(b+c));
    CHECK(typeid(potato::Vec3<float>) == typeid(c+b));

    potato::Vec3<int> cd = c + d;
    potato::Vec3<int> dc = d + c;
    potato::Vec3<float> ac = a + c;
    potato::Vec3<float> ab = a + b;
    potato::Vec3<float> ca = c + a;

    CHECK(cd.x == 6);
    CHECK(cd.y == 2);
    CHECK(cd.z == 7);

    CHECK(dc.x == 6);
    CHECK(dc.y == 2);
    CHECK(dc.z == 7);

    CHECK(close(ac.x, -1.8f));
    CHECK(close(ac.y, 8.8f));
    CHECK(close(ac.z, 1.0f));

    CHECK(close(ab.x, -1.5f));
    CHECK(close(ab.y, 8.7f));
    CHECK(close(ab.z, 2.3f));

    CHECK(close(ca.x, -1.8f));
    CHECK(close(ca.y, 8.8f));
    CHECK(close(ca.z, 1.0f));
}

TEST_CASE("Vec3: Subtraction") {
    potato::Vec3<float> a(1.2f,4.8f,0.0f);
    potato::Vec3<float> b(-2.7f,3.9f,2.3f);
    potato::Vec3<int> c(-3,4,1);
    potato::Vec3<int> d(9,-2,6);

    CHECK(typeid(potato::Vec3<float>) == typeid(a-b));
    CHECK(typeid(potato::Vec3<float>) == typeid(b-a));
    CHECK(typeid(potato::Vec3<int>) == typeid(c-d));
    CHECK(typeid(potato::Vec3<int>) == typeid(d-c));

    CHECK(typeid(potato::Vec3<float>) == typeid(b-c));
    CHECK(typeid(potato::Vec3<float>) == typeid(c-b));

    potato::Vec3<int> cd = c - d;
    potato::Vec3<int> dc = d - c;
    potato::Vec3<float> ac = a - c;
    potato::Vec3<float> ab = a - b;
    potato::Vec3<float> ca = c - a;

    CHECK(cd.x == -12);
    CHECK(cd.y == 6);
    CHECK(cd.z == -5);

    CHECK(dc.x == 12);
    CHECK(dc.y == -6);
    CHECK(dc.z == 5);

    CHECK(close(ac.x, 4.2f));
    CHECK(close(ac.y, 0.8f));
    CHECK(close(ac.z, -1.0f));

    CHECK(close(ab.x, 3.9f));
    CHECK(close(ab.y, 0.9f));
    CHECK(close(ab.z, -2.3f));

    CHECK(close(ca.x, -4.2f));
    CHECK(close(ca.y, -0.8f));
    CHECK(close(ca.z, 1.0f));
}

TEST_CASE("Vec3: Multiplication") {
    potato::Vec3<float> a(1.2f,4.8f,0.3f);
    potato::Vec3<float> b(-2.7f,3.9f,2.3f);
    potato::Vec3<int> c(-3,4,1);
    potato::Vec3<int> d(9,-2,6);

    CHECK(typeid(potato::Vec3<float>) == typeid(a*b));
    CHECK(typeid(potato::Vec3<float>) == typeid(b*a));
    CHECK(typeid(potato::Vec3<int>) == typeid(c*d));
    CHECK(typeid(potato::Vec3<int>) == typeid(d*c));

    CHECK(typeid(potato::Vec3<float>) == typeid(b*c));
    CHECK(typeid(potato::Vec3<float>) == typeid(c*b));

    potato::Vec3<int> cd = c * d;
    potato::Vec3<int> dc = d * c;
    potato::Vec3<float> ac = a * c;
    potato::Vec3<float> ab = a * b;
    potato::Vec3<float> ca = c * a;

    CHECK(cd.x == -27);
    CHECK(cd.y == -8);
    CHECK(cd.z == 6);

    CHECK(dc.x == -27);
    CHECK(dc.y == -8);
    CHECK(dc.z == 6);

    CHECK(close(ac.x, -3.6f));
    CHECK(close(ac.y, 19.2f));
    CHECK(close(ac.z, 0.3f));

    CHECK(close(ab.x, -3.24f));
    CHECK(close(ab.y, 18.72f));
    CHECK(close(ab.z, 0.69f));

    CHECK(close(ca.x, -3.6f));
    CHECK(close(ca.y, 19.2f));
    CHECK(close(ca.z, 0.3f));
}

TEST_CASE("Vec3: Multiplication (Scalar)") {
    
    potato::Vec3<float> a(-2.7f,3.9f,2.3f);
    potato::Vec3<int> c(-3,4,1);
    float f = -0.8f;
    int i = 21;

    CHECK(typeid(potato::Vec3<float>) == typeid(a*f));    
    CHECK(typeid(potato::Vec3<int>) == typeid(c*i));
    
    CHECK(typeid(potato::Vec3<float>) == typeid(a*i));
    CHECK(typeid(potato::Vec3<float>) == typeid(c*f));

    potato::Vec3<int> ci = c * i;    
    potato::Vec3<float> af = a * f;

    potato::Vec3<float> ai = a * i;
    potato::Vec3<float> cf = c * f;

    CHECK(ci.x == -63);
    CHECK(ci.y == 84);
    CHECK(ci.z == 21);

    CHECK(close(af.x, 2.16f));
    CHECK(close(af.y, -3.12f));
    CHECK(close(af.z, -1.84f));

    CHECK(close(ai.x, -56.7f));
    CHECK(close(ai.y, 81.9f));
    CHECK(close(ai.z, 48.3f));

    CHECK(close(cf.x, 2.4f));
    CHECK(close(cf.y, -3.2f));
    CHECK(close(cf.z, -0.8f));
}

TEST_CASE("Vec3: Division (Scalar)") {
    
    potato::Vec3<float> a(-2.7f,3.9f,2.3f);
    potato::Vec3<int> c(3,-4,10);
    float f = -0.8f;
    int i = 4;

    CHECK(typeid(potato::Vec3<float>) == typeid(a/f));    
    CHECK(typeid(potato::Vec3<int>) == typeid(c/i));
    
    CHECK(typeid(potato::Vec3<float>) == typeid(a/i));
    CHECK(typeid(potato::Vec3<float>) == typeid(c/f));

    potato::Vec3<int> ci = c / i;    
    potato::Vec3<float> af = a / f;

    potato::Vec3<float> ai = a / i;
    potato::Vec3<float> cf = c / f;

    CHECK(ci.x == 0);
    CHECK(ci.y == -1);
    CHECK(ci.z == 2);

    CHECK(close(af.x, 3.375f));
    CHECK(close(af.y, -4.875f));
    CHECK(close(af.z, -2.875f));

    CHECK(close(ai.x, -0.675f));
    CHECK(close(ai.y, 0.975f));
    CHECK(close(ai.z, 0.575f));

    CHECK(close(cf.x, -3.75f));
    CHECK(close(cf.y, 5.0f));
    CHECK(close(cf.z, -12.5f));
}

TEST_CASE("Vec3: Array Access") {
    potato::Vec3<float> f(1,2,3);
    potato::Vec3<int> i(1,2,3);

    CHECK(typeid(float) == typeid(f[0]));
    CHECK(typeid(float) == typeid(f[1]));
    CHECK(typeid(float) == typeid(f[2]));

    CHECK(typeid(int) == typeid(i[0]));
    CHECK(typeid(int) == typeid(i[1]));
    CHECK(typeid(int) == typeid(i[2]));  

    CHECK(close(f[0], 1.0f));
    CHECK(close(f[1], 2.0f));
    CHECK(close(f[2], 3.0f));

    CHECK(i[0] == 1);
    CHECK(i[1] == 2);
    CHECK(i[2] == 3);

    f[0] = 4;
    f[1] = 5;
    f[2] = 6;

    i[0] = 4;
    i[1] = 5;
    i[2] = 6;

    CHECK(close(f.x, 4.0f));
    CHECK(close(f.y, 5.0f));
    CHECK(close(f.z, 6.0f));

    CHECK(close(f[0], 4.0f));
    CHECK(close(f[1], 5.0f));
    CHECK(close(f[2], 6.0f));

    CHECK(i.x == 4);
    CHECK(i.y == 5);
    CHECK(i.z == 6);

    CHECK(i[0] == 4);
    CHECK(i[1] == 5);
    CHECK(i[2] == 6);

    const potato::Vec3<float> cf(1.0f,2.0f,3.0f);

    CHECK(close(cf.x, 1.0f));
    CHECK(close(cf.y, 2.0f));
    CHECK(close(cf.z, 3.0f));

    CHECK(close(cf[0], 1.0f));
    CHECK(close(cf[1], 2.0f));
    CHECK(close(cf[2], 3.0f));

    DOCTEST_CHECK_THROWS_AS(cf[-1], std::out_of_range);
    DOCTEST_CHECK_THROWS_AS(cf[3], std::out_of_range);

    DOCTEST_CHECK_THROWS_AS(f[-1]=0.0f, std::out_of_range);
    DOCTEST_CHECK_THROWS_AS(f[3]=0.0f, std::out_of_range);
}

TEST_CASE("Vec3: Printout") {
    potato::Vec3<float> f(-2.7f,3.9f,2.3f);
    potato::Vec3<int> i(-3,4,1);

    ostringstream outS;
	outS << f << endl;
	CHECK(outS.str() == "(-2.7,3.9,2.3)\n");
    f.x = 8.9f;
    outS << f << endl;
    CHECK(outS.str() == "(-2.7,3.9,2.3)\n(8.9,3.9,2.3)\n");
    outS << i << endl;
    CHECK(outS.str() == "(-2.7,3.9,2.3)\n(8.9,3.9,2.3)\n(-3,4,1)\n");
}

TEST_CASE("Vec3: Dot Product") {
    potato::Vec3<float> a(1.2f,4.8f,0.3f);
    potato::Vec3<float> b(-2.7f,3.9f,2.3f);
    potato::Vec3<int> c(-3,4,1);
    potato::Vec3<int> d(9,-2,6);

    CHECK(typeid(float) == typeid(a.dot(b)));
    CHECK(typeid(float) == typeid(b.dot(a)));
    CHECK(typeid(int) == typeid(c.dot(d)));
    CHECK(typeid(int) == typeid(d.dot(c)));

    CHECK(typeid(float) == typeid(b.dot(c)));
    CHECK(typeid(float) == typeid(c.dot(b)));

    int cd = c.dot(d);
    int dc = d.dot(c);
    float ac = a.dot(c);
    float ab = a.dot(b);
    float ca = c.dot(a);

    CHECK(cd == -29);    
    CHECK(dc == -29);
    
    CHECK(close(ac, 15.9f));    
    CHECK(close(ab, 16.17f));
    CHECK(close(ca, 15.9f));
}

TEST_CASE("Vec3: Cross Product") {
    potato::Vec3<float> a(1.2f,4.8f,0.3f);
    potato::Vec3<float> b(-2.7f,3.9f,2.3f);
    potato::Vec3<int> c(-3,4,1);
    potato::Vec3<int> d(9,-2,6);

    CHECK(typeid(potato::Vec3<float>) == typeid(a.cross(b)));
    CHECK(typeid(potato::Vec3<float>) == typeid(b.cross(a)));
    CHECK(typeid(potato::Vec3<int>) == typeid(c.cross(d)));
    CHECK(typeid(potato::Vec3<int>) == typeid(d.cross(c)));

    CHECK(typeid(potato::Vec3<float>) == typeid(b.cross(c)));
    CHECK(typeid(potato::Vec3<float>) == typeid(c.cross(b)));

    potato::Vec3<int> cd = c.cross(d);
    potato::Vec3<int> dc = d.cross(c);
    potato::Vec3<float> ac = a.cross(c);
    potato::Vec3<float> ab = a.cross(b);
    potato::Vec3<float> ca = c.cross(a);

    CHECK(cd.x == 26);
    CHECK(cd.y == 27);
    CHECK(cd.z == -30);

    CHECK(dc.x == -26);
    CHECK(dc.y == -27);
    CHECK(dc.z == 30);

    CHECK(close(ac.x, 3.6f));
    CHECK(close(ac.y, -2.1f));
    CHECK(close(ac.z, 19.2f));

    CHECK(close(ab.x, 9.87f));
    CHECK(close(ab.y, -3.57f));
    CHECK(close(ab.z, 17.64f));

    CHECK(close(ca.x, -3.6f));
    CHECK(close(ca.y, 2.1f));
    CHECK(close(ca.z, -19.2f));
}

TEST_CASE("Vec3: Length") {
    potato::Vec3<float> f(1.2f,4.8f,0.0f);    
    potato::Vec3<int> i(-3,4,1);
    potato::Vec3<unsigned char> u(9,2,6);
    potato::Vec3<double> d(-2.7,3.9,2.3);

    CHECK(typeid(float) == typeid(f.length()));
    CHECK(typeid(float) == typeid(i.length()));
    CHECK(typeid(float) == typeid(u.length()));
    CHECK(typeid(double) == typeid(d.length()));

    CHECK(close(f.length(), 4.94773f));
    CHECK(close(i.length(), 5.09902f));
    CHECK(close(u.length(), 11.0f));
    CHECK(really_close(d.length(), 5.27162214123888));
}

TEST_CASE("Vec3: Normalize") {
    potato::Vec3<float> f(1.2f,4.8f,0.0f);    
    potato::Vec3<int> i(-3,4,1);
    potato::Vec3<unsigned char> u(9,2,6);
    potato::Vec3<double> d(-2.7,3.9,2.3);

    auto nf = f.normalize();
    auto ni = i.normalize();
    auto nu = u.normalize();
    auto nd = d.normalize();

    CHECK(typeid(potato::Vec3<float>) == typeid(nf));
    CHECK(typeid(potato::Vec3<float>) == typeid(ni));
    CHECK(typeid(potato::Vec3<float>) == typeid(nu));
    CHECK(typeid(potato::Vec3<double>) == typeid(nd));

    CHECK(close(nf.length(), 1.0f));
    CHECK(close(ni.length(), 1.0f));
    CHECK(close(nu.length(), 1.0f));
    CHECK(really_close(nd.length(), 1.0));
}

TEST_CASE("Vec3: Alternate Names") {
    potato::Vec3f f(1.2f,4.8f,0.0f);    
    potato::Vec3i i(-3,4,1);
    potato::Vec3u u(9,2,6);
    potato::Vec3d d(-2.7,3.9,2.3);

    CHECK(typeid(potato::Vec3<float>) == typeid(f));
    CHECK(typeid(potato::Vec3<int>) == typeid(i));
    CHECK(typeid(potato::Vec3<unsigned char>) == typeid(u));
    CHECK(typeid(potato::Vec3<double>) == typeid(d));
}

#endif

