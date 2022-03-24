Nume: Ciuciu Anca-Maria
Grupă: 331 CBa (333 AA)

Tema 1

Organizare

Dezvoltarea funcționalității specifice unui preprocesor s-a realizat cu ajutorul unei structuri de date 
de tip hashmap (*hashmap.c & hashmap.h*). Această structură dezvoltă la rândul ei o structură de tip Elem 
pentru păstrarea tuplului cheie-valoare. Funcțiile implementate în cadrul acestei structuri sunt: de creare 
a hashmapului, stergere a acestuia, inserarea unui element în map, returnarea unui element în funcție de cheie 
și stergerea unui element din map. Astfel, în urma preprocesării, se vor insera (și eventual, ulterior sterge) 
asocieri corespunzătoare fiecărei directive în parte.

* Programul funcționează prin citirea fiecărei linii din fișierul de intrare și, în cazul directivelor, 
a aplicării efectelor acestora. În cazul in care întâmpinăm o linie ce nu aparține unei directive, o împărțim 
în tokens pentru a putea înlocui aparițiile prezente în hashmap, adăugate folosind define. După înlocuirea tokenilor, 
linia modificată va fi trimisă către fișierul de output. 
* Pentru directiva #define se va împărți linia (sau liniile in cazul #define-ului de tip multilinie) în key și 
value, pentru a fi adăugate în hashmap. 
*Grupul #if, #elif, #else, #endif este considerat un întreg, folosind o funcție ajutătoare ce primește 
printre parametri valoarea de adevăr a condiției și dacă o altă condiție de tip True a fost procesată anterior 
în același #if. Funcția se apelează recursiv la întâmpinarea #elif și #else și se oprește la întâmpinarea directivei 
#endif.
* Într-un mod asemănător se procedează și pentru grupul #ifdef #ifndef #else #endif. 
* Directiva #include presupune apelarea unei funcții ce cautp fișierul ce trebuie inclus, iar în cazul 
în care acesta este găsit, este apelată funcția principală de parsare a fișierului în mod recursiv, folosind 
ca fișier de intrare pe cel determinat anterior și păstrând fișierul de ieșire original. 
* Codul tratează și cazurile în care o directivă este apelată în interiorul altei directive.
* Consider că această temă este utilă deoarece a trebuit sa analizăm toate constrângerile ce țin de memory leaks.
* Consider că tema ar fi putut fi implementată într-un mod mai ordonat, modularizat.

Implementare
* Am implementat toate cerintele problemei.
* Dificultățile întâmpinate au constat în rezolvarea testelor de memcheck cât și în rezolvarea 
funcționalității preprocesorului pentru ambele sisteme de operare cât și tratarea warning-urilor și a erorilor. 
Spre exemplu, a trebuit sa fac o reimplementare pentru functia getline() din stdio.h deoarece aceasta nu există pe 
Windows.
* Am observat faptul că există anumite warning-uri referitoare la toate functiile din string.h pe Windows. 
Nu am considerat necesară tratarea acestora. Totodată La linia 12: checkerul de coding style detecteaza in mod gresit 
folosirea caracterului * ca inmultire cand, in realitate, este folosit ca pointer, iar acel trailing whitespace din main
nu il vad ca sa il pot sterge. 

Cum se compilează și cum se rulează?
* Bibliotecile folosite în implementarea temei sunt stdio.h, stdlib.h si string.h. Pe langă acestea, am mai folosit și hashmap-ul implementat în hashmap.c si functiile auxiliare implementate in helper.c. 
* Executabilul se rulează folosind comanda ./so-cpp (sau so-cpp.exe pentru windows) urmata, opțional, de argumentele specificate în cerință: -o<OUTFILE> sau -o <OUTFILE> pentru fișierul de ieșire, iar în cazul în care lipsește se va afișa folosind stdout, -I <DIR> pentru includerea directoarelor pentru fișierele procesate prin directiva #include, -D<SYMBOL>[=<MAPPING>] sau -D <SYMBOL>[=<MAPPING>] pentru definirea unui simbol, <INFILE> pentru fișierul de intrare, iar în cazul în care lipsește se va citi folosind stdin
  
Bibliografie
- https://ocw.cs.pub.ro/courses/so/laboratoare/laborator-01
- https://stackoverflow.com/
- https://c-for-dummies.com/

Git
- Repository-ul de git privat:
https://github.com/ancamariac/Multi-platform-Development/tree/master/assignments/1-multi