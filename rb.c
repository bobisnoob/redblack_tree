#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#define RB_SIZE	(sizeof(rb)/sizeof(rb[0]))
#define SWAP_COLOR(i)	(rb[i].color == RED? BLACK : RED)
#define PARENT(i)	rb[i].parent
#define GRANDPARENT(i)	rb[rb[i].parent].parent
#define UNCLE(i)	rb[GRANDPARENT(i)].right == PARENT(i)? rb[GRANDPARENT(i)].left : rb[GRANDPARENT(i)].right

typedef enum {
	BLACK,
	RED
} eColor;

typedef struct {
	bool occupied;
	uint32_t data;
	eColor color;
	int16_t parent, left, right;
} rb_t;

uint16_t head;

rb_t rb[50];

int16_t rb_rotate_right(uint16_t node);
int16_t rb_rotate_left(uint16_t node);

int16_t rb_get_empty_node(void) {
	int16_t i;
	for (i = 0; i < RB_SIZE; i++) {
		if (rb[i].occupied == false)
			return i;
	}
	return -1;
}

void rb_add_node(int16_t index, uint32_t data, int16_t parent) {
	rb[index].occupied = true;
	rb[index].data = data;
	rb[index].left = -1;
	rb[index].right = -1;
	rb[index].parent = parent;
	rb[index].color = RED;
}

void rb_insert(uint32_t data) {
	uint16_t tmp_head = head;
	int16_t node;
	if (tmp_head == 0xffff) {
		node =  rb_get_empty_node();
		rb_add_node(node, data, node);
		head = node;
	} else {
		int16_t index, parent;
		parent = -1;
		do {
			if (data > rb[tmp_head].data) {
				index = rb[tmp_head].right;
				if (index == -1) {
					node =  rb_get_empty_node();
					if (parent == -1) 
						rb_add_node(node, data, rb[tmp_head].parent);
					else
						rb_add_node(node, data, parent);
					rb[tmp_head].right = node;
					break;
				}
				parent = index;
				tmp_head = index;
				continue;
			} else if (data < rb[tmp_head].data) {
				index = rb[tmp_head].left;
				if (index == -1) {
					node =  rb_get_empty_node();
					if (parent == -1) 
						rb_add_node(node, data, rb[tmp_head].parent);
					else
						rb_add_node(node, data, parent);
					rb[tmp_head].left = node;
					break;
				}
				parent = index;
				tmp_head = index;
				continue;
			} else {
				break;
			}
		} while (index != -1);
	}

	while (rb[PARENT(node)].color == RED && node != head) {
		if ((int16_t)(UNCLE(node)) != -1 && rb[UNCLE(node)].color == RED) {
			rb[UNCLE(node)].color = SWAP_COLOR(UNCLE(node));
			rb[PARENT(node)].color = SWAP_COLOR(PARENT(node));
			rb[GRANDPARENT(node)].color = SWAP_COLOR(GRANDPARENT(node));
		} else {
			int16_t n;
			if(rb[GRANDPARENT(node)].right == PARENT(node)) {
				if (rb[PARENT(node)].right == node) {
					n = rb_rotate_left(GRANDPARENT(node));
				} else {
					rb_rotate_right(PARENT(node));
					n = rb_rotate_left(rb[PARENT(node)].parent);
					node = n;
				}
			} else {
				if (rb[PARENT(node)].left == node) {
					n = rb_rotate_left(GRANDPARENT(node));
				} else {
					rb_rotate_left(PARENT(node));
					n = rb_rotate_right(rb[PARENT(node)].parent);
					node = n;
				}
			}
			rb[PARENT(n)].color = BLACK;
			rb[n].color = RED;
		}
		node = GRANDPARENT(node);	
	}
	rb[head].color = BLACK;	
}

int16_t  rb_rotate_left(uint16_t node) {
	int16_t new_node = -1;
	if (rb[node].right != -1) {
		int16_t current_node, previous_node;
		previous_node = node;
		current_node = rb[node].left;

		if (rb[node].parent != -1) {
			new_node =  rb_get_empty_node();
			rb_add_node(new_node, rb[previous_node].data, previous_node);
			rb[new_node].left = current_node;
			if (current_node != -1) {
				rb[current_node].parent = new_node;
			}
			
			int16_t right_previous = rb[previous_node].right;
			if (right_previous != -1) {
				if (rb[right_previous].left != -1) {
					rb[new_node].right = rb[right_previous].left;
					rb[rb[right_previous].left].parent = new_node;
				}
				rb[rb[right_previous].right].parent = previous_node;
			}
			rb[new_node].occupied = true;
			rb[previous_node].data = rb[right_previous].data;
			rb[previous_node].left = new_node;
			rb[previous_node].right = rb[right_previous].right;
			int16_t right_right_previous = rb[right_previous].right;
			if (right_right_previous != -1) {
				rb[right_right_previous].parent = previous_node;
			}
			rb[right_previous].occupied = false;
		}
	}
	return new_node;
}

int16_t rb_rotate_right(uint16_t node) {
	int16_t new_node =-1;
	if (rb[node].left != -1) {
		int16_t current_node, previous_node;
		previous_node = node;
		current_node = rb[node].right;

		if (rb[node].parent != -1) {
			new_node =  rb_get_empty_node();
			rb_add_node(new_node, rb[previous_node].data, previous_node);
			rb[new_node].right = current_node;
			if (current_node != -1) {
				rb[current_node].parent = new_node;
			}
			
			int16_t left_previous = rb[previous_node].left;
			if (left_previous != -1) {
				if (rb[left_previous].right != -1) {
					rb[new_node].left = rb[left_previous].right;
					rb[rb[left_previous].right].parent = new_node;
				}
				rb[rb[left_previous].left].parent = previous_node;
			}
			rb[new_node].occupied = true;
			rb[previous_node].data = rb[left_previous].data;
			rb[previous_node].right = new_node;
			rb[previous_node].left = rb[left_previous].left;
			int16_t left_left_previous = rb[left_previous].left;
			if (left_left_previous != -1) {
				rb[left_left_previous].parent = previous_node;
			}
			rb[left_previous].occupied = false;
		}
	}
	return new_node;
}

int16_t rb_delete(uint32_t data) {
	int16_t current_node = head;

	while (current_node != -1) {
		if (data > rb[current_node].data) {
			current_node = rb[current_node].right;
			continue;
		} else if (data < rb[current_node].data) {
			current_node = rb[current_node].left;
			continue;
		} else {
			break;
		}
	}
	
	int16_t tmp = current_node;
	int16_t successor_node = -1;
	while (tmp != -1) {
		if (rb[tmp].left != -1) {
			successor_node = rb[tmp].left;
			tmp = rb[tmp].right;
		}
		else if (rb[tmp].right != -1)
		{
			successor_node = rb[tmp].right;
			tmp = rb[tmp].left;
		}
		else
		{
			successor_node = tmp;
			break;
		}
	}
	return successor_node;	
}

void rb_print_tree(void) {
	uint32_t i;
	for (i = 0; i < RB_SIZE; i++) {
		if (rb[i].occupied)
			printf("C:%c  %u -> %u (L:%d P:%d R:%d)\n", rb[i].color? 'R': 'B', i, rb[i].data , rb[i].left, rb[i].parent, rb[i].right);
	}
	printf("---------------------\n");
}

int main(void) {
	head = 0xffff;
	rb_insert(10);
	rb_insert(20);
	rb_insert(30);
	rb_insert(25);
	rb_insert(5);
	rb_print_tree();
	printf("%d\n", rb_delete(10));
//	rb_insert(50);
//	rb_insert(40);
//	rb_insert(70);
//	rb_insert(90);
//	rb_insert(7);
//	rb_insert(6);
//	rb_insert(8);
//	rb_insert(9);
//	rb_insert(25);
//	rb_insert(11);
//	rb_insert(28);
//	rb_print_tree();
//	rb_insert(3);
//	rb_insert(6);
//	rb_insert(25);
//	rb_insert(17);
	//rb_insert(4);
	//rb_insert(7);
	//rb_insert(40);
	//rb_insert(50);
	//
	return 0;
}
