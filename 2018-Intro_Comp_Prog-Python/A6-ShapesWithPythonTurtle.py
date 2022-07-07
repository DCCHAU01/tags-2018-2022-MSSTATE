#Program Description: using Python Turtle to create functions that draw certain shapes

import turtle
from math import pi
from random import randint

#setting up turtle's basic stats
window = turtle.Screen()
window.bgcolor("black")
tdrawer = turtle.Turtle()
tcolor = tdrawer.color('black')
tshape = tdrawer.shape('triangle')
tsize = tdrawer.turtlesize(0.5)
tspeed = tdrawer.speed(1000)
tpencolor = tdrawer.pencolor('red')
tdrawer.showturtle()

def draw_circle(x, y, radius):
    tdrawer.pu()
    tdrawer.setx(x)
    tdrawer.sety(y)
    tdrawer.setheading(0)
    tdrawer.fd(radius)
    tdrawer.setheading(90)
    circumference = (2*pi*radius)
    sizeofside = circumference / 360
    tdrawer.pd()
    for i in range(360):
        tdrawer.forward(sizeofside)
        tdrawer.left(1)
    tdrawer.setheading(0)


draw_circle(0,0,75)


def draw_rectangle(x, y, w, h):
    tdrawer.pu()
    tdrawer.setx(x)
    tdrawer.sety(y)
    tdrawer.setheading(0)
    tdrawer.pd()
    for i in range(2):
        tdrawer.fd(w)
        tdrawer.left(90)
        tdrawer.fd(h)
        tdrawer.left(90)
    tdrawer.setheading(0)


draw_rectangle(-100, -100, 25, 75)


def cross_hair(x, y, size):
    tdrawer.pu()
    tdrawer.setx(x)
    tdrawer.sety(y)
    tdrawer.setheading(0)
    tdrawer.pd()
    for i in range(4):
        tdrawer.fd(size/2)
        tdrawer.bk(size/2)
        tdrawer.right(90)
    tdrawer.setheading(0)

for i in range(25):
    xr = randint(-300,300)
    yr = randint(75,300)
    zr = randint(30,100)
    cross_hair(xr,yr,zr)



def line(x, y, a, b):
    tdrawer.pu()
    tdrawer.setx(x)
    tdrawer.sety(y)
    tdrawer.pd()
    angle = tdrawer.towards(a,b)
    tdrawer.left(angle)
    distancetoend = tdrawer.distance(a,b)
    tdrawer.fd(distancetoend)
    tdrawer.right(angle)

for i in range(40,63,3):
    line(-100,100,i,i)

def equilateral_triangle(x, y, side):
    tdrawer.pu()
    tdrawer.setx(x)
    tdrawer.sety(y)
    tdrawer.setheading(60)
    tdrawer.pd()
    for i in range(3):
        tdrawer.fd(side)
        tdrawer.right(120)
    tdrawer.setheading(0)

for i in range(0,100,5):
    equilateral_triangle(i,-100,i)

tdrawer.hideturtle()
