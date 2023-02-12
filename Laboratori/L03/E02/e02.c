#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLEN 256
#define FILENAME "brani.txt"

int loadFile(FILE *f, int **ps, char ***pp, char ****pd);
int rec(int liv, int cnt, int n, int *nSongs, char **playlist, char ***songs);

int main(int argc, char const *argv[]) {
    int n;                   // numero di amici
    int cnt;                 // numero di soluzioni (possibili playlist)
    int *nSongs = NULL;      // lista con numero di canzoni per ciascun amico
    char **playlist = NULL;  // lista di canzoni
    char ***songs = NULL;    // lista di liste di canzoni

    FILE *f = fopen(FILENAME, "r");
    n = loadFile(f, &nSongs, &playlist, &songs);

    cnt = rec(0, 0, n, nSongs, playlist, songs);
    printf("%d playlist possibili\n", cnt);

    // dealloca
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < nSongs[i]; j++)
            free(songs[i][j]);
        free(songs[i]);
        free(playlist[i]);
    }
    free(songs);
    free(playlist);
    free(nSongs);

    return 0;
}

int rec(int liv, int cnt, int n, int *nSongs, char **playlist, char ***songs) {
    // terminazione
    if (liv >= n) {
        for (int i = 0; i < n; i++)
            printf("%s ", playlist[i]);
        printf("\n", cnt + 1);
        return cnt + 1;  // aggiungo una soluzione
    }

    for (int i = 0; i < nSongs[liv]; i++) {
        // aggiungo la canzone corrente alla playlist
        strcpy(playlist[liv], songs[liv][i]);
        // chiamata ricorsiva scendendo di un livello (prossimo amico)
        cnt = rec(liv + 1, cnt, n, nSongs, playlist, songs);
    }
    // sono finite tutte le canzoni a questo livello (amico)
    // ritorno al chiamante il numero di soluzioni di questo ramo
    return cnt;
}

int loadFile(FILE *f, int **ps, char ***pp, char ****pd) {
    int n, lines, *nSongs;
    char s[MAXLEN], **playlist, ***songs;

    fscanf(f, "%d", &n);

    playlist = (char **)malloc(n * sizeof(char *));
    for (int i = 0; i < n; i++)
        playlist[i] = (char *)malloc(MAXLEN * sizeof(char));

    songs = (char ***)malloc(n * sizeof(char **));
    nSongs = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        fscanf(f, "%d", &lines);
        nSongs[i] = lines;
        songs[i] = (char **)malloc(lines * sizeof(char *));

        for (int j = 0; j < lines; j++) {
            fscanf(f, "%s", s);
            // allocazione dinamica in base alla lunghezza di ogni canzone
            songs[i][j] = (char *)malloc((strlen(s) + 1) * sizeof(char));
            strcpy(songs[i][j], s);
        }
    }
    fclose(f);

    *ps = nSongs;
    *pp = playlist;
    *pd = songs;
    return n;
}