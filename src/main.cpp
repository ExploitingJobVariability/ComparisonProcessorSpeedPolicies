    //
    //  main.cpp
    //
    //  Created by *** on 02/05/2017.
    //  Copyright © 2017 ***. All rights reserved.
    //
    #include <iostream>
    #include <math.h>
    #include <map>
    #include <random>
    #include <fstream>
    #include <sstream>
    #include "output_R.hpp"
    #include "print_functions.hpp"
    #include "CoarseFineEquivalent.hpp"
    #include "SimulationDataSetArticle.hpp"
    #include <chrono>

    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;

    int main(int argc, const char * argv[]) {
        // Initiate the grain
        srand(0);

        /*** PARAMETRES ***/
        int max_buffer= 3;  // 4, 3                        // nombre de job maximal
        int max_inter_arrival_time = 1;         // Nombre d'inter-arrival-time maximal
        int max_size=4; //10 115                            // Taille max du job
        int max_deadline=3;                         // Deadline maximale
        int typeloi_uniforme = 1;               // Choix de la loi pour la taille 1=> loi discrete
        
        bool not_reducecase = false; // option not used
        
        vector<double> Reduced_sizeproba;
        int Reduce_maxSize = max_size;
        int Reduce_max_speed = Reduce_maxSize*max_buffer;
            
        vector<vector<double> > ens_possible_proba_IAT,ens_possible_proba_deadline,ens_possible_proba_size;
        vector<double> inter_arrival_time_proba,deadline_proba,size_proba;
        SimulationParameter(ens_possible_proba_IAT,ens_possible_proba_deadline,ens_possible_proba_size,inter_arrival_time_proba,deadline_proba,size_proba,max_inter_arrival_time,max_deadline,max_size,Reduce_maxSize);
        
        int max_speeds=max_buffer*max_size;         // Vitesse Maximale
        int bound_M = 2*max_deadline;
        int WindowsSize = Reduce_Vector_Size(size_proba,Reduced_sizeproba,Reduce_maxSize);

        bool ComputeMDP = true, ComputeOA = true, ComputePACE = true, ComputeHeur = true;
        vector<vector<double> > EtudeTaille_sortie;

        cout << "============ Vectors studied ============" << endl;
        cout << "Size vector studies" << endl;
        Affiche(ens_possible_proba_size);
        cout << "Deadline vector studies" << endl;
        Affiche(ens_possible_proba_deadline);
        cout << "InterArrivalTime vector studies" << endl;
        Affiche(ens_possible_proba_IAT);
        cout << endl;
        
        /*** Computation before applying the table ***/
        // Version MDP, compuation of speed/state equivalence
        vector<vector<vector<int> > > state_space;
        vector<int> Vector_State_inter_arrival_time;
        int tailleListemin = 0;
        
        if (ComputeMDP) {
            // @DONE define state space
            Gen_create_state_space_SPEC(max_buffer,Reduce_maxSize,max_deadline,bound_M,max_speeds,state_space,Vector_State_inter_arrival_time,inter_arrival_time_proba,tailleListemin);
        }
        // Suppose que lon a une taillemax, deadline
        for (int configIAT=0; configIAT<ens_possible_proba_IAT.size();configIAT++) {
            for (int configSize=0; configSize<ens_possible_proba_size.size();configSize++) {
                for (int configDeadline=0; configDeadline<ens_possible_proba_deadline.size();configDeadline++) {
                    srand(0);
                    size_proba=ens_possible_proba_size[configSize];
                    deadline_proba=ens_possible_proba_deadline[configDeadline];
                    inter_arrival_time_proba=ens_possible_proba_IAT[configIAT];
                    bool enterloop = true;
                    if (not_reducecase) {
                        Reduced_sizeproba = size_proba;
                        //Reduced_deadline = deadline_proba;
                        //Reduced_IAT = inter_arrival_time_proba;
                        if (size_proba[3] == 0)
                            enterloop = false;
                    }
                    cout << "\n===> Simu" << endl;
                    cout << "Iter n° " << configSize << "/" << ens_possible_proba_size.size() << endl;
                    cout << "Size distribution: " << size_proba << " < " << max_size <<  endl;
                    cout << "Size distribution: " << Reduced_sizeproba << " < " << Reduce_maxSize <<  endl;
                    cout << "Deadline distribution: " << deadline_proba << " < " << max_deadline <<  endl;
                    cout << "Inter-arrival time distribution: " << inter_arrival_time_proba << " < " << max_inter_arrival_time << endl;
                    cout << "Max Buffer size: " << max_buffer << endl; cout << endl;

                    if ((max_inter_arrival_time!=((int)(inter_arrival_time_proba.size())-1)) || (max_size!=(int)(size_proba.size())) || (max_deadline!=(int)(deadline_proba.size())) || (Reduce_maxSize!=(int)Reduced_sizeproba.size())) {
                        cout << "ERROR ON THE VECTOR SIZE" << endl;
                        return 1;
                    }

                    if (enterloop) {
                        vector<double> Speeds_min_VI((int)state_space.size(),0), Nrj_min_VI((int)state_space.size(),0),TempsVI;
                        if (ComputeMDP) {
                            auto t1 = high_resolution_clock::now();
                            // @TODO compute VI
                            double convergenceVI = 0.01;
                            Value_iteration(max_deadline, max_buffer, state_space, Vector_State_inter_arrival_time, Reduce_maxSize, Reduce_max_speed, inter_arrival_time_proba, deadline_proba, Reduced_sizeproba, Speeds_min_VI, Nrj_min_VI,(int)(inter_arrival_time_proba.size())-1, typeloi_uniforme, WindowsSize,convergenceVI);
                            auto t2 = high_resolution_clock::now();
                            /* Getting number of milliseconds as an integer. */
                            auto ms_int = duration_cast<milliseconds>(t2 - t1);
                            /* Getting number of milliseconds as a double. */
                            duration<double, std::milli> ms_double = t2 - t1;
                            std::cout << "timing VI computation: " << (double)ms_int.count()/(double)60000 << " min\n";
                            std::cout << "timing VI computation: " << (double)ms_double.count()/(double)60000 << " min\n";
                            TempsVI.push_back((double)ms_int.count()/(double)60000);
                        }
                        
                        /*** Simulation ***/
                        int nbre_simu = 1000;
                        int nbre_simuGen = 1000;
                        int compteur  = nbre_simuGen;
                        
                        vector<int> numberOfJobsByTimeStepsMDP;
                        vector<int> Sum_numberOfJobsByTimeStepsMDP(nbre_simu,0);
                        vector<vector<int> > List_numberOfJobsByTimeStepsMDP;
                        int valueEnergieSortieMDP,valueEnergieSortiePACE,valueEnergieSortieHEUR,valueEnergieSortieOA;
                        vector<vector<double> >  Sortie_cost_MDP_Lorch,Sortie_cost_MDP_OA,Sortie_cost_MDP_HEUR;
                        double sommeOAvsMDP=0,sommePACEvsMDP=0,sommeHEURvsMDP=0;
                        
                        vector<double> Available_speed;
                        if (max_size>=Reduce_maxSize) {
                            for (int i=0; i <= Reduced_sizeproba.size(); i++) {
                                Available_speed.push_back(i * WindowsSize);
                            }
                            Affiche(Available_speed);
                        }
                        bool outputBySimu = false;
                        
                        while (compteur>0) {
                            vector<vector<vector<int> > > ListeJobArrivalSimulation_MDP, ListeJobArrivalSimulation_MDP_VraiVecteur;
                            vector<int> Vecteur_SimuTime_Current_InterArrivalTime;
                            // Version MDP on true simulation, one trace to execute
                            if (ComputeMDP) {
                                bool Version_Normal_WithoutReduced = true;
                                if (Version_Normal_WithoutReduced) {
                                    if (outputBySimu)
                                        cout << "=========> NORMAL CASE" << endl;
                                    valueEnergieSortieMDP = Simu_ApplyMDPFromsimulationVI(size_proba, deadline_proba,inter_arrival_time_proba,max_speeds,max_buffer, Speeds_min_VI, max_size, max_deadline, max_inter_arrival_time, state_space,nbre_simu,Vector_State_inter_arrival_time,tailleListemin, Reduced_sizeproba,Reduce_maxSize,ListeJobArrivalSimulation_MDP,ListeJobArrivalSimulation_MDP_VraiVecteur,WindowsSize, Vecteur_SimuTime_Current_InterArrivalTime,numberOfJobsByTimeStepsMDP);

                                    List_numberOfJobsByTimeStepsMDP.push_back(numberOfJobsByTimeStepsMDP);
                                    Sum_numberOfJobsByTimeStepsMDP = Sum_numberOfJobsByTimeStepsMDP + numberOfJobsByTimeStepsMDP;
                                    numberOfJobsByTimeStepsMDP.clear();
                                } else {
                                    if (outputBySimu)
                                        cout << "=========> REDUCED CASE" << endl;
                                    valueEnergieSortieMDP = VersionReduced_Simu_ApplyMDPFromsimulationVI(size_proba, deadline_proba,inter_arrival_time_proba,max_speeds,max_buffer, Speeds_min_VI, max_size, max_deadline, max_inter_arrival_time, state_space,nbre_simu,Vector_State_inter_arrival_time,tailleListemin, Reduced_sizeproba,Reduce_maxSize,ListeJobArrivalSimulation_MDP,ListeJobArrivalSimulation_MDP_VraiVecteur,WindowsSize, Vecteur_SimuTime_Current_InterArrivalTime,Available_speed,numberOfJobsByTimeStepsMDP);
                                    
                                    List_numberOfJobsByTimeStepsMDP.push_back(numberOfJobsByTimeStepsMDP);
                                    Sum_numberOfJobsByTimeStepsMDP = Sum_numberOfJobsByTimeStepsMDP + numberOfJobsByTimeStepsMDP;
                                    numberOfJobsByTimeStepsMDP.clear();
                                }
                            }
                            if (ComputeOA) {
                                // @DONE function apply OA on ListeJobArrivalSimulation_MDP_VraiVecteur
                                valueEnergieSortieOA = SimuOA_onMDP_VraiVecteur(size_proba, deadline_proba,inter_arrival_time_proba,max_speeds,max_buffer, Speeds_min_VI, max_size, max_deadline, max_inter_arrival_time,nbre_simu,nbre_simu, Reduced_sizeproba,Reduce_maxSize,ListeJobArrivalSimulation_MDP,ListeJobArrivalSimulation_MDP_VraiVecteur, Available_speed);
                            }
                            if (ComputeOA && ComputeMDP) {
                                double proportion = (double)(valueEnergieSortieOA - valueEnergieSortieMDP) /  (double)valueEnergieSortieMDP;
                                if (outputBySimu) {
                                    cout << " Energy OA " << valueEnergieSortieOA << " vs Energy MDP " << valueEnergieSortieMDP << " and diff " << proportion*100 << " %" << endl;
                                }
                                sommeOAvsMDP += proportion;
                            }
                            if (ComputePACE) {
                                // @DONE function apply PACE on ListeJobArrivalSimulation_MDP_VraiVecteur
                                valueEnergieSortiePACE = SimuPACE_onMDP_VraiVecteur(size_proba, deadline_proba,inter_arrival_time_proba,max_speeds,max_buffer, Speeds_min_VI, max_size, max_deadline, max_inter_arrival_time,nbre_simu,nbre_simu, Reduced_sizeproba,Reduce_maxSize,ListeJobArrivalSimulation_MDP,ListeJobArrivalSimulation_MDP_VraiVecteur, Available_speed);
                            }
                            if (ComputePACE && ComputeMDP) {
                                double proportion = (double)(valueEnergieSortiePACE - valueEnergieSortieMDP) /  (double)valueEnergieSortieMDP;
                                if (outputBySimu)
                                    cout << " Energy PACE " << valueEnergieSortiePACE << " vs Energy MDP " << valueEnergieSortieMDP << " and diff " << proportion*100 << " %"  << endl;
                                sommePACEvsMDP += proportion;
                            }
                            if (ComputeHeur) {
                                // @DONE function apply Heur on ListeJobArrivalSimulation_MDP_VraiVecteur
                                valueEnergieSortieHEUR = SimuHEUR_onMDP_VraiVecteur(size_proba,deadline_proba,inter_arrival_time_proba,max_speeds,max_buffer, max_size, max_deadline,max_inter_arrival_time, nbre_simu, 0,Reduced_sizeproba,Reduce_maxSize,ListeJobArrivalSimulation_MDP,ListeJobArrivalSimulation_MDP_VraiVecteur, Available_speed, Vecteur_SimuTime_Current_InterArrivalTime);
                            }
                            if (ComputeHeur && ComputeMDP) {
                                double proportion = (double)(valueEnergieSortieHEUR - valueEnergieSortieMDP) /  (double)valueEnergieSortieMDP;
                                if (outputBySimu) {
                                    cout << " Energy HEUR " << valueEnergieSortieHEUR << " vs Energy MDP " << valueEnergieSortieMDP << " and diff " << proportion*100 << " %" << endl;
                                }
                                sommeHEURvsMDP += proportion;
                            }
                            vector<double> MDPPACE{(double)(valueEnergieSortieMDP),(double)(valueEnergieSortiePACE)};
                            Sortie_cost_MDP_Lorch.push_back(MDPPACE);
                            vector<double> MDPOA{(double)(valueEnergieSortieMDP),(double)(valueEnergieSortieOA)};
                            Sortie_cost_MDP_OA.push_back(MDPOA);
                            vector<double> MDPHEUR{(double)(valueEnergieSortieMDP),(double)(valueEnergieSortieHEUR)};
                            Sortie_cost_MDP_HEUR.push_back(MDPHEUR);
                            compteur--;
                        }
                        cout << "PART RESULTS:" << endl;
                        cout << "Size distribution: " << size_proba << " < " << max_size <<  endl;
                        cout << "Size distributionReduced: " << Reduced_sizeproba << " < " << Reduce_maxSize <<  endl;
                        cout << "Deadline distribution: " << deadline_proba << " < " << max_deadline <<  endl;
                        cout << "Inter-arrival time distribution: " << inter_arrival_time_proba << " < " << max_inter_arrival_time << endl;
                        if (ComputeOA && ComputeMDP)
                            cout << "Overconsumption on " << nbre_simu << " * " << nbre_simuGen << " is (OAvsMDP): " << (double)sommeOAvsMDP/(double)nbre_simuGen << endl;
                        if (ComputePACE && ComputeMDP)
                            cout << "Overconsumption on " << nbre_simu << " * " << nbre_simuGen << " is (PACEvsMDP): " << (double)sommePACEvsMDP/(double)nbre_simuGen << endl;
                        if (ComputeHeur && ComputeMDP)
                            cout << "Overconsumption on " << nbre_simu << " * " << nbre_simuGen << " is (HEURvsMDP): " << (double)sommeHEURvsMDP/(double)nbre_simuGen << endl;
                        
                        cout << "Computation time of VI: " << TempsVI;
                        int SumOfMDPbufferByTimeSteps = std::accumulate(Sum_numberOfJobsByTimeStepsMDP.begin(), Sum_numberOfJobsByTimeStepsMDP.end(), 0);
                        cout << "Mean number of Lorch job by timestep    : " << (double)SumOfMDPbufferByTimeSteps/(double)nbre_simu/(double)nbre_simuGen << endl;
                        cout << "State space size :                        " << state_space.size() << " etats" << endl;
                        
                        // Affichage mean + interval de confiance
                        cout << "*********** LORCH vs MDP ***********" << endl;
                        vector<double> sortieLorch = Affichage_resultat(Sortie_cost_MDP_Lorch, Sortie_cost_MDP_Lorch, Sortie_cost_MDP_Lorch, nbre_simuGen, nbre_simu, 1, 1, 0);
                        cout << "*********** OA vs MDP ***********" << endl;
                        vector<double> sortieOA = Affichage_resultat(Sortie_cost_MDP_OA, Sortie_cost_MDP_OA, Sortie_cost_MDP_OA, nbre_simuGen, nbre_simu, 1, 1, 0);
                        cout << "*********** HEUR vs MDP ***********" << endl;
                        vector<double> sortieHEUR = Affichage_resultat(Sortie_cost_MDP_HEUR, Sortie_cost_MDP_HEUR, Sortie_cost_MDP_HEUR, nbre_simuGen, nbre_simu, 1, 1, 0);
                        // Cas Section Impact of JobsSize du papier, preparation des valeurs
                        PreparationVecteur_FichierSortiePourR(10,size_proba,sortieLorch,sortieOA,sortieHEUR,EtudeTaille_sortie);
                    } else {
                        // Cas size_proba[3] != 0
                        PreparationVecteur_FichierSortiePourRElse(10,size_proba,EtudeTaille_sortie);
                    }
                }
            }
        }
        // @DONE Recuperation du vecteur EtudeTaille_sortie dans un fichier
        Enregistrer(EtudeTaille_sortie);
        cout << endl;
    
        return 0;
    }

