#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    char type[50];
    char date[50];
    char adresse[100];
    int estOccupe;
    float loyer;
} Propriete;

typedef struct {
    Propriete* proprietes;
    int taille;
    int capacite;
} Portefeuille;
typedef struct Payment {
    int propertyId;
    float amount;
    char date[50];
    struct Payment* next;
} Payment;
typedef struct {
    Payment* front;
    Payment* rear;
} PaymentQueue;
Portefeuille initialiserPortefeuille(int capacite) {
    Portefeuille p;
    p.proprietes = (Propriete*)malloc(sizeof(Propriete) * capacite);
    p.taille = 0;
    p.capacite = capacite;
    return p;
}
void menuAjouterPropriete(Portefeuille* portefeuille) {
    int id, estOccupe;
    char type[50];
    char adresse[100];
    char date[50] = "--/--/----";
    float loyer;
    printf("Entrer l'ID de la propriete : ");
    scanf("%d", &id);
    getchar();
    printf("Entrer le type de propriete (Appartement, Maison, Bureau, etc.) : ");
    fgets(type, sizeof(type), stdin);
    type[strcspn(type, "\n")] = '\0';
    printf("Entrer l'adresse de la propriete : ");
    fgets(adresse, sizeof(adresse), stdin);
    adresse[strcspn(adresse, "\n")] = '\0';
    printf("Statut (0 = Disponible, 1 = Occupé) : ");
    scanf("%d", &estOccupe);
    getchar();
    if (estOccupe == 1) {
        printf("Entrer la date d'occupation (format JJ/MM/AAAA) : ");
        fgets(date, sizeof(date), stdin);
        date[strcspn(date, "\n")] = '\0';
    }
    printf("Entrer le montant du loyer : ");
    scanf("%f", &loyer);

    ajouterPropriete(portefeuille, id, type, adresse, estOccupe, date, loyer);
}
void ajouterPropriete(Portefeuille* p, int id, const char* type, const char* adresse, int estOccupe, const char* date, float loyer) {
    if (p->taille >= p->capacite) {
        printf("Capacit maximale atteinte. Impossible d'ajouter plus de propriete.\n");
        return;
    }
    Propriete prop;
    prop.id = id;
    strcpy(prop.type, type);
    strcpy(prop.adresse, adresse);
    prop.estOccupe = estOccupe;
    strcpy(prop.date, date);
    prop.loyer = loyer;
    p->proprietes[p->taille++] = prop;
    printf("Propriter ajouter avec sacsess\n");
}

void afficherProprietes(const Portefeuille* p) {
    if (p->taille == 0) {
        printf("Aucune propriete dans le portefeuille.\n");
        return;
    }
    printf("Liste des propriete :\n");
    for (int i = 0; i < p->taille; i++) {
        Propriete prop = p->proprietes[i];
        printf("ID: %d | Type: %s | Adresse: %s | Statut: %s | Date: %s | Loyer: %.2f\n",
               prop.id, prop.type, prop.adresse,
               prop.estOccupe ? "Occuppe" : "Disponible", prop.date,
               prop.loyer);
    }
}

void libererPortefeuille(Portefeuille* p) {
    free(p->proprietes);
    p->proprietes = NULL;
    p->taille = 0;
    p->capacite = 0;
}
PaymentQueue* createQueue() {
    PaymentQueue* queue = (PaymentQueue*)malloc(sizeof(PaymentQueue));
    queue->front = NULL;
    queue->rear = NULL;
    return queue;
}
void enqueuePayment(PaymentQueue* queue, int propertyId, float amount, const char* date) {
    Payment* newPayment = (Payment*)malloc(sizeof(Payment));
    newPayment->propertyId = propertyId;
    newPayment->amount = amount;
    strcpy(newPayment->date, date);
    newPayment->next = NULL;
    if (queue->rear == NULL) {
        queue->front = queue->rear = newPayment;
    } else {
        queue->rear->next = newPayment;
        queue->rear = newPayment;
    }
    printf("Paiement est ajoute avec succee.\n");
}


void displayPayments(const PaymentQueue* queue) {
    if (queue->front == NULL) {
        printf("Aucun paiement enregistrer.\n");
        return;
    }
    Payment* current = queue->front;
    printf("Liste des paiements :\n");
    while (current != NULL) {
        printf("ProprieteID: %d | Montant: %.2f | Date: %s\n",current->propertyId, current->amount, current->date);
        current = current->next;
    }
}

void freeQueue(PaymentQueue* queue) {
    Payment* current = queue->front;
    while (current != NULL) {
        Payment* temp = current;
        current = current->next;
        free(temp);
    }
    free(queue);
}
void ajouterEntretien(const char* nomFichier, int idMaison, const char* description, const char* date) {
    FILE* fichier = fopen(nomFichier, "a");
    if (fichier == NULL) {
        printf("Erreur : Impossible d'ouvrir le fichier des entretiens.\n");
        return;
    }
    fprintf(fichier, "ID: %d | Description: %s | Date: %s\n", idMaison, description, date);
    fclose(fichier);
    printf("Entretien enregistre avec succes dans le fichier.\n");
}
void afficherEntretiens(const char* nomFichier) {
    FILE* fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        printf("Aucun fichier d'entretiens trouve.\n");
        return;
    }
    char ligne[200];
    printf("\n--- Liste des entretiens necessaires ---\n");
    while (fgets(ligne, sizeof(ligne), fichier)) {
        printf("%s", ligne);
    }
    fclose(fichier);
}
int main() {
    int choix;
    int idMaison;
    char description[100];
    char date[50];
    float montant;
    Portefeuille portefeuille = initialiserPortefeuille(10);
    PaymentQueue* paiementQueue = createQueue();

    do {
        printf("\n--- Menu Gestion de Proprietes ---\n");
        printf("1. Ajouter une propriete\n");
        printf("2. Afficher toutes les proprietes\n");
        printf("3. Ajouter paiements\n");
        printf("4. Afficher tous les paiements\n");
        printf("5. Ajouter un entretien\n");
        printf("6. Afficher les entretiens\n");
        printf("7. Quitter\n");
        printf("Votre choix : ");
        scanf("%d", &choix);
        getchar();
        switch (choix) {
            case 1:
                menuAjouterPropriete(&portefeuille);
                break;
            case 2:
                afficherProprietes(&portefeuille);
                break;
            case 3:
                printf("Entrer l'ID de la propriete : ");
                scanf("%d", &idMaison);
                getchar();

                printf("Entrer le montant du paiement : ");
                scanf("%f", &montant);
                getchar();

                printf("Entrer la date du paiement (JJ/MM/AAAA) : ");
                fgets(date, sizeof(date), stdin);
                date[strcspn(date, "\n")] = '\0';

                enqueuePayment(paiementQueue, idMaison, montant, date);
                break;
            case 4:
                displayPayments(paiementQueue);
                break;
            case 5:
                printf("Entrer l'ID de la maison pour l'entretien : ");
                scanf("%d", &idMaison);
                getchar();

                printf("Entrer la description de l'entretien : ");
                fgets(description, sizeof(description), stdin);
                description[strcspn(description, "\n")] = '\0';

                printf("Entrer la date prevue pour l'entretien (JJ/MM/AAAA) : ");
                fgets(date, sizeof(date), stdin);
                date[strcspn(date, "\n")] = '\0';

                ajouterEntretien("entretiens.txt", idMaison, description, date);
                break;
            case 6:
                afficherEntretiens("entretiens.txt");
                break;
            case 7:
                printf("Fermeture du programme.\n");
                break;
            default:
                printf("Choix invalide, veuillez reessayer.\n");
        }
    } while (choix != 7);

    libererPortefeuille(&portefeuille);
    freeQueue(paiementQueue);
    return 0;
}
