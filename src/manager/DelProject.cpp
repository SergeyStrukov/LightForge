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

#include "Command.h"
#include "ProjectReader.h"

namespace App {

template <class S>
static void DelProjectFolder(const Path &forge,S build,const String &projName)
 {
  Path folder=forge/"build"/build/projName;

  DestroyDir(folder);
 }

void DelProject(Path curpath,Path forge,Path proj,const char *const*build,int buildCount)
 {
  ProjectReader inp(curpath/proj/"PROJECT");

  const String &projName=inp.getName();

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

  String infoFile=forge/"PROJECTS";

  ProjectListReader info(infoFile);

  info.delProject(projName);

  info.save(infoFile);
 }

} // namespace App
