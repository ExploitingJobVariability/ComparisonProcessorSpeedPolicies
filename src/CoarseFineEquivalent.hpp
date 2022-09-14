//
//  CoarseFineEquivalent.hpp
//  NewVersion_ExploitingJobVariability
//
//  Created by Stéphan Arthur Plassart on 20.06.22.
//

#ifndef CoarseFineEquivalent_hpp
#define CoarseFineEquivalent_hpp

#include "Simulation_WCET_LorchMDP.hpp"
#include "OA_heuristique.hpp"

int TransformListToListReduced(vector<vector<int> > & etat_suivant_VRAI,vector<vector<int> > & etat_suivant_REDUCED, vector<double> Available_speeds);
int ChangementEtatVRAI(vector<vector<int> > & liste_job_arrivee, vector<vector<int> > & job_arrivee, vector<vector<int> > & etat_suivant, int vitesse_MDP_VRAI);

int FindClosestAvailableSpeed(vector<double> & LorchSpeedParJob, vector<double> & AvailableSpeeds);
int FindClosestAvailableSpeed_ONE(int & vitesse_unique, vector<double> & AvailableSpeeds);
int Upper_FindClosestAvailableSpeed_ONE(int & vitesse_unique, vector<double> & AvailableSpeeds);

int Reduce_Vector_Size(vector<double> & sizeproba, vector<double> & Reduced_sizeproba, int Reduce_maxSize);
int SizeJobOnReduceVector(int size_fineGrain, vector<double> & sizeproba, vector<double> & Reduced_sizeproba, int Reduce_maxSize);

int Simu_ApplyMDPFromsimulationVI(vector<double> & size_proba, vector<double> & deadline_proba, vector<double> inter_arrival_time_proba,int max_speeds,int TailleMaxBuffer, vector<double> & Matrice_vitesse, int max_size, int max_deadline, int max_inter_arrival_time, vector<vector<vector<int> > > & state_space, int T, vector<int> & Vector_State_inter_arrival_time, int tailleListemin, vector<double> & Reduced_sizeproba, int Reduce_maxSize, vector<vector<vector<int> > > & ListeJobArrivalSimulation_MDP, vector<vector<vector<int> > > & ListeJobArrivalSimulation_MDP_VraiVecteur,int WindowsSize,vector<int> & Vecteur_SimuTime_Current_InterArrivalTime,vector<int> & numberOfJobsByTimeStepsMDP);

int Calcul_Vitesse_OA_WCET_AvailableSpeeds(vector<vector<int> > & etat_suivant_OA, int & vitesse_OA, int max_size, vector<double> & AvailableSpeeds);
int calcul_liste_etat_suivant_OA_SPECIAL(vector<vector<int> > & liste_job_present_OA, vector<vector<int> > & job_arrivee, vector<vector<int> > & etat_suivant_OA, int vitesse_OA, int max_size, int & booster_OA);
int SimuOA_onMDP_VraiVecteur(vector<double> & size_proba, vector<double> & deadline_proba, vector<double> inter_arrival_time_proba,int max_speeds,int TailleMaxBuffer, vector<double> & Matrice_vitesse, int max_size, int max_deadline, int max_inter_arrival_time, int T,int tailleListemin, vector<double> & Reduced_sizeproba, int Reduce_maxSize, vector<vector<vector<int> > > & ListeJobArrivalSimulation_MDP, vector<vector<vector<int> > > & ListeJobArrivalSimulation_MDP_VraiVecteur,vector<double> & Available_speed);

int SimuPACE_onMDP_VraiVecteur(vector<double> & size_proba, vector<double> & deadline_proba, vector<double> inter_arrival_time_proba,int max_speeds,int TailleMaxBuffer, vector<double> & Matrice_vitesse, int max_size, int max_deadline, int max_inter_arrival_time, int T, int tailleListemin, vector<double> & Reduced_sizeproba, int Reduce_maxSize, vector<vector<vector<int> > > & ListeJobArrivalSimulation_MDP, vector<vector<vector<int> > > & ListeJobArrivalSimulation_MDP_VraiVecteur,vector<double> & Available_speed);

int SimuHEUR_onMDP_VraiVecteur(vector<double> & size_proba, vector<double> & deadline_proba, vector<double> & inter_arrival_time_proba, int max_speeds, int TailleMaxBuffer, int max_size, int max_deadline, int max_inter_arrival_time, int T, int tailleListemin, vector<double> & Reduced_sizeproba, int Reduce_maxSize, vector<vector<vector<int> > > & ListeJobArrivalSimulation_MDP, vector<vector<vector<int> > > & ListeJobArrivalSimulation_MDP_VraiVecteur,vector<double> & Available_speeds,vector<int> & Vecteur_SimuTime_Current_InterArrivalTime);

int VersionReduced_Simu_ApplyMDPFromsimulationVI(vector<double> & size_proba, vector<double> & deadline_proba, vector<double> inter_arrival_time_proba,int max_speeds,int TailleMaxBuffer, vector<double> & Matrice_vitesse, int max_size, int max_deadline, int max_inter_arrival_time, vector<vector<vector<int> > > & state_space, int T, vector<int> & Vector_State_inter_arrival_time, int tailleListemin, vector<double> & Reduced_sizeproba, int Reduce_maxSize    , vector<vector<vector<int> > > & ListeJobArrivalSimulation_MDP, vector<vector<vector<int> > > & ListeJobArrivalSimulation_MDP_VraiVecteur, int WindowsSize,vector<int> & Vecteur_SimuTime_Current_InterArrivalTime,vector<double> & Available_speeds,vector<int> & numberOfJobsByTimeStepsMDP);

#endif /* CoarseFineEquivalent_hpp */
