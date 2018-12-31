
Precompute some positions instead of a hashmap for storing which ones are valid.


## Week 4

Constructions in CGAL consumes a lot of time.
Make sure to not test every point twice (path inside a triangle).


## Week 5
### Light at the musem
If using combinations, stores them in LONG!
If using a map to store the reversed pair, make sure that the keys do not exist yet.

### Boats
Try to redo it. 

## Week 6
Make sure that the goal of the flow network is well defined.
Check all corner cases: coinflip, make sure that the person does not
have more points already AND that all remaining points are given
away.

Force some edge to have at least x flow by adding a vertex w in between 
and creating a edge w->target with capacity x.

## Week 7
Linear programming. Check the constraints that I already have, they might 
be useful for other constraitns (absolute values)

## Week 8
Pay extremely attention with id of the edges and the face they belong to.
!!!!! INT ARE VERY SMALL! SOLUTION MIGHT BE RIGHT BUT THERE MIGHT BE AN OVERFLOW !!!!!
So if only very few cases are wrong, this might be the problem.
In LP, when using `i*n + j` make sure that n is the right value to multiply and not m!
When picking numeric_limits, make sure that you don't multiply this number otherwise there will 
be another overflow! If you take 1<<30, make sure it is big enough!

Segmentation faults are probably because you're accessing a vector at a wrong position!

## Week 9
Pay attention to indexes of objects. For instance in the real estate problem 
the site id starts with 1, therefore it is important to do -1 when adding the edges.

If a problem has negative values, you might add a constat to each of them to have 
postive values. It doesn't work always though!

## Week 10
Check input size, maybe a brute force solution is feasible for 50 points!
When using a custom base, make sure to use the right number of symbols!

## Week 11
In linear programming use Gmpq !!!!!!
Also use both sol.is_infesible() || sol.objective_value() > x

Binary search: when checking values L<=x<=R, starts with l=L-1 and r=R+1 and while (l<=r). Then solution is in
l+(r-l)/2
Also. keep again attention to int or long! Print the values, if they are negative, you used int instead of long
