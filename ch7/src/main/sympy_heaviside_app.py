#!/usr/bin/python

from sympy import *
init_printing()

import matplotlib.pyplot as plt

t, s = symbols('t, s')
a = symbols('a', real=True, positive=True)
f = exp(-a*t)

def lt(f):
	return laplace_transform(f, t, s, noconds=True)

def inv_lt(F):
	return inverse_laplace_transform(F, s, t)

res_lt = laplace_transform(f, t, s, noconds=True)
print(res_lt)

res_invlt = inv_lt(res_lt).subs({a: 2})
print(res_invlt)

p = plot(f.subs({a: 2}), inv_lt(res_lt).subs({a: 2}),
               xlim=(-1, 4), ylim=(0, 3), show=False)
p[1].line_color = 'red'
p.show()
