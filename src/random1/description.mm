.\"_ ECE3484 algorithm writeup 
.\"_
.nr N 2
.\"_
.\"_ heading at level three (and below) start a new line
.nr Hb 3
.\"_
.PF "'ECE3484 Project'Longest Common Subsequence'Phase 2'"
.\"_
.EQ
delim $$
define linbo3 % Ti:LiNbO sub 3 %
.EN
.\"_
.\"_
\s+4\fBECE3484 Spring 1998, Project Part #2
.SP 2
.\"_
.DS C
.B
\s+2Greedy Algorithm for Longest Common Subsequence\s-2
.R
.SP 2
\s+2\fIJohn Hilliar\fP\s-2
\s+2\fIDavid Morano\fP\s-2
.DE
.\"_
.SP 3
.\"_
.H 1 "Introduction"
.P
This paper gives a brief description of the Greedy algorithm as applied to 
the
problem of finding the longest common subsequence (LCS) in two or more
strings (the LCS of string is itself).  
It was not clear what was the "best" Greedy Algorithm to apply
to this problem.  By "best", I mean most representative of showing
the Greediness of the Greedy Algorithm.  To start off, it is not
clear what constitutes a Greedy start in the problem of finding the
LCS of strings.  Most Greedy algorithms of other problems start
by finding a possible solution but by starting with what might
constitute the best solution if not iterated further.
In this problem, since the only measure of a common subsequence
in this problem is the length of the subsequence, we cannot start
by finding the longest common subsequence because that would be
the same as solving the problem in its entirety.  Instead, we
assume that the first common subsequence that we can find is the
best one to start off with.  The Greediness of the Greedy algorithm
now dictates that we want to try and see if we can create a longer
subsequence out of the one that we already have without ever
abandoning the one we have for another possible start.
.\"_
.H 1 "Our Greedy Algorithm"
.P
The algorithm implemented is as follows.
We first start to find a common subsequence by selecting 
one of the input strings as a search guide.
We then consider, left to right each of the characters in the
selected string as the start of a possible common subsequence.
A search 
is then performed 
on all other strings 
looking for the character focused on from the
selected string.  If all of the other strings also contain
the focus character, then we have found a common subsequence.
If none of the other strings contain that character, then we
move on to the next sequential character in the selected guide string
and use it as a possible start of a common subsequence.
If we go through all of the characters of the selected string without
finding any common subsequences, then we know that there are 
no common subsequences in this set of input
strings.
.P
Once a common subsequence is found, rather than abandoning it
and searching through other dimensions of the problem (which
is exponential in its entirety), we assume that it is the
start of the longest subsequence since it was found first.
There is merit in this line of reasoning but there could be a longer
LCS by considering all of the other common subsequences
found after the first one.  Further, there could be a longer
common subsequence by starting off with each of the other input strings
also, as compared with sticking with the first one that we found.
Using the given common subsequence discovered so far, we try
to augment it by seeing if additional characters can be appended
that are also common to all strings with the requirement that
all following characters must be found in the other strings
after the last character was found that is in the current
common subsequence.
.\"_
.H 1 "Code Subroutine Descriptions"
.P
Our Greedy algorithm is implemented reasonably straight forwardly
in the code within the subroutine
\f(CWgreedy\fP.  The first \fIfor\fP loop finds a common subsequence,
if there is one, and then the second \fIfor\fP loop tries to append
characters to it that are common to all of the other strings.
Of course, only characters found after the previous character
in any given string are considered as common characters since
the order of the common subsequence has to be the same in all strings.
.P
The \f(CWmain\fP procedure simply handles some program
invocation arguments (the input string data files)
and calls the procedure \f(CWprocfile\fP to read and process the input strings
found in the input files.
The \f(CWmain\fP procedure also
prints out any resulting LCS that is found.
.P
The \f(CWprocfile\fP procedure
reads in the input strings and
stores then in a convenient data structure used by the Greedy algorithm
subroutine.  It handles the various differences that there can
be in the input format also.
.P
Finally, there are a couple of extra subroutines located along with the
\f(CWgreedy\fP subroutine that perform some sub-functions of the Greedy
algorithm.  The only important one being searching all other strings
to see if it contains the same character as the focus character
from the selected guide string.  The subroutine which performs this
task is named \f(CWpresent\fP.
.\"_
.H 1 "An Exhaustive Algorithm"
.P
In solving this problem, it is very evident that
it is exponential in processing time.  There are a number of
algorithm approaches to this problem but I will
consider one to illustrate the exponential nature of the
problem.  If we try to extend the Greedy algorithm as implemented
above to exhaustive, it becomes clear that when expanding out
the selected string to chose starting characters from.
An exhaustive algorithm would have to consider
every possible subsequence of the selected string.
This is obviously exponential since the number of subsequences
of any string is $ 2 sup n $ where $ N $ is the length of the
string.  Any algorithm which does not consider all of these
possibilities (even if the time to consider each is vastly different
to due cleverness in pruning search times), could miss the
finding the LCS of the problem.
.\_"
.H 1 "Program Restrictions and Input Format"
.P
There are no restrictions on the number of files containing any
number of input strings.  Also there is no restriction on 
the length of any of the input strings either.  Of course, this restriction
only holds until the process of allocating more memory on the
computer fails !  The input format for strings is very flexible
in that any number of strings can appear in a file (each on a separate
line) and any number of these files may be specified.
All of these strings are the input and a LCS is searched for
using all of them.
.\"_
.\"_
.\"_
