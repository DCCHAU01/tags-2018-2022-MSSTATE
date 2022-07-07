#Program Description: a book url is given, upon which a formula calculates the reading level reading level
from math import ceil
from urllib import request

def main():
    #url)http://www.gutenberg.org/cache/epub/18857/pg18857.txt
    name()
    
#asks for webname prompt
def name():
    url = str(input('Enter web page of the book to be evaluated: \n'))
    open_url(url)
    return

#opens page, reads it
def open_url(url):
    page = request.urlopen(url)
    text = page.read().decode("utf8")
    readability_level(text)
    return

#calculates number of words, sentences, syllables and prints score
def readability_level(text):
    #takes each word in document and appends to list called words
    words = text.split()
    num_words = 0
    num_sentences = 0
    num_syllables = 0

    #reiterates each word in document and adds to word count
    for i in words:
        num_words += 1
        #if there is a period, adds to sentence count
        if i.endswith('.'):
            num_sentences += 1
        #does ceiling function of length of word divided by 4
        #adds the answer to syllable count
        if ceil(len(i)/4) >= 1:
            num_syllables += ceil(len(i)/4)
        #if ceiling function divided by 4 is less than 1, add 1 to syllable count
        elif ceil(len(i)/4) < 1:
            num_syllables += 1
            
    #finds average sentence length and syllables per word
    avg_sen_len = num_words/ num_sentences
    avg_syl_per_word = num_syllables / num_words
    #the FKRA equation, used to find reading score
    FKRA = (0.39 * avg_sen_len) + (11.8 * avg_syl_per_word) - 15.59

    #displays the averages and reading level of document
    print ('Average syllables per word: ', avg_syl_per_word)
    print ('Average sentence length: ', avg_sen_len)
    print ('Reading Level: ', FKRA)
    
##    print (num_words)
##    print (num_sentences)
##    print (num_syllables)
    
    return
main()
