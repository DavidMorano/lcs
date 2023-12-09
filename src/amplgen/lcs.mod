# LCS (MODEL)


param N;


param s1 {n in 0..N-1} integer; 
param s2 {n in 0..N-1} integer; 

var s1o {i in 0..N-1, j in 0..N-1} binary;
var s2o {i in 0..N-1, j in 0..N-1} binary;
 
maximize objective: sum{i in 0..N-1,j in 0..N-1} s1o[i,j];

subject to c1 {i in 0..N-1}: sum {j in 0..N-1} s1o[i,j] <= 1;
subject to c2 {i in 0..N-1}: sum {j in 0..N-1} s2o[i,j] <= 1;

subject to c3 {j in 0..N-1}: sum {i in 0..N-1} s1o[i,j] <= 1;
subject to c4 {j in 0..N-1}: sum {i in 0..N-1} s2o[i,j] <= 1;

subject to c5 {i1 in 0..N-1, i2 in 0..N-1: i1 <= i2}:
 N*(1 - sum {j in 0..N-1} s1o[i2,j]) + sum {j in 0..N-1} j*s1o[i2,j]
 - sum {j in 0..N-1} j*s1o[i1,j] >= 0;

subject to c6 {i1 in 0..N-1, i2 in 0..N-1: i1 <= i2}:
 N*(1 - sum {j in 0..N-1} s2o[i2,j]) + sum {j in 0..N-1} j*s2o[i2,j]
 - sum {j in 0..N-1} j*s2o[i1,j] >= 0;

subject to c7 {i in 0..N-1, c in 0..N-1}:
 sum {k in 0..N-1:s1[k] = c} s1o[i,k] = sum {k in 0..N-1:s2[k] = c} s2o[i,k];

subject to c8 {i in 0..N-2}: 
   sum {j in 0..N-1} s1o[i,j] >= sum {j in 0..N-1} s1o[i+1,j];

subject to c9 {i in 0..N-2 }: 
   sum {j in 0..N-1 } s2o[i,j] >= sum {j in 0..N-1} s2o[i+1,j];


