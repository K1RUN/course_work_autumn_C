#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#define BUF 50

int scan_sentence(char **str){
    int len = BUF;
    int n = 0;
    char c;
    *str = malloc(len * sizeof(char));
    do{
        c = (char)getchar();
        (*str)[n] = c;
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
    } while(c != '.');
    (*str)[n] = '\0';
    return 0;
}

void scan_txt(char ***txt){ //TODO
    int len = BUF;
    int n = 0;
    *txt = malloc(len * sizeof(char*));
};

void del_digit(char **sentence){ //удаляет цифры из
    if (sentence) {
        for (int i = 0; i < strlen(*sentence); i++) {
            if (isdigit((*sentence)[i])) {
                memmove(*sentence + i, *sentence + i + 1, strlen(*sentence + i) + 1);
                i--;
            }
        }
    }
}

int is_palindrome(char* str){
    //TODO: При проверке на палиндром учитывать разделители не нужны, то есть строка “abcd eedcba” является палиндромом.
    char *ptr_start = str; /*Указатель на начало строки*/
    char *ptr_end = strchr(str, '\0') - 1;/*Присвоить указатель на конец строки*/
    while(ptr_start <= ptr_end){
        if(*ptr_start != *ptr_end){
            return 0;
        }
        ptr_start++; ptr_end--;
    }
    return 1;
}

int main(){
    char *sentence;
    scan_sentence(&sentence);
    puts(sentence);
    del_digit(&sentence);
    puts(sentence);
    printf("%d", is_palindrome(sentence));
}
