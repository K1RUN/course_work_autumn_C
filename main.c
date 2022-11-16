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
    if(count == 2){
        return 1; // ввод закончился
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
        char *ptr_end = strchr(*sentence, '.') - 1;/*Присвоить указатель на конец предложения, НЕ учитывая точку*/
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
    for(int i = 0; i <= n; i++){
        printf("%s", text[i]);
    }
    printf("%d ", n);
}