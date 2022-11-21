//
//  SimulationDataSetArticle.cpp
//  NewVersion_ExploitingJobVariability
//
//  Created by Stéphan Arthur Plassart on 04.07.22.
//

#include "SimulationDataSetArticle.hpp"

// Simulation Tabular Section 6.2
int Simulation_Sec62_ImpactVecteurProbIAT(vector<vector<double> > & ens_possible_proba_IAT, vector<vector<double> > & ens_possible_proba_deadline, vector<vector<double> > & ens_possible_proba_size, int maxIAT) {

    ens_possible_proba_IAT.clear();
    vector<double> vector_proba_courant_IAT;
    if (maxIAT==1) {
        vector_proba_courant_IAT = {(double)0,(double)1};
        ens_possible_proba_IAT.push_back(vector_proba_courant_IAT);
        vector_proba_courant_IAT = {0.25,0.75};
        ens_possible_proba_IAT.push_back(vector_proba_courant_IAT);
        vector_proba_courant_IAT = {0.5,0.5};
        ens_possible_proba_IAT.push_back(vector_proba_courant_IAT);
        vector_proba_courant_IAT = {0.75,0.25};
        ens_possible_proba_IAT.push_back(vector_proba_courant_IAT);
    } else if (maxIAT==2) {
        vector_proba_courant_IAT = {0,0.75,0.25};
        ens_possible_proba_IAT.push_back(vector_proba_courant_IAT);
    } else if (maxIAT==3) {
        vector_proba_courant_IAT = {0,0.25,0.5,0.25};
        ens_possible_proba_IAT.push_back(vector_proba_courant_IAT);
    } else if (maxIAT==4) {
        vector_proba_courant_IAT = {0,0,0.25,0.5,0.25};
        ens_possible_proba_IAT.push_back(vector_proba_courant_IAT);
        vector_proba_courant_IAT = {0,0,0,0.25,0.75};
        ens_possible_proba_IAT.push_back(vector_proba_courant_IAT);
    }
    
    ens_possible_proba_deadline.clear();
    vector<double> vector_proba_courant_deadline;
    //    vector_proba_courant_deadline = {0,0,1};
    vector_proba_courant_deadline = {(double)1/(double)3,(double)1/(double)3,(double)1/(double)3};
    ens_possible_proba_deadline.push_back(vector_proba_courant_deadline);
    
    ens_possible_proba_size.clear();
    vector<double> vector_proba_courant_size;
    vector_proba_courant_size = {0.25,0.25,0.25,0.25};//,0.25,0.25};
    ens_possible_proba_size.push_back(vector_proba_courant_size);

    return 0;
}

int Simulation_VecteurProba_deadline(vector<vector<double> > & ens_possible_proba_deadline) {
    bool casdeadline2 = false;
    if (casdeadline2) {
        vector<double> vector_proba_courant = {0,1};
        ens_possible_proba_deadline.push_back(vector_proba_courant);
        vector_proba_courant = {(double)5/(double)10,(double)5/(double)10};
        ens_possible_proba_deadline.push_back(vector_proba_courant);
        vector_proba_courant = {1,0};
        ens_possible_proba_deadline.push_back(vector_proba_courant);
    } else {
        vector<double> vector_proba_courant = {(double)1/(double)3,(double)1/(double)3,(double)1/(double)3};
        ens_possible_proba_deadline.push_back(vector_proba_courant);
        vector_proba_courant = {0.5,0,0.5};
        ens_possible_proba_deadline.push_back(vector_proba_courant);
        vector_proba_courant = {0,0.5,0.5};
        ens_possible_proba_deadline.push_back(vector_proba_courant);
        vector_proba_courant = {0,0,1};
        ens_possible_proba_deadline.push_back(vector_proba_courant);
    }
    return 0;
}

// Simulation Tabular Section 6.3
int Simulation_Sec63_ImpactDeadlines(vector<vector<double> > & ens_possible_proba_IAT, vector<vector<double> > & ens_possible_proba_deadline, vector<vector<double> > & ens_possible_proba_size, bool JobIsole, int maxD) {
    vector<double> vector_proba_courant;
    if (maxD==2) {
        vector_proba_courant = {0.5,0.5};
        ens_possible_proba_deadline.push_back(vector_proba_courant);
        vector_proba_courant = {0,1};
        ens_possible_proba_deadline.push_back(vector_proba_courant);
    } else if (maxD==3) {
        vector_proba_courant = {(double)1/(double)3,(double)1/(double)3,(double)1/(double)3};
        ens_possible_proba_deadline.push_back(vector_proba_courant);
        vector_proba_courant = {0.5,0,0.5};
        ens_possible_proba_deadline.push_back(vector_proba_courant);
        vector_proba_courant = {0,0.5,0.5};
        ens_possible_proba_deadline.push_back(vector_proba_courant);
        vector_proba_courant = {0,0,1};
        ens_possible_proba_deadline.push_back(vector_proba_courant);
    }
    vector<double> vector_proba_courant_size;
    vector_proba_courant_size = {0.25,0.25,0.25,0.25};
    ens_possible_proba_size.push_back(vector_proba_courant_size);
    
    // Cas Inter Arrival Time, if \tau=3, then jobIsole = true
    if (JobIsole) {
        vector<double> vector_proba_courant_IAT = {(double)0,(double)0,(double)0,(double)1};
        ens_possible_proba_IAT.push_back(vector_proba_courant_IAT);
    } else {
        vector<double> vector_proba_courant_IAT = {(double)0,(double)1};
        ens_possible_proba_IAT.push_back(vector_proba_courant_IAT);
    }
    return 0;
}

// Calcul Proba Taille vecteur rassemblant tous les cas
int CalculProbaTaille (double & somme, vector<double> & vector_proba_courant, vector<vector<double> > & ens_possible_proba_size, int & pos, int & compt, int discretisation_pas) {
    if (somme<1 && compt<3) {
        for (int ci=0; ci<=discretisation_pas-somme*discretisation_pas; ci++) {
            compt++;
            somme+=(double)ci/(double)discretisation_pas;
            vector_proba_courant.push_back((double)ci/(double)discretisation_pas);
            CalculProbaTaille(somme, vector_proba_courant, ens_possible_proba_size, pos, compt,discretisation_pas);
            vector_proba_courant.erase(vector_proba_courant.end()-1);
            somme-=(double)ci/(double)discretisation_pas;
            compt--;
        }
    } else {
        vector_proba_courant.push_back(1-somme);
        int suppr=0;
        if ((int)vector_proba_courant.size()<4) {
            while ((int)vector_proba_courant.size()!=4){
                vector_proba_courant.push_back(0);
                suppr++;
            }
        }
        // si la valeur de c = WCET a une proba > 0 alors on l'enregistre
        // si p(WCET)=0 alors ce n'est plus le WCET
        if (vector_proba_courant[vector_proba_courant.size()-1]>0)
            ens_possible_proba_size.push_back(vector_proba_courant);
        
        if (suppr>0) {
            while (suppr>0){
                vector_proba_courant.erase(vector_proba_courant.end()-1);
                suppr--;
            }
        }
        vector_proba_courant.erase(vector_proba_courant.end()-1);
    }
    return 0;
}

int Simulation_VecteurProba_WCET10(vector<vector<double> > & ens_possible_proba_size, int discretisation_pas) {
    // Ensemble des combinaisons possibles en 10ième pour la distrib des WCETs
    int compt=0, pos=-1;
    vector<double> vector_proba_courant;
    double somme;
    for (int c1=0; c1<=discretisation_pas; c1++) {
        compt=1;
        somme=(double)c1/discretisation_pas;
        vector_proba_courant.push_back((double)c1/discretisation_pas);
        CalculProbaTaille(somme, vector_proba_courant, ens_possible_proba_size, pos, compt, discretisation_pas);
        vector_proba_courant.erase(vector_proba_courant.end()-1);
        compt--;
    }
    // @TODO can remove this for loop with the previous TODO loop
    for (int configSize=0; configSize<ens_possible_proba_size.size();configSize++) {
        for (int i=0; i<4; i++) {
            if (ens_possible_proba_size[configSize][i]<0.05) {
                ens_possible_proba_size[configSize][i]=0;
            } else if (ens_possible_proba_size[configSize][i]>0.09 && ens_possible_proba_size[configSize][i]<0.11) {
                ens_possible_proba_size[configSize][i]=0.1;
            } else if (ens_possible_proba_size[configSize][i]>0.19 && ens_possible_proba_size[configSize][i]<0.21) {
                ens_possible_proba_size[configSize][i]=0.2;
            } else if (ens_possible_proba_size[configSize][i]>0.29 && ens_possible_proba_size[configSize][i]<0.31) {
                ens_possible_proba_size[configSize][i]=0.3;
            } else if (ens_possible_proba_size[configSize][i]>0.39 && ens_possible_proba_size[configSize][i]<0.41) {
                ens_possible_proba_size[configSize][i]=0.4;
            } else if (ens_possible_proba_size[configSize][i]>0.49 && ens_possible_proba_size[configSize][i]<0.51) {
                ens_possible_proba_size[configSize][i]=0.5;
            } else if (ens_possible_proba_size[configSize][i]>0.59 && ens_possible_proba_size[configSize][i]<0.61) {
                ens_possible_proba_size[configSize][i]=0.6;
            } else if (ens_possible_proba_size[configSize][i]>0.69 && ens_possible_proba_size[configSize][i]<0.71) {
                ens_possible_proba_size[configSize][i]=0.7;
            } else if (ens_possible_proba_size[configSize][i]>0.79 && ens_possible_proba_size[configSize][i]<0.81) {
                ens_possible_proba_size[configSize][i]=0.8;
            } else if (ens_possible_proba_size[configSize][i]>0.89 && ens_possible_proba_size[configSize][i]<0.91) {
                ens_possible_proba_size[configSize][i]=0.9;
            } else if (ens_possible_proba_size[configSize][i]>0.99) {
                ens_possible_proba_size[configSize][i]=1;
            }
        }
    }
    return 0;
}

// Simulation Tabular Section 6.4
int Simulation_Sec64_ImpactSize(vector<vector<double> > & ens_possible_proba_IAT, vector<vector<double> > & ens_possible_proba_deadline, vector<vector<double> > & ens_possible_proba_size, bool JobIsole) {
    int discretisation_pas = 10;//10
    Simulation_VecteurProba_WCET10(ens_possible_proba_size, discretisation_pas);
    
    vector<double> vector_proba_courant_deadline = {(double)0,(double)0,(double)1};
    ens_possible_proba_deadline.push_back(vector_proba_courant_deadline);
    
    // Cas Inter Arrival Time, if \tau=3, then jobIsole = true
    if (JobIsole) {
        vector<double> vector_proba_courant_IAT = {(double)0,(double)0,(double)0,(double)1};
        ens_possible_proba_IAT.push_back(vector_proba_courant_IAT);
    } else {
        vector<double> vector_proba_courant_IAT = {(double)0,(double)1};
        ens_possible_proba_IAT.push_back(vector_proba_courant_IAT);
    }
    return 0;

}

// Simulation Tabular Section 6.4
int Simulation_Sec65_Video(vector<vector<double> > & ens_possible_proba_IAT, vector<vector<double> > & ens_possible_proba_deadline, vector<vector<double> > & ens_possible_proba_size, int & maxReduceSize, bool JobIsole) {
    vector<double> vector_proba_courant_size { 0,0,0,0,0.678321679,0.114885115,0.086913087,0.058941059,0.025974026,0.018981019,0.006993007,0.002997003,0,0.001998002,0.001998002,0,0.000999001,0,0.000999001};

    vector<double> vector_proba_courant_deadline = {(double)0,(double)0,(double)1};
    ens_possible_proba_deadline.push_back(vector_proba_courant_deadline);
    
    // Cas Inter Arrival Time, if \tau=3, then jobIsole = true
    if (JobIsole) {
        vector<double> vector_proba_courant_IAT = {(double)0,(double)0,(double)0,(double)1};
        ens_possible_proba_IAT.push_back(vector_proba_courant_IAT);
    } else {
        vector<double> vector_proba_courant_IAT = {(double)0,(double)1};
        ens_possible_proba_IAT.push_back(vector_proba_courant_IAT);
    }
    // maxReduceSize = 10;
    return 0;
}

// Proba considéré pour l'arrivée des taches
double calcul_probabilite(int value,int max_value,string type_loi) {
    double value_proba = 0;
    if (type_loi.compare("uniforme")==0) {
        // Loi uniforme, indep de NewExecWork et ExecWork
        value_proba=1/(double)max_value;
    }
    if (type_loi.compare("max")==0) {
        if (value==max_value) {
            value_proba=1;
        } else {
            value_proba=0;
        }
    }
    return value_proba;
}

// Fonction de creation du vecteur de proba de taille, deadline et inter-arrival-time
int SpecialCase_givenProba(vector<double> & inter_arrival_time_proba,vector<double> & deadline_proba, vector<double> & size_proba, int max_inter_arrival_time, int max_size, int max_deadline) {
    for (int i=0; i<= max_inter_arrival_time; i++) {
        inter_arrival_time_proba.push_back(calcul_probabilite(i,max_inter_arrival_time+1,"uniforme"));
    }
    for (int d=1; d<= max_deadline; d++) {
        //                deadline_proba.push_back(calcul_probabilite(d,max_deadline,"uniforme"));
        deadline_proba.push_back(calcul_probabilite(d,max_deadline,"max"));
    }
    for (int c=1; c<= max_size; c++) {
        size_proba.push_back(calcul_probabilite(c,max_size,"uniforme"));
    }
    
    // size proba
    size_proba[0]=(double)1/4;
    size_proba[1]=(double)1/4;//0.9
    size_proba[2]=(double)1/4;
    size_proba[3]=(double)1/4;//0.1
    
    // deadline
    deadline_proba[0]=(double)0;
    deadline_proba[1]=(double)0;
    deadline_proba[2]=(double)1;
    
    // inter-arrival-time
    inter_arrival_time_proba[0]=(double)0;
    inter_arrival_time_proba[1]=(double)1;
    //    inter_arrival_time_proba[2]=(double)0;
    //    inter_arrival_time_proba[3]=(double)1;
    //    inter_arrival_time_proba[4]=(double)1/4;

    // Check the size of each vectors
    if (size_proba.size() != max_size)
        cout << "ERROR SIZE" << endl;
    if (deadline_proba.size() != max_deadline)
        cout << "ERROR SIZE" << endl;
    if (inter_arrival_time_proba.size() != max_inter_arrival_time+1)
        cout << "ERROR SIZE" << endl;
    return 0;
}

// Choose your simulation !
int SimulationParameter(vector<vector<double> > & ens_possible_proba_IAT,vector<vector<double> > & ens_possible_proba_deadline,vector<vector<double> > & ens_possible_proba_size,vector<double> & inter_arrival_time_proba,vector<double> & deadline_proba,vector<double> & size_proba,int & max_inter_arrival_time,int & max_deadline,int & max_size, int Reduce_maxSize) {
    // Cas Spec test
    bool CasSpec = true;
    // Section62 impact of IAT
    bool Section62_IAT_tab1a = false;
    bool Section62_IAT_tab1b_Deq2 = false;
    bool Section62_IAT_tab1b_Deq3 = false;
    bool Section62_IAT_tab1b_Deq4 = false;
    // Section63 impact of deadlines
    bool Section63tab2a_MaxDeq3_IATeq1 = false;
    bool Section63tab2a_MaxDeq2_IATeq1 = false;
    bool Section63tab2a_MaxDeq3_IATeq3 = false;
    bool Section63tab2a_MaxDeq2_IATeq3 = false;
    // Section 64 impact ET
    bool Section64_IATeq1 = false;
    bool Section64_IATeq3 = false;
    // Section 65 Video
    bool Section65_Video = false;

    // Section62
    if (Section62_IAT_tab1a) {
        Simulation_Sec62_ImpactVecteurProbIAT(ens_possible_proba_IAT,ens_possible_proba_deadline,ens_possible_proba_size,1);
    }
    if (Section62_IAT_tab1b_Deq2) {
        Simulation_Sec62_ImpactVecteurProbIAT(ens_possible_proba_IAT,ens_possible_proba_deadline,ens_possible_proba_size,2);
    }
    if (Section62_IAT_tab1b_Deq3) {
        Simulation_Sec62_ImpactVecteurProbIAT(ens_possible_proba_IAT,ens_possible_proba_deadline,ens_possible_proba_size,3);
    }
    if (Section62_IAT_tab1b_Deq4) {
        Simulation_Sec62_ImpactVecteurProbIAT(ens_possible_proba_IAT,ens_possible_proba_deadline,ens_possible_proba_size,4);
    }
    // Section63
    if (Section63tab2a_MaxDeq2_IATeq1) {
        bool JobIsole = false;
        int maxD = 2;
        Simulation_Sec63_ImpactDeadlines(ens_possible_proba_IAT,ens_possible_proba_deadline,ens_possible_proba_size,JobIsole,maxD);
    }
    if (Section63tab2a_MaxDeq2_IATeq3) {
        bool JobIsole = true;
        int maxD = 2;
        Simulation_Sec63_ImpactDeadlines(ens_possible_proba_IAT,ens_possible_proba_deadline,ens_possible_proba_size,JobIsole,maxD);
    }
    if (Section63tab2a_MaxDeq3_IATeq1) {
        bool JobIsole = false;
        int maxD = 3;
        Simulation_Sec63_ImpactDeadlines(ens_possible_proba_IAT,ens_possible_proba_deadline,ens_possible_proba_size,JobIsole,maxD);
    }
    if (Section63tab2a_MaxDeq3_IATeq3) {
        bool JobIsole = true;
        int maxD = 3;
        Simulation_Sec63_ImpactDeadlines(ens_possible_proba_IAT,ens_possible_proba_deadline,ens_possible_proba_size,JobIsole,maxD);
    }
    if (Section64_IATeq1) {
        bool JobIsole = false;
        Simulation_Sec64_ImpactSize(ens_possible_proba_IAT,ens_possible_proba_deadline,ens_possible_proba_size,JobIsole);
    }
    if (Section64_IATeq3) {
        bool JobIsole = true;
        Simulation_Sec64_ImpactSize(ens_possible_proba_IAT,ens_possible_proba_deadline,ens_possible_proba_size,JobIsole);
    }
    if (Section65_Video) {
        bool JobIsole = true;
        Simulation_Sec65_Video(ens_possible_proba_IAT,ens_possible_proba_deadline,ens_possible_proba_size,Reduce_maxSize,JobIsole);
    }
    if (CasSpec) {
        SpecialCase_givenProba(inter_arrival_time_proba,deadline_proba,size_proba, max_inter_arrival_time, max_size, max_deadline);
        ens_possible_proba_IAT.push_back(inter_arrival_time_proba);
        ens_possible_proba_size.push_back(size_proba);
        ens_possible_proba_deadline.push_back(deadline_proba);
    }
    if (!ens_possible_proba_size.empty() and !ens_possible_proba_deadline.empty() and !ens_possible_proba_IAT.empty()) {
        inter_arrival_time_proba = ens_possible_proba_IAT[0];
        size_proba = ens_possible_proba_size[0];
        deadline_proba = ens_possible_proba_deadline[0];
        max_inter_arrival_time = (int)(inter_arrival_time_proba.size())-1;
        max_size = (int)(size_proba.size());
        max_deadline = (int)(deadline_proba.size());
    }
    return 0;
}
