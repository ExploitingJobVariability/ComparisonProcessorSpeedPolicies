//
//  Simulation_WCET_LorchMDP.hpp
//
//  Created by *** on 05/03/2019.
//  Copyright © 2019 ***. All rights reserved.
//

#ifndef Simulation_WCET_LorchMDP_hpp
#define Simulation_WCET_LorchMDP_hpp

#include "VI_MDP.hpp"

int calcul_liste_etat_suivant(int DerniereArriveeJob, vector<vector<int> > & liste_job_arrivee, vector<vector<int> > & job_arrivee, int & index_etat_suivant, vector<vector<vector<int> > > & state_space, int vitesse_MDP, vector<int> & Vector_State_inter_arrival_time);
int calcul_liste_etat_suivant_Lorch(vector<vector<int> > & liste_job_arrivee, vector<vector<int> > & job_arrivee, vector<vector<int> > & Etat_suivant_lorch, vector<double> LorchSpeedParJob);
int Calcul_Vitesse_Lorch_WCET(vector<vector<int> > & etat_courant, vector<double> & LorchSpeedParJob, int max_size, vector<double> & size_proba);

#endif /* Simulation_WCET_LorchMDP_hpp */
