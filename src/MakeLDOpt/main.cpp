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
#include <fstream>
#include <filesystem>

#include <vector>
#include <string>
#include <algorithm>

namespace App {

/* Main() */

int Main(int argc,const char *argv[])
 {
  std::cout << "LightForge MakeLDopt 1.00" << std::endl ;
  std::cout << "Copyright (c) 2023 Sergey Strukov. All rights reserved." << std::endl << std::endl ;

  return 0;
 }

} // namespace App

/* main() */

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



