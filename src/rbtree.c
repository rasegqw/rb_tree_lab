#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  return p;
}

void delete_rbtree(rbtree *t) {
  // 삭제할 때, 노드의 자녀가 없거나, 하나라면,
  // 삭제되는 색 = 삭제되는 노드의 색

  // 삭제할 때, 노드의 자녀가 둘이라면, successor와 값만 바꾸고 삭제
  // 삭제되는 색 = 삭제되는 노드의 successor의 색 

  // 삭제되는 색이 red라면 어떤 속성도 위반하지 않음.

  // 삭제되는 색이 black이라면 rbtree의 속성을 다시 따져봐야 함.

  // 삭제되는 색이 black이고, 그 자리를 대체하는게 black이다? -> doubly black
  // 삭제되는 색이 black이고, 그 자리를 대체하는게 red다? -> red and black -> 그냥 black으로 바꿔주면 됨.

  // doubly black 처리 4가지 케이스
  // 형제의 색, 형제의 자녀들의 색에 따라 나눔.

  // 1. 오른쪽 형제가 black이고, 그 형제의 오른쪽 자녀가 red일 때,
  // (부모와, 형제의 왼쪽노드는 어떤 색이어도 상관 없음.)

  // 2. 오른쪽 형제가 black이고, 그 형제의 왼쪽 자녀가 red이며, 오른쪽 자녀는 black일 때,
  // (부모 노드는 어떤 색이어도 상관 없음.)

  // 3. 오른쪽 형제가 black이고, 그 형제의 자녀들 모두 black일 때,
  // (부모 노드는 어떤 색이어도 상관 없음.)

  // 4. 오른쪽 형제가 red이고, 그 자녀들 모두 black일 때,
  // (부모 노드도 black)


  // TODO: reclaim the tree nodes's memory
  free(t);
}




node_t *rbtree_insert(rbtree *t, const key_t key) {
  // 삽입할 때는 항상 레드로 삽입.
  // 루트노드라면, 레드를 블랙을 바꿔주면 됨.

  // 삽입된 노드의 부모가 레드이고, 부모의 형제가 블랙일때 -> 회전

  // 삽입된 노드의 부모가 레드이고, 부모의 형제도 레드이면 -> 부모와 할아버지의 색 바꾸기
  // -> 할아버지와 색 바꾸고 계속 RB트리 조건 확인해야함.
   
  
// typedef enum { RBTREE_RED, RBTREE_BLACK } color_t;

// typedef int key_t;

// node_t
//   color_t color;
//   key_t key;
//   struct node_t *parent, *left, *right;

// rbtree
//   node_t *root;
//   node_t *nil;  // for sentinel
  
  node_t *new_node = malloc(sizeof(node_t));
  new_node->color = RBTREE_RED;
  new_node->key = key;
  new_node->left = NULL;
  new_node->right = NULL;
  new_node->parent = NULL;

  if (t->root == NULL)
  {
    t->root = new_node;
    t->root->color = RBTREE_BLACK;
  }
  else{
    node_t *cur = t->root;

    while (cur != NULL)
    {
      if (key > t->root->key)
        cur = cur->right;
      else
        cur = cur->left;
    }

    if (cur->parent->color == RBTREE_RED)
    {
      
      


    }
    else
      cur = new_node;








  }

  // TODO: implement insert
  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  return t->root;
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
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}
