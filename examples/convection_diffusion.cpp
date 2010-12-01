#include <iostream>
#include "casadi/stl_vector_tools.hpp"
#include "casadi/expression_tools.hpp"
#include "ipopt_interface/ipopt_solver.hpp"
#include "sundials_interface/cvodes_integrator.hpp"
#include "ocp_tools.hpp"
#include "casadi/fx/simulator.hpp"
#include "casadi/fx/simulator_internal.hpp"

using namespace CasADi;
using namespace OPTICON;
using namespace std;

// Symbolc representation of the optimal control problem
struct SymbolicOCP{
  // State variables
  SXMatrix x;
  
  // Bounds on the state
  vector<double> x_lb, x_ub;
  
  // Initial values of the states
  vector<double> x_init;

  // Controls
  SXMatrix u;
  
  // Parameters
  vector<SX> p;
  
  // Bounds on the control
  vector<double> u_lb, u_ub;
  
  // Initial values
  vector<double> u_init;

  // Dynamic equation
  SXFunction ffcn;
  
  // Objective function
  SXFunction mfcn;
  
  // Path constraint function
  SXFunction cfcn;
  
  // Final time
  double tf;
};

// Formulate the optimal control problem
void model(SymbolicOCP &ocp){
  // Time 
  SX t("t");
  
  // Diffusion coefficient
  SX D = 1e-9;
/*  SX D("D");
  ocp.u = D;
  ocp.u_lb.push_back(1e-9);
  ocp.u_ub.push_back(1e-9);
  ocp.u_init.push_back(1e-9);*/
  
  // Speed
  SX v = 2-4*t;
  
  // Number of discretizations
  int n = 30;
  
  // Temperatures
  SXMatrix T("T",n);
  
  ocp.x = T;
  
  // Bounds on the states
  ocp.x_lb = vector<double>(ocp.x.numel(),-numeric_limits<double>::infinity());
  ocp.x_ub = vector<double>(ocp.x.numel(),numeric_limits<double>::infinity());
  ocp.x_init.resize(n,0);
  for(int i=0; i<n/2; ++i)
    ocp.x_init[i] = 1;
    
  // Time horizon
  ocp.tf = 1;

  // Length
  
  // step size
  SX dz = 1.0/n;
  SX dz2 = dz*dz;
  
  // Approximation of dT_dz for positive v
  SXMatrix dT_dz_pos(n,1);
  dT_dz_pos[0] = (2-T[0])/dz;
  for(int i=1; i<n; ++i){
    dT_dz_pos[i] = (T[i-1]-T[i])/dz;
  }
  
  // Approximation of dT_dz for negative v
  SXMatrix dT_dz_neg(n,1);
  for(int i=0; i<n-1; ++i){
    dT_dz_neg[i] = (T[i]-T[i+1])/dz;
  }
  dT_dz_neg[n-1] = (T[n-1]-0.2)/dz;
  
  // Approximation of d2T_dz2
  SXMatrix d2T_dz2(n,1);
  d2T_dz2[0] = (T[1]-T[0])/dz2;
  for(int i=1; i<n-1; ++i){
    d2T_dz2[i] = (T[i-1]-2*T[i]+T[i+1])/dz2;
  }
  d2T_dz2[n-1] = (T[n-2]-T[n-1])/dz2;
  
  // Convection-diffusion equation
  SXMatrix Tdot = v*if_else(v>=0, dT_dz_pos, dT_dz_neg) + D*d2T_dz2;
    
  // Right hand side of the ODE
  vector<SXMatrix> y(3);
  y[0] = t;
  y[1] = T;
//  y[2] = D;
  ocp.ffcn = SXFunction(y,Tdot);
  
  // Objective function (meyer term)
  ocp.mfcn = SXFunction(y,inner_prod(T,T));

  // Nonlinear constraint function
//  ocp.cfcn = SXFunction(y,Tj-Ta);
  
}

int main(){
  cout << "program started" << endl;

  // Formulate the OCP
  SymbolicOCP ocp;
  model(ocp);
  
  // Create an integrator
  Sundials::CVodesIntegrator integrator(ocp.ffcn);
//  integrator.setOption("ad_order",1);
//  integrator.init();

/*  // Pass initial values
  double t0 = 0;
  double tf = 0.00001;
  integrator.setInput(t0, IVP_T0);
  integrator.setInput(tf,IVP_TF);
  integrator.setInput(ocp.x_init,IVP_X_INIT);
  integrator.setInput(ocp.u_init,IVP_P);
  
  cout << ocp.u_init << endl;
  cout << ocp.u << endl;
  
  double seed = 1;
  integrator.setInput(seed,IVP_P,1);
    
  integrator.evaluateFwd();

  vector<double> r(ocp.x_init.size());
  integrator.getOutput(r,0,1);
  cout << "der(r) = " << r << endl;
  
  integrator.getOutput(r);
  cout << "r = " << r << endl;
  

  

  return 0;*/
  
  // Create a grid
  int ngrid = 10;
  vector<double> times(ngrid);
  linspace(times,0.0,ocp.tf);
  
  // Create a simulator
  Simulator simulator(integrator,times);
  simulator.init();
  
  // Initial condition
  simulator.input(SIMULATOR_X0).set(ocp.x_init);

  // Pass parameters
  simulator.input(SIMULATOR_P).set(ocp.u_init);
  
  // Simulate
  simulator.evaluate();
  
  // Print the output to screen
  vector<double> T_sim(ngrid*ocp.x.numel());
  simulator.output().get(T_sim);
  cout << "T_sim = " << T_sim << endl;
  
  // Degree of interpolating polynomial
  int K = 3;
  
  // Number of finite elements
  int N = 15;

  // Radau collocation points
  CollocationPoints cp = LEGENDRE;
  
  // Size of the finite elements
  double h = ocp.tf/N;

  // Coefficients of the collocation equation
  vector<vector<double> > C;

  // Coefficients of the continuity equation
  vector<double> D;

  // Get the coefficients
  get_collocation_coeff(K,C,D,cp);
                    
  // Collocated times
  vector<vector<double> > T;
  T.resize(N);
  for(int i=0; i<N; ++i){
    T[i].resize(K+1);
    for(int j=0; j<=K; ++j){
      T[i][j] = h*(i + collocation_points[cp][K][j]);
    }
  }
  
  // Collocated states
  vector< vector< SXMatrix > > X;
  collocate(ocp.x,X,N,K);
  
  // Collocated control (piecewice constant)
  vector< SXMatrix > U;  
  collocate(ocp.u,U,N);

  // State at end time
  SXMatrix XF;

  // 
  bool periodic = true;
  
  if(periodic)
    XF = X[0][0];
  else
    collocate_final(ocp.x,XF);
    
  // All variables with bounds and initial guess
  vector<SX> vars;
  vector<double> vars_lb;
  vector<double> vars_ub;
  vector<double> vars_sol;

  // Loop over the finite elements
  for(int i=0; i<N; ++i){
    // collocated controls 
    for(int r=0; r<ocp.u.numel(); ++r){
      // Add to list of NLP variables
      vars.push_back(SX(U[i][r]));
      vars_lb.push_back(ocp.u_lb[r]);
      vars_ub.push_back(ocp.u_ub[r]);
      vars_sol.push_back(ocp.u_init[r]);
    }

    // Collocated states
    for(int j=0; j<=K; ++j){
      for(int r=0; r<ocp.x.numel(); ++r){
        // Add to list of NLP variables
        vars.push_back(SX(X[i][j][r]));
        vars_sol.push_back(ocp.x_init[r]);
        if(i==0 && j==0 && !periodic){
          // Initial constraints
          vars_lb.push_back(ocp.x_init[r]);
          vars_ub.push_back(ocp.x_init[r]);
        } else {
          // Variable bounds
          vars_lb.push_back(ocp.x_lb[r]);
          vars_ub.push_back(ocp.x_ub[r]);
        }
      }
    }
  }

  if(!periodic){
    // Add states at end time
    for(int r=0; r<ocp.x.numel(); ++r){
      vars.push_back(SX(XF[r]));
      vars_sol.push_back(ocp.x_init[r]);
      vars_lb.push_back(ocp.x_lb[r]);
      vars_ub.push_back(ocp.x_ub[r]);
    }
  }
  
  // Constraint function for the NLP
  SXMatrix g;
  vector<double> lbg,ubg;
  for(int i=0; i<N; ++i){
    for(int k=1; k<=K; ++k){
      // augmented state vector
      vector<SXMatrix> y_ik(3);
      y_ik[0] = T[i][k];
      y_ik[1] = X[i][k];
//       y_ik[2] = U[i];

      // Add collocation equations to NLP
      SXMatrix rhs(ocp.x.numel(),1);
      for(int j=0; j<=K; ++j)
        rhs += X[i][j]*C[j][k];
      g << h*ocp.ffcn.eval(y_ik)[0] - rhs;
      lbg.insert(lbg.end(),ocp.x.numel(),0); // equality constraints
      ubg.insert(ubg.end(),ocp.x.numel(),0); // equality constraints
      
      // Add nonlinear constraints
/*      g << ocp.cfcn(y_ik);
      lbg.insert(lbg.end(),1,0);
      ubg.insert(ubg.end(),1,numeric_limits<double>::infinity());*/
      
    }

   // Add continuity equation to NLP
   SXMatrix rhs(ocp.x.numel(),1);
   for(int j=0; j<=K; ++j)
     rhs += D[j]*X[i][j];

   if(i<N-1)
     g << X[i+1][0] - rhs;
   else
     g << XF - rhs;
   lbg.insert(lbg.end(),ocp.x.numel(),0); // equality constraints
   ubg.insert(ubg.end(),ocp.x.numel(),0); // equality constraints
  }
  
  SXMatrix v = vertcat(vars);
  SXFunction gfcn_nlp(v,g);
  
  // Objective function of the NLP
  vector<SXMatrix> y_f(3);
  y_f[0] = T.back().back();
  y_f[1] = XF;
/*  y_f[2] = U.back();*/
  SXMatrix f = ocp.mfcn.eval(y_f)[0];
  SXFunction ffcn_nlp(v, f);

#if 0
  // Hessian of the Lagrangian:
  // Lagrange multipliers
  SXMatrix lambda("lambda",g.size());

  // Objective function scaling
  SXMatrix sigma("sigma");

  // Lagrangian function
  vector<SXMatrix> lfcn_input(3);
  lfcn_input[0] = v;
  lfcn_input[1] = lambda;
  lfcn_input[2] = sigma;
  SXFunction lfcn(lfcn_input, sigma*f + trans(lambda)*g);
  
  // Hessian of the Lagrangian
  SXFunction HL = lfcn.hessian();


  // ----
  // SOLVE THE NLP
  // ----

  // Allocate an NLP solver
  IpoptSolver solver(ffcn_nlp,gfcn_nlp, HL);
#else
  // Allocate an NLP solver
  IpoptSolver solver(ffcn_nlp,gfcn_nlp);
#endif
      

  // Set options
  solver.setOption("abstol",1e-6);

  // initialize the solver
  solver.init();

  // Bounds on x and initial guess
  solver.input(NLP_LBX).set(vars_lb);
  solver.input(NLP_UBX).set(vars_ub);
  solver.input(NLP_X_INIT).set(vars_sol);
  
  // Bounds on the constraints
  solver.input(NLP_LBG).set(lbg);
  solver.input(NLP_UBG).set(ubg);
  
  // Solve the problem
  solver.solve();

  // Print the optimal cost
  double cost;
  solver.output(NLP_COST).get(cost);
  cout << "optimal cost: " << cost << endl;

  // ----
  // SAVE SOLUTION TO DISK
  // ----

  vector<double> t_opt(N*(K+1));
  int ind = 0; // index of nlp->x
  for(int i=0; i<N; ++i){
    for(int j=0; j<=K; ++j){
      int ij = (K+1)*i+j;
      t_opt[ij] = T[i][j];
    }
  }
  
  std::ofstream resfile;
  resfile.open ("results_convection_diffusion.txt");

  // Get the solution
  solver.output(NLP_X_OPT).get(vars_sol);
  resfile << "T_opt " << vars_sol << endl;
  
  // Save optimal solution to disk
  resfile << "t_opt " << t_opt << endl;
  
  resfile.close();

  return 0;
}
  
