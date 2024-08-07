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
#include <cstdio>

#include "FullBaseList.h"

namespace App {

/* functions */

bool CompareFiles(const char *fileName1,const char *fileName2)
 {
  std::ifstream inp1(fileName1,std::ios::binary);
  std::ifstream inp2(fileName2,std::ios::binary);

  if( !inp1.is_open() || !inp2.is_open() ) return false;

  inp1.exceptions(std::ifstream::badbit);
  inp2.exceptions(std::ifstream::badbit);

  for(;;)
    {
     int ch1=inp1.get();
     int ch2=inp2.get();

     if( ch1==ch2 )
       {
        if( ch1==std::char_traits<char>::eof() ) return true;
       }
     else
       {
        return false;
       }
    }
 }

void UpdateFile(const char *oldFileName,const char *newFileName)
 {
  if( CompareFiles(oldFileName,newFileName) )
    {
     std::filesystem::remove(newFileName);
    }
  else
    {
     std::filesystem::rename(newFileName,oldFileName);
    }
 }

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

String FullBaseList::Cat(const std::vector<String> &list)
 {
  std::ostringstream out;

  for(const String &str: list )
    {
     Append(out,Path(str)/"LDpublic-opt.txt");
    }

  return out.str();
 }

void FullBaseList::AppendSlash(std::ostream &out,String text)
 {
  auto append = [&out] (std::string_view line) { out << line << " \\\n" ; } ;

  std::string_view view(text);

  for(size_t pos=0,len=view.length(); pos<len ;)
    {
     size_t eolpos=view.find('\n',pos);

     if( eolpos==view.npos )
       {
        append(view.substr(pos));
        break;
       }
     else
       {
        append(view.substr(pos,eolpos-pos));

        pos=eolpos+1;
       }
    }
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
  std::ofstream out("CC-opt.txt.new");

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

  UpdateFile("CC-opt.txt","CC-opt.txt.new");
 }

void FullBaseList::buildLDopt()
 {
  String text=Cat(list);

  // LD-opt
  {
   std::ofstream out("LD-opt.txt.new");

   out << "-Wl,--start-group\n" ;
   out << text ;
   out << "-Wl,--end-group\n" ;

   out.close();

   if( !out )
     {
      throw std::runtime_error("'LD-opt.txt' creation error");
     }

   UpdateFile("LD-opt.txt","LD-opt.txt.new");
  }

  // Makefile-libs
  {
   std::ofstream out("Makefile-libs.new");

   out << "LIBS = \\\n" ;

   AppendSlash(out,text) ;

   out.close();

   if( !out )
     {
      throw std::runtime_error("'Makefile-libs' creation error");
     }

   UpdateFile("Makefile-libs","Makefile-libs.new");
  }
 }

void FullBaseList::buildDeepclean()
 {
  std::ofstream out("Makefile-deepclean");

  out << "# Makefile-deepclean AUTOGENERATED, DON'T EDIT\n\n" ;

  out << "deepclean:\n" ;

  for(const String &str: list )
    {
     out << "\t$(MAKE) -C " << str << " clean\n" ;
    }

  out.close();

  if( !out )
    {
     throw std::runtime_error("'Makefile-deepclean' creation error");
    }
 }

} // namespace App
