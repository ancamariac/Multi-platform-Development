Nume: Ciuciu Anca-Maria
Grupă: 331 CBa (333 AA)

# Tema 1

Organizare

Dezvoltarea funcționalității specifice unui preprocesor s-a realizat cu ajutorul unei structuri de date de tip hashmap (*hashmap.c & hashmap.h*). Această structură dezvoltă la rândul ei o structură de tip Elem pentru păstrarea tuplului cheie-valoare. Funcțiile implementate în cadrul acestei structuri sunt: de creare a hashmapului, stergere a acestuia, inserarea unui element în map, returnarea unui element în funcție de cheie și stergerea unui element din map. Astfel, în urma preprocesării, se vor insera (și eventual, ulterior sterge) asocieri corespunzătoare fiecărei directive în parte.

- Funcția **getArgs** are rolul de a interpreta fiecare argument regăsit în semnătura executabilului.
- Funcția **parseFile** analizează fiecare fișier de input în parte, citindu-se din fișier linie cu linie (cu ajutorul funcției **getLine()**)

***Obligatoriu:*** 
* De făcut referință la abordarea generală menționată în paragraful de mai sus. Aici se pot băga bucăți de cod/funcții - etc.
* Consideri că tema este utilă?
* Consideri implementarea naivă, eficientă, se putea mai bine?

***Opțional:***
* De menționat cazuri speciale (corner cases), nespecificate în enunț și cum au fost tratate.


Implementare
-

* De specificat dacă întregul enunț al temei e implementat
* Dacă există funcționalități extra, pe lângă cele din enunț - descriere succintă (maximum 3-4 rânduri/funcționalitate) + motivarea lor (maximum o frază)
* De specificat funcționalitățile lipsă din enunț (dacă există) și menționat dacă testele reflectă sau nu acest lucru
* Dificultăți întâmpinate
* Lucruri interesante descoperite pe parcurs

Cum se compilează și cum se rulează?
-
* Explicație, ce biblioteci linkează, cum se face build
* Cum se rulează executabilul, se rulează cu argumente (sau nu)

Bibliografie
-

* Resurse utilizate - toate resursele publice de pe internet/cărți/code snippets, chiar dacă sunt laboratoare de SO

Git
-
1. Link către repo-ul de git

Ce să **NU**
-
* Detalii de implementare despre fiecare funcție/fișier în parte
* Fraze lungi care să ocolească subiectul în cauză
* Răspunsuri și idei neargumentate
* Comentarii și *TODO*-uri