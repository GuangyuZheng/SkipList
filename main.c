#include "SkipList.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int k, i;
	list_t *l;
	void *ptr = &k;
	l = new_list(10);
	k = 10;

	srand(time(NULL));
	i = k;

	printf("---------------\n");
	while (i > 0) {
		int key;
		key = rand() % k;
		insert(l, key, ptr);
		//	show_list(l);
		i--;
	}
	i = k ;
	show_list(l);
	while (i > 0) {
		int key;
		void *p;
		key = rand() % k;
		p = find(l, key);
		if (p == NULL) {
			printf("key %d not found\n", key);
		}
		else {
			delete(l, key);
			printf("key %d found\n", key);
		}
		i--;
	}
	printf("\n--------------\n");
	show_list(l);
	destroy(l);
	return 0;
}

