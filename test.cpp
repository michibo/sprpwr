
// This C++ header is part of the sprpwr library. 
// With this library multivariate formal power 
// series can be expanded. 
// 
// Author: Michael Borinsky
// License: MIT
// Copyright 2019

#include <iostream>
#include <iterator>
#include <algorithm>
#include <chrono>
#include <functional>
#include <cmath>

#include <gmpxx.h>

#include "powerseries.hpp"

using namespace std;

template<class T1, class T2>
void check( T1 entry1, T2 entry2, int n )
{
    if(Equal(entry1, entry2, n))
        cout << "CHECK PASSED" << endl;
    else
        cout << "CHECK FAILED" << endl;
}

template<class Func>
double time( Func&& func )
{
    auto start = chrono::high_resolution_clock::now();
    func();
    auto end = chrono::high_resolution_clock::now();

    auto diff = end - start;

    return chrono::duration <double> (diff).count();
}

template<class Func>
void benchmark(Func func, int multiplier)
{
    int n = 100;
    int total = 0;
    double sum_logd = 0.0;
    double sum_logt = 0.0;
    double sum_logdlogd = 0.0;
    double sum_logdlogt = 0.0;
    double sum_logtlogt = 0.0;

    auto start = chrono::high_resolution_clock::now();

    for(int i=0;i<n;i++)
    {
        int d = i*multiplier;
        
        double t = time([=]() { func(d); } );
        total += d;
        cout << "." << flush;

        if (i!= 0)
        {
            sum_logd += log(d);
            sum_logdlogd += log(d)*log(d);
            sum_logt += log(t);
            sum_logdlogt += log(t)*log(d);
            sum_logtlogt += log(t)*log(t);
        }
    }
    cout << endl;

    double gamma = (n*sum_logdlogt - sum_logd*sum_logt)/(n*sum_logdlogd - sum_logd*sum_logd);
    double beta = sum_logt/n - gamma*sum_logd/n;

    double e_error_sqr = 1.0/(n*(n-2))*(n*sum_logtlogt - sum_logt*sum_logt- gamma*(n*sum_logdlogd - sum_logd*sum_logd));
    
    double gamma_error_sqr = n*e_error_sqr/(n*sum_logdlogd- sum_logd*sum_logd);

    auto end = chrono::high_resolution_clock::now();

    auto diff = end - start;

    cout << "Total time: " << chrono::duration <double> (diff).count() << "s" << endl;
    cout << "Total compute: " << total << endl;
    cout << "Max compute: " << (n-1)*multiplier << endl;
    cout << "exp " << gamma << " +/- " << sqrt(gamma_error_sqr) << endl;
    cout << "C (" << exp(beta) << " +/- " << exp(beta)*sqrt(gamma_error_sqr) << ") s" << endl;
}

template<class T>
void check_1d()
{
    using namespace pwrsrs;

    auto ONE = PowerSeries<T>{1};
    auto X = PowerSeries<T>{0,1};

    cout << T(1)/T(10) << endl;

    cout << ONE << endl;
    cout << X << endl;
    cout << (1+X)*(1+X) << endl;

    cout << 1/(1-X-X*X) << endl;
    cout << 1/(1-X)*(1-X) << endl;

    cout << diffo(1/(1-X)) << endl;
    cout << inteo(1/(1-X)) << endl;

    cout << log(1/(1-X)) << endl;

    cout << exp(X) << endl;

    cout << X/(exp(X)-1) << endl;

    check(1+2*X+X*X, (1+X)*(1+X), 50 );
    check(1-2*X+X*X, (1-X)*(1-X), 50 );
    check(1-X*X, (1-X)*(1+X), 50);
    check(1/(1-X)*(1-X), ONE, 50);

    cout << "Polynom:" << endl;
    benchmark([&](int n) { Eval(1+X, n); }, 1000);

    cout << "Polynom squared:" << endl;
    benchmark([&](int n) { Eval((1+X)*(1+X), n); }, 1000);

    cout << "Geometric series:" << endl;
    benchmark([&](int n) { Eval(1/(1-X), n); }, 10);

    cout << "Geometric series diffo:" << endl;
    benchmark([&](int n) { Eval(diffo(1/(1-X)), n); }, 10);

    cout << "Geometric series squared:" << endl;
    auto geo = 1/(1-X);
    benchmark([&](int n) { Eval(geo*geo, n); }, 5);

    cout << "Geometric series squared2:" << endl;
    benchmark([&](int n) { Eval(geo/(1-X), n); }, 50);

    cout << "Fibonacci:" << endl;
    benchmark([&](int n) { Eval(1/(1-X-X*X), n); }, 50);
}

template<class T>
void check_2d()
{ 
    using namespace pwrsrs;

    auto Z0 = PowerSeries<T>{0};
    auto O0 = PowerSeries<T>{1};
    auto X0 = PowerSeries<T>{0,1};
    auto ONE = PowerSeries<PowerSeries<T>>(1);
    auto X = PowerSeries<PowerSeries<T>>{X0};
    auto Y = PowerSeries<PowerSeries<T>>{Z0, O0};

    cout << X*X << endl;

    cout << 1/(1-Y-X*Y) << endl;

    cout << exp(X*log(1+Y)) << endl;
    cout << exp(X*(exp(Y)-1)) << endl;

    check(1/(1-X-X*Y)*(1-X*(1+Y)), ONE, 50);

    cout << "Polynom squared:" << endl;
    benchmark([&](int n) { Eval((1+X)*(1+X), n); }, 10);
    cout << "Polynom product:" << endl;
    benchmark([&](int n) { Eval((1-Y-X*Y)*(1-Y*(1+Y)), n); }, 2);
    cout << "Binomial:" << endl;
    benchmark([&](int n) { Eval(1/(1-Y-X*Y), n); }, 2);
}

int main()
{
    cout << "Check 1d" << endl;
    check_1d<mpq_class>();
    cout << "Check 2d" << endl;
    check_2d<mpq_class>();
}

