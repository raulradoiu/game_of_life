#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 625  // 25*25
#define VIE 'X'
#define MOARTA '+'

void citire(FILE *fisier1, FILE *fisier2, char ***matrice, int *nr_task, int *n, int *m, int *k) {
    if(fisier1 == NULL) {
        printf("Fisierul nu s-a deschis");
        exit(1);
    }
    fscanf(fisier1, "%d %d %d %d", nr_task, n, m, k);

    *matrice = (char **)malloc(*n * sizeof(char *));//vector de pointeri pt liniile matricei
    if(*matrice == NULL) {
        printf("Memorie insuficienta\n");
        exit(1);
    }

    for(int i=0; i<*n; i++) {
        (*matrice)[i] = (char *)malloc(*m * sizeof(char)); //aloca coloanele
        if((*matrice)[i] == NULL) {
            printf("Memorie insuficienta\n");
            exit(1);
        }
    }

    for(int i=0; i<*n; i++) {
        for(int j=0; j<*m; j++) {
            fscanf(fisier1, " %c", &((*matrice)[i][j])); 
        }
    }
    //afisare matrice initiala pt task 1
    if (*nr_task ==1) {
        for(int i=0; i<*n; i++) {
            for(int j=0; j< *m; j++) {
                fprintf(fisier2, "%c", (*matrice)[i][j]);
            }
            fprintf(fisier2, "\n");
        }
        fprintf(fisier2, "\n");
    }
}
//functie care returneaza nr de vecini vii
int numar_vecini(int i, int j, int n, int m, char **game) {
    int x,y,nr = 0;
    int dx[]={-1, -1, -1, 0, 0, 1, 1, 1};
    int dy[]={-1, 0, 1, -1, 1, -1, 0, 1};

    for(int d = 0; d < 8; d++) {
        x = i + dx[d];
        y = j + dy[d];
        if(x>=0 && x<n && y>=0 && y<m && game[x][y] == VIE) {//daca vecinul este in interiorul grilei si e viu
            nr++;
        }
    }
    return nr;
}
//functia cu regulile jocului
void generatii(int n, int m, char **matrice) {
    char **mat1 =(char **)malloc(n * sizeof(char *));//matrice pt rezultatul generatiei urm
    //verifica alocarea    
    if(mat1 == NULL) {
        printf("Memorie insuficienta pentru noua matrice\n");
        exit(1);
    }

    for(int i=0; i<n; i++) {
        mat1[i] =(char *)malloc(m * sizeof(char));//aloca memorie fiecarei linii
        if(mat1[i] == NULL) {
            printf("Memorie insuficienta pentru randul %d al noii matrice\n", i);
            exit(1);
        }
    }

    for(int i=0; i<n; i++) {
        for(int j=0; j<m; j++) {
            int nr_vecini=numar_vecini(i, j, n, m, matrice);
            
            //aplicam regulile 
            if(matrice[i][j]==VIE) {
                if(nr_vecini<2 || nr_vecini>3) {
                    mat1[i][j]=MOARTA;
                } else {
                    mat1[i][j]=VIE;
                }
            } else {
                if(nr_vecini==3) {
                    mat1[i][j]=VIE;
                } else {
                    mat1[i][j]=MOARTA;
                }
            }
        }
    }
    //copiaza rezultatul
    for(int i=0; i<n; i++) {
        for(int j=0; j<m; j++) {
            matrice[i][j] = mat1[i][j];
        }
    }
    //elibereaza memoria
    for(int i=0; i<n; i++) {
        free(mat1[i]);
    }
    free(mat1);
}

void afisare(FILE *fisier1, char **matrice, int n, int m, int k) {
    if(fisier1 == NULL) {
        printf("Fisierul nu s-a deschis");
        exit(1);
    }

    for(int nr_gen = 0; nr_gen <= k; nr_gen++) {
        if(nr_gen > 0) {//sarim peste gen 0
            for(int i=0; i<n; i++) {
                for(int j=0; j<m; j++) {
                    fprintf(fisier1, "%c", matrice[i][j]);
                }
                fprintf(fisier1, "\n");
            }
            fprintf(fisier1, "\n");
        }
        generatii(n, m, matrice);//aplica o generatie noua
    }
}
//task 2
typedef struct Nod { //lista cu celulele care si au schimbat starea
    int linie;
    int coloana;
    struct Nod *urm;
} Nod;

//insereasa un nod intr o lista 
void inserare_nod(Nod **lista, int linie, int coloana) {
    Nod *cel =  (Nod *)malloc(sizeof (Nod));
    cel->linie = linie;
    cel->coloana = coloana; //seteaza valorile nodului
    cel->urm = NULL;
    
    //daca e goala nodul devine capul listei
    if(*lista == NULL) {
        *lista = cel;
        return;
    }
    //daca noul nod trb pus la inceput, il insereaza
    Nod *p = *lista;
    if (linie < p->linie || (linie == p->linie && coloana < p->coloana)) {
        cel->urm = *lista;
        *lista = cel;
        return;
    }
    //parcurge lista pana la pozitia unde trebuie inserat
    while(p->urm != NULL && (p->urm->linie < linie || 
          (p->urm->linie == linie && p->urm->coloana < coloana))) {
        p = p->urm;
    }
    //insereaza nodul
    cel->urm = p->urm;
    p->urm = cel;
}

// creeaza o stiva de liste care retine celulele modificate dupa fiecare gen
void task2(FILE *fisier2, char **matrice, int n, int m, int k) {
    //matrice pentru comparatie
    char **mat_veche = (char **)malloc(n * sizeof(char *));
    for (int i=0; i<n; i++) {
        mat_veche[i] = (char *)malloc(m * sizeof(char));
    }
    //initializeaza copia cu valorile curente
    for(int i=0; i<n; i++)
        for(int j=0; j<m; j++) 
            mat_veche[i][j] = matrice[i][j];

    //aplica regulile pt fiecare gen
    for (int gen=1; gen<=k; gen++) {
        generatii(n, m, matrice);

        //comp matricea veche cu cea noua si insereaza coordonatele schimbate
        Nod *lista_dif = NULL;
        for (int i=0; i<n; i++) {
            for (int j=0; j<m; j++) {
                if (mat_veche[i][j] != matrice[i][j]) {
                    inserare_nod(&lista_dif, i, j);
                }
            }
        }
        //scrie in fisier gen si lista de coordonate
        fprintf(fisier2, "%d", gen);
        Nod *p = lista_dif;
        while (p != NULL) {
            fprintf(fisier2, " %d %d", p->linie, p->coloana);
            p = p->urm;
        }
        fprintf(fisier2,"\n");
        
        //actualizam matricea veche cu cea noua
        for(int i = 0; i < n; i++)
            for(int j = 0; j < m; j++)
                mat_veche[i][j] = matrice[i][j];
        //elibereaza lista de noduri
        while(lista_dif != NULL) {
            Nod *tmp = lista_dif;
            lista_dif = lista_dif->urm;
            free(tmp);
        }
    }

    for (int i=0; i<n; i++)
        free(mat_veche[i]);
    free(mat_veche);
}

//bonus
typedef struct Celula {
    int l, c;
    struct Celula *next;
} Celula;

typedef struct Nod {
    Celula *celule;
    struct Nod *next;
} Nod;

// Functie pentru scrierea matricei in fisier
void scriu_bonus(FILE *f, int n, int m, char **matrice) {
    for (int i=0; i<n; i++) {
        for (int j=0; j<m; j++)
            fputc(matrice[i][j], f);
        fputc('\n', f);
    }
}

// Functie bonus care inverseaza modificarile din stiva
void bonus(Nod *top, int n, int m, char **matrice) {
    while (top) {
        Celula *cel = top->celule;
        while (cel) {
            int i = cel->l, j = cel->c;
            if (i >= 0 && i < n && j >= 0 && j < m) {
                if (matrice[i][j] == VIE)  // VIE
                    matrice[i][j] = MOARTA;  // devine MOARTA
                else
                    matrice[i][j] = VIE;  // altfel devine VIE
            }
            cel = cel->next;
        }
        top = top->next;
    }
}


// task 3
//arbore binar al generatiilor
typedef struct Arbore {
    char **matrice;//starea grilei la acel moment
    int generatie;
    struct Arbore *left, *right;
} Arbore;

//returneaza copia unei matrici
char **copie_matrice_noua(int n, int m, char **src) {
    char **dest = (char **)malloc(n * sizeof(char *));
    for(int i=0; i<n; i++) {
        dest[i] = (char *)malloc(m * sizeof(char));
        memcpy(dest[i], src[i], m);
    }
    return dest;
}

//aplica regula B
char **generatii_B(int n, int m, char **matrice) {
    char **noua = copie_matrice_noua(n, m, matrice);//copie a matricii
    for(int i=0; i<n; i++) {
        for(int j=0; j<m; j++) {
            int vecini = numar_vecini(i, j, n, m, matrice);
            if(matrice[i][j] == MOARTA && vecini==2)
                noua[i][j] = VIE;
        }
    }
    return noua;
}
//reurneaza nodul construit
Arbore *construire(int n, int m, int gen_curenta, int k, char **matrice) {
    if(gen_curenta > k) return NULL;

    Arbore *nod = malloc(sizeof(Arbore));//nod nou in arbore
    nod->generatie = gen_curenta;
    nod->matrice = copie_matrice_noua(n, m, matrice);//salveaza copia matriceai curente

    char **mat1 = generatii_B(n, m, matrice);//creeza o mat cu regulile B
    char **mat2 = copie_matrice_noua(n, m, matrice);//se aplica regulile clasice
    generatii(n, m, mat2);

    //construieste arborele recursiv
    nod->left = construire(n, m, gen_curenta + 1, k, mat1);
    nod->right = construire(n, m, gen_curenta + 1, k, mat2);

    for (int i=0; i<n; i++) free(mat1[i]);
    free(mat1);
    for (int i=0; i<n; i++) free(mat2[i]);
    free(mat2);

    return nod;
}
//scrie mat in fisiere
void scriere_mat(FILE *fisier, char **matrice, int n, int m) {
    for (int i=0; i<n; i++) {
        for (int j=0; j<m; j++) {
            fprintf(fisier, "%c", matrice[i][j]);
        }
        fprintf(fisier, "\n");
    }
}

void preorder(Arbore* root, FILE *fisier, int n, int m) {
    if (root) { //nodul curent
        scriere_mat(fisier, root->matrice, n, m);//afiseaza gen curenta
        fprintf(fisier, "\n");
        preorder(root->left, fisier, n, m);//apeleaza recursiv parcurgerea
        preorder(root->right, fisier, n, m);
    }
}

//construieste si afiseaza arborele
void task3(FILE *fisier, char **matrice, int n, int m, int k) {
    Arbore *arb = construire(n, m, 0, k, matrice);
    preorder(arb, fisier, n, m);
}

//TASK 4 
//structura pt celule vii
typedef struct {
    int linie, coloana;
} Celula;

//vectorii de directie
int dx[]={-1,-1,-1,0,0,1,1,1};
int dy[]={-1,0,1,-1,1,-1,0,1};

// variabile globale 
Celula celule[MAX];//vector cu celule vii
int adiacenta[MAX][MAX];//1 daca celula i este vecina cu j
int nr_celule;//nr celule vii

int vizitat[MAX];//daca am trecut prin nodul i in DFS
Celula drum[MAX];//drum curent
Celula best_drum[MAX];//cel mai lung drum gasit pana acum
int lungime_best;//lungimea lui

//cauta o celula in vectorul celule si returneza pozitia sa(indexul din graf)
int pozitie(int l, int c) {
    for (int i=0; i<nr_celule; i++) {
        if (celule[i].linie == l && celule[i].coloana == c) return i;
    }
    return -1;
}

void construieste_graf(char **matrice, int n, int m) {
    nr_celule=0;
    //parcurge matricea si adauga in vecorul celule toate celulele vii
    for (int i=0; i<n; i++) {
        for (int j=0; j<m; j++) {
            if (matrice[i][j]==VIE) {
                celule[nr_celule].linie=i;
                celule[nr_celule].coloana=j;
                nr_celule++;
            }
        }
    }
    //contruieste mat de adiacenta
    for (int i=0; i<nr_celule; i++) {
        for (int d=0; d<8; d++) {
            int ni=celule[i].linie + dx[d];
            int nj=celule[i].coloana + dy[d];
            int p=pozitie(ni, nj);//indicele celulei vii de la pozitia ni,nj
            if (p !=-1) {//daca la pozitia vecina exista o celula vie
                adiacenta[i][p] = 1;//creeaza muchia grafului
            }
        }
    }
}
//cautare in adancime
void dfs(int nod, int pas) { //nod-celula curenta  pas-lungimea drumului pana acum
    vizitat[nod] = 1;//marcheaza nodul ca vizitat
    drum[pas] = celule[nod];//il adauga in drumul curent

    //daca drumul actual e mai lung decat cel gasit anterior,il salveaza
    if (pas + 1 > lungime_best) {
        lungime_best = pas + 1;
        memcpy(best_drum, drum, sizeof(Celula) * (pas + 1));
    }

    for (int i = 0; i < nr_celule; i++) {
        if (adiacenta[nod][i]==1 && vizitat[i]!=1) {//daca e vecin real si nu a mai fost vizitat
            dfs(i, pas + 1);//continua recursiv drumul
        }
    }
    vizitat[nod] = 0;//deblocam celula
}

//cauta cel mai lung lant de celule vii conectate intre ele
void gaseste_lant_hamiltonian(FILE *fisier, char **matrice, int n, int m) {
    lungime_best=-1;
    //rseteaza grafurile si vectorii
    for (int i=0; i<MAX; i++) {
        for (int j=0; j<MAX; j++) adiacenta[i][j] = 0;
        vizitat[i]=0;
    }
    //creeaza graful din matricea data
    construieste_graf(matrice, n, m);
    
    ///aplica dfs pt fiecare nod
    for(int i=0; i< nr_celule; i++) {
        dfs(i, 0);
    }
    //daca nu are cel putin 2 pasi
    if(lungime_best< 2) {
        fprintf(fisier, "-1\n");
        return;
    }
    
    //scrie lungimea drumului si toate celulele din lant
    fprintf(fisier, "%d\n", lungime_best - 1);
    for (int i=0; i< lungime_best; i++) {
        fprintf(fisier, "(%d,%d) ", best_drum[i].linie, best_drum[i].coloana);
    }
    fprintf(fisier, "\n");
}

//parcurge arborele in preordine
void hamilton_preorder(Arbore *node,FILE *fisier,int n,int m) {

        if(!node) return;//ver daca nodul curent exista
        gaseste_lant_hamiltonian(fisier, node->matrice, n, m); //construieste graful pe baza matricei din nod si cauta cel mai lung lant de celule vii vecine(drum ham)
        hamilton_preorder(node->left,fisier,n,m); //continua toata ramura stanga
        hamilton_preorder(node->right,fisier,n,m); //apoi dreapta
    }

//constrieste arborele de generatii si scrie lantul hamiltonian
void task4(FILE *fisier, char **matrice, int n, int m, int k) {
    
    Arbore *arb =construire(n, m, 0, k, matrice);
    hamilton_preorder(arb,fisier,n,m);
} 
  
int main(int argc, const char* argv[]) {
    for (int i=1; i < argc; i++) {
        FILE *input_file = fopen(argv[i], "r");

        char OutputFile[30];
        strcpy(OutputFile + 1, argv[i]);
        OutputFile[0] = 'O';
        OutputFile[1] = 'u';
        OutputFile[2] = 't';
        int len = strlen(OutputFile);
        OutputFile[len - 2] = 'o';
        OutputFile[len - 1] = 'u';
        OutputFile[len + 0] = 't';
        OutputFile[len + 1] = '\0';

        FILE *output_file = fopen(OutputFile, "w");

        if(input_file == NULL || output_file == NULL) {
            printf("Fisierul nu s-a deschis\n");

            if(input_file != NULL) fclose(input_file);
            if(output_file != NULL) fclose(output_file);

            return 1;
        }

        int n, m, nr_task, k;
        char **matrice;
        citire(input_file, output_file, &matrice, &nr_task, &n, &m, &k);

        if(nr_task==1)
            afisare(output_file, matrice, n, m, k);
        else if(nr_task==2)
            task2(output_file, matrice, n, m, k);
        else if(nr_task==3)
            task3(output_file, matrice, n, m, k);
        else if(nr_task==4)
            task4(output_file, matrice, n, m, k);

        fclose(input_file);
        fclose(output_file);

        for(int i=0; i<n; i++) {
            free(matrice[i]);
        }
        free(matrice);
    }

    return 0;
}

