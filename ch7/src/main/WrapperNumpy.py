import numpy as np
import random
import datetime

def plus(*list):
    
    sizeList = len(list)
    halfSizeList = int(sizeList/2)
    sqrtHalfSizeList = int(halfSizeList**0.5)
    print ('@Python, HalfSizeList is ' + str(halfSizeList))
    print ('@Python, SqrtHalfSizeList is ' + str(sqrtHalfSizeList))

    print ('@Python, Arguments Size is ' + str(sizeList))
    for index in range(0, sizeList):
        print ('@Python, Argument' + str(index) + ' is ' + str(list[index]))

    list1  = list[:halfSizeList]    
    list2 = list[halfSizeList:]    
    
    mat1 = np.array(list1).reshape((sqrtHalfSizeList,sqrtHalfSizeList))
    mat2 = np.array(list2).reshape((sqrtHalfSizeList,sqrtHalfSizeList))
    print ('')
    print ('@Python, Matrix1 is ')
    print (mat1)
    print ('@Python, Matrix2 is ')
    print (mat2)
    print ('')

    result = mat1 + mat2
    print ('@Python, Matrix1 + Matrix2 is ')
    print (result)

    listResult = np.asarray(result).reshape(-1)
    return tuple(listResult)
