/* main.cpp */
//----------------------------------------------------------------------------------------
//
//  Project: LightForge 1.00
//
//  License: Boost Software License - Version 1.0 - August 17th, 2003
//
//            see http://www.boost.org/LICENSE_1_0.txt or the local copy
//
//  Copyright (c) 2023 Sergey Strukov. All rights reserved.
//
//----------------------------------------------------------------------------------------

#include <exception>
#include <stdexcept>
#include <iostream>

namespace App {

using namespace std;

int Main(int argc,const char *argv[])
 {
  cout << "LightForge MakeList 1.00" << endl ;
  cout << "Copyright (c) 2023 Sergey Strukov. All rights reserved." << endl << endl ;


  return 0;
 }

} // namespace App

int main(int argc,const char *argv[])
 {
  try
    {
     return App::Main(argc,argv);
    }
  catch(std::exception &ex)
    {
     std::cout << std::endl << "Exception: " << ex.what() << std::endl ;

     return 1;
    }
  catch(...)
    {
     std::cout << std::endl << "Unknown exception" << std::endl ;

     return 1;
    }
 }



