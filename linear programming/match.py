import numpy as np
import graph as graph
import opt as op

dim = graph.graph.shape[0]

zeros = np.zeros((graph.graph.shape))
costs = np.vstack([graph.graph, zeros])
zeros = np.zeros((costs.shape))
costs = np.hstack([zeros,costs])*-1

ones = np.ones((graph.graph.shape))
zeros = np.zeros((graph.graph.shape))
capacities = np.vstack([ones, zeros])
zeros = np.zeros((capacities.shape))
capacities = np.hstack([zeros,capacities])

b1 = np.ones((dim,1))
b2 = np.ones((dim,1))*-1
b = np.vstack([b1,b2])
print("Cost, capacity graphs built.")
sol = op.min_cost_flow(costs,capacities,b)
for i in range(dim):
	for j in range(dim):
		if sol[0].item((i,j+dim))==1:
			if i>=len(graph.companyData):
				print("no match for "+ graph.proData[j]["Name"])
			elif j>=len(graph.proData):
				print("no match for "+ graph.companyData[i]["Company"])
			else:
				print(graph.companyData[i]["Company"]+" + "+graph.proData[j]["Name"])
