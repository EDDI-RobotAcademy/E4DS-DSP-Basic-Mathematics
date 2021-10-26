#!/usr/bin/python

from sympy import *
init_printing()

import matplotlib.pyplot as plt

t, s = symbols('t, s')
a = symbols('a', real=True, positive=True)
f = exp(-a*t)

res = laplace_transform(f, t, s, noconds=True)
print(res)
