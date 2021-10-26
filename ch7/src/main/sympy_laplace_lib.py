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

res = lt(f)
print(res)

res = inv_lt(res)
print(res)
