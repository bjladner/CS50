exerpt = input("Text: ")

characters = 0
words = 0
sentences = 0
prev_was_alpha = False

for char in exerpt:
    # print(char, end="")
    if char.isalpha():
        # print(" - character")
        prev_was_alpha = True
        characters += 1
    elif prev_was_alpha and (char == ' ' or char == ','):
        # print(" - word")
        prev_was_alpha = False
        words += 1
    elif prev_was_alpha and (char == '.' or char == '!' or char == '?'):
        # print(" - sentence")
        prev_was_alpha = False
        sentences += 1
        words += 1

c_per_100_w = 100 * characters / words
s_per_100_w = 100 * sentences / words
grade_level = (0.0588 * c_per_100_w) - (0.296 * s_per_100_w) - 15.8

if grade_level > 16:
    print("Grade 16+")
elif grade_level < 1:
    print("Before Grade 1")
else:
    print("Grade", round(grade_level))
