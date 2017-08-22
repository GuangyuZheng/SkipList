#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define SKIP_LIST_INIT_LEVEL  5

struct list_node_s;
struct list_s;
typedef struct list_node_s list_node_t;
typedef struct list_s list_t;

struct list_node_s {
	int key;
	void *value;
	int high;
	struct list_node_s **forward;
};

struct list_s {
	int level;
	struct list_node_s *header;
};


int random_level();
list_t *new_list(int init_level);
list_node_t *new_list_node(int high);
void *find(list_t *l, int key);
void insert(list_t *l, int key, void *value);
void *delete_node(list_t *l, int key);
void destroy(list_t *l);

int random_level()
{
	int k = 0;
//	srand(time(NULL));
	while (rand() % 2) {
		k++;
	}

	return k;
}

list_t *new_list(int init_level)
{
	list_t *l;
	l = malloc(sizeof(list_t));
	l->header = new_list_node(init_level);
	l->level = 0;
	l->header->key = -1;
	return l;
}

list_node_t *new_list_node(int high)
{
	int i;
	list_node_t *n;
	n = malloc(sizeof(list_node_t));
	n->forward = malloc(high *sizeof(void *));
	n->high = high;

	for (i = 0; i < high; i++) {
		n->forward[i] = NULL;
	}

	return n;
}

void destroy(list_t *l)
{
	list_node_t *n, *next;
	n = l->header;
	while (n) {
		next = n->forward[0];
		free(n->forward);
		free(n);
		n = next;
	}
	free(l);
}

void *find(list_t *l, int key)
{
	int k = l->level;
	list_node_t *n = l->header;

	while (n && k >= 0) {
		while(n->forward[k] && n->forward[k]->key <= key) {
			n = n->forward[k];
		}

		if (n->key == key) {
			return n->value;
		}
		k--;
	}

	return NULL;
}

void insert(list_t *l, int key, void *value)
{
	int insert_level;
	int k;
	size_t size;
	list_node_t *p, *q, *n;
	list_node_t **update;

	k = l->level;
	size = sizeof(void *) * (k + 1);
	update = malloc(size);
	p = q = l->header;

	while (q && k >= 0) {
		while (q->key < key) {
			p = q;
			if (q->forward[k]) {
				q = q->forward[k];
			}
			else {
				break;
			}
		}
		q = update[k] = p;
		//	printf("level:%d->%d ",k, p->key);
		k--;
	}

	insert_level = random_level();
	if (insert_level > l->level ) {

		k = l->level++;
		insert_level = l->level;
		n = new_list_node(insert_level + 1);
		n->key = key;
		n->value = value;
		if (insert_level >= l->header->high) {
			list_node_t **l1, **l2;
			l1 = malloc(sizeof(void *) * (insert_level + 1));
			l2 = l->header->forward;
			memcpy(l1, l2, sizeof(void *) * insert_level);
			l1[insert_level] = NULL;
			l->header->forward = l1;
			free(l2);
			l->header->high++;
			l->header->forward[insert_level] = n;
		}
		else {
			l->header->forward[insert_level] = n;
		}

	}
	else {
		k = insert_level;
		n = new_list_node(k + 1);
		n->value = value;
		n->key = key;
	}
//	printf("key: %d update: ", key);

	while (k >= 0) {
		p = update[k];
//		printf("%d ", p->key);
		q = p->forward[k];
		p->forward[k] = n;
		n->forward[k] = q;
		k--;
	}
//	printf("\n");
	free(update);
}

void *delete(list_t *l, int key)
{
	int k = l->level;
	list_node_t *p, *q, *n;
	p = q = l->header;
	n = NULL;
	list_node_t **update;

	update = malloc(sizeof(void *) * (k + 1));

	while (q && k >= 0) {
		while (q->key < key) {
			p = q;
			if (q->forward[k]) {
				q = q->forward[k];
			}
			else {
				break;
			}
		}

		q = update[k] = p;

		k--;
	}

	if (update[0]->forward[0] && update[0]->forward[0]->key == key) {
		n = update[0]->forward[0];
	}
	else {
		free(update);
		return NULL;
	}

	k = n->high - 1;
	while (k >= 0) {
		p = update[k];
		p->forward[k] = n->forward[k];
		k--;
	}

	free(update);
	return n->value;
}



void show_list(list_t *l)
{
	int k = l->level;
	list_node_t *n;
	n = l->header;
	while (k >= 0) {
		n = l->header;
		while (n) {
			printf("%d ", n->key);
			n = n->forward[k];
		}
		printf("\n");
		k--;
	}
}
