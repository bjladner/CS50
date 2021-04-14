from cs50 import get_int

while True:
    height = get_int("Height: ")
    if height in range(1, 9):
        break

for i in range(1, height+1):
    for j in range(height-i):
        print(" ", end="")
    for k in range(i):
        print("#", end="")
    print("  ", end="")
    for l in range(i):
        print("#", end="")
    print()
