# OS 2

#### Computing Standard Deviation with 2-processes on a multi-core CPU.

### Usage for sequential operation
```
gcc seq.c -lm -o output.out

./output.out file1.txt file2.txt
```

### Sample output of sequential operation

```
 Sequential program launched!
 Reading file1.txt
 Reading file2.txt
 Timer started.
 The mean is 52.35729
 The standard deviation is 49.04489
 Calculation completed in 19 milliseconds.
```

### Usage for multiprocess operation

```
gcc mul.c -lm -o output.out

./output.out file1.txt file2.txt
```

### Sample output of multiprocess operation

```
Multiple program launched!
Reading file1.txt
Reading file2.txt
Timer started.
The mean is 52.357285
The standard deviation is 49.04489
Calculation completed in 9 milliseconds.
```
