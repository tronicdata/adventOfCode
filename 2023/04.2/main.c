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
    
    int total = 0;
    
    // create array for instances counter. Set all to 1
    size_t totalRows = 1;
    
    for(int i=0; buffer[i] != '\0'; i++){
        if(buffer[i] == '\n'){ totalRows++; }
    }
    
    int *instancesArray = malloc(totalRows * sizeof(int));
    
    for(size_t i = 0; i < totalRows; i++){
        instancesArray[i] = 1;
    }
    
    const char line_delimiters[] = "\n";
 
    char* token;
    char *line_saveptr;
    int rowNum = 1;
 
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
                        rowTotal++;
                    }
                    numbers_individual_token = strtok_r(NULL, " ", &numbers_individual_saveptr);
                }
                
            } else {
                free(winningNumbers);
                free(testNumbers);
                exit(EXIT_FAILURE);
            }
            
            
        } 
        
        // add +1 to all following cards equal to number of matches
        // do this one time for EACH instance (outer loop)
        for(int j=instancesArray[rowNum-1]; j > 0; j--){
            for(int i=rowNum; i < ((rowNum) + rowTotal); i++){
                instancesArray[i]++;
            }
        }
        
        rowNum++;
 
        token = strtok_r(NULL, line_delimiters, &line_saveptr);
        
    }    
    
    for(size_t i = 0; i < totalRows; i++){
        total+=instancesArray[i];
    }
    
    free(instancesArray);
    
    printf("TOTAL= %i\n", total);
    
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
    
    
    processFile(buffer);
    
    return 0; 
}
