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
Parent has read file1.txt
Child has read file2.txt
The parent started the timer.
Partial Sum is computed on parent
Partial Sum is computed on child
Child sent Partial sum to Parent
The global average is computed on parent
Mean is 52.35729
Parent sent global average to child
Partial variance is computed on parent
Partial variance is computed on child
Child sent partial variance to parent
The standard deviation is computed on parent
The standard deviation: 49.04489
Calculation completed in 8 milliseconds.
```
