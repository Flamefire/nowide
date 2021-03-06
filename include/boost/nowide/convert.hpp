//
//  Copyright (c) 2012 Artyom Beilis (Tonkikh)
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_NOWIDE_CONVERT_H_INCLUDED
#define BOOST_NOWIDE_CONVERT_H_INCLUDED

#include <iterator>
#include <string>
#include <boost/locale/utf.hpp>
#include <boost/nowide/replacement.hpp>

namespace boost {
namespace nowide {
    ///
    /// \brief Template function that converts a buffer of UTF sequences in range [source_begin,source_end)
    /// to the output \a buffer of size \a buffer_size.
    ///
    /// In case of success a NULL terminated string is returned (buffer), otherwise 0 is returned.
    ///
    /// If there is not enough room in the buffer 0 is returned, and the content of the buffer is undefined.
    /// Any illegal sequences are replaced with the replacement character, see #BOOST_NOWIDE_REPLACEMENT_CHARACTER
    ///
    template<typename CharOut, typename CharIn>
    CharOut *basic_convert(CharOut *buffer, size_t buffer_size, CharIn const *source_begin, CharIn const *source_end)
    {
        CharOut *rv = buffer;
        if(buffer_size == 0)
            return 0;
        buffer_size--;
        while(source_begin != source_end)
        {
            using namespace boost::locale::utf;
            code_point c = utf_traits<CharIn>::decode(source_begin, source_end);
            if(c == illegal || c == incomplete)
            {
                c = BOOST_NOWIDE_REPLACEMENT_CHARACTER;
            }
            size_t width = utf_traits<CharOut>::width(c);
            if(buffer_size < width)
            {
                rv = 0;
                break;
            }
            buffer = utf_traits<CharOut>::encode(c, buffer);
            buffer_size -= width;
        }
        *buffer++ = 0;
        return rv;
    }

    ///
    /// \brief Template function that converts a buffer of UTF sequences in range [source_begin,source_end) and returns a string containing
    /// converted value
    ///
    /// Any illegal sequences are replaced with the replacement character, see #BOOST_NOWIDE_REPLACEMENT_CHARACTER
    ///
    template<typename CharOut, typename CharIn>
    std::basic_string<CharOut> basic_convert(CharIn const *begin, CharIn const *end)
    {
        std::basic_string<CharOut> result;
        result.reserve(end - begin);
        typedef std::back_insert_iterator<std::basic_string<CharOut> > inserter_type;
        inserter_type inserter(result);
        using namespace boost::locale::utf;
        code_point c;
        while(begin != end)
        {
            c = utf_traits<CharIn>::decode(begin, end);
            if(c == illegal || c == incomplete)
            {
                c = BOOST_NOWIDE_REPLACEMENT_CHARACTER;
            }
            utf_traits<CharOut>::encode(c, inserter);
        }
        return result;
    }

    /// \cond INTERNAL
    namespace details {
        //
        // wcslen defined only in C99... So we will not use it
        //
        template<typename Char>
        Char const *basic_strend(Char const *s)
        {
            while(*s)
                s++;
            return s;
        }
    } // namespace details
    /// \endcond

    ///
    /// \brief Template function that converts a string \a s from one type of UTF to another UTF and returns a string containing converted
    /// value
    ///
    /// Any illegal sequences are replaced with the replacement character, see #BOOST_NOWIDE_REPLACEMENT_CHARACTER
    ///
    template<typename CharOut, typename CharIn>
    std::basic_string<CharOut> basic_convert(std::basic_string<CharIn> const &s)
    {
        return basic_convert<CharOut>(s.c_str(), s.c_str() + s.size());
    }
    ///
    /// \brief Template function that converts a string \a s from one type of UTF to another UTF and returns a string containing converted
    /// value
    ///
    /// Any illegal sequences are replaced with the replacement character, see #BOOST_NOWIDE_REPLACEMENT_CHARACTER
    ///
    template<typename CharOut, typename CharIn>
    std::basic_string<CharOut> basic_convert(CharIn const *s)
    {
        return basic_convert<CharOut>(s, details::basic_strend(s));
    }

    ///
    /// Convert NULL terminated UTF source string to NULL terminated \a output string of size at
    /// most output_size (including NULL)
    ///
    /// In case of success output is returned, if there is not enough room NULL is returned.
    /// Any illegal sequences are replaced with the replacement character, see #BOOST_NOWIDE_REPLACEMENT_CHARACTER
    ///
    inline char *narrow(char *output, size_t output_size, wchar_t const *source)
    {
        return basic_convert(output, output_size, source, details::basic_strend(source));
    }
    ///
    /// Convert UTF text in range [begin,end) to NULL terminated \a output string of size at
    /// most output_size (including NULL)
    ///
    /// In case of success output is returned, if there is not enough room NULL is returned.
    /// Any illegal sequences are replaced with the replacement character, see #BOOST_NOWIDE_REPLACEMENT_CHARACTER
    ///
    inline char *narrow(char *output, size_t output_size, wchar_t const *begin, wchar_t const *end)
    {
        return basic_convert(output, output_size, begin, end);
    }
    ///
    /// Convert NULL terminated UTF source string to NULL terminated \a output string of size at
    /// most output_size (including NULL)
    ///
    /// In case of success output is returned, if there is not enough room NULL is returned.
    /// Any illegal sequences are replaced with the replacement character, see #BOOST_NOWIDE_REPLACEMENT_CHARACTER
    ///
    inline wchar_t *widen(wchar_t *output, size_t output_size, char const *source)
    {
        return basic_convert(output, output_size, source, details::basic_strend(source));
    }
    ///
    /// Convert UTF text in range [begin,end) to NULL terminated \a output string of size at
    /// most output_size (including NULL)
    ///
    /// In case of success output is returned, if there is not enough room NULL is returned.
    /// Any illegal sequences are replaced with the replacement character, see #BOOST_NOWIDE_REPLACEMENT_CHARACTER
    ///
    inline wchar_t *widen(wchar_t *output, size_t output_size, char const *begin, char const *end)
    {
        return basic_convert(output, output_size, begin, end);
    }

    ///
    /// Convert between Wide - UTF-16/32 string and UTF-8 string.
    ///
    /// Any illegal sequences are replaced with the replacement character, see #BOOST_NOWIDE_REPLACEMENT_CHARACTER
    ///
    inline std::string narrow(wchar_t const *s)
    {
        return basic_convert<char>(s);
    }
    ///
    /// Convert between UTF-8 and UTF-16 string
    ///
    /// Any illegal sequences are replaced with the replacement character, see #BOOST_NOWIDE_REPLACEMENT_CHARACTER
    ///
    inline std::wstring widen(char const *s)
    {
        return basic_convert<wchar_t>(s);
    }
    ///
    /// Convert between Wide - UTF-16/32 string and UTF-8 string
    ///
    /// Any illegal sequences are replaced with the replacement character, see #BOOST_NOWIDE_REPLACEMENT_CHARACTER
    ///
    inline std::string narrow(std::wstring const &s)
    {
        return basic_convert<char>(s);
    }
    ///
    /// Convert between UTF-8 and UTF-16 string
    ///
    /// Any illegal sequences are replaced with the replacement character, see #BOOST_NOWIDE_REPLACEMENT_CHARACTER
    ///
    inline std::wstring widen(std::string const &s)
    {
        return basic_convert<wchar_t>(s);
    }

} // namespace nowide
} // namespace boost

#endif
///
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
