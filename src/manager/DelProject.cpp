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

#include <iostream>

namespace App {

void DelProject(Path curpath,Path forge,Path proj,const char **build,int buildCount)
 {
  std::cout << curpath << std::endl ;
  std::cout << forge << std::endl ;
  std::cout << proj << std::endl ;

  // TODO
 }

} // namespace App
