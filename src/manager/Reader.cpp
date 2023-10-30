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
#include <vector>

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

FileReader::FileReader(const String &fileName)
 : inp(fileName.c_str())
 {
  inp.exceptions(std::ifstream::badbit);
 }

FileReader::~FileReader()
 {
 }

Token FileReader::next(Char beg,TokenKind kind)
 {
  Token token(kind,pos,beg.ch);

  skip(1);

  pos.update(beg.ch);

  return token;
 }

void FileReader::skipSpace()
 {
  for(;;)
    {
     Char beg=peek(0);

     if( beg.kind!=CharSpace ) break;

     skip(1);

     pos.update(beg.ch);
    }
 }

String FileReader::skipName(char first)
 {
  Builder builder(first);

  for(;;)
    {
     Char beg=peek(0);

     if( beg.kind!=CharLetter && beg.kind!=CharDigit ) break;

     skip(1);

     pos.update(beg.ch);

     builder.add(beg.ch);
    }

  return builder.complete();
 }

Token FileReader::nextEOF()
 {
  return Token(TokenNull,pos);
 }

Token FileReader::nextSpace(Char beg)
 {
  Token token(TokenSpace,pos);

  skip(1);

  pos.update(beg.ch);

  skipSpace();

  return token;
 }

Token FileReader::nextLetter(Char beg)
 {
  Token token(TokenName,pos);

  skip(1);

  pos.update(beg.ch);

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

Token FileReader::nextOther(Char beg) // TODO
 {
  if( beg.ch=='/' )
    {
     // comments
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

} // namespace App



