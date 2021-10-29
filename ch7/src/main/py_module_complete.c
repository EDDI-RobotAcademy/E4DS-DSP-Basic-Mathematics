#define PY_SSIZE_T_CLEAN
#include <Python.h>

// 함수 인자의 있는 정보도 해당 함수 스택 프레임 내에 있는 변수
void initialize(char* module_name, PyObject **pModule)
{
    Py_Initialize();

	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append(\".\")");

	PyObject *pName = PyUnicode_FromString("my_module"); // my_module.py를 PyObject로 생성한다.
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
	// Tuple이 뭔가요 ?
	// Tuple은 const(C/C++), final(Java)
	// 왜 const, final, Tuple등을 작성하나요 ? (함수형 언어 계열들)
	// 생산성을 저해하는것이 무엇인가 ?
	// 제품 개발시 협업을 하고 있으며
	// 특히 데이터를 주고 받는 인터페이스가 되는 부분들
	// 현재 들어온 데이터 xxx 인데 이부분을 이렇게 가공하면 해결되겠는데 ?
	// 가공을 해서 데이터 처리 성공! ---> 구동 완료!
	// 데이터를 주고 받는 인터페이스에 변경이 발생하였으므로
	// 다른 정보를 받는 사람들도 같이 영향을 받게됨
	// 애초에 이 데이터 자체가
	// const, final, Tuple이였다면 발생조차 안할 문제였다는 것
    pArgs = PyTuple_New(1);

	// 튜플에 문자열 정보를 저장함
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
	// PyObject *pModule;
	// ----------
	// | 정보값 | 0x1000
	// ~~~~~~~~~~ <<<---- main Stack
	// ----------
	// | 0x1000 | 0x2000
	// ----------
	// ~~~~~~~~~~ <<<---- initialize Stack
	// ----------
	// | 0x2000 | 0x3000
	// ----------
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
