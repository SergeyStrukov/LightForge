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
#include <algorithm>

#include "FullBaseList.h"

namespace App {

/* classes */

class IndexSet;
class DeepList;

/* class IndexSet */

class IndexSet
 {
   std::set<size_t> indexSet;
   std::vector<size_t> list;

  public:

   IndexSet();

   void add(size_t elem);

   bool test(size_t ind) const { return ind<list.size(); }

   size_t get(size_t ind) const { return list[ind]; }

   template <class Func>
   void applyOther(Func func) const
    {
     std::for_each(list.begin()+1,list.end(),func);
    }
 };

/* class DeepList */

class DeepList
 {
   struct Rec
    {
     String path;
     std::vector<String> bases;
     std::vector<size_t> indexes;
     TargetKind kind;
     bool flag = true ;
     bool lock = false ;

     bool isFirst() const { return kind==TargetPregen || kind==TargetMake || kind==TargetGroup ; }

     Rec(const String &path_,std::vector<String> &&bases_,TargetKind kind_) : path(path_),bases(std::move(bases_)),kind(kind_) {}
    };

   std::vector<Rec> list;
   std::map<String,size_t> indexMap;

   struct FillList
    {
     Rec *rec;
     IndexSet baseList;
     size_t cur;
     size_t ind;

     FillList(Rec &obj,size_t recind);

     struct FillResult
      {
       bool done = false ;
       size_t recind = 0 ;
      };

     FillResult fill(Rec *list);

     size_t copy(Rec *list,Rec **olist) const;
    };

   std::vector<FillList> stack;

   std::vector<Rec *> olist;
   size_t osize = 0 ;

  private:

   DeepList(const DeepList &) = delete ;

   DeepList & operator = (const DeepList &) = delete ;

   static TargetKind GetKind(String fileName);

   size_t add(String path);

   void push(size_t recind);
   void pop();
   FillList & top();

   void orderFirst(size_t recind);

  public:

   DeepList();

   ~DeepList();

   void order();

   void buildMakeDeep();
 };

} // namespace App

#endif

