//
//  backward_resolution.hpp
//
//  Created by *** on 02/05/2017.
//  Copyright © 2017 ***. All rights reserved.
//

#ifndef general_function_hpp
#define general_function_hpp

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <numeric>
#include <map>


using namespace std;

/*********** SURCHARGE d'opérateur pour la classe vector ***********/
template <class Ty, class Alloc, typename T>
std::vector<Ty,Alloc>& operator*=(std::vector<Ty,Alloc> & this_, T v_)
{
        for( std::size_t i = 0; i < this_.size(); ++i) this_[i]*= (Ty)v_; // C++11
        return this_;
}

template <class Ty, class Alloc, class T>
std::vector<Ty,Alloc> operator*(std::vector<Ty,Alloc> const & A, T const & a)
{
        std::vector<Ty,Alloc> tmp(A);
        tmp *= a;
        return tmp;
}

// soustrait une constante a tous les elements de mon vecteur
template <class Ty, class Alloc, typename T>
std::vector<Ty,Alloc>& operator-=(std::vector<Ty,Alloc> & this_, T v_)
{
        for( std::size_t i = 0; i < this_.size(); ++i) this_[i]-= (Ty)v_; // C++11
        return this_;
}

template <class Ty, class Alloc, class T>
std::vector<Ty,Alloc> operator-(std::vector<Ty,Alloc> const & A, T const & a)
{
        std::vector<Ty,Alloc> tmp(A);
        tmp -= a;
        return tmp;
}
// additionne une constante a tous les elements de mon vecteur
template <class Ty, class Alloc, typename T>
std::vector<Ty,Alloc>& operator+=(std::vector<Ty,Alloc> & this_, T v_)
{
        for( std::size_t i = 0; i < this_.size(); ++i) this_[i]-= (Ty)v_; // C++11
        return this_;
}

template <class Ty, class Alloc, class T>
std::vector<Ty,Alloc> operator+(std::vector<Ty,Alloc> const & A, T const & a)
{
        std::vector<Ty,Alloc> tmp(A);
        tmp += a;
        return tmp;
}
template <class Ty, class Alloc, class T>
std::vector<Ty,Alloc> operator*(T const & a, std::vector<Ty,Alloc> const & A)
{
        std::vector<Ty,Alloc> tmp(A);
        tmp *= a;
        return tmp;
}

template <class Ty, class Alloc, class T>
std::vector<Ty,Alloc> operator/(std::vector<Ty,Alloc> const & A, T const & a)
{
        std::vector<Ty,Alloc> tmp(A);
        tmp *= 1/a;
        return tmp;
}

template <class Ty, class Alloc>
std::vector<Ty,Alloc> operator+(std::vector<Ty,Alloc> const & A, std::vector<Ty,Alloc> const & B)
{
        std::vector<Ty,Alloc> tmp(A);
        for( std::size_t i = 0; i < A.size(); ++i) tmp[i] = A[i] + B[i]; // C++11
        return tmp;
}

template <class Ty, class Alloc>
std::vector<Ty,Alloc> operator-(std::vector<Ty,Alloc> const & A, std::vector<Ty,Alloc> const & B)
{
        std::vector<Ty,Alloc> tmp(A);
        for( std::size_t i = 0; i < A.size(); ++i) tmp[i] = A[i] - B[i]; // C++11
        return tmp;
}

template <class Ty, class Alloc>
std::ostream &operator<<(std::ostream &out, std::vector<Ty,Alloc> &vecteur) {
        for( std::size_t i = 0; i < vecteur.size(); ++i) cout << std::setprecision(12) << vecteur[i] << ','; // C++11
        cout << endl;
        return out;
}

/***** AFFICHE VECTEUR ********/
void Affiche(const vector<double> & vecteur);
void Affiche(const vector<int> & vecteur);
/***** AFFICHE ESPACE DETAT ********/
void Affiche(const vector<vector<vector<double> > > & vecteur);
void Affiche(const vector<vector<double> > & vecteur);
void Affiche(const vector<vector<int> > & vecteur);
void Affiche_int(const vector<vector<vector<int> > > & vecteur);
vector<int> Affiche(vector<int> & vecteur, int scalaire);
vector<int> add_vector(vector<int> & vecteur1, vector<int> & vecteur2);

/***** FUNCTION COUT *****/
double cout_nrj(const double & vitesse, double vitesseMax);
double cout_nrj(const double & vitesse, double vitesseMax, int windowSize);

double span(vector<double> a);

int Simu_VectorProba(vector<double> Vector);

bool cmp(const vector<int> &a, const vector<int> &b);

#endif /* general_function_hpp */
