
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
        template<class>
        class gen;

        template<class>
        class cache_clone;
    };

    template<class>
    class PowerSeries;

    template<class T>
    PowerSeries<T> make_static( std::initializer_list<T> );

    template<class T>
    bool is_cached( const PowerSeries<T>& prs );

    template<class T>
    class PowerSeries
    {
        friend class gen::cache_clone<T>;

    protected:
        std::unique_ptr<gen::gen<T>> g = nullptr;

    protected:
        PowerSeries( std::unique_ptr<gen::gen<T>> gen ) : g(std::move(gen))
        {   }
        
    public:
        PowerSeries()
        {   }

    public:
        PowerSeries( PowerSeries&& prs )
        {
            *this = std::move(prs);
        }

        PowerSeries( const PowerSeries& prs )
        {
            *this = prs;
        }

        PowerSeries& operator=( PowerSeries&& prs )
        {
            g = std::move(prs.g);

            return *this;
        }

        PowerSeries& operator=( const PowerSeries& prs )
        {
            if(prs.g)
                g = prs.g->clone();
            else
                g = nullptr;

            return *this;
        }

    public:
        PowerSeries( std::initializer_list<T> poly )
        {
            *this = make_static( std::move(poly) );
        }

        PowerSeries( int c )
        {
            if( c != 0 )
                *this = make_static( std::initializer_list<T>{c} );
        }

    protected:
        T pop_from_gen()
        {
            auto p = g.get();

            auto head_tail = p->pop(std::move(*this));
            *this = std::move(head_tail.second);

            return std::move(head_tail.first);
        }

    public:
        T pop()
        {
            if(g)
                return pop_from_gen();
            else 
                return T(0);
        }

    public:
        bool is_zero() const
        {
            return nullptr == g;
        }

    protected:
        template <template <typename> class Gen>
        class generator : public Gen<T>
        {
            using Gen<T>::Gen;

        public:
            std::unique_ptr<gen::gen<T>> clone() const
            {
                return std::unique_ptr<gen::gen<T>>( new generator(*this) );
            }
        };

    public:
        template<template <typename> class Gen, typename... GenArgs>
        static PowerSeries<T> make_pwrsrs(GenArgs&&... args)
        {
            return PowerSeries<T>( std::unique_ptr<gen::gen<T>>( new generator<Gen>( std::forward<GenArgs>(args)... ) ) );
        }

    public:
        class iterator : public std::iterator< std::input_iterator_tag, T, const T*, T& >
        {
        protected:
            PowerSeries<T> p;
            T val;

        public:
            iterator(PowerSeries<T> prs) : p(std::move(prs))
            {
                val = p.pop();
            }

            iterator& operator++()
            {
                val = p.pop();
                return *this;
            }

            T operator*()
            {   return std::move(val);     }
        }; 
    };

    template<class T>
    bool is_zero(const T& c)
    {
        return c == 0;
    }

    template<class T>
    bool is_zero( const PowerSeries<T>& p )
    {
        return p.is_zero();
    }

    template<class T>
    typename PowerSeries<T>::iterator begin(PowerSeries<T> p)
    {
        typename PowerSeries<T>::iterator iter(std::move(p));
        return std::move(iter);
    }
 
    template<class T>
    std::ostream& operator<<(std::ostream& stream, PowerSeries<T> p)
    {
        auto it = begin(std::move(p));
        
        for(int i=0;i<10;i++)
        {
            stream << *it << ", ";
            ++it;
        }

        stream << "..." << std::endl;
        return stream;
    }
};
