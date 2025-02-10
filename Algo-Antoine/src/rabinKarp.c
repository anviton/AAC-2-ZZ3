#include <stdio.h>
#include <string.h>

#include "rabinKarp.h"

#define PRIME 101  // Nombre premier utilisé pour le hachage
#define MAX 500   // Taille maximale des chaînes

/**
 * Fonction pour calculer le hash d'une chaîne de caractères
 * @param str la chaîne de caractères
 * @param length la longueur de la chaîne
 * @return le hash de la chaîne
 */
long long calculateHash(char *str, int length) {
    long long hash = 0;
    for (int i = 0; i < length; i++) {
        hash = (hash * 256 + str[i]) % PRIME;
    }
    return hash;
}

/**
 * Fonction pour trouver un motif dans une chaîne de caractères
 * @param text la chaîne de caractères
 * @param pattern motif à trouver
 * @param motifIndex tableau pour stocker les index des motifs trouvés
 * @return le nombre de motifs trouvés
 */
int rabinKarp(char *text, char *pattern, int motifIndex[]) {
    int textLen = strlen(text);
    int patternLen = strlen(pattern);
    long long patternHash = calculateHash(pattern, patternLen);
    long long textHash = calculateHash(text, patternLen);
    long long h = 1;
    int nbMotifFound = 0;
    for (int i = 0; i < patternLen - 1; i++) {
        h = (h * 256) % PRIME;
    }

    // Recherche du motif
    for (int i = 0; i <= textLen - patternLen; i++) {
        if (patternHash == textHash) {
            // Vérification caractère par caractère pour éviter les collisions de hash
            if (strncmp(text + i, pattern, patternLen) == 0) {
                printf("Motif trouvé à l'index %d\n", i);
                motifIndex[nbMotifFound] = i;
                nbMotifFound++;
            }
        }
        
        if (i < textLen - patternLen) {
            textHash = (256 * (textHash - text[i] * h) + text[i + patternLen]) % PRIME;
            // Correction pour éviter les valeurs négatives
            if (textHash < 0) {
                textHash += PRIME;
            }
        }
    }

    return nbMotifFound;
}


void test1() {
    char *text = "ABCDDAEFGABCDAZEOPKJHHYABCDAAACREDMOPNBABBACD";
    char *pattern = "ABCD";

    int nbMotifFound, motifIndex[MAX];
    
    nbMotifFound = rabinKarp(text, pattern, motifIndex);

    printf("Index des motifs trouvés: ");
    for (int i = 0; i < nbMotifFound; i++) {
        printf("%d ", motifIndex[i]);
    }
    printf("\n");

}

void test2() {
    char *text = "En informatique, plus précisément en algorithmique du texte, l algorithme de Rabin-Karp ou algorithme de Karp-Rabin est un algorithme de recherche de sous-chaîne créé par Richard M. Karp et Michael O. Rabin (1987). Cette méthode recherche la présence d un motif dans un texte, ou plus généralement la présence d un motif parmi un ensemble de motifs donnés (c est-à-dire des sous-chaînes) dans un texte. L algorithme de Rabin-Karp améliore l algorithme naïf en utilisant une fonction de hachage. L algorithme n est pas beaucoup employé pour les recherches d une unique sous-chaîne mais a une importance théorique et s avère très efficace pour des recherches de multiples sous-chaînes.";
    char *pattern = "algo";

    int nbMotifFound, motifIndex[MAX];
    
    nbMotifFound = rabinKarp(text, pattern, motifIndex);

    printf("Index des motifs trouvés: ");
    for (int i = 0; i < nbMotifFound; i++) {
        printf("%d ", motifIndex[i]);
    }
    printf("\n");

}
