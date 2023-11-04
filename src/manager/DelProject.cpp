/* DelProject.cpp */
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

#include <iostream>

#include "Command.h"
#include "Reader.h"

namespace App {

template <class S>
static void DelProjectFolder(Path forge,S build,String name)
 {
  Path folder=forge/"build"/build/name;

  std::filesystem::remove_all(folder);
 }

void DelProject(Path curpath,Path forge,Path proj,const char **build,int buildCount)
 {
  ProjectReader inp(curpath/proj/"PROJECT");

  String projName=inp.getName();

  if( buildCount )
    {
     for(int i=0; i<buildCount ;i++)
       {
        DelProjectFolder(forge,build[i],projName);
       }
    }
  else
    {
     Directory dirList(forge/"build");

     for(const auto &entry : dirList )
       {
        if( entry.is_directory() )
          {
           DelProjectFolder(forge,entry.path().filename(),projName);
          }
       }
    }

  String infoFile = forge/"projects";

  ProjectListReader info(infoFile);

  info.delProject(projName);

  info.save(infoFile);
 }

} // namespace App
