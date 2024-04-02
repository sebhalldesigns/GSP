#include "gSkXml.h"
#include <stdio.h>

char* gsk_xml_serialize(gsk_xml_node_t* root) {

}


gsk_xml_node_t* gsk_xml_parse(char* contents) {

    size_t char_index = 0;

    while (contents[char_index] != '\0') {
        printf("%c\n", contents[char_index]);

        char_index++;
    }

}