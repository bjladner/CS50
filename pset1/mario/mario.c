#include <stdio.h>
#include <cs50.h>

int main(void)
{
    int size = 0;

    // Ask the user for the size of the pyramid and save it in 'size'
    // Use do-while loop to make sure 'size' is between 1 and 8
    do
    {
        size = get_int("Height: ");
    }
    while (size < 1 || size > 8);

    // loop through each line of the pyramid
    for (int i = 1; i <= size; i++)
    {
        // loop through size-i empty spaces
        for (int j = 0; j < size - i; j++)
        {
            printf(" ");
        }
        // loop through i # signs
        for (int k = 0; k < i; k++)
        {
            printf("#");
        }
        // add 2 spaces to seperate the pyramids
        printf("  ");
        // loop through i # signs
        for (int l = 0; l < i; l++)
        {
            printf("#");
        }
        printf("\n");
    }
}