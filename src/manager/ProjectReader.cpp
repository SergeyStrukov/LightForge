/* ProjectReader.cpp */
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

#include "ProjectReader.h"

namespace App {

/* class ProjectReader */

ProjectReader::ProjectReader(const String &fileName)
 {
  base.reserve(BigReserve);

  FileReader inp(fileName);

  Token t1=inp.nextValuable();
  Token t2=inp.nextValuable();

  if( t1.kind!=TokenName )
    {
     std::cout << "File " << fileName << t1.pos << " : name is expected" << std::endl ;

     throw std::runtime_error("file processing error");
    }

  if( t2.kind!=TokenPunct || t2.text!=":" )
    {
     std::cout << "File " << fileName << t2.pos << " : ':' is expected" << std::endl ;

     throw std::runtime_error("file processing error");
    }

  name=std::move(t1.text);

  for(;;)
    {
     Token t=inp.nextValuable();

     if( !t ) break;

     if( t.kind!=TokenName )
       {
        std::cout << "File " << fileName << t.pos << " : name is expected" << std::endl ;

        throw std::runtime_error("file processing error");
       }

     base.emplace_back(std::move(t.text));
    }
 }

ProjectReader::~ProjectReader()
 {
 }

/* class ProjectListReader */

ProjectListReader::Rec::Rec(const std::vector<String> &base_)
 : base(base_.begin(),base_.end())
 {
 }

bool ProjectListReader::Rec::findBaseName(const String &projName) const
 {
  return base.find(projName)!=base.end();
 }

void ProjectListReader::warnBaseMissing(const String &projName)
 {
  for(const auto &rec : list )
    {
     if( rec.second.findBaseName(projName) )
       {
        std::cout << "Project " << rec.first << " lost the base " << projName << std::endl ;
       }
    }
 }

void ProjectListReader::append(String &&projName,const std::vector<String> &base)
 {
  bool ok=list.insert(std::make_pair(std::move(projName),base)).second;

  if( !ok )
    {
     throw std::runtime_error("cannot add project");
    }
 }

bool ProjectListReader::findProjName(const String &projName) const
 {
  return list.find(projName)!=list.end();
 }

ProjectListReader::ProjectListReader(const String &fileName)
 {
  if( !PathExists(fileName) ) return;

  FileReader inp(fileName);

  // 1

  Token t1=inp.nextValuable();

  if( !t1 ) return;

  if( t1.kind!=TokenName )
    {
     std::cout << "File " << fileName << t1.pos << " : name is expected" << std::endl ;

     throw std::runtime_error("file processing error");
    }

  Token t2=inp.nextValuable();

  if( t2.kind!=TokenPunct || t2.text!=":" )
    {
     std::cout << "File " << fileName << t2.pos << " : ':' is expected" << std::endl ;

     throw std::runtime_error("file processing error");
    }

  // 2

  std::vector<String> base;

  base.reserve(BaseReserve);

  Token t3=inp.nextValuable();

  if( !t3 )
    {
     append(std::move(t1.text),std::move(base));

     return;
    }

  if( t3.kind!=TokenName )
    {
     std::cout << "File " << fileName << t3.pos << " : name is expected" << std::endl ;

     throw std::runtime_error("file processing error");
    }

  for(;;)
    {
     Token t=inp.nextValuable();

     if( !t ) break;

     if( t.kind==TokenName )
       {
        base.emplace_back(std::move(t3.text));
        t3=std::move(t);
       }
     else
       {
        if( t.kind!=TokenPunct || t.text!=":" )
          {
           std::cout << "File " << fileName << t.pos << " : ':' is expected" << std::endl ;

           throw std::runtime_error("file processing error");
          }

        append(std::move(t1.text),std::move(base));

        t1=std::move(t3);
        base={};
        base.reserve(BaseReserve);

        t3=inp.nextValuable();

        if( !t3 )
          {
           append(std::move(t1.text),std::move(base));

           return;
          }

        if( t3.kind!=TokenName )
          {
           std::cout << "File " << fileName << t3.pos << " : name is expected" << std::endl ;

           throw std::runtime_error("file processing error");
          }
       }
    }

  base.emplace_back(std::move(t3.text));
  append(std::move(t1.text),std::move(base));
 }

ProjectListReader::~ProjectListReader()
 {
 }

void ProjectListReader::addProject(const String &projName,const std::vector<String> &baseList)
 {
  if( findProjName(projName) )
    {
     std::cout << "Project " << projName << " is already installed" << std::endl ;

     throw std::runtime_error("cannot install project");
    }

  for(const String &base : baseList )
    if( !findProjName(base) )
      {
       std::cout << "There is no base project " << base << std::endl ;

       throw std::runtime_error("cannot install project");
      }

  std::set<String> deepSet(baseList.begin(),baseList.end());
  std::vector<String> deep(baseList.begin(),baseList.end());

  deep.reserve(BigReserve);

  for(size_t ind=0; ind<deep.size() ;ind++)
    {
     String base=deep[ind];

     if( base==projName )
       {
        std::cout << "Project makes a circle" << std::endl ;

        throw std::runtime_error("cyclic project dependencies found");
       }

     auto ptr=list.find(base);

     if( ptr!=list.end() )
       {
        for(const String &obj : ptr->second.base )
          {
           if( deepSet.insert(obj).second )
             {
              deep.push_back(obj);
             }
           else
             {
              std::cout << "Project " << obj << " makes a circle" << std::endl ;

              throw std::runtime_error("cyclic project dependencies found");
             }
          }
       }
     else
       {
        std::cout << "There is no deep base project " << base << std::endl ;
       }
    }

  append(String(projName),baseList);
 }

void ProjectListReader::delProject(const String &projName)
 {
  auto ptr=list.find(projName);

  if( ptr!=list.end() )
    {
     list.erase(ptr);

     warnBaseMissing(projName);

     return;
    }

  std::cout << "There is no project " << projName << std::endl ;
 }

void ProjectListReader::Rec::print(std::ostream &out) const
 {
  for(const String &x : base )
    {
     out << x << " " ;
    }
 }

void ProjectListReader::save(const String &fileName) const
 {
  std::ofstream out(fileName);

  out.exceptions(std::ofstream::badbit);

  for(const auto &rec : list )
    {
     out << rec.first << ": " << rec.second << "\n" ;
    }

  out.close();

  if( !out )
    {
     throw std::runtime_error("saving 'PROJECTS' error");
    }
 }

} // namespace App
