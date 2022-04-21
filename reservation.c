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
	if (new_node->code == NULL) {
		handle_oom();
	}
	new_node->passenger_count = res_passenger_count;
	new_node->next = NULL;

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
	if (head == NULL || strcmp(new_node->code, head->code) < 0) {
		new_node->next = head;
		return new_node;
	}

	current = head;
	prev = NULL;
	/* Locate first node in list "smaller" (in strcmp) than our current node */
	while (current->next != NULL && strcmp(new_node->code, current->code) > 0) {
			prev = current;
			current = current->next;
	}
	if (strcmp(new_node->code, current->code) > 0) {
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
		next = head->next;
		free(head);
		head = next;
	}
	return NULL;
}
