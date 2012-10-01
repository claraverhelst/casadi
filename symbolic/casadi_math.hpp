/*
 *    This file is part of CasADi.
 *
 *    CasADi -- A symbolic framework for dynamic optimization.
 *    Copyright (C) 2010 by Joel Andersson, Moritz Diehl, K.U.Leuven. All rights reserved.
 *
 *    CasADi is free software; you can redistribute it and/or
 *    modify it under the terms of the GNU Lesser General Public
 *    License as published by the Free Software Foundation; either
 *    version 3 of the License, or (at your option) any later version.
 *
 *    CasADi is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public
 *    License along with CasADi; if not, write to the Free Software
 *    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#ifndef CASADI_MATH_HPP
#define CASADI_MATH_HPP

#include "casadi_calculus.hpp"
namespace CasADi{

template<template<int> class F>
bool operation_checker(unsigned int op){
  switch(op){
    case OP_ASSIGN:        return F<OP_ASSIGN>::check;
    case OP_ADD:           return F<OP_ADD>::check;
    case OP_SUB:           return F<OP_SUB>::check;
    case OP_MUL:           return F<OP_MUL>::check;
    case OP_DIV:           return F<OP_DIV>::check;
    case OP_NEG:           return F<OP_NEG>::check;
    case OP_EXP:           return F<OP_EXP>::check;
    case OP_LOG:           return F<OP_LOG>::check;
    case OP_POW:           return F<OP_POW>::check;
    case OP_CONSTPOW:      return F<OP_CONSTPOW>::check;
    case OP_SQRT:          return F<OP_SQRT>::check;
    case OP_SIN:           return F<OP_SIN>::check;
    case OP_COS:           return F<OP_COS>::check;
    case OP_TAN:           return F<OP_TAN>::check;
    case OP_ASIN:          return F<OP_ASIN>::check;
    case OP_ACOS:          return F<OP_ACOS>::check;
    case OP_ATAN:          return F<OP_ATAN>::check;
    case OP_LT:            return F<OP_LT>::check;
    case OP_LE:            return F<OP_LE>::check;
    case OP_FLOOR:         return F<OP_FLOOR>::check;
    case OP_CEIL:          return F<OP_CEIL>::check;
    case OP_EQUALITY:      return F<OP_EQUALITY>::check;
    case OP_FABS:          return F<OP_FABS>::check;
    case OP_SIGN:          return F<OP_SIGN>::check;
    case OP_ERF:           return F<OP_ERF>::check;
    case OP_FMIN:          return F<OP_FMIN>::check;
    case OP_FMAX:          return F<OP_FMAX>::check;
    case OP_INV:           return F<OP_INV>::check;
    case OP_SINH:          return F<OP_SINH>::check;
    case OP_COSH:          return F<OP_COSH>::check;
    case OP_TANH:          return F<OP_TANH>::check;
    case OP_ASINH:         return F<OP_ASINH>::check;
    case OP_ACOSH:         return F<OP_ACOSH>::check;
    case OP_ATANH:         return F<OP_ATANH>::check;
    case OP_CONST:         return F<OP_CONST>::check;
    case OP_CALL:          return F<OP_CALL>::check;
    case OP_INPUT:         return F<OP_INPUT>::check;
    case OP_OUTPUT:        return F<OP_OUTPUT>::check;
    case OP_ERFINV:        return F<OP_ERFINV>::check;
    case OP_PRINTME:       return F<OP_PRINTME>::check;
    case OP_ATAN2:         return F<OP_ATAN2>::check;
  }
  
  // False by default
  return false;
};


/// Easy access to all the functions for a particular type
template<typename T>
struct casadi_math{

  /** \brief Evaluate a built in function */
  static inline void fun(unsigned char op, const T& x, const T& y, T& f);
  
  /** \brief Evaluate a built in derivative function */
  static inline void der(unsigned char op, const T& x, const T& y, const T& f, T* d);

  /** \brief Evaluate the function and the derivative function */
  static inline void derF(unsigned char op, const T& x, const T& y, T& f, T* d);
  
  /** \brief Number of dependencies */
  static inline int ndeps(unsigned char op);
  
  /** \brief Print */
  static inline void print(unsigned char op, std::ostream &stream, const std::string& x, const std::string& y);
  static inline void printPre(unsigned char op, std::ostream &stream);
  static inline void printSep(unsigned char op, std::ostream &stream);
  static inline void printPost(unsigned char op, std::ostream &stream);
};

/// Specialize the class so that it can be used with integer type
template<>
struct casadi_math<int>{

  /** \brief Evaluate a built in function */
  static inline void fun(unsigned char op, const int& x, const int& y, int& f){
    double f_real(f);
    casadi_math<double>::fun(op,double(x),double(y),f_real);
    f = int(f_real);
  }
  
  /** \brief Evaluate a built in derivative function */
  static inline void der(unsigned char op, const int& x, const int& y, const int& f, int* d){
    double d_real[2] = {double(d[0]),double(d[1])};
    casadi_math<double>::der(op,double(x),double(y),double(f),d_real);
    d[0] = int(d_real[0]);
    d[1] = int(d_real[1]);
  }

  /** \brief Evaluate the function and the derivative function */
  static inline void derF(unsigned char op, const int& x, const int& y, int& f, int* d){
    double d_real[2] = {double(d[0]),double(d[1])};
    double f_real(f);
    casadi_math<double>::derF(op,double(x),double(y),f_real,d_real);
    f = int(f_real);
    d[0] = int(d_real[0]);
    d[1] = int(d_real[1]);
  }
  
  /** \brief Number of dependencies */
  static inline int ndeps(unsigned char op){ return casadi_math<double>::ndeps(op);}
  
  /** \brief Print */
  static inline void print(unsigned char op, std::ostream &stream, const std::string& x, const std::string& y){ casadi_math<double>::print(op,stream,x,y);}
  static inline void printPre(unsigned char op, std::ostream &stream){ casadi_math<double>::printPre(op,stream);}
  static inline void printSep(unsigned char op, std::ostream &stream){ casadi_math<double>::printSep(op,stream);}
  static inline void printPost(unsigned char op, std::ostream &stream){ casadi_math<double>::printPost(op,stream);}
};

// Template implementations

template<typename T>
inline void casadi_math<T>::fun(unsigned char op, const T& x, const T& y, T& f){
// NOTE: We define the implementation in a preprocessor macro to be able to force inlining, and to allow extensions in the VM
#define CASADI_MATH_FUN_BUILTIN(X,Y,F,C,OFF) \
    case OP_ASSIGN+OFF:    C<OP_ASSIGN>::fcn(X,Y,F);        break;\
    case OP_ADD+OFF:       C<OP_ADD>::fcn(X,Y,F);           break;\
    case OP_SUB+OFF:       C<OP_SUB>::fcn(X,Y,F);           break;\
    case OP_MUL+OFF:       C<OP_MUL>::fcn(X,Y,F);           break;\
    case OP_DIV+OFF:       C<OP_DIV>::fcn(X,Y,F);           break;\
    case OP_NEG+OFF:       C<OP_NEG>::fcn(X,Y,F);           break;\
    case OP_EXP+OFF:       C<OP_EXP>::fcn(X,Y,F);           break;\
    case OP_LOG+OFF:       C<OP_LOG>::fcn(X,Y,F);           break;\
    case OP_POW+OFF:       C<OP_POW>::fcn(X,Y,F);           break;\
    case OP_CONSTPOW+OFF:  C<OP_CONSTPOW>::fcn(X,Y,F);      break;\
    case OP_SQRT+OFF:      C<OP_SQRT>::fcn(X,Y,F);          break;\
    case OP_SIN+OFF:       C<OP_SIN>::fcn(X,Y,F);           break;\
    case OP_COS+OFF:       C<OP_COS>::fcn(X,Y,F);           break;\
    case OP_TAN+OFF:       C<OP_TAN>::fcn(X,Y,F);           break;\
    case OP_ASIN+OFF:      C<OP_ASIN>::fcn(X,Y,F);          break;\
    case OP_ACOS+OFF:      C<OP_ACOS>::fcn(X,Y,F);          break;\
    case OP_ATAN+OFF:      C<OP_ATAN>::fcn(X,Y,F);          break;\
    case OP_LT+OFF:        C<OP_LT>::fcn(X,Y,F);            break;\
    case OP_LE+OFF:        C<OP_LE>::fcn(X,Y,F);            break;\
    case OP_FLOOR+OFF:     C<OP_FLOOR>::fcn(X,Y,F);         break;\
    case OP_CEIL+OFF:      C<OP_CEIL>::fcn(X,Y,F);          break;\
    case OP_EQUALITY+OFF:  C<OP_EQUALITY>::fcn(X,Y,F);      break;\
    case OP_FABS+OFF:      C<OP_FABS>::fcn(X,Y,F);          break;\
    case OP_SIGN+OFF:      C<OP_SIGN>::fcn(X,Y,F);          break;\
    case OP_ERF+OFF:       C<OP_ERF>::fcn(X,Y,F);           break;\
    case OP_FMIN+OFF:      C<OP_FMIN>::fcn(X,Y,F);          break;\
    case OP_FMAX+OFF:      C<OP_FMAX>::fcn(X,Y,F);          break;\
    case OP_INV+OFF:       C<OP_INV>::fcn(X,Y,F);           break;\
    case OP_SINH+OFF:      C<OP_SINH>::fcn(X,Y,F);          break;\
    case OP_COSH+OFF:      C<OP_COSH>::fcn(X,Y,F);          break;\
    case OP_TANH+OFF:      C<OP_TANH>::fcn(X,Y,F);          break;\
    case OP_ASINH+OFF:     C<OP_ASINH>::fcn(X,Y,F);         break;\
    case OP_ACOSH+OFF:     C<OP_ACOSH>::fcn(X,Y,F);         break;\
    case OP_ATANH+OFF:     C<OP_ATANH>::fcn(X,Y,F);         break;\
    case OP_ATAN2+OFF:     C<OP_ATAN2>::fcn(X,Y,F);         break; \
    case OP_ERFINV+OFF:    C<OP_ERFINV>::fcn(X,Y,F);        break;\
    case OP_PRINTME+OFF:   C<OP_PRINTME>::fcn(X,Y,F);       break;
  
  #define CASADI_MATH_FUN_ALL_BUILTIN(X,Y,F) \
    CASADI_MATH_FUN_BUILTIN(X,Y,F,BinaryOperation,0)\
    CASADI_MATH_FUN_BUILTIN(X,Y,F,AddBinaryOperation,NUM_BUILT_IN_OPS)\
    CASADI_MATH_FUN_BUILTIN(X,Y,F,SubBinaryOperation,2*NUM_BUILT_IN_OPS)\
    CASADI_MATH_FUN_BUILTIN(X,Y,F,MulBinaryOperation,3*NUM_BUILT_IN_OPS)\
    CASADI_MATH_FUN_BUILTIN(X,Y,F,DivBinaryOperation,4*NUM_BUILT_IN_OPS)
  
  switch(op){
    CASADI_MATH_FUN_ALL_BUILTIN(x,y,f)
  }
}

template<typename T>
inline void casadi_math<T>::der(unsigned char op, const T& x, const T& y, const T& f, T* d){
// NOTE: We define the implementation in a preprocessor macro to be able to force inlining, and to allow extensions in the VM
#define CASADI_MATH_DER_BUILTIN(X,Y,F,D) \
    case OP_ASSIGN:    BinaryOperation<OP_ASSIGN>::der(X,Y,F,D);     break;\
    case OP_ADD:       BinaryOperation<OP_ADD>::der(X,Y,F,D);        break;\
    case OP_SUB:       BinaryOperation<OP_SUB>::der(X,Y,F,D);        break;\
    case OP_MUL:       BinaryOperation<OP_MUL>::der(X,Y,F,D);        break;\
    case OP_DIV:       BinaryOperation<OP_DIV>::der(X,Y,F,D);        break;\
    case OP_NEG:       BinaryOperation<OP_NEG>::der(X,Y,F,D);        break;\
    case OP_EXP:       BinaryOperation<OP_EXP>::der(X,Y,F,D);        break;\
    case OP_LOG:       BinaryOperation<OP_LOG>::der(X,Y,F,D);        break;\
    case OP_POW:       BinaryOperation<OP_POW>::der(X,Y,F,D);        break;\
    case OP_CONSTPOW:  BinaryOperation<OP_CONSTPOW>::der(X,Y,F,D);   break;\
    case OP_SQRT:      BinaryOperation<OP_SQRT>::der(X,Y,F,D);       break;\
    case OP_SIN:       BinaryOperation<OP_SIN>::der(X,Y,F,D);        break;\
    case OP_COS:       BinaryOperation<OP_COS>::der(X,Y,F,D);        break;\
    case OP_TAN:       BinaryOperation<OP_TAN>::der(X,Y,F,D);        break;\
    case OP_ASIN:      BinaryOperation<OP_ASIN>::der(X,Y,F,D);       break;\
    case OP_ACOS:      BinaryOperation<OP_ACOS>::der(X,Y,F,D);       break;\
    case OP_ATAN:      BinaryOperation<OP_ATAN>::der(X,Y,F,D);       break;\
    case OP_LT:        BinaryOperation<OP_LT>::der(X,Y,F,D);         break;\
    case OP_LE:        BinaryOperation<OP_LE>::der(X,Y,F,D);         break;\
    case OP_FLOOR:     BinaryOperation<OP_FLOOR>::der(X,Y,F,D);      break;\
    case OP_CEIL:      BinaryOperation<OP_CEIL>::der(X,Y,F,D);       break;\
    case OP_EQUALITY:  BinaryOperation<OP_EQUALITY>::der(X,Y,F,D);   break;\
    case OP_FABS:      BinaryOperation<OP_FABS>::der(X,Y,F,D);       break;\
    case OP_SIGN:      BinaryOperation<OP_SIGN>::der(X,Y,F,D);       break;\
    case OP_ERF:       BinaryOperation<OP_ERF>::der(X,Y,F,D);        break;\
    case OP_FMIN:      BinaryOperation<OP_FMIN>::der(X,Y,F,D);       break;\
    case OP_FMAX:      BinaryOperation<OP_FMAX>::der(X,Y,F,D);       break;\
    case OP_INV:       BinaryOperation<OP_INV>::der(X,Y,F,D);        break;\
    case OP_SINH:      BinaryOperation<OP_SINH>::der(X,Y,F,D);       break;\
    case OP_COSH:      BinaryOperation<OP_COSH>::der(X,Y,F,D);       break;\
    case OP_TANH:      BinaryOperation<OP_TANH>::der(X,Y,F,D);       break;\
    case OP_ASINH:     BinaryOperation<OP_ASINH>::der(X,Y,F,D);      break;\
    case OP_ACOSH:     BinaryOperation<OP_ACOSH>::der(X,Y,F,D);      break;\
    case OP_ATANH:     BinaryOperation<OP_ATANH>::der(X,Y,F,D);      break;\
    case OP_ATAN2:      BinaryOperation<OP_ATAN2>::der(X,Y,F,D);     break;\
    case OP_ERFINV:    BinaryOperation<OP_ERFINV>::der(X,Y,F,D);     break;\
    case OP_PRINTME:   BinaryOperation<OP_PRINTME>::der(X,Y,F,D);    break;
  
  switch(op){
    CASADI_MATH_DER_BUILTIN(x,y,f,d)
  }
}


template<typename T>
inline void casadi_math<T>::derF(unsigned char op, const T& x, const T& y, T& f, T* d){
// NOTE: We define the implementation in a preprocessor macro to be able to force inlining, and to allow extensions in the VM
#define CASADI_MATH_DERF_BUILTIN(X,Y,F,D) \
    case OP_ASSIGN:    DerBinaryOpertion<OP_ASSIGN>::derf(X,Y,F,D);        break;\
    case OP_ADD:       DerBinaryOpertion<OP_ADD>::derf(X,Y,F,D);        break;\
    case OP_SUB:       DerBinaryOpertion<OP_SUB>::derf(X,Y,F,D);        break;\
    case OP_MUL:       DerBinaryOpertion<OP_MUL>::derf(X,Y,F,D);        break;\
    case OP_DIV:       DerBinaryOpertion<OP_DIV>::derf(X,Y,F,D);        break;\
    case OP_NEG:       DerBinaryOpertion<OP_NEG>::derf(X,Y,F,D);        break;\
    case OP_EXP:       DerBinaryOpertion<OP_EXP>::derf(X,Y,F,D);        break;\
    case OP_LOG:       DerBinaryOpertion<OP_LOG>::derf(X,Y,F,D);        break;\
    case OP_POW:       DerBinaryOpertion<OP_POW>::derf(X,Y,F,D);        break;\
    case OP_CONSTPOW:  DerBinaryOpertion<OP_CONSTPOW>::derf(X,Y,F,D);   break;\
    case OP_SQRT:      DerBinaryOpertion<OP_SQRT>::derf(X,Y,F,D);       break;\
    case OP_SIN:       DerBinaryOpertion<OP_SIN>::derf(X,Y,F,D);        break;\
    case OP_COS:       DerBinaryOpertion<OP_COS>::derf(X,Y,F,D);        break;\
    case OP_TAN:       DerBinaryOpertion<OP_TAN>::derf(X,Y,F,D);        break;\
    case OP_ASIN:      DerBinaryOpertion<OP_ASIN>::derf(X,Y,F,D);       break;\
    case OP_ACOS:      DerBinaryOpertion<OP_ACOS>::derf(X,Y,F,D);       break;\
    case OP_ATAN:      DerBinaryOpertion<OP_ATAN>::derf(X,Y,F,D);       break;\
    case OP_LT:        DerBinaryOpertion<OP_LT>::derf(X,Y,F,D);         break;\
    case OP_LE:        DerBinaryOpertion<OP_LE>::derf(X,Y,F,D);         break;\
    case OP_FLOOR:     DerBinaryOpertion<OP_FLOOR>::derf(X,Y,F,D);      break;\
    case OP_CEIL:      DerBinaryOpertion<OP_CEIL>::derf(X,Y,F,D);       break;\
    case OP_EQUALITY:  DerBinaryOpertion<OP_EQUALITY>::derf(X,Y,F,D);   break;\
    case OP_FABS:      DerBinaryOpertion<OP_FABS>::derf(X,Y,F,D);       break;\
    case OP_SIGN:      DerBinaryOpertion<OP_SIGN>::derf(X,Y,F,D);       break;\
    case OP_ERF:       DerBinaryOpertion<OP_ERF>::derf(X,Y,F,D);        break;\
    case OP_FMIN:      DerBinaryOpertion<OP_FMIN>::derf(X,Y,F,D);       break;\
    case OP_FMAX:      DerBinaryOpertion<OP_FMAX>::derf(X,Y,F,D);       break;\
    case OP_INV:       DerBinaryOpertion<OP_INV>::derf(X,Y,F,D);        break;\
    case OP_SINH:      DerBinaryOpertion<OP_SINH>::derf(X,Y,F,D);       break;\
    case OP_COSH:      DerBinaryOpertion<OP_COSH>::derf(X,Y,F,D);       break;\
    case OP_TANH:      DerBinaryOpertion<OP_TANH>::derf(X,Y,F,D);       break;\
    case OP_ASINH:     DerBinaryOpertion<OP_ASINH>::derf(X,Y,F,D);      break;\
    case OP_ACOSH:     DerBinaryOpertion<OP_ACOSH>::derf(X,Y,F,D);      break;\
    case OP_ATANH:     DerBinaryOpertion<OP_ATANH>::derf(X,Y,F,D);      break;\
    case OP_ATAN2:     DerBinaryOpertion<OP_ATAN2>::derf(X,Y,F,D);      break;\
    case OP_ERFINV:    DerBinaryOpertion<OP_ERFINV>::derf(X,Y,F,D);     break;\
    case OP_PRINTME:   DerBinaryOpertion<OP_PRINTME>::derf(X,Y,F,D);    break;
  
  switch(op){
    CASADI_MATH_DERF_BUILTIN(x,y,f,d)
  }
}

template<typename T>
inline int casadi_math<T>::ndeps(unsigned char op){
#define CASADI_MATH_BINARY_BUILTIN \
    case OP_ADD:\
    case OP_SUB:\
    case OP_MUL:\
    case OP_DIV:\
    case OP_POW:\
    case OP_CONSTPOW:\
    case OP_LT:\
    case OP_LE:\
    case OP_EQUALITY:\
    case OP_FMIN:\
    case OP_FMAX:\
    case OP_ATAN2:\
    case OP_PRINTME:
  
  switch(op){
    case OP_CONST:
    case OP_PARAMETER:
    case OP_INPUT:
      return 0;
    CASADI_MATH_BINARY_BUILTIN
      return 2;
    default:
      return 1;
  }
}

template<typename T>
inline void casadi_math<T>::print(unsigned char op, std::ostream &stream, const std::string& x, const std::string& y){
  if(ndeps(op)==2){
    printPre(op,stream);
    stream << x;
    printSep(op,stream);
    stream << y;
    printPost(op,stream);
  } else {
    printPre(op,stream);
    stream << x;
    printPost(op,stream);
  }
}

template<typename T>
inline void casadi_math<T>::printPre(unsigned char op, std::ostream &stream){
  switch(op){
    case OP_ASSIGN:                          break;
    case OP_ADD:       stream << "(";        break;
    case OP_SUB:       stream << "(";        break;
    case OP_MUL:       stream << "(";        break;
    case OP_DIV:       stream << "(";        break;
    case OP_NEG:       stream << "(-";       break;
    case OP_EXP:       stream << "exp(";     break;
    case OP_LOG:       stream << "log(";     break;
    case OP_POW:       stream << "pow(";     break;
    case OP_CONSTPOW:  stream << "pow(";     break;
    case OP_SQRT:      stream << "sqrt(";    break;
    case OP_SIN:       stream << "sin(";     break;
    case OP_COS:       stream << "cos(";     break;
    case OP_TAN:       stream << "tan(";     break;
    case OP_ASIN:      stream << "asin(";    break;
    case OP_ACOS:      stream << "acos(";    break;
    case OP_ATAN:      stream << "atan(";    break;
    case OP_ASINH:     stream << "asinh(";   break;
    case OP_ACOSH:     stream << "acosh(";   break;
    case OP_ATANH:     stream << "atanh(";   break;
    case OP_LT:        stream << "(";        break;
    case OP_LE:        stream << "(";        break;
    case OP_FLOOR:     stream << "floor(";   break;
    case OP_CEIL:      stream << "ceil(";    break;
    case OP_EQUALITY:  stream << "(";        break;
    case OP_FABS:      stream << "fabs(";    break;
    case OP_SIGN:      stream << "sign(";    break;
    case OP_ERF:       stream << "erf(";     break;
    case OP_FMIN:      stream << "fmin(";    break;
    case OP_FMAX:      stream << "fmax(";    break;
    case OP_INV:       stream << "(1./";     break;
    case OP_SINH:      stream << "sinh(";    break;
    case OP_COSH:      stream << "cosh(";    break;
    case OP_TANH:      stream << "tanh(";    break;
    case OP_ATAN2:     stream << "atan2(";   break;
    case OP_ERFINV:    stream << "erfinv(";  break;
    case OP_PRINTME:   stream << "printme("; break;
  }
}

template<typename T>
inline void casadi_math<T>::printSep(unsigned char op, std::ostream &stream){
  switch(op){
    case OP_ADD:       stream << "+";        break;
    case OP_SUB:       stream << "-";        break;
    case OP_MUL:       stream << "*";        break;
    case OP_DIV:       stream << "/";        break;
    case OP_LT:        stream << "<";        break;
    case OP_LE:        stream << "<=";       break;
    case OP_EQUALITY:  stream << "==";       break;
    default:           stream << ",";        break;
  }
}

template<typename T>
inline void casadi_math<T>::printPost(unsigned char op, std::ostream &stream){
  switch(op){
    case OP_ASSIGN:                       break;
    default:        stream << ")";        break;
  }
}

} // namespace CasADi

#endif //CASADI_MATH_HPP