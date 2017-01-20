# mt4-numpy-example
Example to call numpy function from dll with MT4.

## Test Environment
- Windows 10 x64
- Metatrader 4 Build 1031
- Visual Studio 2010

### Use x86 Python
Use same os version of your MT4.

### Add Include path to 2 header files `Python.h` and `numpy/arrayobject.h`.
It'll be like
```
C:\Python27\include;C:\Python27\lib\site-packages\numpy\core\include;
```

### Add Library path.
It'll be like

```
C:\Python27\libs
```

### NumpyFunction
#### GetNumpyMean
returns the mean value of input value.
```
MT4_EXPFUNC double __stdcall GetNumpyMean(double *value, int arraysize, int period, int index)
```
- value : input value
- arraysize : size of input value
- period : moving average period
- index of value like MQL iMA
