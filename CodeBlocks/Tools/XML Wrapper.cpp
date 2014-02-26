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

#include "XML Wrapper.h"
//------------------------------------------------------------------------------

#define en_dash "\xE2\x80\x93"
#define em_dash "\xE2\x80\x94"
//------------------------------------------------------------------------------

static int ENTITY_Compare(void* Left, void* Right){
 XML_Wrapper::ENTITY* left  = (XML_Wrapper::ENTITY*)Left;
 XML_Wrapper::ENTITY* right = (XML_Wrapper::ENTITY*)Right;

 return left->Name.Compare(right->Name);
}
//------------------------------------------------------------------------------

static int ATTRIBUTE_Compare(void* Left, void* Right){
 XML_Wrapper::ATTRIBUTE* left  = (XML_Wrapper::ATTRIBUTE*)Left;
 XML_Wrapper::ATTRIBUTE* right = (XML_Wrapper::ATTRIBUTE*)Right;

 return left->Name.Compare(right->Name);
}
//------------------------------------------------------------------------------

XML_Wrapper::ENTITY::ENTITY(const char* Name){
 this->Name += Name;

 Children  .Compare = ENTITY_Compare;
 Attributes.Compare = ATTRIBUTE_Compare;
}
//------------------------------------------------------------------------------

XML_Wrapper::ENTITY::~ENTITY(){
 ENTITY* Child = (ENTITY*)Children.First();
 while(Child){
  delete Child;
  Child = (ENTITY*)Children.Next();
 }
 Children.Clear();

 ATTRIBUTE* Attribute = (ATTRIBUTE*)Attributes.First();
 while(Attribute){
  delete Attribute;
  Attribute = (ATTRIBUTE*)Attributes.Next();
 }
 Attributes.Clear();
}
//------------------------------------------------------------------------------

XML_Wrapper::ATTRIBUTE::ATTRIBUTE(const char* Name, const char* Value){
 this->Name  += Name;
 this->Value += Value;
}
//------------------------------------------------------------------------------

XML_Wrapper::ATTRIBUTE::~ATTRIBUTE(){
}
//------------------------------------------------------------------------------

XML_Wrapper::NESTING::NESTING(const char* EntityName){
 Next   = 0;
 Entity = new ENTITY(EntityName);
}
//------------------------------------------------------------------------------

XML_Wrapper::NESTING::~NESTING(){
 // This happens when End() is called.
 // Entity deleted later.
}
//------------------------------------------------------------------------------

XML_Wrapper::XML_Wrapper(){
 Nesting = 0;
 Root    = 0;
}
//------------------------------------------------------------------------------

XML_Wrapper::~XML_Wrapper(){
 Clear();
}
//------------------------------------------------------------------------------

void XML_Wrapper::Clear(){
 while(Nesting)        End();
 if   (Root   ) delete Root;
 Root = 0;
}
//------------------------------------------------------------------------------

void XML_Wrapper::New(const char* Document){
 Clear();

 STRING LegalName;
 GetLegalName(Document, &LegalName);

 Nesting = new NESTING(LegalName.UTF8());
 Root    = Nesting->Entity;
}
//------------------------------------------------------------------------------

void XML_Wrapper::Begin(const char* Entity){
 if(!Nesting) return;

 STRING LegalName;
 GetLegalName(Entity, &LegalName);

 NESTING* Temp = new NESTING(LegalName.UTF8());
 Nesting->Entity->Children.Insert(Temp->Entity);

 Temp->Next = Nesting;
 Nesting    = Temp;
}
//------------------------------------------------------------------------------

void XML_Wrapper::Comment(const char* Comment){
 if(!Nesting) return;

 Nesting->Entity->Comments += "<!-- ";

 int j;
 for(j = 0; Comment[j]; j++){
  if(Comment[j] != '-' || Comment[j+1] != '-'){
   if(Comment[j] == '\n') Nesting->Entity->Comments += "\n     ";
   else                   Nesting->Entity->Comments += Comment[j];

  }else{
   if(Comment[j+2] == '-'){
    Nesting->Entity->Comments += em_dash;
    j += 2;
   }else{
    Nesting->Entity->Comments += en_dash;
    j++;
   }
  }
 }
 Nesting->Entity->Comments += " -->\n";
}
//------------------------------------------------------------------------------

void XML_Wrapper::Attribute(const char* Name, int Value){
 if(!Nesting) return;

 STRING s;
 s = Value;
 Attribute(Name, s.UTF8());
}
//------------------------------------------------------------------------------

void XML_Wrapper::Attribute(const char* Name, bool Value){
 if(!Nesting) return;

 if(Value) Attribute(Name, "1");
 else      Attribute(Name, "0");
}
//------------------------------------------------------------------------------

void XML_Wrapper::Attribute(const char* Name, double Value){
 if(!Nesting) return;

 STRING s;
 s = Value;
 Attribute(Name, s.UTF8());
}
//------------------------------------------------------------------------------

void XML_Wrapper::Attribute(const char* Name, unsigned Value){
 if(!Nesting) return;

 STRING s;
 s = "0x";
 s.AppendHex(Value, 8);
 Attribute(Name , s.UTF8());
}
//------------------------------------------------------------------------------

void XML_Wrapper::Attribute(const char* Name, const char* Value){
 if(!Nesting) return;

 STRING LegalName;
 GetLegalName(Name, &LegalName);

 ATTRIBUTE* Temp = new ATTRIBUTE(LegalName.UTF8(), Value);
 Nesting->Entity->Attributes.Insert(Temp);
}
//------------------------------------------------------------------------------

void XML_Wrapper::Attribute(const char* Name, STRING* Value){
 Attribute(Name, Value->UTF8());
}
//------------------------------------------------------------------------------

void XML_Wrapper::Content(int Value){
 if(!Nesting) return;

 STRING s;
 s = Value;
 Content(s.UTF8());
}
//------------------------------------------------------------------------------

void XML_Wrapper::Content(bool Value){
 if(!Nesting) return;

 if(Value) Content("1");
 else      Content("0");
}
//------------------------------------------------------------------------------

void XML_Wrapper::Content(double Value){
 if(!Nesting) return;

 STRING s;
 s = Value;
 Content(s.UTF8());
}
//------------------------------------------------------------------------------

void XML_Wrapper::Content(unsigned Value){
 if(!Nesting) return;

 STRING s;
 s = "0x";
 s.AppendHex(Value, 8);
 Content (s.UTF8());
}
//------------------------------------------------------------------------------

void XML_Wrapper::Content(const char* Value){
 if(!Nesting) return;
 if(!Value  ) return;

 Nesting->Entity->Content += Value;
}
//------------------------------------------------------------------------------

void XML_Wrapper::End(){
 if(!Nesting) return;

 NESTING* Temp = Nesting;
 Nesting = Nesting->Next;
 delete Temp;
}
//------------------------------------------------------------------------------

void XML_Wrapper::GetLegalName(const char* Name, STRING* LegalName){
 STRING Temp;
 Temp = Name;

 *LegalName = "";

 int j = 0;

 if(
  (Temp[0] == ':'                          ) ||
  (Temp[0] == '_'                          ) ||
  (Temp[0] >= 'A'     && Temp[0] <= 'Z'    ) ||
  (Temp[0] >= 'a'     && Temp[0] <= 'z'    ) ||
  (Temp[0] >= 0x000C0 && Temp[0] <= 0x000D6) ||
  (Temp[0] >= 0x000D8 && Temp[0] <= 0x000F6) ||
  (Temp[0] >= 0x000F8 && Temp[0] <= 0x002FF) ||
  (Temp[0] >= 0x00370 && Temp[0] <= 0x0037D) ||
  (Temp[0] >= 0x0037F && Temp[0] <= 0x01FFF) ||
  (Temp[0] >= 0x0200C && Temp[0] <= 0x0200D) ||
  (Temp[0] >= 0x02070 && Temp[0] <= 0x0218F) ||
  (Temp[0] >= 0x02C00 && Temp[0] <= 0x02FEF) ||
  (Temp[0] >= 0x03001 && Temp[0] <= 0x0D7FF) ||
  (Temp[0] >= 0x0F900 && Temp[0] <= 0x0FDCF) ||
  (Temp[0] >= 0x0FDF0 && Temp[0] <= 0x0FFFD) ||
  (Temp[0] >= 0x10000 && Temp[0] <= 0xEFFFF)
 )    *LegalName += Temp[0];
 else *LegalName += '_'    ;

 j = 1;
 while(Temp[j]){
  if(
   (Temp[j] >= '.'                          ) ||
   (Temp[j] == ':'                          ) ||
   (Temp[j] >= '-'                          ) ||
   (Temp[j] == '_'                          ) ||
   (Temp[j] >= '0'     && Temp[j] <= '9'    ) ||
   (Temp[j] >= 'A'     && Temp[j] <= 'Z'    ) ||
   (Temp[j] >= 'a'     && Temp[j] <= 'z'    ) ||
   (Temp[j] >= 0x000B7                      ) ||
   (Temp[j] >= 0x000C0 && Temp[j] <= 0x000D6) ||
   (Temp[j] >= 0x000D8 && Temp[j] <= 0x000F6) ||
   (Temp[j] >= 0x000F8 && Temp[j] <= 0x002FF) ||
   (Temp[j] >= 0x00300 && Temp[j] <= 0x0037D) ||
   (Temp[j] >= 0x0037F && Temp[j] <= 0x01FFF) ||
   (Temp[j] >= 0x0200C && Temp[j] <= 0x0200D) ||
   (Temp[j] >= 0x0203F && Temp[j] <= 0x02040) ||
   (Temp[j] >= 0x02070 && Temp[j] <= 0x0218F) ||
   (Temp[j] >= 0x02C00 && Temp[j] <= 0x02FEF) ||
   (Temp[j] >= 0x03001 && Temp[j] <= 0x0D7FF) ||
   (Temp[j] >= 0x0F900 && Temp[j] <= 0x0FDCF) ||
   (Temp[j] >= 0x0FDF0 && Temp[j] <= 0x0FFFD) ||
   (Temp[j] >= 0x10000 && Temp[j] <= 0xEFFFF)
  )    *LegalName += Temp[j];
  else *LegalName += '_'    ;
  j++;
 }
}
//------------------------------------------------------------------------------

void XML_Wrapper::GetLegalContent(STRING* Content, STRING* LegalContent){
 *LegalContent = "";

 int   j;
 char* Temp = Content->UTF8();
 for(j = 0; Temp[j]; j++){
  switch(Temp[j]){
   case '<':
    *LegalContent += "&lt";
    break;

   case '>':
    *LegalContent += "&gt";
    break;

   case '"':
    *LegalContent += "&quot";
    break;

   case '\'':
    *LegalContent += "&apos";
    break;

   case '&':
    *LegalContent += "&amp";
    break;

   default:
    *LegalContent += Temp[j];
    break;
  }
 }
}
//------------------------------------------------------------------------------

void XML_Wrapper::SaveEntity(ENTITY* Entity, unsigned Indent){
 unsigned j;

 if(Entity->Comments.Length32()){
  char*    s  = Entity->Comments.UTF8();
  unsigned i1 = 0;
  unsigned i2 = 0;

  // Comments are "\n\0" terminated
  while(s[i2]){
   if(s[i2] == '\n'){
    for(j = 0; j < Indent; j++) Buffer += ' ';
    s[i2] = 0;
    Buffer += s + i1;
    s[i2] = '\n';
    i1    = ++i2;
    Buffer += '\n';

   }else{
    i2++;
   }
  }
 }

 for(j = 0; j < Indent; j++) Buffer += ' ';
 Buffer += '<';
 Buffer += Entity->Name;

 // Add Attributes
 if(Entity->Attributes.ItemCount()){
  unsigned   EqualPos = 0;
  ATTRIBUTE* Temp     = (ATTRIBUTE*)Entity->Attributes.First();
  while(Temp){
   GetLegalContent(&Temp->Value, &Temp->LegalValue);
   j = Temp->Name.Length8();
   if(EqualPos < j) EqualPos = j;
   Temp = (ATTRIBUTE*)Entity->Attributes.Next();
  }

  Temp = (ATTRIBUTE*)Entity->Attributes.First();
  while(Temp){
   Buffer += "\n ";
   for(j = 0; j < Indent; j++) Buffer += ' ';
   Buffer += Temp->Name;

   j = Temp->Name.Length8();
   for(; j < EqualPos ; j++) Buffer += ' ';
   Buffer += " = \"";

   Buffer += Temp->LegalValue;
   Buffer += '"';
   Temp = (ATTRIBUTE*)Entity->Attributes.Next();
  }
  Buffer += '\n';
  for(j = 0; j < Indent; j++) Buffer += ' ';
 }

 // Add children and then the content
 if(Entity->Content.Length32() || Entity->Children.ItemCount()){
  Buffer += ">\n";

  if(Entity->Content.Length32()){
   GetLegalContent(&Entity->Content, &Entity->LegalContent);

   char*    s  = Entity->LegalContent.UTF8();
   unsigned i1 = 0;
   unsigned i2 = 0;

   while(true){
    if(s[i2] == '\n'){
     for(j = 0; j <= Indent; j++) Buffer += ' ';
     s[i2] = 0;
     Buffer += s + i1;
     s[i2] = '\n';
     i1    = ++i2;
     Buffer += '\n';

    }else if(s[i2] == 0){
     for(j = 0; j <= Indent; j++) Buffer += ' ';
     if(s[i1]) Buffer += s + i1;
     Buffer += '\n';
     break;

    }else{
     i2++;
    }
   }
  }

  if(Entity->Children.ItemCount()){
   ENTITY* Temp = (ENTITY*)Entity->Children.First();
   while(Temp){
    SaveEntity(Temp, Indent + 1);
    Temp = (ENTITY*)Entity->Children.Next();
   }
  }
  for(j = 0; j < Indent; j++) Buffer += ' ';
  Buffer += "</";
  Buffer += Entity->Name;
  Buffer += ">\n";

 }else{
  Buffer += "/>\n";
 }
}
//------------------------------------------------------------------------------

bool XML_Wrapper::Save(const char* Filename){
 if(!Root) return false;

 while(Nesting) End();

 FileWrapper File;
 if(!File.Open(Filename, FileWrapper::faCreate)) return false;

 Buffer += "<?"
  "xml version ""= \"1.0\" "
  "encoding "   "= \"UTF-8\" "
  "standalone " "= \"yes\" "
 "?>\n";
 SaveEntity(Root);
 File.Write(Buffer.UTF8(), Buffer.Length8());
 File.Close();

 // Clear memory
 Buffer = "";

 return true;
}
//------------------------------------------------------------------------------

bool XML_Wrapper::ReadSpace(){
 if(ReadIndex >= ReadSize) return false;

 if(
  ReadBuffer[ReadIndex] == ' '  ||
  ReadBuffer[ReadIndex] == '\t' ||
  ReadBuffer[ReadIndex] == '\r' ||
  ReadBuffer[ReadIndex] == '\n'
 ){
  ReadIndex++;
  return true;
 }
 return false;
}
//------------------------------------------------------------------------------

bool XML_Wrapper::ReadComment(){
 if(ReadIndex >= ReadSize) return false;

 if(
  ReadBuffer[ReadIndex  ] == '<' &&
  ReadBuffer[ReadIndex+1] == '!' &&
  ReadBuffer[ReadIndex+2] == '-' &&
  ReadBuffer[ReadIndex+3] == '-'
 ){
  ReadIndex += 4;

  while(ReadIndex < ReadSize){
   if(
    ReadBuffer[ReadIndex  ] == '-' &&
    ReadBuffer[ReadIndex+1] == '-' &&
    ReadBuffer[ReadIndex+2] == '>'
   ){
    ReadIndex += 3;
    return true;
   }
   ReadIndex++;
  }
 }

 return false;
}
//------------------------------------------------------------------------------

bool XML_Wrapper::ReadName(STRING* Buffer){
 *Buffer = "";

 while(ReadSpace() || ReadComment());

 while(ReadIndex < ReadSize){
  if(
   ReadBuffer[ReadIndex] <= ' ' ||
   ReadBuffer[ReadIndex] == '=' ||
   ReadBuffer[ReadIndex] == '<' ||
   ReadBuffer[ReadIndex] == '>' ||
   ReadBuffer[ReadIndex] == '?' ||
   ReadBuffer[ReadIndex] == '!' ||
   ReadBuffer[ReadIndex] == '/'
  ){
   break;
  }
  *Buffer += ReadBuffer[ReadIndex++];
 }

 return Buffer->Length8();
}
//------------------------------------------------------------------------------

bool XML_Wrapper::ReadContent(STRING* Buffer, char End){
 while(ReadIndex < ReadSize){
  if(
   ReadBuffer[ReadIndex] == '<' ||
   ReadBuffer[ReadIndex] == '>' ||
   ReadBuffer[ReadIndex] == End
  ){
   break;

  }else if(ReadBuffer[ReadIndex] == '&'){
   ReadIndex++;
   if(
    ReadBuffer[ReadIndex  ] == 'q' &&
    ReadBuffer[ReadIndex+1] == 'u' &&
    ReadBuffer[ReadIndex+2] == 'o' &&
    ReadBuffer[ReadIndex+3] == 't'
   ){
    ReadIndex += 4;
    *Buffer += '"';

   }else if(
    ReadBuffer[ReadIndex  ] == 'a' &&
    ReadBuffer[ReadIndex+1] == 'p' &&
    ReadBuffer[ReadIndex+2] == 'o' &&
    ReadBuffer[ReadIndex+3] == 's'
   ){
    ReadIndex += 4;
    *Buffer += '\'';

   }else if(
    ReadBuffer[ReadIndex  ] == 'l' &&
    ReadBuffer[ReadIndex+1] == 't'
   ){
    ReadIndex += 2;
    *Buffer += '<';

   }else if(
    ReadBuffer[ReadIndex  ] == 'g' &&
    ReadBuffer[ReadIndex+1] == 't'
   ){
    ReadIndex += 2;
    *Buffer += '>';

   }else if(
    ReadBuffer[ReadIndex  ] == 'a' &&
    ReadBuffer[ReadIndex+1] == 'm' &&
    ReadBuffer[ReadIndex+2] == 'p'
   ){
    ReadIndex += 3;
    *Buffer += '&';
   }

  }else{
   *Buffer += ReadBuffer[ReadIndex++];
  }
 }

 return Buffer->Length8();
}
//------------------------------------------------------------------------------

bool XML_Wrapper::ReadAttribute(LLRBTree* Tree){
 ATTRIBUTE* Temp = new ATTRIBUTE("", "");

 if(!ReadName(&Temp->Name)){ // No name
  delete Temp;
  return false;
 }

 while(ReadSpace() || ReadComment());

 if(ReadBuffer[ReadIndex] != '='){ // No assignment
  delete Temp;
  MessageBoxA(0, "No Assignment", "XML Error", MB_ICONERROR);
  return false;
 }
 ReadIndex++;

 while(ReadSpace() || ReadComment());

 if( // No value
  ReadBuffer[ReadIndex] != '"' &&
  ReadBuffer[ReadIndex] != '\''
 ){
  delete Temp;
  MessageBoxA(0, "No Value", "XML Error", MB_ICONERROR);
  return false;
 }

 char End = ReadBuffer[ReadIndex++];

 ReadContent(&Temp->Value, End);

 if(ReadBuffer[ReadIndex] != End){ // Open string
  delete Temp;
  MessageBoxA(0, "Open String", "XML Error", MB_ICONERROR);
  return false;
 }
 ReadIndex++;

 if(Tree->Find(Temp)){ // Duplicate entry
  delete Temp;
  MessageBoxA(0, "Duplicate Attribute", "XML Error", MB_ICONERROR);
  return false;
 }

 Tree->Insert(Temp);
 return true;
}
//------------------------------------------------------------------------------

bool XML_Wrapper::ReadHeader(){
 while(ReadSpace() || ReadComment());

 if(
  ReadBuffer[ReadIndex  ] == '<' &&
  ReadBuffer[ReadIndex+1] == '?' &&
  ReadBuffer[ReadIndex+2] == 'x' &&
  ReadBuffer[ReadIndex+3] == 'm' &&
  ReadBuffer[ReadIndex+4] == 'l'
 ){
  ReadIndex += 5;

  LLRBTree Attributes;
  Attributes.Compare = ATTRIBUTE_Compare;

  while(ReadAttribute(&Attributes));

  while(ReadSpace() || ReadComment());

  if(
   ReadBuffer[ReadIndex  ] == '?' &&
   ReadBuffer[ReadIndex+1] == '>'
  ){
   ReadIndex += 2;

   ATTRIBUTE  Key("version", "");
   ATTRIBUTE* Attribute = (ATTRIBUTE*)Attributes.Find(&Key);
   if(!Attribute){
    MessageBoxA(0, "No version", "XML Error", MB_ICONERROR);
    return false;
   }

   Key.Name  = "encoding";
   Attribute = (ATTRIBUTE*)Attributes.Find(&Key);
   if(!Attribute){
    MessageBoxA(0, "No encoding", "XML Error", MB_ICONERROR);
    return false;
   }
   if(Attribute->Value != "UTF-8"){
    MessageBoxA(0, "Encoding other than UTF-8", "XML Error", MB_ICONERROR);
    return false;
   }
   return true;
  }
 }
 MessageBoxA(0, "No end ?>", "XML Error", MB_ICONERROR);
 return false;
}
//------------------------------------------------------------------------------

XML_Wrapper::ENTITY* XML_Wrapper::ReadEntity(){
 while(ReadSpace() || ReadComment());

 if(
  ReadBuffer[ReadIndex  ] != '<' || // Not an entity tag
  ReadBuffer[ReadIndex+1] == '/'    // Closing tag
 ) return 0;
 ReadIndex++;

 STRING Buffer;
 if(!ReadName(&Buffer)){
  MessageBoxA(0, "Invalid tag", "XML Error", MB_ICONERROR);
  return 0;
 }

 ENTITY* Entity = new ENTITY(Buffer.UTF8());
 while(ReadAttribute(&Entity->Attributes));

 while(ReadSpace() || ReadComment());

 if(
  ReadBuffer[ReadIndex  ] == '/' &&
  ReadBuffer[ReadIndex+1] == '>'
 ){
  ReadIndex += 2;
  return Entity;
 }

 if(ReadBuffer[ReadIndex] != '>'){
  MessageBoxA(0, "Invalid tag", "XML Error", MB_ICONERROR);
  delete Entity;
  return 0;
 }
 ReadIndex++;

 bool FoundBody;
 while(ReadIndex < ReadSize){
  FoundBody = false;

  while(ReadSpace() || ReadComment());
  if(
   ReadBuffer[ReadIndex  ] == '<' &&
   ReadBuffer[ReadIndex+1] == '/'
  ){
   ReadIndex += 2;
   Buffer = "";
   if(!ReadName(&Buffer)){
    MessageBoxA(0, "Invalid closing tag", "XML Error", MB_ICONERROR);
    delete Entity;
    return 0;
   }
   if(ReadBuffer[ReadIndex] != '>'){
    MessageBoxA(0, "Invalid closing tag", "XML Error", MB_ICONERROR);
    delete Entity;
    return 0;
   }
   ReadIndex++;
   if(Buffer != Entity->Name){
    MessageBoxA(
     0,
     "Closing tag does not match opening tag",
     "XML Error",
     MB_ICONERROR
    );
    delete Entity;
    return 0;
   }
   return Entity;
  }

  ENTITY* Child = ReadEntity();
  if(Child){
   Entity->Children.Insert(Child);
   FoundBody = true;
  }

  Buffer = "";
  if(ReadContent(&Buffer)){
   Entity->Content += Buffer;
   FoundBody = true;
  }

  if(!FoundBody) break;
 }

 MessageBoxA(0, "No closing tag", "XML Error", MB_ICONERROR);
 delete Entity;
 return 0;
}
//------------------------------------------------------------------------------

bool XML_Wrapper::Load(const char* Filename){
 Clear();

 FileWrapper File;
 if(!File.Open(Filename, FileWrapper::faRead)) return false;

 ReadSize   = File.GetSize();
 ReadBuffer = new char[ReadSize];

 if(!ReadBuffer){
  File.Close();
  return false;
 }

 if(!File.Read(ReadBuffer, ReadSize)){
  File.Close();
  delete[] ReadBuffer;
  return   false;
 }
 File.Close();

 ReadIndex = 0;
 if(!ReadHeader()){
  MessageBoxA(0, "No header", "XML Error", MB_ICONERROR);
  delete[] ReadBuffer;
  return   false;
 }

 Root = ReadEntity();
 if(!Root){
  MessageBoxA(0, "No root entity", "XML Error", MB_ICONERROR);
  delete[] ReadBuffer;
  return   false;
 }

 // Clear memory
 delete[] ReadBuffer;
 return   true;
}
//------------------------------------------------------------------------------

XML_Wrapper::ENTITY* XML_Wrapper::FindChild(ENTITY* Entity, const char* Name){
 if(!Entity) return 0;

 STRING LegalName;
 GetLegalName(Name, &LegalName);

 ENTITY Key(LegalName.UTF8());
 return (ENTITY*)Entity->Children.Find(&Key);
}
//------------------------------------------------------------------------------

XML_Wrapper::ENTITY* XML_Wrapper::NextChild(ENTITY* Entity, const char* Name){
 if(!Entity) return 0;

 STRING LegalName;
 GetLegalName(Name, &LegalName);

 ENTITY* Result = (ENTITY*)Entity->Children.Next();
 if(Result && Result->Name == LegalName) return Result;
 return 0;
}
//------------------------------------------------------------------------------

XML_Wrapper::ATTRIBUTE* XML_Wrapper::FindAttribute(
 ENTITY* Entity, const char* Name
){
 if(!Entity) return 0;

 STRING LegalName;
 GetLegalName(Name, &LegalName);

 ATTRIBUTE Key(LegalName.UTF8(), "");
 return (ATTRIBUTE*)Entity->Attributes.Find(&Key);
}
//------------------------------------------------------------------------------

bool XML_Wrapper::ReadAttribute(
 ENTITY*     Entity,
 const char* Name,
 int*        Value
){
 ATTRIBUTE* A = FindAttribute(Entity, Name);
 if(A){
  *Value = Calc.Calculate(A->Value.UTF8());
  return true;
 }
 return false;
}
//------------------------------------------------------------------------------

bool XML_Wrapper::ReadAttribute(
 ENTITY*     Entity,
 const char* Name,
 bool*       Value
){
 int Temp;
 if(ReadAttribute(Entity, Name, &Temp)){
  *Value = Temp;
  return true;
 }
 return false;
}
//------------------------------------------------------------------------------

bool XML_Wrapper::ReadAttribute(
 ENTITY*     Entity,
 const char* Name,
 char*       Value
){
 ATTRIBUTE* A = FindAttribute(Entity, Name);
 if(A){
  strcpy(Value, A->Value.UTF8());
  return true;
 }
 return false;
}
//------------------------------------------------------------------------------

bool XML_Wrapper::ReadAttribute(
 ENTITY*     Entity,
 const char* Name,
 STRING*     Value
){
 ATTRIBUTE* A = FindAttribute(Entity, Name);
 if(A){
  *Value = A->Value;
  return true;
 }
 return false;
}
//------------------------------------------------------------------------------

bool XML_Wrapper::ReadAttribute(
 ENTITY*     Entity,
 const char* Name,
 double*     Value
){
 ATTRIBUTE* A = FindAttribute(Entity, Name);
 if(A){
  *Value = Calc.Calculate(A->Value.UTF8());
  return true;
 }
 return false;
}
//------------------------------------------------------------------------------

bool XML_Wrapper::ReadAttribute(
 ENTITY*     Entity,
 const char* Name,
 unsigned*   Value
){
 ATTRIBUTE* A = FindAttribute(Entity, Name);
 if(A){
  *Value = Calc.Calculate(A->Value.UTF8());
  return true;
 }
 return false;
}
//------------------------------------------------------------------------------
