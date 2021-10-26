#!/usr/bin/python

from sympy import *
init_printing()

import matplotlib.pyplot as plt

t, s = symbols('t, s')
a = symbols('a', real=True, positive=True)

f = exp(-a*t)
print(f)

res = integrate(f * exp(-s*t), (t, 0, oo))
print(res)
