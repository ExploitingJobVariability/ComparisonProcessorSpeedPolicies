//
//  VI_MDP.hpp
//
//  Created by *** on 05/02/2019.
//  Copyright © 2019 ***. All rights reserved.
//

#ifndef VI_MDP_hpp
#define VI_MDP_hpp

#include "general_function.hpp"

int create_state_space_SPEC_OPTIMIZED(int l,int max_size,int max_deadline, int bound_M, int max_speeds,vector<vector<vector<int> > > & state_space);
int Gen_create_state_space_SPEC(int max_buffer,int max_size,int max_deadline, int bound_M, int max_speeds,vector<vector<vector<int> > > & state_space, vector<int> & Vector_State_inter_arrival_time, vector<double> & inter_arrival_time_proba, int & tailleListemin);

double Matrix_proba_computation(int lx, int ly, int TailleMaxBuffer, double & somme_probal1, double & somme_probal2, bool & calcul_proba_int, int ind_state_prev, int ind_state_suiv, int speed, vector<vector<vector<int> > > & state_space, int max_size, int max_speeds, vector<double> & inter_arrival_time_proba,vector<double> & deadline_proba,vector<double> & size_proba, int typeloi_uniforme, bool Affiche_MatProba);

double probability_computation_Arrival_Jobs(int lx, int TailleMaxBuffer, int nb_job_exec,vector<vector<int> > & state_prev, vector<vector<int> > & Nouvel_Arrive, vector<double> & inter_arrival_time_proba,vector<double> & deadline_proba,vector<double> & size_proba, int tailleShiftEtatPrec);

int Vitesses_possible(vector<double> & ens_speeds, int x, vector<vector<vector<int> > > & state_space, int max_speeds, int max_size);
int Value_iteration(int max_deadline,int TailleMaxBuffer, vector<vector<vector<int> > > & state_space, vector<int> & Vector_State_inter_arrival_time, int max_size, int max_speeds, vector<double> & inter_arrival_time_proba,vector<double> & deadline_proba,vector<double> & size_proba, vector<double> & Speeds_min, vector<double> & Nrj_min, int tailleListemin,int typeloi_uniforme, int WindowsSize, double convergenceVI);

#endif /* VI_MDP_hpp */
