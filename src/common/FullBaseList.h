/* FullBaseList.h */
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

#ifndef FullBaseList_h
#define FullBaseList_h

#include <vector>
#include <set>

#include "Tools.h"

namespace App {

/* classes */

class BaseList;
class FullBaseList;

/* class BaseList */

class BaseList
 {
   std::vector<String> list;

  private:

   BaseList(const BaseList &) = delete ;

   BaseList & operator = (const BaseList &) = delete ;

  public:

   explicit BaseList(const String &fileName);

   ~BaseList();

   std::vector<String> & getList() { return list; }
 };

/* class FullBaseList */

class FullBaseList
 {
   std::vector<String> list;
   std::set<String> findSet;

  private:

   FullBaseList(const FullBaseList &) = delete ;

   FullBaseList & operator = (const FullBaseList &) = delete ;

   static String GetSelf();

   static void Append(std::ostream &out,const String &fileName);

   void add(String &&str);

  public:

   FullBaseList();

   ~FullBaseList();

   void buildCCopt();
   void buildLDopt();
 };

} // namespace App

#endif
