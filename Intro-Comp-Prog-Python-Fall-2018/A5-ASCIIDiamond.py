#Date Due: Oct. 1, 2018
#Program Description: the program will print out a diamond based on the given user input

#height of diamond
hd = int(input("Enter the height of the diamond: "))
while (hd <= 0) or (hd%2 == 0):
    print('ERROR:  the height must be an odd number greater than 0.')
    hd = int(input('Please re-enter: '))

if (hd%2 == 1):
    print('The validated height of the diamond is: ', hd)

for i in range(hd//2):
    for s in range(hd-i):
        print(' ',end='')
    for j in range(i+1):
        print('*', end=' ')
    print()
for i in range(hd//2, -1, -1):
    for s in range(hd-i):
        print(' ',end='')
    for j in range(i+1):
        print('*', end=' ')
    print()
