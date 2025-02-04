#include <iostream>
#include <vector>

// Structure pour représenter un nœud dans la matrice creuse
struct Node {
    Node *left, *right, *up, *down;
    Node *col_header; // Pointeur vers l'en-tête de la colonne
    int row; // Numéro de la ligne (pour identifier la solution)
    int size; // Taille de la colonne (utilisé uniquement pour les en-têtes de colonne)

    Node() : left(this), right(this), up(this), down(this), col_header(nullptr), row(-1), size(0) {}
};

// Fonction pour couvrir une colonne
void cover(Node *col) {
    col->right->left = col->left;
    col->left->right = col->right;

    for (Node *row = col->down; row != col; row = row->down) {
        for (Node *node = row->right; node != row; node = node->right) {
            node->down->up = node->up;
            node->up->down = node->down;
            node->col_header->size--;
        }
    }
}

// Fonction pour découvrir une colonne
void uncover(Node *col) {
    for (Node *row = col->up; row != col; row = row->up) {
        for (Node *node = row->left; node != row; node = node->left) {
            node->col_header->size++;
            node->down->up = node;
            node->up->down = node;
        }
    }

    col->right->left = col;
    col->left->right = col;
}

// Fonction principale pour résoudre le problème de la couverture exacte
bool solve(Node *header, std::vector<int> &solution) {
    if (header->right == header) {
        return true; // Toutes les colonnes ont été couvertes
    }

    // Choisir la colonne avec le moins de nœuds (heuristique)
    Node *col = header->right;
    for (Node *node = header->right; node != header; node = node->right) {
        if (node->size < col->size) {
            col = node;
        }
    }

    cover(col);

    for (Node *row = col->down; row != col; row = row->down) {
        solution.push_back(row->row);

        for (Node *node = row->right; node != row; node = node->right) {
            cover(node->col_header);
        }

        if (solve(header, solution)) {
            return true;
        }

        // Backtrack
        solution.pop_back();
        for (Node *node = row->left; node != row; node = node->left) {
            uncover(node->col_header);
        }
    }

    uncover(col);
    return false;
}

// Fonction pour créer la structure de Dancing Links à partir d'une matrice binaire
Node* create_dancing_links(const std::vector<std::vector<int>> &matrix) {
    Node *header = new Node(); // En-tête principal
    std::vector<Node*> column_headers(matrix[0].size(), nullptr);

    // Créer les en-têtes de colonnes
    for (int i = 0; i < matrix[0].size(); ++i) {
        column_headers[i] = new Node();
        column_headers[i]->right = header;
        column_headers[i]->left = header->left;
        header->left->right = column_headers[i];
        header->left = column_headers[i];
        column_headers[i]->col_header = column_headers[i];
    }

    // Ajouter les nœuds pour chaque ligne
    for (int i = 0; i < matrix.size(); ++i) {
        Node *row_header = nullptr;
        Node *prev_node = nullptr;

        for (int j = 0; j < matrix[i].size(); ++j) {
            if (matrix[i][j] == 1) {
                Node *node = new Node();
                node->row = i;
                node->col_header = column_headers[j];

                // Lier le nœud à la colonne
                node->up = column_headers[j]->up;
                node->down = column_headers[j];
                column_headers[j]->up->down = node;
                column_headers[j]->up = node;
                column_headers[j]->size++;

                // Lier le nœud à la ligne
                if (row_header == nullptr) {
                    row_header = node;
                    prev_node = node;
                } else {
                    node->left = prev_node;
                    node->right = row_header;
                    prev_node->right = node;
                    row_header->left = node;
                    prev_node = node;
                }
            }
        }
    }

    return header;
}

// Fonction principale
int main() {
    // Exemple de matrice binaire
    std::vector<std::vector<int>> matrix = {
        {1, 0, 0, 1, 0, 0, 1},
        {1, 0, 0, 1, 0, 0, 0},
        {0, 0, 0, 1, 1, 0, 1},
        {0, 0, 1, 0, 1, 1, 0},
        {0, 1, 1, 0, 0, 1, 1},
        {0, 1, 0, 0, 0, 0, 1}
    };

    // Créer la structure de Dancing Links
    Node *header = create_dancing_links(matrix);

    // Résoudre le problème
    std::vector<int> solution;
    if (solve(header, solution)) {
        std::cout << "Solution trouvée: ";
        for (int row : solution) {
            std::cout << row << " ";
        }
        std::cout << std::endl;
    } else {
        std::cout << "Pas de solution trouvée." << std::endl;
    }

    return 0;
}