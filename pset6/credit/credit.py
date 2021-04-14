from cs50 import get_int

card_number = get_int("Number: ")
card_number_size = 0
curr_tested_number = 0
prev_tested_number = 0
even_numbers_place = True
sum_even = 0
sum_odd = 0
odd_number = 0

while card_number > 0:
    prev_tested_number = curr_tested_number
    curr_tested_number = card_number % 10
#    print(card_number, end="")
    if even_numbers_place == True:
        sum_even += curr_tested_number
#        print(" Even number:", curr_tested_number, " Running Total:", sum_even)
    else:
        odd_number = 2 * curr_tested_number
        if odd_number >= 10:
            sum_odd += odd_number % 10
            odd_number = int(odd_number / 10)
            sum_odd += odd_number % 10
        else:
            sum_odd += odd_number
#        print(" Odd number:", curr_tested_number, " Running Total:", sum_even)
    card_number = int(card_number / 10)
    even_numbers_place = not even_numbers_place
    card_number_size += 1

first_two = (curr_tested_number * 10) + prev_tested_number
#print("Size:", card_number_size, " First two digits:", first_two, " SumOdd:", sum_odd, " SumEven:", sum_even)

if (sum_odd + sum_even) % 10 == 0:
    if card_number_size == 16 and first_two >= 51 and first_two <= 55:
        print("MASTERCARD")
    elif card_number_size == 15 and (first_two == 34 or first_two == 37):
        print("AMEX")
    elif (card_number_size == 13 or card_number_size == 16) and first_two >= 40 and first_two <= 49:
        print("VISA")
    else:
        print("INVALID")
else:
    print("INVALID")
