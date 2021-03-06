#include <boost/nowide/iostream.hpp>
#include <boost/locale/utf.hpp>
#include "test.hpp"
#include <iostream>

bool isValidUTF8(const std::string &s)
{
    using namespace boost::locale::utf;
    for(std::string::const_iterator it = s.begin(); it != s.end();)
    {
        code_point c = utf_traits<char>::decode(it, s.end());
        if(!is_valid_codepoint(c))
            return false;
    }
    return true;
}

int main(int argc, char **argv)
{
    char const *example = "Basic letters: \xd7\xa9-\xd0\xbc-\xce\xbd\n"
                          "East Asian Letters: \xe5\x92\x8c\xe5\xb9\xb3\n"
                          "Non-BMP letters: \xf0\x9d\x84\x9e\n"
                          "Invalid UTF-8: `\xFF' `\xd7\xFF' `\xe5\xFF\x8c' `\xf0\x9d\x84\xFF' \n"
                          "\n";

    try
    {
        // If we are using the standard rdbuf we can only put back 1 char
        if(boost::nowide::cin.rdbuf() == std::cin.rdbuf())
        {
            std::cout << "Using std::cin" << std::endl;
            int maxval = 15000;
            for(int i = 0; i < maxval; i++)
            {
                char c = i % 96 + ' ';
                TEST(boost::nowide::cin.putback(c));
                int ci = i % 96 + ' ';
                TEST(boost::nowide::cin.get() == ci);
            }
        } else
        {
            int maxval = 15000;
            for(int i = 0; i < maxval; i++)
            {
                char c = i % 96 + ' ';
                TEST(boost::nowide::cin.putback(c));
            }
            for(int i = maxval - 1; i >= 0; i--)
            {
                int c = i % 96 + ' ';
                TEST(boost::nowide::cin.get() == c);
            }
        }
        boost::nowide::cout << "Normal I/O:" << std::endl;
        boost::nowide::cout << example << std::endl;
        boost::nowide::cerr << example << std::endl;

        boost::nowide::cout << "Flushing each character:" << std::endl;

        for(char const *s = example; *s; s++)
        {
            boost::nowide::cout << *s << std::flush;
            TEST(boost::nowide::cout);
        }

        TEST(boost::nowide::cout);
        TEST(boost::nowide::cerr);
        if(argc == 2 && argv[1] == std::string("-i"))
        {
            std::string v1, v2;
            boost::nowide::cin >> v1 >> v2;
            TEST(boost::nowide::cin);
            TEST(isValidUTF8(v1));
            TEST(isValidUTF8(v2));
            boost::nowide::cout << "First:  " << v1 << std::endl;
            boost::nowide::cout << "Second: " << v2 << std::endl;
            TEST(boost::nowide::cout);
        }
    } catch(std::exception const &e)
    {
        std::cerr << "Fail: " << e.what() << std::endl;
        return 1;
    }
    std::cout << "Ok" << std::endl;
    return 0;
}
