//
//  Copyright (c) 2012 Artyom Beilis (Tonkikh)
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_NOWIDE_CSTDIO_H_INCLUDED
#define BOOST_NOWIDE_CSTDIO_H_INCLUDED

#include <boost/config.hpp>
#ifdef BOOST_WINDOWS
#include <boost/nowide/stackstring.hpp>
#else
#include <cstdio>
#endif

#ifdef BOOST_MSVC
#pragma warning(push)
#pragma warning(disable : 4996)
#endif

namespace boost {
namespace nowide {
#if !defined(BOOST_WINDOWS) && !defined(BOOST_NOWIDE_DOXYGEN)
    using std::fopen;
    using std::freopen;
    using std::remove;
    using std::rename;
#else

    ///
    /// \brief Same as freopen but file_name and mode are UTF-8 strings
    ///
    inline FILE *freopen(char const *file_name, char const *mode, FILE *stream)
    {
        wshort_stackstring const wmode(mode);
        return _wfreopen(file_name ? wstackstring(file_name).c_str() : NULL, wmode.c_str(), stream);
    }
    ///
    /// \brief Same as fopen but file_name and mode are UTF-8 strings
    ///
    inline FILE *fopen(char const *file_name, char const *mode)
    {
        wstackstring const wname(file_name);
        wshort_stackstring const wmode(mode);
        return _wfopen(wname.c_str(), wmode.c_str());
    }
    ///
    /// \brief Same as rename but old_name and new_name are UTF-8 strings
    ///
    inline int rename(char const *old_name, char const *new_name)
    {
        wstackstring const wold(old_name), wnew(new_name);
        return _wrename(wold.c_str(), wnew.c_str());
    }
    ///
    /// \brief Same as rename but name is UTF-8 string
    ///
    inline int remove(char const *name)
    {
        wstackstring const wname(name);
        return _wremove(wname.c_str());
    }
#endif
} // namespace nowide
} // namespace boost

#ifdef BOOST_MSVC
#pragma warning(pop)
#endif

#endif
///
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
