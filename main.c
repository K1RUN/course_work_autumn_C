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

int scan_txt(char ***txt){ //TODO
    int len = BUF;
    int n = 0;
    *txt = malloc(len * sizeof(char*));
    while(!scan_sentence(&((*txt)[n]))){
        if (n == len){
            char **tmp = realloc(*txt, len + BUF);
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
//                printf("%s %s\n", *(*txt + i), *(*txt + j));
                if(*(*txt + i) != NULL && *(*txt + j) != NULL) {
                    if (strlen(*(*txt + i)) != strlen(*(*txt + j))) {
                        continue;
                    } else {
                        for (int k = 0; k < strlen(*(*txt + i)); k++) { // посимвольно проверяем два предложения
                            if (tolower(*(*(*txt + i) + k)) != tolower(*(*(*txt + j) + k))) {
                                flag = 0;
                            }
                        }
                        if (flag == 1) {
                            free(*(*txt + j));
                            *(*txt + j) = NULL; //???? прокатит ли? TODO?
                            new_n--;
                        }
                        flag = 1;
                    }
                }
            }
        }
        del_null_pointers(txt, n);
        return new_n;
    }
    return -1;
}

void del_digit(char **sentence){ //удаляет цифры из предложения TODO: сделать применение функции ко всему тексту
    if (sentence) {
        for (int i = 0; i < strlen(*sentence); i++) {
            if (isdigit((*sentence)[i])) {
                memmove(*sentence + i, *sentence + i + 1, strlen(*sentence + i));
                i--;
            }
        }
    }
}

int is_palindrome(char **sentence){ //TODO: сделать прменение функции ко всему тексту
    if (sentence) {
        for (int i = 0; i < strlen(*sentence); i++) {
            if (isspace((*sentence)[i]) || (*sentence)[i] == ',') {
                memmove(*sentence + i, *sentence + i + 1, strlen(*sentence + i));
                i--;
            }
        }
        char *ptr_start = *sentence; /*Указатель на начало строки*/
        char *ptr_end = strchr(*sentence, '.') - 1; /*Присвоить указатель на конец предложения, НЕ учитывая точку*/
        while (ptr_start <= ptr_end) {
            if (*ptr_start != *ptr_end) {
                return 0;
            }
            ptr_start++;
            ptr_end--;
        }
        return 1;
    }
    return -1;
}

int main(){
//    char *sentence;
//    scan_sentence(&sentence);
//    puts(sentence);
//    del_digit(&sentence);
//    puts(sentence);
//    printf("%d", is_palindrome(&sentence));
    char **text;
    int n = scan_txt(&text);
//    for(int i = 0; i < n; i++){
//        printf("%s", text[i]);
//    }
    n = check_txt(&text, n);
    for(int i = 0; i < n; i++){
        printf("%s", text[i]);
    }
//    printf("%d ", n);
}