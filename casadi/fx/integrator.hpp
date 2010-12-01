#ifndef INTEGRATOR_HPP
#define INTEGRATOR_HPP

#include "../expression_tools.hpp"
#include "fx.hpp"

namespace CasADi{

/** \brief  Input arguments of an integrator */
enum IntegratorInput{INTEGRATOR_T0, INTEGRATOR_TF, INTEGRATOR_X0, INTEGRATOR_P, INTEGRATOR_XP0, INTEGRATOR_NUM_IN};

/** \brief  Output arguments of an integrator */
enum IntegratorOutput{INTEGRATOR_XF, INTEGRATOR_XPF, INTEGRATOR_NUM_OUT};

  /** \brief  Input arguments of an explicit ODE right hand side */
enum ODEInput{ODE_T, ODE_Y, ODE_P, ODE_NUM_IN};

/** \brief  Output arguments of an explicit ODE right hand side */
enum ODEOutput{ODE_RHS, ODE_NUM_OUT};

/** \brief  Input arguments of an DAE residual function */
enum DAEInput{DAE_T, DAE_Y, DAE_YDOT, DAE_P, DAE_NUM_IN};

/** \brief  Output arguments of an DAE residual function */
enum DAEOutput{DAE_RES, DAE_NUM_OUT};

/** \brief  Forward declaration of internal class */
class IntegratorInternal;

/** Integrator class
  An "integrator" is a function that solves an initial value problem (IVP) of the generic form:
  
  F(x,der(x),p,t) == 0
  x(t0) = x0

  It has 4 inputs, initial time, final time, initial state (vector-valued) and parameter (vector-valued) and one output, the state at the final time. 
  In addition to this, the integrator provides some additional functionality, such as getting the value of the state and/or sensitivities at certain time points.
  Controls are assumed to be parametrized at this point. 
    
  The class does not specify how the function F above should be represented, nor the method used for the integration, but assumes that it steps forward in time (ruling out collocation in particular). 
  The actual form of the ODE/DAE is defined in the derived classes.
    
  inputs:
  0: Initial time t0 (dimension 1-by-1)
  1: Final time tf (dimension 1-by-1)
  2: State at t0  (dimension nx-by-1)
  3: p  (dimension np-by-1)
  
  outputs:
  0: y(tf)
  
*/
class Integrator : public FX{
public:
  /** \brief  Constructors */
  Integrator();
  
  /** \brief  Print solver statistics */
  void printStats(std::ostream &stream=std::cout) const;
  
  /** \brief  Access functions of the node */
  IntegratorInternal* operator->();
  const IntegratorInternal* operator->() const;

  /** \brief  Reset the solver and bring the time back to t0 */
  void reset(int fsens_order, int asens_order=0);

  /** \brief  Integrate until a specified time point */
  void integrate(double t_out);

  /** \brief  Set a stop time for the forward integration */
  void setStopTime(double tf);

  /// Assert that the node is pointing to the right type of object
  void assertNode() const;
  
};


} // namespace CasADi

#endif //INTEGRATOR_HPP
