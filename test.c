#include <python2.7/Python.h>
#include <Python.h>
#include <stdlib.h>
#include <stdio.h>
void processPython(int, char**, char*, char*);
/***************************************
 Constants to files and corresponding 
 functions
*****************************************/
char *automatedFile = "grove_light_sensor";
char *fAuto = "LightSensor";

char *blinkF = "grove_led_blink";
char *fBlink = "blink";


int main(int argc, char *argv[])
{
    processPython(argc, argv, blinkF, fBlink);
    return 0;
}

void processPython(int argc, char *argv[], char *pfile, char *pfunction){
PyObject *pName, *pModule, *pDict, *pFunc;
    PyObject *pArgs, *pValue;
    int i;
    /*
    if (argc < 3) {
        fprintf(stderr,"Usage: call pythonfile funcname [args]\n");
        exit(1);
        //return 1;
    }
    */
    Py_Initialize();

    PyRun_SimpleString("import sys"); 
    PyRun_SimpleString("sys.path.append(\"/home/pi/Desktop/LightDemo\")");



    pName = PyString_FromString(pfile);
    /* Error checking of pName left out */

    pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if (pModule != NULL) {
        pFunc = PyObject_GetAttrString(pModule, pfunction);
        /* pFunc is a new reference */

        if (pFunc && PyCallable_Check(pFunc)) {
            pArgs = PyTuple_New(argc - 3);
            for (i = 0; i < argc - 3; ++i) {
                pValue = PyInt_FromLong(atoi(argv[i + 3]));
                if (!pValue) {
                    Py_DECREF(pArgs);
                    Py_DECREF(pModule);
                    fprintf(stderr, "Cannot convert argument\n");
                    exit(1);
                    //return 1;
                }
                /* pValue reference stolen here: */
                PyTuple_SetItem(pArgs, i, pValue);
            }
            pValue = PyObject_CallObject(pFunc, pArgs);
            Py_DECREF(pArgs);
            if (pValue != NULL) {
                printf("Result of call: %ld\n", PyInt_AsLong(pValue));
                Py_DECREF(pValue);
            }
            else {
                Py_DECREF(pFunc);
                Py_DECREF(pModule);
                PyErr_Print();
                fprintf(stderr,"Call failed\n");
                exit(1);
                //return 1;
            }
        }
        else {
            if (PyErr_Occurred())
                PyErr_Print();
            fprintf(stderr, "Cannot find function \"%s\"\n", pfunction);
        }
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
    }
    else {
        PyErr_Print();
        fprintf(stderr, "Failed to load \"%s\"\n", pfile);
         exit(1);
        //return 1;
    }
    Py_Finalize();

}