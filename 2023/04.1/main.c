#include <stdio.h>
#include <ctype.h> 
#include <string.h> 
#include <stdlib.h> 

int findMatch( char *input, const char *str ){
    
    int found = 0;
    char *duplicate = strdup(input);
    char *saveptr;
    
    char *token = strtok_r(duplicate, " ", &saveptr);
    
    while (token != NULL){
    
        if( strcmp(token, str) == 0 ){
            found = 1;
            break;
        }
        
        token = strtok_r(NULL, " ", &saveptr);
        
    }
    
    free(duplicate);
    
    return found; 
}

void processFile(char *buffer){
    
    int lineLen = 1;
    int cardIndex;
    int splitIndex;
    for(int i=0; buffer[i] != '\n'; i++, lineLen++){
        if( buffer[i] == ':' ){
            cardIndex = i;
        }
        if( buffer[i] == '|' ){
            splitIndex = i;
        }
    };
    
    //pass through data and collect index of all symbols
    int total = 0;
    
    char winningNumbers[100] = "";
    char currentNumber[5] = "";
    int winningNumbersIndex=0;
    int currNumIndex = 0;
    int passSplit = 0; 
    int rowTotal = 0;
    
    int wholeNumberCount = 0;
    int i=cardIndex+2;
    while ( buffer[i] != '\0' ){
        //end of the row reset
         if( passSplit && isdigit(buffer[i]) ){
            currentNumber[currNumIndex] = buffer[i];
            
            currNumIndex++;
        } else if( passSplit && !isdigit(buffer[i]) ){
            
            currentNumber[currNumIndex] = '\0'; // Null-terminate the current number
            
             
            int isMatch = findMatch(winningNumbers, currentNumber);
            
            if(isMatch){
                rowTotal = rowTotal ? rowTotal * 2 : 1;
            }
            
            memset(currentNumber, '\0', sizeof(currentNumber));
            
            currNumIndex = 0;
             
        } else if( buffer[i] == '|' ){
            passSplit = 1;
        } else if( !passSplit ) {
            winningNumbers[winningNumbersIndex] = buffer[i]; 
            winningNumbersIndex++;
        }
        
        if( buffer[i] == '\n'){
            
            total += rowTotal;
            
            // end of row resets
            memset(winningNumbers, '\0', sizeof(winningNumbers));
            memset(currentNumber, '\0', sizeof(currentNumber));
            passSplit = 0;
            winningNumbersIndex=0;
            currNumIndex=0;
            rowTotal = 0;
            
            //skip ahead pass card number index
            i += cardIndex+2;
        } 
        
        i++;
        
    }
    
    printf("GRAND TOTAL= %i\n", total);
    
    if( total != 24542 ){
        printf("WRONG!");
    }
    
}

void processFile2(char *buffer){
    
    int total = 0;
    
    const char line_delimiters[] = "\n";
 
    char* token;
    char *line_saveptr;
 
    token = strtok_r(buffer, line_delimiters, &line_saveptr);
 
    while (token != NULL) {
 
        char *id_part = strchr(token, ':');
        char *numbers_part = strchr(token, '|');
        int rowTotal = 0;
        
        if( id_part && numbers_part ){
            int winningNumbersLen = numbers_part - (id_part + 1);
            char *winningNumbers = malloc(winningNumbersLen);
            char *testNumbers = malloc(strlen(numbers_part) + 1);
            
            if(winningNumbers && testNumbers){
                
                strncpy(winningNumbers, id_part + 1, winningNumbersLen);
                winningNumbers[winningNumbersLen] = '\0';
                
                strcpy(testNumbers, numbers_part + 1);
                
                char* numbers_individual_saveptr = NULL;
                char* numbers_individual_token = strtok_r(testNumbers, " ", &numbers_individual_saveptr);

                while (numbers_individual_token != NULL) {
                    if (findMatch(winningNumbers, numbers_individual_token)) {
                        rowTotal = rowTotal ? rowTotal * 2 : 1;
                    }
                    numbers_individual_token = strtok_r(NULL, " ", &numbers_individual_saveptr);
                }
                
            } else {
                free(winningNumbers);
                free(testNumbers);
                exit(EXIT_FAILURE);
            }
            
            
        } 
        
        total += rowTotal;
 
        token = strtok_r(NULL, line_delimiters, &line_saveptr);
        
    }    
    
    printf("GRAND TOTAL= %i\n", total);
    
    if( total != 24542 ){
        printf("WRONG!");
    }
    
}

int main(int argc, char *argv[]) {
    
    FILE *file;
    char filename[] = "input.txt"; // Replace with your file path
    // char filename[] = "testInput.txt"; // Replace with your file path
    char *buffer;
    long fileLength;
    
    // Open the file in read mode
    file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }
    
    //get file length
    fseek(file, 0, SEEK_END);
    //get file pointer position
    fileLength = ftell(file);
    //rewind file pointer to start
    rewind(file);
    
    // allocate memory for file content
    buffer= malloc(fileLength + 1);
    
    if (buffer == NULL){
        perror("Memory allocation failed");
        fclose(file);
        return 1;
    }
    
    // read file into the buffer
    fread(buffer, 1, fileLength, file);
    buffer[fileLength + 1] = '\0'; // Null-terminate the buffer
    
    fclose(file);
    
    
    // processFile(buffer);
    processFile2(buffer);
    
    return 0; 
}
