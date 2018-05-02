## Adapted from Benjamin Van Roy, Stanford University

import numpy as np
from scipy import optimize as op
import traceback

def linprog(A, b, c, A_eq=None, b_eq=None, lb=None, ub=None, form='symmetric', print_message=True):
	options = {"disp": True, "maxiter": 5000}
	if form=='symmetric':
		sol = op.linprog(c=-np.array(c)[0], A_ub=A, b_ub=np.array(b.T)[0], options=options)
	elif form=='standard':
		sol = op.linprog(c=-np.array(c)[0], A_eq=A, b_eq=np.array(b.T)[0], options=options)
	elif form=='general':
		if (lb is None) and (ub is not None):
			lb = -np.inf*np.ones(ub.shape)
		if (ub is None) and (lb is not None):
			ub = np.inf*np.ones(lb.shape)
		if (ub is not None) and (lb is not None):
			bounds = zip([lb[i,0] for i in range(len(lb))], [ub[i,0] for i in range(len(ub))])
		else:
			bounds = (None, None)
		sol = op.linprog(c=-np.array(c)[0],
							   A_ub=A,
							   b_ub=np.array(b.T)[0],
							   A_eq=A_eq,
							   b_eq=np.array(b_eq.T)[0],
							   bounds=bounds, options=options)
	else:
		print 'ERROR: unrecognized LP form ' + form
		return np.nan, np.nan
	if print_message:
		print sol['message']
	x = np.matrix(sol['x']).T
	objective_value = -sol['fun']
	return x, objective_value

def min_cost_flow(c, u, b):

	n = c.shape[0]
	c = c.flatten().reshape((1, n**2))
	ub = u.flatten().T.reshape((n**2, 1))
	lb = np.zeros((n**2, 1))
	b_eq = b
	A1 = np.repeat(np.identity(n), n, axis=1)
	A2 = np.tile(np.identity(n), (1,n))
	A_eq = A1 - A2
	x, objective_value = linprog(np.zeros((1,n**2)), np.zeros((1,1)), -c, A_eq, b_eq, lb, ub, 'general')
	f = x.reshape((n,n))
	return f, -objective_value