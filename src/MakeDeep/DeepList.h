/* DeepList.h */
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

#ifndef DeepList_h
#define DeepList_h

#include <map>

#include "FullBaseList.h"

namespace App {

/* classes */

class DeepList;

/* class DeepList */

class DeepList
 {
   struct Rec
    {
     String path;
     std::vector<String> bases;
     std::vector<size_t> indexes;
     TargetKind kind;

     Rec(const String &path_,std::vector<String> &&bases_,TargetKind kind_) : path(path_),bases(std::move(bases_)),kind(kind_) {}
    };

   std::vector<Rec> list;
   std::map<String,size_t> indexMap;

  private:

   DeepList(const DeepList &) = delete ;

   DeepList & operator = (const DeepList &) = delete ;

   static TargetKind GetKind(String fileName);

   size_t add(String path);

  public:

   DeepList();

   ~DeepList();

   void order();

   void buildMakeDeep();
 };

} // namespace App

#endif

