#define PY_SSIZE_T_CLEAN
#include <Python.h>

void initialize(char* module_name, PyObject **pModule)
{
    Py_Initialize();

	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append(\".\")");

	PyObject *pName = PyUnicode_FromString("my_module"); // pyc_lib.py를 PyObject로 생성한다.
    *pModule = PyImport_Import(pName);

    Py_DECREF(pName);
}

void finalize(PyObject *pModule)
{
    Py_DECREF(pModule);
    Py_FinalizeEx();
}

void make_string(PyObject *pModule, char *arg)
{
    PyObject *pFunc, *pArgs, *pValue;

    pFunc = PyObject_GetAttrString(pModule, "make_string");
    pArgs = PyTuple_New(1);

    PyTuple_SetItem(pArgs, 0, PyUnicode_FromString(arg));
    pValue = PyObject_CallObject(pFunc, pArgs);
    Py_DECREF(pArgs);

    printf("Result of call: %s \n", PyUnicode_AsUTF8(pValue));
    Py_DECREF(pValue);
    Py_DECREF(pFunc);
}


void make_int(PyObject *pModule, int arg)
{
    PyObject *pFunc, *pArgs, *pValue;

    pFunc = PyObject_GetAttrString(pModule, "make_int");
    pArgs = PyTuple_New(1);

    PyTuple_SetItem(pArgs, 0, PyLong_FromLong(arg));
    pValue = PyObject_CallObject(pFunc, pArgs);
    Py_DECREF(pArgs);

    printf("Result of call: %ld \n", PyLong_AsLong(pValue));
    Py_DECREF(pValue);
    Py_DECREF(pFunc);

}

void make_list(PyObject *pModule)
{
    PyObject *pFunc, *pArgs, *pValue;

    pFunc = PyObject_GetAttrString(pModule, "make_list");

    pValue = PyObject_CallObject(pFunc, NULL);

    printf("Result of call: \n");
    for (int i = 0; i < PyList_Size(pValue); ++i)
    {
        printf("\t%dst item: %s \n", i, PyUnicode_AsUTF8(PyList_GetItem(pValue, i)));
    }
    Py_DECREF(pValue);
    Py_DECREF(pFunc);
}

int main(int argc, char *argv[])
{
    PyObject *pName, *pModule, *pFunc;
    PyObject *pArgs, *pValue;
    int i;

    //initialize(argv[1], &pModule);
    initialize("my_module", &pModule);

    make_string(pModule, "Hello World!");
    make_int(pModule, 99);
    make_list(pModule);

    finalize(pModule);

    return 0;
}

#if 0
int main(int argc, char *argv[])
{
    wchar_t *program = Py_DecodeLocale(argv[0], NULL);
	PyObject *pName, *pModule, *pFunc, *pValue, *pArg;
	PyObject *objectsRepresentation, *ptemp, *str;
	PyObject *pyTupleIn, *pyTupleOut;

	char *ret_val;
	int count;
	int *arr;
	int i;

	double in[] = { 1, 4, 7, 2, 3, 1, 9, 0 };
	double *output;
	double *input;

	input = (double *)malloc(sizeof(double) * 8);

	for (i = 0; i < 8; i++)
	{
		input[i] = in[i];
	}

    if (program == NULL)
	{
        fprintf(stderr, "Fatal error: cannot decode argv[0]\n");
        exit(1);
    }

    Py_SetProgramName(program);
    Py_Initialize();

	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append(\".\")");

	pName = PyUnicode_FromString("WrapperNumpy"); // pyc_lib.py를 PyObject로 생성한다.
	pModule = PyImport_Import(pName); // 생성한 PyObject pName을 import한다.
	pFunc = PyObject_GetAttrString(pModule, "plus"); // 실행할 함수인 ret_intarr을 PyObject에 전달한다.

	CreatePyTuple(input, pyTupleIn, 8);

	//pArg = Py_BuildValue("(list)", input);
	//pyTupleOut = PyObject_CallObject(pFunc, pArg);
	pyTupleOut = PyObject_CallObject(pFunc, pyTupleIn);
	ConvertPyTupleToVector(pyTupleOut, output);

	printf("sizeof(output): %ld\n", sizeof(output));
	//pArg = Py_BuildValue("(list)", input);
	//output = PyObject_CallObject(pFunc, pArg); // pFunc에 매개변수를 전달해서 실행한다.


#if 0
	count = (int)PyList_Size(pValue);
	arr = (int *)malloc(sizeof(int) * count);
#endif

#if 0

#endif

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
#endif
