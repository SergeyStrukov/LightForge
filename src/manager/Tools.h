/* Tools.h */
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

#ifndef Tools_h
#define Tools_h

#include <filesystem>

namespace App {

/* using */

using String = std::string;
using Path = std::filesystem::path ;
using Directory = std::filesystem::directory_iterator ;
using DirTree = std::filesystem::recursive_directory_iterator ;

} // namespace App

#endif
