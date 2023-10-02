/* AddProject.cpp */
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

#include <iostream>

namespace App {

void AddProject(filesystem::path curpath,filesystem::path forge,filesystem::path proj,const char **build,int buildCount)
 {
  cout << curpath << endl ;
  cout << forge << endl ;
  cout << proj << endl ;

  // TODO
 }

} // namespace App
