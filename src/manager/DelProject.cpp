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

void DelProject(Path curpath,Path forge,Path proj,const char *const*build,int buildCount)
 {
  ProjectReader inp(curpath/proj/"PROJECT");

  const String &projName=inp.getName();

  DelProjectByName(forge,projName,build,buildCount);
 }

} // namespace App
