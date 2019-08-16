
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
        template<class T>
        class gen
        {
        public:
            virtual ~gen()
            {   }

            virtual std::unique_ptr<gen> clone() const = 0;
            virtual std::pair<T, PowerSeries<T>> pop(PowerSeries<T> self) = 0;
        };
    };
};

