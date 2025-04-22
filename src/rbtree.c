#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));

  node_t *nil = malloc(sizeof(node_t));

  nil->color = RBTREE_BLACK;
  nil->key = 0;
  nil->left = nil;
  nil->right = nil;
  nil->parent = nil;

  p->nil = nil;
  p->root = nil;
  // p->root->parent = nil;

  return p;
}

void delete_rbtree(rbtree *t) {

  if (t->root == t->nil)
    free(t->nil);
    
  free(t);
}


void rotate_right(node_t *cur, node_t *Par, rbtree *t)
{
  
  Par->left = cur->right;
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

node_t *rbtree_insert(rbtree *t, const key_t key) {
  
  // 삽입할 노드 메모리 할당.
  node_t *new_node = malloc(sizeof(node_t));
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
    node_t *Par = cur;
    // parent 추적용 Par. cur->parent를 추적함.
    // 마지막에 cur->parent를 찾으면, cur이 nil이기 때문에, parent가 없어서 오류남.

    while (cur != t->nil)
    {
      if (key > cur->key)
      {
        Par = cur;
        cur = cur->right;
      }
      else
      {
        Par = cur;
        cur = cur->left;
      }
    }

    new_node->parent = Par;
    cur = new_node;

    if (new_node->key <= Par->key)
      Par->left = new_node;
    else
      Par->right = new_node;

    // cur의 색이 red면서, parent의 색도 red면 계속 진행
    while (cur->color == RBTREE_RED && Par->color == RBTREE_RED)
    {
        node_t *uncle;
  
        if (Par == Par->parent->left)
          uncle = Par->parent->right;
        else
          uncle = Par->parent->left;

        // 삼촌 노드가 nil노드이면,
        if (uncle->color == RBTREE_BLACK)
        {
            // 할아버지 노드의 값이 현재 노드보다 작으면(할아버지에서 오른쪽으로 내려온 경우면)
            if (Par->parent->key < cur->key)
            {
              // 근데 부모 노드보다는 값이 작으면(꺾여있는 경우이면)
              if (cur->key < Par->key)
                rotate_right(cur, Par, t);
              else
                cur = Par;

              Par = cur->parent;

              cur->color = RBTREE_BLACK;
              Par->color = RBTREE_RED;
              
              rotate_left(cur, Par, t);
            }
            // 할아버지 노드 값보다 작으면(왼쪽으로 내려온 경우면)
            else if (cur->key <= Par->parent->key)
            {
              // 부모의 값보다 크면(꺾여있는 경우면)
              if (Par->key < cur->key)
                rotate_left(cur, Par, t);
              else
                cur = Par;

              Par = cur->parent;

              cur->color = RBTREE_BLACK;
              Par->color = RBTREE_RED;
  
              rotate_right(cur, Par, t);
            }

            cur = Par;
            Par = cur->parent;
        }
        // 삼촌도 red여서 red가 한쪽에 몰려있는 경우가 아니면,
        else
        {
          // 삼촌도, 부모도 black으로 바꿔주고
            Par->color = RBTREE_BLACK;
            uncle->color = RBTREE_BLACK;
  
          // 할아버지는 red로 바꿔줌.
            Par->parent->color = RBTREE_RED;
            if (Par->parent == t->root)
              t->root->color = RBTREE_BLACK;
  
            cur = Par->parent;
            Par = cur->parent;
        }   
      }
  }

  // TODO: implement insert
  return t->root;
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

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  return t->root;
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

    while (del_cur != t->nil)
    {
      Par = del_cur;
      del_cur = del_cur->left;
    }

    // 후임자 값, 루트에 넣어주고
    cur->key = Par->key;

    // 이제 다시 삭제할 노드를 cur로 잡아두고
    cur = Par;
    Par = cur->parent;
  }
  
  // 이제 자식이 없거나, 하나 있거나 한경우 삭제.
  // 삭제할 노드의 색이 빨간색인 경우
  if (cur->color == RBTREE_RED)
  {
    // 삭제할 노드의 자식이 아무도 없다면, 삭제
    if (cur->left == t->nil && cur->right == t->nil)
    {
      if (cur == Par->left)
        Par->left = t->nil;
      else
        Par->right = t->nil;

    }
    // 자식이 왼쪽에 하나 있으면?
    else if (cur->left != t->nil)
    {
      if (cur == Par->left)
        Par->left = cur->left;
      else
        Par->right = cur->left;
    }
    // 자식이 오른쪽에 하나 있으면?
    else
    {
      if (cur == Par->left)
        Par->left = cur->right;
      else
        Par->right = cur->right;
    }

    free(cur);
  }

  // 삭제할 노드의 색이 검은색인 경우
  else
  {
    node_t *del = cur;
    node_t *bro;

    // 삭제 상위 노드들의 색 보정을 위해서 계속해서 확인해야함.
    // 커서 노드가 red가 되거나, 루트까지 올라가면? 끝.
    while (cur != t->root && cur->color == RBTREE_BLACK)
    {
      if (cur == Par->left)
      {
        bro = Par->right;

        // 형제의 색이 빨강이다?
        if (bro->color == RBTREE_RED)
        {
          // 형제와 부모의 색을 바꾸고, 회전
          bro->color = RBTREE_BLACK;
          Par->color = RBTREE_RED;

          rotate_left(bro, Par, t);

          Par = cur->parent;
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

          Par = cur->parent;
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
    
    if (del == t->root)
      t->root = t->nil;
    free(del);
  }         
      
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n)
{




  return 0;
}
