#include <stdio.h>
#include <ctype.h> 
#include <string.h> 
#include <stdlib.h> 

int checkForNumbers( int i, int lineLen, char *buffer){
    
    int total = 0;
    int isValid = 0;
    char firstNum[5]= "";
    char secondNum[5]= "";
    
    //array of indexs in the buffer that are adjacent to symbol
    int validIndexArr[8] = {
        i - lineLen - 1, 
        i - lineLen, 
        i - lineLen + 1, 
        i-1, 
        i+1, 
        i + lineLen - 1, 
        i + lineLen, 
        i + lineLen + 1
    };
    
    //track index of last number recorded to avoid duplicates
    int lastRecordedNumIndex = -1;
    
    //check for digits in all valid locations
    for(int j=0; j < 8; j++){
        
        if ( validIndexArr[j] > lastRecordedNumIndex && isdigit(buffer[validIndexArr[j]])) {
            
            // set index to the beginning of the number
            int l= validIndexArr[j] - 0;
            
            while ( isdigit(buffer[l-1]) ){
                l--; 
            }        
            
            char num[5] = "";
            int numIndex = 0;
            
            // loop through the number
            while ( isdigit(buffer[l]) ){
                
                num[numIndex] = buffer[l];
                
                numIndex++;
                l++;
                
            }
            
            if( strlen(num) ){
                total+= atoi(num);
            }
            
            lastRecordedNumIndex = l;
            
        } 
        
    }
    
    return total;
}

void processFile(char *buffer){
    int lineLen = 1;
    for(int i=0; buffer[i] != '\n'; i++, lineLen++){};
    
    //pass through data and collect index of all symbols
    int total = 0;
    int i=0;
    while ( buffer[i] != '\0' ){
        
        if ( 
            buffer[i] != '.' && 
            buffer[i] != '\n' &&
            !isdigit(buffer[i])
            ){
            
            total += checkForNumbers(i, lineLen, buffer);
            
        } 
        
        i++;
        
    }
    
    printf("GRAND TOTAL= %i\n", total);
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
