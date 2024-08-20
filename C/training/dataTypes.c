#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // int will store integer and it always take up 4 bytes of memory (32 bits)
    int myInt;
    // char will store single characters and it always take up 1 bytes of memory (8 bits)
    char myChar;
    // float will store floating-point values, also known as real numbers. It takes up 4 bytes of memory (32 bits).
    float myFloat;
    // double will store floating-point values, double are double precision. It takes up 8 bytes of memory (64 bits)
    double myDouble;
    // bool will store a boolean value (true or false)
    bool myBool;
    // string will store a series of characters.
    string myString;

    myInt = 10;
    printf("This is myInt using 'int' as data type: %i\n\n", myInt);

    myChar = 64; // 64 is @ in ASCII codes
    printf("This is myChar using 'char' as data type: %c\n\n", myChar);

    myFloat = 35.666777888999;
    printf("This is myFloat using 'float' as data type: %.10f\n\n", myFloat);

    myDouble = 35.666777888999;
    printf("This is myDouble using 'double' as data type: %.10f\n\n", myDouble);

    myBool = true;
    printf("This is myBool using 'bool' as data type: %s\n\n", myBool ? "true" : "false");

    myString = "123@abcd.example";
    printf("This is myString using 'string' as data type: %s\n", myString);
}