#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define  MAX_STRING_LEN 1000
#define  MAX_STRING_NUM 100
char *infile;
FILE *inptr;


size_t strings_count(void)  { // функция подсчета количества строк в файле
	int c = 0; 
	int endfile = EOF;
	size_t lines_count = 0; 
	while (!ferror(inptr) && !feof(inptr)) {
		c = fgetc(inptr);
		endfile = c;
		if (c == '\n') ++lines_count;
	}
	if (endfile != '\n' && endfile != EOF) ++lines_count;
	return lines_count;
}

void delete_punctuation(char** strings_array, size_t array_size) { // функция удаления знаков препинания
	char punct[7]=".,;:!?";
  	for (size_t i = 0; i < array_size; i++) {
		char* part; 
		char whole_string[MAX_STRING_LEN + 2];
		part = strtok(strings_array[i],punct);
		while (part != NULL) {
			strcat(whole_string, part);
	    	part = strtok(NULL,punct);
   		}
		strcpy(strings_array[i],whole_string);
		strcpy(whole_string,"");
	}	
}
 
void sort(char** strings_array, size_t array_size) {  // функция сортировки строк по убыванию
	char* tmp;
    char flag;
    do {
        flag = 0;
        for (size_t i = 1; i < array_size; i++) {
            if (strcmp(strings_array[i-1],strings_array[i])<0) {
                tmp = strings_array[i];
                strings_array[i] = strings_array[i-1];
                strings_array[i-1] = tmp;
                flag = 1;
            }
        }
    } while (flag);
}

int main(int argc, char* argv[]) {
	if (argc < 2) return 0;
    if (argc != 2) {
        printf("The needed command format: task7 input.txt\n");
        return 0;
    }
    // Чтение команды ввода с консоли
	infile = argv[1]; 
    inptr = fopen(infile, "r");   // Открытие файла на чтение
    if (inptr == NULL) {
        printf("Could not open %s.\n", infile);
        return -1;
    }
	size_t strings_number = strings_count();   // количество строк в файле
	rewind(inptr);
    // выделение памяти для массива, который будет содержать строки из файла и заполнение его
    char **strings_array = (char**)malloc(strings_number * (MAX_STRING_LEN+2)); //+2 (для fgets)возможные символы новой и нулевой строки
    for (size_t i = 0; i < strings_number; i++) {
        strings_array[i] = (char*)malloc(MAX_STRING_LEN+2);
        if (fgets(strings_array[i], MAX_STRING_LEN, inptr) == NULL)
			break; 
    }
    fclose(inptr);
	delete_punctuation(strings_array, strings_number);  // вызов функции удаления знаков препинания

	if (strings_number > 1)  { // пропустим сортировку, если строка всего одна
    	sort(strings_array, strings_number);  // вызов функции сортировки
	}
	size_t input_strings_number;
	if (strings_number > MAX_STRING_NUM) 
		input_strings_number = MAX_STRING_NUM;
	else
		input_strings_number = strings_number;

	for (size_t i = 0; i < input_strings_number; i++) {  // запись строк в поток вывода и освобождение памяти
		printf("%s", strings_array[i]);
		free(strings_array[i]);
    }

    free(strings_array);    

    return 0;
}
