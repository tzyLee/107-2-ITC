/* Use 0 if Result is negative */
maxSum(X, Sum):-
    maxSumImpl(X, _, Sum).
/* Reversed kadane's algorithm (Tail recursive) */
/* Base case
 maxSumStartWithHead([], 0).
   Maximum subarray sum starts with X = X or X + maxSumStartWithHead(Xs)
 maxSumStartWithHead([X|Xs], MaxStartWithX):-
     maxSumStartWithHead(Xs, MaxAfterX),
	 MaxStartWithX is max(X, X+MaxAfterX).
*/

maxSumImpl([], 0, 0).
maxSumImpl([X|Xs], MaxStartWithX, OverallMax):-
    maxSumImpl(Xs, MaxAfterX, OverallMaxWithoutX),
	MaxStartWithX is max(X, X+MaxAfterX),
	OverallMax is max(OverallMaxWithoutX, MaxStartWithX).
	
/* Tests

maxSum([], A).
maxSum([1], A).
maxSum([-1], A).
maxSum([1, 2, -4, 5, 1], A).
maxSum([-1, 2, -3, 3], A).

*/
