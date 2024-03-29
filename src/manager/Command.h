/* Command.h */
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

#ifndef Command_h
#define Command_h

#include "Tools.h"

namespace App {

/* functions */

void AddProject(Path curpath,Path forge,Path proj,const char *const*build,int buildCount);

void DelProject(Path curpath,Path forge,Path proj,const char *const*build,int buildCount);

void DelProjectByName(Path forge,Path proj,const char *const*build,int buildCount);

} // namespace App

#endif

