#Program Description: Determining if a point is outside, inside, or on a rectangle

#llc=lower left corner
llcx = int(input('Enter the x coordinate of the lower lefthand corner of the rectangle: '))
llcy = int(input('Enter the y coordinate of the lower lefthand corner of the rectangle: '))

#height of rectangle and width
hr = int(input('Enter the height of the rectangle: '))
wr = int(input('Enter the width of the rectangle: '))

#the x & y coordinate of the point
xc = int(input('Enter the x cooridnate of the point: '))
yc = int(input('Enter the y coordinate of the point: '))

#----------------------------------------------------
#lower left corner print
print ('lower left corner is ', llcx, llcy)

#lrc = lower right corner
lrcx = wr
lrcy = llcy
print ('lower right corner is ', lrcx, lrcy)

#ulc = upper left corner
ulcx = llcx
ulcy = hr
print ('upper left corner is ', ulcx, ulcy)

#urc = upper right corner
urcx = wr
urcy = hr
print ('upper right corner is ', urcx, urcy)
#----------------------------------------------------

#Finding if point is on bottom line
if xc <= lrcx and xc >= llcx and yc == llcy:
    print ('point is on rectangle\'s bottom line')
else:
    #right line
        if xc == lrcx and yc <= urcy and yc >= lrcy:
            print ('point is on rectangle\'s right line')
        else:
        #top line
            if xc <= urcx and xc >= ulcx and yc == urcy:
                print('point is on the rectangle\'s top line')
            else:
            #left line
                if xc == llcx and yc <= ulcy and yc >= llcy:
                    print('point is on the rectangle\'s left line')
                #upper right corner
                elif (xc * yc) == (urcx * urcy):
                    print ('point is on the rectangle\'s upper right corner')
                    #upper left corner
                elif xc == llcx and yc == ulcy:
                    print ('point is on the rectangle\'s upper left corner')
                    #lower left corner
                elif xc == llcx and yc == llcy:
                    print ('point is on the rectangle\'s lower left corner')
                    #lower right corner
                elif xc == lrcx and yc == lrcy:
                    print ('point is on the rectangle\'s lower right corner')

                    #Inside the Rectangle
                    #given points < area of rectangle
                    #and given points > lower left points
                elif (xc * yc) < (urcx * urcy) and (xc * yc) > (llcx * llcy):
                    print ('point is inside the rectangle')
                else:
                    print ('point is outside the rectangle')
