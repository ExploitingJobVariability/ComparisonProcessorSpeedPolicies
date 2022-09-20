//
//  output_R.cpp
//  finite_case
//
//  Created by *** on 02/10/2018.
//  Copyright © 2018 ***. All rights reserved.
//

#include "output_R.hpp"
#include <math.h>

// Enregistrer EtudeTaille_sortie
void Enregistrer(vector<vector<double> > & EtudeTaille_sortie) {
    ofstream dataTailleOAvsMDPvsLorch("../output/dataTailleOAvsHeurtau1.txt");//dataTailleOAvsHeurtau1
    dataTailleOAvsMDPvsLorch << "MoyenneTaille<-c(";
    for (int i=0; i<EtudeTaille_sortie.size(); i++) {
        dataTailleOAvsMDPvsLorch << EtudeTaille_sortie[i][0];
        if (i < EtudeTaille_sortie.size()-1) {
            dataTailleOAvsMDPvsLorch << ",";
        }
    }
    dataTailleOAvsMDPvsLorch << ")" << endl;
    dataTailleOAvsMDPvsLorch << "LorchvsMDP<-c(";
    for (int i=0; i<EtudeTaille_sortie.size(); i++) {
        dataTailleOAvsMDPvsLorch << EtudeTaille_sortie[i][1];
        if (i < EtudeTaille_sortie.size()-1) {
            dataTailleOAvsMDPvsLorch << ",";
        }
    }
    dataTailleOAvsMDPvsLorch << ")" << endl;
    dataTailleOAvsMDPvsLorch << "OAvsMDP<-c(";
    for (int i=0; i<EtudeTaille_sortie.size(); i++) {
        dataTailleOAvsMDPvsLorch << EtudeTaille_sortie[i][2];
        if (i < EtudeTaille_sortie.size()-1) {
            dataTailleOAvsMDPvsLorch << ",";
        }
    }
    dataTailleOAvsMDPvsLorch << ")" << endl;
    dataTailleOAvsMDPvsLorch << "HEURvsMDP<-c(";
    for (int i=0; i<EtudeTaille_sortie.size(); i++) {
        dataTailleOAvsMDPvsLorch << EtudeTaille_sortie[i][3];
        if (i < EtudeTaille_sortie.size()-1) {
            dataTailleOAvsMDPvsLorch << ",";
        }
    }
    dataTailleOAvsMDPvsLorch << ")" << endl;
    
}

int PreparationVecteur_FichierSortiePourR(int discretisation_pas, vector<double> size_proba, vector<double> sortieLorch,vector<double> sortieOA,vector<double> sortieHEUR,vector<vector<double> > & EtudeTaille_sortie) {
    vector<double> Sortie_int;
    // Ajout de la moyenne des tailles des jobs
    double moyenne_proba_size=0;
    double squaredValue=0;
    for (int i=0; i<size_proba.size();i++) {
        moyenne_proba_size+=size_proba[i]*(i+1);
        squaredValue+=pow(size_proba[i]*(i+1)*discretisation_pas,2);
    }
    Sortie_int.push_back(moyenne_proba_size);
    Sortie_int.push_back(sortieLorch[0]*100);
    Sortie_int.push_back(sortieOA[0]*100);
    Sortie_int.push_back(sortieHEUR[0]*100);
    Sortie_int.push_back(squaredValue/(double)discretisation_pas - pow(moyenne_proba_size,2));
    //    if (sortieOA[0]*100<0)
    //        cout << "INFERIEUR à 0" << endl;
    EtudeTaille_sortie.push_back(Sortie_int);
    return 0;
}

int PreparationVecteur_FichierSortiePourRElse(int discretisation_pas, vector<double> size_proba,vector<vector<double> > & EtudeTaille_sortie) {
    // Ajout de la moyenne des tailles des jobs
    vector<double> Sortie_int;
    double moyenne_proba_size=0;
    double squaredValue=0;
    for (int i=0; i<size_proba.size();i++) {
        moyenne_proba_size+=size_proba[i]*(i+1);
        squaredValue+=pow(size_proba[i]*(i+1)*discretisation_pas,2);
    }
    Sortie_int.push_back(moyenne_proba_size);
    Sortie_int.push_back(0);
    Sortie_int.push_back(0);
    Sortie_int.push_back(0);
    EtudeTaille_sortie.push_back(Sortie_int);
    return 0;
}
