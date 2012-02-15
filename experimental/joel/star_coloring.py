from casadi import *

# This pattern should work fine for unidirectional coloring
A = DMatrix.eye(5)
A[0,:] = 1

# Unidirectional coloring
coloring = A.sparsity().unidirectionalColoring()
print coloring

# Create a symmetric matrix with a for unidirectional coloring "bad" sparsity pattern
A = DMatrix.eye(5)
#A = DMatrix.zeros(5,5)
A[:,-1] = 1
A[-1,:] = 1

# Unidirectional coloring
coloring = A.sparsity().unidirectionalColoring()
print coloring

# Star coloring
coloring = A.sparsity().starColoring()
print coloring

# Largest first ordering
ordering = A.sparsity().largestFirstOrdering()
print "ordering = ", ordering

# Create a function whose hessian has the corresponding sparsity pattern
x = ssym("x",5)
y = sin(x[4])
f = y*(x[0]+x[1]+x[2]+x[3])
ff = SXFunction([x],[f])
ff.init()

gf = ff.jac().trans()
#gf = gf[0]

gff = SXFunction([x],[gf])
gff.init()

hf = gff.jac(0,0,False,True)
hf.printDense()

hff2 = ff.hess()
hff2.printDense()



# Unidi
