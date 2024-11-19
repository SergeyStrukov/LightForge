/* main.cpp */
//----------------------------------------------------------------------------------------
//
//  Project: LightForge 1.00
//
//  License: Boost Software License - Version 1.0 - August 17th, 2003
//
//            see http://www.boost.org/LICENSE_1_0.txt or the local copy
//
//  Copyright (c) 2024 Sergey Strukov. All rights reserved.
//
//----------------------------------------------------------------------------------------

#include <stdexcept>
#include <fstream>

#include "Tools.h"

namespace App {

/* Main() */

int Main(const char *optFileName,const char *jsonFileName)
 {
  std::cout << "LightForge MakeVSCode 1.00" << std::endl ;
  std::cout << "Copyright (c) 2024 Sergey Strukov. All rights reserved." << std::endl << std::endl ;

  std::ifstream inp(optFileName);

  Path base=Path(jsonFileName).parent_path();

  if( !PathExists(base) )
    {
     if( !CreateDir(base) )
       {
        throw std::runtime_error("cannot create directory");
       }
    }

  String line;

  std::ofstream out(jsonFileName);

  out << "{ \"C_Cpp.default.includePath\": [\n" ;

  while( (inp>>line).good() )
    {
     String path=line.substr(2);
     String relpath=Relative(path,base);

     out << "  \"" << relpath << "\",\n" ;
    }

  out << "] }\n" ;

  out.close();

  if( !out )
    {
     throw std::runtime_error("file creation error");
    }

  return 0;
 }

} // namespace App

/* main() */

int main(int argc,const char *const argv[])
 {
  try
    {
     if( argc!=3 )
       {
        std::cout << "Two arguments are expected" << std::endl ;

        return 1;
       }

     return App::Main(argv[1],argv[2]);
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



