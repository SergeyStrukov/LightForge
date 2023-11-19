/* main.cpp */
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

#include <exception>
#include <stdexcept>

#include <iostream>
#include <fstream>
#include <filesystem>

#include <vector>
#include <set>
#include <string>
#include <algorithm>

namespace App {

using String = std::string ;
using Path = std::filesystem::path ;

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

BaseList::BaseList(const String &fileName)
 {
  list.reserve(100);

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

   void buildLDopt();
 };

String FullBaseList::GetSelf()
 {
  Path path=std::filesystem::current_path();

  String target=path.filename();
  String proj=path.parent_path().filename();

  return Path("../..")/proj/target;
 }

void FullBaseList::Append(std::ostream &out,const String &fileName)
 {
  std::ifstream inp(fileName,std::ios::binary);

  if( !inp.is_open() )
    {
     std::cout << "Cannot open file " << fileName << std::endl ;

     throw std::runtime_error("no such file");
    }

  int ch=inp.get();

  if( ch==-1 ) return;

  inp.putback(ch);

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

FullBaseList::FullBaseList()
 {
  list.reserve(1000);

  add(GetSelf());

  size_t ind=0;

  while( ind<list.size() )
    {
     BaseList ext(Path(list[ind])/"BaseList.txt");

     for(String &str : ext.getList() )
       {
        add(std::move(str));
       }

     ind++;
    }
 }

FullBaseList::~FullBaseList()
 {
 }

void FullBaseList::buildLDopt()
 {
  std::ofstream out("LD-opt.txt",std::ios::binary);

  out << "-Wl,--start-group\n" ;

  for(const String &str: list )
    {
     Append(out,Path(str)/"LDpublic-opt.txt");
    }

  out << "-Wl,--end-group\n" ;

  out.close();

  if( !out )
    {
     throw std::runtime_error("'CC-opt.txt' creation error");
    }
 }

/* Main() */

int Main()
 {
  std::cout << "LightForge MakeCCopt 1.00" << std::endl ;
  std::cout << "Copyright (c) 2023 Sergey Strukov. All rights reserved." << std::endl << std::endl ;

  FullBaseList bases;

  bases.buildLDopt();

  return 0;
 }

} // namespace App

/* main() */

int main()
 {
  try
    {
     return App::Main();
    }
  catch(std::exception &ex)
    {
     std::cout << std::endl << "Exception: " << ex.what() << std::endl ;

     return 1;
    }
  catch(...)
    {
     std::cout << std::endl << "Unknown exception" << std::endl ;

     return 1;
    }
 }



