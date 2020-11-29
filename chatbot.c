/*
 * ICT1002 (C Language) Group Project.
 *
 * This file implements the behaviour of the chatbot. The main entry point to
 * this module is the chatbot_main() function, which identifies the intent
 * using the chatbot_is_*() functions then invokes the matching chatbot_do_*()
 * function to carry out the intent.
 *
 * chatbot_main() and chatbot_do_*() have the same method signature, which
 * works as described here.
 *
 * Input parameters:
 *   inc      - the number of words in the question
 *   inv      - an array of pointers to each word in the question
 *   response - a buffer to receive the response
 *   n        - the size of the response buffer
 *
 * The first word indicates the intent. If the intent is not recognised, the
 * chatbot should respond with "I do not understand [intent]." or similar, and
 * ignore the rest of the input.
 *
 * If the second word may be a part of speech that makes sense for the intent.
 *    - for WHAT, WHERE and WHO, it may be "is" or "are".
 *    - for SAVE, it may be "as" or "to".
 *    - for LOAD, it may be "from".
 * The word is otherwise ignored and may be omitted.
 *
 * The remainder of the input (including the second word, if it is not one of the
 * above) is the entity.
 *
 * The chatbot's answer should be stored in the output buffer, and be no longer
 * than n characters long (you can use snprintf() to do this). The contents of
 * this buffer will be printed by the main loop.
 *
 * The behaviour of the other functions is described individually in a comment
 * immediately before the function declaration.
 *
 * You can rename the chatbot and the user by changing chatbot_botname() and
 * chatbot_username(), respectively. The main loop will print the strings
 * returned by these functions at the start of each line.
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "chat1002.h"
/*
 * ICT1002 (C Language) Group Project.
 *
 * This file implements the behaviour of the chatbot. The main entry point to
 * this module is the chatbot_main() function, which identifies the intent
 * using the chatbot_is_*() functions then invokes the matching chatbot_do_*()
 * function to carry out the intent.
 *
 * chatbot_main() and chatbot_do_*() have the same method signature, which
 * works as described here.
 *
 * Input parameters:
 *   inc      - the number of words in the question
 *   inv      - an array of pointers to each word in the question
 *   response - a buffer to receive the response
 *   n        - the size of the response buffer
 *
 * The first word indicates the intent. If the intent is not recognised, the
 * chatbot should respond with "I do not understand [intent]." or similar, and
 * ignore the rest of the input.
 *
 * If the second word may be a part of speech that makes sense for the intent.
 *    - for WHAT, WHERE and WHO, it may be "is" or "are".
 *    - for SAVE, it may be "as" or "to".
 *    - for LOAD, it may be "from".
 * The word is otherwise ignored and may be omitted.
 *
 * The remainder of the input (including the second word, if it is not one of the
 * above) is the entity.
 *
 * The chatbot's answer should be stored in the output buffer, and be no longer
 * than n characters long (you can use snprintf() to do this). The contents of
 * this buffer will be printed by the main loop.
 *
 * The behaviour of the other functions is described individually in a comment
 * immediately before the function declaration.
 *
 * You can rename the chatbot and the user by changing chatbot_botname() and
 * chatbot_username(), respectively. The main loop will print the strings
 * returned by these functions at the start of each line.
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "chat1002.h"

/*
 * Get the name of the chatbot.
 *
 * Returns: the name of the chatbot as a null-terminated string
 */
const char *chatbot_botname() {

	return "Jarvis";

}

/*
 * Get the name of the user.
 *
 * Returns: the name of the user as a null-terminated string
 */
const char *chatbot_username() {

	return "User";

}

/*
 * Get a response to user input.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0, if the chatbot should continue chatting
 *   1, if the chatbot should stop (i.e. it detected the EXIT intent)
 */
int chatbot_main(int inc, char *inv[], char *response, int n) {

	/* check for empty input */
	if (inc < 1) {
		snprintf(response, n, "");
		return 0;
	}

	/* look for an intent and invoke the corresponding do_* function */
	if (chatbot_is_exit(inv[0]))
		return chatbot_do_exit(inc, inv, response, n);
	else if (chatbot_is_smalltalk(inv[0]))
		return chatbot_do_smalltalk(inc, inv, response, n);
	else if (chatbot_is_load(inv[0]))
		return chatbot_do_load(inc, inv, response, n);
	else if (chatbot_is_question(inv[0]))
		return chatbot_do_question(inc, inv, response, n);
	else if (chatbot_is_reset(inv[0]))
		return chatbot_do_reset(inc, inv, response, n);
	else if (chatbot_is_save(inv[0]))
		return chatbot_do_save(inc, inv, response, n);
	else {
		snprintf(response, n, "I don't understand \"%s\".", inv[0]);
		return 0;
	}

}

/*
 * Determine whether an intent is EXIT.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "exit" or "quit"
 *  0, otherwise
 */
int chatbot_is_exit(const char *intent) {

	return compare_token(intent, "exit") == 0 || compare_token(intent, "quit") == 0 || compare_token(intent, "bye") == 0;

}

/*
 * Perform the EXIT intent.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after a question)
 */
int chatbot_do_exit(int inc, char *inv[], char *response, int n) {

	knowledge_reset();
	snprintf(response, n, "Goodbye!");

	return 1;

}

/*
 * Determine whether an intent is LOAD.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "load"
 *  0, otherwise
 */
int chatbot_is_load(const char *intent) {

	return compare_token(intent,"load") == 0;

}

/*
 * Load a chatbot's knowledge base from a file.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after loading knowledge)
 */
int chatbot_do_load(int inc, char *inv[], char *response, int n) {


	FILE * filePtr;
	int filepath = 1;
	char *extension_check="";
	int num_response;

	if (inc != 2) {
        snprintf(response, n, "Please specify a valid file to load.");
        return 0;
	}

	//Check if file extension is correct
	while(inv[filepath]!=NULL){
		if (strlen(inv[filepath])<4){
			filepath++;
			continue;
		}else{
			extension_check = &inv[filepath][strlen(inv[filepath])-4];
			if (compare_token(extension_check,".ini")!=0){

				filepath++;
				continue;
			}else{
				break;
			}
		}
	}

	//Open file
    filePtr = fopen(inv[filepath], "r");

	//Check if file exists
    if (filePtr != NULL) {
        num_response = knowledge_read(filePtr);
        fclose(filePtr);
		snprintf(response, n, "Read %d responses", num_response);
    } else {
		snprintf(response, n, "Unable to find file :(");
    }
    return 0;

}

/*
 * Determine whether an intent is a question.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "what", "where", or "who"
 *  0, otherwise
 */
int chatbot_is_question(const char *intent) {

	return compare_token(intent,"what") == 0 || compare_token(intent,"where") == 0 || compare_token(intent,"who") == 0;

}

/*
 * Answer a question.
 *
 * inv[0] contains the the question word.
 * inv[1] may contain "is" or "are"; if so, it is skipped.
 * The remainder of the words form the entity.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after a question)
 */
int chatbot_do_question(int inc, char *inv[], char *response, int n) {

    /* Check for the length of the input */

	if (inc < 2) {
		snprintf(response, n, "Please enter a proper input!");
		return 0;
	}

	/* Define a list of irrelevant words */
	char * filler_words[] = {"is", "are", "did", "do", "was", "were", "has", "have", "had", "the"};
	char * current_intent = inv[0];
	char entity[MAX_INPUT]  = "\0"; //Ensure it ends with \0

	/* Skip the irrelevant words */
	int i = 1; // index 0 will be intent.
	while (i < inc) {
		if (!compare_words(inv[i], filler_words, 10))
			break;
		i++;
	}

	/* Reject if no meaningful words found */
	if (i == (inc)) {
		snprintf(response, n, "Please enter a proper input!");
		return 0;
	}

	/* combine all the remaining words to form the entity */
	while (i < inc) {
		strcat(entity, inv[i]);
		strcat(entity, " "); //adding space after a each inv
		i++;
	}

    /* Preform searching on the input  */
    int search_response;
    char user_response[MAX_RESPONSE] = "";
	search_response = knowledge_get(current_intent, entity, response, n);

	//if not found
	if(search_response == KB_NOTFOUND){

		prompt_user(user_response, MAX_RESPONSE, "I do not know, please tell me %s is %s?", inv[0], entity);
        //if user want to skip
		if (compare_token(user_response,"")==0){
			snprintf(response, n, ":(");
		}else{

			//Check if answer was inserted into knowledge base successfully
			if (knowledge_put(inv[0], entity, user_response)==KB_OK){
				snprintf(response, n, "Ah! %s",user_response);
			}else{
				snprintf(response, n, "Something went wrong,unable to save :(");
			}
		}

	}else if (search_response == KB_INVALID) {
		snprintf(response, n, "I don't understand \"%s\".", inv[0]);
		return 0;
	}

	return 0;

}

/*
 * Determine whether an intent is RESET.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "reset"
 *  0, otherwise
 */
int chatbot_is_reset(const char *intent) {

	return compare_token(intent,"reset") == 0;
}

/*
 * Reset the chatbot.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after beign reset)
 */
int chatbot_do_reset(int inc, char *inv[], char *response, int n) {

	//Resets knowledge base
	knowledge_reset();

	snprintf(response, n, "Jarvis has been reset.");

	return 0;

}

/*
 * Determine whether an intent is SAVE.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "what", "where", or "who"
 *  0, otherwise
 */
int chatbot_is_save(const char *intent) {


	return compare_token(intent,"save") == 0;

}

/*
 * Save the chatbot's knowledge to a file.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after saving knowledge)
 */
int chatbot_do_save(int inc, char *inv[], char *response, int n) {

	FILE * filePtr;
	int filepath = 1;
	char *extension_check="";

	//Checks if file extension is correct
	while(inv[filepath]!=NULL){
		if (strlen(inv[filepath])<4){
			filepath++;
			continue;
		}else{
			extension_check = &inv[filepath][strlen(inv[filepath])-4];
			if (compare_token(extension_check,".ini")!=0){

				filepath++;
				continue;
			}else{
				break;
			}
		}
	}

    filePtr = fopen(inv[filepath], "w+");

	//Checks if file exists
    if (filePtr != NULL) {
        knowledge_write(filePtr);
        fclose(filePtr);
		snprintf(response, n, "All the data were saved to: %s", inv[filepath]);
    } else {
		snprintf(response, n, "Can't find file :(");
    }
    return 0;

}

/*
 * Determine which an intent is smalltalk.
 *
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is the first word of one of the smalltalk phrases
 *  0, otherwise
 */
int chatbot_is_smalltalk(const char *intent) {

	return compare_token(intent, "hello") == 0 ||
		compare_token(intent, "hi") == 0 ||
		compare_token(intent, "hey") == 0 ||

		compare_token(intent, "morning") == 0 ||
		compare_token(intent, "afternoon") == 0 ||
		compare_token(intent, "evening") == 0 ||

		compare_token(intent, "it's") == 0 ||

		compare_token(intent, "good") == 0 ||

		compare_token(intent, "thank") == 0 ||
		compare_token(intent, "thanks") == 0 ||

		compare_token(intent, "goodbye") == 0 ||
		compare_token(intent, "bye") == 0;

}

/*
 * Respond to smalltalk.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0, if the chatbot should continue chatting
 *   1, if the chatbot should stop chatting (e.g. the smalltalk was "goodbye" etc.)
 */
int chatbot_do_smalltalk(int inc, char *inv[], char *response, int n) {

	// simply echo the user input as response for these smalltalk topics
	if (compare_token(inv[0], "hello") == 0 || compare_token(inv[0], "hi") == 0 ||
		compare_token(inv[0], "hey") == 0 || compare_token(inv[0], "morning") == 0 ||
		compare_token(inv[0], "afternoon") == 0 || compare_token(inv[0], "evening") == 0)
	{
		snprintf(response, n, "%s.", inv[0]);
		return 0;
	}

	// we agree with any statement beginning with "it's" (as per project specs)
	else if (compare_token(inv[0], "it's") == 0)
	{
		snprintf(response, n, "Indeed it is.");
		return 0;
	}

	// we have three options: 1) second word is 'bot' or 'job', OR there is only one word 'good' -> praise
	//                        2) second word is 'morning', 'afternoon', 'evening', 'night' -> greeting
	//                        3) second word is 'bye' -> we should say goodbye and return 1 (exit).
	// else, the chatbot will respond that it does not understand.
	else if (compare_token(inv[0], "good") == 0)
	{
		if (inc > 1) {  // if there is more than one word, only then can we can access the next word.
			if (compare_token(inv[1], "bot") == 0 || compare_token(inv[1], "job") == 0)
			{
				snprintf(response, n, "Thank you :)");
				return 0;
			}
			else if (compare_token(inv[1], "morning") == 0 || compare_token(inv[1], "afternoon") == 0 ||
				compare_token(inv[1], "evening") == 0 || compare_token(inv[1], "night") == 0)
			{
				snprintf(response, n, "Good %s to you too!", inv[1]);
				return 0;
			}
			else if (compare_token(inv[1], "bye") == 0)  // if the second word is bye, then it does the same as
			{                                         // 'bye' and 'goodbye' below -> responds 'Goodbye!' and exits.
				snprintf(response, n, "Goodbye!");
				return 1;
			}
			else if (compare_token(inv[1], "day") == 0)  // if the second word is bye, then it does the same as
			{                                         // 'bye' and 'goodbye' below -> responds 'Goodbye!' and exits.
				snprintf(response, n, "Good day!");
				return 1;
			}
		}
		else if (inc == 1) {  // if we only have one word, then it is just 'good' -> praise
			snprintf(response, n, "Nice Jarvis.");
			return 0;
		}
		else {
			snprintf(response, n, "I don't understand.");
			return 0;
		}
	}

	// if first word is "thank" or "thanks", then we reply with "You're welcome, <username>! :)"
	else if (compare_token(inv[0], "thank") == 0 || compare_token(inv[0], "thanks") == 0)
	{
		snprintf(response, n, "You're welcome, %s! :)", chatbot_username());
		return 0;
	}
	else
	{
		snprintf(response, n, "ERROR in doing smalltalk!");
		return 0;
	}

}
