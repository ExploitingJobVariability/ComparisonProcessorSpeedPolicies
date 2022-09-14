//
//  OA_Heuristique.hpp
//
//  Created by *** on 08/12/2021.
//  Copyright © 2021 ***. All rights reserved.
//

#ifndef OA_heuristique
#define OA_heuristique

#include <stdio.h>

int Calcul_Vitesse_Heur(vector<vector<int> > etat_courant, vector<double> & size_proba, vector<double> & deadline_proba, vector<double> & inter_arrival_time_proba,int current_inter_arrival_time_Heur, int max_size, int max_deadline, int max_inter_arrival_time);

int calcul_liste_etat_suivant_Heur(vector<vector<int> > & liste_job_present_Heur, vector<vector<int> > & job_arrivee_Heur, vector<vector<int> > & etat_suivant_Heur, int vitesse_Heur);

#endif
