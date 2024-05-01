#include <stdio.h>
#include <string.h>
#include <gSkXml.h>
#include <stdlib.h>

int main() {

    printf("Hello world!\n");

    FILE* file;
    file = fopen("../testing/books.xml", "r");

    if (file == NULL) {
        printf("FILE OPEN FAILED!\n");
        return -1;
    }

    printf("FILE OPEN OK!\n");
    
    char ch;
    char* buffer;
    size_t buffer_size = 0;
    while (!feof(file)) {
        ch = fgetc(file);
        //printf("%c", ch);
        if (buffer_size == 0) {
            buffer = malloc(sizeof(char));
            memcpy(buffer, &ch, sizeof(char));
        } else {
            buffer = realloc(buffer, buffer_size + 1);
            memcpy(buffer + buffer_size, &ch, sizeof(char));
        }
        
        buffer_size++;
    }

    fclose(file);

    gsk_xml_node_t* root = gsk_xml_parse(buffer);

    //printf("STRING IS: %s!\n", buffer);




    

    return 0;
}