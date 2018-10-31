
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
