/**
 * @file gSkXml.h
 * @brief This file contains the gSerKit XML serializer and parser interface.
 */

#ifndef GSERKIT_H
#define GSERKIT_H

#include <stddef.h>
#include <stdint.h>

typedef struct gsk_xml_attribute_t gsk_xml_attribute_t;
typedef struct gsk_xml_node_t gsk_xml_node_t;


struct gsk_xml_attribute_t {
    char* key;
    char* value;
};

struct gsk_xml_node_t {
    char* type;
    char* content;
    gsk_xml_node_t* children;
    size_t num_children;
    gsk_xml_attribute_t* attributes;
    size_t num_attributes;
};

char* gsk_xml_serialize(gsk_xml_node_t* root);
gsk_xml_node_t* gsk_xml_parse(char* contents);




#endif // GSERKIT_H