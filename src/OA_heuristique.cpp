//
//  OA_Heuristique.hpp
//
//  Created by *** on 08/12/2021.
//  Copyright © 2021 ***. All rights reserved.
//

#include "general_function.hpp"
#include "OA_heuristique.hpp"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <list>
#include <math.h>
#include <cmath>
#include <assert.h>
#include <fstream>
#include <iomanip>

using namespace std;

double SommeProba(vector<double> & vecteur_proba, int executeddata, int AddSizeDeadline) {
    // Parcours de Vector et sommation
    double sommeProba=0;
    for (int i=0; i<=executeddata-AddSizeDeadline; i++) {
        sommeProba+=vecteur_proba[i];
    }
    return sommeProba;
}
double SommexProba(vector<double> & vecteur_proba, int executeddata, int AddSizeDeadline) {
    // Parcours de Vector et sommation
    double sommeProba=0;
    for (int i=executeddata; i<(int)(vecteur_proba.size())+AddSizeDeadline; i++) {
        sommeProba+=i*vecteur_proba[i-AddSizeDeadline];
    }
    return sommeProba;
}

// Return la vitesse choisie par lheuristique, a partir de letat courant [(ei,di)]
int Calcul_Vitesse_Heur(vector<vector<int> > etat_courant, vector<double> & size_proba, vector<double> & deadline_proba, vector<double> & inter_arrival_time_proba,int current_inter_arrival_time_Heur, int max_size, int max_deadline, int max_inter_arrival_time) {
    int vitesse_Heur=0;
    int sommeGen=0;
    double executedwork, deadline;
    double wichap, tauchap, w0,d0;
    vector<vector<double> > etat_construit;
    vector<double> etat_add;
    
    // Parcours la liste des jobs et construit letat provisoire non entier en moyenne
    if (!etat_courant.empty()) {
        if (!(etat_courant[0].empty())) {
            for (int i=0; i<etat_courant.size(); i++) {
                executedwork = etat_courant[i][0];
                deadline = etat_courant[i][1];
                // @DONE Calcul des chap(wi) pour toutes taches
                if (deadline!=1) {
                    wichap = 1/(1-SommeProba(size_proba,executedwork,1)) * SommexProba(size_proba,executedwork+1,1) - executedwork;
                } else {
                    wichap = max_size - executedwork;
                }
                etat_add.clear();
                etat_add.push_back(wichap);etat_add.push_back(deadline);
                etat_construit.push_back(etat_add);
            }
        }
    }
    // Construction de la nouvelle tache, 1) Calcul des chap(\tau)
    tauchap = 1/(1-SommeProba(inter_arrival_time_proba,current_inter_arrival_time_Heur,0)) * SommexProba(inter_arrival_time_proba,current_inter_arrival_time_Heur+1,0);
    // 2) New jobs
    w0 = 1/(1-inter_arrival_time_proba[0])*SommexProba(size_proba,1,1); //E(N)*E(w), nbre de jobs moyens times taille moyenne
    d0 = SommexProba(deadline_proba,1,1) + tauchap;
    etat_add.clear();
    etat_add.push_back(w0); etat_add.push_back(d0);
    // Add this new jobs in etat_construit
    int index=0;
    if (!etat_construit.empty()) {
        if (!(etat_construit[0].empty())) {
            if (tauchap<etat_construit[(int)(etat_construit.size())-1][1]) {
                for (int i=0; i<etat_construit.size(); i++) {
                    executedwork = etat_construit[i][0];
                    deadline = etat_construit[i][1];
                    if (deadline==d0 || deadline>d0) {
                        etat_construit.insert(etat_construit.begin() + i, etat_add);
                        break;
                    }
                }
                if (d0>etat_construit[(int)(etat_construit.size())-1][1])
                    etat_construit.insert(etat_construit.end(), etat_add); // cas longer deadline than the WC in state
            }
        }
    }
    // Calcul de la Vitesse choisie pente en double
    // Parcourir letat choisie cumulé et rassembler les etats avec meme deadline
    index=0;
    vector<vector<double> > etatescalier;
    if (!etat_construit.empty()) {
        if (!(etat_construit[0].empty())) {
            for (int i=0; i<etat_construit.size(); i++) {
                executedwork = etat_construit[i][0];
                deadline = etat_construit[i][1];
                if (index==deadline) { // cas deadline exitante
                    etatescalier[(int)(etatescalier.size())-1][0] += executedwork;
                } else { // cas deadline non presente
                    index=deadline;
                    etat_add.clear();
                    etat_add.push_back(executedwork); etat_add.push_back(deadline);
                    etatescalier.push_back(etat_add);
                }
            }
            // parcourir etatescalier pour detemriner la vitesse max
            vector<double> pente; pente.push_back(0);
            double somme=0;
            for (int i=0; i<etatescalier.size(); i++) {
                if (etatescalier[i][1]==0)
                    cout << "ERREUR: " << etatescalier << endl;
                somme += etatescalier[i][0];                          //  \sum (WCET - e_i)
                pente.push_back((double)somme/(double)(etatescalier[i][1]));     //  max( \sum (WCET - e_i) / d_i) ) \forall J_i
            }
            // @DONE Recupere la pente entière la plus grande, closest
            vitesse_Heur = ceil(*max_element(pente.begin(), pente.end()));
        } else {
            vitesse_Heur = 0;
        }
    }
    // cas x=0, aucun job en attente
    if (etat_courant.empty())
        vitesse_Heur=0;
    return vitesse_Heur;
}

int calcul_liste_etat_suivant_Heur(vector<vector<int> > & liste_job_present_Heur, vector<vector<int> > & job_arrivee_Heur, vector<vector<int> > & etat_suivant_Heur, int vitesse_Heur) {
    // @TODO 1) Heuristique appliquer la vitesse, suppress job from
    // @TODO Mise ajour de liste_job_present_OA ET etat_suivant_OA => enleve vitesse_OA
    int speedInt = vitesse_Heur;
    if (!(liste_job_present_Heur.empty())) {
        if (!(liste_job_present_Heur[0].empty())) {
            while (!(liste_job_present_Heur.empty()) && speedInt-liste_job_present_Heur[0][0]+etat_suivant_Heur[0][0]>=0) {
                // TODO Nouvelle vitesse decrut de la qté déjà faite
                speedInt=speedInt-liste_job_present_Heur[0][0]+etat_suivant_Heur[0][0];
                liste_job_present_Heur.erase(liste_job_present_Heur.begin()+0);
                etat_suivant_Heur.erase(etat_suivant_Heur.begin()+0);
            }
            // @TODO Augmentation des e_in pour le job critique
            if (!(etat_suivant_Heur.empty()))
                if (!(etat_suivant_Heur[0].empty()))
                    etat_suivant_Heur[0][0]+=speedInt;
        }
    }
    // @TODO 2) deadline shift for the two lists
    if (!(etat_suivant_Heur.empty())) {
        if (!(etat_suivant_Heur[0].empty())) {
            for (int i=0; i<etat_suivant_Heur.size(); i++) { // meme taille de liste
                etat_suivant_Heur[i][1] = etat_suivant_Heur[i][1]-1;
                //                for (int i=0; i<liste_job_present_Heur.size(); i++)
                liste_job_present_Heur[i][1] = liste_job_present_Heur[i][1]-1;
            }
        }
    }
    // @TODO 3) Ajoute les job_arrivee a etat
    // @TODO 3-a) Ajoute les job_arrivee a liste_job_arrivee Et Etat_suivant_lorch
    vector<int> jobInt;
    for (int i=0; i<job_arrivee_Heur.size(); i++) {
        jobInt=job_arrivee_Heur[i];
        liste_job_present_Heur.push_back(job_arrivee_Heur[i]);
        jobInt[0]=0;
        etat_suivant_Heur.push_back(jobInt);
    }
    // @TODO 3-b) Reordonne etat_suivant_Heur et liste_job_present_Heur
    if (!(etat_suivant_Heur.empty())) {
        if (!(etat_suivant_Heur[0].empty())) {
            sort(etat_suivant_Heur.begin(),etat_suivant_Heur.end(),cmp);
        }
    }
    if (!(liste_job_present_Heur.empty())) {
        if (!(liste_job_present_Heur[0].empty())) {
            sort(liste_job_present_Heur.begin(),liste_job_present_Heur.end(),cmp);
        }
    }
    return 0;
}
