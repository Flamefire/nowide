//
//  Copyright (c) 2012 Artyom Beilis (Tonkikh)
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_NOWIDE_FSTREAM_INCLUDED_HPP
#define BOOST_NOWIDE_FSTREAM_INCLUDED_HPP

#include <boost/nowide/config.hpp>
#include <boost/nowide/filebuf.hpp>
#if BOOST_NOWIDE_USE_WIN_FSTREAM
#include <iosfwd>
#include <streambuf>
#include <cstdio>
#ifdef BOOST_NOWIDE_USE_FILESYSTEM
#include <boost/filesystem/path.hpp>
#endif
#else
#ifdef BOOST_NOWIDE_USE_FILESYSTEM
#include <boost/filesystem/fstream.hpp>
#else
#include <fstream>
#endif
#endif

namespace boost {
///
/// \brief This namespace includes implementation of the standard library functios
/// such that they accept UTF-8 strings on Windows. On other platforms it is just an alias
/// of std namespace (i.e. not on Windows)
///
namespace nowide {
#if !BOOST_NOWIDE_USE_WIN_FSTREAM && !defined(BOOST_NOWIDE_DOXYGEN)
#ifdef BOOST_NOWIDE_USE_FILESYSTEM
#define BOOST_NOWIDE_FS_NS boost::filesystem
#else
#define BOOST_NOWIDE_FS_NS std
#endif
    using BOOST_NOWIDE_FS_NS::basic_ifstream;
    using BOOST_NOWIDE_FS_NS::basic_ofstream;
    using BOOST_NOWIDE_FS_NS::basic_fstream;
    using BOOST_NOWIDE_FS_NS::ifstream;
    using BOOST_NOWIDE_FS_NS::ofstream;
    using BOOST_NOWIDE_FS_NS::fstream;
#undef BOOST_NOWIDE_FS_NS

#else
    ///
    /// \brief Same as std::basic_ifstream<char> but accepts UTF-8 strings under Windows
    ///
    template<typename CharType, typename Traits = std::char_traits<CharType> >
    class basic_ifstream : public std::basic_istream<CharType, Traits>
    {
    public:
        typedef basic_filebuf<CharType, Traits> internal_buffer_type;
        typedef std::basic_istream<CharType, Traits> internal_stream_type;

        basic_ifstream() : internal_stream_type(NULL)
        {
            this->init(&buf_);
        }

        explicit basic_ifstream(char const *file_name, std::ios_base::openmode mode = std::ios_base::in) : internal_stream_type(NULL)
        {
            this->init(&buf_);
            open(file_name, mode);
        }

        explicit basic_ifstream(std::string const &file_name, std::ios_base::openmode mode = std::ios_base::in) : internal_stream_type(NULL)
        {
            this->init(&buf_);
            open(file_name, mode);
        }

        void open(wchar_t const *file_name, std::ios_base::openmode mode = std::ios_base::in)
        {
            if(!buf_.open(file_name, mode | std::ios_base::in))
                this->setstate(std::ios_base::failbit);
            else
                this->clear();
        }
#ifdef BOOST_NOWIDE_USE_FILESYSTEM
        explicit basic_ifstream(boost::filesystem::path const &file_path, std::ios_base::openmode mode = std::ios_base::in) :
            internal_stream_type(NULL)
        {
            this->init(&buf_);
            open(file_path, mode);
        }
        void open(boost::filesystem::path const &file_path, std::ios_base::openmode mode = std::ios_base::in)
        {
            open(file_path.c_str(), mode);
        }
#endif

        void open(std::string const &file_name, std::ios_base::openmode mode = std::ios_base::in)
        {
            open(file_name.c_str(), mode);
        }
        void open(char const *file_name, std::ios_base::openmode mode = std::ios_base::in)
        {
            if(!buf_.open(file_name, mode | std::ios_base::in))
                this->setstate(std::ios_base::failbit);
            else
                this->clear();
        }
        bool is_open() const
        {
            return buf_.is_open();
        }
        void close()
        {
            if(!buf_.close())
                this->setstate(std::ios_base::failbit);
        }

        internal_buffer_type *rdbuf() const
        {
            return const_cast<internal_buffer_type *>(&buf_);
        }

    private:
        internal_buffer_type buf_;
    };

    ///
    /// \brief Same as std::basic_ofstream<char> but accepts UTF-8 strings under Windows
    ///

    template<typename CharType, typename Traits = std::char_traits<CharType> >
    class basic_ofstream : public std::basic_ostream<CharType, Traits>
    {
    public:
        typedef basic_filebuf<CharType, Traits> internal_buffer_type;
        typedef std::basic_ostream<CharType, Traits> internal_stream_type;

        basic_ofstream() : internal_stream_type(NULL)
        {
            this->init(&buf_);
        }
        explicit basic_ofstream(char const *file_name, std::ios_base::openmode mode = std::ios_base::out) : internal_stream_type(NULL)
        {
            this->init(&buf_);
            open(file_name, mode);
        }
        explicit basic_ofstream(std::string const &file_name, std::ios_base::openmode mode = std::ios_base::out) :
            internal_stream_type(NULL)
        {
            this->init(&buf_);
            open(file_name, mode);
        }

        void open(wchar_t const *file_name, std::ios_base::openmode mode = std::ios_base::out)
        {
            if(!buf_.open(file_name, mode | std::ios_base::out))

                this->setstate(std::ios_base::failbit);
            else
                this->clear();
        }
#ifdef BOOST_NOWIDE_USE_FILESYSTEM
        explicit basic_ofstream(boost::filesystem::path const &file_path, std::ios_base::openmode mode = std::ios_base::out) :
            internal_stream_type(NULL)
        {
            this->init(&buf_);
            open(file_path, mode);
        }
        void open(boost::filesystem::path const &file_path, std::ios_base::openmode mode = std::ios_base::out)
        {
            open(file_path.c_str(), mode);
        }
#endif

        void open(std::string const &file_name, std::ios_base::openmode mode = std::ios_base::out)
        {
            open(file_name.c_str(), mode);
        }
        void open(char const *file_name, std::ios_base::openmode mode = std::ios_base::out)
        {
            if(!buf_.open(file_name, mode | std::ios_base::out))
                this->setstate(std::ios_base::failbit);
            else
                this->clear();
        }
        bool is_open() const
        {
            return buf_.is_open();
        }
        void close()
        {
            if(!buf_.close())
                this->setstate(std::ios_base::failbit);
        }

        internal_buffer_type *rdbuf() const
        {
            return const_cast<internal_buffer_type *>(&buf_);
        }

    private:
        internal_buffer_type buf_;
    };

    ///
    /// \brief Same as std::basic_fstream<char> but accepts UTF-8 strings under Windows
    ///

    template<typename CharType, typename Traits = std::char_traits<CharType> >
    class basic_fstream : public std::basic_iostream<CharType, Traits>
    {
    public:
        typedef basic_filebuf<CharType, Traits> internal_buffer_type;
        typedef std::basic_iostream<CharType, Traits> internal_stream_type;

        basic_fstream() : internal_stream_type(NULL)
        {
            this->init(&buf_);
        }
        explicit basic_fstream(char const *file_name, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out) :
            internal_stream_type(NULL)
        {
            this->init(&buf_);
            open(file_name, mode);
        }
        explicit basic_fstream(std::string const &file_name, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out) :
            internal_stream_type(NULL)
        {
            this->init(&buf_);
            open(file_name, mode);
        }

        void open(wchar_t const *file_name, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out)
        {
            if(!buf_.open(file_name, mode))
                this->setstate(std::ios_base::failbit);
            else
                this->clear();
        }
#ifdef BOOST_NOWIDE_USE_FILESYSTEM
        explicit basic_fstream(boost::filesystem::path const &file_path,
                               std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out) :
            internal_stream_type(NULL)
        {
            this->init(&buf_);
            open(file_path, mode);
        }
        void open(boost::filesystem::path const &file_path, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out)
        {
            open(file_path.c_str(), mode);
        }
#endif

        void open(std::string const &file_name, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out)
        {
            open(file_name.c_str(), mode);
        }
        void open(char const *file_name, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out)
        {
            if(!buf_.open(file_name, mode))
                this->setstate(std::ios_base::failbit);
            else
                this->clear();
        }
        bool is_open() const
        {
            return buf_.is_open();
        }
        void close()
        {
            if(!buf_.close())
                this->setstate(std::ios_base::failbit);
        }

        internal_buffer_type *rdbuf() const
        {
            return const_cast<internal_buffer_type *>(&buf_);
        }

    private:
        internal_buffer_type buf_;
    };

    ///
    /// \brief Same as std::filebuf but accepts UTF-8 strings under Windows
    ///
    typedef basic_filebuf<char> filebuf;
    ///
    /// Same as std::ifstream but accepts UTF-8 strings under Windows
    ///
    typedef basic_ifstream<char> ifstream;
    ///
    /// Same as std::ofstream but accepts UTF-8 strings under Windows
    ///
    typedef basic_ofstream<char> ofstream;
    ///
    /// Same as std::fstream but accepts UTF-8 strings under Windows
    ///
    typedef basic_fstream<char> fstream;

#endif
} // namespace nowide
} // namespace boost

#endif
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
