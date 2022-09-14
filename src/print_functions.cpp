//
//  print_functions.cpp
//  finite_case
//
//  Created by *** on 02/10/2018.
//  Copyright © 2018 ***. All rights reserved.
//

#include "print_functions.hpp"
#include <cmath>
#include <algorithm>

vector<double> int_conf(vector<vector<double> > sortie_gen_DP, double somme, double sommeEcartType) {
        vector<double> sortie;
        double mean = somme/(sortie_gen_DP.size());
        cout <<"mean " <<  somme/(sortie_gen_DP.size()) << endl;
        //        cout << "value_test " << sommeEcartType/(double)(sortie_gen_DP.size()) - pow(mean,2) << endl;
        double ecart_type = sqrt(sommeEcartType/(double)(sortie_gen_DP.size()) - pow(mean,2));
        //        cout << "standard deviation " <<  ecart_type << endl;
        double intconfmin = mean - 1.96*ecart_type/sqrt((int)(sortie_gen_DP.size()));
        double intconfmax = mean + 1.96*ecart_type/sqrt((int)(sortie_gen_DP.size()));
        cout << "confidence interval 95% " << intconfmin << "    " << intconfmax << endl;
        sortie.push_back(mean);sortie.push_back(intconfmin); sortie.push_back(intconfmax);
        return sortie;
};

vector<double> Affichage_resultat(vector<vector<double> > sortie_gen_DP, vector<vector<double> > sortie_gen_VI, vector<vector<double> > sortie_gen_autre, int sample, int T, int affichage, bool int_conf_index, int nbre_simulation_autre) {
        vector<double> moyenne_sortie;
        cout << "The number of simulations of " << T << " iterations is " << sample << endl;
        cout << "Iteration number of " << T << " (time horizon)" << endl;
        double somme = 0, sommeEcartType = 0;
        int nbreAEnlever = 0;
        vector<double> diff;
        double moyenne_OAvsDP=0,moyenne_OAvsVI=0,moyenne_VIvsDP=0,valTotalDP=0,valTotalVI=0,valOA=0,moyenne_OAvsAutre=0,valTotalAutre=0;
        
        if (affichage==0 || affichage==1) {
                // OAvsDP
                for (int j=0; j<sortie_gen_DP.size(); j++){
                        if (sortie_gen_DP[j][0] != 0) {
                                diff.push_back((sortie_gen_DP[j][1]-sortie_gen_DP[j][0])/sortie_gen_DP[j][0]);
                        } else {
                                nbreAEnlever++;
                        }
                }
                for (int j=0; j<diff.size(); j++){
                        somme+=diff[j];
                        sommeEcartType+=pow(diff[j],2);
                        valTotalDP+=sortie_gen_DP[j][0];
                        valOA+=sortie_gen_DP[j][1];
                }
                if (int_conf_index) {
                        int_conf(sortie_gen_DP,somme, sommeEcartType);
                }
                moyenne_OAvsDP = somme/(double)diff.size();
        }
        if (affichage==0 || affichage==2) {
                // OAvsVI
                somme=0; diff.erase(diff.begin(), diff.end());sommeEcartType=0;
                for (int j=0; j<sortie_gen_VI.size(); j++){
                        if (sortie_gen_VI[j][0] != 0) {
                                diff.push_back((sortie_gen_VI[j][1]-sortie_gen_VI[j][0])/sortie_gen_VI[j][0]);
                        } else {
                                nbreAEnlever++;
                        }
                }
                for (int j=0; j<diff.size(); j++){
                        somme+=diff[j];
                        sommeEcartType+=pow(diff[j],2);
                        valTotalVI+=sortie_gen_VI[j][0];
                }
                if (int_conf_index) {
                        int_conf(sortie_gen_VI,somme,sommeEcartType);
                }
                moyenne_OAvsVI = somme/(double)diff.size();
        }
        if (affichage==0 || affichage==3) {
                // VIvsDP
                somme=0; diff.erase(diff.begin(), diff.end());sommeEcartType=0;
                for (int j=0; j<sortie_gen_DP.size(); j++){
                        if (sortie_gen_DP[j][0] != 0) {
                                diff.push_back((sortie_gen_VI[j][0]-sortie_gen_DP[j][0])/sortie_gen_DP[j][0]);
                        } else {
                                nbreAEnlever++;
                        }
                }
                for (int j=0; j<diff.size(); j++){
                        somme+=diff[j];
                        sommeEcartType+=pow(diff[j],2);
                }
                int_conf_index=false;
                if (int_conf_index) {
                        int_conf(sortie_gen_DP,somme, sommeEcartType);
                }
                moyenne_VIvsDP = somme/(double)diff.size();
        }
        if (affichage==0 || affichage==4) {
                // OAvsVI
                somme=0; diff.erase(diff.begin(), diff.end());sommeEcartType=0;
                for (int j=0; j<sortie_gen_autre.size(); j++){
                        if (sortie_gen_autre[j][0] != 0) {
                                diff.push_back((sortie_gen_autre[j][1]-sortie_gen_autre[j][0])/sortie_gen_autre[j][0]);
                        } else {
                                nbreAEnlever++;
                        }
                }
                for (int j=0; j<diff.size(); j++){
                        somme+=diff[j];
                        sommeEcartType+=pow(diff[j],2);
                        valTotalAutre+=sortie_gen_autre[j][0];
                }
                if (int_conf_index) {
                        int_conf(sortie_gen_autre,somme,sommeEcartType);
                }
                moyenne_OAvsAutre = somme/(double)diff.size();
        }
        cout << "<<<<<======================" << endl;
        if (affichage==0 || affichage==1) {
                cout << "Overconsumption vs DP (%) : " << moyenne_OAvsDP*100 << endl;
        }
        if (affichage==0 || affichage==2) {
                cout << "Overconsumption vs VI (%) : " << moyenne_OAvsVI*100 << endl;
        }
        if (affichage==0 || affichage==4) {
                cout << "Overconsumption vs Qlearning (%) : " << moyenne_OAvsAutre*100 << endl;
        }
        if (affichage==3) {
                cout << "VI vs DP (%) : " << moyenne_VIvsDP*100 << endl;
        }
        if (affichage==0)
                cout << "VI vs Qlearning (%) : " << (moyenne_OAvsAutre-moyenne_OAvsVI)*100 << endl;
        cout << "<<<<<======================" << endl;
        moyenne_sortie.push_back(moyenne_OAvsDP);moyenne_sortie.push_back(moyenne_OAvsVI);moyenne_sortie.push_back(moyenne_VIvsDP);moyenne_sortie.push_back(moyenne_OAvsAutre);
        return moyenne_sortie;
}
