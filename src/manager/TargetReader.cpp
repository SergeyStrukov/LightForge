/* TargetReader.cpp */
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
#include <algorithm>

#include "TargetReader.h"

namespace App {

/* struct TargetInfo */

void TargetInfo::checkBases(const std::vector<String> &baseList) const
 {
  std::vector<String> list(baseList);

  std::sort(list.begin(),list.end());

  for(const BaseSpec &obj : base )
    {
     if( !obj.proj.empty() )
       {
        if( !std::binary_search(list.begin(),list.end(),obj.proj) )
          {
           std::cout << "Target " << name << " has bad base project" << obj.proj << std::endl ;

           throw std::runtime_error("target base is not in the project base list");
          }
       }
    }
 }

/* class TargetReader */

void TargetReader::apply(String &&key,std::vector<String> &&list,const Path &fileName,TextPos pos)
 {
  if( key=="OUT" )
    {
     if( outFlag )
       {
        std::cout << "File " << fileName << pos << " : OUT key duplication" << std::endl ;

        throw std::runtime_error("file processing error");
       }

     if( list.size()!=1 )
       {
        std::cout << "File " << fileName << pos << " : bad number of OUT strings" << std::endl ;

        throw std::runtime_error("file processing error");
       }

     outName=std::move(list[0]);
     outFlag=true;
    }
  else if( key=="SRC" )
    {
     if( srcFlag )
       {
        std::cout << "File " << fileName << pos << " : SRC key duplication" << std::endl ;

        throw std::runtime_error("file processing error");
       }

     src=std::move(list);
     srcFlag=true;
    }
  else if( key=="INC" )
    {
     if( incFlag )
       {
        std::cout << "File " << fileName << pos << " : INC key duplication" << std::endl ;

        throw std::runtime_error("file processing error");
       }

     inc=std::move(list);
     incFlag=true;
    }
  else if( key=="INC_PRIVATE" )
    {
     if( incPrivateFlag )
       {
        std::cout << "File " << fileName << pos << " : INC_PRIVATE key duplication" << std::endl ;

        throw std::runtime_error("file processing error");
       }

     incPrivate=std::move(list);
     incPrivateFlag=true;
    }
  else
    {
     std::cout << "File " << fileName << pos << " : unknown key '" << key << "'" << std::endl ;

     throw std::runtime_error("file processing error");
    }
 }

TargetReader::TargetReader(Path &&path_,const Path &fileName)
 {
  path=std::move(path_);

  // 1

  FileReader inp(fileName);

  Token t1=inp.nextValuable();

  if( t1.kind!=TokenName )
    {
     std::cout << "File " << fileName << t1.pos << " : name is expected" << std::endl ;

     throw std::runtime_error("file processing error");
    }

  if( t1.text=="exe" )
    {
     kind=TargetExe;
    }
  else if( t1.text=="lib" )
    {
     kind=TargetLib;
    }
  else if( t1.text=="pregen" )
    {
     kind=TargetPregen;
    }
  else
    {
     std::cout << "File " << fileName << t1.pos << " : unknown target kind" << std::endl ;

     throw std::runtime_error("file processing error");
    }

  Token t2=inp.nextValuable();

  if( t2.kind!=TokenName )
    {
     std::cout << "File " << fileName << t2.pos << " : name is expected" << std::endl ;

     throw std::runtime_error("file processing error");
    }

  name=std::move(t2.text);

  Token t3=inp.nextValuable();

  if( t3.kind!=TokenPunct || t3.text!=":" )
    {
     std::cout << "File " << fileName << t3.pos << " : ':' is expected" << std::endl ;

     throw std::runtime_error("file processing error");
    }

  base.reserve(100);

  bool flag=true;

  for(;;)
    {
     if( flag )
       {
        t1=inp.nextValuable();

        if( t1.kind!=TokenName )
          {
           std::cout << "File " << fileName << t1.pos << " : name is expected" << std::endl ;

           throw std::runtime_error("file processing error");
          }
       }

     t2=inp.nextValuable();

     if( t2.kind==TokenPunct )
       {
        if( t2.text=="." )
          {
           t3=inp.nextValuable();

           if( t3.kind!=TokenName )
             {
              std::cout << "File " << fileName << t3.pos << " : name is expected" << std::endl ;

              throw std::runtime_error("file processing error");
             }

           base.emplace_back(std::move(t1.text),std::move(t3.text));

           flag=true;
          }
        else if( t2.text=="=" )
          {
           break;
          }
        else
          {
           std::cout << "File " << fileName << t2.pos << " : '.' or '=' is expected" << std::endl ;

           throw std::runtime_error("file processing error");
          }
       }
     else if( t2.kind==TokenName )
       {
        base.emplace_back(std::move(t1.text));

        t1=std::move(t2);

        flag=false;
       }
     else if( t2.kind==TokenNull )
       {
        std::cout << "File " << fileName << t2.pos << " : unexpected end of content" << std::endl ;

        throw std::runtime_error("file processing error");
       }
    }

  // 2

  std::vector<String> list;

  list.reserve(100);

  Token s1=inp.nextString();

  for(;;)
    {
     Token s2=inp.nextString();

     if( s2.kind==TokenNull )
       {
        if( s1.kind!=TokenNull )
          {
           list.emplace_back(std::move(s1.text));
           apply(std::move(t1.text),std::move(list),fileName,t1.pos);
          }

        break;
       }
     else if( s2.text=="=" )
       {
        apply(std::move(t1.text),std::move(list),fileName,t1.pos);
        t1=std::move(s1);
        list={};
        list.reserve(100);
        s1=inp.nextString();
       }
     else
       {
        list.emplace_back(std::move(s1.text));
        s1=std::move(s2);
       }
    }

  if( !outFlag )
    {
     std::cout << "File " << fileName << " : OUT is not defined" << std::endl ;

     throw std::runtime_error("file processing error");
    }

  if( !srcFlag )
    {
     std::cout << "File " << fileName << " : SRC is not defined" << std::endl ;

     throw std::runtime_error("file processing error");
    }
 }

TargetReader::~TargetReader()
 {
 }

/* class TargetListReader */

TargetListReader::TargetListReader(const Path &projRoot)
 {
  list.reserve(1000);

  DirTree tree(projRoot);

  for(const auto &entry : tree )
    {
     const Path &path=entry.path();

     Path fileName=path/"TARGET";

     if( isRegularFile(fileName) )
       {
        list.emplace_back(Relative(path,projRoot),fileName);
       }
    }

  size_t len=list.size();

  if( len>=2 )
    {
     const TargetReader *ptr=list.data();

     std::vector<const TargetInfo *> temp(len);

     for(size_t i=0; i<len ;i++)
       {
        temp[i]=&ptr[i].getInfo();
       }

     std::sort(temp.begin(),temp.end(), [] (const TargetInfo *a,const TargetInfo *b) { return a->name<b->name; } );

     for(size_t i=0; i<len-1 ;i++)
       {
        if( temp[i]->name==temp[i+1]->name )
          {
           std::cout << "Target name " << temp[i]->name << " duplication" << std::endl ;
           std::cout << "Path1 " << temp[i]->path << std::endl ;
           std::cout << "Path2 " << temp[i+1]->path << std::endl ;

           throw std::runtime_error("target name duplication");
          }
       }
    }

  for(const TargetReader &obj: list )
    {
     targetNameSet.insert(obj.getInfo().name);
    }
 }

TargetListReader::~TargetListReader()
 {
 }

void TargetListReader::checkBases(const std::vector<String> &baseList) const
 {
  for(const TargetReader &obj : list )
    {
     obj.getInfo().checkBases(baseList);
    }
 }

} // namespace App
