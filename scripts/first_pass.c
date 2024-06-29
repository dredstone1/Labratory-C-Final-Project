#include "../header/first_pass.h"
#include <stdlib.h>

void format_line(char line[LINE_SIZE], word_list_block *current_line_word_block, symbol_table *table, int IC, error_array *error, int line_number);

void first_pass(file *file1, symbol_table *table, word_list_block *file_code_block, error_array *error){
	line_node *current_line = file1->first_line;
	word_list_block *current_line_word_block;
	int IC = 100, line_number = 0;

	while (current_line != NULL && error->importance != CRITICAL) {
		error->importance = NO_ERROR;
		current_line_word_block = create_new_word_list_block(error);
		if (error->importance != NO_ERROR) {
			line_number++;
			current_line = current_line->next;
			continue;
		}

		format_line(current_line->line_text.content, current_line_word_block, table, IC, error, line_number);
		if (error->importance != NO_ERROR) {
			free(current_line_word_block);
			line_number++;
			current_line = current_line->next;
			continue;
		}

		if (current_line_word_block->head != NULL) {
			IC += current_line_word_block->size;
			combine_word_list_blocks(file_code_block, current_line_word_block);
		}
		line_number++;
		current_line = current_line->next;
	}
}

void format_line(char line[LINE_SIZE], word_list_block *current_line_word_block, symbol_table *table, int IC, error_array *error, int line_number) {
	int offset = 0;
	symbol *symbol;
	line_data data;
	data.directive = NULL;
	data.command = NULL;
	symbol = get_symbol(line, &offset, error);
	
	if (symbol == NULL) {
		add_error(error, MEMORY_ALLOCATION_FAILED, 0, 0, 0, CRITICAL, "", 0);
		return;
	}

	line_data_set(&data, offset, line, symbol, error, line_number);
	if (error->importance != NO_ERROR) {
		free(symbol);
		return;
	}
	line_data_to_word_list_block(current_line_word_block, &data, error, line_number, line);


	if (symbol->label[0] != '\0') {
		if (data.directive != NULL && data.directive->type == EXTERN)
			add_symbol(table, symbol->label, error, 0, EXTERNAL);
		else
			add_symbol(table, symbol->label, error, IC, symbol->type);
	}
	free(symbol);
}