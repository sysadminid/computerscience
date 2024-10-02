# Implements linear search for names using loop

# A list of names
names = ['Aqsa', 'Dita', 'Sofyan']

# Ask for name
name = input('Name: ')

# Search for name
for n in names:
    if n == name:
        print('Found')
        break
else:
    print('Not Found')