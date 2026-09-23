/*
*
* Write a program in C to store n elements in an array and print the elements using a pointer.
*
* Test Data :
* Input the number of elements to store in the array :5
* Input 5 number of elements in the array :
* element - 0 : 5
* element - 1 : 7
* element - 2 : 2
* element - 3 : 9
* element - 4 : 8
*
* Expected Output :
*
* The elements you entered are :                                                                               
* element - 0 : 5                                                                                              
* element - 1 : 7                                                                                              
* element - 2 : 2                                                                                              
* element - 3 : 9                                                                                              
* element - 4 : 8 
*/

#include <stdio.h>
#include <stdlib.h>

int main() {
    
    int len = 0;
    printf("Input the number of elements to store in the array: ")    ;
    scanf("%d", &len);

    int *arr;

    for (size_t i = 0; i < len ; i++, arr++) {
        printf("element - %d: ", i);
        scanf("%d", arr);
    }

    printf("The elements you entered are: \n");

    for (size_t i = len; i > 0 && i != (size_t)(-1); i--) {
        printf("element - %d: %d\n", len - i, *(arr-i));
    }
    return EXIT_SUCCESS;
}