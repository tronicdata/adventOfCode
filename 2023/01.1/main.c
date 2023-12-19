#include <stdio.h>
#include <stdlib.h>
#include <ctype.h> 

int main(int argc, char *argv[])
{
    FILE *file;
    char filename[] = "input.txt"; // Replace with your file path
    char buffer[1000];
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
        
        for(int i=0; buffer[i] != '\0'; i++){
            
            if( isdigit(buffer[i]) ){
                
                if( firstNumber == '0' ){
                    firstNumber = buffer[i];
                } 
                
                lastNumber = buffer[i];
                
            }
        }
        
        
        char numStr[3] = {firstNumber, lastNumber, '\0'};
         
        printf("\nfirstNumber= %c\n", firstNumber);
        printf("lastNumber= %c\n", lastNumber);
        printf("fullNumber= %s\n", numStr);
        
        total += atoi(numStr);
        
    }
    
    // Close the file
    fclose(file);
    
    printf("Total: %d", total);
    
    return 0; 
}
