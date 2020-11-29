/*
 * ICT1002 (C Language) Group Project.
 *
 * This file implements the chatbot's knowledge base.
 *
 * knowledge_get() retrieves the response to a question.
 * knowledge_put() inserts a new response to a question.
 * knowledge_read() reads the knowledge base from a file.
 * knowledge_reset() erases all of the knowledge.
 * knowledge_write() saves the knowledge base in a file.
 *
 * You may add helper functions as necessary.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "chat1002.h"

typedef struct node {
	char entity[MAX_ENTITY];
	char response[MAX_RESPONSE];
	struct node *next;
} NODE;

typedef NODE * ptr;

ptr what_head = NULL;
ptr where_head = NULL;
ptr who_head = NULL;

/*
 * Get the response to a question.
 *
 * Input:
 *   intent   - the question word
 *   entity   - the entity
 *   response - a buffer to receive the response
 *   n        - the maximum number of characters to write to the response buffer
 *
 * Returns:
 *   KB_OK, if a response was found for the intent and entity (the response is copied to the response buffer)
 *   KB_NOTFOUND, if no response could be found
 *   KB_INVALID, if 'intent' is not a recognised question word
 */
int knowledge_get(const char *intent, const char *entity, char *response, int n) {

    //set the pointer so it can be either what, where or who pointer.
	ptr pointer;

	//Retrieve the response by intent
	if (compare_token(intent,"what")==0){
		pointer = what_head;

	}else if (compare_token(intent,"where")==0){
		pointer = where_head;

	}
	//it is been verify by is_question() so only 3 intent, dont need compare.
	else{
		pointer = who_head;
	}

	//Search for the entity that meet input
	while (pointer!=NULL){
        if (compare_token(entity,pointer->entity)==0){
            strcpy(response, pointer->response);
            return KB_OK;
        }
		pointer = pointer->next;
	}
    //When nothing is found
	return KB_NOTFOUND;
}

/*
 * Insert a new response to a question. If a response already exists for the
 * given intent and entity, it will be overwritten. Otherwise, it will be added
 * to the knowledge base.
 *
 * Input:
 *   intent    - the question word
 *   entity    - the entity
 *   response  - the response for this question and entity
 *
 * Returns:
 *   KB_FOUND, if successful
 *   KB_NOMEM, if there was a memory allocation failure
 *   KB_INVALID, if the intent is not a valid question word
 */
int knowledge_put(const char *intent, const char *entity, const char *response) {

	//declare newnode and store the response from user
	ptr new_node = (ptr)malloc(sizeof(NODE));
	//ensure enough memory;
	if(new_node==NULL){
        return KB_NOMEM;
	}
	strcpy(new_node->entity, entity);
	strcpy(new_node->response, response);
	new_node->next=NULL;

	//linked each data according to its intent
	if (compare_token(intent,"what")==0){
        //when the head is empty
		if (what_head==NULL){
			what_head = new_node;
        //linked to front on the existing node
		}else{
			new_node->next = what_head;
			what_head = new_node;
		}
		//action complete
		return KB_OK;

	}else if (compare_token(intent,"where")==0){
		if (where_head==NULL){
			where_head = new_node;
		}else{
			new_node->next = where_head;
			where_head = new_node;
		}
		return KB_OK;

	}else if (compare_token(intent,"who")==0){
		if (who_head==NULL){
			who_head= new_node;
		}else{
			new_node->next = who_head;
			who_head = new_node;
		}

		return KB_OK;
	}
	return KB_INVALID;
}


/*
 * Read a knowledge base from a file.
 *
 * Input:
 *   f - the file
 *
 * Returns: the number of entity/response pairs successful read from the file
 */
int knowledge_read(FILE *f) {

	int counter = 0;
    char inputLine[MAX_INPUT];

    const size_t MAX_READLENGTH = (size_t)(MAX_ENTITY + MAX_RESPONSE);

    while (fgets(inputLine, MAX_READLENGTH, f)) {
        char *entity, *response, *intent;
        char *carriageReturn, *newLine;

        // Scan line by line
        if (strcmp(inputLine, "\n") == 0 || strcmp(inputLine, "\r\n") == 0) {
            // If empty line
            continue;
        } else {
            carriageReturn = strchr(inputLine, '\r');
            newLine = strchr(inputLine, '\n');

            if (carriageReturn != NULL) {
                // Carriage return found, replace it with null terminator
                *carriageReturn = '\0';
            } else if (newLine != NULL) {
                // newline found, replace it with null terminator
                *newLine = '\0';
            } else {
                // Clear any remaining input to prevent overflow
                int inputRemain;
                while ((inputRemain = getchar()) != '\n' && inputRemain != EOF) {
                    continue;
                }
            }
			if (strchr(inputLine, '[') != NULL && strchr(inputLine, ']') != NULL) {
                // When square brackets found. Check intent.
                if (compare_token(inputLine, "[what]") == 0) {
                    intent = "what";
                } else if (compare_token(inputLine, "[where]") == 0) {
                    intent = "where";
                } else if (compare_token(inputLine, "[who]") == 0) {
                    intent = "who";
                } else {
                    // Invalid intent
                    intent = NULL;
                }
            } else if (intent != NULL && (strchr(inputLine, '=') != NULL)) {
                // Grab entity and response
                entity = strtok(inputLine, "=");
                response = strtok(NULL, "=");

                if (knowledge_put(intent, entity, response) == KB_OK) {
                    // Increment the successful counter
                    counter++;
                }
            }
        }
    }

    return counter;
}

/*
 * Reset the knowledge base, removing all know entitities from all intents.
 */
void knowledge_reset() {

	ptr temp;

	//Free the what linked list
	while (what_head != NULL){
		temp = what_head;
		what_head = what_head->next;
		free(temp);
	}
	//Free the where linked list
	while (where_head != NULL){
		temp = where_head;
		where_head = where_head->next;
		free(temp);
	}
	//Free the who linked list
	while (who_head != NULL){
		temp = who_head;
		who_head = who_head->next;
		free(temp);
	}
}

/*
 * Write the knowledge base to a file.
 *
 * Input:
 *   f - the file
 */
void knowledge_write(FILE *f){

    // Write what linked list into the file
	ptr pointer = what_head;
    fprintf(f, "[WHAT]\n");
    while(pointer != NULL) {
        fprintf(f,"%s=%s\n", pointer->entity, pointer->response);
        pointer = pointer->next;
    }

	// Write where linked list into the file
	pointer = where_head;
	fprintf(f, "\n[WHERE]\n");
    while(pointer != NULL) {
        fprintf(f,"%s=%s\n", pointer->entity, pointer->response);
        pointer = pointer->next;
    }

	// Write who linked list into the file
	pointer = who_head;
	fprintf(f, "\n[WHO]\n");
    while(pointer != NULL) {
        fprintf(f,"%s=%s\n", pointer->entity, pointer->response);
        pointer = pointer->next;
    }
}
