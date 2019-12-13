//
//  Copyright (c) 2015 Artyom Beilis (Tonkikh)
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifdef _MSC_VER
# define _CRT_SECURE_NO_WARNINGS
#endif

#include <boost/nowide/fstream.hpp>
#include <boost/nowide/cstdio.hpp>
#include <boost/filesystem/operations.hpp>
#include <iostream>

#include "test.hpp"


int main()
{
    std::string prefix = boost::filesystem::unique_path( "nowide-%%%%-%%%%-" ).string();

    std::string example_str = prefix + "\xd7\xa9-\xd0\xbc-\xce\xbd" ".txt";
    std::wstring wexample_str = boost::nowide::widen( prefix ) + L"\u05e9-\u043c-\u03bd.txt";
    
    char const * example = example_str.c_str();
    #ifdef BOOST_WINDOWS
    wchar_t const * wexample = wexample_str.c_str();
    #endif

    try {
        namespace nw=boost::nowide;
        
        std::cout << "Testing fstream" << std::endl;
        {
            nw::ofstream fo;
            fo.open(example);
            TEST(fo);
            fo<<"test"<<std::endl;
            fo.close();
            #ifdef BOOST_WINDOWS
            {
                FILE *tmp=_wfopen(wexample,L"r");
                TEST(tmp);
                TEST(fgetc(tmp)=='t');
                TEST(fgetc(tmp)=='e');
                TEST(fgetc(tmp)=='s');
                TEST(fgetc(tmp)=='t');
                TEST(fgetc(tmp)=='\n');
                TEST(fgetc(tmp)==EOF);
                fclose(tmp);
            }
            #endif
            {
                nw::ifstream fi;
                fi.open(example);
                TEST(fi);
                std::string tmp;
                fi  >> tmp;
                TEST(tmp=="test");
                fi.close();
            }
            {
                nw::ifstream fi(example);
                TEST(fi);
                std::string tmp;
                fi  >> tmp;
                TEST(tmp=="test");
                fi.close();
            }
            #if defined(BOOST_WINDOWS) || defined(BOOST_NOWIDE_FSTREAM_TESTS) 
            // C++11 interfaces aren't enabled at all platforms so need to skip
            // for std::*fstream
            {
                std::string name = example;
                nw::ifstream fi(name);
                TEST(fi);
                std::string tmp;
                fi  >> tmp;
                TEST(tmp=="test");
                fi.close();
            }
            {
                nw::ifstream fi;
                fi.open(std::string(example));
                TEST(fi);
                std::string tmp;
                fi  >> tmp;
                TEST(tmp=="test");
                fi.close();
            }
            #endif
            {
                nw::ifstream fi(example,std::ios::binary);
                TEST(fi);
                std::string tmp;
                fi  >> tmp;
                TEST(tmp=="test");
                fi.close();
            }

            {
                nw::ifstream fi;
                nw::remove(example);
                fi.open(example);
                TEST(!fi);
            }
            {
                nw::fstream f(example,nw::fstream::in | nw::fstream::out | nw::fstream::trunc | nw::fstream::binary);
                TEST(f);
                f << "test2" ;
                std::string tmp;
                f.seekg(0);
                f>> tmp;
                TEST(tmp=="test2");
                f.close();
            }
            {
                nw::ifstream fi(example,nw::fstream::ate | nw::fstream::binary);
                TEST(fi);
                TEST(fi.tellg()==std::streampos(5));
                fi.seekg(-2,std::ios_base::cur);
                std::string tmp;
                fi >> tmp;
                TEST(tmp == "t2");
                fi.close();
            }
            nw::remove(example);
        }
        
        for(int i=-1;i<16;i++) {
            std::cout << "Complex io with buffer = " << i << std::endl;
            char buf[16];
            nw::fstream f;
            if(i==0)
                f.rdbuf()->pubsetbuf(0,0);
            else if (i > 0) 
                f.rdbuf()->pubsetbuf(buf,i);
            
            f.open(example,nw::fstream::in | nw::fstream::out | nw::fstream::trunc | nw::fstream::binary);
            f.put('a');
            f.put('b');
            f.put('c');
            f.put('d');
            f.put('e');
            f.put('f');
            f.put('g');
            f.seekg(0);
            TEST(f.get()=='a');
            f.seekg(1,std::ios::cur);
            TEST(f.get()=='c');
            f.seekg(-1,std::ios::cur);
            TEST(f.get()=='c');
            TEST(f.seekg(1));
            f.put('B');
            TEST(f.flush()); // Flush or seek needed when changing out->in
            TEST(f.get()=='c');
            TEST(f.seekg(1));
            TEST(f.get() == 'B');
            f.seekg(2);
            f.put('C');
            TEST(f.seekg(3)); // Flush or seek needed when changing out->in
            TEST(f.get()=='d');
            f.seekg(0);
            TEST(f.get()=='a');
            TEST(f.get()=='B');
            TEST(f.get()=='C');
            TEST(f.get()=='d');
            TEST(f.get()=='e');
            TEST(f.putback('e'));
            TEST(f.putback('d'));
            TEST(f.get()=='d');
            TEST(f.get()=='e');
            TEST(f.get()=='f');
            TEST(f.get()=='g');
            TEST(f.get()==EOF);
            f.clear();
            f.seekg(1);
            TEST(f.get()=='B');
            TEST(f.putback('B'));
#if !defined(_LIBCPP_VERSION)
            // libc++ fails this putback
            TEST(f.putback('a'));
            TEST(!f.putback('x'));
#endif
            f.close();
            TEST(boost::nowide::remove(example)==0);
            
        }
    }
    catch(std::exception const &e) {
        std::cerr << e.what() << std::endl;
        BOOST_NOWIDE_TEST_RETURN_FAILURE;
    }

    return boost::report_errors();
}
