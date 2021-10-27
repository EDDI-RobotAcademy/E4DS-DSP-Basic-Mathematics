#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    wchar_t *program = Py_DecodeLocale(argv[0], NULL);
	PyObject *pName, *pModule, *pFunc, *pValue, *pArg;
	PyObject *objectsRepresentation, *ptemp, *str;
	int count;
	int *arr;
	char *ret_val;
	int i;

    if (program == NULL)
	{
        fprintf(stderr, "Fatal error: cannot decode argv[0]\n");
        exit(1);
    }

    Py_SetProgramName(program);
    Py_Initialize();

	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append(\".\")");

	pName = PyUnicode_FromString("pyc_lib"); // pyc_lib.py를 PyObject로 생성한다.
	pModule = PyImport_Import(pName); // 생성한 PyObject pName을 import한다.
	pFunc = PyObject_GetAttrString(pModule, "ret_intarr"); // 실행할 함수인 ret_intarr을 PyObject에 전달한다.

	pArg = Py_BuildValue("(i, i, i, i)", 1, 2, 3, 4); // 매개변수를 만든다.
	pValue = PyObject_CallObject(pFunc, pArg); // pFunc에 매개변수를 전달해서 실행한다.

	//count = (int)PyList_Size(pValue);
	//arr = (int *)malloc(sizeof(int) * count);
	printf("Result of call: %ld\n", PyLong_AsLong(pValue));

#if 0
	for (i = 0; i < count; i++)
	{
		ptemp = PyList_GetItem(pValue, i);
		objectsRepresentation = PyObject_Repr(ptemp);
		str = PyUnicode_AsEncodedString(objectsRepresentation, "utf-8", "~E~");
		ret_val = PyBytes_AsString(str);
		arr[i] = strtod(ret_val, NULL);
		printf("arr[%d] = %d\n", i, arr[i]);
	}
#endif

	Py_Finalize();

    PyMem_RawFree(program);

    return 0;
}
