// Copyright 2021-2022 Povarna Andreea 314CA
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ll.h"
#include "ht.h"

#define MAX_STRING_SIZE	256
#define HMAX 100
#define SCANF_WHOLE(str) scanf(" \42%[^\42]\42", str);

typedef struct BOOK
{
    char book_name[40];
    double rating;
    int purchase;
	int reachable;
    hashtable_t *perechi_def;
} BOOK;

typedef struct USER {
	char user_name[20];
	int score;
	char *borr_book_name;
	int allowed;
	int days_available;
	BOOK *book;
}USER;

void ADD_BOOK(hashtable_t *LIBRARY) {
    char def_key[20];
    char def_value[20];
    char book_name[41];
    int nr_definitions = 0;

	// buffer in care citim toata linia, respectiv, un cuvant
	char* buffer = malloc(sizeof(char) * 100);
	char* word_buffer = malloc(41 * sizeof(char));
	// citim toata linia

	fgets(buffer, 60, stdin);
	// offset fata de inceputul bufferului
	int offset = 0;

	// citim spatiul si prima ghilimea
	sscanf(buffer + offset, "%c", &word_buffer[0]);
	offset += 1;
	sscanf(buffer + offset, "%c", &word_buffer[0]);
	offset += 1;

	// citim ceea ce este intre ghilimele
	book_name[0] = '\0';
	do {
		sscanf(buffer + offset, "%s", word_buffer);
		strcat(book_name, word_buffer);
		strcat(book_name, " ");
		offset += strlen(word_buffer) + 1;
	} while (word_buffer[strlen(word_buffer) - 1] != '\"');

	// scoatem ultima ghilimea din nume
	book_name[strlen(book_name) - 2] = '\0';

	// citim numarul de definitii
	sscanf(buffer + offset, "%d", &nr_definitions);

	free(buffer);
	free(word_buffer);

	BOOK carte;

	carte.purchase = 0;
	carte.rating = 0;
	// 0 - reachable, 1 - unreachable
	carte.reachable = 0;

	carte.perechi_def = ht_create(9, hash_function_string,
	compare_function_strings);

	for(int i = 0 ; i < nr_definitions; i++) {
		scanf("%s %s", def_key, def_value);
		ht_put(carte.perechi_def, def_key, strlen(def_key) + 1,
		def_value, strlen(def_value) + 1);
	}

	strcpy(carte.book_name, book_name);

	ht_put(LIBRARY, book_name, strlen(book_name) + 1, &carte, sizeof(BOOK));
}

void GET_BOOK(hashtable_t* LIBRARY) {
	// double rating;
	char book_name[40];
	// buffer in care citim toata linia, respectiv, un cuvant
	char* buffer = malloc(sizeof(char) * 60);
	char* word_buffer = malloc(40 * sizeof(char));
	// citim toata linia

	fgets(buffer, 60, stdin);
	// offset fata de inceputul bufferului
	int offset = 0;

	// citim spatiul si prima ghilimea
	sscanf(buffer + offset, "%c", &word_buffer[0]);
	offset += 1;
	sscanf(buffer + offset, "%c", &word_buffer[0]);
	offset += 1;

	// citim ceea ce este intre ghilimele
	book_name[0] = '\0';
	do {
		sscanf(buffer + offset, "%s", word_buffer);
		strcat(book_name, word_buffer);
		strcat(book_name, " ");
		offset += strlen(word_buffer) + 1;
	} while (word_buffer[strlen(word_buffer) - 1] != '\"');

	// scoatem ultima ghilimea din nume
	book_name[strlen(book_name) - 2] = '\0';

	free(buffer);
	free(word_buffer);

	BOOK *carte = (BOOK*)ht_get(LIBRARY, book_name);

	if(!ht_has_key(LIBRARY, book_name)) {
		printf("The book is not in the library.\n");
		return;
	}

	if (carte->purchase == 0) {
		carte->rating = 0;
	} else {
		carte->rating = carte->rating / carte->purchase;
	}

	// hashtable_t *def = (hashtable_t*)carte->perechi_def;
	printf("Name:%s Rating:%.3lf Purchases:%d\n", carte->book_name,
	carte->rating, carte->purchase);
}

void RMV_BOOK(hashtable_t *LIBRARY) {
	char book_name[40];
	// buffer in care citim toata linia, respectiv, un cuvant
	char* buffer = malloc(sizeof(char) * 60);
	char* word_buffer = malloc(40 * sizeof(char));
	// citim toata linia

	fgets(buffer, 60, stdin);
	// offset fata de inceputul bufferului
	int offset = 0;

	// citim spatiul si prima ghilimea
	sscanf(buffer + offset, "%c", &word_buffer[0]);
	offset += 1;
	sscanf(buffer + offset, "%c", &word_buffer[0]);
	offset += 1;

	// citim ceea ce este intre ghilimele
	book_name[0] = '\0';
	do {
		sscanf(buffer + offset, "%s", word_buffer);
		strcat(book_name, word_buffer);
		strcat(book_name, " ");
		offset += strlen(word_buffer) + 1;
	} while (word_buffer[strlen(word_buffer) - 1] != '\"');

	// scoatem ultima ghilimea din nume
	book_name[strlen(book_name) - 2] = '\0';

	free(buffer);
	free(word_buffer);

	if(!ht_has_key(LIBRARY, book_name)) {
		printf("The book is not in the library.\n");
		return;
	}

	BOOK *book = (BOOK *)ht_get(LIBRARY, book_name);

	ht_free(book->perechi_def);
	ht_remove_entry(LIBRARY, book_name);
}

void ADD_DEF(hashtable_t *LIBRARY) {
	char book_name[40];
	char def_key[20];
	char def_value[20];

	// buffer in care citim toata linia, respectiv, un cuvant
	char* buffer = malloc(sizeof(char) * 60);
	char* word_buffer = malloc(40 * sizeof(char));
	// citim toata linia

	fgets(buffer, 60, stdin);
	// offset fata de inceputul bufferului
	int offset = 0;

	// citim spatiul si prima ghilimea
	sscanf(buffer + offset, "%c", &word_buffer[0]);
	offset += 1;
	sscanf(buffer + offset, "%c", &word_buffer[0]);
	offset += 1;

	// citim ceea ce este intre ghilimele
	book_name[0] = '\0';
	do {
		sscanf(buffer + offset, "%s", word_buffer);
		strcat(book_name, word_buffer);
		strcat(book_name, " ");
		offset += strlen(word_buffer) + 1;
	} while (word_buffer[strlen(word_buffer) - 1] != '\"');

	// scoatem ultima ghilimea din nume
	book_name[strlen(book_name) - 2] = '\0';

	sscanf(buffer + offset, "%s %s", def_key, def_value);

	free(buffer);
	free(word_buffer);

	BOOK *book = (BOOK *)ht_get(LIBRARY, book_name);

	if(!ht_has_key(LIBRARY, book_name)) {
		printf("The book is not in the library.\n");
		return;
	}
	ht_put(book->perechi_def, def_key, strlen(def_key) + 1,
						def_value, strlen(def_value) + 1);
}

void GET_DEF(hashtable_t *LIBRARY) {
	char book_name[40];
	char def_key[40];

	// buffer in care citim toata linia, respectiv, un cuvant
	char* buffer = malloc(sizeof(char) * 60);
	char* word_buffer = malloc(40 * sizeof(char));
	// citim toata linia

	fgets(buffer, 60, stdin);
	// offset fata de inceputul bufferului
	int offset = 0;

	// citim spatiul si prima ghilimea
	sscanf(buffer + offset, "%c", &word_buffer[0]);
	offset += 1;
	sscanf(buffer + offset, "%c", &word_buffer[0]);
	offset += 1;

	// citim ceea ce este intre ghilimele
	book_name[0] = '\0';
	do {
		sscanf(buffer + offset, "%s", word_buffer);
		strcat(book_name, word_buffer);
		strcat(book_name, " ");
		offset += strlen(word_buffer) + 1;
	} while (word_buffer[strlen(word_buffer) - 1] != '\"');

	// scoatem ultima ghilimea din nume
	book_name[strlen(book_name) - 2] = '\0';

	sscanf(buffer + offset, "%s", def_key);

	free(buffer);
	free(word_buffer);

	if(!ht_has_key(LIBRARY, book_name)) {
		printf("The book is not in the library.\n");
		return;
	}

	BOOK *book = (BOOK *)ht_get(LIBRARY, book_name);
	hashtable_t *definition = (hashtable_t*)book->perechi_def;

	if(!ht_has_key(definition, def_key)) {
		printf("The definition is not in the book.\n");
		return;
	}

	char* definitia = (char*)ht_get(definition, def_key);
	puts(definitia);
}


void RMV_DEF(hashtable_t *LIBRARY) {
	char book_name[40];
	char def_key[40];

	// buffer in care citim toata linia, respectiv, un cuvant
	char* buffer = malloc(sizeof(char) * 60);
	char* word_buffer = malloc(40 * sizeof(char));
	// citim toata linia

	fgets(buffer, 60, stdin);
	// offset fata de inceputul bufferului
	int offset = 0;

	// citim spatiul si prima ghilimea
	sscanf(buffer + offset, "%c", &word_buffer[0]);
	offset += 1;
	sscanf(buffer + offset, "%c", &word_buffer[0]);
	offset += 1;

	// citim ceea ce este intre ghilimele
	book_name[0] = '\0';
	do {
		sscanf(buffer + offset, "%s", word_buffer);
		strcat(book_name, word_buffer);
		strcat(book_name, " ");
		offset += strlen(word_buffer) + 1;
	} while (word_buffer[strlen(word_buffer) - 1] != '\"');

	// scoatem ultima ghilimea din nume
	book_name[strlen(book_name) - 2] = '\0';

	sscanf(buffer + offset, "%s", def_key);

	free(buffer);
	free(word_buffer);

	if(!ht_has_key(LIBRARY, book_name)) {
		printf("The book is not in the library.\n");
		return;
	}

	BOOK *book = (BOOK *)ht_get(LIBRARY, book_name);
	hashtable_t* definition = (hashtable_t *)book->perechi_def;

	if(ht_has_key(definition, def_key)){
		ht_remove_entry(definition, def_key);
	} else {
		printf("The definition is not in the book.\n");
	}
}

void EXIT_OP(hashtable_t *LIBRARY, hashtable_t *USERS)
{
	for (uint i = 0; i < LIBRARY->hmax; i++) {
		linked_list_t *ll_tmp = LIBRARY->buckets[i];
		ll_node_t *ll_node = ll_tmp->head;
		while (ll_node) {
			info *book_info = ll_node->data;
			BOOK *book = (BOOK *)book_info->value;
			free(book_info->key);
			ht_free(book->perechi_def);
			free(book_info->value);
			ll_node = ll_node->next;
		}
		ll_free(&LIBRARY->buckets[i]);
	}
	free(LIBRARY->buckets);
	free(LIBRARY);
	ht_free(USERS);
}

void ADD_USER(hashtable_t *USERS)
{
	USER user;
	user.score = 0;
	user.allowed = 0;
	user.borr_book_name = NULL;
	user.days_available = 0;

	scanf("%s", user.user_name);

	if(ht_has_key(USERS, user.user_name)) {
		printf("User is already registred.\n");
		return;
	}

	ht_put(USERS, user.user_name, strlen(user.user_name) + 1, &user, sizeof(USER));
}

void BORROW(hashtable_t *LIBRARY, hashtable_t* USERS)
{
	char user_name[20];
	scanf("%s", user_name);
	char book_name[40];
	SCANF_WHOLE(book_name);
	int days_available;
	scanf("%d", &days_available);

	USER *verify_user = (USER *)ht_get(USERS, user_name);

	if(verify_user == NULL) {
		printf("You are not registered yet.\n");
		return;
	}

	if(verify_user->allowed == 1) {
		printf("You are banned from this library.\n");
		return;
	}

	if(verify_user->borr_book_name != NULL) {
		printf("You have already borrowed a book.\n");
		return;
	}

	BOOK *book = (BOOK *)ht_get(LIBRARY, book_name);

	if(book == NULL) {
		printf("The book is not in the library.\n");
		return;
	}

	if(book->reachable == 1) {
		printf("The book is borrowed.\n");
		return;
	}

	book->reachable = 1;
	verify_user->borr_book_name = book->book_name;
	verify_user->days_available = days_available;
}



void TOP_BOOKS(hashtable_t *LIBRARY)
{
	printf("Books ranking:\n");
	int ok = 0;
	int index = 1;

	if (LIBRARY->size == 0)
		return;

	while (ok == 0) {
		// presupunem ca am trecut deja prin toate cartile
		ok = 1;
		// luam un pointer catre cartea top curenta
		BOOK* max_book = NULL;
		for (uint i = 0; i < LIBRARY->hmax; i++) {
			linked_list_t* buck = LIBRARY->buckets[i];
			ll_node_t* node = buck->head;
			for (uint j = 0; j < buck->size; j++) {
				info* data = node->data;
				BOOK* book = data->value;

				if (book == NULL) {
					puts("Oops.. Something went wrong..\n");
					exit(1);
				}

				// verificam daca mai sunt carti prin care nu s-a trecut
				if (book->purchase != -1)
					ok = 0;

				// daca e prima iteratie, luam direct prima carte
				if (max_book == NULL) {
					max_book = book;
					node = node->next;
					continue;
				}
				// verificam daca cartea a fost deja afisata
				if (book->purchase == -1) {
					node = node->next;
					continue;
				}

				// comparam cartea curenta cu cea top in toate modurile
				if (book->rating > max_book->rating)
					max_book = book;
				else if (book->rating == max_book->rating &&
				book->purchase > max_book->purchase)
					max_book = book;
				else if (book->rating == max_book->rating &&
				book->purchase == max_book->purchase &&
				strcmp(max_book->book_name, book->book_name) > 0)
					max_book = book;
				// continuam la urmatoarea carte
				node = node->next;
			}
		}
		// afisam cartea
		if (max_book != NULL && max_book->purchase != -1) {
			printf("%d. Name:%s Rating:%.3lf Purchases:%d\n", index,
			max_book->book_name, max_book->rating, max_book->purchase);
			index++;
		}
		// marcam cartea ca fiind afisata
		max_book->purchase = -1;
	}
}

int main() {
	char command[50];
	hashtable_t *LIBRARY = ht_create(9, hash_function_string,
								compare_function_strings);
	hashtable_t *USERS = ht_create(9, hash_function_string,
								compare_function_strings);

	while (1)
	{
		scanf("%s", command);
		if(strcmp(command, "ADD_BOOK") == 0) {
			ADD_BOOK(LIBRARY);
		}

		if(strcmp(command, "GET_BOOK") == 0) {
			GET_BOOK(LIBRARY);
		}

		if(strcmp(command, "RMV_BOOK") == 0) {
			RMV_BOOK(LIBRARY);
		}

		if(strcmp(command, "ADD_DEF") == 0) {
			ADD_DEF(LIBRARY);
		}

		if(strcmp(command, "GET_DEF") == 0) {
			GET_DEF(LIBRARY);
		}

		if(strcmp(command, "RMV_DEF") == 0) {
			RMV_DEF(LIBRARY);
		}

		if(strcmp(command, "ADD_USER") == 0) {
			ADD_USER(USERS);
		}

		if(strcmp(command, "BORROW") == 0) {
			BORROW(LIBRARY, USERS);
		}

		if(strcmp(command, "EXIT") == 0){
			TOP_BOOKS(LIBRARY);
			printf("Users ranking:\n");
			EXIT_OP(LIBRARY, USERS);
			break;
		}
	}



	return 0;
}
