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

namespace App {

bool TestStr(const char *str1,const char *str2)
 {
  return strcmp(str1,str2)==0;
 }

class Opt
 {
   filesystem::path curpath=filesystem::current_path();

   bool add;
   const char *self;
   const char *path;

   const char **build;
   int buildCount;

  private:

   Opt(const Opt &) = delete;

   Opt & operator = (const Opt &) = delete;

   static bool Command(const char *arg)
    {
     if( TestStr(arg,"add") ) return true;

     if( !TestStr(arg,"del") )
       {
        throw runtime_error("unknown command");
       }

     return false;
    }

  public:

   Opt(int argc,const char *argv[])
    {
     if( argc<3 )
       {
        throw runtime_error("bad argument's number");
       }

     self=argv[0];
     add=Command(argv[1]);
     path=argv[2];
     build=argv+3;
     buildCount=argc-3;
    }

   void print(ostream &out) const
    {
     out << "self: " << self << endl ;
     out << "wdir: " << curpath.c_str() << endl ;
     out << (add? "add":"del") << endl ;
     out << "path: " << path << endl ;

     for(int i=0; i<buildCount ;i++)
       {
        out << "   build: " << build[i] << endl ;
       }
    }

   int commit()
    {
     cout << (*this) << endl ;

     filesystem::path forge=filesystem::path(self).parent_path();

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

int Main(int argc,const char *argv[])
 {
  cout << "LightForge manager 1.00" << endl ;
  cout << "Copyright (c) 2023 Sergey Strukov. All rights reserved." << endl << endl ;

  Opt opt(argc,argv);

  return opt.commit();
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



