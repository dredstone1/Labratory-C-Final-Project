#include "../header/symbol_table.h"
#include "../header/line_data.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

int is_valid_symbol_name(char *symbol){
	if (!isalpha(*symbol) || strlen(symbol) > MAX_SYMBOL_SIZE)
		return 2;
	
	while (*(++symbol) != '\0' && (isdigit(*symbol) || isalpha(*symbol)));
	if (*symbol == ':')
		return 1;
	
	return 2;
}

int is_valid_symbol(char *symbol){
	int status;
	if (symbol[strlen(symbol)-1] != ':')
		return 0;
	
	symbol[strlen(symbol)] = '\0';
	status = is_valid_symbol_name(symbol);
	
	symbol[strlen(symbol)-1] = ':';
/*	symbol[strlen(symbol)] = ' ';*/

	return status;
}


int add_symbol(symbol_address **symbols, int *size, int address, int line_number, char symbol_name[], int is_data_line, error_array *error){
	*symbols = (symbol_address*) use_realloc(*symbols, (*size + 1) * sizeof(symbol_address), error);
	if (*symbols == NULL){
		add_error(error, MEMORY_ALLOCATION_FAILED, 0, 0, 0, CRITICAL, "", 0);
		return 0;
	}

	(*symbols)[(*size)].address = address;
	(*symbols)[(*size)].symbol_name = (char*)use_malloc((strlen(symbol_name) + 1)*sizeof(char), error);
	strcpy((*symbols)[(*size)].symbol_name, symbol_name);
	(*symbols)[(*size)].line_number = line_number;
	(*symbols)[(*size)].is_data_line = is_data_line;
	(*size)++;
	return 1;
}

int update_table_by(symbol_address *symbols, int by, int size, int is_data_line, error_array *error){
	int i;
	for (i = 0; i < size; i++) {
		if (symbols[i].is_data_line == is_data_line)
			symbols[i].address += by;
	}
	return 1;
}




/*
void add_symbol(symbol_table *table, char label[], error_array *error, int address, symbol_type type){
	table->size++;
	table->symbols = (symbol *)realloc(table->symbols, sizeof(symbol) * (table->size));
	if (table->symbols == NULL) {
		add_error(error, MEMORY_ALLOCATION_FAILED, 0, 0, 0, CRITICAL, "", 0);
		return;
	}

	table->symbols[table->size-1].type = type;

	strcpy(table->symbols[table->size-1].label, label);

	table->symbols[table->size-1].address = address;
}

void initialize_symbol_table(symbol_table *symbol_table_){
	symbol_table_->size = 0;
	symbol_table_->symbols = NULL;
}

symbol* get_symbol(char line[], int *offset, error_array *error){
	int i=0;
	char word[LINE_SIZE];
	symbol *symbol_obj = (symbol*)malloc(sizeof(symbol));
	
	if (symbol_obj == NULL) {
		add_error(error, MEMORY_ALLOCATION_FAILED, 0, 0, 0, CRITICAL, "", 0);
		return NULL;
	}

	symbol_obj->label[0] = '\0';
	get_next_word_n_skip(word, &i, line, " :\t \0", 4);
	if(line[i] != ':')
		return symbol_obj;

	strcpy(symbol_obj->label, word);
	symbol_obj->type = START;

	*offset += i+1;
	return symbol_obj;
}

symbol* get_symbol_address_from_symbol_name(symbol_table *table, char label[MAX_SYMBOL_SIZE]){
	int i;
	for (i = 0; i < table->size; i++) {
		if (table->symbols[i].type != ENTRY_ && strcmp(table->symbols[i].label, label) == 0)
			return &table->symbols[i];
	}
	
	return NULL;
}
*/
