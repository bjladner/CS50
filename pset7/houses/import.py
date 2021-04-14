import cs50
import csv
from sys import argv, exit

# Read in command line arguments
if len(argv) != 2:
    print("Usage: python import.py file.csv")
    exit(1)

# Create database by opening and closing an empty file first
open(f"students.db", "w").close()
db = cs50.SQL("sqlite:///students.db")

# Create table called students, and specify the columns we want
db.execute("CREATE TABLE students (first TEXT, middle TEXT, last TEXT, house TEXT, birth NUMERIC)")

# Read in csv file and save to students.db
with open(argv[1], "r") as csv_students:

    # Create DictReader
    reader = csv.DictReader(csv_students)

    # Iterate over rows which have (name, house, birth) columns
    for row in reader:
        names = row["name"].split()
        if len(names) == 2:
            db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES(?, ?, ?, ?, ?)",
                       names[0], None, names[1], row["house"], int(row["birth"]))
        elif len(names) == 3:
            db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES(?, ?, ?, ?, ?)",
                       names[0], names[1], names[2], row["house"], int(row["birth"]))
        else:
            print("Invalid name in csv file: ", row.name)
