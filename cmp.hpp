
// This C++ header is part of the sprpwr library. 
// With this library multivariate formal power 
// series can be expanded. 
// 
// Author: Michael Borinsky
// License: MIT
// Copyright 2019

namespace sprpwr
{
    template<class T1, class T2>
    bool Equal(T1 entry1, T2 entry2, int n)
    {
        if(n == 0)
            return true;
        else
            return entry1 == entry2;
    }

    template<class T1, class T2>
    bool Equal(PowerSeries<T1> entry1, T2 entry2, int n)
    {
        return Equal(std::move(entry1), constant(std::move(entry2)), n);
    }
    
    template<class T1, class T2>
    bool Equal(T1 entry1, PowerSeries<T2> entry2, int n)
    {
        return Equal(std::move(entry2), std::move(entry1), n);
    }
    
    template<class T1, class T2>
    bool Equal(PowerSeries<T1> entry1, PowerSeries<T2> entry2, int n)
    {
        auto it1 = begin(std::move(entry1));
        auto it2 = begin(std::move(entry2));
        for ( int i=0; i<n; ++it1,++it2,++i )
        {
            if(!Equal(*it1, *it2, n))
                return false;
        }

        return true;
    }

    template<class T>
    void Eval(T t, int n)
    {
    }

    template<class T>
    void Eval(PowerSeries<T> p, int n)
    {
        auto it = begin(std::move(p));
        for(int i=0; i<n; ++it,++i)
        {
            Eval(std::move(*it), n);
        }
    }
};
