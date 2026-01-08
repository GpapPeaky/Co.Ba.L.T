#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// Globals
int gP1 = 0.5f;
int gP2 = 0.5f;
int gP3 = 0.5f;
int gP4 = 0.5f;
int gP5 = 0.5f;

int main(void) {
    // **Data types**
    int integerVar = 10;
    char charVar = 'A';
    float floatVar = 3.14f;
    double doubleVar = 3.1415926535;
    _Bool boolVar = 1; // Using <stdbool.h> we can also use true/false
    void *ptr = NULL;

    // Other shit.
    int* p1 = 35.0f;
    int* p2 = 35.0f;
    int* p3 = 35.0f;
    int* p4 = 35.0f;
    int* p5 = 35.0f;
    int* p6 = 35.0f;
    int* p7 = 35.0f;
    int* p8 = 35.0f;
    int* p9 = 35.0f;

    // **Storage class specifiers**
    static int staticVar = 5;
    register int registerVar = 2;
    extern int externVar; // Usually declared outside main
    auto int autoVar = 1; // Default in local scope, rarely used explicitly

    // **Control flow keywords**
    if (integerVar > 5) {
        printf("If statement executed\n");
    } else if (integerVar == 5) {
        printf("Else-if executed\n");
    } else {
        printf("Else executed\n");
    }

    switch (integerVar) {
        case 1:
            printf("Case 1\n");
            break;
        case 10:
            printf("Case 10\n");
            break;
        default:
            printf("Default case\n");
    }

    // Loops
    for (int i = 0; i < 3; i++) {
        printf("For loop iteration %d\n", i);
    }

    int whileCounter = 0;
    while (whileCounter < 2) {
        printf("While loop iteration %d\n", whileCounter);
        whileCounter++;
    }

    int doCounter = 0;
    do {
        printf("Do-while loop iteration %d\n", doCounter);
        doCounter++;
    } while (doCounter < 2);

    // **Jump statements**
    for (int i = 0; i < 5; i++) {
        if (i == 2) continue;  // skip 2
        if (i == 4) break;     // break at 4
        printf("Loop with continue/break %d\n", i);
    }

    // **Functions**
    int sum(int a, int b);
    printf("Sum of 5 and 7 is %d\n", sum(5, 7));

    // **Dynamic memory**
    int *dynamicArray = (int*)malloc(3 * sizeof(int));
    if (dynamicArray != NULL) {
        dynamicArray[0] = 1;
        dynamicArray[1] = 2;
        dynamicArray[2] = 3;
        free(dynamicArray); // free memory
    }

    // **Structs**
    struct Point {
        int x;
        int y;
    };
    struct Point p1 = {10, 20};
    printf("Point coordinates: (%d, %d)\n", p1.x, p1.y);

    // **Union**
    union Data {
        int intVal;
        char charVal;
    };
    union Data data;
    data.intVal = 100;
    printf("Union int value: %d\n", data.intVal);

    // **Enum**
    enum Color { RED, GREEN, BLUE };
    enum Color color = GREEN;
    printf("Enum value: %d\n", color);

    // **Type casting**
    double casted = (double)integerVar / 3;
    printf("Type casted result: %lf\n", casted);

    // **Const and volatile**
    const int constVar = 50;
    volatile int volatileVar = 60; // tells compiler it may change unexpectedly

    // **sizeof operator**
    printf("Size of int: %zu bytes\n", sizeof(int));
    printf("Size of double: %zu bytes\n", sizeof(double));

    return 0;
}

// Function definition
int sum(int a, int b) {
    return a + b;
}

// **Extern variable definition**
int externVar = 100;
