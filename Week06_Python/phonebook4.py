# Implements a phone book using a dictionary

from cs50 import get_string

people = {
    "Aqsa": "1234",
    "Dita": "5678",
    "Sofyan": "9123",
}

# Search for name
name = get_string("Name: ")
if name in people:
    print(f"Number: {people[name]}")
else:
    print("Not found")