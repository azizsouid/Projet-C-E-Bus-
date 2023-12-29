// TRAVAIL DE MOHAMED AZIZ SOUID II1A
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main()
{

    int commande, continuer=1;
    int voyagerID = 0, resID = 0;
    char sortir;
    printf("Bonjour cher(e) client(e). Soyez le bienvenu(e) sur Aziz E-Bus Platform. \n");
    while (continuer==1){
    printf("Veuillez choisir la commande que vous desirez : \n Pour creer un compte voyageur tapez 1 \n Pour afficher les details des bus tapez 2  \n Pour creer une nouvelle reservation tapez 3 \n Pour afficher et modifier les details d'une reservation tapez 4 \n Pour savoir le prix et le temps du depart du bus taper 5 \n");
    scanf("%d", &commande);

    switch (commande)
    {
    case 1: {
    char nomVoy[10];
    char prenomVoy[10];
    char mdp[15];
    FILE *idFile = fopen("voyager_id.txt", "r");

    if (idFile != NULL) {

        fscanf(idFile, "%d", &voyagerID);
        fclose(idFile);
    }

    printf("Donnez votre nom de famille :\n");
    scanf("%s", nomVoy);
    printf("Donnez votre prenom :\n");
    scanf("%s", prenomVoy);
    printf("Ecrire le mot de passe :\n");
    scanf("%s", mdp);

    FILE *filePointer;
    filePointer = fopen("voyageurs.txt", "a");

    if (filePointer == NULL) {
        printf("Le fichier ne peut pas etre ouvert.\n");
        return 1;
    }
    fprintf(filePointer, "%d %s %s %s\n", voyagerID + 1, nomVoy, prenomVoy, mdp);
    voyagerID++;

    printf("Votre voyID est : %d\n", voyagerID);

    fclose(filePointer);
    idFile = fopen("voyager_id.txt", "w");
    if (idFile != NULL) {
        fprintf(idFile, "%d", voyagerID);
        fclose(idFile);
    }

    break;
}



   case 2: {
    FILE *filePointer;
    filePointer = fopen("buses.txt", "r");
    if (filePointer == NULL) {
        fprintf(stderr, "Error opening the file.\n");
        return 1;
    }

    char line[100];
    int i = 0;
    while (fgets(line, sizeof(line), filePointer) != NULL) {
        i++;
        printf("Bus %d: %s", i, line);
    }
    fclose(filePointer);
    break;
}

    case 3:
    {
        char dest[15];
        int voyID2 = 0;
        FILE *idFile = fopen("res_id.txt", "r");

        if (idFile != NULL)
        {
            fscanf(idFile, "%d", &resID);
            fclose(idFile);
        }

        printf("Donnez votre voyID: \n");
        scanf("%d", &voyID2);
        printf("Donnez votre destination: \n");
        scanf("%s", dest);

        FILE *filePointer;
        filePointer = fopen("reservations.txt", "a");

        if (filePointer == NULL)
        {
            printf("Le fichier ne peut pas etre ouvert.\n");
            return 1;
        }
        fprintf(filePointer, "%d %d %s \n", resID + 1, voyID2, dest);
        resID++;

        fclose(filePointer);

        idFile = fopen("res_id.txt", "w");
        if (idFile != NULL)
        {
            fprintf(idFile, "%d", resID);
            fclose(idFile);
        }
        puts("Votre reservation a ete bien sauvegardee. Pour savoir plus de details sur votre voyage vous pouvez essayer les autres commandes");
        break;
    }




case 4: {

    FILE *resFile = fopen("reservations.txt", "r");
    if (resFile == NULL) {
        fprintf(stderr, "Error opening reservations file.\n");
        return 1;
    }

    printf("Liste des reservations :\n");
    int resID, voyID;
    char dest[15];
    while (fscanf(resFile, "%d %d %14[^\n]", &resID, &voyID, dest) == 3) {
        printf("Reservation ID: %d, Voyager ID: %d, Destination: %s\n", resID, voyID, dest);
    }

    fclose(resFile);
    int selectedResID;
    printf("Entrez l'ID de la reservation a modifier : ");
    scanf("%d", &selectedResID);

    resFile = fopen("reservations.txt", "r+");
    if (resFile == NULL) {
        fprintf(stderr, "Error opening reservations file for modification.\n");
        return 1;
    }

    rewind(resFile);

    FILE *tempFile = fopen("temp_reservations.txt", "w");
    if (tempFile == NULL) {
        fprintf(stderr, "Error creating temporary file.\n");
        fclose(resFile);
        return 1;
    }
    while (fscanf(resFile, "%d %d %14[^\n]", &resID, &voyID, dest) == 3) {
        if (resID == selectedResID) {
            // For simplicity, let's just modify the destination
            printf("Modifiez la destination : ");
            scanf("%s", dest);
        }

        // Write the data to the temporary file
        fprintf(tempFile, "%d %d %s\n", resID, voyID, dest);

    }
    printf("Votre destination a ete bien modifiee \n");

    fclose(resFile);
    fclose(tempFile);

    // Remove the original reservations file
    if (remove("reservations.txt") != 0) {
        fprintf(stderr, "Error removing original reservations file.\n");
        return 1; // Indicates an error
    }

    // Rename the temporary file to reservations.txt
    if (rename("temp_reservations.txt", "reservations.txt") != 0) {
        fprintf(stderr, "Error renaming temporary file.\n");
        return 1; // Indicates an error
    }

    break;
}


   case 5: {
        // Ask the user for the destination
        char customerDestination[15];
        printf("Entrez votre destination : ");
        scanf("%14s", customerDestination);

        // Convert the customer's input to lowercase (or uppercase)
        for (int i = 0; customerDestination[i]; i++) {
            customerDestination[i] = tolower(customerDestination[i]);
        }
        FILE *busFile = fopen("buses.txt", "r");
        if (busFile == NULL) {
            fprintf(stderr, "Error opening buses file.\n");
            return 1;
        }

        char prix[30], destination[15], departTime[6];
        int found = 0;
        while (fscanf(busFile, "%29[^,], %14[^,], %5[^\n]\n", prix, destination, departTime) == 3) {

            for (int i = 0; destination[i]; i++) {
                destination[i] = tolower(destination[i]);
            }

            if (strcmp(customerDestination, destination) == 0) {

                printf("Le bus pour la destination %s coute %s et part a %s.\n", destination, prix, departTime);
                found = 1;
                break;
            }
        }
        fclose(busFile);

        // Check if the destination was not found
        if (!found) {
            printf("Destination non trouvee.\n");
        }

        break;
    }







    default:
        printf("Commande inexistante\n");
    }

    printf("Si vous voulez continuer a naviguer appuyer sur 1 \n");
    scanf("%d",&continuer);

    }
    return 0;

}
