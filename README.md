Last name, first name: Bianca Ciuche
Group: 315 CA

Homework started on: 01.05.2019
Homework finished on: 04.05.2019

Difficulties encountered:
- Problem: Crating a graph that contains strings;
- Solution: I used hashtables where the key is the string, and the value is
            the vertex associated to that key;

- Problem: Computing the distance in a way to avoid memory leaks on last tests;
- Solution: I implemented a BFS that stops when the value is found;

- Problem: Solving task 4 without getting TIME_OUT errors;
- Solution: At the end of task 3 I calculated all the distances between the
        nodes. Locally it worked perfectly, but on vmchecker it gave me
        TIME_OUT error on test 10 for task 3; Also I implemented a sorted
        list so that it would be easier to print the tops;


My solution for the problem:
  First I added all the vertexes and edges given into a graph. I implemented
methodes such as has_road (verifies if it has_road), compute_distance (it has
a bfs that stops when the vertex is found), get_distance (it returns the dist
from vertex's vector of distances) and others in order to solve the tasks.
  In task 4 I used 3 sorted lists of pairs to keep the name of the driver and
the rating, distance and rides. In task 5 I calculated the distances and I 
sorted the results.

Homework complexity:
  In my opininon, it was a medium to hard homework, but I think that if I would
have started it sooner it would have been better;