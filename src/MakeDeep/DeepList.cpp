/* DeepList.cpp */
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

#include <stdexcept>
#include <fstream>

#include "DeepList.h"

namespace App {

/* class IndexSet */

IndexSet::IndexSet()
 {
  list.reserve(BaseReserve);
 }

void IndexSet::add(size_t elem)
 {
  if( indexSet.insert(elem).second )
    {
     list.push_back(elem);
    }
 }

/* class DeepList */

DeepList::FillList::FillList(Rec &obj,size_t recind)
 {
  obj.lock=true;

  rec=&obj;
  baseList.add(recind);
  cur=0;
  ind=0;
 }

auto DeepList::FillList::fill(Rec *list) -> FillResult
 {
  for(; baseList.test(cur) ;ind=0,cur++)
    {
     const std::vector<size_t> &indexes=list[baseList.get(cur)].indexes;

     size_t size=indexes.size();

     while( ind<size )
       {
        size_t i=indexes[ind++];
        Rec *ptr=list+i;

        if( ptr->flag )
          {
           if( ptr->isFirst() )
             {
              if( ptr->lock )
                {
                 std::cout << "Pregen target makes a circle " << ptr->path << std::endl ;

                 throw std::runtime_error("'Makefile-deep' creation error: cyclic pregen target dependencies found");
                }

              return { .recind=i };
             }
           else
             {
              baseList.add(i);
             }
          }
       }
    }

  return { .done=true };
 }

size_t DeepList::FillList::copy(Rec *list,Rec **olist) const
 {
  Rec **start=olist;

  if( rec->kind==TargetPregen )
    {
     baseList.applyOther( [list,&olist] (size_t ind)
                          {
                           Rec *ptr=list+ind;

                           if( ptr->flag && ptr->kind==TargetLib )
                             {
                              ptr->flag=false;
                              *(olist++)=ptr;
                             }

                          } );
    }
  else // TargetMake || TargetGroup
    {
     baseList.applyOther( [list,&olist] (size_t ind)
                          {
                           Rec *ptr=list+ind;

                           if( ptr->flag && ptr->kind==TargetLib )
                             {
                              ptr->flag=false;
                              *(olist++)=ptr;
                             }

                          } );

     baseList.applyOther( [list,&olist] (size_t ind)
                          {
                           Rec *ptr=list+ind;

                           if( ptr->flag )
                             {
                              ptr->flag=false;
                              *(olist++)=ptr;
                             }

                          } );
    }

  rec->flag=false;
  *(olist++)=rec;

  return olist-start;
 }

TargetKind DeepList::GetKind(String fileName)
 {
  std::ifstream inp(fileName);

  if( !inp.is_open() )
    {
     std::cout << "Cannot open file " << fileName << std::endl ;

     throw std::runtime_error("no such file");
    }

  inp.exceptions(std::ifstream::badbit);

  String tag;

  inp >> tag ;

  if( tag==GetTag(TargetLib) ) return TargetLib;
  if( tag==GetTag(TargetExe) ) return TargetExe;
  if( tag==GetTag(TargetPregen) ) return TargetPregen;
  if( tag==GetTag(TargetMake) ) return TargetMake;
  if( tag==GetTag(TargetGroup) ) return TargetGroup;

  std::cout << "Bad type tag: " << tag << std::endl ;

  throw std::runtime_error("bad type tag");
 }

size_t DeepList::add(String path)
 {
  auto ptr=indexMap.find(path);

  if( ptr!=indexMap.end() )
    {
     return (*ptr).second;
    }
  else
    {
     Path dir(path);

     BaseList ext(dir/"BaseList.txt");

     size_t ret=list.size();

     list.emplace_back(path,std::move(ext.getList()),GetKind(dir/"TargetKind.txt"));

     indexMap.emplace(std::move(path),ret);

     return ret;
    }
 }

void DeepList::push(size_t recind)
 {
  stack.emplace_back(list[recind],recind);
 }

void DeepList::pop()
 {
  stack.pop_back();
 }

auto DeepList::top() -> FillList &
 {
  return stack[stack.size()-1];
 }

void DeepList::orderFirst(size_t recind)
 {
  push(recind);

  while( !stack.empty() )
    {
     FillList &obj=top();

     auto res=obj.fill(list.data());

     if( res.done )
       {
        osize+=obj.copy(list.data(),olist.data()+osize);

        pop();
       }
     else
       {
        push(res.recind);
       }
    }
 }

DeepList::DeepList()
 {
  list.reserve(BigReserve);
  stack.reserve(BaseReserve);

  add(FullBaseList::GetSelf());

  for(size_t ind=0; ind<list.size() ;ind++)
    {
     size_t count=list[ind].bases.size();

     std::vector<size_t> indexes(count);

     for(size_t i=0; i<count ;i++)
       {
        indexes[i]=add(list[ind].bases[i]);
       }

     list[ind].indexes=std::move(indexes);
    }
 }

DeepList::~DeepList()
 {
 }

void DeepList::order()
 {
  size_t count=list.size();

  olist.resize(count);

  for(size_t ind=0; ind<count ;ind++)
    {
     if( list[ind].isFirst() && list[ind].flag )
       {
        orderFirst(ind);
       }
    }

  size_t libInd=osize;
  size_t exeInd=count;

  for(size_t ind=0; ind<count ;ind++)
    {
     if( list[ind].flag )
       {
        if( list[ind].kind==TargetLib )
          {
           olist[libInd++]=&list[ind];
          }
        else // TargetExe
          {
           olist[--exeInd]=&list[ind];
          }
       }
    }

  if( libInd!=exeInd )
    {
     throw std::runtime_error("'Makefile-deep' creation error: internal 'libInd!=exeInd'");
    }

  osize=count;
 }

void DeepList::buildMakeDeep()
 {
  std::ofstream out("Makefile-deep");

  out << "# Makefile-deep AUTOGENERATED, DON'T EDIT\n\n" ;

  out << "deep:\n" ;

  for(Rec *rec : olist )
    {
     out << "\t$(MAKE) -C " << rec->path << "\n" ;
    }

  out.close();

  if( !out )
    {
     throw std::runtime_error("'Makefile-deep' creation error");
    }
 }

} // namespace App

