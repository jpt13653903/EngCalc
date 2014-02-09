//==============================================================================
// Copyright (C) John-Philip Taylor
// jpt13653903@gmail.com
//
// This file is part of a library
//
// This file is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>
//==============================================================================

// An abstraction of a subset of the XML-1.0 standard.

// The only supported encoding is UTF-8.

// Content characters "<", ">", """, "'" and "&" are automatically escaped when
// writing and resolved when reading. All other non-null characters are
// considered legal content.

// Illegal characters in names are replaced with underscores.
//------------------------------------------------------------------------------

#ifndef XML_Wrapper_h
#define XML_Wrapper_h
//------------------------------------------------------------------------------

#include "Calculator.h"
#include "FileWrapper.h"
//------------------------------------------------------------------------------

class XML_Wrapper{
 public:
  struct ATTRIBUTE{
   STRING Name;
   STRING Value;
   STRING LegalValue;

   ATTRIBUTE(const char* Name, const char* Value);
  ~ATTRIBUTE();
  };

  struct ENTITY{
   STRING Name;
   STRING Comments;
   STRING Content;
   STRING LegalContent;

   LLRBTree Children;   // Child entities, sorted by name
   LLRBTree Attributes; // Sorted by name

   ENTITY(const char* Name);
  ~ENTITY();
  };
  ENTITY* Root; // When changing externally, call Clear() first

 private:
  struct NESTING{
   ENTITY*  Entity;
   NESTING* Next;

   NESTING(const char* EntityName);
  ~NESTING();
  };
  NESTING* Nesting;

  STRING Buffer;
  void GetLegalName   (const char* Name   , STRING*  LegalName);
  void GetLegalContent(STRING    * Content, STRING*  LegalContent);
  void SaveEntity     (ENTITY    * Entity , unsigned Indent = 0);

  CALCULATOR Calc;

  unsigned ReadSize;
  unsigned ReadIndex;
  char*    ReadBuffer;

  bool    ReadSpace    ();
  bool    ReadComment  ();
  bool    ReadName     (STRING  * Buffer);
  bool    ReadContent  (STRING  * Buffer, char End = 0);
  bool    ReadAttribute(LLRBTree* Tree);
  bool    ReadHeader   ();
  ENTITY* ReadEntity   ();

 public:
  XML_Wrapper();
 ~XML_Wrapper();

 // Discard all previous data
 void Clear();

 // Discard all previous data and creates a new document,
 // with a top entity named Document
 void New(const char* Document);

 // Nests a new entity under the current one
 void Begin(const char* Entity);

 // Adds a new comment to the current entity
 void Comment(const char* Comment);

 // Adds a new attribute to the current entity
 void Attribute(const char* Name, int         Value); // Decimal
 void Attribute(const char* Name, bool        Value);
 void Attribute(const char* Name, double      Value);
 void Attribute(const char* Name, unsigned    Value); // Hexadecimal
 void Attribute(const char* Name, const char* Value);
 void Attribute(const char* Name, STRING    * Value);

 // Adds to the existing content of the current entity
 void Content(int         Value);
 void Content(bool        Value);
 void Content(double      Value);
 void Content(unsigned    Value);
 void Content(const char* Value);

 // Closes the current entity and goes up one nesting level
 // Once the top level entity is closed, no other operations are possible
 void End();

 // Closes all entities on the nesting stack and saves the document to a file
 bool Save(const char* Filename);

 // Discards all previous data and loads the file into the current document
 bool Load(const char* Filename);

 // Finds the entity with name Name (Memory managed by this class)
 ENTITY* FindChild(ENTITY* Entity, const char* Name);
 // After calling FindChild, call this to get the next duplicate
 ENTITY* NextChild(ENTITY* Entity, const char* Name);

 // Finds the attribute with name Name (Memory managed by this class)
 ATTRIBUTE* FindAttribute(ENTITY* Entity, const char* Name);

 // Use these to read attributes directly
 bool ReadAttribute(ENTITY* Entity, const char* Name, int*      Value);
 bool ReadAttribute(ENTITY* Entity, const char* Name, bool*     Value);
 bool ReadAttribute(ENTITY* Entity, const char* Name, char*     Value);
 bool ReadAttribute(ENTITY* Entity, const char* Name, STRING*   Value);
 bool ReadAttribute(ENTITY* Entity, const char* Name, double*   Value);
 bool ReadAttribute(ENTITY* Entity, const char* Name, unsigned* Value);
};
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------
