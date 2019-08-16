sprpwr: A C++11 header-only multivariate power series library
===========================================================

The sprpwr power series library can be used to expand multivariate power series.

It was written to study some new features of C++ and to implement lazy evaluation in C++. It makes heavy use of templates, virtual functions and the move construct. It also uses an interesting part of the C++ language: some objects `destroy themselves' in a member function. This is used to implement an actor like behaviour of the generators which can replace themselves with other generators. 

This library is more a study on the C++ language, but it can also be used as a tool to do calculations. 

How to use it:
--------------

Here are some examples of how to use it. There are a couple of features that I didn't describe here.
  
    using namespace sprpwr;

    auto X = PowerSeries<int>{0,1};

creates the pretty trivial power series X = 0 + 1 * x + 0 * x^2 + ...

We can print it using 

    cout << X << endl;

    > 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, ...

and use standard operators

    cout << 2*X << endl;

    > 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, ...

    cout << 1+X << endl;

    > 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, ...

    cout << (1+X)*(1+X) << endl;

    > 1, 2, 1, 0, 0, 0, 0, 0, 0, 0, ...

    cout << 1/(1+X) << endl;

    > 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, ...

    cout << 1/(1+X+X*X) << endl;

    > 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, ...

Some mathematical functions are overloaded to be able to applied on power series

    cout << exp(X) << endl;

    > 1, 1, 1/2, 1/6, 1/24, 1/120, 1/720, 1/5040, 1/40320, 1/362880, ...

    cout << log(1/(1-X)) << endl;

    > 1, 1/2, 1/3, 1/4, 1/5, 1/6, 1/7, 1/8, 1/9, 1/10, ...

    cout << X/(exp(X)-1) << endl;

    > 1, -1/2, 1/12, 0, -1/720, 0, 1/30240, 0, -1/1209600, 0, ...

Because the ground field of the power series is implemented as a template, it is easy to use power series of power series (i.e. multivariate power series).

First we have to define a couple of basic 1-dimensional power series, to be able to define two basic 2-dimensional power series:

    auto Z0 = PowerSeries<T>{0};
    auto O0 = PowerSeries<T>{1};
    auto X0 = PowerSeries<T>{0,1};

    auto X = PowerSeries<PowerSeries<T>>{X0};
    auto Y = PowerSeries<PowerSeries<T>>{Z0, O0};

The objects X and Y are now power series of power series.

For instance, 

    cout << 1/(1-Y-X*Y) << endl;

results in a print out of Pascal's triangle:
    
    > 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, ...
    > , 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, ...
    > , 1, 2, 1, 0, 0, 0, 0, 0, 0, 0, ...
    > , 1, 3, 3, 1, 0, 0, 0, 0, 0, 0, ...
    > , 1, 4, 6, 4, 1, 0, 0, 0, 0, 0, ...
    > , 1, 5, 10, 10, 5, 1, 0, 0, 0, 0, ...
    > , 1, 6, 15, 20, 15, 6, 1, 0, 0, 0, ...
    > , 1, 7, 21, 35, 35, 21, 7, 1, 0, 0, ...
    > , 1, 8, 28, 56, 70, 56, 28, 8, 1, 0, ...
    > , 1, 9, 36, 84, 126, 126, 84, 36, 9, 1, ...

