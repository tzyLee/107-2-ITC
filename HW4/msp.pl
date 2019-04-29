/* Reversed kadane's algorithm */
/* Base case*/
maxSum([],0).
/* Maximum subarray sum starts with X = X or X + maxSum(Xs)
max(Result, 0) to deal with negative number*/
maxSum([X|Xs],Sum):-
    maxSum(Xs,RestSum), Sum is max(max(X, X+RestSum), 0).
