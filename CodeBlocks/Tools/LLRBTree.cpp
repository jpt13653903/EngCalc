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

#include "LLRBTree.h"
//------------------------------------------------------------------------------

static int DefaultCompare(void* Left, void* Right){
 if(Left < Right) return -1;
 if(Left > Right) return  1;
 return 0;
}
//------------------------------------------------------------------------------

LLRBTree::Node::Node(void* Data, Node* Next, Node* Prev){
 Left = Right = 0;
 Colour       = true;
 this->Data   = Data;
 this->Next   = Next;
 this->Prev   = Prev;

 if(Next) Next->Prev = this;
 if(Prev) Prev->Next = this;
}
//------------------------------------------------------------------------------

LLRBTree::Node::~Node(){
 if(Left ) delete Left;
 if(Right) delete Right;

 if(Next) Next->Prev = Prev;
 if(Prev) Prev->Next = Next;
}
//------------------------------------------------------------------------------

LLRBTree::LLRBTree(){
 Root         = 0;
 CurrentNode  = 0;
 TheItemCount = 0;
 Compare      = DefaultCompare;
}
//------------------------------------------------------------------------------

LLRBTree::~LLRBTree(){
 Clear();
}
//------------------------------------------------------------------------------

bool LLRBTree::IsRed(Node* N){
 if(N) return N->Colour;
 return false;
}
//------------------------------------------------------------------------------

LLRBTree::Node* LLRBTree::FixUp(Node* N){
 if(IsRed(N->Right) && !IsRed(N->Left      )) N = RotateLeft (N);
 if(IsRed(N->Left ) &&  IsRed(N->Left->Left)) N = RotateRight(N);
 if(IsRed(N->Left ) &&  IsRed(N->Right     ))     FlipColours(N);

 return N;
}
//------------------------------------------------------------------------------

LLRBTree::Node* LLRBTree::RotateLeft(Node* N){
 Node* t   = N->Right;
 N->Right  = t->Left;
 t->Left   = N;
 t->Colour = N->Colour;
 N->Colour = true;

 return t;
}
//------------------------------------------------------------------------------

LLRBTree::Node* LLRBTree::RotateRight(Node* N){
 Node* t   = N->Left;
 N->Left   = t->Right;
 t->Right  = N;
 t->Colour = N->Colour;
 N->Colour = true;

 return t;
}
//------------------------------------------------------------------------------

void LLRBTree::FlipColours(Node* N){
 N       ->Colour = !N       ->Colour;
 N->Left ->Colour = !N->Left ->Colour;
 N->Right->Colour = !N->Right->Colour;
}
//------------------------------------------------------------------------------

LLRBTree::Node* LLRBTree::Insert(Node* N, void* Data){
 if(!N){
  TheItemCount++;
  return new Node(Data, TempNext, TempPrev);
 }

 int result = Compare(Data, N->Data);
 // Sort duplicates by pointer
 if(!result) result = DefaultCompare(Data, N->Data);

 if(result < 0){
  TempNext = N;
  N->Left  = Insert(N->Left , Data);

 }else{
  TempPrev = N;
  N->Right = Insert(N->Right, Data);
 }

 return FixUp(N);
}
//------------------------------------------------------------------------------

void LLRBTree::Insert(void* Data){
 TempNext = TempPrev = 0;

 Root = Insert(Root, Data);
 Root->Colour = false;
}
//------------------------------------------------------------------------------

LLRBTree::Node* LLRBTree::MoveRedLeft(Node* N){
 FlipColours(N);

 if(IsRed(N->Right->Left)){
  N->Right = RotateRight(N->Right);
  N        = RotateLeft (N);
  FlipColours(N);
 }

 return N;
}
//------------------------------------------------------------------------------

LLRBTree::Node* LLRBTree::MoveRedRight(Node* N){
 FlipColours(N);

 if(IsRed(N->Left->Left)){
  N = RotateRight(N);
  FlipColours(N);
 }

 return N;
}
//------------------------------------------------------------------------------

int LLRBTree::RemoveCompare(void* Left, void* Right){
 int result;

 result = Compare(Left, Right);
 if(!result && PointerExist) result = DefaultCompare(Left, Right);

 return result;
}
//------------------------------------------------------------------------------

LLRBTree::Node* LLRBTree::RemoveMin(Node* N){
 if(!N->Left){
  if(CurrentNode == N) CurrentNode = 0;
  TheItemCount--;
  delete N;
  return 0;
 }

 if(!IsRed(N->Left) && !IsRed(N->Left->Left)) N = MoveRedLeft(N);
 N->Left = RemoveMin(N->Left);

 return FixUp(N);
}
//------------------------------------------------------------------------------

LLRBTree::Node* LLRBTree::Remove(Node* N, void* Key){
 Node* Temp;

 if(!N) return 0;

 if(RemoveCompare(Key, N->Data) < 0){
  if(!N->Left) return N; // Key not in tree
  if(!IsRed(N->Left) && !IsRed(N->Left->Left)) N = MoveRedLeft(N);
  N->Left = Remove(N->Left, Key);

 }else{
  if(IsRed(N->Left)) N = RotateRight(N);

  // If N->Right is null, N is always a red leaf;
  if(!RemoveCompare(Key, N->Data) && !N->Right){
   if(CurrentNode == N) CurrentNode = 0;
   TheItemCount--;
   delete N;
   return 0;
  }

  if(!N->Right) return FixUp(N); // Key not in tree

  if(!IsRed(N->Right) && !IsRed(N->Right->Left)) N = MoveRedRight(N);

  if(!RemoveCompare(Key, N->Data)){
   // Find the minimum in the right sub-tree
   Temp = N->Right;
   while(Temp && Temp->Left) Temp = Temp->Left;
   // Copy Temp's data to this location and delete Temp
   if(CurrentNode == Temp) CurrentNode = N;
   N->Data  = Temp->Data;
   N->Right = RemoveMin(N->Right);

  }else{
   N->Right = Remove(N->Right, Key);
  }
 }

 return FixUp(N);
}
//------------------------------------------------------------------------------

void LLRBTree::Remove(void* Key){
 int   result;
 Node* Temp;

 PointerExist = true; // So that the compare function compares pointers as well

 Temp = Root;
 while(Temp){
  result = RemoveCompare(Key, Temp->Data);
  if     (result < 0) Temp = Temp->Left;
  else if(result > 0) Temp = Temp->Right;
  else break;
 }
 PointerExist = Temp;

 Root = Remove(Root, Key);
 if(Root) Root->Colour = false;
}
//------------------------------------------------------------------------------

LLRBTree::Node* LLRBTree::Find(Node* N, void* Key){
 int   result;
 Node* Temp = 0;

 while(N){
  result = Compare(Key, N->Data);
  if     (result < 0) N = N->Left;
  else if(result > 0) N = N->Right;
  else{
   Temp = N;       // Store the current "best match" and
   N    = N->Left; // carry on searching the left sub-tree
  }
 }
 return Temp;
}
//------------------------------------------------------------------------------

void* LLRBTree::Find(void* Key){
 CurrentNode = Find(Root, Key);
 if(CurrentNode) return CurrentNode->Data;

 return 0;
}
//------------------------------------------------------------------------------

void* LLRBTree::Before(void* Key){
 int   result;
 Node* N;

 CurrentNode = 0;

 N = Root;
 while(N){
  result = Compare(Key, N->Data);
  if(result > 0){
   CurrentNode = N;
   N           = N->Right;
  }else{
   N           = N->Left;
  }
 }

 if(CurrentNode) return CurrentNode->Data;
 return 0;
}
//------------------------------------------------------------------------------

void* LLRBTree::After(void* Key){
 int   result;
 Node* N;

 CurrentNode = 0;

 N = Root;
 while(N){
  result = Compare(Key, N->Data);
  if(result < 0){
   CurrentNode = N;
   N           = N->Left;
  }else{
   N           = N->Right;
  }
 }

 if(CurrentNode) return CurrentNode->Data;
 return 0;
}
//------------------------------------------------------------------------------

void* LLRBTree::First(){
 if(!Root) return 0;

 CurrentNode = Root;
 while(CurrentNode && CurrentNode->Left) CurrentNode = CurrentNode->Left;

 return CurrentNode->Data;
}
//------------------------------------------------------------------------------

void* LLRBTree::Last(){
 if(!Root) return 0;

 CurrentNode = Root;
 while(CurrentNode && CurrentNode->Right) CurrentNode = CurrentNode->Right;

 return CurrentNode->Data;
}
//------------------------------------------------------------------------------

void* LLRBTree::Next(){
 if(CurrentNode) CurrentNode = CurrentNode->Next;
 if(CurrentNode) return        CurrentNode->Data;
 return 0;
}
//------------------------------------------------------------------------------

void* LLRBTree::Previous(){
 if(CurrentNode) CurrentNode = CurrentNode->Prev;
 if(CurrentNode) return        CurrentNode->Data;
 return 0;
}
//------------------------------------------------------------------------------

void* LLRBTree::Current(){
 if(CurrentNode) return CurrentNode->Data;
 return 0;
}
//------------------------------------------------------------------------------

void* LLRBTree::RootItem(){
 CurrentNode = Root;
 if(CurrentNode) return CurrentNode->Data;
 return 0;
}
//------------------------------------------------------------------------------

void LLRBTree::Clear(){
 if(Root) delete Root;
 Root         = 0;
 CurrentNode  = 0;
 TheItemCount = 0;
}
//------------------------------------------------------------------------------

unsigned LLRBTree::ItemCount(){
 return TheItemCount;
}
//------------------------------------------------------------------------------
