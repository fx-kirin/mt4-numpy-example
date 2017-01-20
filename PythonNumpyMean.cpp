//+------------------------------------------------------------------+
//|                                              Sample DLL for MQL4 |
//|                 Copyright © 2004-2006, MetaQuotes Software Corp. |
//|                                        http://www.metaquotes.net |
//+------------------------------------------------------------------+
#define WIN32_LEAN_AND_MEAN  // Exclude rarely-used stuff from Windows headers
//#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include "Python.h"
#include "numpy/arrayobject.h"
//----
#define MT4_EXPFUNC __declspec(dllexport)

PyObject *NumPy;
PyObject *PyNumpyMeanFunc;

//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
#pragma pack(push,1)
struct RateInfo
  {
   unsigned int      ctm;
   double            open;
   double            low;
   double            high;
   double            close;
   double            vol;
  };
#pragma pack(pop)
//----
struct MqlStr
  {
   int               len;
   char             *string;
  };
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+

BOOL APIENTRY DllMain(HANDLE hModule,DWORD ul_reason_for_call,LPVOID lpReserved)
  {
//----
   switch(ul_reason_for_call)
     {
      case DLL_PROCESS_ATTACH:
          Py_Initialize();
          import_array1(-1);
          PyObject *pName;
          pName = PyString_FromString("numpy");
          NumPy = PyImport_Import(pName);
          Py_DECREF(pName);
          if(NumPy != NULL){
             PyNumpyMeanFunc = PyObject_GetAttrString(NumPy, "mean");
             if(PyNumpyMeanFunc == NULL){
                 OutputDebugString("Failed to get mean func.");
             }
          }else{
              OutputDebugString("Failed to load numpy.");
          }
          OutputDebugString("Attached.");
          break;
      case DLL_THREAD_ATTACH:
          break;
      case DLL_THREAD_DETACH:
          break;
      case DLL_PROCESS_DETACH:
          Py_DECREF(NumPy);
          Py_DECREF(PyNumpyMeanFunc);
          Py_Finalize();
          break;
     }
//----
   return(TRUE);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
MT4_EXPFUNC double __stdcall GetNumpyMean(double *value, int arraysize, int period, int index)
  {
//----
   if(value==NULL)
     {
      //OutputDebugString("GetArrayItemValue: NULL array\n");
      return(0.0);
     }
   if(arraysize < index + period)
     {
      //OutputDebugString("GetArrayItemValue: wrong arraysize \n");
      return(0.0);
     }

   npy_intp npy_arraysize;

   npy_arraysize = period;
   double* shifted_value = (double*)((int)value + sizeof(double)*index);
   PyObject *np_value = PyArray_SimpleNewFromData(1, &npy_arraysize, NPY_DOUBLE, shifted_value);
   Py_INCREF(np_value);;

   PyObject *pArgs = PyTuple_New(1);
   PyTuple_SetItem(pArgs, 0, np_value);
   PyObject *pResult = PyObject_CallObject(PyNumpyMeanFunc, pArgs);
   Py_DECREF(pArgs);
   Py_DECREF(np_value);

   double mean = PyFloat_AsDouble(pResult);
   Py_DECREF(pResult);
//----
   return(mean);
  }
