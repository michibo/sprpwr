
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
    T make_cached(T);

    template<class T>
    PowerSeries<T> make_cached( PowerSeries<T> );

    namespace gen
    {
        template<class T>
        class cache_clone : public gen<T>
        {
        protected:
            class cache : public std::enable_shared_from_this<cache>
            {
            protected:
                std::vector<T> vals;
                size_t stop = -1;

                PowerSeries<T> p;
                
            public:
                cache( PowerSeries<T> prs ) : p(std::move(prs))
                { }

                cache( std::initializer_list<T> poly ) : vals(std::move(poly)), stop(vals.size()-1)
                { }

                std::pair<T, PowerSeries<T>> get_val( PowerSeries<T> prs, cache_clone<T>& clone )
                {
                    if(vals.size() > clone.index)
                        return get_cached_val( std::move(prs), clone );
                    else
                        return get_val_grow( std::move(prs), clone );
                }

            protected:
                std::pair<T, PowerSeries<T>> get_cached_val( PowerSeries<T> prs, cache_clone<T>& clone ) const
                {
                    assert(vals.size() > clone.index);

                    T val = vals[clone.index];

                    if(clone.index == stop)
                        return std::make_pair(std::move(val), p);
                    
                    clone.index++;

                    return std::make_pair(std::move(val), std::move(prs));
                }

                std::pair<T, PowerSeries<T>> get_val_grow( PowerSeries<T> prs, cache_clone<T>& clone )
                {
                    assert(-1 == stop);
                    assert(clone.index == vals.size());

                    std::shared_ptr<cache> self = this->shared_from_this();
                    size_t index = clone.index;
                    cache_clone<T>* pClone = &clone;

                    auto head_tail = p.g->pop(std::move(prs));
                    prs = std::move(head_tail.second);

                    T val = make_cached(std::move(head_tail.first));
                    vals.push_back(val);

                    if(prs.g.get() != pClone)
                    {
                        if( is_cached( prs ) || is_zero( prs ) )
                        {
                            p = prs;
                            stop = index;
                        }
                        else
                        {
                            p = std::move(prs);
                            prs = PowerSeries<T>::template make_pwrsrs<cache_clone>( self, index+1 );
                        }
                    }
                    else
                        clone.index++;

                    return std::make_pair(std::move(val), std::move(prs));
                }
            };
            
            std::shared_ptr<cache> cch;
            size_t index;

        public:
            cache_clone( PowerSeries<T> prs ) 
                : cache_clone( std::make_shared<cache>(std::move(prs)), 0 )
            { }

            cache_clone( std::initializer_list<T> poly )
                : cache_clone( std::make_shared<cache>(std::move(poly)), 0 )
            { }

            cache_clone( std::shared_ptr<cache> c, size_t i )
                : cch(std::move(c)), index(i)
            { }

        public:
            std::pair<T, PowerSeries<T>> pop( PowerSeries<T> self )
            {
                auto p = cch.get();
                return p->get_val(std::move(self), *this);
            }

        public:
            static bool is_pwrsrs_cached( const PowerSeries<T>& prs )
            {
                return nullptr != dynamic_cast<const cache_clone<T>*>(prs.g.get());
            }
        };
    };

    template<class T>
    bool is_cached( const PowerSeries<T>& prs )
    {
        return gen::cache_clone<T>::is_pwrsrs_cached( prs );
    }

    template<class T>
    T make_cached(T t)
    {
        return std::move(t);
    }

    template<class T>
    PowerSeries<T> make_cached( PowerSeries<T> prs )
    {
        if( is_cached( prs ) || is_zero(prs) )
            return std::move(prs);

        return PowerSeries<T>::template make_pwrsrs<gen::cache_clone>( std::move(prs) );
    }

    template<class T>
    PowerSeries<T> make_static( std::initializer_list<T> poly )
    {
        return PowerSeries<T>::template make_pwrsrs<gen::cache_clone>( std::move(poly) );
    }
};

