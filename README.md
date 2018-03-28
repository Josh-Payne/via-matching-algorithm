# via-matching-algorithm
This repository contains an algorithm for matching Via Fellows with partner projects. 

At a high level, the idea is to arrange a gridded graph with one axis being fellows and another being projects. The algorithm finds the optimal path from the top of the graph to the bottom without touching the same row or column twice. (As a side note, this is likely an NP-hard problem, so we'll eventually need to find a way to approximate efficiently with other techniques like machine learning when the number of fellows or projects gets sufficiently large.) 

At this point, the matching algorithm is implemented to optimize, but unfortunately, this is indeed NP-hard so we'll need to find a way to approximate. 

An added efficiency includes cleaning the graph - the ability to set a threshold where pairings with scores lower than this are removed from it.