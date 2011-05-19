set terminal postscript
set output 'lte-mcs-index.eps'
set xlabel 'SINR [dB]'
set ylabel 'MCS index'

set grid
set key bottom right
plot '-'  title 'QPSK' with linespoints lw 4 pt 8, '-'  title '16QAM' with linespoints lw 4 pt 4, '-'  title '64QAM' with linespoints lw 4 pt 10
-2 0
-1 0
0 2
1 2
2 2
3 4
4 4
5 6
6 6
7 8
8 8
9 10
e
10 12
11 12
12 14
13 14
14 16
15 18
16 18
e
17 20
18 20
19 22
20 22
21 24
22 24
23 26
24 26
25 28
26 28
27 28
28 28
29 28
30 28
e
