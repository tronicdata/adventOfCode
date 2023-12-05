#include <stdio.h>
#include <ctype.h> 
#include <string.h> 
#include <stdlib.h> 

#define MAX_RED 12 
#define MAX_GREEN 13
#define MAX_BLUE 14

void freeArrayMemory(char **arr, int len){
    for(int i=0; i < len; i++){
        free(arr[i]);
    }
}

char *strtok_substr(char *str, const char *delim) {
    static char *lasts;
    if (str) {
        lasts = str;
    }

    if (lasts == NULL || *lasts == '\0') {
        return NULL;
    }

    char *start = lasts;
    char *end = strstr(start, delim);

    if (end) {
        *end = '\0';
        lasts = end + strlen(delim);
    } else {
        lasts = NULL;
    }

    return start;
}

int countCharsInString( char str[], char delimiter[] ){
    int count = 0;
    int delimLen = strlen(delimiter);
    int strLen = strlen(str);
    
    for (int i=0; i < strLen; i++) {
        
        if (str[i] == delimiter[0] && (i + delimLen <= strLen)) {
            int match = 1;
            
            for (int j=1; j < delimLen; j++){
                if(str[i + j] != delimiter[j]){
                    match = 0; 
                } 
            }
            
            if (match) {
                count++;
                i += delimLen - 1;
            }
        }
        
    }
    
    return count;
}

char **createSubstring( char str[], char delimiter[], int *tokenCount ){
    
    *tokenCount = countCharsInString(str, delimiter) + 1;
    char **arr = malloc(*tokenCount * sizeof(char*));
    
    if(arr == NULL){
        exit(1);
    }
    
    char *token = strtok_substr(str, delimiter);
    
    int i = 0; 
    while (token != NULL) {
        
        arr[i] = malloc(strlen(token) + 1);
        if (arr[i] == NULL) {
            exit(1);
        }
        
        strcpy(arr[i], token);
        token = strtok_substr(NULL, delimiter); // Continue to get next token
        
        i++;
    }
    
    return arr;
    
} 

int getGameId(char *str){
        
    const char delimiters[] = " ";
    char *token = strtok_substr(str, delimiters);
    int id;
    
    while (token != NULL) {
        
        if( !strstr(token, "Game") ){
            id = atoi(token);     
        } 
        
        token = strtok_substr(NULL, delimiters); // Continue to get next token
    }
    
    return id;
}

int validateBlock(char str[]){
        
    int isValid = 1;
    
    int blockCount;
    char **block = createSubstring(str, " ", &blockCount);
    
    int count = atoi(block[0]);
    
    if(strcmp(block[1], "red") == 0 && count > MAX_RED){
        isValid = 0;
    } else if (strcmp(block[1], "blue") == 0 && count > MAX_BLUE){
        isValid = 0;
    } else if (strcmp(block[1], "green") == 0 && count > MAX_GREEN) {
        isValid = 0;
    }
    
    freeArrayMemory(block, blockCount);
    
    return isValid;
}


int validateSet(char str[]){
        
    int isValid = 1;
    
    int blocksCount;
    char **blocks = createSubstring(str, ", ", &blocksCount);
    
    for (int i = 0; i < blocksCount; i++) {
        if (!validateBlock(blocks[i])) {
            isValid = 0;   
            break; 
        }
    }
    
    freeArrayMemory(blocks, blocksCount);
    
    return isValid;
}



int main(int argc, char *argv[])
{
    
    FILE *file;
    char filename[] = "input.txt"; // Replace with your file path
    // char filename[] = "testInput.txt"; // Replace with your file path
    char buffer[1000];
    int total = 0;
    
    // Open the file in read mode
    file = fopen(filename, "r");

    if (file == NULL) {
        return 1;
    }
    
    // Read and print the file content
    while (fgets(buffer, 1000, file) != NULL) {
        
        int id;
        int isValid = 1;
       
        int arrCount; 
        char **arr = createSubstring(buffer, ": ", &arrCount);
        
        id = getGameId(arr[0]);
        
        int setsCount; 
        char **sets = createSubstring(arr[1], "; ", &setsCount);
        
        freeArrayMemory(arr, arrCount) ;
        
        for (int i = 0; i < setsCount; i++) {
           
            if (!validateSet(sets[i])) {
                isValid = 0;
                break; 
            } 
        }
        
        freeArrayMemory(sets, setsCount);
        
        if(isValid){
            total += id;    
        }
        
    }
    
    printf("GRAND TOTAL= %i\n", total);
    // Close the file
    fclose(file);
    
    return 0; 
}
