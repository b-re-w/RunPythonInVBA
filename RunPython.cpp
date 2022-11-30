// RunPython.cpp : DLL을 위해 내보낸 함수를 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "RunPython.h"


char * wcharToChar(const wchar_t * pwstrSrc) {
    int nLen = (int)wcslen(pwstrSrc);
    char * pstrDest = (char *)malloc(sizeof(char) * (nLen+1));
    wcstombs(pstrDest, pwstrSrc, nLen + 1);
    return pstrDest;
}

/*
RUNPYTHON_API wchar_t * runPythonInC(int argc, LPCWSTR *args) {
    PyObject *pName, *pModule, *pFunc;
    PyObject *pArgs, *pValue;
    wchar_t *result;

    if (argc < 3) {
        return (wchar_t *)L"{\"code\":1, \"message\":\"Usage: call pythonfile funcname [args]\"}";
    }

    char **argv = (char **)malloc(sizeof(char **) * argc);
    for (size_t i = 0; i < argc; i++) {
        argv[i] = wcharToChar(args[i]);
    }

    Py_SetProgramName(args[0]);
    Py_Initialize();

    pName = PyUnicode_DecodeFSDefault(argv[1]);
    pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if (pModule != NULL) {
        pFunc = PyObject_GetAttrString(pModule, argv[2]);

        if (pFunc && PyCallable_Check(pFunc)) {
            pArgs = PyTuple_New(argc - 3);
            for (int i = 0; i < argc - 3; ++i) {
                pValue = PyUnicode_FromString(argv[i + 3]);
                if (!pValue) {
                    Py_DECREF(pArgs);
                    Py_DECREF(pModule);
                    for (size_t j = 0; j < argc; j++) free(argv[j]);
                    free(argv);
                    if (Py_FinalizeEx() < 0) exit(120);
                    result = (wchar_t *)L"{\"code\":2, \"message\":\"Cannot convert argument\"}";
                    return result;
                }
                PyTuple_SetItem(pArgs, i, pValue);
            }
            pValue = PyObject_CallObject(pFunc, pArgs);
            Py_DECREF(pArgs);
            if (pValue != NULL) {
                result = (wchar_t *)PyUnicode_AsWideCharString(pValue, NULL);
                Py_DECREF(pValue);
            } else {
                Py_DECREF(pFunc);
                Py_DECREF(pModule);
                PyErr_Print();
                result = (wchar_t *)L"{\"code\":5, \"message\":\"Method Call Failed!\"}";
            }
        } else {
            if (PyErr_Occurred()) PyErr_Print();
            result = (wchar_t *)L"{\"code\":4, \"message\":\"Cannot find requested function in the module.\"}";
        }
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
    } else {
        PyErr_Print();
        result = (wchar_t *)L"{\"code\":3, \"message\":\"Cannot load requested module.\"}";
    }

    for (size_t i = 0; i < argc; i++) free(argv[i]);
    free(argv);

    if (Py_FinalizeEx() < 0) {
        exit(120);
    }
    return result;
}*/


RUNPYTHON_API VARIANT _stdcall runPython(int argc, LPCWSTR *args) {
    PyObject *pName, *pModule, *pFunc;
    PyObject *pArgs, *pValue;
    wchar_t *result;

    if (argc < 3) {
        _bstr_t bstrt(L"{\"code\":1, \"message\":\"Usage: call pythonfile funcname [args]\"}");
        return _variant_t(bstrt).Detach();
    }

    char **argv = (char **)malloc(sizeof(char **) * argc);
    for (size_t i = 0; i < argc; i++) {
        argv[i] = wcharToChar(args[i]);
    }

    Py_SetProgramName(args[0]);
    Py_Initialize();

    pName = PyUnicode_DecodeFSDefault(argv[1]);
    pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if (pModule != NULL) {
        pFunc = PyObject_GetAttrString(pModule, argv[2]);

        if (pFunc && PyCallable_Check(pFunc)) {
            pArgs = PyTuple_New(argc - 3);
            for (int i = 0; i < argc - 3; ++i) {
                pValue = PyUnicode_FromString(argv[i + 3]);
                if (!pValue) {
                    Py_DECREF(pArgs);
                    Py_DECREF(pModule);
                    for (size_t j = 0; j < argc; j++) free(argv[j]);
                    free(argv);
                    if (Py_FinalizeEx() < 0) exit(120);
                    _bstr_t bstrt(L"{\"code\":2, \"message\":\"Cannot convert argument\"}");
                    return _variant_t(bstrt).Detach();
                }
                PyTuple_SetItem(pArgs, i, pValue);
            }
            pValue = PyObject_CallObject(pFunc, pArgs);
            Py_DECREF(pArgs);
            if (pValue != NULL) {
                result = (wchar_t *)PyUnicode_AsWideCharString(pValue, NULL);
                Py_DECREF(pValue);
            } else {
                Py_DECREF(pFunc);
                Py_DECREF(pModule);
                PyErr_Print();
                result = (wchar_t *)L"{\"code\":5, \"message\":\"Method Call Failed!\"}";
            }
        } else {
            if (PyErr_Occurred()) PyErr_Print();
            result = (wchar_t *)L"{\"code\":4, \"message\":\"Cannot find requested function in the module.\"}";
        }
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
    } else {
        PyErr_Print();
        result = (wchar_t *)L"{\"code\":3, \"message\":\"Cannot load requested module.\"}";
    }

    for (size_t i = 0; i < argc; i++) free(argv[i]);
    free(argv);

    if (Py_FinalizeEx() < 0) {
        exit(120);
    }
    _bstr_t bstrt(result);
    return _variant_t(bstrt).Detach();
}


RUNPYTHON_API VARIANT _stdcall runPythonTest(void) {
    Py_Initialize();

    PyObject *obj = Py_BuildValue("s", "test.py");
    FILE *file = _Py_fopen_obj(obj, "r+");
    if (file != NULL) {
        PyRun_SimpleString("import os; print(os.getcwd())");
        PyRun_SimpleFile(file, "test.py");
    } else {
        _bstr_t bstrt(L"Test Failed: Module not Found.");
        return _variant_t(bstrt).Detach();
    }

    if (Py_FinalizeEx() < 0) {
        exit(120);
    }
    _bstr_t bstrt(L"Test Finished.");
    return _variant_t(bstrt).Detach();
}


RUNPYTHON_API int _stdcall runPythonGUITest(void) {
    Py_Initialize();

    PyRun_SimpleString(
        "from tkinter import *; import os; import sys\n"
        "print(sys.path)\n"
        "tk = Tk()\n"
        "tk.title(os.getcwd())\n"
        "def Ft2Cm():\n"
        "    ft2cm = entry1.get()\n"
        "    entry2.delete(0,'end')\n"
        "    entry2.insert(0,round(float(ft2cm)*30.48,4))\n"
        "def Cm2Ft():\n"
        "    cm2ft = entry2.get()\n"
        "    entry1.delete(0,'end')\n"
        "    entry1.insert(0,round(float(cm2ft)/30.48,4))\n"
        "label1 = Label(tk,text='피트(ft)').grid(row=0, column=0)\n"
        "label2 = Label(tk,text='센티미터(cm)').grid(row=1,column=0)\n"
        "entry1 = Entry(tk)\n"
        "entry2 = Entry(tk)\n"
        "entry1.grid(row=0,column=1)\n"
        "entry2.grid(row=1,column=1)\n"
        "btn1 = Button(tk,text='ft->cm',bg='black',fg='white',command=Ft2Cm).grid(row=2,column=0)\n"
        "btn2 = Button(tk,text='cm->ft',bg='black',fg='white',command=Cm2Ft).grid(row=2,column=1)\n"
        "tk.mainloop()\n"
        "\n");
    if (Py_FinalizeEx() < 0) {
        exit(120);
    }
    return 20000;
}

