#
#     This file is part of CasADi.
# 
#     CasADi -- A symbolic framework for dynamic optimization.
#     Copyright (C) 2010 by Joel Andersson, Moritz Diehl, K.U.Leuven. All rights reserved.
# 
#     CasADi is free software; you can redistribute it and/or
#     modify it under the terms of the GNU Lesser General Public
#     License as published by the Free Software Foundation; either
#     version 3 of the License, or (at your option) any later version.
# 
#     CasADi is distributed in the hope that it will be useful,
#     but WITHOUT ANY WARRANTY; without even the implied warranty of
#     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#     Lesser General Public License for more details.
# 
#     You should have received a copy of the GNU Lesser General Public
#     License along with CasADi; if not, write to the Free Software
#     Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
# 
# 
#! createParent
#!======================
from casadi import *
from numpy import *

A = MX("A",2,1)         # Here a matrix
B = MX("B",1,2)         # There a matrix
C = MX("C")             # And an other little matrix
D = MX("D",sp_tril(4))  # Triangular matrix


L = [A,B,C,D]

for m in L:
  print m, " = ", array(m.sparsity())

V, (A,B,C,D) = createParent(L)

# The code below has been commented out (it relies on the mapping() function which no longer exists
print V[A.mapping()]

for m,n in zip(L,[A,B,C,D]):
  print m, " => ", n, " = ", array(n.sparsity())
