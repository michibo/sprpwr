
// This C++ header is part of the sprpwr library. 
// With this library multivariate formal power 
// series can be expanded. 
// 
// Author: Michael Borinsky
// License: MIT
// Copyright 2019

namespace sprpwr
{
    template<class T>
    T exp( const T& );

    template<class T>
    T log( const T& );

    template<class T>
    PowerSeries<T> exp( PowerSeries<T> );

    template<class T>
    PowerSeries<T> log( PowerSeries<T> );

    namespace gen
    {
        template<class T>
        class exper : public gen<T>
        {
        protected:
            PowerSeries<T> p;

        public:
            exper(PowerSeries<T> prs) : p(prs)
            { }

        public:
            std::pair<T, PowerSeries<T>> pop(PowerSeries<T> self)
            {
                auto cpy = self;

                T res = exp( p.pop() );

                return std::make_pair( std::move(res), inteo( diffo(std::move(p)) * std::move(cpy) ) );
            }
        };

        template<class T>
        class loger : public gen<T>
        {
        protected:
            PowerSeries<T> p;

        public:
            loger(PowerSeries<T> prs) : p(prs)
            { }

        public:
            std::pair<T, PowerSeries<T>> pop(PowerSeries<T> self)
            {
                auto cpy = self;

                PowerSeries<T> pCpy = p;
                T res = log( p.pop() );

                return std::make_pair( std::move(res), inteo( diffo(std::move(p)) / std::move(pCpy) ) );
            }
        };
    };

    template<class T>
    T exp(const T& t)
    {
        if( t == 0 )
            return T(1);
        else
        {
            assert(false);
            return T(-1);
        }
    }

    template<class T>
    T log(const T& t)
    {
        if( t == 1 )
            return T(0);
        else
        {
            assert(false);
            return T(-1);
        }
    }

    template<class T>
    PowerSeries<T> exp(PowerSeries<T> prs)
    {
        return make_cached(
            PowerSeries<T>::template make_pwrsrs<gen::exper>(std::move(prs))
            );
    }

    template<class T>
    PowerSeries<T> log(PowerSeries<T> prs)
    {
        return make_cached(
            PowerSeries<T>::template make_pwrsrs<gen::loger>(std::move(prs))
            );
    }
};

