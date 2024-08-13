#include "../header/utilities.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>


/**
 * @brief Checks if a character is a separator.
 *
 * This function checks if a given character is present in the provided string of separators.
 *
 * @param c The character to check.
 * @param separators A string containing characters considered as separators.
 *
 * @return 1 if the character is a separator, 0 otherwise.
 */
int is_separator(char c, char separators[]);


int int_to_octal(int decimal_number) {
	/* Initialize the octal result and the place value */
	int octal_result = 0, place_value = 1;

	/* Iterate until the decimal number becomes zero */
	while (decimal_number != 0) {
		/* Calculate the remainder when dividing by 8 (the octal base) */
		/* Multiply the remainder by the current place value and add it to the octal result */
		octal_result += (decimal_number % BASE) * place_value;
		
		/* Divide the decimal number by 8 to move to the next place value */
		decimal_number /= BASE;

		/* Multiply the place value by 8 to move to the next position */
		place_value *= PLACE_VALUE_MULTIPLIER;
	}

	/* Return the calculated octal result */
	return octal_result;
}


int count_commas_until_text(char **workable_line) {
	/* Initialize comma counter */
	int count = 0;

	/* Handle NULL string input and return 0 if the string is NULL */
	if (*workable_line == NULL) {
		return 0;
	}

	/* Iterate through the string until non-whitespace and non-comma is encountered */
	while (**workable_line == ' ' || **workable_line == '\t' || **workable_line == ',') {
		/* Increment the counter for each comma */
		if (**workable_line == ',') {
			count++;
		}

		/* Move the pointer to the next character */
		(*workable_line)++;
	}

	/* Return the total count of commas */
	return count;
}


int get_next_word(char **workable_line, char word[], char separators[]) {
	/* Initialize word index */
	int i = 0;

	/* Handle NULL string input */
	if (*workable_line == NULL) {
		return 0;
	}

	/* Copy characters to word until a separator or end of string is reached */
	while (is_separator(**workable_line, separators) == 0 && **workable_line != '\0') {
		/* Copy current character to word buffer */
		word[i++] = **workable_line;

		/* Move to the next character in the input string */
		(*workable_line)++;
	}
	
	/* Null-terminate the extracted word */
	word[i] = '\0';
	
	/* Return the length of the extracted word */
	return i;
}


int is_separator(char c, char separators[]) {
	int i;

	/* Iterate through the separators string */
	for (i = 0; i < strlen(separators); i++) {

		/* Check if the current character matches the separator */
		if (c == separators[i]) {
			/* Return 1 if a match is found */
			return 1;
		}
	}
	
	/* Return 0 if no match is found */
	return 0;
}


int is_empty_line(char *line) {
	skip_spaces_and_tabs(&line);

	return line == NULL || *line == '\0' || *line == '\n' || *line == '\r';
}


int is_comment_line(char line[]) {
	int offset = 0;
	skip_spaces_and_tabs_with_offset(&offset, line);

	return line[offset] == ';';
}


void skip_spaces_and_tabs_with_offset(int *offset, char line[]) {
	
	
	while (line[*offset] == ' ' || line[*offset] == '\t') {
		(*offset)++;
	}
}


void skip_spaces_and_tabs(char **line) {
	if (*line == NULL) {
		return;
	}
	while (**line == ' ' || **line == '\t') {
		(*line)++;
	}
}


void *use_malloc(size_t size, error *error) {
	void *ptr = (void *) malloc(size);
	if (ptr == NULL) {
		print_system_error(MEMORY_ALLOCATION_FAILED_MESSAGE, error, CRITICAL);
	}

	return ptr;
}


void *use_realloc(void *ptr, size_t size, error *error) {
	if (ptr == NULL) {
		ptr = use_malloc(size, error);
	} else {
		ptr = realloc(ptr, size);
		if (ptr == NULL) {
			print_system_error(MEMORY_ALLOCATION_FAILED_MESSAGE, error, CRITICAL);
		}
	}


	return ptr;
}


int is_register(const char word[]) {
	int offset = 0;

	if (word[0] == '*') {
		offset = 1;
	}

	if (word[offset] == 'r' && word[offset + 1] >= '0' && word[offset + 1] <= '9' && word[offset + 2] == '\0') {
		return 1;
	}

	return 0;
}


int get_line_from_file(char line[], int line_number, char file_name[], error *error) {
	FILE *file = fopen(file_name, "r");
	if (file == NULL) {
		print_system_error(FILE_NOT_FOUND_MESSAGE, error, CANCELLATION);
		return 0;
	}

	while (line_number > 0 && fgets(line, MAX_LINE_LENGTH, file) != NULL) {
		line_number--;
	}

	if (line[strlen(line) - 1] == '\n' || line[strlen(line) - 1] == '\r') {
		line[strlen(line) - 1] = '\0';
	}

	fclose(file);
	return 1;
}


void go_to_next_line(FILE *file, char line[], int line_number, int *current_line_number) {
	for (; *current_line_number < line_number; (*current_line_number)++) {
		fgets(line, MAX_LINE_LENGTH, file);
	}

	if (line[strlen(line) - 1] == '\n' || line[strlen(line) - 1] == '\r') {
		line[strlen(line) - 1] = '\0';
	}
}


saved_word_type is_a_saved_word(const char word[]) {
	if (get_opcode_from_string(word) != -1) {
		return SAVED_WORD_OPCODE;
	}

	if (get_directive_type_from_string(word) != -1) {
		return SAVED_WORD_DIRECTIVE;
	}

	if (is_register(word) == 1) {
		return SAVED_WORD_REGISTER;
	}

	if (strcmp(word, MACRO) == 0 || strcmp(word, END_MACRO) == 0) {
		return SAVED_WORD_MACRO;
	}

	return NON_SAVED_WORD;
}


directive_type get_directive_type_from_string(const char *str) {
	directive_type directive;

	for (directive = DATA; directive <= EXTERN; directive++) {
		if (strcmp(str, directive_type_names[directive]) == 0) {
			return directive;
		}
	}

	return -1;
}


opcode get_opcode_from_string(const char *str) {
	opcode code;

	for (code = MOV; code <= STOP; code++) {
		if (strcmp(str, opcode_names[code][0]) == 0) {
			return code;
		}
	}

	return -1;
}


char *duplicate_string(const char source[], error *error) {
	char *destination = (char *) use_malloc((strlen(source) + 1) * sizeof(char), error);
	if (destination == NULL) {
		return NULL;
	}

	strcpy(destination, source);
	return destination;
}


int is_line_too_long(const char line[]) {
	int i = 0;

	while (line[i] != '\0' && line[i] != '\n' && line[i] != '\r' && i < MAX_LINE_LENGTH) {
		i++;
	}

	return i < (MAX_LINE_LENGTH - 2);
}

void handle_free(void *ptr) {
	if (ptr != NULL) {
		free(ptr);
	}
}

char* str_last_char(char* str, const char c) {
	char *last = NULL;
	if (str == NULL || *str == '\0') {
		return NULL;
	}

	while (*str != '\0') {
		if (*str == c) {
			last = str;
		}
		
		str++;
	}

	return last;
}