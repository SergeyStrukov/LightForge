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

#include <stdexcept>
#include <cstring>

#include "Command.h"
#include "Tools.h"

namespace App {

/* class Opt */

class Opt
 {
   Path curpath=CurPath();

   enum CommandType
    {
     CommandAdd,
     CommandDel,
     CommandDelProj
    };

   CommandType cmd;
   const char *self;
   const char *path;

   const char *const*build;
   int buildCount;

  private:

   Opt(const Opt &) = delete ;

   Opt & operator = (const Opt &) = delete ;

   static bool TestStr(const char *str1,const char *str2)
    {
     return std::strcmp(str1,str2)==0;
    }

   static CommandType Command(const char *arg)
    {
     if( TestStr(arg,"add") ) return CommandAdd;

     if( TestStr(arg,"del") ) return CommandDel;

     if( TestStr(arg,"delproj") ) return CommandDelProj;

     throw std::runtime_error("unknown command");

     return CommandDelProj;
    }

   static const char * CommandName(CommandType cmd)
    {
     switch( cmd )
       {
        case CommandAdd : return "add";
        case CommandDel : return "del";
        case CommandDelProj : return "delproj";

        default: return "???";
       }
    }

  public:

   Opt(int argc,const char *const*argv);

   int commit();

   void print(std::ostream &out) const;
 };

Opt::Opt(int argc,const char *const*argv)
 {
  if( argc<3 )
    {
     throw std::runtime_error("bad argument's number");
    }

  self=argv[0];
  cmd=Command(argv[1]);
  path=argv[2];
  build=argv+3;
  buildCount=argc-3;
 }

void Opt::print(std::ostream &out) const
 {
  out << "self: " << self << std::endl ;
  out << "wdir: " << curpath.c_str() << std::endl ;
  out << CommandName(cmd) << std::endl ;
  out << "path: " << path << std::endl ;

  for(int i=0; i<buildCount ;i++)
    {
     out << "   build: " << build[i] << std::endl ;
    }
 }

int Opt::commit()
 {
  Path forge=Path(self).parent_path();

  switch( cmd )
    {
     case CommandAdd :
      {
       AddProject(curpath,forge,path,build,buildCount);
      }
     break;

     case CommandDel :
      {
       DelProject(curpath,forge,path,build,buildCount);
      }
     break;

     case CommandDelProj :
      {
       DelProjectByName(forge,path,build,buildCount);
      }
     break;
    }

  return 0;
 }

/* Main() */

int Main(int argc,const char *const*argv)
 {
  std::cout << "LightForge manager 1.00" << std::endl ;
  std::cout << "Copyright (c) 2023 Sergey Strukov. All rights reserved." << std::endl << std::endl ;

  Opt opt(argc,argv);

  return opt.commit();
 }

} // namespace App

/* main() */

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



