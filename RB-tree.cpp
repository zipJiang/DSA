/*** This cpp file is the instantiation of data structure RB-tree. ***/
/*** a linked-list version is implemented. ***/
/*** zipJiang 2016 ***/
#include<cstring>
#define BLACK true
#define RED false

struct Node{
	Node *p;
	Node *child[2]; //left is child[0], right is child[1];
	int key;
	bool color; //Red related to RED, Black BLACK respectively
	Node():p(NULL), key(0), color(RED) {
		child[0] = NULL;
		child[1] = NULL;
	}
	Node(Node *nil):p(nil), key(0), color(RED) {
		child[0] = nil;
		child[1] = nil;
	}
	Node(int k, Node *nil):p(nil), key(k), color(RED) {
		child[0] = nil;
		child[1] = nil;
	}
};

struct Tree {
	Node *root;
	Node *nil;
	Tree():root(NULL) {nil = new Node(); nil->color = BLACK;}
};

//Left Rotate is written with reference to CLRS p313.
void left_rotate(Tree *T, Node *x) {
	Node *y = x->child[1];
	x->child[1] = y->child[0];
	if(y->child[0] != T->nil)
		y->child[0]->p = x;
	y->p = x->p;
	if(x->p == T->nil)
		T->root = y;
	else if(x == x->child[0]) {
		x->p->child[0] = y;
	}
	else {
		x->p->child[1] = y;
	}
	x->child[0] = x;
	x->p = y;
	return ;
}

//A Right Rotate counterpart
void right_rotate(Tree *T, Node *x) {
	Node *y = x->child[0];
	x->child[0] = y->child[1];
	if(x->child[0] != T->nil)
		y->child[0]->p = x;
	y->p = x->p;
	if(x->p == T->nil)
		T->root = y;
	else if(x == x->child[1]) {
		x->p->child[1] = y;
	}
	else {
		x->p->child[0] = y;
	}
	x->child[1] = x;
	x->p = y;
	return ;
}
void rb_insert_fixup(Tree *T, Node *z) {
	while(z->p->color == RED) {
		if(z->p == z->p->p->child[0]) {
			Node *y = z->p->p->child[1];
			if(!y->color) {
				//case 1:
				z->p->color = BLACK;
				y->color = BLACK;
				z->p->p->color = RED;
				z = z->p->p;
			}
			else {
				if(z == z->p->child[1]) {
					//case2:
					z = z->p;
					left_rotate(T, z);
				}
				//case3:
				z->p->color = BLACK;
				z->p->p->color = RED;
				right_rotate(T, z->p->p);
			}
		}
		else {	
			Node *y = z->p->p->child[0];
			if(!y->color) {
				//case 1:
				z->p->color = BLACK;
				y->color = BLACK;
				z->p->p->color = RED;
				z = z->p->p;
			}
			else {
				if(z == z->p->child[0]) {
					//case2:
					z = z->p;
					right_rotate(T, z);
				}
				//case3:
				z->p->color = BLACK;
				z->p->p->color = RED;
				left_rotate(T, z->p->p);
			}
		}
	}
	return ;
}

void rb_insert(Tree *T, Node *z) {
	Node *y = T->nil;
	Node *x = T->root;
	//Loop until we find the normal and initial position of node z.
	while( x != T->nil) {
		y = x;
		if(z->key < x->key) {
			x = x->child[0];
		}
		else x = x->child[1];
	}
	z->p = y;
	if(y == T->nil) {
		T->root = z;
	}
	else if(z->key < y->key) {
		y->child[0] = z;
	}
	else {
		y->child[1] = z;
	}
	z->child[0] = T->nil;
	z->child[1] = T->nil;
	z->color = RED;
	rb_insert_fixup(T, z);
	return ;
}

void rb_transplant(Tree *T, Node *u, Node *v) {
	//replace subtree rooted at u with subtree rooted at v.
	if(u->p == T->nil) {
		T->root = v;
	}
	else if(u == u->p->child[0]) {
		u->p->child[0] = v;
	}
	else u->p->child[1] = v;
	v->p = u->p;
	return ;
}

Node *T_min(Tree *T, Node *root) {
	while(root->child[0] != T->nil)
		root = root->child[0];
	return root;
}

void rb_delete_fixup(Tree *T, Node *x) {
	Node *w = NULL;
	while(x != T->root && x->color) {
		if(x == x->p->child[0]) {
			w = x->p->child[1];
			if(!w->color) {
				//case 1:
				w->color = BLACK;
				x->p->color = RED;
				left_rotate(T, x->p);
				w = x->p->child[1];
			}
			if(!(w->child[0]->color && w->child[1]->color)) {
				//case 2:
				w->color = RED;
				x = x->p;
			}
			else {
				if(w->child[1]->color) {
					//case 3:
					w->child[0]->color = BLACK;
					w->color = RED;
					right_rotate(T, w);
					w = x->p->child[1];
				}
				w->color = x->p->color;
				x->p->color = BLACK;
				w->child[1]->color = BLACK;
				left_rotate(T, x->p);
				x = T->root;
			}
		}
		else {
			//Mirror true the sight I see.
			w = x->p->child[0];
			if(!w->color) {
				//case 1:
				w->color = BLACK;
				x->p->color = RED;
				right_rotate(T, x->p);
				w = x->p->child[0];
			}
			if(!(w->child[0]->color && w->child[1]->color)) {
				//case 2:
				w->color = RED;
				x = x->p;
			}
			else {
				if(w->child[0]->color) {
					//case 3:
					w->child[1]->color = BLACK;
					w->color = RED;
					left_rotate(T, w);
					w = x->p->child[0];
				}
				w->color = x->p->color;
				x->p->color = BLACK;
				w->child[0]->color = BLACK;
				right_rotate(T, x->p);
				x = T->root;
			}
		}
	}
	return ;
}

void rb_delete(Tree *T, Node *z) {
	Node *y = z;
	Node *x = NULL;
	bool y_original_color = y->color;
	if(z->child[0] == T->nil) {
		x = z->child[1];
		rb_transplant(T, z, z->child[1]);
		delete z;
	}
	else if(z->child[1] == T->nil) {
		x = z->child[0];
		rb_transplant(T, z, z->child[0]);
		delete z;
	}
	else {
		y = T_min(T, z->child[1]);
		y_original_color = y->color;
		x = y->child[1];
		if(y->p == z)
			x->p = y;
		else {
			rb_transplant(T, y, y->child[1]);
			y->child[1] = z->child[1];
			y->child[1]->p = y;
		}
		rb_transplant(T, z, y);
		y->child[0] = z->child[0];
		y->child[0]->p = y;
		y->color = z->color;
		delete z;
	}
	if(y_original_color)
		rb_delete_fixup(T, x);
	return ;
}
