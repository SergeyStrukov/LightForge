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
#include <cstring>

#include "StdPrint.h"
#include "Command.h"
#include "Tools.h"

namespace App {

bool TestStr(const char *str1,const char *str2)
 {
  return std::strcmp(str1,str2)==0;
 }

class Opt
 {
   Path curpath=std::filesystem::current_path();

   bool add;
   const char *self;
   const char *path;

   const char *const*build;
   int buildCount;

  private:

   Opt(const Opt &) = delete ;

   Opt & operator = (const Opt &) = delete ;

   static bool Command(const char *arg)
    {
     if( TestStr(arg,"add") ) return true;

     if( !TestStr(arg,"del") )
       {
        throw std::runtime_error("unknown command");
       }

     return false;
    }

  public:

   Opt(int argc,const char *const*argv)
    {
     if( argc<3 )
       {
        throw std::runtime_error("bad argument's number");
       }

     self=argv[0];
     add=Command(argv[1]);
     path=argv[2];
     build=argv+3;
     buildCount=argc-3;
    }

   void print(std::ostream &out) const
    {
     out << "self: " << self << std::endl ;
     out << "wdir: " << curpath.c_str() << std::endl ;
     out << (add? "add":"del") << std::endl ;
     out << "path: " << path << std::endl ;

     for(int i=0; i<buildCount ;i++)
       {
        out << "   build: " << build[i] << std::endl ;
       }
    }

   int commit()
    {
     Path forge=std::filesystem::path(self).parent_path();

     if( add )
       {
        AddProject(curpath,forge,path,build,buildCount);
       }
     else
       {
        DelProject(curpath,forge,path,build,buildCount);
       }

     return 0;
    }
 };

int Main(int argc,const char *const*argv)
 {
  std::cout << "LightForge manager 1.00" << std::endl ;
  std::cout << "Copyright (c) 2023 Sergey Strukov. All rights reserved." << std::endl << std::endl ;

  Opt opt(argc,argv);

  return opt.commit();
 }

} // namespace App

int main(int argc,const char *const*argv)
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



