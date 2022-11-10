/*
 * Name: John Ruane
 * Date Submitted: 11/9/2022
 * Lab Section: 001
 * Assignment Name: Lab 7
 */

#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <assert.h>

#include "binarySearchTree.h"

using namespace std;

void fix_size(Node *T)
{
  T->size = 1;
  if (T->left) T->size += T->left->size;
  if (T->right) T->size += T->right->size;
}

// insert key k into tree T, returning a pointer to the resulting tree
Node *insert(Node *T, int k){
  if (T == nullptr) return new Node(k);
  if (k < T->key) T->left = insert(T->left, k);
  else T->right = insert(T->right, k);
  fix_size(T);
  return T;
}

// returns a vector of key values corresponding to the inorder traversal of T (i.e., the contents of T in sorted order)
vector<int> inorder_traversal(Node *T)
{
  vector<int> inorder;
  vector<int> rhs;
  if (T == nullptr) return inorder;
  inorder=inorder_traversal(T->left);
  inorder.push_back(T->key);
  rhs=inorder_traversal(T->right);
  inorder.insert(inorder.end(), rhs.begin(), rhs.end());
  return inorder;
}

// return a pointer to the node with key k in tree T, or nullptr if it doesn't exist
Node *find(Node *T, int k) {
  //if T doesnt exist return null
  if(T == nullptr)
    return nullptr;
  
  //if T is k return it
  if(T->key == k)
    return T;

  //if T is less than K search the right side by going recursive
  else if(T->key < k)
    return find(T->right, k);
  
  //if T is greater than k search the left side by going recursive
  else if(T->key > k)
    return find(T->left, k);
  
  //return null if nothing goes through
  return nullptr;
}

// return pointer to node of rank r (with r'th largest key; e.g. r=0 is the minimum)
Node *select(Node *T, int r) {
  assert(T!=nullptr && r>=0 && r<T->size);

  //sort T 
  vector<int> vec = inorder_traversal(T);

  //find the rth value in T using the sorted T in vec
  return find(T, vec[r]);
}

// Join trees L and R (with L containing keys all <= the keys in R)
// Return a pointer to the joined tree.  
Node *join(Node *L, Node *R){
  //check if L and R exist if either dont return the other
  if(L == nullptr)
    return R;
  if(R == nullptr) 
    return L;
  
  //makes a random number to compare to p to check which side is dominate
  float p = (float)(L->size) / (L->size + R->size);
  float r = (float)rand();
  
  //if r is less than p make L dominant if it isnt make R
  if(r < p){
    L->right = join(L->right, R);
    fix_size(L);
    return L;
  }else{
    R->left = join(L, R->left);
    fix_size(R);
    return R;
  }


}

// remove key k from T, returning a pointer to the resulting tree.
// it is required that k be present in T
Node *remove(Node *T, int k){
  assert(T != nullptr);
  
  //check if T is greater than K and if it is remove the left node and set the new node to left then fix size
  if(T->key > k){
    T->left = remove(T->left, k);
    fix_size(T);

  //check if T is less than K and if it is remove the right node and set the new node to right then fix size
  }else if(T->key < k){
    T->right = remove(T->right, k);
    fix_size(T);

  //if K is equal to T join left and right nodes and set that to T deleting the original T in the process and fix size of T
  }else{
    Node *temp = T;

    T = join(T->left, T->right);

    if(T != nullptr){
      fix_size(T);
    }
    delete temp;
  }

  return T;
}

// Split tree T on key k into tree L (containing keys <= k) and a tree R (containing keys > k)
void split(Node *T, int k, Node **L, Node **R){
  //set L and R to nullptr
  *L = nullptr;
  *R = nullptr;

  //if T is less than or equal to K  check right and if it exists split right and set right to left then set left to T
  if(T->key <= k){
    if(T->right != nullptr){
      split(T->right, k, L, R);
      T->right = *L;
      *L = T;

    //if not set L to T
    }else
      *L = T;

    //if L exists fix size
    if(*L != nullptr)
      fix_size(*L);
  }else{
    //if left exists split left and then set it to R and set R to T
    if(T->left != nullptr){
      split(T->left, k, L, R);
      T->left = *R;
      *R = T;
    
    //if it doesnt set R to T
    }else
      *R = T;

    //if R doesnt exist fix size of R
    if(*R != nullptr)
      fix_size(*R);
  } 
}

// insert key k into the tree T, returning a pointer to the resulting tree
Node *insert_random(Node *T, int k){
  //if T doesnt exist return an empty node
  if(T == nullptr)
    return new Node(k);

  //set r to random number between 0 and size + 1
  int r = rand() % (T->size + 1);
  
  //if r is 0 insert k into the base node
  if(r == 0){
    Node *n = new Node(k);
    split(T,k,&n->left,&n->right);
    T = n;
    //if T is less than k set right to recursive insert random using T.right
  }else if(T->key < k)
    T->right = insert_random(T->right, k);
    //if not do the same but with left
  else
    T->left = insert_random(T->left, k);
  
  //fix size of T and then return it
  fix_size(T);
  return T;
}

void printVector(vector<int> v)
{
    for (int i=0; i<v.size(); i++)
    {
        cout << v[i] << endl;
    }
}

/*
int main(void)
{
  vector<int> inorder;
  vector<int> A(10,0);
  
  // put 0..9 into A[0..9] in random order
  for (int i=0; i<10; i++) A[i] = i;
  for (int i=9; i>0; i--) swap(A[i], A[rand()%i]);
  cout << "Contents of A (to be inserted into BST):\n";
  printVector(A);
  
  // insert contents of A into a BST
  Node *T = nullptr;
  for (int i=0; i<10; i++) T = insert(T, A[i]);
  
  // print contents of BST (should be 0..9 in sorted order)
  cout << "Contents of BST (should be 0..9 in sorted order):\n";
  inorder=inorder_traversal(T);
  printVector(inorder);

  // test select
  for (int i=0; i<10; i++) {
    Node *result = select(T, i);
    if (!result || result->key != i) cout << "Select test failed for select(" << i << ")!\n";
  }
  
  // test find: Elements 0..9 should be found; 10 should not be found
  cout << "Elements 0..9 should be found; 10 should not be found:\n";
  for (int i=0; i<11; i++) 
    if (find(T,i)) cout << i << " found\n";
    else cout << i << " not found\n";


  // test split
  Node *L, *R;
  split(T, 4, &L, &R);
  cout << "Contents of left tree after split (should be 0..4):\n";
  inorder=inorder_traversal(L);
  printVector(inorder);
  cout << "Left tree size " << L->size << "\n";
  cout << "Contents of right tree after split (should be 5..9):\n";
  inorder=inorder_traversal(R);
  printVector(inorder);
  cout << "Right tree size " << R->size << "\n";
    
  // test join
  T = join(L, R);
  cout << "Contents of re-joined tree (should be 0..9)\n";
  inorder=inorder_traversal(T);
  printVector(inorder);
  cout << "Tree size " << T->size << "\n";
  
  // test remove
  for (int i=0; i<10; i++) A[i] = i;
  for (int i=9; i>0; i--) swap(A[i], A[rand()%i]);
  for (int i=0; i<10; i++) {
    T = remove(T, A[i]);
    cout << "Contents of tree after removing " << A[i] << ":\n";
    inorder=inorder_traversal(T);
    printVector(inorder);
    if (T != nullptr)
      cout << "Tree size " << T->size << "\n";
  }

  // test insert_random
  cout << "Inserting 1 million elements in order; this should be very fast...\n";
  for (int i=0; i<1000000; i++) T = insert_random(T, i);
  cout << "Tree size " << T->size << "\n";
  cout << "Done\n";

  
  return 0;
}
*/
