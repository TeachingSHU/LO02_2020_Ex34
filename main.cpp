#include <iostream>

#include <iostream>
#include "evenement.h"
using namespace std;
using namespace TIME;


void ex34_question_1()
{
    /**/
    Evt1j e1(Date(4,10,1957),"Spoutnik");
    Evt1j e2(Date(11,6,2013),"Shenzhou");
    Evt1jDur e3(Date(11,6,2013),"Lancement de Longue Marche",Horaire(17,38),Duree(10));
    Rdv e4(Date(11,4,2013),"reunion UV",Horaire(17,30),Duree(60),"Intervenants UV","bureau");
    /**/

    /**/
    Agenda a;

    a << e1 << e2 << e3 << e4;
    a.afficher(std::cout);

    for(Agenda::iterator it = a.begin(); it != a.end() ; it++) {
        (*it).afficher();
    }
}


void ex32_question_2()
{

}


int main(){
    ex34_question_1();
    return 0;
}


