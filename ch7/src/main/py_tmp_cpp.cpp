#ifdef _DEBUG
#define _DEBUG_WAS_DEFINED 1
#undef _DEBUG
#endif
#include <Python.h>
#ifdef _DEBUG_WAS_DEFINED
#define _DEBUG 1
#endif

// C++ 헤더 ////////////////////////////////
#include <iostream>
#include <vector>
using namespace std;

// 임베디드 파이썬을 위한 함수 선언 ////////
bool RunPython(string fileName, string functionName,
	PyObject*& input, PyObject*& output);
bool RunPython(string fileName, string functionName);
bool RunPython(string fileName, string functionName,
	vector<double>& input, vector<double>& output);
void CreatePyTuple(vector<double>& input, PyObject*& output);
bool ConvertPyTupleToVector(PyObject*& input, vector<double>& output);

bool _PyUnicode_FromString(const char* input, PyObject*& output);
bool _PyImport_Import(PyObject*& input, PyObject*& output);
bool _PyObject_GetAttrString(PyObject*& object, const char* functionName, PyObject*& output);

////////////////////////////////////////////

int main()
{
	string fileNamePython0 = "InitSystemPath"; // InitSystemPath.py
	string funcNamePython0 = "InitSystemPath"; // Function Name
	string fileNamePython1 = "WrapperNumpy";
	string funcNamePython2 = "plus";

	// 파이썬에 입력할 값 
	vector<double> input, output;
	input.push_back(1); // for matrix1 (0,0)
	input.push_back(4); // for matrix1 (0,1)
	input.push_back(7); // for matrix1 (1,0)
	input.push_back(2);	// for matrix1 (1,1)
	input.push_back(3); // for matrix2 (0,0)
	input.push_back(1); // for matrix2 (0,1)
	input.push_back(9); // for matrix2 (1,0)
	input.push_back(0);	// for matrix2 (1,1)

	// C++에서 입력값들 출력
	for (int index = 0; index < input.size(); index++)
		cout << "@C++, Input" << index << " is " << input[index] << endl;
	cout << endl;

	Py_Initialize();
	RunPython(fileNamePython0, funcNamePython0); // InitSystemPath
	RunPython(fileNamePython1, funcNamePython2, input, output); // WrapperNumpy, plus

	// C++에서 출력값들 출력
	cout << endl;
	for (int index = 0; index < output.size(); index++)
		cout << "@C++, Output" << index << " is "<< output[index] << endl;

	Py_Finalize();

	return 0;
}

// 임베디드 파이썬을 위한 함수 정의 (1) ////////////////////////////////

bool RunPython(
	string fileName, string functionName,
	PyObject*& input, PyObject*& output)
{
	PyObject* pName;   // Python File Name (*.py)
	PyObject* pModule; // Python Module
	PyObject* pFunc;   // Python Function on Module
	PyObject* pArgs;   // Arguments for Python Function

	if (!_PyUnicode_FromString(fileName.c_str(), pName))
	{
		printf("No File Name");
		return false;
	}
	if (!_PyImport_Import(pName, pModule))
	{
		printf("No Module");
		return false;
	}
	if (!_PyObject_GetAttrString(pModule, functionName.c_str(), pFunc))
	{
		printf("No Function");
		return false;
	}
	output = PyObject_CallObject(pFunc, input);
}

bool RunPython(string fileName, string functionName)
{
	PyObject* input = NULL;
	PyObject* output;
	return RunPython(fileName, functionName, input, output);
}

bool RunPython(
	string fileName, string functionName,
	vector<double>& input, vector<double>& output)
{
	PyObject* pytupleInput, *pytupleOutput;
	CreatePyTuple(input, pytupleInput);
	if (!RunPython(fileName, functionName, pytupleInput, pytupleOutput)) return false;
	//if (!RunPython(fileName, functionName, pytupleInput, pytupleOutput)) return false;

	if (!ConvertPyTupleToVector(pytupleOutput, output)) return false;

	return true;
}

void CreatePyTuple(vector<double>& input, PyObject*& output)
{
	output = PyTuple_New(input.size());
	for (int index = 0; index < input.size(); index++)
		//PyTuple_SetItem(output, index, Py_BuildValue("i", input[index]));
		PyTuple_SetItem(output, index, PyLong_FromLong(input[index]));
}

bool ConvertPyTupleToVector(PyObject*& input, vector<double>& output)
{
	if (input == NULL) return false;

	int size = PyTuple_Size(input);
	for (int index = 0; index < size; index++)
	{
		double value = PyLong_AsLong(PyTuple_GetItem(input, index));
		output.push_back(value);
	}
	return true;
}

// 임베디드 파이썬을 위한 함수 정의 (2) ////////////////////////////////

bool _PyUnicode_FromString(const char* input, PyObject*& output)
{
	output = PyUnicode_FromString(input);
	return output != NULL;
}
bool _PyImport_Import(PyObject*& input, PyObject*& output)
{
	output = PyImport_Import(input);
	return output != NULL;
}
bool _PyObject_GetAttrString(PyObject*& object, const char* functionName, PyObject*& output)
{
	output = PyObject_GetAttrString(object, functionName);
	return output != NULL;
}
