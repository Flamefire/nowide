//
//  Copyright (c) 2012 Artyom Beilis (Tonkikh)
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/nowide/convert.hpp>
#include <boost/nowide/stackstring.hpp>
#include "test.hpp"
#include "test_sets.hpp"
#include <iostream>

int main()
{
    try
    {
        std::string hello = "\xd7\xa9\xd7\x9c\xd7\x95\xd7\x9d";
        std::wstring whello = L"\u05e9\u05dc\u05d5\u05dd";
        std::wstring whello_3e = L"\u05e9\u05dc\u05d5\ufffd";
        std::wstring whello_3 = L"\u05e9\u05dc\u05d5";
        std::cout << "- boost::nowide::widen" << std::endl;
        {
            char const *b = hello.c_str();
            char const *e = b + 8;
            wchar_t buf[6] = {0, 0, 0, 0, 0, 1};
            TEST(boost::nowide::widen(buf, 5, b, e) == buf);
            TEST(buf == whello);
            TEST(buf[5] == 1);
            TEST(boost::nowide::widen(buf, 4, b, e) == 0);
            TEST(boost::nowide::widen(buf, 5, b, e - 1) == buf);
            TEST(buf == whello_3e);
            TEST(boost::nowide::widen(buf, 5, b, e - 2) == buf);
            TEST(buf == whello_3);
            TEST(boost::nowide::widen(buf, 5, b, b) == buf && buf[0] == 0);
            TEST(boost::nowide::widen(buf, 5, b, b + 2) == buf && buf[1] == 0 && buf[0] == whello[0]);
            b = "\xFF\xFF";
            e = b + 2;
            TEST(boost::nowide::widen(buf, 5, b, e) == buf);
            TEST(buf == std::wstring(L"\ufffd\ufffd"));
            b = "\xd7\xa9\xFF";
            e = b + 3;
            TEST(boost::nowide::widen(buf, 5, b, e) == buf);
            TEST(buf == std::wstring(L"\u05e9\ufffd"));
            TEST(boost::nowide::widen(buf, 5, b, b + 1) == buf);
            TEST(buf == std::wstring(L"\ufffd"));
            b = "\xFF\xd7\xa9";
            e = b + 3;
            TEST(boost::nowide::widen(buf, 5, b, e) == buf);
            TEST(buf == std::wstring(L"\ufffd\u05e9"));
        }
        std::cout << "- boost::nowide::narrow" << std::endl;
        {
            wchar_t const *b = whello.c_str();
            wchar_t const *e = b + 4;
            char buf[10] = {0};
            buf[9] = 1;
            TEST(boost::nowide::narrow(buf, 9, b, e) == buf);
            TEST(buf == hello);
            TEST(buf[9] == 1);
            TEST(boost::nowide::narrow(buf, 8, b, e) == 0);
            TEST(boost::nowide::narrow(buf, 7, b, e - 1) == buf);
            TEST(buf == hello.substr(0, 6));
            wchar_t tmp[3] = {0xDC01, 0x05e9, 0};
            b = tmp;
            TEST(boost::nowide::narrow(buf, 10, b, b + 2) == buf);
            TEST(buf == std::string("\xEF\xBF\xBD\xd7\xa9"));
            wchar_t tmp2[3] = {0x05e9, 0xD800, 0};
            b = tmp2;
            TEST(boost::nowide::narrow(buf, 10, b, b + 2) == buf);
            TEST(buf == std::string("\xd7\xa9\xEF\xBF\xBD"));
        }
        {
            char buf[3];
            wchar_t wbuf[3];
            TEST(boost::nowide::narrow(buf, 3, L"xy") == std::string("xy"));
            TEST(boost::nowide::widen(wbuf, 3, "xy") == std::wstring(L"xy"));
        }
        std::cout << "- Substitutions" << std::endl;
        run_all(boost::nowide::widen, boost::nowide::narrow);
    } catch(std::exception const &e)
    {
        std::cerr << "Failed :" << e.what() << std::endl;
        return 1;
    }
    std::cout << "Passed" << std::endl;
    return 0;
}

///
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
