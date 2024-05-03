#include "GSPCore/GSPCore.h"
#include "GSPCore/GVector.h"

#include <stdio.h>


typedef struct {
    float a;
    int b;
    const char* c;
} exampleStruct;

void LaunchEvent() {
    printf("Launched!\n");

    exampleStruct struct1;
    struct1.a = 1.0f;
    struct1.b = -5;
    struct1.c = "Hello world from struct 1!";

    exampleStruct struct2;
    struct2.a = 10.0f;
    struct2.b = -10;
    struct2.c = "Hello world from struct 2!";

    GVector vector = GVector_Init();
    printf("vector has size %lu\n", GVector_Size(vector));
    GVector_Add(vector, &struct1);

    printf("vector has size %lu\n", GVector_Size(vector));
    GVector_Add(vector, &struct2);

    printf("vector has size %lu\n", GVector_Size(vector));

    GVector_Set(vector, 0, &struct2);
    GVector_Set(vector, 1, &struct1);

    for (int i = 0; i < GVector_Size(vector) + 1; i++) {
        
        exampleStruct* ex = (exampleStruct*)GVector_Get(vector, i);
        if (ex == NULL) {
            printf("ex INVALID\n");
        } else {
            printf("%d : %f %d %s \n", i, ex->a, ex->b, ex->c);
        }
        
    }

    printf("struct 1 is at index %lu\n", GVector_IndexOf(vector, &struct1));
    printf("struct 2 is at index %lu\n", GVector_IndexOf(vector, &struct2));

    GVector_Free(vector);
}

int main() {

    GApplicationInfo info = {
        "example app", 
        "Seb Hall", 
        0, 0
    };

    GApplication app = GApplication_Init(info);
    
    GApplication_SetLaunchEvent(app, LaunchEvent);

    return GSPRun(app);
}