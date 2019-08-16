
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
        class div : public gen<T>
        {
            PowerSeries<T> c;
            PowerSeries<T> p;

        public:
            div(PowerSeries<T> nom, PowerSeries<T> prs)
                : c(std::move(nom)), p(std::move(prs))
            { }

        public:
            std::pair<T, PowerSeries<T>> pop(PowerSeries<T> self)
            {
                auto cpy = self;
                
                T p0;
                T c0;

                do
                {
                    p0 = p.pop();
                    c0 = c.pop();
                }
                while( is_zero(p0) && is_zero(c0) );

                T rec = T(1)/std::move(p0);
                T res = std::move(c0) * rec;

                return std::make_pair( std::move(res), 
                     cmul(std::move(rec), std::move(c) - std::move(p)*std::move(cpy) )
                    );
            }
        };
    };

    template<class T>
    PowerSeries<T> operator/( PowerSeries<T> entry1, PowerSeries<T> entry2 )
    {
        return make_cached(
            PowerSeries<T>::template make_pwrsrs<gen::div>(std::move(entry1), std::move(entry2))
            );
    }

    template<class T>
    PowerSeries<T> operator/( int entry1, PowerSeries<T> entry2 )
    {
        return PowerSeries<T>(1)/std::move(entry2);
    }

    template<class T>
    PowerSeries<T> operator/( PowerSeries<T> entry1, int entry2 )
    {
        return std::move(entry1)/PowerSeries<T>(entry2);
    }
};
