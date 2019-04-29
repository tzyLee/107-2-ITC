/* Use 0 if Result is negative */
maxSum(X, Sum):-
    maxSumStartWithHead(X, Result), Sum is max(Result, 0).
/* Reversed kadane's algorithm */
/* Base case*/
maxSumStartWithHead([],0).
/* Maximum subarray sum starts with X = X or X + maxSumStartWithHead(Xs)*/
maxSumStartWithHead([X|Xs],Sum):-
    maxSumStartWithHead(Xs,RestSum), Sum is max(X, X+RestSum), 0).
