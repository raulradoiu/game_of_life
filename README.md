# game_of_life

Acest proiect implementează simularea **Game of Life** a lui John Conway, în limbajul C.


## Ce face programul

Programul primește o matrice de celule (vii sau moarte) și simulează evoluția acesteia de-a lungul a `k` generații, aplicând reguli de supraviețuire și reproducere.

Pe lângă afișarea matricei la fiecare pas, programul poate construi un arbore de evoluții alternative, analiza lanțuri Hamiltoniene și reconstrui generația inițială.

Task 1 – Afișarea evoluției matricei

    Afișează matricea inițială și următoarele k generații, aplicând regulile clasice Game of Life:

       - O celulă vie cu 2 sau 3 vecini supraviețuiește.

       - O celulă moartă cu exact 3 vecini devine vie.

       - Altfel, celula moare sau rămâne moartă.

Task 2 – Diferențe între generații

    - Afișează, pentru fiecare generație, pozițiile celulelor care își schimbă starea.

    - Se folosește o listă ordonată pentru a salva aceste diferențe.

Bonus - Se aplica operatia de inversare a generatiilor,revine la starea initiala

Task 3 – Arbore binar de evoluții

    Construiește un arbore binar de generații în care:

       - În ramura stângă se aplică o regulă alternativă (celule moarte cu 2 vecini devin vii).

       - În ramura dreaptă se aplică regula clasică Game of Life.

    Parcurgerea preordine (preorder) salvează toate configurațiile posibile într-un fișier.

Task 4 – Lanțuri Hamiltoniene

    Reprezintă matricea ca graf (nod = celulă vie, muchie = vecinătate).

    Caută cel mai lung lanț Hamiltonian în fiecare generație din arbore.

    Afișează coordonatele nodurilor din cel mai lung drum găsit.

## Functii folosite

citire() - Citește inputul inițial și alocă matricea.

numar_vecini() - Calculează numărul de vecini vii ai unei celule.

generatii() - Aplică regula clasică Game of Life.

afisare() - Scrie matricea în fișier pentru fiecare generație.

copie_matrice() - Copiază o matrice în alta.

inserare_nod() - Adaugă o celulă într-o listă ordonată de diferențe.

task2() - Afișează celulele care își schimbă starea.

copie_matrice_noua() - Creează o copie nouă a unei matrice.

generatii_B() - Aplică o regulă alternativă de evoluție.

construire() - Creează arborele binar de generații.

preorder() - Parcurge arborele și scrie matricea în fișier.

task3() - Execută Task 3 complet.

construieste_graf() - Creează graf de celule vii și vecini.

dfs() - Caută recursiv cel mai lung drum (lanț Hamiltonian).

gaseste_lant_hamiltonian() - Caută lanțuri în fiecare generație.

hamilton_preorder() - Parcurge arborele și apelează Hamiltonianul.

task4() - Execută Task 4 complet.

bonus() - Inversează modificările dintr-o stivă.

scriu_bonus() - Scrie matricea restaurată într-un fișier.


## Cum se folosește

### Ce trebuie instalat
- Compilator C (ex: GCC)
- Terminal (Linux/macOS/WSL pe Windows)

### Compilare

gcc game_of_life.c -o game_of_life

