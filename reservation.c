/*
	Rafael Girão - ist199309
	2nd IAED Project 2021/2022
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "proj.h"

extern Date system_date;

Reservation *res_create(Date *res_date, char *res_code, int res_passenger_count ) {
	Reservation *new_node = malloc(sizeof(Reservation));
	if (new_node == NULL) {
		handle_oom();
	}
	new_node->date = res_date;
	new_node->code = strdup(res_code);
	if (new_node->code == NULL) {
		handle_oom();
		/* free(new_head); */
		/* return NULL; */
	}
	new_node->passenger_count = res_passenger_count;

	new_node->next = NULL;

	return new_node;
}


/*Returns lists' new head
 * (which only changes if new node's code is smaller than head's*/
Reservation* res_insert(Reservation *head, Reservation *new_node) {
	Reservation *current;



	/* fprintf(stderr, "VALGRIND DEBUG: head->code == NULL = %d\n", head->code == NULL); */

	if (head == NULL || strcmp(new_node->code, head->code) < 0) {
		new_node->next = head;
		return new_node;
	}

	current = head;
	/* Locate first node in list "smaller" (in strcmp) than our current node */
	while (current->next != NULL && strcmp(new_node->code, current->code) > 0) {
		/*new_node->code > current->code*/
			fprintf(stderr, "DEBUG(res_push): %s smaller than %s, continuing\n", current->code, new_node->code);
			current = current->next;
	}
	fprintf(stderr, "DEBUG(res_push): Going to insert %s after %s\n", new_node->code, current->code);
	/*Insert in a sorted manner*/
	new_node->next = current->next;
	current->next = new_node;

	return head;
}

void res_print(Reservation *head) {
	while (head != NULL) {
		printf("%s %d\n", head->code, head->passenger_count);
		head = head->next;
	}
	return;
}

Reservation * res_destroy(Reservation *head) {
	/*TODO: implement*/
	return head;
}

Reservation *res_pop(Reservation *head) {
	/*TODO: implement. maybe?*/
	return head;
}
