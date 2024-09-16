# Sorts favorites by value using .get

import csv

# Open CSV file
with open('favorites.csv', 'r') as file:

    # Create DictReader
    reader = csv.DictReader(file)

    # Counts
    count = {}

    # Iterate over CSV file, counting favorites
    for row in reader:
        favorite = row['language']
        if favorite in count:
            count[favorite] += 1
        else:
            count[favorite] = 1

# Print counts
for favorite in sorted(count, key=count.get, reverse=True):
    print(f"{favorite}: {count[favorite]}")