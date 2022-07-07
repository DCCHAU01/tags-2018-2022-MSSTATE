#Date Due: Oct.22, 2018
#Program Description:  finds the average of a list of grades

file_prompt = input('What is your file name (with extension)?' )

try:
    file = open(file_prompt, 'r')
    total = 0
    count = 0

    print('Reading...')

    for grade in file:
        total += int(grade)
        count += 1
        print('Grade %d: %s' % (count, grade.strip('\n')))
    
    print()
    avg = total / count
    print ('Your average is ', format(avg, '.2f'))

    if avg >= 90:
        print('You have an A')
    elif avg < 90 and avg >= 80:
        print('You have a B')
    elif avg < 80 and avg >= 70:
        print('You have a C')
    elif avg < 70 and avg >= 60:
        print('You have a D')
    else:
        print('You have an F')

except:
    print('That file does not exist.')
