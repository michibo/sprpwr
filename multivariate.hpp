
// This C++ header is part of the sprpwr library. 
// With this library multivariate formal power 
// series can be expanded. 
// 
// Author: Michael Borinsky
// License: MIT
// Copyright 2019

namespace pwrsrs
{
    namespace gen
    {
        template<class Func>
        struct linear_mapper
        {
            template<class T>
            class type : public gen<T>
            {
                PowerSeries<T> p;
                Func func;

            public:
                type( Func f, PowerSeries<T> prs ) : p(std::move(prs)), func(std::move(f))
                { }

                std::pair<T, PowerSeries<T>> pop( PowerSeries<T> self )
                {
                    T val = func(p.pop());
                    return std::make_pair(std::move(val), is_zero(p) ? PowerSeries<T>() : std::move(self));
                }
            };
        };
    };

    template<class T, class Func>
    PowerSeries<T> linear_map( Func&& f, PowerSeries<T> prs )
    {
        return PowerSeries<T>::template make_pwrsrs< 
            gen::linear_mapper<Func>::template type > ( std::forward<Func>(f), std::move(prs) );
    }

    template<int N, class T, class Func>
    class deep_linear_mapper
    {
        Func func;

    public:
        deep_linear_mapper( Func f ) : func(std::move(f))
        { }

        PowerSeries<T> operator() ( PowerSeries<T> entry )
        {
            return linear_map( deep_linear_mapper< N-1, T, Func >( func ), std::move(entry) );
        }
    };

    template<class T, class Func>
    class deep_linear_mapper<0, T, Func>
    {
        Func func;

    public:
        deep_linear_mapper( Func f ) : func(std::move(f))
        { }

        T operator() ( T entry )
        {
            return func( std::move(entry) );
        }
    };

    template<int N, class T, class Func>
    PowerSeries<T> deep_linear_map( Func&& f, PowerSeries<T> prs )
    {
        return deep_linear_mapper<N, T, Func>( std::forward<Func>(f) )( std::move(prs) );
    }
};
