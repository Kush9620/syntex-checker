#include <stdio.h>

// Function prototype
void print_numbers(int n);

int main() {
    int n;

    printf("Enter a number: ")
    scanf("%d", &n); // Correctly written, but ensure to include the missing semicolon in previous line

    print_numbers(n);

    return 0;
}

// Function to print numbers from 1 to n
void print_numbers(int n) {
    for (int i = 1; i <= n; i++) { // Correctly written
        if (i % 2 == 0) 
            printf("%d is even.\n", i);
        else
            printf("%d is odd.\n", i; // Missing closing parenthesis here

    } // Missing closing brace for the for loop
    while (n > 0) // Correctly written, but can cause an infinite loop if not managed properly
        printf("%d\n", n);
        n--; // This line is outside the loop due to missing braces

    // Correctly written end of function
}
