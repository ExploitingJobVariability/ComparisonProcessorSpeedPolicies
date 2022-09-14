//
//  print_functions.hpp
//  finite_case
//
//  Created by *** on 02/10/2018.
//  Copyright © 2018 ***. All rights reserved.
//

#ifndef print_functions_hpp
#define print_functions_hpp

#include "general_function.hpp"

// Affichage des résultats en % avec les intervalles de confiance
vector<double> int_conf(vector<vector<double> > sortie_gen_DP,double somme,double sommeEcartType);
vector<double> Affichage_resultat(vector<vector<double> > sortie_gen_DP, vector<vector<double> > sortie_gen_VI, vector<vector<double> > sortie_gen_Qlearning, int sample, int T, int affichage, bool int_conf_index, int nbre_simulation_Qlearning);

#endif /* print_functions_hpp */
