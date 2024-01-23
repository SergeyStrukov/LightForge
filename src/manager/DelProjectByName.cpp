/* DelProjectByName.cpp */
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

  String infoFile=forge/"build"/build/"PROJECTS";

  ProjectListReader info(infoFile);

  info.delProject(projName);

  info.save(infoFile);
 }

void DelProjectByName(Path forge,Path projName,const char *const*build,int buildCount)
 {
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
 }

} // namespace App
