# via-matching-algorithm
This repository contains an algorithm for matching Via Fellows with partner projects. 

At a high level, the idea is to arrange a gridded graph with one axis being fellows and another being projects. The algorithm finds the optimal path from the top of the graph to the bottom without touching the same row or column twice. 

I've tried a brute force approach and minimum-cost flow linear program.