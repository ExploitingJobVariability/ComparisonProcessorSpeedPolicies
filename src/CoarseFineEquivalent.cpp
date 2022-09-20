//
//  CoarseFineEquivalent.cpp
//
//  Created by Stéphan Arthur Plassart on 20.06.22.
//
#include "CoarseFineEquivalent.hpp"
#include <algorithm>
#include <math.h>

// Function 1: list detat = (ei,di) -> (eiReduced,di)
int TransformListToListReduced(vector<vector<int> > & etat_suivant_VRAI,vector<vector<int> > & etat_suivant_REDUCED, vector<double> Available_speeds) {
    etat_suivant_REDUCED.clear();
    for (int i=0;i<etat_suivant_VRAI.size();i++) {
        vector<int> provTask = etat_suivant_VRAI[i];
        int pos = 0;
        if (!provTask.empty()) {
            int executedwork = etat_suivant_VRAI[i][0];
            int cas1 = false; // choix de cas 2, car sinon on sous estime la vitesse restante a executer (01/07
            if (cas1) {
                pos = FindClosestAvailableSpeed_ONE(executedwork,Available_speeds); // Cherche le ei le plus proche des tailles disponibles sur le set de taille
                if (pos>=(int)(Available_speeds.size())-1)
                    pos = pos - 1; // Que pour la derniere taille
            } else {
                pos = Upper_FindClosestAvailableSpeed_ONE(executedwork,Available_speeds);
            }
            provTask[0] = pos; // @DONE remplacer executedwork par pos
        }
        etat_suivant_REDUCED.push_back(provTask);
    }
    return 0;
}

int ChangementEtatVRAI(vector<vector<int> > & liste_job_arrivee, vector<vector<int> > & job_arrivee, vector<vector<int> > & etat_suivant, int vitesse_MDP_VRAI) {
    // Position de l'etat_suivant
    int pos, reste_vitesse = vitesse_MDP_VRAI;
    
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
            //@DONE Atttention remplacement par empty             if (index_etat_suivant>=0) {
            if (!etat_suivant.empty()) {
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
    return 0;
}

int FindClosestAvailableSpeed(vector<double> & LorchSpeedParJob, vector<double> & AvailableSpeeds) {
    // @DONE Recover the first speed available above vitesse_OA in the vector AvailableSpeeds
    for (int j=0; j<LorchSpeedParJob.size(); j++) {
        int vitesseStudied = LorchSpeedParJob[j];
        if (vitesseStudied != 0) {
            for (int i=0; i<AvailableSpeeds.size(); i++) {
                if (AvailableSpeeds[i] >= vitesseStudied) {
                    vitesseStudied = AvailableSpeeds[i];
                    break;
                }
            }
        }
        LorchSpeedParJob[j] = vitesseStudied;
    }
    return 1;
}

int FindClosestAvailableSpeed_ONE(int & vitesse_unique, vector<double> & AvailableSpeeds) {
    int pos = 0;
    // @DONE Recover the first speed available above vitesse_OA in the vector AvailableSpeeds
    if (vitesse_unique != 0) {
        for (int i=0; i<AvailableSpeeds.size(); i++) {
            if (AvailableSpeeds[i] >= vitesse_unique) {
                vitesse_unique = AvailableSpeeds[i];
                pos = i;
                break;
            }
        }
    }
    return pos;
}

int Upper_FindClosestAvailableSpeed_ONE(int & vitesse_unique, vector<double> & AvailableSpeeds) {
    int pos = 0;
    // @DONE Recover the first speed available above vitesse_OA in the vector AvailableSpeeds
    if (vitesse_unique != 0) {
        for (int i=0; i<AvailableSpeeds.size(); i++) {
            if (AvailableSpeeds[i] > vitesse_unique) {
                vitesse_unique = AvailableSpeeds[i-1];
                pos = i-1;
                break;
            }
        }
    }
    return pos;
}

int Reduce_Vector_Size(vector<double> & sizeproba, vector<double> & Reduced_sizeproba, int Reduce_maxSize) {
    double WindowSize = ceil((double)(sizeproba.size()) / (double)Reduce_maxSize);
    //    cout << "taille de la fenetre: " << WindowSize << endl;
    for (int i=0; i<Reduce_maxSize; i++) {
        double SommeWindowSize = 0;
        for (int j=0; j<WindowSize; j++) {
            if (i*WindowSize+j < (int)(sizeproba.size())) {
                SommeWindowSize += sizeproba[i*WindowSize+j];
            } else {
                break;
            }
        }
        Reduced_sizeproba.push_back(SommeWindowSize);
    }
    if (Reduce_maxSize>sizeproba.size())
        cout << "ERROR REDUCE MAX SIZE TOO LARGE" << endl;
    return WindowSize;
}

// @RETURN the coarse grain size from fine grain size and vector of probability of size
int SizeJobOnReduceVector(int size_fineGrain, vector<double> & sizeproba, vector<double> & Reduced_sizeproba, int Reduce_maxSize) {
    double WindowSize = ceil((double)(sizeproba.size()) / (double)Reduce_maxSize);
    int size_coarseGrain = ceil((double)size_fineGrain/(double)WindowSize);
    return size_coarseGrain;
}

int Simu_ApplyMDPFromsimulationVI(vector<double> & size_proba, vector<double> & deadline_proba, vector<double> inter_arrival_time_proba,int max_speeds,int TailleMaxBuffer, vector<double> & Matrice_vitesse, int max_size, int max_deadline, int max_inter_arrival_time, vector<vector<vector<int> > > & state_space, int T, vector<int> & Vector_State_inter_arrival_time, int tailleListemin, vector<double> & Reduced_sizeproba, int Reduce_maxSize    , vector<vector<vector<int> > > & ListeJobArrivalSimulation_MDP, vector<vector<vector<int> > > & ListeJobArrivalSimulation_MDP_VraiVecteur, int WindowsSize,vector<int> & Vecteur_SimuTime_Current_InterArrivalTime,vector<int> & numberOfJobsByTimeStepsMDP) {
        
    vector<vector<int> > job_arrivee; // job_arrivee = ensemble des jobs qui arrivent en t.
    vector<vector<int> > liste_job_present_MDP, etat_suivant_MDP;
    int vitesse_MDP=0;
    
    vector<int> StockageVitesseMDP, StockageVitesseLORCH,StockageVitesseOA,pasdeTemps;
    vector<vector<vector<int> > > ListeJobArrivalSimulation,ListeEtatMDP, Stockage_etat;
    vector<double> LorchSpeedParJob;
    
    int current_size, current_size_True, current_deadline,current_inter_arrival_time, Nbre_job;
    int nbre_job_executer = 0, delai_k=0;
    int t=0;
    int index_etat_suivant_MDP = tailleListemin-1;
    etat_suivant_MDP=state_space[index_etat_suivant_MDP];
        
    /**** Partie simulation en fonction du temps ****/
    // @DONE Simulation du premier Inter-arrival-time: inter_arrival_time_proba_Normaliser, distribution normalisé dans le cas particulier
    vector<double> inter_arrival_time_proba_Normaliser (inter_arrival_time_proba.begin()+1,inter_arrival_time_proba.end());
    inter_arrival_time_proba_Normaliser=inter_arrival_time_proba_Normaliser/(1-inter_arrival_time_proba[0]);
    current_inter_arrival_time = Simu_VectorProba(inter_arrival_time_proba_Normaliser);
    
    vector<vector<int> > PremArriv;
    ListeJobArrivalSimulation.push_back(PremArriv);
    ListeEtatMDP.push_back(PremArriv);
    
    vector<double> Nbre_job_arrivParXMinB(4,0); // 0,1,2,3
    vector<vector<double> > Nbre_job_arriv;
    for (int i=0; i<4; i++)
        Nbre_job_arriv.push_back(Nbre_job_arrivParXMinB);
    
    int delai_k_MDP=0, current_inter_arrival_time_MDP, Nbre_job_MDP;
    current_inter_arrival_time_MDP = Simu_VectorProba(inter_arrival_time_proba_Normaliser);
    vector<vector<int> > job_arrivee_MDP,job_arrivee_MDP_VraiVecteur;
    
    while (t<T) {
        if (t<T-max_deadline) {
            if (index_etat_suivant_MDP + delai_k_MDP*tailleListemin>Matrice_vitesse.size())
                cout << "OUT OF BOUND" << endl;
            vitesse_MDP = Matrice_vitesse[index_etat_suivant_MDP + delai_k_MDP*tailleListemin];
        } else {
            if (index_etat_suivant_MDP > state_space.size())
                cout << "OUT OF BOUND " << endl;
            vitesse_MDP = Matrice_vitesse[index_etat_suivant_MDP];
        }
        StockageVitesseMDP.push_back(vitesse_MDP);
        
        delai_k_MDP++; // add pour aller au bon indice
        // @DONE création de de l'ensemble des jobs arrivée a(t)
        // @DONE Calcul du nombre de job executé
        int res_MDP=vitesse_MDP;
        int nbre_job_executer_MDP=0;
        for (int i=0; i<liste_job_present_MDP.size(); i++) {
            if (res_MDP-liste_job_present_MDP[i][0]+etat_suivant_MDP[i][0]>=0) {
                nbre_job_executer_MDP++;
                res_MDP-=liste_job_present_MDP[i][0]+etat_suivant_MDP[i][0];
            } else {
                break;
            }
        }
        
        Nbre_job_MDP = (int)(etat_suivant_MDP.size()) - nbre_job_executer_MDP;
        // Création du job A(t+1)
        while (current_inter_arrival_time_MDP==0 && Nbre_job_MDP<TailleMaxBuffer) {
            if (t<=T-max_deadline) {
                current_size_True = Simu_VectorProba(size_proba)+1;
                // @DONE ajout de current_size
                current_size = SizeJobOnReduceVector(current_size_True, size_proba,Reduced_sizeproba,Reduce_maxSize);
                current_deadline = Simu_VectorProba(deadline_proba)+1;
                vector<int> Ajout_VraiVecteur; Ajout_VraiVecteur.push_back(current_size_True); Ajout_VraiVecteur.push_back(current_deadline);
                vector<int> Ajout; Ajout.push_back(current_size); Ajout.push_back(current_deadline);
                job_arrivee_MDP.push_back(Ajout);
                job_arrivee_MDP_VraiVecteur.push_back(Ajout_VraiVecteur);
                Nbre_job_MDP++;
            }
            current_inter_arrival_time_MDP = Simu_VectorProba(inter_arrival_time_proba);
        }
        // add pour remettre lindice a 0 de recherche dans la metrice_vitesse quand un  job arrive
        if (!(job_arrivee_MDP.empty()) || Nbre_job_MDP==TailleMaxBuffer) {
            delai_k_MDP = 0;
        }
        ListeJobArrivalSimulation_MDP.push_back(job_arrivee_MDP); // job arrived reduced
        ListeJobArrivalSimulation_MDP_VraiVecteur.push_back(job_arrivee_MDP_VraiVecteur); // job arrived true
                
        // Avec vitesse en t et A(t+1)- : détermine letat suivant en t+1
        index_etat_suivant_MDP = calcul_liste_etat_suivant(0,liste_job_present_MDP, job_arrivee_MDP, index_etat_suivant_MDP, state_space, vitesse_MDP,Vector_State_inter_arrival_time);
        
        if (index_etat_suivant_MDP>state_space.size())
            cout << "OUT OF BOUNDED" << endl;
        etat_suivant_MDP=state_space[index_etat_suivant_MDP];
        ListeEtatMDP.push_back(etat_suivant_MDP);
        
        if (current_inter_arrival_time_MDP==0) {
            current_inter_arrival_time_MDP = Simu_VectorProba(inter_arrival_time_proba_Normaliser)+1;
        }
        job_arrivee_MDP.erase(job_arrivee_MDP.begin(), job_arrivee_MDP.end());
        job_arrivee_MDP_VraiVecteur.erase(job_arrivee_MDP_VraiVecteur.begin(), job_arrivee_MDP_VraiVecteur.end());
        
        current_inter_arrival_time_MDP--;
        Vecteur_SimuTime_Current_InterArrivalTime.push_back(current_inter_arrival_time_MDP);
        
        pasdeTemps.push_back(t);
        t++;
    }
    /** Number of jobs present at each time step **/
    for (int i=0; i < ListeEtatMDP.size(); i++) {
        numberOfJobsByTimeStepsMDP.push_back((int)(ListeEtatMDP[i].size()));
    }
    int sommeSpeeds = 0, value_energie_MDP = 0;
    for (int i=0; i<StockageVitesseMDP.size(); i++) {
        sommeSpeeds += StockageVitesseMDP[i]*WindowsSize;
        value_energie_MDP += cout_nrj(StockageVitesseMDP[i], max_speeds, WindowsSize);
    }
    int nbredetat = 0;
    vector<int> list_buffersizeMDP;
    for (int i=0; i<ListeEtatMDP.size(); i++) {
        nbredetat += (int)(ListeEtatMDP[i].size());
        list_buffersizeMDP.push_back((int)ListeEtatMDP[i].size());
    }
    return value_energie_MDP;
}

// ========> OA PART
// Choix de vitesse OA en fonction du WCET
int Calcul_Vitesse_OA_WCET_AvailableSpeeds(vector<vector<int> > & etat_suivant_OA, int & vitesse_OA, int max_size, vector<double> & AvailableSpeeds) {
    // @DONE Calcul du max des pentes sur le WCET
    vector<double> pente; pente.push_back(0);
    double somme=0;
    for (int i=0; i<etat_suivant_OA.size(); i++) {
        if (etat_suivant_OA[i][1]==0) {
            cout << "ERREUR: " << etat_suivant_OA << endl;
        }
        somme += max_size - etat_suivant_OA[i][0];                          //  \sum (WCET - e_i)
        pente.push_back((double)somme/(double)(etat_suivant_OA[i][1]));     //  max( \sum (WCET - e_i) / d_i) ) \forall J_i
    }
    // @DONE Recupere la pente entière la plus grande
    vitesse_OA = ceil(*max_element(pente.begin(), pente.end()));
    // @DONE Recover the first speed available above vitesse_OA in the vector AvailableSpeeds
    if (vitesse_OA != 0) {
        for (int i=0; i<AvailableSpeeds.size(); i++) {
            if (AvailableSpeeds[i] >= vitesse_OA) {
                vitesse_OA = AvailableSpeeds[i];
                break;
            }
        }
    }
    return vitesse_OA;
}

// @DONE Determine liste_job_present_OA ET etat_suivant_OA en fonction de vitesse_OA et job_arrivee
int calcul_liste_etat_suivant_OA_SPECIAL(vector<vector<int> > & liste_job_present_OA, vector<vector<int> > & job_arrivee, vector<vector<int> > & etat_suivant_OA, int vitesse_OA, int max_size, int & booster_OA) {
    int SpeedInt = vitesse_OA;
    int compteur=0;
    // Mise ajour de liste_job_present_OA ET etat_suivant_OA => enleve vitesse_OA
    if (!(liste_job_present_OA.empty())) {
        if (!(liste_job_present_OA[0].empty())) {
            while (!(liste_job_present_OA.empty()) && SpeedInt-liste_job_present_OA[compteur][0]+etat_suivant_OA[compteur][0]>=0) {
                // Nouvelle vitesse decrut de la qté déjà faite
                SpeedInt=SpeedInt-liste_job_present_OA[compteur][0]+etat_suivant_OA[compteur][0];
                liste_job_present_OA.erase(liste_job_present_OA.begin()+compteur);
                etat_suivant_OA.erase(etat_suivant_OA.begin()+compteur);
            }
            // Augmentation des e_in pour le job critique
            if (!(etat_suivant_OA.empty()))
                if (!(etat_suivant_OA[0].empty()))
                    etat_suivant_OA[compteur][0]+=SpeedInt;
        }
    }
    // Shift des deadlines
    if (!(liste_job_present_OA.empty())) {
        if (!(liste_job_present_OA[0].empty())) {
            for (int i=0; i<liste_job_present_OA.size(); i++) {
                if (liste_job_present_OA[i][1]==1 || etat_suivant_OA[i][1]==1) {
                    cout << "ERREUR DANS OA" << endl;
                }
                liste_job_present_OA[i][1]--;
                etat_suivant_OA[i][1]--;
            }
        }
    }
    // Mise ajour de liste_job_present_OA Rajout job_arrivee
    // Mise a jour etat_suivant_OA Rajout job_arrivee
    vector<int> JobInt;
    for (int i=0; i<job_arrivee.size(); i++) {
        JobInt=job_arrivee[i];
        liste_job_present_OA.push_back(job_arrivee[i]);
        JobInt[0]=0;
        etat_suivant_OA.push_back(JobInt);
    }
    // tri des jobs dans les deux listes, reordonne
    if (!(etat_suivant_OA.empty())) {
        if (!(etat_suivant_OA[0].empty())) {
            sort(etat_suivant_OA.begin(),etat_suivant_OA.end(),cmp);
            sort(liste_job_present_OA.begin(),liste_job_present_OA.end(),cmp);
        }
    }
    return 0;
}

int SimuOA_onMDP_VraiVecteur(vector<double> & size_proba, vector<double> & deadline_proba, vector<double> inter_arrival_time_proba,int max_speeds,int TailleMaxBuffer, vector<double> & Matrice_vitesse, int max_size, int max_deadline, int max_inter_arrival_time, int T, int tailleListemin, vector<double> & Reduced_sizeproba, int Reduce_maxSize, vector<vector<vector<int> > > & ListeJobArrivalSimulation_MDP, vector<vector<vector<int> > > & ListeJobArrivalSimulation_MDP_VraiVecteur,vector<double> & Available_speed){
    
    vector<vector<int> > job_arrivee; // job_arrivee = ensemble des jobs qui arrivent en t.
    vector<vector<int> > liste_job_present_OA, etat_suivant_OA; int vitesse_OA=0;
    
    vector<int> StockageVitesseMDP, StockageVitesseLORCH,StockageVitesseOA,pasdeTemps;
    vector<vector<double> > StockageVitessesLorchParJobs;
    vector<vector<vector<int> > > ListeJobArrivalSimulation, ListeEtatLORCH,ListeEtatMDP, ListeEtatOA, Stockage_etat;
    vector<double> LorchSpeedParJob;
    
    int current_size, current_size_True, current_deadline,current_inter_arrival_time, Nbre_job;
    int nbre_job_executer = 0, delai_k=0;
    int t=0;
    
    vector<vector<int> > PremArriv;
    ListeJobArrivalSimulation.push_back(PremArriv);
    ListeEtatMDP.push_back(PremArriv);
    ListeEtatOA.push_back(PremArriv);
    
    vector<vector<int> > job_arrivee_OA;
    vector<int> PlusJobsQuePlaceBuffer;

    int booster_OA = 0;
    while (t<T) {
        Calcul_Vitesse_OA_WCET_AvailableSpeeds(etat_suivant_OA, vitesse_OA, max_size, Available_speed);
        StockageVitesseOA.push_back(vitesse_OA);
        // Création du job A(t+1)
        if (ListeJobArrivalSimulation_MDP_VraiVecteur[t].size() > 0) {
            job_arrivee_OA = ListeJobArrivalSimulation_MDP_VraiVecteur[t];
        }
        calcul_liste_etat_suivant_OA_SPECIAL(liste_job_present_OA,job_arrivee_OA,etat_suivant_OA,vitesse_OA,max_size,booster_OA);
        ListeEtatOA.push_back(etat_suivant_OA);
        job_arrivee_OA.erase(job_arrivee_OA.begin(), job_arrivee_OA.end());
        pasdeTemps.push_back(t);
        t++;
    }
    int sommeSpeeds_OA = 0;
    int value_energie_OA = 0;
    for (int i=0; i<StockageVitesseOA.size(); i++) {
        sommeSpeeds_OA += StockageVitesseOA[i];
        value_energie_OA += cout_nrj(StockageVitesseOA[i], 10000);//@DONE move max_speeds
    }
    
    int nbredetat = 0;
    vector<int> list_buffersizeOA;
    for (int i=0; i<ListeEtatOA.size(); i++) {
        nbredetat += (int)(ListeEtatOA[i].size());
        list_buffersizeOA.push_back((int)(ListeEtatOA[i].size()));
    }
    return value_energie_OA;
}

int SimuPACE_onMDP_VraiVecteur(vector<double> & size_proba, vector<double> & deadline_proba, vector<double> inter_arrival_time_proba,int max_speeds,int TailleMaxBuffer, vector<double> & Matrice_vitesse, int max_size, int max_deadline, int max_inter_arrival_time, int T, int tailleListemin, vector<double> & Reduced_sizeproba, int Reduce_maxSize, vector<vector<vector<int> > > & ListeJobArrivalSimulation_MDP, vector<vector<vector<int> > > & ListeJobArrivalSimulation_MDP_VraiVecteur,vector<double> & Available_speed){
    vector<vector<int> > job_arrivee; // job_arrivee = ensemble des jobs qui arrivent en t.
    vector<vector<int> > liste_job_present_LORCH, etat_suivant_LORCH;
    
    vector<int> StockageVitesseMDP, StockageVitesseLORCH,StockageVitesseOA,pasdeTemps;
    vector<vector<double> > StockageVitessesLorchParJobs;
    vector<vector<vector<int> > > ListeJobArrivalSimulation, ListeEtatLORCH,ListeEtatMDP, ListeEtatOA, Stockage_etat;
    vector<double> LorchSpeedParJob;
    
    int current_size, current_size_True, current_deadline,current_inter_arrival_time, Nbre_job;
    int nbre_job_executer = 0, delai_k=0;
    int t=0;
    
    vector<vector<int> > PremArriv;
    ListeJobArrivalSimulation.push_back(PremArriv);
    ListeEtatLORCH.push_back(PremArriv);
    
    vector<vector<int> > job_arrivee_LORCH;
    vector<vector<vector<int> > > ListeJobArrivalSimulation_LORCH;
    
    while (t<T) {
        LorchSpeedParJob.erase(LorchSpeedParJob.begin(), LorchSpeedParJob.end());
        Calcul_Vitesse_Lorch_WCET(etat_suivant_LORCH, LorchSpeedParJob, max_size,size_proba);
        if (etat_suivant_LORCH.empty())
            LorchSpeedParJob.push_back(0); //Cas spécifique zéro job présent
        
        FindClosestAvailableSpeed(LorchSpeedParJob,Available_speed);
        StockageVitessesLorchParJobs.push_back(LorchSpeedParJob);
        // Création du job A(t+1)
        if (ListeJobArrivalSimulation_MDP_VraiVecteur[t].size() > 0) {
            job_arrivee_LORCH = ListeJobArrivalSimulation_MDP_VraiVecteur[t];
        }
        ListeJobArrivalSimulation_LORCH.push_back(job_arrivee_LORCH);
        calcul_liste_etat_suivant_Lorch(liste_job_present_LORCH, job_arrivee_LORCH, etat_suivant_LORCH, LorchSpeedParJob);
        ListeEtatLORCH.push_back(etat_suivant_LORCH);
        
        job_arrivee_LORCH.erase(job_arrivee_LORCH.begin(), job_arrivee_LORCH.end());
        pasdeTemps.push_back(t);
        t++;
    }
    vector<int> Somme_StockageVitessesLorchParJobs;
    for (int i=0; i<StockageVitessesLorchParJobs.size(); i++) {
        if (!(StockageVitessesLorchParJobs[i].empty())) {
            int somme_int = 0;
            for (int j=0; j<StockageVitessesLorchParJobs[i].size(); j++) {
                somme_int+=StockageVitessesLorchParJobs[i][j];
            }
            Somme_StockageVitessesLorchParJobs.push_back(somme_int);
        } else {
            Somme_StockageVitessesLorchParJobs.push_back(0);
        }
    }
    int sommeSpeeds_PACE = 0, value_energie_PACE=0;
    for (int i=0; i<Somme_StockageVitessesLorchParJobs.size(); i++) {
        sommeSpeeds_PACE+=Somme_StockageVitessesLorchParJobs[i];
        value_energie_PACE+=cout_nrj(Somme_StockageVitessesLorchParJobs[i],1000);// @DONE move max_speeds
    }
    int nbredetat = 0;
    for (int i=0; i<ListeEtatLORCH.size(); i++) {
        nbredetat += (int)(ListeEtatLORCH[i].size());
    }
    return value_energie_PACE;
}

int SimuHEUR_onMDP_VraiVecteur(vector<double> & size_proba, vector<double> & deadline_proba, vector<double> & inter_arrival_time_proba, int max_speeds, int TailleMaxBuffer, int max_size, int max_deadline, int max_inter_arrival_time, int T, int tailleListemin, vector<double> & Reduced_sizeproba, int Reduce_maxSize, vector<vector<vector<int> > > & ListeJobArrivalSimulation_MDP, vector<vector<vector<int> > > & ListeJobArrivalSimulation_MDP_VraiVecteur,vector<double> & Available_speeds, vector<int> & Vecteur_SimuTime_Current_InterArrivalTime) {
    
    int new_size, new_deadline, t=0;
    // Initialisation Heuristique1
    vector<vector<int> > liste_job_present_Heur, etat_suivant_Heur,job_arrivee_HEUR; // job_arrivee = ensemble des jobs qui arrivent en t.
    int vitesse_Heur=0;
    // Storing vectors
    vector<vector<vector<int> > > ListeJobArrivalSimulation_OA,ListeJobArrivalSimulation_Heur,ListeEtatOA,ListeEtatHeur;
    vector<int> StockageVitesseHeur,StockageVitesseOA;
    int current_inter_arrival_time;
    
    while (t<T) {
        // @INPUT: etat_suivant_Heur (ei,di)
        //         liste_job_present_Heur (ci,di)
        // Heuristique 1: calcul vitesse
        current_inter_arrival_time = Vecteur_SimuTime_Current_InterArrivalTime[t];
        vitesse_Heur = Calcul_Vitesse_Heur(etat_suivant_Heur,size_proba,deadline_proba,inter_arrival_time_proba,current_inter_arrival_time,max_size,max_deadline,max_inter_arrival_time);
        FindClosestAvailableSpeed_ONE(vitesse_Heur,Available_speeds);
        StockageVitesseHeur.push_back(vitesse_Heur);
            
        // Recover job for Heur
        if (ListeJobArrivalSimulation_MDP_VraiVecteur[t].size() > 0) {
            job_arrivee_HEUR = ListeJobArrivalSimulation_MDP_VraiVecteur[t];
        }
        ListeJobArrivalSimulation_Heur.push_back(job_arrivee_HEUR);

        // Heuristique 1: etat suivant
        calcul_liste_etat_suivant_Heur(liste_job_present_Heur, job_arrivee_HEUR, etat_suivant_Heur,vitesse_Heur);
        ListeEtatHeur.push_back(etat_suivant_Heur);
        
        t++;
        job_arrivee_HEUR.clear();
    }
    // Calcul energie
    int sommeSpeed_Heur=0, value_energie_Heur=0;
    for (int i=0; i<StockageVitesseHeur.size(); i++) {
        sommeSpeed_Heur+=StockageVitesseHeur[i];
        value_energie_Heur+=cout_nrj(StockageVitesseHeur[i],1000); 
    }
    int nbredetat = 0;
    for (int i=0; i<ListeEtatHeur.size(); i++) {
        nbredetat += (int)(ListeEtatHeur[i].size());
    }
    return value_energie_Heur;
}

int VersionReduced_Simu_ApplyMDPFromsimulationVI(vector<double> & size_proba, vector<double> & deadline_proba, vector<double> inter_arrival_time_proba,int max_speeds,int TailleMaxBuffer, vector<double> & Matrice_vitesse, int max_size, int max_deadline, int max_inter_arrival_time, vector<vector<vector<int> > > & state_space, int T, vector<int> & Vector_State_inter_arrival_time, int tailleListemin, vector<double> & Reduced_sizeproba, int Reduce_maxSize, vector<vector<vector<int> > > & ListeJobArrivalSimulation_MDP, vector<vector<vector<int> > > & ListeJobArrivalSimulation_MDP_VraiVecteur, int WindowsSize,vector<int> & Vecteur_SimuTime_Current_InterArrivalTime,vector<double> & Available_speeds, vector<int> & numberOfJobsByTimeStepsMDP) {
    vector<vector<int> > job_arrivee; // job_arrivee = ensemble des jobs qui arrivent en t.
    vector<vector<int> > liste_job_present_MDP_VRAI,liste_job_present_MDP_REDUCED, etat_suivant_VRAI,etat_suivant_REDUCED;
    int vitesse_MDP_REDUCED=0, vitesse_MDP_VRAI=0;
    vector<vector<int> > job_arrivee_MDP,job_arrivee_MDP_VraiVecteur;
    
    vector<int> StockageVitesseMDP,pasdeTemps;
    vector<vector<vector<int> > > ListeJobArrivalSimulation,ListeEtatMDP, Stockage_etat;
    
    int current_size, current_size_True, current_deadline,current_inter_arrival_time, Nbre_job;
    int nbre_job_executer = 0, delai_k=0;
    int t=0;
    int index_etat_suivant_MDP = tailleListemin-1;
    etat_suivant_REDUCED=state_space[index_etat_suivant_MDP];
    etat_suivant_VRAI=state_space[index_etat_suivant_MDP];
        
    // @DONE Simulation du premier Inter-arrival-time: inter_arrival_time_proba_Normaliser, distribution normalisé dans le cas particulier
    vector<double> inter_arrival_time_proba_Normaliser (inter_arrival_time_proba.begin()+1,inter_arrival_time_proba.end());
    inter_arrival_time_proba_Normaliser=inter_arrival_time_proba_Normaliser/(1-inter_arrival_time_proba[0]);
    current_inter_arrival_time = Simu_VectorProba(inter_arrival_time_proba_Normaliser);
    
    vector<vector<int> > PremArriv;
    ListeJobArrivalSimulation.push_back(PremArriv);
    ListeEtatMDP.push_back(PremArriv);
    
    bool Monitoring=false;
    vector<double> Nbre_job_arrivParXMinB(4,0); // 0,1,2,3
    vector<vector<double> > Nbre_job_arriv;
    for (int i=0; i<4; i++)
        Nbre_job_arriv.push_back(Nbre_job_arrivParXMinB);
    
    int delai_k_MDP=0, current_inter_arrival_time_MDP, Nbre_job_MDP;
    current_inter_arrival_time_MDP = Simu_VectorProba(inter_arrival_time_proba_Normaliser);
    
    while (t<T) {
        // etat_suivant_reduced -> vitesse_reduced
        if (t<T-max_deadline) {
            if (index_etat_suivant_MDP + delai_k_MDP*tailleListemin>Matrice_vitesse.size())
                cout << "OUT OF BOUND " << index_etat_suivant_MDP << "," << delai_k_MDP << "," << tailleListemin << "," << Matrice_vitesse.size() << endl;
            vitesse_MDP_REDUCED = Matrice_vitesse[index_etat_suivant_MDP + delai_k_MDP*tailleListemin];
        } else {
            if (index_etat_suivant_MDP > state_space.size())
                cout << "OUT OF BOUND " << endl;
            vitesse_MDP_REDUCED = Matrice_vitesse[index_etat_suivant_MDP];
        }
        
        // vitesse reduced -> vitesse reel
        vitesse_MDP_VRAI = vitesse_MDP_REDUCED*WindowsSize;
        StockageVitesseMDP.push_back(vitesse_MDP_VRAI); // StockageVitesseMDP contient les vrais valeurs de vitesse
                
        delai_k_MDP++; // add pour aller au bon indice
        // @DONE création de de l'ensemble des jobs arrivée a(t)
        // @DONE Calcul du nombre de job executé
        int res_MDP=vitesse_MDP_VRAI;
        int nbre_job_executer_MDP=0;
        for (int i=0; i<liste_job_present_MDP_VRAI.size(); i++) {
            if (res_MDP-liste_job_present_MDP_VRAI[i][0]+etat_suivant_VRAI[i][0]>=0) {
                nbre_job_executer_MDP++;
                res_MDP-=liste_job_present_MDP_VRAI[i][0]+etat_suivant_VRAI[i][0];
            } else {
                break;
            }
        }
        
        Nbre_job_MDP = (int)(etat_suivant_REDUCED.size()) - nbre_job_executer_MDP;
        // Création du job A(t+1)
        while (current_inter_arrival_time_MDP==0 && Nbre_job_MDP<TailleMaxBuffer) {
            // @DONE A supprimer si on veut garder les valeurs des probas
            if (t<=T-max_deadline) {
                current_size_True = Simu_VectorProba(size_proba)+1;
                // @DONE ajout de current_size
                current_size = SizeJobOnReduceVector(current_size_True, size_proba,Reduced_sizeproba,Reduce_maxSize);
                current_deadline = Simu_VectorProba(deadline_proba)+1;
                vector<int> Ajout_VraiVecteur; Ajout_VraiVecteur.push_back(current_size_True); Ajout_VraiVecteur.push_back(current_deadline);
                vector<int> Ajout; Ajout.push_back(current_size); Ajout.push_back(current_deadline);
                job_arrivee_MDP.push_back(Ajout);
                job_arrivee_MDP_VraiVecteur.push_back(Ajout_VraiVecteur);
                Nbre_job_MDP++;
            }
            current_inter_arrival_time_MDP = Simu_VectorProba(inter_arrival_time_proba);
        }
        // add pour remettre lindice a 0 de recherche dans la metrice_vitesse quand un  job arrive
        if (!(job_arrivee_MDP.empty()) || Nbre_job_MDP==TailleMaxBuffer) {
            delai_k_MDP = 0;
        }
        ListeJobArrivalSimulation_MDP.push_back(job_arrivee_MDP);
        ListeJobArrivalSimulation_MDP_VraiVecteur.push_back(job_arrivee_MDP_VraiVecteur);
                
        ChangementEtatVRAI(liste_job_present_MDP_VRAI,job_arrivee_MDP_VraiVecteur,etat_suivant_VRAI,vitesse_MDP_VRAI);
        TransformListToListReduced(etat_suivant_VRAI,etat_suivant_REDUCED,Available_speeds);
        // From reduced state, determine the index on the state space.
        //       calcul de la position de cet nouvel état dans l'espace d'état
        std::vector<vector<vector<int> > >::iterator it = find(state_space.begin(),state_space.end(), etat_suivant_REDUCED);
        // @DONE Rechercher la bonne valeur avec a la fois Vector_State_inter_arrival_time et state_space
        // DerniereArriveeJob
        index_etat_suivant_MDP = (int)(distance(state_space.begin(),it));
        //        index_etat_suivant_MDP = pos;
        if (t<T-max_deadline && (index_etat_suivant_MDP>=state_space.size() || index_etat_suivant_MDP <0)) {
            cout << "STATE ERROR OUT OF BOUND" << endl;
            cout << etat_suivant_REDUCED << endl;
            cout << endl;
        }
        if (t<T-max_deadline && (index_etat_suivant_MDP>=state_space.size() || index_etat_suivant_MDP <0))
            cout << "OUT OF BOUNDED" << endl;
        //        etat_suivant_MDP=state_space[index_etat_suivant_MDP];
        ListeEtatMDP.push_back(etat_suivant_REDUCED);
                
        if (current_inter_arrival_time_MDP==0) {
            current_inter_arrival_time_MDP = Simu_VectorProba(inter_arrival_time_proba_Normaliser)+1;
        }
        job_arrivee_MDP.erase(job_arrivee_MDP.begin(), job_arrivee_MDP.end());
        job_arrivee_MDP_VraiVecteur.erase(job_arrivee_MDP_VraiVecteur.begin(), job_arrivee_MDP_VraiVecteur.end());
        
        current_inter_arrival_time_MDP--;
        Vecteur_SimuTime_Current_InterArrivalTime.push_back(current_inter_arrival_time_MDP);
        
        pasdeTemps.push_back(t);
        t++;
    }
    /** Number of jobs present at each time step **/
    for (int i=0; i < ListeEtatMDP.size(); i++) {
        numberOfJobsByTimeStepsMDP.push_back((int)(ListeEtatMDP[i].size()));
    }
    int sommeSpeeds = 0, value_energie_MDP = 0;
    for (int i=0; i<StockageVitesseMDP.size(); i++) {
        sommeSpeeds += StockageVitesseMDP[i];
        value_energie_MDP += cout_nrj(StockageVitesseMDP[i], max_speeds);
    }
    return value_energie_MDP;
}
