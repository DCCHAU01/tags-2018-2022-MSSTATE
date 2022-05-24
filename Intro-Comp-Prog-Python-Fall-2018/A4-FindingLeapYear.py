#Date Due: Sept. 24, 2018
#Program Description: Finding leap years for every 4 years and 400 years

#starting year
sy = int(input("Enter the starting year: "))
#ending year
ey = int(input("Enter the ending year: "))

#leap year finder
for i in range(sy, ey):
    if (sy % 100 == 0) and (sy % 400 == 0):
        print(sy)
        sy += 1
    elif (sy % 4 == 0):
        print(sy)
        sy += 1 
    else:
        sy += 1


