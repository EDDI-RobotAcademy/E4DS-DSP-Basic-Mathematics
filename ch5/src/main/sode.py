#!/usr/bin/python

from sympy import *
import matplotlib.pyplot as plt

y = symbols('y', cls=Function)
x = symbols('x')

res = dsolve(y(x).diff(x, 2) + y(x).diff() + y(x), ics = {y(0):1, y(x).diff(x).subs(x, 0):0})
#plt.plot(res.rhs, xlim=(-5, 10), ylim=(-7, 3))
plot(res.rhs, xlim=(-5, 10), ylim=(-7, 3))

#deq = Eq( 10*y(t).diff(t,2) + 10*y(t).diff(t) + 90*y(t), 0 )
#sol = dsolve( deq, ics= { y(0):0.16, y(t).diff(t).subs(t,0):0 } )
#plot( sol.rhs, xlim=(0,8), ylim=(-0.5,0.5) )

res = dsolve(10 * y(x).diff(x, 2) + 10 * y(x).diff() + 90 * y(x), ics = {y(0):0.16, y(x).diff(x).subs(x, 0):0})
#plt.plot(res.rhs, xlim=(0, 8), ylim=(-0.5, 0.5))
plot(res.rhs, xlim=(0, 8), ylim=(-0.5, 0.5))

print(res.rhs)
