#include <stdio.h>
#include <ctype.h> 
#include <string.h> 
#include <stdlib.h> 

typedef struct {
    char *key; // String key
    char value; // Integer value
} NumberMap;

int main(int argc, char *argv[])
{
    FILE *file;
    char filename[] = "input.txt"; // Replace with your file path
    // char filename[] = "testInput.txt"; // Replace with your file path
    
    char buffer[1000];
    
    NumberMap numbers[9] = {
        {"one", '1'},
        {"two", '2'},
        {"three", '3'},
        {"four", '4'},
        {"five", '5'},
        {"six", '6'},
        {"seven", '7'},
        {"eight", '8'},
        {"nine", '9'}
    };
    
    int arraySize = sizeof(numbers) / sizeof(numbers[0]);
    int total = 0;

    // Open the file in read mode
    file = fopen(filename, "r");

    if (file == NULL) {
        printf("Error opening file.\n");
        return 1;
    }
    
    // Read and print the file content
    while (fgets(buffer, 1000, file) != NULL) {
        printf("%s", buffer);
        
        char firstNumber= '0';
        char lastNumber= '0';
        
        // loop through each char of line
        for(int i=0; buffer[i] != '\0'; i++){
            
            if( isdigit(buffer[i]) ){
                
                if( firstNumber == '0' ){
                    firstNumber = buffer[i];
                } 
                
                lastNumber = buffer[i];
                
            } else {
                
                // loop through numbers array
                for(int j=0; j < arraySize; j++){
                    
                    size_t lengthOfCurrNum = strlen(numbers[j].key);
                    int isNum = 0;
                    
                    if( i >= lengthOfCurrNum - 1) {
                        
                        int k = 0;
                        
                        int lastIndexOfCurrNum = lengthOfCurrNum - 1;
                        
                        // loop through current number string and compare if previous indexes of the current line match chars
                        while( k < lengthOfCurrNum && numbers[j].key[lastIndexOfCurrNum-k] == buffer[i-k]){
                            k++;
                        }
                        
                        // if k reached the length of the current number then there was a full match
                        if(k == lengthOfCurrNum){
                            isNum = 1;
                        }
                        
                    }
                    
                    if(isNum){
                        
                        if( firstNumber == '0' ){
                            firstNumber = numbers[j].value;
                        } 
                        
                        lastNumber = numbers[j].value;
                           
                    }
                    
                }
            }
        }
        
        
        char numStr[3] = {firstNumber, lastNumber, '\0'};
         
        printf("firstNumber= %c\n", firstNumber);
        printf("lastNumber= %c\n", lastNumber);
        printf("fullNumber= %s\n", numStr);
        
        total += atoi(numStr);
        
    }
    
    // Close the file
    fclose(file);
    
    printf("Total: %d", total);
    
    return 0; 
}
