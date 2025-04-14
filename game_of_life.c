#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VIE 'X'
#define MOARTA '+'

void citire(FILE *fisier1, FILE *fisier2, char ***matrice, int *nr_task, int *n, int *m, int *k) {
    if(fisier1 == NULL) {
        printf("Fisierul nu s-a deschis");
        exit(1);
    }
    fscanf(fisier1, "%d %d %d %d", nr_task, n, m, k);

    *matrice = (char **)malloc(*n * sizeof(char *));
    if(*matrice == NULL) {
        printf("Memorie insuficienta\n");
        exit(1);
    }

    for(int i=0; i<*n; i++) {
        (*matrice)[i] = (char *)malloc(*m * sizeof(char));
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

int numar_vecini(int i, int j, int n, int m, char **game) {
    int x,y,nr = 0;
    int dx[]={-1, -1, -1, 0, 0, 1, 1, 1};
    int dy[]={-1, 0, 1, -1, 1, -1, 0, 1};
    
    for(int d = 0; d < 8; d++) {
        x = i + dx[d];
        y = j + dy[d];
        if(x>=0 && x<n && y>=0 && y<m && game[x][y] == VIE) {
            nr++;
        }
    }
    return nr;
}

void generatii(int n, int m, char **matrice) {
    char **mat1 =(char **)malloc(n * sizeof(char *));
    if(mat1 == NULL) {
        printf("Memorie insuficienta pentru noua matrice!\n");
        exit(1);
    }

    for(int i=0; i<n; i++) {
        mat1[i] =(char *)malloc(m * sizeof(char));
        if(mat1[i] == NULL) {
            printf("Memorie insuficienta pentru randul %d al noii matrice!\n", i);
            exit(1);
        }
    }

    for(int i=0; i<n; i++) {
        for(int j=0; j<m; j++) {
            int nr_vecini = numar_vecini(i, j, n, m, matrice);
            
            if(matrice[i][j]==VIE) {
                if(nr_vecini<2 || nr_vecini>3) {
                    mat1[i][j] = MOARTA;
                } else {
                    mat1[i][j] = VIE;
                }
            } else {
                if(nr_vecini==3) {
                    mat1[i][j] = VIE;
                } else {
                    mat1[i][j] = MOARTA;
                }
            }
        }
    }

    for(int i=0; i<n; i++) {
        for(int j=0; j<m; j++) {
            matrice[i][j] = mat1[i][j];
        }
    }

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
        if(nr_gen > 0) {
            for(int i=0; i<n; i++) {
                for(int j=0; j<m; j++) {
                    fprintf(fisier1, "%c", matrice[i][j]);
                }
                fprintf(fisier1, "\n");
            }
            fprintf(fisier1, "\n");
        }
        generatii(n, m, matrice);
    }
}

// Task 2
typedef struct Nod {
    int linie;
    int coloana;
    struct Nod *urm;
} Nod;


void inserare_nod(Nod **lista, int linie, int coloana) {
 Nod *cel =  (Nod *)malloc(sizeof (Nod));
    cel->linie = linie;
    cel->coloana = coloana;
    cel->urm = NULL;

    if (*lista == NULL) {
        *lista = cel;
        return;
    }

 Nod *p = *lista;

    // Dacă trebuie inserat la început (ordonare simplificată)
    if (linie < p->linie || (linie == p->linie && coloana < p->coloana)) {
        cel->urm = *lista;
        *lista = cel;
        return;
    }

    // Caut locul de inserare
    while (p->urm != NULL && (p->urm->linie < linie || 
          (p->urm->linie == linie && p->urm->coloana < coloana))) {
        p = p->urm;
    }

    cel->urm = p->urm;
    p->urm = cel;
}

void copie_matrice(char **destinatie, char **sursa, int n, int m) {
    for(int i=0; i<n; i++)
        for(int j=0; j<m; j++)
            destinatie[i][j]=sursa[i][j];
}


void task2(FILE *fisier2, char **matrice, int n, int m, int k) {
    char **mat_veche = (char **)malloc(n * sizeof(char *));
    for (int i=0; i<n; i++) {
        mat_veche[i] = (char *)malloc(m * sizeof(char));
    }

    for (int i=0; i<n; i++)
        for (int j=0; j<m; j++)
            mat_veche[i][j] = matrice[i][j];

    for (int gen=1; gen<=k; gen++) {
        generatii(n, m, matrice);

     Nod *lista_dif = NULL;

        for (int i=0; i<n; i++) {
            for (int j=0; j<m; j++) {
                if (mat_veche[i][j] != matrice[i][j]) {
                    inserare_nod(&lista_dif, i, j);
                }
            }
        }

        fprintf(fisier2, "%d", gen);
     Nod *p = lista_dif;
        while (p != NULL) {
            fprintf(fisier2, " %d %d", p->linie, p->coloana);
            p = p->urm;
        }
        fprintf(fisier2,"\n");

    
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
                mat_veche[i][j] = matrice[i][j];

        while (lista_dif != NULL) {
         Nod *tmp = lista_dif;
            lista_dif = lista_dif->urm;
            free(tmp);
        }
    }

    for (int i=0; i<n; i++)
        free(mat_veche[i]);
    free(mat_veche);
}

int main(int argc, const char* argv[]) {
    for (int i = 1; i < argc; i++) {
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

        if(nr_task == 1)
            afisare(output_file, matrice, n, m, k);
        else if(nr_task == 2)
            task2(output_file, matrice, n, m, k);

        fclose(input_file);
        fclose(output_file);

        for(int i = 0; i < n; i++) {
            free(matrice[i]);
        }
        free(matrice);
    }

    return 0;
}

