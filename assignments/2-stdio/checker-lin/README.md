Nume: CIUCIU ANCA-MARIA
Grupă: 331 CBa (333 AA)

# Tema 2

Organizare
-
    Pentru implementarea enunțului am folosit structura SO_FILE ce conține mai multe câmpuri: file descriptor-ul, poziția cursorului, bufferul de scriere/citire, poziția în buffer, mărimea fișierului, un indicator de sfârșit al fișierului, un indicator de eroare, numărul chunk-ului de date la care se află cursorul, PID-ul procesului copil și un indicator pentru ultima operație efectuată setat pe r/w.

* De făcut referință la abordarea generală menționată în paragraful de mai sus. Aici se pot băga bucăți de cod/funcții - etc.
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