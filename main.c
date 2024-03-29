#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#define BUF 50

int scan_sentence(char **str){
    int len = BUF;
    int n = 0;
    int count = 0; //счетчик для \n
    char c;
    *str = malloc(len * sizeof(char));
    do{
        c = (char)getchar();
        (*str)[n] = c;
        if (c == '\n'){
            count++;
        } else {
            count = 0;
        }
        n++;
        if (n == len - 1){
            char *tmp = realloc(*str, len + BUF);
            if(tmp){
                *str = tmp;
                len += BUF;
            } else {
                fprintf(stderr, "Error while allocating memory!\n");
                free(*str);
                return -1;
            }
        }
    } while(c != '.' && count != 2);
    (*str)[n] = '\0';
    if(count == 2){ //дважды встретили \n - конец текста
        return 1; //ввод закончился
    }
    return 0;
}

void free_txt(char **txt, int n){
    for(int i = 0; i < n; i++){
        free(txt[i]);
    }
    free(txt);
}

int scan_txt(char ***txt){
    int len = BUF;
    int n = 0;
    *txt = malloc(len * sizeof(char*));
    while(!scan_sentence(*txt + n)){
        if (n == len - 1){
            char **tmp = realloc(*txt, sizeof(char*) * (len + BUF));
            if(tmp){
                *txt = tmp;
                len += BUF;
            } else {
                fprintf(stderr, "Error while allocating memory!\n");
                free_txt(*txt, n);
                return -1;
            }
        }
        n++;
    }
    return n;
}

void del_null_pointers(char ***txt, int n){
    if(txt){
        for (int i = 0; i < n; i++){
            if ((*txt)[i] == NULL){
                memmove((*txt + i), (*txt + i + 1), (n - i) * sizeof(char *));
                i--;
            }
        }
    }
}

int check_txt(char ***txt, int n){
    if(txt){
        int flag = 1;
        int new_n = n; // новое число предложений
        for (int i = 0; i < n; i++){
            for (int j = i + 1; j < n; j++){ // проверяем i-ое предложение с j-ым
                if (*(*txt + i) != NULL && *(*txt + j) != NULL) {
                    if (strcasecmp(*(*txt + i), *(*txt + j)) != 0) {
                        flag = 0;
                    }
                    if (flag) {
                        free(*(*txt + j));
                        *(*txt + j) = NULL;
                        new_n--;
                    }
                    flag = 1;
                }
            }
        }
        del_null_pointers(txt, n);
        return new_n;
    }
    return -1;
}

int del_equal_char(char ***txt, int n){
    if(txt){
        int flag = 1;
        int new_n = n; // новое число предложений
        for (int i = 0; i < n; i++) {
            if ((strlen(*(*txt + i))) > 1) {
                if (tolower(*(*(*txt + i))) != tolower(*(*(*txt + i) + strlen((*(*txt + i))) - 2))) {
                    flag = 0;
                }
                if (flag) {
                    free(*(*txt + i));
                    *(*txt + i) = NULL;
                    new_n--;
                }
                flag = 1;
            }
        }
        del_null_pointers(txt, n);
        return new_n;
    }
    return -1;
}

void del_equal_words(char **sentence){
    if (sentence) {
        int flag = 0;
        char *token;
        char *copy = (char *) malloc(strlen(*sentence) + 1);
        strcpy(copy, *sentence);
        token = strtok(copy, " ,");
        while (token != NULL) {
            for (int i = 0; i < strlen(*sentence); i++) {
                if ((*sentence)[i] == ' ' || (*sentence)[i] == ',') {
                    continue;
                } else if (!strncmp(*sentence + i, token, strlen(token)) &&
                        ((i != 0 &&
                          (((*sentence)[i - 1] == ' ' ||
                            (*sentence)[i - 1] == '.'
                            || (*sentence)[i - 1] == ',')
                           && ((*sentence)[i + (int)strlen(token)] == ' '
                               || (*sentence)[i + (int)strlen(token)] == '.'
                               || (*sentence)[i + (int)strlen(token)] == ','))) || (i == 0 &&
                               ((*sentence)[i + (int)strlen(token)] == ' '
                               || (*sentence)[i + (int)strlen(token)] == '.'
                               || (*sentence)[i + (int)strlen(token)] == ',')))
                    ) {
                    flag += 1;
                }
            }
            for (int i = 0; i < strlen(*sentence); i++) {
                if ((*sentence)[i] == ' ' || (*sentence)[i] == ',') {
                    continue;
                } else if (!strncmp(*sentence + i, token, strlen(token)) &&
                        ((i != 0 &&
                        (((*sentence)[i - 1] == ' ' ||
                        (*sentence)[i - 1] == '.'
                        || (*sentence)[i - 1] == ',')
                        && ((*sentence)[i + (int)strlen(token)] == ' '
                        || (*sentence)[i + (int)strlen(token)] == '.'
                        || (*sentence)[i + (int)strlen(token)] == ','))) || (i == 0 &&
                        ((*sentence)[i + (int)strlen(token)] == ' '
                        || (*sentence)[i + (int)strlen(token)] == '.'
                        || (*sentence)[i + (int)strlen(token)] == ','))) && flag > 1) {
                    memmove(*sentence + i, *sentence + i + strlen(token), strlen(*sentence + i + strlen(token)));
                }
            }
            token = strtok(NULL, " ,");
            flag = 0;
            *(strchr(*sentence, '.') + 1) = '\0';
        }
        free(copy);
    }
}

void del_digits(char **sentence){
    if (sentence) {
        for (int i = 0; i < strlen(*sentence); i++) {
            if (isdigit((*sentence)[i])) {
                memmove(*sentence + i, *sentence + i + 1, strlen(*sentence + i));
                i--;
            }
        }
    }
}

int third_word_len(char *sentence){
    int len = 3;
    char *token;
    char *copy = (char *)malloc(strlen(sentence) + 1);
    int counter = 0; //номер слова
    strcpy(copy, sentence);
    token = strtok(copy, " ,.");
    while(token != NULL){
        if(counter == 2){
            len = strlen(token);
            break;
        }
        counter++;
        token = strtok(NULL, " ,."); //ищем третье слово
    }
    free(copy);
    return len;
}

int cmp(const void *a, const void *b){
    char **first = (char**)a;
    char **second = (char**)b;
    return third_word_len(*first) - third_word_len(*second);
}

int is_palindrome(char **sentence){
    if (sentence) {
        if(strlen(*sentence) > 1) {
            char *ptr_start = *sentence; /*Указатель на начало строки*/
            char *ptr_end = strchr(*sentence, '.') - 1; /*Присвоить указатель на конец предложения, НЕ учитывая точку*/
            char *cpy_ptr_start = *sentence;
            char *cpy_ptr_end = strchr(*sentence, '.') - 1;
            while (cpy_ptr_start <= cpy_ptr_end) {
                while ((cpy_ptr_start != ptr_end && cpy_ptr_end != ptr_start) &&
                       (*cpy_ptr_start == ' ' || *cpy_ptr_start == ',' || *cpy_ptr_end == ' ' || *cpy_ptr_end == ',')) {
                    if (*cpy_ptr_start == ' ' || *cpy_ptr_start == ',') {
                        cpy_ptr_start++;
                    } else if (*cpy_ptr_end == ' ' || *cpy_ptr_end == ',') {
                        cpy_ptr_end--;
                    }
                }
                if (*cpy_ptr_start != *cpy_ptr_end) {
                    return 0;
                }
                cpy_ptr_start++;
                cpy_ptr_end--;
            }
        }
        return 1;
    }
    return -1;
}

void print_out(char ***txt, int n){
    for(int i = 0; i < n; i++){
        printf("%s", (*txt)[i]);
    }
}

int what_to_do(char ***txt, int n)  {
    int num = n;
    while(1) {
        if(num == 0){
            printf("Text consists of 0 sentences now. Terminating...\n");
            return num;
        }
        printf("\nWhat to do?\n1 - delete digits in every sentence.\n2 - find palindromes in text.\n"
               "3 - delete sentences, where the first char is equal to the last.\n"
               "4 - sort sentences by the length of the third word.\n5 - delete all words, which occur more than once.\n"
               "6 - exit.\n");
        char a = (char)getchar();
        while(a == '\n'){
            a = (char)getchar();
        }
        switch(a) {
            case '1': {
                for (int i = 0; i < num; i++) {
                    del_digits(*txt + i);
                }
                print_out(txt, num);
                break;
            }
            case '2': {
                for (int i = 0; i < num; i++) {
                    if (is_palindrome(*txt + i)) {
                        printf("%d Palindrome\n", i + 1);
                    } else {
                        printf("%d Nothing interesting\n", i + 1);
                    }
                }
                print_out(txt, num);
                break;
            }
            case '3': {
                num = del_equal_char(txt, num);
                print_out(txt, num);
                break;
            }
            case '4': {
                qsort(*txt, num, sizeof(char *), cmp);
                print_out(txt, num);
                break;
            }
            case '5': {
                for (int i = 0; i < num; i++) {
                    del_equal_words(*txt + i);
                }
                print_out(txt, num);
                break;
            }
            case '6': {
                return num;
            }
            default: {
                printf("Got wrong instruction. Please, try again.\n");
            }
        }
    }
}

int main(){
    char **text;
    int n = scan_txt(&text);
    n = check_txt(&text, n);
    n = what_to_do(&text, n);
    free_txt(text, n + 1);
}