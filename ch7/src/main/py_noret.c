#define PY_SSIZE_T_CLEAN
//#include <python3.8/Python.h>
#include <Python.h>

int main(int argc, char *argv[])
{
	// 없어도 무방
    wchar_t *program = Py_DecodeLocale(argv[0], NULL);
	PyObject *pName, *pModule, *pFunc, *pValue;

    if (program == NULL)
	{
        fprintf(stderr, "Fatal error: cannot decode argv[0]\n");
        exit(1);
    }

    Py_SetProgramName(program);
	// 파이썬을 C와 연동하기 위한 초기 설정을 진행함
    Py_Initialize();

	// 만약 이를 응용하여 응용 솔루션을 제공하는 경우라면
	// 실제 시스템 환경 변수쪽에 *.py 라이브러리를 이동시켜서
	// 언제든지 라이브러리가 호출될 수 있도록 만들어줘야합니다!
	PyRun_SimpleString("import sys");
	//PyRun_SimpleString("import os");
	//PyRun_SimpleString("sys.path.append(os.getcwd())");
	//PyRun_SimpleString("sys.path.append('/home/oem/proj/e4ds/E4DS-DSP-Basic-Mathematics/ch7/src/main')");
	PyRun_SimpleString("sys.path.append(\".\")");

	pName = PyUnicode_FromString("pyc_lib"); // pyc_lib.py를 PyObject로 생성한다.
	printf("pName: %p\n", pName);

	pModule = PyImport_Import(pName); // 생성한 PyObject pName을 import한다.
	printf("pModule: %p\n", pModule);

	pFunc = PyObject_GetAttrString(pModule, "hello_test"); // 실행할 함수인 hello_test을 PyObject에 전달한다.
	printf("pFunc: %p\n", pFunc);

	pValue = PyObject_CallObject(pFunc, NULL); // pFunc에 매개변수를 전달해서 실행한다. 현재 매개변수가 NULL인 경우이다.
	printf("pValue: %p\n", pValue);

	Py_Finalize();

    PyMem_RawFree(program);

    return 0;
}
