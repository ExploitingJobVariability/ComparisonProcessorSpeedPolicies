//
//  SimulationDataSetArticle.hpp
//
//  Created by Stéphan Arthur Plassart on 04.07.22.
//
#ifndef SimulationDataSetArticle_hpp
#define SimulationDataSetArticle_hpp

#include "VI_MDP.hpp"

int Simulation_Sec62_ImpactVecteurProbIAT(vector<vector<double> > & ens_possible_proba_IAT, vector<vector<double> > & ens_possible_proba_deadline, vector<vector<double> > & ens_possible_proba_size, int maxIAT);

int Simulation_Sec63_ImpactDeadlines(vector<vector<double> > & ens_possible_proba_IAT, vector<vector<double> > & ens_possible_proba_deadline, vector<vector<double> > & ens_possible_proba_size, bool JobIsole, int maxD);

int Simulation_Sec64_ImpactSize(vector<vector<double> > & ens_possible_proba_IAT, vector<vector<double> > & ens_possible_proba_deadline, vector<vector<double> > & ens_possible_proba_size, bool JobIsole);

int Simulation_Sec65_Video(vector<vector<double> > & ens_possible_proba_IAT, vector<vector<double> > & ens_possible_proba_deadline, vector<vector<double> > & ens_possible_proba_size, int & maxReduceSize, bool JobIsole);

int SpecialCase_givenProba(vector<double> & inter_arrival_time_proba,vector<double> & deadline_proba, vector<double> & size_proba, int max_inter_arrival_time, int max_size, int max_deadline);

int SimulationParameter(vector<vector<double> > & ens_possible_proba_IAT,vector<vector<double> > & ens_possible_proba_deadline,vector<vector<double> > & ens_possible_proba_size,vector<double> & inter_arrival_time_proba,vector<double> & deadline_proba,vector<double> & size_proba,int & max_inter_arrival_time,int & max_deadline,int & max_size,int Reduce_maxSize);
#endif /* SimulationDataSetArticle_hpp */
