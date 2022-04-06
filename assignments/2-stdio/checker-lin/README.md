Nume: CIUCIU ANCA-MARIA
Grupă: 331 CBa (333 AA)

# Tema 2

Organizare
-
Pentru implementarea enunțului am folosit structura SO_FILE ce conține mai multe câmpuri: file descriptor-ul, poziția cursorului, bufferul de scriere/citire, poziția în buffer, mărimea fișierului, un indicator de sfârșit al fișierului, un indicator de eroare, numărul chunk-ului de date la care se află cursorul, PID-ul procesului copil și un indicator pentru ultima operație efectuată setat pe r/w. Toate funcțiile bibliotecii sunt implementate în fișierul *libso_stdio.c*, folosind apeluri de sistem.

Abordarea generală
-
* Deschiderea unui fișier în modul dat prin argument, prin intermediul lui **so_fopen** are rolul de a popula structura SO_FILE cu valorile inițiale. Pentru închiderea acestuia cu **so_fclose**, se verifică dacă ultima operație efectuată pe fișier a fost cea de scriere. În acest caz, se face **so_fflush** la fișier pentru a nu pierde datele din buffer care poate încă n-au fost scrise (operație efectuată și la **so_fseek** - unde se setează poziția cursorului).
* Pentru minimizare numărului de apeluri de sistem făcute implementarea funcției **so_fgetc** se bazează pe împărțirea fișierului în chunks, de dimensiunea buffer-ului. Dacă cursorul se află în chunk-ul încărcat în buffer, citirea unui caracter se va face direct din buffer, dacă nu, se va realiza un apel de sistem. 
* Funcția **so_fwrite** se folosește de **so_fputc** pentru a scrie datele din memorie în fișier. În mod asemănător, **so_fread** scrie în spațiul de memorie precizat datele extrase cu **so_fgetc**.
* Pentru crearea unui canal de comunicare între un proces părinte și procesul copil, am implementat funcția **so_popen**. Procesul părinte creează un pipe, rezultând doi descriptori de fișier deschiși: de citire și de scriere, după care se lansează un nou proces cu *fork()*.
În cazul în care această acțiune nu se execută cu succes, se vor închide ambele capete ale pipe-ului. Pentru comunicarea datelor între părinte și copil, în funcție de modul specificat (r/w), procesul copil va închide descriptorul de fișier de care nu are nevoie și va redirecta STDOUT la fd[1] (pentru modul de citire) și STDIN la fd[0] (pentru modul de scriere). Totodată, procesul părinte va închide descriptorii nefolosiți. Prin **so_pclose** se așteaptă finalizarea procesului copil si se eliberează spațiul de memorie alocat.
  
* Dacă pe parcurs, s-a întâlnit vreo eroare în urma unei operații, err_ind va fi setat la valoarea SO_EOF, fiind returnat în **so_ferror**.
* Utilitatea implementării acestei teme constă în o înțelegerea profundă asupra modului de funcționare a bibliotecii standard input/output cât și a conceptelor de I/O buffering.
* Consider implementarea a fi una mai degrabă naivă deoarece aceasta a fost realizată în mod progresiv pe testele din checker.

Implementare
-
* Întregul enunț al problemei a fost implementat.
* Dificultățile întâmpinate pe parcursul implementării fac referire la teste precum fwrite/fread huge chunked deoarece următoarele aspecte nu mi s-au părut intuitive: faptul că se seta write să fie random mai mic decât buffer, sau că se pun valori random maxime pe care poate să le citească read, mai mici decât dimensiunea buffer-ului. Ca să rezolv problema, am pus un cod de eroare (-2) atunci când încerc să citesc din ceva ce ar trebui să fie în buffer și nu este. În fread se apelează read până când se citește tot chunk-ul care ar fi trebuit încărcat inițial în buffer. 

Cum se compilează și cum se rulează?
-
* Generarea bibliotecii dinamice libso_stdio.so se realizează cu ajutorul comenzii *make*
* Structura Makefile:
    - **build** => compilează biblioteca dinamică libso_stdio.o
    - **libso_stdio.so** => creează biblioteca dinamică cu ajutorul fișierului obiect (opțiunea *-shared* este folosită pentru crearea unui shared object)
    - **libso_stdio.o** => creează fișierul obiect (cu opțiunea -fPIC) folosindu-se de implementarea din libso_stdio.c
    - **clean** => șterge biblioteca dinamică rezultată și toate fișierele obiect

Bibliografie
-
* https://man7.org/linux/man-pages/index.html
* https://ocw.cs.pub.ro/courses/so/teme/tema-2
* https://ocw.cs.pub.ro/courses/so/laboratoare

Git
-
* Link către repo-ul privat de git: https://github.com/ancamariac/Multi-platform-Development/tree/master/assignments/2-stdio/checker-lin