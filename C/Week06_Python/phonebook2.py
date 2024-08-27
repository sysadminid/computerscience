# Implements a phone book as a list of dictionaries

from cs50 import get_string

people = [
    {"name": "Aqsa", "number": "1234"},
    {"name": "Dita", "number": "5678"},
    {"name": "Sofyan", "number": "9123"},
]

# Search for name
name = get_string("Name: ")
for person in people:
    if person["name"] == name:
        number = person["number"]
        print(f"Found {number}")
        break
else:
    print("Not found")