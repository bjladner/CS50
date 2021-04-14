#include <stdio.h>
#include <cs50.h>

int main(void)
{
    // Ask the user for their name and save it in 'name'
    string name = get_string("Please enter your name: ");

    // Print 'hello, ' plus the name entered by the user
    printf("Hello, %s\n", name);
}