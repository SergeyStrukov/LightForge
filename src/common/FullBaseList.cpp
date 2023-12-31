/* FullBaseList.cpp */
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

#include "FullBaseList.h"

namespace App {

/* class BaseList */

BaseList::BaseList(const String &fileName)
 {
  list.reserve(BaseReserve);

  std::ifstream inp(fileName);

  if( !inp.is_open() )
    {
     std::cout << "Cannot open file " << fileName << std::endl ;

     throw std::runtime_error("no such file");
    }

  inp.exceptions(std::ifstream::badbit);

  String line;

  while( std::getline(inp,line) )
    {
     list.emplace_back(std::move(line));
    }
 }

BaseList::~BaseList()
 {
 }

/* class FullBaseList */

void FullBaseList::Append(std::ostream &out,const String &fileName)
 {
  std::ifstream inp(fileName);

  if( !inp.is_open() )
    {
     std::cout << "Cannot open file " << fileName << std::endl ;

     throw std::runtime_error("no such file");
    }

  inp.seekg(0,inp.end);

  if( !inp.tellg() ) return;

  inp.seekg(0,inp.beg);

  inp.exceptions(std::ifstream::badbit);

  out << inp.rdbuf() ;
 }

void FullBaseList::add(String &&str)
 {
  if( findSet.insert(str).second )
    {
     list.emplace_back(std::move(str));
    }
 }

String FullBaseList::GetSelf()
 {
  Path path=CurPath();

  String target=path.filename();
  String proj=path.parent_path().filename();

  return Path("../..")/proj/target;
 }

FullBaseList::FullBaseList()
 {
  list.reserve(BigReserve);

  add(GetSelf());

  for(size_t ind=0; ind<list.size() ;ind++)
    {
     BaseList ext(Path(list[ind])/"BaseList.txt");

     for(String &str : ext.getList() )
       {
        add(std::move(str));
       }
    }
 }

FullBaseList::~FullBaseList()
 {
 }

void FullBaseList::buildCCopt()
 {
  std::ofstream out("CC-opt.txt");

  Append(out,"CCprivate-opt.txt");

  for(const String &str: list )
    {
     Append(out,Path(str)/"CCpublic-opt.txt");
    }

  out.close();

  if( !out )
    {
     throw std::runtime_error("'CC-opt.txt' creation error");
    }
 }

void FullBaseList::buildLDopt()
 {
  std::ofstream out("LD-opt.txt");

  out << "-Wl,--start-group\n" ;

  for(const String &str: list )
    {
     Append(out,Path(str)/"LDpublic-opt.txt");
    }

  out << "-Wl,--end-group\n" ;

  out.close();

  if( !out )
    {
     throw std::runtime_error("'LD-opt.txt' creation error");
    }
 }

} // namespace App
