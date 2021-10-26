#define PY_SSIZE_T_CLEAN
//#include <python3.8/Python.h>
#include <Python.h>

int main(int argc, char *argv[])
{
    wchar_t *program = Py_DecodeLocale(argv[0], NULL);
	PyObject *pName, *pModule, *pFunc, *pValue;

    if (program == NULL)
	{
        fprintf(stderr, "Fatal error: cannot decode argv[0]\n");
        exit(1);
    }

    Py_SetProgramName(program);
    Py_Initialize();

	PyRun_SimpleString("import sys");
	PyRun_SimpleString("import os");
	//PyRun_SimpleString("sys.path.append(os.getcwd())");
	PyRun_SimpleString("sys.path.append('/home/oem/proj/e4ds/E4DS-DSP-Basic-Mathematics/ch7/src/main')");

	pName = PyUnicode_FromString("./pyc_lib"); // pyc_lib.py를 PyObject로 생성한다.
	printf("pName: 0x%x", pName);

	pModule = PyImport_Import(pName); // 생성한 PyObject pName을 import한다.
	printf("pModule: 0x%x", pModule);

	pFunc = PyObject_GetAttrString(pModule, "hello_test"); // 실행할 함수인 hello_test을 PyObject에 전달한다.
	printf("pFunc: 0x%x", pFunc);

	pValue = PyObject_CallObject(pFunc, NULL); // pFunc에 매개변수를 전달해서 실행한다. 현재 매개변수가 NULL인 경우이다.
	printf("pValue: 0x%x", pValue);

	Py_Finalize();

    PyMem_RawFree(program);

    return 0;
}
