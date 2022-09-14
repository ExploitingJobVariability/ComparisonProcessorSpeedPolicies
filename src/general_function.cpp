
//
//  backward_resolution.cpp
//  finite_case
//
//  Created by *** on 02/05/2017.
//  Copyright © 2017 ***. All rights reserved.
//

#include "general_function.hpp"
#include <algorithm>
#include <iterator>
#include <list>
#include <math.h>
#include <cmath>
#include <assert.h>
#include <fstream>
#include <iomanip>
using namespace std;

/***** AFFICHE VECTEUR ********/
void Affiche(const vector<double> & vecteur) {
        // utilisation d'un itérateur
        for (vector<double>::const_iterator i = vecteur.begin(); i != vecteur.end(); ++i)
                cout << *i << ' ';
        return void();
}
void Affiche(const vector<int> & vecteur) {
        // utilisation d'un itérateur
        for (vector<int>::const_iterator i = vecteur.begin(); i != vecteur.end(); ++i)
                cout << *i << ' ';
        return void();
}
/***** AFFICHE ESPACE DETAT ********/
void Affiche(const vector<vector<double> > & vecteur) {
        // utilisation d'un itérateur
        for (int i = 0; i < (int)vecteur.size(); i++) {
                cout << i << ":  "; Affiche(vecteur[i]); cout<<endl;
        }
        return void();
}
void Affiche(const vector<vector<vector<double> > > & vecteur){
        for (int i = 0; i < (int)vecteur.size(); i++) {
                cout << i << ":  "; Affiche(vecteur[i]); cout<<endl;
        }
        return void();
}

/***** AFFICHE ESPACE DETAT ********/
void Affiche(const vector<vector<int> > & vecteur) {
        // utilisation d'un itérateur
        for (int i = 0; i < (int)vecteur.size(); i++) {
                cout << "       "; Affiche(vecteur[i]); cout<<endl;
        }
        return void();
}
void Affiche_int(const vector<vector<vector<int> > > & vecteur){
        for (int i = 0; i < (int)vecteur.size(); i++) {
                cout << i << ":  "; Affiche(vecteur[i]); cout<<endl;
        }
        return void();
}

/***** AJOUT SCALAIRE ********/
vector<int> Affiche(vector<int> & vecteur, int scalaire) {
        for (int i = 0; i < (int)vecteur.size(); i++) {
                vecteur.at(i) = vecteur.at(i) + scalaire;
        }
        return(vecteur);
}
/***** AJOUT VECTOR ********/
vector<int> add_vector(vector<int> & vecteur1, vector<int> & vecteur2) {
        vector<int> vecteur;
        for (int i = 0; i < (int)vecteur1.size(); i++) {
                vecteur.at(i) = vecteur1.at(i) + vecteur2.at(i);
        }
        return(vecteur);
}

/***************************************************************************************************************************************/
// coût énergétique lié à la vitesse => fonction de coût de backward itération (Ici fonction coût = fonction cubique)
//           donne le cout en fonction de la vitesse, choix d'une fonction quadratique pour commencer
//
// sortie:
//          double : energie, énergie consommé par le processeur sous une vitesse fixée
// entrée :
//          vitesse : vitesse à laquelle fonctionne le processeur
/***************************************************************************************************************************************/
double cout_nrj(const double & vitesse, double vitesseMax) {
        double energie;
        if (vitesse <=vitesseMax) {
                energie = vitesse*vitesse*vitesse;
        } else {
                energie = +INFINITY; cout << "WARNING: Infinite Speed" << endl;
        }
        return energie;
}

double cout_nrj(const double & vitesse, double vitesseMax, int windowSize) {
    return cout_nrj(windowSize*vitesse,windowSize*vitesseMax);
}

double calcul_vitesse_OA(int etat,vector<vector<double> > espace_detat_init) {
        vector<double> pente;
        for (int i=0; i<espace_detat_init[etat].size(); i++) {
                pente.push_back((double)espace_detat_init[etat][i]/(double)(i+1));
        }
        return(ceil(*max_element(pente.begin(), pente.end())));
}
double calcul_vitesse_OA(vector<double> & nvel_etat_OA) {
        vector<double> pente;
        for (int i=0; i<nvel_etat_OA.size(); i++) {
                pente.push_back((double)nvel_etat_OA[i]/(double)(i+1));
        }
        return(ceil(*max_element(pente.begin(), pente.end())));
}

/****************************************************************************************************/
// semi-norme : retourne la semi-norme span : max-min
// sortie: la valeur de la norm
/****************************************************************************************************/
double span(vector<double> a) {
    std::vector<double>::iterator it = a.begin();
    //    cout << "distance min " << std::distance(it, min_element(a.begin(), a.end())) <<endl;
    //    cout << "distance max " << std::distance(it, max_element(a.begin(), a.end())) <<endl;
    return (*max_element(a.begin(), a.end()) - *min_element(a.begin(), a.end()));
}

int unif_size(int max_size) {
    int size;
    double value_random,deb,fin;
    
    value_random = rand()/(double)RAND_MAX;
    for (int i=0; i<max_size; i++) {
        deb =(double)i/(double)max_size;
        fin = (double)(i+1)/(double)max_size;
        if ( value_random >= deb && fin > value_random) {
            size = i+1;
            break;
        }
    }
    return size;
}

int Simu_VectorProba(vector<double> Vector) {
    int value;
    // Parcours de Vector et sommation
    double somme=0;
    vector<double> Somme_Vector;
    for (int i=0; i<Vector.size(); i++) {
        somme+=Vector[i];
        Somme_Vector.push_back(somme);
    }
    
    double value_random = rand()/(double)RAND_MAX;
    double deb,fin;
    deb=0;
    for (int i=0; i<Somme_Vector.size(); i++) {
        fin = Somme_Vector[i];
        if ( value_random >= deb && fin > value_random) {
            value = i;
            break;
        }
        deb = Somme_Vector[i];
    }
    return value;
}

// Function de comparaison de deux vecteurs en fonctions des deadlines (ie le deuxième paramètre) => utile pour le tri (function sort())
bool cmp(const vector<int> &a, const vector<int> &b) {
    return a[1]<b[1];
}
