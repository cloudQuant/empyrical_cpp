# empyrical_cpp
empyrical_cpp is a header-only project using C++20 re-implement the "empyrical" project, 
which was originally written in Python.

<p style="text-align: center;">
    <img src="https://img.shields.io/badge/version-0.1-blueviolet.svg" alt="Version 0.1"/>
    <img src="https://img.shields.io/badge/platform-mac|linux-yellow.svg" alt="Supported Platforms: Mac and Linux"/>
    <img src="https://img.shields.io/badge/build-passing-brightgreen" alt="Build: Passing"/>
    <img src="https://img.shields.io/badge/license-MIT-orange" alt="License: MIT"/>
</p>

### how to use it
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


