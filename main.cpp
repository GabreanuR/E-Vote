#include "include/Menus/MainMenu.h"
#include "include/Services/UserService.h"
#include "include/Services/ElectionService.h"
#include "include/Services/CandidateService.h"
#include "include/Services/LocationService.h"
#include <iostream>
#include <exception>

int main() {
    try {
        UserService::getInstance();
        ElectionService::getInstance();
        CandidateService::getInstance();
        LocationService::getInstance();

        MainMenu mainMenu;
        mainMenu.display();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    try {
        LocationService::getInstance().saveLocationsToDataManager();
        CandidateService::getInstance().saveCandidatesToDataManager();
        ElectionService::getInstance().saveElectionsToDataManager();
        UserService::getInstance().saveUsersToDataManager();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 2;
    }

    return 0;
}


// COD INITIAL
//
// #include <iostream>
// #include <array>
//
// int main() {
//     std::cout << "Hello, world!\n";
//     std::array<int, 100> v{};
//     int nr;
//     std::cout << "Introduceți nr: ";
//     /////////////////////////////////////////////////////////////////////////
//     /// Observație: dacă aveți nevoie să citiți date de intrare de la tastatură,
//     /// dați exemple de date de intrare folosind fișierul tastatura.txt
//     /// Trebuie să aveți în fișierul tastatura.txt suficiente date de intrare
//     /// (în formatul impus de voi) astfel încât execuția programului să se încheie.
//     /// De asemenea, trebuie să adăugați în acest fișier date de intrare
//     /// pentru cât mai multe ramuri de execuție.
//     /// Dorim să facem acest lucru pentru a automatiza testarea codului, fără să
//     /// mai pierdem timp de fiecare dată să introducem de la zero aceleași date de intrare.
//     ///
//     /// Pe GitHub Actions (bife), fișierul tastatura.txt este folosit
//     /// pentru a simula date introduse de la tastatură.
//     /// Bifele verifică dacă programul are erori de compilare, erori de memorie și memory leaks.
//     ///
//     /// Dacă nu puneți în tastatura.txt suficiente date de intrare, îmi rezerv dreptul să vă
//     /// testez codul cu ce date de intrare am chef și să nu pun notă dacă găsesc vreun bug.
//     /// Impun această cerință ca să învățați să faceți un demo și să arătați părțile din
//     /// program care merg (și să le evitați pe cele care nu merg).
//     ///
//     /////////////////////////////////////////////////////////////////////////
//     std::cin >> nr;
//     /////////////////////////////////////////////////////////////////////////
//     for(int i = 0; i < nr; ++i) {
//         std::cout << "v[" << i << "] = ";
//         std::cin >> v[i];
//     }
//     std::cout << "\n\n";
//     std::cout << "Am citit de la tastatură " << nr << " elemente:\n";
//     for(int i = 0; i < nr; ++i) {
//         std::cout << "- " << v[i] << "\n";
//     }
//     ///////////////////////////////////////////////////////////////////////////
//     /// Pentru date citite din fișier, NU folosiți tastatura.txt. Creați-vă voi
//     /// alt fișier propriu cu ce alt nume doriți.
//     /// Exemplu:
//     /// std::ifstream fis("date.txt");
//     /// for(int i = 0; i < nr2; ++i)
//     ///     fis >> v2[i];
//     return 0;
// }
