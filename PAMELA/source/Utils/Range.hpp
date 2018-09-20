#include <Utils/Types.hpp>

#pragma once

namespace PAMELA
{
    class range {
    public:
        template < typename T1, typename T2 >
        range( T1 begin, T2 end )
            : m_iter( static_cast< global_idx_t >( begin ) ),
              m_last( static_cast< global_idx_t >( end ) )
        {
        }
        template < typename T >
        explicit range( T end ) : m_last( static_cast< global_idx_t >( end ) )
        {
        }
        const range& begin() const
        {
            return *this;
        }
        const range& end() const
        {
            return *this;
        }
        bool operator!=( const range& /*unused*/ ) const
        {
            return m_iter < m_last;
        }
        void operator++()
        {
            ++m_iter;
        }
        global_idx_t operator*() const
        {
            return m_iter;
        }

    protected:
        global_idx_t m_iter{ 0 };
        global_idx_t m_last{ 0 };
    };

} // namespace RINGMesh
