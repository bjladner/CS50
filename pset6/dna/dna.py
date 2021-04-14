import csv
import re
from sys import argv, exit

# Read in command line arguments
if len(argv) != 3:
    print("Usage: python dna.py data.csv sequence.txt")
    exit(1)

csv_array = []

# Read in csv file and save to a dict
with open(argv[1], "r") as csv_file:
    csv_dict_reader = csv.DictReader(csv_file)
    # get column names from a csv file
    column_names = csv_dict_reader.fieldnames
    for row in csv_dict_reader:
        csv_array.append(row)

# Read in sequence file and save to string
with open(argv[2], "r") as sequence_file:
    sequence = sequence_file.read()

sequence_count = []
# For each of the dna sequences
for i in range(1, len(column_names)):
    # Search for all the repeated dna sequence in the sequence string
    searches = re.finditer(f"({column_names[i]})+", sequence)
    search_count = []
    # Count each found repeated sequence
    for search in searches:
        search_count.append(search.group().count(column_names[i]))
    # Find largest count and add to the sequence count
    if search_count:
        sequence_count.append(max(search_count))
    else:
        sequence_count.append(0)

found = False
number_matches = 0
# Loop through each person in csv_array and each sequence
for i in range(len(csv_array)):
    for j in range(len(column_names) - 1):
        if sequence_count[j] == int(csv_array[i].get(column_names[j+1])):
            number_matches += 1
    # If each sequence max matches, set found to True
    if number_matches == len(sequence_count):
        found = True
        break
    else:
        number_matches = 0

# If found a match, then print out name
if found:
    print(csv_array[i].get(column_names[0]))
else:
    print("No match")

# print(column_names)
# print(sequence_count)
# for row in csv_array:
#     print(row)

