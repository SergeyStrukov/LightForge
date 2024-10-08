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
#include <iostream>

namespace App {

/* using */

using String = std::string ;
using Path = std::filesystem::path ;
using Directory = std::filesystem::directory_iterator ;
using DirTree = std::filesystem::recursive_directory_iterator ;

/* concepts */

template <class T>
concept HasPrint = requires(T obj,std::ostream &out)
 {
  obj.print(out);
 } ;

/* consts */

inline constexpr size_t BaseReserve = 100 ;
inline constexpr size_t BigReserve = 1000 ;

enum TargetKind
 {
  TargetLib,
  TargetExe,
  TargetPregen,
  TargetMake,
  TargetGroup
 };

/* functions */

inline constexpr const char * GetTag(TargetKind kind)
 {
  switch( kind )
    {
     case TargetLib : return "lib" ;
     case TargetExe : return "exe" ;
     case TargetPregen : return "pregen" ;
     case TargetMake : return "make" ;
     case TargetGroup : return "group";

     default: return "???";
    }
 }

inline bool PathExists(const Path &path) { return std::filesystem::exists(path); }

inline bool isRegularFile(const Path &path) { return std::filesystem::is_regular_file(path); }

inline Path toCanonical(const Path &path) { return std::filesystem::canonical(path); }

inline Path CurPath() { return std::filesystem::current_path(); }

inline Path Relative(const Path &path,const Path &base) { return path.lexically_relative(base); }

inline bool CreateDir(const Path &path) { return std::filesystem::create_directory(path); }

inline void DestroyDir(const Path &path) { std::filesystem::remove_all(path); }

std::ostream & operator << (std::ostream &out,const HasPrint auto &obj)
 {
  obj.print(out);

  return out;
 }

/* copy functions */

template <class T>
void ForwardCopy(T *dst,const T *src,unsigned len)
 {
  for(; len ;len--,dst++,src++) *dst=*src;
 }

} // namespace App

#endif
