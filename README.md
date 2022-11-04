# Luca-Corner-Stitching

## Usage
```console
$ make
$ ./Lab1 <INPUT-NAME> <OUTPUT-NAME>
```
For example
```console
$ ./Lab1 data/case0.txt data/ans_by_me/output0.txt
$ ./Lab1 data/case1.txt data/ans_by_me/output1.txt
$ ./Lab1 data/case2.txt data/ans_by_me/output2.txt
$ ./Lab1 data/case7.txt data/ans_by_me/output7.txt
$ cat data/ans_by_me/output0.txt
$ cat data/ans_by_me/output1.txt
$ cat data/ans_by_me/output2.txt
$ cat data/ans_by_me/output7.txt
```

If you want to rebuild, please use `make cleanall` before `make`
```console
$ make cleanall
$ make
$ ./Lab1 <INPUT-NAME> <OUTPUT-NAME>
```

## Source Folder Layout
```console
└── lcs
    ├── definitions.hpp
    ├── functional.hpp
    ├── geometry.hpp
    ├── input.hpp
    ├── layout.hpp
    ├── lcs.hpp
    └── utility.hpp
```

## Demo Code
```console
examples/
└── demo_example.cpp
```