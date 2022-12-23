#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_WORD_LEN 10

typedef struct _
{
    int index;
    char *text;
    size_t length;
}sword;

//create global structs for words
sword word[100];
void procWords(char*[],int);

int main(){
    char *inputs[]={"apple","banana","kivi","man","jumbotron","cat"};
    procWords(inputs,6);
    printf("Words processed!!");
    return 0;
}


void procWords(char *inputs[],int count)
{
    int max_len=0;
    //get the maximum length of any word
    for (size_t i = 0; i < count; i++)
    {
        max_len=(strlen(inputs[i])>max_len)?strlen(inputs[i]):max_len;
    }

    //sort the inputs according to length descending
    int sort[count];
    int len=max_len;
    int index=0;
    for (size_t y = 0; index<count; y++)
    {
        for (size_t i = 0; i < count; i++)
        {
            if (strlen(inputs[i])==len){
                sort[i]=index;
                index++;
            }
        }
        len--;
    }
    
    
    //assign word structs
    for (size_t i = 0; i < count; i++)
    {
        word[sort[i]].index=0;
        word[sort[i]].text=inputs[i];
        word[sort[i]].length=strlen(inputs[i]);
    }
}
