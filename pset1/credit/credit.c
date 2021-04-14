#include <stdio.h>
#include <cs50.h>

int main(void)
{
    bool even_numbers_place = true;
    int sum_even = 0;
    int sum_odd = 0;
    int odd_number = 0;
    int first_two = 0;
    int curr_tested_number = 0;
    int prev_tested_number = 0;
    int card_number_size = 0;
    // save credit card number in 'card_number'
    long card_number = get_long("Number: ");

    do
    {
        // process each digit of 'card_number'

        // save the previous number and the current number
        // in case they are the last two digits
        prev_tested_number = curr_tested_number;
        curr_tested_number = card_number % 10;
        if (even_numbers_place == true)
        {
            // if evens place, add digit to running total in 'sum_even'
            sum_even += curr_tested_number;
        }
        else
        {
            // if odds place, multiply digit by 2
            odd_number = 2 * curr_tested_number;
            do
            {
                // then add each digit of product to running total in 'sum_odd'
                sum_odd += odd_number % 10;
                odd_number /= 10;
            }
            while (odd_number > 0);
        }
        // divide 'card_number' by 10 to move to the next digit
        card_number /= 10;
        // change from even to odd digit or odd to even digit
        even_numbers_place = !even_numbers_place;
        // track the size of 'card_number'
        card_number_size++;
    }
    while (card_number > 0);

    // save the first two digits in 'first_two' to test later
    first_two = (curr_tested_number * 10) + prev_tested_number;

    if ((sum_odd + sum_even) % 10 == 0)
    {
        // if sum of 'sum_odd' and 'sum_even' ends in 0
        // this is possibly a valid card
        if (card_number_size == 15 && (first_two == 34 || first_two == 37))
        {
            // AMEX (15 digits) - start with 34 or 37
            printf("AMEX\n");
        }
        else if (card_number_size == 16 && first_two >= 51 && first_two <= 55)
        {
            // MASTERCARD (16 digits) - starts with 51, 52, 53, 54, 55
            printf("MASTERCARD\n");
        }
        else if ((card_number_size == 13 || card_number_size == 16) && first_two >= 40 && first_two <= 49)
        {
            // VISA (13 or 16 digits) - starts with 4
            printf("VISA\n");
        }
        else
        {
            // if 'card_number' is not a VISA, AMEX, or MASTERCARD
            // then this is an invalid card
            printf("INVALID\n");
        }
    }
    else
    {
        // if sum of 'sum_odd' and 'sum_even' ends in something other than 0
        // then this is an invalid card
        printf("INVALID\n");
    }
}