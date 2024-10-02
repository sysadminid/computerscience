# Implements linear search for names using `in`

# A list of names
names = ['Aqsa', 'Dita', 'Sofyan']

# Ask for name
name = input('Name: ')

# Search for name
if name in names:
    print('Found')
else:
    print('Not found')