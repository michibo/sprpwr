
// This C++ header is part of the sprpwr library. 
// With this library multivariate formal power 
// series can be expanded. 
// 
// Author: Michael Borinsky
// License: MIT
// Copyright 2019

namespace pwrsrs
{
    template<class T>
    T mulsum( const std::vector<std::pair<T,T>>& );

    template<class T>
    PowerSeries<T> mulsum( std::vector<std::pair<PowerSeries<T>,PowerSeries<T>>> );

    template<class T>
    PowerSeries<T> cmulsum( std::vector<std::pair<T, PowerSeries<T>>> );

    namespace gen
    {
        template<class T>
        class cmul_summer : public gen<T>
        {
        protected:
            std::vector<std::pair<T, PowerSeries<T>>> pvs;

        public:
            cmul_summer(std::vector<std::pair<T, PowerSeries<T>>> pvs_entry) 
                : pvs(std::move(pvs_entry))
            { }

        protected:
            template<class Iterator>
            Iterator pop_slice(Iterator iterOut)
            {
                auto iterBv = begin(pvs);
                auto iterEv = end(pvs);

                iterOut = transform( iterBv, iterEv, iterOut, 
                    [](std::pair<T, PowerSeries<T>>& p) 
                        { return std::make_pair(p.first, p.second.pop()); } );

                iterBv = remove_if( iterBv, iterEv, 
                    [](const std::pair<T,PowerSeries<T>>& p) 
                    { return is_zero(p.second); } );
                pvs.erase(iterBv, iterEv);

                return std::move(iterOut);
            }

        public:
            std::pair<T, PowerSeries<T>> pop(PowerSeries<T> self)
            {
                std::vector<std::pair<T,T>> tmp( pvs.size() );
                auto iter = begin(tmp);

                pop_slice(std::move(iter));

                if( pvs.empty() )
                    return std::make_pair(mulsum(std::move(tmp)), PowerSeries<T>());
                else
                    return std::make_pair(mulsum(std::move(tmp)), std::move(self));
            }
        };

        template<class T>
        class mul_summer : public cmul_summer<T>
        {
        protected:
            std::vector<std::pair<PowerSeries<T>, PowerSeries<T>>> pms;

            bool leap = true;

        public:
            mul_summer(std::vector<std::pair<PowerSeries<T>, PowerSeries<T>>> pms_entry) 
                : cmul_summer<T>(std::vector<std::pair<T, PowerSeries<T>>>()), pms(std::move(pms_entry))
            { }

        protected:
            template<class Iterator>
            Iterator pop_pms_slice(Iterator iterOut)
            {
                auto iterBm = begin(pms);
                auto iterEm = end(pms);

                for(;iterBm!=iterEm; ++iterBm)
                {
                    T c0 = make_cached(std::get<0>(*iterBm).pop());
                    T c1 = make_cached(std::get<1>(*iterBm).pop());

                    if(!is_zero(c0))
                        this->pvs.push_back(std::make_pair(c0, std::get<1>(*iterBm)));

                    if(!is_zero(c1))
                        this->pvs.push_back(std::make_pair(c1, std::get<0>(*iterBm)));

                    *iterOut++ = std::make_pair(std::move(c0), std::move(c1));
                }

                iterBm = remove_if( begin(pms), iterEm, 
                    [](const std::pair<PowerSeries<T>, PowerSeries<T>>& p) 
                    { return is_zero(std::get<0>(p)) || is_zero(std::get<1>(p)); } );
                pms.erase(iterBm, iterEm);

                return iterOut;
            }

        public:
            std::pair<T, PowerSeries<T>> pop( PowerSeries<T> self )
            {
                std::vector<std::pair<T,T>> tmp( leap ? this->pvs.size() + pms.size() : this->pvs.size() );
                auto iter = begin(tmp);

                iter = this->pop_slice(std::move(iter));

                if( leap )
                {
                    iter = pop_pms_slice(std::move(iter));

                    leap = false;
                }
                else
                    leap = true;

                assert( iter == end(tmp) );

                if( pms.empty() )
                    return std::make_pair( mulsum(std::move(tmp)), cmulsum(std::move(this->pvs)) );
                else
                    return std::make_pair( mulsum(std::move(tmp)), std::move(self) );
            }
        };
    };

    template<class T>
    T mulsum( const std::vector<std::pair<T,T>>& ps )
    {
        T res(0);
        for( const std::pair<T,T>& val : ps )
            res += val.first * val.second;

        return std::move(res);
    }

    template<class T>
    PowerSeries<T> mulsum( std::vector<std::pair<PowerSeries<T>, PowerSeries<T>>> ps )
    {
        for( auto& p : ps )
        {
            std::get<0>(p) = make_cached( std::get<0>(p) );
            std::get<1>(p) = make_cached( std::get<1>(p) );
        }

        return PowerSeries<T>::template make_pwrsrs<gen::mul_summer>(std::move(ps));
    }

    template<class T>
    PowerSeries<T> cmulsum( std::vector<std::pair<T, PowerSeries<T>>> ps )
    {
        for( auto& p : ps )
        {
            std::get<0>(p) = make_cached( std::get<0>(p) );
        }

        return PowerSeries<T>::template make_pwrsrs<gen::cmul_summer>(std::move(ps));
    }

    template<class T>
    PowerSeries<T> cmul( T c, PowerSeries<T> p )
    {
        return cmulsum(std::vector<std::pair<T, PowerSeries<T>>>({std::make_pair(std::move(c), std::move(p))}));
    }

    template<class T>
    PowerSeries<T> operator-( PowerSeries<T> p )
    {
        return cmul( T(-1), std::move(p) );
    }

    template<class T>
    PowerSeries<T> operator+( PowerSeries<T> entry1, PowerSeries<T> entry2 )
    {
        return cmulsum( std::vector<std::pair<T, PowerSeries<T>>>(
            { std::make_pair(T(1), std::move(entry1)), 
                std::make_pair(T(1), std::move(entry2)) } ));
    }

    template<class T>
    PowerSeries<T> operator-( PowerSeries<T> entry1, PowerSeries<T> entry2 )
    {
        return cmulsum( std::vector<std::pair<T, PowerSeries<T>>>(
            { std::make_pair(T(1), std::move(entry1)), 
                std::make_pair(T(-1), std::move(entry2)) } ));
    }

    template<class T>
    PowerSeries<T> operator+( PowerSeries<T> entry1, int entry2 )
    {
        return std::move(entry1) + PowerSeries<T>(entry2);
    }

    template<class T>
    PowerSeries<T> operator+( int entry1, PowerSeries<T> entry2 )
    {
        return std::move(entry2) + entry1;
    }
    
    template<class T>
    PowerSeries<T> operator-( PowerSeries<T> entry1, int entry2 )
    {
        return std::move(entry1) + (-entry2);
    }

    template<class T>
    PowerSeries<T> operator-( int entry1, PowerSeries<T> entry2 )
    {
        return PowerSeries<T>(entry1) - std::move(entry2);
    }

    template<class T>
    PowerSeries<T> operator*( PowerSeries<T> entry1, PowerSeries<T> entry2 )
    {
        return mulsum(
            std::vector<std::pair<PowerSeries<T>, PowerSeries<T>>>(
                { std::make_pair( std::move(entry1), std::move(entry2)) }
                    )
            );
    }

    template<class T>
    PowerSeries<T> operator*( int entry1, PowerSeries<T> entry2 )
    {
        return cmul( T(entry1), std::move(entry2) );
    }

    template<class T>
    PowerSeries<T> operator*( PowerSeries<T> entry1, int entry2 )
    {
        return entry2 * std::move(entry1);
    }

};
