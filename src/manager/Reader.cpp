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

/* class FileReader */

namespace {

template <class T>
void Copy(T *dst,const T *src,unsigned len)
 {
  for(; len ;len--,dst++,src++) *dst=*src;
 }

} // local namespace

void FileReader::read(Char *dst,unsigned count)
 {
  for(; count ;count--,dst++)
    {
     int sym=inp.get();

     if( sym==std::char_traits<char>::eof() )
       {
        *dst={'\0',CharEOF};
       }
     else
       {
        char ch=(char)sym;

        *dst={ch,CharTable::Object(ch)};
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

     read(buf+t,Len-t);

     len+=t;
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

Token FileReader::next()
 {
 }

} // namespace App



