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

/*Create a new Reservation.*/
Reservation *res_create(Date *res_date, char *res_code, int res_passenger_count ) {
	Reservation *new_node = malloc(sizeof(Reservation));
	if (new_node == NULL) {
		handle_oom();
	}
	new_node->date = *res_date;
	new_node->code = strdup(res_code);
	new_node->passenger_count = res_passenger_count;
	new_node->next = NULL;
	if (new_node->code == NULL) {
		handle_oom();
		/* free(new_head); */
		/* return NULL; */
	}

	return new_node;
}

/*Push an existing Reservation into the list, becoming the new head.*/
Reservation *res_push(Reservation *head, Reservation *new_node) {
	new_node->next = head;
	return new_node;
}

/*Insert existing Reservation into list in a sorted manner.*/
/*Returns lists' new head
 * (which only changes if new node's code is smaller than head's*/
Reservation* res_insert(Reservation *head, Reservation *new_node) {
	Reservation *current, *prev;

	/* fprintf(stderr, "VALGRIND DEBUG: head->code == NULL = %d\n", head->code == NULL); */

	if (head == NULL || strcmp(new_node->code, head->code) < 0) {
		new_node->next = head;
		return new_node;
	}

	current = head;
	prev = NULL;
	/* Locate first node in list "smaller" (in strcmp) than our current node */
	while (current->next != NULL && strcmp(new_node->code, current->code) > 0) {
		/*new_node->code > current->code*/
			fprintf(stderr, "DEBUG(res_insert): %s smaller than %s, continuing\n", current->code, new_node->code);
			prev = current;
			current = current->next;
	}
	if (strcmp(new_node->code, current->code) > 0) {
		fprintf(stderr, "DEBUG(res_push): Going to insert %s after %s\n", new_node->code, current->code);
		current->next = new_node;
	}
	else {
		prev->next = new_node;
		new_node->next = current;
	}

	return head;
}

void res_print(Reservation *head, Date date) {
	while (head != NULL) {
		if (compareDate(date, head->date) == 0) {
			printf("%s %d\n", head->code, head->passenger_count);
			head = head->next;

		}
	}
	return;
}

/*Destroys a linked list of reservations.*/
Reservation * res_destroy(Reservation *head) {
	Reservation *next;

	while (head != NULL) {
		/* fprintf(stderr, "%s", next->code); */
		next = head->next;
		free(head);
		head = next;
	}
	return NULL;
}

Reservation *res_pop(Reservation *head) {
	/*TODO: implement. maybe?*/
	return head;
}

/*Reservation hashtable functions*/
/*Reservation STsearch(unsigned long int id) {*/
/*	int i = hash(id, RES_PRIME);*/
/*	return searchList(heads[i], id);*/
/*}*/

/*void rt_insert(Reservation *res) {*/
/*	int i = hash(res->code, RES_PRIME);*/
/*	res_table[i] = res_push(res_table[i], res);*/
/*	res_table[i] = insertBeginList(heads[i], p); */
/*}*/

/*void STdelete(long id) {*/
/*	int i = hash(id, RES_PRIME);*/
/*	heads[i] = removeItemList(heads[i], id);*/
/*}*/

/*int res_hash(char *v) {*/
/*	int h = 0, a = 127;*/
/*	for (; *v != ’\0’; v++)*/
/*	h = (a*h + *v) % RES_PRIME;*/
/*	return h;*/
/*}*/
