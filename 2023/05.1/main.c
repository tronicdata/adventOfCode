#include <stdio.h>
#include <string.h> 
#include <stdlib.h> 

const char *fieldNames[] = {
    "seed",
    "soil",
    "fertilizer",
    "water",
    "light",
    "temperature",
    "humidity",
    "location"
};

int findFieldIndex(const char *fieldName) {

    for (int i = 0; i < sizeof(fieldNames) / sizeof(fieldNames[0]); i++) {
        if (strcmp(fieldName, fieldNames[i]) == 0) {
            return i; // Match found, return the index
        }
    }

    return -1; // No match found, return -1
}

void processFile(char *buffer){

    long seeds[50][8];
    int seedsSize;

    char srcName[50];
    char destName[50];

    char *line_ptr;
    char *token= strtok_r(buffer, "\n", &line_ptr);

    while(token != NULL){

        // gather all the seeds
        if( strstr(token, "seeds:") ){
            int i = 0;
            char *colon = strchr(token, ':');
            char *seed_ptr;
            char *seedTok= strtok_r(colon + 1, " ", &seed_ptr);

            while(seedTok != NULL){
                char *endptr;
                seeds[i][0]= strtol(seedTok, &endptr, 10);

                //initialize all fields to -1
                for(int j=1; j<8; j++){ seeds[i][j]=-1;}

                i++;
                seedTok= strtok_r(NULL, " ", &seed_ptr);
            }

            seedsSize = i;

        // Find src/dst field names
        } else if( strstr(token, "map:") ){

            char *hyphen = strstr(token, "-");

            int len = hyphen - token;

            strncpy(srcName, token, len);

            srcName[len] = '\0';

            char *space = strstr(token, " ");

            int destLen = space - ( hyphen + 4 );

            strncpy(destName, token + len + 4, destLen);
            destName[destLen]= '\0';

        } else {
            
            long destRangeStart=-1;
            long srcStart=-1;
            long range=-1;
            char *location_ptr;
            char *locationTok= strtok_r(token, " ", &location_ptr); 

            while ( locationTok != NULL){
                char *endptr;
                
                if(destRangeStart == -1){
                    destRangeStart= strtol(locationTok, &endptr, 10);
                } else if (srcStart == -1){
                    srcStart= strtol(locationTok, &endptr, 10);
                } else {
                    range= strtol(locationTok, &endptr, 10);
                }

                locationTok= strtok_r(NULL, " ", &location_ptr); 
            }

            int srcFieldIndex = findFieldIndex(srcName);
            int destFieldIndex = findFieldIndex(destName);

            for(int i=0; i < seedsSize; i++){

                // check if src falls in range
                if ( seeds[i][srcFieldIndex] >= srcStart && seeds[i][srcFieldIndex] < (srcStart + range) ){
                    // then map new destination
                    long diff = destRangeStart - srcStart;
                    seeds[i][destFieldIndex] = seeds[i][srcFieldIndex] + diff;  

                // if not in range then map to itself, unless the dest field already has been updated
                } else if(seeds[i][destFieldIndex] == -1 ){
                    seeds[i][destFieldIndex] = seeds[i][srcFieldIndex];  
                }
                
            }

        }

        token= strtok_r(NULL, "\n", &line_ptr);
    }

    long lowestLocation = -1;
    
    // find lowest Location value in seeds array
    for(int i=0; i < seedsSize; i++){

        int locationFieldNameIndex = findFieldIndex("location");

        if( lowestLocation == -1 || seeds[i][locationFieldNameIndex] < lowestLocation){
            lowestLocation = seeds[i][locationFieldNameIndex];
        }

    }

    printf("Lowest Location=%ld\n", lowestLocation);
    
}

int main(int argc, char *argv[]) {
    
    FILE *file;
    char filename[] = "input.txt"; // Replace with your file path
    //char filename[] = "testinput.txt"; // Replace with your file path
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
