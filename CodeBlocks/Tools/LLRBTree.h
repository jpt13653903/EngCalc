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

#ifndef LLRBTree_h
#define LLRBTree_h
//------------------------------------------------------------------------------

#include <stdlib.h>
#include <stddef.h>
//------------------------------------------------------------------------------

// Should return:
// -1 if Left key < Right key
//  0 if Left key = Right key
//  1 if Left key > Right key
// For find and remove, Left is the key and Right the node
typedef int (*LLRBTree_Compare)(void* Left, void* Right);
//------------------------------------------------------------------------------

/**
The data is stored in a Left-Leaning Red-Black Tree, as described by
Robert Sedgewick, Department of Computer Science, Princeton University,
Princeton, NJ 08544
*/
class LLRBTree{
 private:
  struct Node{
   Node* Left;
   Node* Right;
   bool  Colour; // true => Red
   void* Data;

   // Double linked list maintained for quick Next() Prev() calls
   Node* Next;
   Node* Prev;

   Node(void* Data, Node* Next, Node* Prev);
  ~Node();
  };
  Node* Root;
  Node* CurrentNode;

  Node* TempNext; // Used while inserting to find the correct place
  Node* TempPrev; // in the linked list to place the new item

  unsigned TheItemCount;

  inline bool  IsRed(Node* N);
  inline Node* FixUp(Node* N);

  // Insert functions
  Node* Insert     (Node* N, void* Data);
  Node* RotateLeft (Node* N);
  Node* RotateRight(Node* N);
  void  FlipColours(Node* N);

  // Remove functions
  bool  PointerExist;
  int   RemoveCompare(void* Left, void* Right);
  Node* Remove       (Node* N, void* Key);
  Node* MoveRedLeft  (Node* N);
  Node* MoveRedRight (Node* N);
  Node* RemoveMin    (Node* N);

  // Find function
  Node* Find(Node* N, void* Key);
//------------------------------------------------------------------------------

 public:
   LLRBTree();
  ~LLRBTree();

  void  Insert(void* Data); // Adds "Data" to the tree. Duplicates are sorted
                            // by pointer.
  void  Remove(void* Key ); // Removes the item with the same key as "Key"
                            // In the case of duplicates: if Key is in the tree
                            // under the same pointer, removes that one,
                            // otherwise removes the first duplicate found.
  void* Find  (void* Key ); // Returns the item with the same key as "Key",
                            // null if the item is not found.  Returns the
                            // first of the list of duplicates.  Also updates
                            // "Current"
  void* Before(void* Key ); // Returns the item just before "Key" and
                            // updates "Current"
  void* After (void* Key ); // Returns the item just after "Key" and
                            // updates "Current"
  void* First(); // Returns the item with the smallest key and updates "Current"
  void* Last (); // Returns the item with the largest key and updates "Current"
  void* Next (); // Returns the next item and updates "Current";
                 // null if "Current" is the last item (or null)
  void* Previous(); // Returns the previous item and updates "Current";
                    // null if "Current" is the first item (or null)
  void* Current(); // Returns the current item, null if "Current" is null
  void* RootItem(); // Returns the item at the root of the tree, which is
                    // approximately the median, and updates "Current"
  void  Clear(); // Clears the data;

  unsigned ItemCount();

  LLRBTree_Compare Compare;
};
//------------------------------------------------------------------------------

#endif
//------------------------------------------------------------------------------
