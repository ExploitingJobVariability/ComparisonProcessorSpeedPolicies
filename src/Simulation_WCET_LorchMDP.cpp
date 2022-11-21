//
//  Simulation_WCET_LorchMDP.cpp
//  finite_case
//
//  Created by *** on 05/03/2019.
//  Copyright © 2019 ***. All rights reserved.
//

#include "Simulation_WCET_LorchMDP.hpp"
#include <algorithm>
#include <iterator>
#include <list>
#include <math.h>
#include <cmath>
#include <assert.h>
#include <fstream>
#include <iomanip>
using namespace std;

bool ceilMode = false, floorMode = false, midMode = true;

/*** BUT    : A partir de la liste des jobs determine letat suivant                                                                  ***/
/*** ENTREE : liste des jobs (ci,di), liste des jobs arrives en t (ci^t,di^t), l'indice de l'etat courant (ei,di) et l'espace d'état ***/
// liste_job_arrivee = (ci,di)
// etat_suivant = (ei,di) (etat de index_etat_suivant
// Ct=liste_job_arrivee, At=job_arrivee, it=etat_suivant, vt=vitesse_MDP
/*** RETURN : Indice de l'etat suivant                                                                                               ***/
int calcul_liste_etat_suivant(int DerniereArriveeJob, vector<vector<int> > & liste_job_arrivee, vector<vector<int> > & job_arrivee, int & index_etat_suivant, vector<vector<vector<int> > > & state_space, int vitesse_MDP, vector<int> & Vector_State_inter_arrival_time) {
    // Position de l'etat_suivant
    int pos, reste_vitesse = vitesse_MDP;
    vector<vector<int> > etat_suivant;
    if (index_etat_suivant>=0)
        etat_suivant=state_space[index_etat_suivant];
    
    // SUPPRESS EXECUTED JOBS: Suppression des jobs déjà terminé avec les taches considérées et la vitesse choisie
    int borne = (int)etat_suivant.size();
    
    if (!(etat_suivant.empty())) {
        if (!(etat_suivant[0].empty())) {
            for (int i=0; i<borne; i++) {
                // comparer la taille du job ave la vitesse pour le cas MDP
                if (- liste_job_arrivee[i][0] + etat_suivant[i][0] + reste_vitesse >= 0) {
                    // on supprime un job
                    reste_vitesse = reste_vitesse - liste_job_arrivee[i][0] + etat_suivant[i][0];
                    // Suppression des jobs dans la liste des jobs executés et présent
                    liste_job_arrivee.erase(liste_job_arrivee.begin()+i);
                    etat_suivant.erase(etat_suivant.begin()+i);
                    borne--; i--; // Du a la suppression des jobs et donc au décalage dans le vecteur
                } else {
                    // modification de l'executed work, rajout de la partie executée
                    etat_suivant[i][0] = etat_suivant[i][0] + reste_vitesse;
                    break;
                    
                }
            }
            // TIME SHIFT: déclage des jobs présents dans la liste, deadline diminuée de 1 ET ENLEVE les jobs ac deadline terminés
            for (int i=0; i<liste_job_arrivee.size(); i++) {
                liste_job_arrivee[i][1]--;
                if (liste_job_arrivee[i][1]==0) {
                    liste_job_arrivee.erase(liste_job_arrivee.begin()+i);
                    i--; // Du a la suppression des jobs et donc au décalage dans le vecteur
                }
            }
            if (index_etat_suivant>=0) {
                for (int i=0; i<etat_suivant.size(); i++) {
                    etat_suivant[i][1]--;
                    if (etat_suivant[i][1]==0) {
                        etat_suivant.erase(etat_suivant.begin()+i);
                        i--;  // Du a la suppression des jobs et donc au décalage dans le vecteur
                    }
                }
            }
            
        }
    }
    // AJOUT NEW JOBS: ajoute les nouveaux jobs à la liste des anciens jobs déjà présent
    vector<int> job_intermediaire;
    // Si pas de job_arrivée on ne le rajoute pas
    if (!(job_arrivee.empty())) {
        if (!(job_arrivee[0].empty())) {
            if (!(etat_suivant.empty())) {
                if (etat_suivant[0].empty()) {
                    etat_suivant.erase(etat_suivant.begin(), etat_suivant.end());
                }
            }
            for (int i=0; i<job_arrivee.size(); i++) {
                // ajout des nouveaux jobs aux deux listes présentes
                liste_job_arrivee.push_back(job_arrivee[i]);
                job_intermediaire = job_arrivee[i];
                job_intermediaire[0] = 0; // transforme le ci en ei qui vaut 0
                etat_suivant.push_back(job_intermediaire);
            }
            // ORDONNE liste_job_arrivee et etat_suivant en fonction des deadlines (2nd param)
            sort(liste_job_arrivee.begin(),liste_job_arrivee.end(),cmp);
            if (!(etat_suivant.empty())) {
                if (!(etat_suivant[0].empty())) {
                    sort(etat_suivant.begin(),etat_suivant.end(),cmp);
                }
            }
        }
    }
    // @ADD Cas specifique plus de jobs
    if (etat_suivant.empty()) {
        vector<int> test;
        etat_suivant.push_back(test);
    }
    // calcul de la position de cet nouvel état dans l'espace d'état
    std::vector<vector<vector<int> > >::iterator it = find(state_space.begin(),state_space.end(), etat_suivant);
    pos = (int)(distance(state_space.begin(),it));
    index_etat_suivant = pos;
    return index_etat_suivant;
}

int calcul_liste_etat_suivant_Lorch(vector<vector<int> > & liste_job_arrivee, vector<vector<int> > & job_arrivee, vector<vector<int> > & Etat_suivant_lorch, vector<double> LorchSpeedParJob) {
    // @DONE changer Lorch pour la vitesse par jobs et non de manière générale
    int compteur = 0;
    if (!(liste_job_arrivee.empty())) {
        if (!(liste_job_arrivee[0].empty())) {
            for (int i=0; i<Etat_suivant_lorch.size(); i++) {
                // Shift des deadlines
                liste_job_arrivee[i][1]--;
                Etat_suivant_lorch[i][1]--;
                // augmentation de e_i
                Etat_suivant_lorch[i][0]+=LorchSpeedParJob[compteur];
                if (Etat_suivant_lorch[i][0]-liste_job_arrivee[i][0]>=0) {
                    // Suppression des jobs si les jobs de liste_job valent 0
                    liste_job_arrivee.erase(liste_job_arrivee.begin()+i);
                    Etat_suivant_lorch.erase(Etat_suivant_lorch.begin()+i);
                    i--;
                }
                compteur++;
            }
        }
    }
    vector<int> jobInt;
    // @DONE Ajoute les job_arrivee a liste_job_arrivee Et Etat_suivant_lorch
    for (int i=0; i<job_arrivee.size(); i++) {
        jobInt=job_arrivee[i];
        liste_job_arrivee.push_back(job_arrivee[i]);
        jobInt[0]=0;
        Etat_suivant_lorch.push_back(jobInt);
    }
    // @DONE Reordonne
    if (!(Etat_suivant_lorch.empty())) {
        if (!(Etat_suivant_lorch[0].empty())) {
            sort(Etat_suivant_lorch.begin(),Etat_suivant_lorch.end(),cmp);
        }
    }
    if (!(liste_job_arrivee.empty())) {
        if (!(liste_job_arrivee[0].empty())) {
            sort(liste_job_arrivee.begin(),liste_job_arrivee.end(),cmp);
        }
    }
    return 0;
}

// @DONE Calcul vitesse Lorch en fonction du vecteur des tailles 
// @DONE Calcul somme des probas
// @DONE Calcul des a_i, b_i pour chaque changement de proba dans la somme
int Calcul_fonction_repartition_Lorch(vector<double> & size_proba, int deadline_courant, int executed_work, double & vitesse) {
    // @DONE Calcul de la somme des probas en fonction de la taille
    vector<double> Somme_proba = size_proba;
    double somme=0;
    for (int i=0; i<size_proba.size(); i++) {
        somme+=size_proba[i];
        Somme_proba[i]= somme;
    }
    // @DONE Calcul des a_i et b_i pour chaque portion de droite considéré
    // Repartition_droite = [fin de la pente, coef. dir. pente (ai), ordonnée a lorigine (bi)]
    vector<vector<double> > Repartition_droite;
    double value_cour = 0,res = 0;
    double ordonnee_origine, pente;
    // Calcul des pentes 
    for (int i=0; i<size_proba.size(); i++) {
        value_cour+=size_proba[i];
        pente = (value_cour-res); // Pente vaut 0 si arrivée de job de taille i nulle
        ordonnee_origine = res - (double)(i)*pente;
        vector<double> Ajout; Ajout.push_back(i); Ajout.push_back(pente); Ajout.push_back(ordonnee_origine);
        Repartition_droite.push_back(Ajout);
        res=value_cour;
    }
    // @DONE Calcul de \sigma en fonction de l'intégrale de la fonction de répartition de la loi uniforme par morceaux
    // Parcours de l'ensemble des portions de droites considérées
    double sigma = 0;
    double ai,bi,deb,fin;
    for (int i=0; i<Repartition_droite.size(); i++) {
        // Ajout de chaque portion de droite
        ai = Repartition_droite[i][1];
        bi = min((double)1,Repartition_droite[i][2]);
        deb = Repartition_droite[i][0];
        fin = deb + 1;
        // En fonction de la valeur de la pente
        if (ai!=0) {
            // Cas ou on a une integration (pente de la fonction de repartition)
            // valeur = 3/(4*ai) * [(1-ai*i-bi)^(4/3) - (1-ai*i'-bi)^(4/3)]
            if (((1-ai*deb-bi)<0) || ((1-ai*fin-bi)<0)) {
                //                                cout << "===+++===> " << 1-ai*deb-bi << "  " << 1-ai*fin-bi << endl;
                sigma += (double)3*(pow(max(1-ai*deb-bi,(double)0),(double)4/3)-pow(max(1-ai*fin-bi,(double)0),(double)4/3))/((double)4*(double)ai);
            } else {
                sigma += (double)3*(pow(1-ai*deb-bi,(double)4/3)-pow(1-ai*fin-bi,(double)4/3))/((double)4*(double)ai);
            }
        } else {
            // Cas ou on integre pas (plateau de la fonction de repartition)
            // valeur = (1-bi)^(1/3)
            sigma += pow((1-bi),(double)1/3);
        }
    }
    sigma = (double)sigma/deadline_courant;
    // @DONE calcul de la vitesse en fonction du job \ie J_i(e_i,d)
    // @DONE Parcours de l'ensemble des droites et choisir la bonne droite en fonction de e_i
    if (executed_work==0) {
        vitesse = sigma;
    } else {
        vitesse = sigma / pow((1-Somme_proba[executed_work-1]), (double)1/3);
    }
    return 0;
}

// Retourne un vecteur de vitesse correspondant à la vitesse à appliquer à chaque job present dans etat_suivant
int Calcul_Vitesse_Lorch_WCET(vector<vector<int> > & etat_courant, vector<double> & LorchSpeedParJob, int max_size, vector<double> & size_proba) {
    int deadline, executedwork;
    // Parcours des jobs presents
    if (!etat_courant.empty()) {
        if (!(etat_courant[0].empty())) {
            for (int i=0; i<etat_courant.size(); i++) {
                executedwork = etat_courant[i][0];
                deadline = etat_courant[i][1];
                double speed_Lorch_1job;
                // @DONE Dans le cas générale d'une fonction à valeur discrète approximé par une fonction uniforme par morceaux
                Calcul_fonction_repartition_Lorch(size_proba, deadline, executedwork, speed_Lorch_1job);
                
                if (deadline==1)
                    speed_Lorch_1job=max_size-executedwork;
                if (ceilMode) {
                    LorchSpeedParJob.push_back(ceil(speed_Lorch_1job));
                } else if (floorMode) {
                    LorchSpeedParJob.push_back(floor(speed_Lorch_1job));
                } else if (midMode) {
                    if (speed_Lorch_1job-floor(speed_Lorch_1job)<0.5) {
                        LorchSpeedParJob.push_back(floor(speed_Lorch_1job));
                    } else {
                        LorchSpeedParJob.push_back(ceil(speed_Lorch_1job));
                    }
                }
            }
        }
    }
    return 1;
}
