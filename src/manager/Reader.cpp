/* Reader.cpp */
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

#include "Reader.h"

namespace App {

/* class CharTable */

void CharTable::set(const char *str,CharKind kind)
 {
  for(; char ch=*str ;++str)
    {
     table[Index(ch)]=kind;
    }
 }

CharTable::CharTable()
 {
  for(CharKind &kind : table ) kind=CharOther;

  set(" \t\f\v\r\n",CharSpace);
  set("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_",CharLetter);
  set("0123456789",CharDigit);
  set(":.=",CharPunct);
 }

const CharTable CharTable::Object;

/* enum TokenKind */

const char * GetTextDesc(TokenKind kind)
 {
  switch( kind )
    {
     case TokenName         : return "Name";
     case TokenPunct        : return "Punct";
     case TokenSpace        : return "Space";
     case TokenShortComment : return "ShortComment";
     case TokenLongComment  : return "LongComment";
     case TokenError        : return "Error";

     default: // case TokenNull
       return "Null";
    }
 }

/* class FileReader::Builder */

class FileReader::Builder
 {
   std::vector<char> buf;

  public:

   explicit Builder(char ch)
    {
     buf.reserve(1000);
     add(ch);
    }

   ~Builder() {}

   void add(char ch) { buf.push_back(ch); }

   String complete() { return String(buf.begin(),buf.end()); }
 };

/* class FileReader */

namespace {

template <class T>
void Copy(T *dst,const T *src,unsigned len)
 {
  for(; len ;len--,dst++,src++) *dst=*src;
 }

} // local namespace

void FileReader::Char::set(char ch_)
 {
  ch=ch_;
  kind=CharTable::Object(ch_);
 }

void FileReader::Char::setEOF()
 {
  ch=0;
  kind=CharEOF;
 }

void FileReader::read(Char *dst,unsigned count)
 {
  for(; count ;count--,dst++)
    {
     int sym=inp.get();

     if( sym==std::char_traits<char>::eof() )
       {
        dst->setEOF();
       }
     else
       {
        dst->set((char)sym);
       }
    }
 }

auto FileReader::peek(unsigned ind) -> Char
 {
  if( ind>=Len )
    {
     throw std::runtime_error("too big index");
    }

  if( ind>=len )
    {
     if( ind>=Len-off )
       {
        Copy(buf,buf+off,len);
        off=0;
       }

     unsigned t=off+len;
     unsigned s=Len-t;

     read(buf+t,s);

     len+=s;
    }

  return buf[off+ind];
 }

void FileReader::skip(unsigned delta)
 {
  if( delta<len )
    {
     off+=delta;
     len-=delta;
    }
  else
    {
     delta-=len;

     off=0;
     len=0;

     if( delta )
       {
        throw std::runtime_error("too big skip");
       }
    }
 }

void FileReader::move(Char beg)
 {
  skip(1);

  pos.update(beg.ch);
 }

void FileReader::move()
 {
  move(peek(0));
 }

void FileReader::move(unsigned count)
 {
  for(; count ;count--) move();
 }

FileReader::FileReader(const String &fileName_)
 : fileName(fileName_),
   inp(fileName.c_str())
 {
  if( !inp.is_open() )
    {
     std::cout << "Cannot open file " << fileName << std::endl ;

     throw std::runtime_error("no such file");
    }

  inp.exceptions(std::ifstream::badbit);
 }

FileReader::~FileReader()
 {
 }

Token FileReader::next(Char beg,TokenKind kind)
 {
  Token token(kind,pos,beg.ch);

  move(beg);

  return token;
 }

void FileReader::skipSpace()
 {
  for(;;)
    {
     Char beg=peek(0);

     if( beg.kind!=CharSpace ) break;

     move(beg);
    }
 }

String FileReader::skipName(char first)
 {
  Builder builder(first);

  for(;;)
    {
     Char beg=peek(0);

     if( beg.kind!=CharLetter && beg.kind!=CharDigit ) break;

     move(beg);

     builder.add(beg.ch);
    }

  return builder.complete();
 }

void FileReader::skipLongComment()
 {
  TextPos startpos=pos;

  move(2);

  for(;;)
    {
     Char beg=peek(0);

     if( beg.kind==CharEOF )
       {
        std::cout << "File " << fileName << startpos << " : long comment is not closed" << std::endl ;

        throw std::runtime_error("file processing error");

        break;
       }

     if( beg.ch=='*' )
       {
        Char sec=peek(1);

        if( sec.ch=='/' )
          {
           move(beg);
           move(sec);
           break;
          }
        else
          {
           move(beg);
          }
       }
     else
       {
        move(beg);
       }
    }
 }

void FileReader::skipShortComment()
 {
  move(2);

  for(;;)
    {
     Char beg=peek(0);

     if( beg.kind==CharEOF ) break;

     move(beg);

     if( beg.ch=='\n' ) break;
    }
 }

Token FileReader::nextEOF()
 {
  return Token(TokenNull,pos);
 }

Token FileReader::nextSpace(Char beg)
 {
  Token token(TokenSpace,pos);

  move(beg);

  skipSpace();

  return token;
 }

Token FileReader::nextLetter(Char beg)
 {
  Token token(TokenName,pos);

  move(beg);

  token.text=skipName(beg.ch);

  return token;
 }

Token FileReader::nextDigit(Char beg)
 {
  return nextOther(beg);
 }

Token FileReader::nextPunct(Char beg)
 {
  return next(beg,TokenPunct);
 }

Token FileReader::nextLongComment()
 {
  Token token(TokenLongComment,pos);

  skipLongComment();

  return token;
 }

Token FileReader::nextShortComment()
 {
  Token token(TokenShortComment,pos);

  skipShortComment();

  return token;
 }

Token FileReader::nextOther(Char beg)
 {
  if( beg.ch=='/' )
    {
     Char next=peek(1);

     if( next.ch=='*' ) return nextLongComment();

     if( next.ch=='/' ) return nextShortComment();
    }

  return next(beg,TokenError);
 }

Token FileReader::next()
 {
  Char beg=peek(0);

  switch( beg.kind )
    {
     case CharEOF    : return nextEOF();
     case CharSpace  : return nextSpace(beg);
     case CharLetter : return nextLetter(beg);
     case CharDigit  : return nextDigit(beg);
     case CharPunct  : return nextPunct(beg);

     default: // case CharOther
       return nextOther(beg);
    }
 }

Token FileReader::nextValuable()
 {
  for(;;)
    {
     Token ret=next();

     if( ret.kind==TokenError )
       {
        std::cout << "File " << fileName << ret.pos << " : forbidden symbol" << std::endl ;

        throw std::runtime_error("file processing error");
       }

     if( hasValue(ret.kind) ) return ret;
    }
 }

/* class ProjectReader */

ProjectReader::ProjectReader(const String &fileName)
 {
  base.reserve(1000);

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

bool ProjectListReader::Rec::findBaseName(const String &projName) const
 {
  auto beg=base.begin();
  auto end=base.end();

  return std::find_if(beg,end, [&] (const String &obj) { return obj==projName; } ) != end ;
 }

void ProjectListReader::warnBaseMissing(const String &projName)
 {
  for(const auto &rec : list )
    {
     if( rec.findBaseName(projName) )
       {
        std::cout << "Project " << rec.name << " lost the base " << projName << std::endl ;
       }
    }
 }

void ProjectListReader::append(String &&projName,std::vector<String> &&base)
 {
  list.emplace_back(std::move(projName),std::move(base));
 }

bool ProjectListReader::findProjName(const String &projName) const
 {
  auto beg=list.begin();
  auto end=list.end();

  return std::find_if(beg,end, [&] (const Rec &obj) { return obj.name==projName; } ) != end ;
 }

ProjectListReader::ProjectListReader(const String &fileName)
 {
  list.reserve(1000);

  FileReader inp(fileName);

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

  std::vector<String> base;

  base.reserve(100);

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
        base.reserve(100);
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

  append(String(projName),std::vector<String>(baseList));
 }

void ProjectListReader::delProject(const String &projName)
 {
  for(auto ptr=list.begin(),end=list.end(); ptr!=end ;++ptr)
    {
     if( ptr->name==projName )
       {
        list.erase(ptr);

        warnBaseMissing(projName);

        return;
       }
    }

  std::cout << "There is no project " << projName << std::endl ;
 }

void ProjectListReader::Rec::print(std::ostream &out) const
 {
  out << name << ": " ;

  for(const String &x : base )
    {
     out << x << " " ;
    }
 }

void ProjectListReader::save(const String &fileName) const
 {
  std::ofstream out(fileName);

  out.exceptions(std::ifstream::badbit);

  for(const Rec &rec : list )
    {
     out << rec << "\n" ;
    }

  out.close();

  if( !out )
    {
     throw std::runtime_error("saving 'projects' error");
    }
 }

/* class TargetReader */

TargetReader::TargetReader(const Path &path,const Path &fileName)
 {
 }

TargetReader::~TargetReader()
 {
 }

/* class TargetListReader */

TargetListReader::TargetListReader(const Path &projRoot)
 {
  list.reserve(100);

  DirTree tree(projRoot);

  for(const auto &entry : tree )
    {
     const Path &path=entry.path();

     Path fileName=path/"TARGET";

     if( std::filesystem::is_regular_file(fileName) )
       {
        list.emplace_back(path,fileName);
       }
    }
 }

TargetListReader::~TargetListReader()
 {
 }

} // namespace App



