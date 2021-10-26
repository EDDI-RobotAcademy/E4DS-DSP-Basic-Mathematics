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

res0 = res_invlt.subs({t: 0.0})
res1 = res_invlt.subs({t: 0.1})
res2 = res_invlt.subs({t: 0.2})
res3 = res_invlt.subs({t: 0.3})

print(res0)
print(res1)
print(res2)
print(res3)
