# E-VOTE

# PROJECT INSTRUCTONS:

| Teaching Assistent | Dragoș B | Link template | https://github.com/Ionnier/oop-template |

## Compiling Instructions

The project is configuered with Cmake.

Terminal instructions:

1. The configuration step
```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
# or ./scripts/cmake.sh configure
```

Or on Windows with GCC:
```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -G Ninja
# or ./scripts/cmake.sh configure -g Ninja
```

At this step we can ask to generate project files for various work environments.

## Mandatory requirements

Failure to comply will result in the project being disqualified.

  - the program will be written in C++
  - the program will have an interactive menu (just to illustrate the functionality)
  - the program has no compilation errors
  - no global variables
  - private (or protected) member data
  - GitHub Actions passed
  - appropriate and punctual Git commits
  - meaningless use of language features
  - use of language features with the aim of "legally" violating another rule
      - excessive use of friend classes
      - excessive use of static elements
  - each requirement will be scored 1/2 on implementation + 1/2 on the fact that it was implemented meaningfully

## Cerințe
- [ ] definirea a minim **2-3 ieararhii de clase** care sa interactioneze in cadrul temei alese (fie prin compunere, agregare sau doar sa apeleze metodele celeilalte intr-un mod logic) (6p)
  - minim o clasa cu:
    - [ ] constructori de inițializare [*](https://github.com/Ionnier/poo/tree/main/labs/L02#crearea-obiectelor)
    - [ ] constructor supraîncărcat [*](https://github.com/Ionnier/poo/tree/main/labs/L02#supra%C3%AEnc%C4%83rcarea-func%C8%9Biilor)
    - [ ] constructori de copiere [*](https://github.com/Ionnier/poo/tree/main/labs/L02#crearea-obiectelor)
    - [ ] `operator=` de copiere [*](https://github.com/Ionnier/poo/tree/main/labs/L02#supra%C3%AEnc%C4%83rcarea-operatorilor)
    - [ ] destructor [*](https://github.com/Ionnier/poo/tree/main/labs/L02#crearea-obiectelor)
    - [ ] `operator<<` pentru afișare (std::ostream) [*](https://github.com/Ionnier/poo/blob/main/labs/L02/fractie.cpp#L123)
    - [ ] `operator>>` pentru citire (std::istream) [*](https://github.com/Ionnier/poo/blob/main/labs/L02/fractie.cpp#L128)
    - [ ] alt operator supraîncărcat ca funcție membră [*](https://github.com/Ionnier/poo/blob/main/labs/L02/fractie.cpp#L32)
    - [ ] alt operator supraîncărcat ca funcție non-membră [*](https://github.com/Ionnier/poo/blob/main/labs/L02/fractie.cpp#L39) - nu neaparat ca friend
  - in derivate
      - [ ] implementarea funcționalităților alese prin [upcast](https://github.com/Ionnier/poo/tree/main/labs/L04#solu%C8%9Bie-func%C8%9Bii-virtuale-late-binding) și [downcast](https://github.com/Ionnier/poo/tree/main/labs/L04#smarter-downcast-dynamic-cast)
        - aceasta va fi făcută prin **2-3** metode specifice temei alese
        - funcțiile pentru citire / afișare sau destructorul nu sunt incluse deși o să trebuiască să le implementați 
      - [ ] apelarea constructorului din clasa de bază din [constructori din derivate](https://github.com/Ionnier/poo/tree/main/labs/L04#comportamentul-constructorului-la-derivare)
      - [ ] suprascris [cc](https://github.com/Ionnier/poo/tree/main/labs/L04#comportamentul-constructorului-de-copiere-la-derivare)/op= pentru copieri/atribuiri corecte
      - [ ] destructor [virtual](https://github.com/Ionnier/poo/tree/main/labs/L04#solu%C8%9Bie-func%C8%9Bii-virtuale-late-binding)
  - pentru celelalte clase se va definii doar ce e nevoie
  - minim o ierarhie mai dezvoltata (cu 2-3 clase dintr-o clasa de baza)
  - ierarhie de clasa se considera si daca exista doar o clasa de bază însă care nu moștenește dintr-o clasă din altă ierarhie
- [ ] cât mai multe `const` [(0.25p)](https://github.com/Ionnier/poo/tree/main/labs/L04#reminder-const-everywhere)
- [ ] funcții și atribute `static` (în clase) [0.5p](https://github.com/Ionnier/poo/tree/main/labs/L04#static)
  - [ ] 1+ atribute statice non-triviale 
  - [ ] 1+ funcții statice non-triviale
- [ ] excepții [0.5p](https://github.com/Ionnier/poo/tree/main/labs/L04#exception-handling)
  - porniți de la `std::exception`
  - ilustrați propagarea excepțiilor
  - ilustrati upcasting-ul în blocurile catch
  - minim folosit într-un loc în care tratarea erorilor în modurile clasice este mai dificilă
- [ ] folosirea unei clase abstracte [(0.25p)](https://github.com/Ionnier/poo/tree/main/labs/L04#clase-abstracte)
 - [ ] clase template
   - [ ] crearea unei clase template [(1p)](https://github.com/Ionnier/poo/tree/main/labs/L08)
   - [ ] 2 instanțieri ale acestei clase (0.5p)
 - STL [(0.25p)](https://github.com/Ionnier/poo/tree/main/labs/L07#stl)
   - [ ] utilizarea a două structuri (containere) diferite (vector, list sau orice alt container care e mai mult sau mai putin un array)
   - [ ] utilizarea a unui algoritm cu funcție lambda (de exemplu, sort, transform)
 - Design Patterns [(0.75p)](https://github.com/Ionnier/poo/tree/main/labs/L08)
   - [ ] utilizarea a două șabloane de proiectare

### Observații

* Pot exista depunctări până la 2p pentru diferite aspecte precum:
  - memory leak-uri
  - nefolosirea destructorului virtual la nevoie
  - abuzarea de diferite concepte (toate funcțiile declarate virtual)
  - apelarea de funcții virtual în constructori

* În general, acestea sunt prezente în [CppCoreGuideline](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md), dar nu e nevoie să parcurgeți documentul, doar să scrieți codul suficient de organizat

* folderele `build/` și `install_dir/` sunt adăugate în fișierul `.gitignore` deoarece
conțin fișiere generate și nu ne ajută să le versionăm.
