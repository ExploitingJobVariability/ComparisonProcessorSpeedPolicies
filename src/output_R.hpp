//
//  output_R.hpp
//  finite_case
//
//  Created by *** on 02/10/2018.
//  Copyright © 2018 ***. All rights reserved.
//

#ifndef output_R_hpp
#define output_R_hpp

#include "general_function.hpp"
#include <fstream>
#include <iomanip>

void Enregistrer(vector<vector<double> > & EtudeTaille_sortie);

int PreparationVecteur_FichierSortiePourR(int discretisation_pas, vector<double> size_proba, vector<double> sortieLorch,vector<double> sortieOA,vector<double> sortieHEUR,vector<vector<double> > & EtudeTaille_sortie);
int PreparationVecteur_FichierSortiePourRElse(int discretisation_pas, vector<double> size_proba,vector<vector<double> > & EtudeTaille_sortie);

#endif /* output_R_hpp */
