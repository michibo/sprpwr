
// This C++ header is part of the sprpwr library. 
// With this library multivariate formal power 
// series can be expanded. 
// 
// Author: Michael Borinsky
// License: MIT
// Copyright 2019

namespace sprpwr
{
    namespace gen
    {
        template<class T>
        class xmuler : public gen<T>
        {
            PowerSeries<T> p;
            
        public:
            xmuler(PowerSeries<T> prs) : p(std::move(prs))
            { }

        public:
            std::pair<T, PowerSeries<T>> pop(PowerSeries<T> self)
            {
                return std::make_pair(T(0), p);
            }
        };

        template<class T>
        class tailer : public gen<T>
        {
            PowerSeries<T> p;
            
        public:
            tailer(PowerSeries<T> prs) : p(std::move(prs))
            { }

        public:
            std::pair<T, PowerSeries<T>> pop(PowerSeries<T> self)
            {
                p.pop();
                T res = p.pop();

                return std::make_pair(std::move(res), p);
            }
        };
    };

    template<class T>
    PowerSeries<T> xmul( PowerSeries<T> prs )
    {
        return PowerSeries<T>::template make_pwrsrs<gen::xmuler>(std::move(prs));
    }

    template<class T>
    PowerSeries<T> tail( PowerSeries<T> prs )
    {
        return PowerSeries<T>::template make_pwrsrs<gen::tailer>(std::move(prs));
    }

    template<class T>
    class diffo_func
    {
        int n = 0;

    public:
        T operator()(T e)
        {
            return T(++n)*std::move(e);
        }
    };
    
    template<class T>
    class inteo_func
    {
        int n = 0;

    public:
        T operator()(T e)
        {
            return std::move(e)/T(++n);
        }
    };

    template<class T>
    PowerSeries<T> diffo( PowerSeries<T> prs )
    {
        return linear_map( diffo_func<T>(), std::move(prs) );
    }

    template<class T>
    PowerSeries<T> inteo( PowerSeries<T> prs )
    {
        return linear_map( inteo_func<T>(), std::move(prs) );
    }

    template<class T>
    PowerSeries<T> diff( PowerSeries<T> prs )
    {
        return diffo(tail(prs));
    }

    template<class T>
    PowerSeries<T> inte( PowerSeries<T> prs, T c )
    {
        return xmul(inteo(prs)) + PowerSeries<T>{c};
    }
};

