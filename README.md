# via-matching-algorithm
This repository contains an algorithm for matching Via Fellows with partner projects. 
At a high level, the idea is to arrange a gridded graph with one axis being fellows and another being projects. The algorithm will then need to find theoptimal path from the top of the graph to the bottom without touching the same row or column twice. (As a side note, this is likely an NP-complete problem, so we'll eventually need to find a way to approximate efficiently with other techniques like machine learning when the number of fellows or projects gets sufficiently large.)
