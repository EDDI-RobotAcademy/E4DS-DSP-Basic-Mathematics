#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    wchar_t *program = Py_DecodeLocale(argv[0], NULL);
	PyObject *pName, *pModule, *pFunc, *pValue, *pArg;
	PyObject *objectsRepresentation, *str;
	char *res;

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
	pFunc = PyObject_GetAttrString(pModule, "ret_string"); // 실행할 함수인 ret_string을 PyObject에 전달한다.

	pArg = Py_BuildValue("(z)", (const char *)"hello"); // 문자열 hello를 담은 매개변수를 만든다.
	pValue = PyObject_CallObject(pFunc, pArg); // pFunc에 매개변수를 전달해서 실행한다.

	objectsRepresentation = PyObject_Repr(pValue); // 객체를 문자열로 표현한다.
	str = PyUnicode_AsEncodedString(objectsRepresentation, "utf-8", "~E~"); // 객체를 인코딩한다.
	res = PyBytes_AsString(str);
	printf("return value: %s\n", res);

	Py_Finalize();

    PyMem_RawFree(program);

    return 0;
}
