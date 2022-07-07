#Program Description: takes a price and determines the most efficient way to meet that price in terms of bills and coins

def main():
    #The introduction
    print('Hello!  Welcome to the currency calculator.')
    print('Enter in a valid price (in decimal) and we\'ll tell you how much of each bill and\n coin you\'ll need to best meet that amount.')
    print()
    try:#tests for input validity
        price_input = float(input('Enter in a price: '))
        price = round(price_input, 2)
        #finds bill, gives remainder, then finds coins
        price = calcBills(price)
        print()
        calcCoins(price)
    except:#if input is not valid, repeats
        print('input not valid, please enter a number price')
        print()
        main()

def calcBills(num):#calculates number of bills
    hundreds = num//100
    num = num - 100*hundreds
    fifties = num//50
    num = num - 50*fifties
    twenties = num//20
    num = num - 20*twenties
    tens = num//10
    num = num - 10*tens
    fives = num//5
    num = num - 5*fives
    ones = num//1
    num = num - 1*ones
    displayBills(hundreds, fifties, twenties, tens, fives, ones)
    return num
    
def displayBills(hundreds, fifties, twenties, tens, fives, ones):#displays bill
    print('The paper bills you will need is: ')
    print(int(hundreds), 'hundreds')
    print(int(fifties), 'fifties')
    print(int(twenties), 'twenties')
    print(int(tens), 'tens')
    print(int(fives), 'fives')
    print(int(ones), 'ones')
    

def calcCoins(num):#calculates number of coins
    quarters = num//.25
    num = num - .25*quarters
    dimes = num//.10
    num = num - .10*dimes
    nickels = num//.05
    num = num - .05*nickels
    pennies = num//.01
    displayCoins(quarters, dimes, nickels, pennies)

def displayCoins(quarters, dimes, nickels, pennies):#displays coins
    print('The coins you will need is: ')
    print(int(quarters), 'quarters')
    print(int(dimes), 'dimes')
    print(int(nickels), 'nickels')
    print(int(pennies), 'pennies')

main()
