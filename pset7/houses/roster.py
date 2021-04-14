import cs50
import csv
from sys import argv, exit

# Read in command line arguments
if len(argv) != 2:
    print("Usage: python roster.py house")
    exit(1)

# Create database by opening and closing an empty file first
db = cs50.SQL("sqlite:///students.db")

# CREATE TABLE students (first TEXT, middle TEXT, last TEXT, house TEXT, birth NUMERIC);
house_people = db.execute("SELECT * FROM students WHERE house = (?) ORDER BY last, first", argv[1])

# For each person in the house specified, print name, and birth year
for person in house_people:
    if person["middle"]:
        print(person["first"], person["middle"], person["last"] + ", born", person["birth"])
    else:
        print(person["first"], person["last"] + ", born", person["birth"])
