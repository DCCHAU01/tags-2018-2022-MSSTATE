#Date Due: Oct.29, 2018
#Program Description: finds the rank of the inputted name in the selected file


def main():
    find()

def find():
    #Prompts to find the file, name, and gender
    file_prompt = input('Plese enter the file name(with extension): ')
    name_prompt = input('Enter babyname: ')
    gender_prompt = input('Enter the gender of the baby (m/f): ')
    
    #splits the inputted name into first and last name variables
    first, last = name_prompt.split()
    file = open(file_prompt, 'r')

    #strips unneeded text from file name to find the year
    file_name_list = file_prompt.strip('baby_names_.txt')

    #skips first four lines of given file
    for i in range(4):
        file.readline()
    #splits read information into separate variables
    for line in file:
        rank, m_name, m_num, f_name, f_num = line.split()
        #checks for a match.  if a match is found, then rank is printed
        if first == m_name:
            print (file_name_list+'s', "rank:", rank)
        if first == f_name:
            print(file_name_list+'s', "rank:", rank)
    
main()

