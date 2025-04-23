#include "rbtree.h"
#include <stdlib.h>
#include <stdio.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));

  node_t *nil = (node_t *)malloc(sizeof(node_t));

  nil->color = RBTREE_BLACK;
  nil->key = -1;
  nil->left = nil;
  nil->right = nil;
  nil->parent = nil;

  p->nil = nil;
  p->root = nil;

  return p;
}

void delete_node(node_t *node, node_t *nil)
{
  if (node == nil)
    return;
  
  delete_node(node->left, nil);
  delete_node(node->right, nil);
  

  free(node);
}

void delete_rbtree(rbtree *t) 
{
  delete_node(t->root, t->nil);
  
  free(t->nil);
  free(t);
}

void rotate_right(node_t *cur, node_t *Par, rbtree *t)
{
  Par->left = cur->right;

  if (cur->right != t->nil)
    cur->right->parent = Par;

  cur->parent = Par->parent;
  
  if (Par == t->root)
    t->root = cur;
  else if (Par == Par->parent->left)
    Par->parent->left = cur;
  else
    Par->parent->right = cur;

  cur->right = Par;
  Par->parent = cur;
}

void rotate_left(node_t *cur, node_t *Par, rbtree *t)
{
  Par->right = cur->left;

  if (cur->left != t->nil)
    cur->left->parent = Par;

  cur->parent = Par->parent;
  
  if (Par == t->root)
    t->root = cur;
  else if (Par == Par->parent->left)
    Par->parent->left = cur;
  else
    Par->parent->right = cur;

  cur->left = Par;
  Par->parent = cur;
}

node_t *rbtree_insert(rbtree *t, const key_t key)
{  
  // 삽입할 노드 메모리 할당.
  node_t *new_node = (node_t*)malloc(sizeof(node_t));
  new_node->color = RBTREE_RED;
  new_node->key = key;
  new_node->parent = t->nil;
  
  new_node->left = t->nil;
  new_node->right = t->nil;

  // 루트가 비어있으면 루트 채우기
  if (t->root == t->nil)
  {
    t->root = new_node;
    t->root->color = RBTREE_BLACK;
  }
  // 그게 아니면 넣을 자리 찾아서 넣기
  else{
    node_t *cur = t->root;
    node_t *Par = cur->parent;
    // parent 추적용 Par. cur->parent를 추적함.
    // 마지막에 cur->parent를 찾으면, cur이 nil이기 때문에, parent가 없어서 오류남.

    while (cur != t->nil)
    {
      Par = cur;
      
      if (key < cur->key)
        cur = cur->left;
      else
        cur = cur->right;
    }

    new_node->parent = Par;
    cur = new_node;

    if (new_node->key < Par->key)
      Par->left = new_node;
    else
      Par->right = new_node;

    Par = cur->parent;

    // 추가한 노드 바로 위가 빨간색이면 계속
    while (Par->color == RBTREE_RED)
    {


      if (Par == Par->parent->left)
      {
        node_t *uncle = Par->parent->right;
        
        if (uncle->color == RBTREE_RED)
        {
          Par->color = RBTREE_BLACK;
          uncle->color = RBTREE_BLACK;

          Par->parent->color = RBTREE_RED;

          cur = Par->parent;
          Par = cur->parent;
        }
        else
        {
          if (cur == Par->right)
          {
            // 부모의 값보다 크면(꺾여있는 경우면)
            cur = Par;
            rotate_left(cur->right, cur, t);
          }
          
          Par = cur->parent;
        
          Par->color = RBTREE_BLACK;
          Par->parent->color = RBTREE_RED;
          
          rotate_right(Par, Par->parent, t);
        }
      }
      else
      {
        node_t *uncle = Par->parent->left;
        
        if (uncle->color == RBTREE_RED)
        {
          Par->color = RBTREE_BLACK;
          uncle->color = RBTREE_BLACK;

          Par->parent->color = RBTREE_RED;

          cur = Par->parent;
          Par = cur->parent;
        }
        else
        {
          if (cur == Par->left)
          {
            // 부모의 값보다 크면(꺾여있는 경우면)
            cur = Par;
            rotate_right(cur->left, cur, t);
          }
          
          Par = cur->parent;
        
          Par->color = RBTREE_BLACK;
          Par->parent->color = RBTREE_RED;
          
          rotate_left(Par, Par->parent, t);
        }        
      }
    }
    

    t->root->color = RBTREE_BLACK;
  }
  
  
  return new_node;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  node_t *cur;

  cur = t->root;
  
  while (cur != t->nil)
  {
    if (key > cur->key)
      cur = cur->right;
    else if (key < cur->key)
      cur = cur->left;
    else
      break;
  }
  
  if (cur == t->nil)
    return NULL;
  else
    return cur;
}

node_t *rbtree_min(const rbtree *t) 
{
  node_t *cur;
  node_t *Par;

  cur = t->root;
  Par = cur->parent;
  if (t->root == t->nil)
    return t->nil;

  while (cur != t->nil)
  {
    Par = cur;
    cur = cur->left;
  }
  
  return Par;
}

node_t *rbtree_max(const rbtree *t) 
{
  node_t *cur;
  node_t *Par;

  cur = t->root;
  Par = cur->parent;
  if (t->root == t->nil)
    return t->nil;

  while (cur != t->nil)
  {
    Par = cur;
    cur = cur->right;
  }
  
  return Par;
}

int rbtree_erase(rbtree *t, node_t *p) {
  node_t *cur = t->root;
  node_t *Par = t->root;

  while (cur != t->nil)
  {
    if (p->key > cur->key)
    {
      Par = cur;
      cur = cur->right;
    }
    else if (p->key < cur->key)
    {
      Par = cur;
      cur = cur->left;
    }  
    else
      break;
  }
  
  // 삭제하는데, 자식이 둘다 있는 경우
  if (cur->left != t->nil && cur->right != t->nil)
  {
    node_t *del_cur;
    // 후임자 선택
    del_cur = cur->right;

    while (del_cur->left != t->nil)
      del_cur = del_cur->left;

    // 후임자 값, 루트에 넣어주고
    cur->key = del_cur->key;

    // 이제 다시 삭제할 노드를 cur로 잡아두고
    cur = del_cur;
    Par = cur->parent;
  }
  
  // 이제 자식이 없거나, 하나 있거나 한경우 삭제.
  // 삭제할 노드의 색이 빨간색인 경우
  if (cur->color == RBTREE_RED)
  {
    // 삭제할 노드의 자식이 아무도 없다면, 삭제
    if (cur->left == t->nil && cur->right == t->nil)
    {
      if (cur == cur->parent->left)
        cur->parent->left = t->nil;
      else
        cur->parent->right = t->nil;

    }
    // 자식이 왼쪽에 하나 있으면?
    else if (cur->left != t->nil)
    {
      if (cur == cur->parent->left)
        cur->parent->left = cur->left;
      else
        cur->parent->right = cur->left;
    }
    // 자식이 오른쪽에 하나 있으면?
    else
    {
      if (cur == cur->parent->left)
        cur->parent->left = cur->right;
      else
        cur->parent->right = cur->right;
    }

    // printf("0x%012X\n", cur);
    free(cur);
  }

  // 삭제할 노드의 색이 검은색인 경우
  else
  {
    if (cur->left != t->nil && cur->left->color == RBTREE_RED)
    {
      if (cur == t->root)
      {
        t->root = cur->left;
        t->root->parent = t->nil;
        t->root->color = RBTREE_BLACK;

        free(cur);
        return 0;
      }

      if (cur == cur->parent->left)
        cur->parent->left = cur->left;
      else
        cur->parent->right = cur->left;

      cur->left->parent = Par;
      cur->left->color = RBTREE_BLACK;

      // printf("%p\n", cur);
      free(cur);
    }
    else if (cur->right != t->nil && cur->right->color == RBTREE_RED)
    {
      if (cur == t->root)
      {
        t->root = cur->right;
        t->root->parent = t->nil;
        t->root->color = RBTREE_BLACK;

        free(cur);
        return 0;
      }

      if (cur == cur->parent->left)
        cur->parent->left = cur->right;
      else
        cur->parent->right = cur->right;

      cur->right->parent = Par;        
      cur->right->color = RBTREE_BLACK;

      // printf("%p\n", cur);
      free(cur);
    }
    else
    {
      int left_right;

      if (cur == Par->left)
        left_right = 1;
      else
        left_right = 0;

      node_t *del = cur;
      node_t *bro;
  
      // 삭제 상위 노드들의 색 보정을 위해서 계속해서 확인해야함.
      // 커서 노드가 red가 되거나, 루트까지 올라가면? 끝.
      while (cur != t->root && cur->color == RBTREE_BLACK)
      {
        if (cur == cur->parent->left)
        {
          bro = Par->right;
  
          // 형제의 색이 빨강이다?
          if (bro->color == RBTREE_RED)
          {
            // 형제와 부모의 색을 바꾸고, 회전
            bro->color = RBTREE_BLACK;
            Par->color = RBTREE_RED;
  
            rotate_left(bro, Par, t);
  
            bro = Par->right;
          }
  
          // 형제는 이제 검은색. 근데 형제의 자녀 모두 black이다?
          if (bro->left->color == RBTREE_BLACK && bro->right->color == RBTREE_BLACK)
          {
            bro->color = RBTREE_RED;
            cur = Par;
            Par = cur->parent;
          }
          else
          {
            if (bro->right->color == RBTREE_BLACK)
            {
              bro->left->color = RBTREE_BLACK;
              bro->color = RBTREE_RED;
              rotate_right(bro->left, bro, t);
  
              bro = Par->right;
            }
  
            bro->color = Par->color;
            Par->color = RBTREE_BLACK;
  
            bro->right->color = RBTREE_BLACK;
  
            rotate_left(bro, Par, t);
  
            cur = t->root;
          }
        }
        else
        {
          bro = Par->left;
  
          // 형제의 색이 빨강이다?
          if (bro->color == RBTREE_RED)
          {
            // 형제와 부모의 색을 바꾸고, 회전
            bro->color = RBTREE_BLACK;
            Par->color = RBTREE_RED;
  
            rotate_right(bro, Par, t);
  
            bro = Par->left;
          }
  
          // 형제는 이제 검은색. 근데 형제의 자녀 모두 black이다?
          if (bro->left->color == RBTREE_BLACK && bro->right->color == RBTREE_BLACK)
          {
            bro->color = RBTREE_RED;
            cur = Par;
            Par = cur->parent;
          }
          else
          {
            if (bro->left->color == RBTREE_BLACK)
            {
              bro->right->color = RBTREE_BLACK;
              bro->color = RBTREE_RED;
              rotate_left(bro->right, bro, t);
  
              bro = Par->left;
            }
  
            bro->color = Par->color;
            Par->color = RBTREE_BLACK;
  
            bro->left->color = RBTREE_BLACK;
  
            rotate_right(bro, Par, t);
  
            cur = t->root;
          }
        }
      }
      
      // 만약 삭제할 노드가 루트노드라면?
      if (del == t->root)
      {
        // 루트의 자식이 있으면 루트 교체, 자식이 없으면 nil로 두고
        if (t->root->left == t->nil && t->root->right == t->nil)
          t->root = t->nil;
        else if (t->root->left != t->nil)
          t->root = t->root->left;
        else
          t->root = t->root->right;
        
        t->root->color = RBTREE_BLACK;
      }
      else if (left_right)
        del->parent->left = del->right;
      else
        del->parent->right = del->right;
      
      
      cur->color = RBTREE_BLACK;

      if (del != t->nil)  
      {
        // printf("%p\n", del);
        free(del);
      }
    }
  }         
      
  return 0;
}

int rbtree_Inorder(const rbtree *t, node_t *p, key_t *arr, size_t i)
{
  int a = i;

  if (p->left != t->nil)
    a = rbtree_Inorder(t, p->left, arr, a);
  
  *(arr + a) = p->key;
  printf("%d, ", p->key);
  printf("%s\n", p->color == RBTREE_RED ? "Red" : "Black");
  a++;
  
  if (p->right != t->nil)
    a = rbtree_Inorder(t, p->right, arr, a);

  return a;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n)
{
  rbtree_Inorder(t, t->root, arr, 0);

  return 0;
}
