//
// Created by lunde on 2020/11/1.
//

#ifndef EVENEMENT_H_INCLUDED
#define EVENEMENT_H_INCLUDED

#include <iostream>
#include <string>
#include "timing.h"
#include <vector>

using namespace std;

namespace TIME{

    // Une fois implemente : on peut changer Evt1f par Evt dans Agenda
    class Evt {
        string sujet;
    public:
        Evt(string sujet) : sujet(sujet) {

        }
        const string & getDescription() const {
            return sujet;
        }

        virtual string toString() const=0;
        void afficher(std::ostream& f=std::cout) const { f<<toString(); }

        virtual ~Evt() {};
        virtual Evt* clone() const=0;

    };

    class EvtPj : public Evt {
    private:
        Date date_debut;
        Date date_fin;
    public:
        EvtPj(const Date& debut, const Date& fin, const std::string& s):date_debut(debut), date_fin(fin),Evt(s){
//            cout << "Construction of Evt1j" << endl;
        }
        const Date& getDateDebut() const { return date_debut; }
        const Date& getDateFin() const { return date_fin; }

//        void afficher(std::ostream& f= std::cout) const {
//            f<<"***** Evt ********"<<"\n"<<"Date debut="<<date_debut<<", date fin = " << date_fin << " sujet="<< getDescription()<<"\n";
//        }

        EvtPj* clone() const;

        string toString() const;

        ~EvtPj()
        {
//            std::cout << "Destruction of EvtPj" << std::endl;
        }
    };


    class Evt1j : public Evt {
    private:
        Date date;
    public:
        Evt1j(const Date& d, const std::string& s):date(d),Evt(s){
//            cout << "Construction of Evt1j" << endl;
        }
        const Date& getDate() const { return date; }
//        void afficher(std::ostream& f= std::cout) const {
//            f<<"***** Evt ********"<<"\n"<<"Date="<<date<<" sujet="<< getDescription()<<"\n";
//        }

        Evt1j* clone() const;

        string toString() const;

        virtual ~Evt1j()
        {
//            std::cout << "Destruction of Evt1j" << std::endl;
        }
    };

    class Evt1jDur: public Evt1j
    {
    private:
        Horaire debut;
        Duree duree;
    public:
        string id;
        Evt1jDur(const Date& d, const std::string& s,
                 const Horaire& h, const Duree& t):
                Evt1j(d,s), debut(h), duree(t){
//            cout << "Construction of Evt1jDur" << endl;
        }
        const Horaire& getHoraire() const { return debut; }
        const Duree& getDuree() const { return duree; }
//        void afficher(std::ostream& f= std::cout) const {
//            Evt1j::afficher(f);
//            f<<"debut="<<debut<<" duree="<<duree<<"\n";
//        }

        Evt1jDur* clone() const;

        string toString() const;

        ~Evt1jDur()
        {
//            std::cout << "Destruction of Evt1jDur" << std::endl;
        }
    };


    class Rdv : public Evt1jDur {
        std::string personne;
        std::string lieu;
    public:
        Rdv(const Date& d, const std::string& s,
            const Horaire& h,const Duree& dur,
            const std::string& p, const std::string& l):
                Evt1jDur(d,s,h,dur),
                personne(p),lieu(l){
//            cout << "Construction of Rdv" << endl;
        }
        const std::string& getPersonne() const { return personne; }
        const std::string& getLieu() const { return lieu; }
//        void afficher(std::ostream& f=std::cout) const {
//            Evt1jDur::afficher(f); // rappel de la méthode de la classe de base.
//            f<<"personne(s)="<<personne<<" lieu="<<lieu<<"\n";
//        }

        Rdv* clone() const;

        string toString() const;

        ~Rdv()
        {
//            std::cout << "Destruction of Rdv" << std::endl;
        }

        Rdv(const Rdv& r):
                Evt1jDur(r.getDate(), r.getDescription(), getHoraire(), r.getDuree()),
                personne(r.personne), lieu(r.lieu)
        {
        }

        Rdv& operator=(const Rdv& r) {
            Evt1jDur * p = this;
            *p = r;
//            (*this).Evt1jDur::operator=(r);
            personne = r.personne;
            lieu = r.lieu;
            return *this;
        }
    };

    class Agenda {

    private:
        vector<Evt *> events;

    public:

        Agenda()=default;

        Agenda(const Agenda &)=delete;
        Agenda& operator = (const Agenda &)=delete;

        Agenda& operator<<(Evt& e);

        void afficher(std::ostream& f= std::cout) const;

        // ITERATOR DESIGN PATTTERN : On implement un classe "iterator" a l'interieur de la classe qui necessite un iterateur

        // Ici la classe de base est "std::vector<Evt *>::iterator:
        class iterator : public std::vector<Evt *>::iterator {
        public :
            Evt & operator *() {
                // Note 1: Il est possible d'appeler le code d'un operateur comme une fonction :
                // ClasseDeBase::operator@()
                // Note 2: Ici notre classe de base est "std::vector<Evt*>::iterator", car iterator herite de cette classe
                // Note 3: La valeur retourner par l'operator * d'un iterateur STL est un des elements du conteneur, ici un element du conteneur a un type Evt *
                //         Donc pour retourner une reference on doit retourner : *(*it), ici : *std::vector<Evt *>::iterator::operator*()
                return *std::vector<Evt *>::iterator::operator*();
            }

            void operator ++ (int) {
                std::vector<Evt *>::iterator::operator++();
            }

            void operator --(int) {
                std::vector<Evt*>::iterator::operator--();
            }

            // Lors de la creation de l'objet de type 'iterator' on viendra donner en parametre d'entree un "vector iterateur"
            iterator(const std::vector<Evt*>::iterator &it) : std::vector<Evt*>::iterator(it)
            {}
        };

        // ITERATOR DESIGN PATTTERN : On implement une methode begin pour acceder au premier iterateur de la sequence
        iterator begin() {
            return iterator(events.begin());
        }

        // ITERATOR DESIGN PATTTERN : On implement une methode begin pour acceder au dernier iterateur de la sequence
        iterator end() {
            return iterator(events.end());
        }

        ~Agenda();
    };

    inline Date getDate(const Evt& e) {
        const Evt1j *pt1 = dynamic_cast<const Evt1j *>(&e);
        const EvtPj *pt2 = dynamic_cast<const EvtPj *>(&e);
        if (pt1) return pt1->getDate();
        if (pt2) return pt2->getDateDebut();
        throw "type evt inattendu";
    }

//    inline bool operator<(const Evt& e1, const Evt& e2){
//        Date d1=getDate(e1);
//        Date d2=getDate(e2);
//        if (d1<d2) return true;
//        if (d2<d1) return false;
//
//        // d1==d2
//        const Evt1jDur* pt1=dynamic_cast<const Evt1jDur*>(&e1);
//        const Evt1jDur* pt2=dynamic_cast<const Evt1jDur*>(&e2);
//        if (pt1==nullptr && pt2!=nullptr) return true;
//        if (pt1!=nullptr && pt2==nullptr) return false;
//        // ce sont deux evt avec un horaire, on les compare
//        return pt1->getHoraire() < pt2->getHoraire();
//    }

}

std::ostream& operator<<(std::ostream& f, const TIME::Evt& e);



#endif // EVENEMENT_H_INCLUDED
