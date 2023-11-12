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

/* functions */

inline Path CurPath() { return std::filesystem::current_path(); }

inline Path Relative(const Path &path,const Path &base) { return path.lexically_relative(base); }

inline void DestroyDir(const Path &path) { std::filesystem::remove_all(path); }

/* copy functions */

template <class T>
void ForwardCopy(T *dst,const T *src,unsigned len)
 {
  for(; len ;len--,dst++,src++) *dst=*src;
 }

} // namespace App

#endif
