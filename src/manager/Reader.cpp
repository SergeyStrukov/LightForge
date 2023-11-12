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

/* struct Char */

void Char::set(char ch_)
 {
  ch=ch_;
  kind=CharTable::Object(ch_);
 }

void Char::setEOF()
 {
  ch=0;
  kind=CharEOF;
 }

/* class ScanFile */

void ScanFile::read(Char *dst,unsigned count)
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

ScanFile::ScanFile(const String &fileName_)
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

ScanFile::~ScanFile()
 {
 }

Char ScanFile::peek(unsigned ind)
 {
  if( ind>=Len )
    {
     throw std::runtime_error("too big index");
    }

  if( ind>=len )
    {
     if( ind>=Len-off )
       {
        ForwardCopy(buf,buf+off,len);
        off=0;
       }

     unsigned t=off+len;
     unsigned s=Len-t;

     read(buf+t,s);

     len+=s;
    }

  return buf[off+ind];
 }

void ScanFile::skip(unsigned delta)
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

void ScanFile::move(Char beg)
 {
  skip(1);

  pos.update(beg.ch);
 }

void ScanFile::move()
 {
  move(peek(0));
 }

void ScanFile::move(unsigned count)
 {
  for(; count ;count--) move();
 }

/* class FileReader::Builder */

class FileReader::Builder
 {
   std::vector<char> buf;

  public:

   Builder() { buf.reserve(1000); }

   explicit Builder(char ch) : Builder() { add(ch); }

   ~Builder() {}

   void add(char ch) { buf.push_back(ch); }

   String complete() { return String(buf.begin(),buf.end()); }
 };

/* class FileReader */

Token FileReader::next(Char beg,TokenKind kind)
 {
  Token token(kind,getPos(),beg.ch);

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
  TextPos startPos=getPos();

  move(2);

  for(;;)
    {
     Char beg=peek(0);

     if( beg.kind==CharEOF )
       {
        std::cout << "File " << getFileName() << startPos << " : long comment is not closed" << std::endl ;

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
  return Token(TokenNull,getPos());
 }

Token FileReader::nextSpace(Char beg)
 {
  Token token(TokenSpace,getPos());

  move(beg);

  skipSpace();

  return token;
 }

Token FileReader::nextLetter(Char beg)
 {
  Token token(TokenName,getPos());

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
  Token token(TokenLongComment,getPos());

  skipLongComment();

  return token;
 }

Token FileReader::nextShortComment()
 {
  Token token(TokenShortComment,getPos());

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

FileReader::FileReader(const String &fileName)
 : ScanFile(fileName)
 {
 }

FileReader::~FileReader()
 {
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
        std::cout << "File " << getFileName() << ret.pos << " : forbidden symbol" << std::endl ;

        throw std::runtime_error("file processing error");
       }

     if( hasValue(ret.kind) ) return ret;
    }
 }

Token FileReader::nextString()
 {
  skipSpace();

  Token ret(TokenName,getPos());
  Builder builder;

  for(;;)
    {
     Char beg=peek(0);

     if( beg.kind==CharEOF || beg.kind==CharSpace ) break;

     move(beg);

     builder.add(beg.ch);
    }

  ret.text=builder.complete();

  if( ret.text.size()==0 ) ret.kind=TokenNull;

  return ret;
 }

} // namespace App



