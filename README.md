### empyrical_cpp
empyrical_cpp is a header-only project using C++20 re-implement the "empyrical" project, 
which was originally written in Python.

### using 
```git 
git clone https://github.com/cloudQuant/empyrical_cpp.git
cd empyrical_cpp
cmake -B build
cmake --build build
# 运行测试
./build/empyrical_test
```
### why re-implement the empyrical module

- practise my c++ coding skills
- used for quantitative finance, eg, financial data analysis, time processing, performance measurement,
  performance attribution, and so on.
- used for other modules, eg, pyfolio and alphalens, which I will re-implement in c++.
- as a module, it saves the computational methods I need for quantitative finance processes in the future

### target

-[ ] run successfully in windows
-[ ] update the class of DateTime, add timezone support and improve computational speed。
-[ ] add new functions to perf_attrib.h 
-[ ] re-implement the pyfolio using c++20 in another module 
-[ ] re-implement the alphalens using c++20 in another module 


