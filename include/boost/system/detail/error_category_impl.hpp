#ifndef BOOST_SYSTEM_DETAIL_ERROR_CATEGORY_IMPL_HPP_INCLUDED
#define BOOST_SYSTEM_DETAIL_ERROR_CATEGORY_IMPL_HPP_INCLUDED

//  Copyright Beman Dawes 2006, 2007
//  Copyright Christoper Kohlhoff 2007
//  Copyright Peter Dimov 2017, 2018
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//  See library home page at http://www.boost.org/libs/system

#include <boost/system/detail/error_category.hpp>
#include <boost/system/detail/error_condition.hpp>
#include <boost/system/detail/error_code.hpp>
#include <boost/config.hpp>
#include <string>
#include <cstring>

namespace boost
{

namespace system
{

// error_category default implementation

inline error_condition error_category::default_error_condition( int ev ) const BOOST_NOEXCEPT
{
    return error_condition( ev, *this );
}

inline bool error_category::equivalent( int code, const error_condition & condition ) const BOOST_NOEXCEPT
{
    return default_error_condition( code ) == condition;
}

inline bool error_category::equivalent( const error_code & code, int condition ) const BOOST_NOEXCEPT
{
    return *this == code.category() && code.value() == condition;
}

inline char const * error_category::message( int ev, char * buffer, std::size_t len ) const BOOST_NOEXCEPT
{
    if( len == 0 )
    {
        return buffer;
    }

    if( len == 1 )
    {
        buffer[0] = 0;
        return buffer;
    }

#if !defined(BOOST_NO_EXCEPTIONS)
    try
#endif
    {
        std::string m = this->message( ev );

# if defined( BOOST_MSVC )
#  pragma warning( push )
#  pragma warning( disable: 4996 )
# elif defined(__clang__) && defined(__has_warning)
#  pragma clang diagnostic push
#  if __has_warning("-Wdeprecated-declarations")
#   pragma clang diagnostic ignored "-Wdeprecated-declarations"
#  endif
# endif

        std::strncpy( buffer, m.c_str(), len - 1 );
        buffer[ len-1 ] = 0;

# if defined( BOOST_MSVC )
#  pragma warning( pop )
# elif defined(__clang__) && defined(__has_warning)
#  pragma clang diagnostic pop
# endif

        return buffer;
    }
#if !defined(BOOST_NO_EXCEPTIONS)
    catch( ... )
    {
        return "Message text unavailable";
    }
#endif
}

} // namespace system

} // namespace boost

// interoperability with std::error_code, std::error_condition

#if defined(BOOST_SYSTEM_HAS_SYSTEM_ERROR)

#include <boost/system/detail/std_interoperability.hpp>

inline boost::system::error_category::operator std::error_category const & () const
{
    return boost::system::detail::to_std_category( *this );
}

#endif // #if defined(BOOST_SYSTEM_HAS_SYSTEM_ERROR)

#endif // #ifndef BOOST_SYSTEM_DETAIL_ERROR_CATEGORY_IMPL_HPP_INCLUDED
