#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define COLOR_RESET   "\x1b[0m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_RED     "\x1b[31m"
#define COLOR_YELLOW "\x1b[33m"
#define MAX_CARTE 100
#define MAX_TITLU 100
#define MAX_AUTOR 100
#define MAX_TIP 50
#define MAX_LOCATIE 50
#define FILENAME "carti.txt"

typedef struct 
{
    char titlu[MAX_TITLU];
    char autor[MAX_AUTOR];
    char tip[MAX_TIP];
    char locatie[MAX_LOCATIE];
    int valabil;
    time_t data_imprumut; 
    int zile_imprumut;
} carte;

carte librarie[MAX_CARTE];
int bc = 0;

void clear_screen() 
{
    #ifdef _WIN32
        system("cls"); 
    #else
        system("clear"); 
    #endif
}

void goleste_stdin() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}



void newline_remover(char *str) 
{
    size_t len = strlen(str);
    if (len > 0 && str[len-1] == '\n') {
        str[len-1] = '\0';
    }
}

void lowercase(char *str) 
{
    if (!str) return;
    for (int i = 0; str[i]; i++) 
    {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int contine_virgula(const char *sir) {
    if(strchr(sir, ',') != NULL)
    {
        printf("Nu poate contine virgula. Apasati 'Enter' pentru a reveni la meniu.");
        getchar();
        return 1;
    }
    else return 0;
}

void load_carte() 
{
    FILE *file = fopen(FILENAME, "r");
    if (!file) 
    {
        printf("Nu s-a gasit fisierul. Se va crea unul nou la salvare.\n");
        return;
    }
    bc = 0;
    while (fscanf(file, "%99[^,],%99[^,],%49[^,],%49[^,],%d,%ld,%d\n",
                  librarie[bc].titlu,
                  librarie[bc].autor,
                  librarie[bc].tip,
                  librarie[bc].locatie,
                  &librarie[bc].valabil,
                  &librarie[bc].data_imprumut,
                  &librarie[bc].zile_imprumut) == 7) 
    {
        bc++;
        if (bc >= MAX_CARTE) break;
    }
    fclose(file);
}

void salveaza_carte() 
{
    FILE *file = fopen(FILENAME, "w");
    if (!file)
    {
        printf("Eroare la salvarea fisierului.\n");
        return;
    }
    for (int i = 0; i < bc; i++) 
    {
        fprintf(file, "%s,%s,%s,%s,%d,%ld,%d\n",
                librarie[i].titlu, librarie[i].autor, librarie[i].tip,
                librarie[i].locatie, librarie[i].valabil,
                librarie[i].data_imprumut, librarie[i].zile_imprumut);
    }
    fclose(file);
}

void adauga_carte() 
{
    clear_screen();
    if (bc >= MAX_CARTE) 
    {
        printf("Nu se mai pot adauga carti.\n");
        return;
    }

    char buffer[MAX_TITLU];
    printf("--- Adauga Carte ---\n");
    printf("Nu folositi virgule\n");
    printf("Titlu ('Testament'): ");
    fgets(buffer, MAX_TITLU, stdin);
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] != '\n') {
        goleste_stdin();
    }

    newline_remover(buffer);
    if(contine_virgula(buffer))
    {
        return;
    }
    else strcpy(librarie[bc].titlu, buffer);

    printf("Autor (Nume Prenume 'Tudor Arghezi'): ");
    fgets(buffer, MAX_AUTOR, stdin);
    len = strlen(buffer);
    if (len > 0 && buffer[len - 1] != '\n') {
        goleste_stdin();
    }

    newline_remover(buffer);

    if(contine_virgula(buffer))
    {
        return;
    }
    else strcpy(librarie[bc].autor, buffer);

    int carte_existenta = 0;
    for (int i = 0; i < bc; i++) 
    {
        if (strcmp(librarie[i].titlu, librarie[bc].titlu) == 0 && 
            strcmp(librarie[i].autor, librarie[bc].autor) == 0) 
        {
            carte_existenta = 1;
            break;
        }
    }

    if (carte_existenta) 
    {
        printf("Aceasta carte este deja in librarie.\n");
        getchar();
        clear_screen();
        return;
    }

    printf("Tip ('Actiune Drama ...'): ");
    fgets(buffer, MAX_TIP, stdin);
    len = strlen(buffer);
    if (len > 0 && buffer[len - 1] != '\n') {
        goleste_stdin();    
    }

    newline_remover(buffer);

    if(contine_virgula(buffer))
    {
        return;
    }
    else strcpy(librarie[bc].tip, buffer);

    printf("Locatie ('Raftul 5'): ");
    fgets(buffer, MAX_LOCATIE, stdin);
    len = strlen(buffer);
    if (len > 0 && buffer[len - 1] != '\n') {
        goleste_stdin();
    }

    newline_remover(buffer);

    if(contine_virgula(buffer))
    {
        return;
    }
    else strcpy(librarie[bc].locatie, buffer);

    librarie[bc].valabil = 1;
    bc++;
    salveaza_carte();
    printf("Cartea a fost adaugata.\n");
    while (getchar() != '\n'); 
    clear_screen();
}

void cauta_dupa_criteriu(int criteriu) 
{
    clear_screen();
    char search_term[MAX_LOCATIE] = "";
    int search_disponibilitate = -1; 

    switch (criteriu) 
    {
        case 1:
            printf("--- Cauta dupa Titlu ---\n");
            printf("Introduceti titlul: ");
            if (fgets(search_term, MAX_TITLU, stdin)) {
                newline_remover(search_term);
                if (strlen(search_term) == 0) {
                    printf("Nu ai introdus niciun titlu.\n");
                    printf("\nApasa 'Enter' pentru a continua ");
                    while (getchar() != '\n');
                    return;
                }
                lowercase(search_term);
            }
            break;

        case 2:
            printf("--- Cauta dupa Autor ---\n");
            printf("Introduceti autorul: ");
            if (fgets(search_term, MAX_AUTOR, stdin)) {
                newline_remover(search_term);
                if (strlen(search_term) == 0) {
                    printf("Nu ai introdus niciun autor.\n");
                    printf("\nApasa 'Enter' pentru a continua ");
                    while (getchar() != '\n');
                    return;
                }
                lowercase(search_term);
            }
            break;
            
        case 3:
            printf("--- Cauta dupa Disponibilitate ---\n");
            printf("1 - Disponibile | 0 - Imprumutate (Introduceti orice alt nr ('2',...) pentru a reveni la meniul principal)\n");
            printf("Introduceti disponibilitatea: ");
            while (scanf("%d", &search_disponibilitate) != 1) {
                printf("Introducere invalida, te rog introdu 1 sau 0. Apasati 'Enter' pentru a reincerca.");
                while (getchar() != '\n'); getchar();
                clear_screen();
                printf("--- Cauta dupa Disponibilitate ---\n");
                printf("1 - Disponibile | 0 - Imprumutate (Introduceti orice alt nr ('2',...) pentru a reveni la meniul principal)\n");
                printf("Introduceti disponibilitatea: ");
            }
            getchar();
            
            if (search_disponibilitate != 1 && search_disponibilitate != 0) {
                printf("Optiune anulata Apasati 'Enter' pentru a revenii la meniul principal");
                while (getchar() != '\n');
                return;
            }
            break;

        case 4:
            printf("--- Cauta dupa Locatie ---\n");
            printf("Introduceti locatia ('Raftul 5','5',...): ");
            if (fgets(search_term, MAX_LOCATIE, stdin)) {
                newline_remover(search_term);
                if (strlen(search_term) == 0) {
                    printf("Nu ai introdus nicio locatie.\n");
                    printf("\nApasa 'Enter' pentru a continua ");
                    while (getchar() != '\n');
                    return;
                }
                lowercase(search_term);
            }
            break;

        case 5:
            printf("--- Afiseaza biblioteca ---\n");
            if(bc==0)
            {
                printf("Biblioteca este goala.");
                getchar();
                return;
            }
            break;

        default:
            printf("Optiune invalida. Apasa 'Enter' pentru a reincerca\n");
            while (getchar() != '\n');
            return;
    }

    int found = 0;
    for (int i = 0; i < bc; i++) 
    {
        char lower_titlu[MAX_TITLU], lower_autor[MAX_AUTOR], lower_locatie[MAX_LOCATIE];
        strcpy(lower_titlu, librarie[i].titlu);
        strcpy(lower_autor, librarie[i].autor);
        strcpy(lower_locatie, librarie[i].locatie);
        lowercase(lower_titlu);
        lowercase(lower_autor);
        lowercase(lower_locatie);

        int matches = 0;
        switch (criteriu) 
        {
            case 1:
                matches = strstr(lower_titlu, search_term) != NULL;
                break;
            case 2:
                matches = strstr(lower_autor, search_term) != NULL;
                break;
            case 3:
                matches = (librarie[i].valabil == search_disponibilitate);
                break;
            case 4:
                matches = strstr(lower_locatie, search_term) != NULL;
                break;
            case 5:
                matches = 1;
                break;
        }

        if (matches) 
        {
            if (librarie[i].valabil) {
                printf("- %s%s%s de %s%s%s [%s] - %sDisponibil%s - Locatie: %s\n",
                       COLOR_GREEN, librarie[i].titlu, COLOR_RESET,
                       COLOR_YELLOW, librarie[i].autor, COLOR_RESET, librarie[i].tip,
                       COLOR_GREEN, COLOR_RESET,
                       librarie[i].locatie);
            } else {
                printf("- %s%s%s de %s%s%s [%s] - %sImprumutat%s - Locatie: %s\n",
                       COLOR_RED, librarie[i].titlu, COLOR_RESET,
                       COLOR_YELLOW, librarie[i].autor, COLOR_RESET, librarie[i].tip,
                       COLOR_RED, COLOR_RESET,
                       librarie[i].locatie);
            }
            found = 1;
        }
    }

    if (!found) 
    {
        printf("Nici-o carte gasita.\n");
    }
    printf("\nApasa 'Enter' pentru a te intoarce la meniul principal ");
    while (getchar() != '\n');
}

void imprumuta_carte() 
{
    clear_screen(); 
    char titlu[MAX_TITLU];
    int matches[MAX_CARTE] = {0}; 
    int match_count = 0;         
    
    printf("--- Imprumuta Carte ---\n");
    printf("Introdu titlul cartii (sau o parte din titlu): ");
    if(fgets(titlu, MAX_TITLU, stdin) == NULL)
    {
        printf("Eroare la citirea titlului. Apasati 'Enter' pentru a reveni la meniu\n");
        getchar();
        return;
    }
    newline_remover(titlu);
    lowercase(titlu);

    for (int i = 0; i < bc; i++) 
    {
        char lower_titlu[MAX_TITLU];
        strcpy(lower_titlu, librarie[i].titlu);
        lowercase(lower_titlu);
        
        if (strstr(lower_titlu, titlu) != NULL) 
        {
            matches[i] = 1;
            match_count++;
        }
    }

    if (match_count == 0) 
    {
        printf("Nu s-a gasit nicio carte care sa corespunda.\n");
        while (getchar() != '\n');
        clear_screen();
        return;
    }

    if (match_count == 1) 
    {
        for (int i = 0; i < bc; i++) 
        {
            if (matches[i]) 
            {
                if (librarie[i].valabil) 
                {
                    int z;
                    printf("Vreti sa imprumutati cartea '%s'?\n", librarie[i].titlu);
                    printf("1 - Da || 0 - Nu\n");
                    printf("Alege o optiune (1 sau 0): ");
                    
                    if((scanf("%d", &z) != 1 || (z != 1 && z!=0)))
                    {
                    do
                    {
                        printf("Optiune invalida. Apasati 'Enter' pentru a reincerca");
                        while (getchar() != '\n');
                        getchar();
                        clear_screen();
                        printf("Vreti sa imprumutati cartea '%s'?\n", librarie[i].titlu);
                        printf("1 - Da || 0 - Nu\n");
                        printf("Alege o optiune (1 sau 0): ");
                    }
                    while(scanf("%d", &z) != 1 || (z != 1 && z!=0));
                    }
                    while (getchar() != '\n');
                    
                    if(z != 1 )
                    {
                        printf("Operatiune anulata. Apasa 'Enter' pentru a reveni la meniu");
                        getchar();
                        clear_screen();
                        return;
                    }
                    
                    int zile;
                    printf("Introduceti numarul de zile pentru imprumut a carti (maxim 10 ex:'1','2',....'10'): ");
                    if (scanf("%d", &zile) != 1 || zile<1 || zile>10) {
                        "Numar invalid de zile. Apasati 'Enter' pentru a va intoarce la meniu";
                        while (getchar() != '\n'); getchar();
                        clear_screen();
                        return;
                    }
                    while (getchar() != '\n'); 
                    
                    librarie[i].valabil = 0;
                    librarie[i].data_imprumut = time(NULL);
                    librarie[i].zile_imprumut = zile;
                    salveaza_carte();
                    printf("Cartea \"%s\" a fost imprumutata pentru %d zile.\n", librarie[i].titlu, zile);
                } 
                else 
                {
                    time_t now = time(NULL);
                    double diff_seconds = difftime(now, librarie[i].data_imprumut);
                    int diff_days = (int)(diff_seconds / (60 * 60 * 24));
                    int remaining_days = librarie[i].zile_imprumut - diff_days;
                    
                    if (remaining_days > 0) {
                        printf("Cartea \"%s\" nu este disponibila. Mai sunt %d zile pana la returnare.\n", 
                               librarie[i].titlu, remaining_days);
                        while (getchar() != '\n');
                    } else {
                        printf("Cartea \"%s\" este depasita cu %d zile. Va fi marcată ca returnată.\n", 
                               librarie[i].titlu, -remaining_days);
                        librarie[i].valabil = 1;
                        salveaza_carte();
                    }
                }
                while (getchar() != '\n'); 
                clear_screen();
                return;
            }
        }
    }
    else 
    {
        printf("\nAu fost gasite %d carti care corespund:\n", match_count);
        for (int i = 0, j = 1; i < bc; i++) 
        {
            if (matches[i]) 
            {
                if (!librarie[i].valabil) {
                    time_t now = time(NULL);
                    double diff_seconds = difftime(now, librarie[i].data_imprumut);
                    int diff_days = (int)(diff_seconds / (60 * 60 * 24));
                    int remaining_days = librarie[i].zile_imprumut - diff_days;
                    
                    printf("%d. %s%s%s de %s%s%s [%s] - %sImprumutat%s (%d zile ramase) - Locatie: %s\n",
                        j++, COLOR_RED, librarie[i].titlu, COLOR_RESET,
                        COLOR_YELLOW, librarie[i].autor, COLOR_RESET, librarie[i].tip,
                        COLOR_RED, COLOR_RESET, remaining_days, librarie[i].locatie);
                } else {
                    printf("%d. %s%s%s de %s%s%s [%s] - %sDisponibil%s - Locatie: %s\n",
                        j++, COLOR_GREEN, librarie[i].titlu, COLOR_RESET,
                        COLOR_YELLOW, librarie[i].autor, COLOR_RESET, librarie[i].tip,
                        COLOR_GREEN, COLOR_RESET, librarie[i].locatie);
                }
            }
        }

        printf("\nIntrodu numarul cartii pe care doresti sa o imprumuti (sau 0 pentru a anula): ");
        int choice;
        while (scanf("%d", &choice) != 1 || choice < 0 || choice > match_count) 
        {
            while (getchar() != '\n');
            printf("Optiune invalida. Te rog introdu un numar intre 0 si %d.", match_count); getchar();
            clear_screen();
            printf("\nAu fost gasite %d carti care corespund:\n", match_count);
            for (int i = 0, j = 1; i < bc; i++) 
            {
                if (matches[i]) 
                {
                    if (!librarie[i].valabil) {
                        time_t now = time(NULL);
                        double diff_seconds = difftime(now, librarie[i].data_imprumut);
                        int diff_days = (int)(diff_seconds / (60 * 60 * 24));
                        int remaining_days = librarie[i].zile_imprumut - diff_days;
                        
                        printf("%d. %s%s%s de %s%s%s [%s] - %sImprumutat%s (%d zile ramase) - Locatie: %s\n",
                            j++, COLOR_RED, librarie[i].titlu, COLOR_RESET,
                            COLOR_YELLOW, librarie[i].autor, COLOR_RESET, librarie[i].tip,
                            COLOR_RED, COLOR_RESET, remaining_days, librarie[i].locatie);
                    } else {
                        printf("%d. %s%s%s de %s%s%s [%s] - %sDisponibil%s - Locatie: %s\n",
                            j++, COLOR_GREEN, librarie[i].titlu, COLOR_RESET,
                            COLOR_YELLOW, librarie[i].autor, COLOR_RESET, librarie[i].tip,
                            COLOR_GREEN, COLOR_RESET, librarie[i].locatie);
                    }
                }
            }
            printf("\nIntrodu numarul cartii pe care doresti sa o imprumuti (sau 0 pentru a anula): ");
        }
        getchar();

        if (choice == 0) 
        {
            printf("Operatiune anulata. Apasa 'Enter' pentru a reveni la meniu\n");
            while (getchar() != '\n'); 
            clear_screen();
            return;
        }

        for (int i = 0, j = 1; i < bc; i++) 
        {
            if (matches[i]) 
            {
                if (j == choice) 
                {
                    if (librarie[i].valabil) 
                    {
                        int zile;
                        printf("Introdu numarul de zile pentru imprumut (maxim 10 ex:'1','2',....'10'): ");
                        if(scanf("%d", &zile) != 1)
                        {
                            printf("Input invalid. Apasati 'Enter' pentru a reveni la meniu");
                            while (getchar() != '\n'); getchar();
                            return;
                        }
                        while (getchar() != '\n'); 
                        
                        if (zile <= 0 || zile > 10) {
                            printf("Numar invalid de zile. Trebuie sa fie intre 1 si 10.\n");
                            getchar();
                            clear_screen();
                            return;
                        }
                        
                        librarie[i].valabil = 0;
                        librarie[i].data_imprumut = time(NULL);
                        librarie[i].zile_imprumut = zile;
                        salveaza_carte();
                        printf("Cartea \"%s\" a fost imprumutata pentru %d zile.\n", librarie[i].titlu, zile);
                    } 
                    else 
                    {
                        time_t now = time(NULL);
                        double diff_seconds = difftime(now, librarie[i].data_imprumut);
                        int diff_days = (int)(diff_seconds / (60 * 60 * 24));
                        int remaining_days = librarie[i].zile_imprumut - diff_days;
                        
                        if (remaining_days > 0) {
                            printf("Cartea \"%s\" nu este disponibila. Mai sunt %d zile pana la returnare.\n", 
                                   librarie[i].titlu, remaining_days);
                        } else {
                            printf("Cartea \"%s\" este depasita cu %d zile. Va fi marcată ca returnată.\n", 
                                   librarie[i].titlu, -remaining_days);
                            librarie[i].valabil = 1;
                            salveaza_carte();
                        }
                    }
                    getchar();
                    clear_screen();
                    return;
                }
                j++;
            }
        }
    }
}

void returneaza_carte() 
{
    clear_screen();
    char titlu[MAX_TITLU];
    int matches[MAX_CARTE] = {0}; 
    int match_count = 0;         
    
    printf("--- Returneaza Carte ---\n");
    printf("Introdu titlul cartii (sau o parte din titlu): ");
    fgets(titlu, MAX_TITLU, stdin);
    newline_remover(titlu);
    lowercase(titlu);

    for (int i = 0; i < bc; i++) 
    {
        char lower_titlu[MAX_TITLU];
        strcpy(lower_titlu, librarie[i].titlu);
        lowercase(lower_titlu);
        
        if (strstr(lower_titlu, titlu) != NULL) 
        {
            matches[i] = 1;
            match_count++;
        }
    }

    if (match_count == 0) 
    {
        printf("Nu s-a gasit nicio carte care sa corespunda.\n");
        while (getchar() != '\n');
        clear_screen();
        return;
    }
    else if (match_count == 1) 
    {
        for (int i = 0; i < bc; i++) 
        {
            if (matches[i]) 
            {
                if (!librarie[i].valabil) 
                {
                    time_t now = time(NULL);
                    double diff_seconds = difftime(now, librarie[i].data_imprumut);
                    int diff_days = (int)(diff_seconds / (60 * 60 * 24));
                    
                    librarie[i].valabil = 1;
                    salveaza_carte();
                    
                    if (diff_days > librarie[i].zile_imprumut) {
                        printf("Cartea \"%s\" a fost returnata cu %d zile intarziere!\n", 
                               librarie[i].titlu, diff_days - librarie[i].zile_imprumut);
                    } else {
                        printf("Cartea \"%s\" a fost returnata la timp.\n", librarie[i].titlu);
                    }
                } 
                else 
                {
                    printf("Cartea \"%s\" nu era imprumutata.\n", librarie[i].titlu);
                }
                while (getchar() != '\n');
                clear_screen();
                return;
            }
        }
    }
    else 
    {
        printf("\nAu fost gasite %d carti care corespund:\n", match_count);
        for (int i = 0, j = 1; i < bc; i++) 
        {
            if (matches[i]) 
            {
                if (!librarie[i].valabil) {
                    time_t now = time(NULL);
                    double diff_seconds = difftime(now, librarie[i].data_imprumut);
                    int diff_days = (int)(diff_seconds / (60 * 60 * 24));
                    int remaining_days = librarie[i].zile_imprumut - diff_days;
                    
                    printf("%d. %s%s%s de %s%s%s [%s] - %sImprumutat (%d zile ramase)%s - Locatie: %s\n",
                        j++, 
                        COLOR_RED, librarie[i].titlu, COLOR_RESET,
                        COLOR_YELLOW, librarie[i].autor, COLOR_RESET, 
                        librarie[i].tip,
                        COLOR_RED,
                        remaining_days,
                        COLOR_RESET,
                        librarie[i].locatie);
                } else {
                    printf("%d. %s%s%s de %s%s%s [%s] - %sDisponibil%s - Locatie: %s\n",
                        j++,
                        COLOR_GREEN, librarie[i].titlu, COLOR_RESET,  
                        COLOR_YELLOW, librarie[i].autor, COLOR_RESET, 
                        librarie[i].tip,                              
                        COLOR_GREEN, COLOR_RESET,                     
                        librarie[i].locatie);                        
                }
            }
        }

        printf("\nIntrodu numarul cartii pe care doresti sa o returnezi (sau 0 pentru a anula): ");
        int choice;
        while (scanf("%d", &choice) != 1 || choice < 0 || choice > match_count) 
        {
            while (getchar() != '\n');
            printf("Optiune invalida. Te rog introdu un numar intre 0 si %d: ", match_count);
        }
        getchar();

        if (choice == 0) 
        {
            printf("Operatiune anulata. Apsati 'Enter' pentru a va intoarce la meniu\n");
            getchar();
            clear_screen();
            return;
        }

        for (int i = 0, j = 1; i < bc; i++) 
        {
            if (matches[i]) 
            {
                if (j == choice) 
                {
                    if (!librarie[i].valabil) 
                    {
                        time_t now = time(NULL);
                        double diff_seconds = difftime(now, librarie[i].data_imprumut);
                        int diff_days = (int)(diff_seconds / (60 * 60 * 24));
                        
                        librarie[i].valabil = 1;
                        salveaza_carte();
                        
                        if (diff_days > librarie[i].zile_imprumut) {
                            printf("Cartea \"%s\" a fost returnata cu %d zile intarziere!\n", 
                                   librarie[i].titlu, diff_days - librarie[i].zile_imprumut);
                        } else {
                            printf("Cartea \"%s\" a fost returnata la timp.\n", librarie[i].titlu);
                        }
                    } 
                    else 
                    {
                        printf("Cartea \"%s\" nu era imprumutata.\n", librarie[i].titlu);
                    }
                    getchar();
                    clear_screen();
                    return;
                }
                j++;
            }
        }
    }
}

void submeniu_adauga() {
    int option;
    do {
        clear_screen();
        printf("--- Submeniu Adauga ---\n");
        printf("1. Adauga o carte noua\n");
        printf("2. Intoarcere la meniul principal\n");
        printf("Alege o optiune: ");
        if (scanf("%d", &option) != 1) {
            option = 0;
        }
        while (getchar() != '\n'); 

        switch (option) 
        {
            case 1: adauga_carte(); break;
            case 2: return;
            default: printf("Optiune invalida. (Apasati 'Enter' pentru a reveni la meniu)\n"); while (getchar() != '\n');
        }
    } while (option != 2);
}

void submeniu_cauta() 
{
    int option;
    do 
    {
        clear_screen();
        printf("--- Submeniu Cautare ---\n");
        printf("1. Cauta dupa Titlu\n");
        printf("2. Cauta dupa Autor\n");
        printf("3. Cauta dupa Disponibilitate\n");
        printf("4. Cauta dupa Locatie\n");
        printf("5. Afiseaza biblioteca\n");
        printf("6. Inapoi la meniul principal\n");
        printf("Alege o optiune: ");
        
        if (scanf("%d", &option) != 1) {
            option = 0;
        }
        while (getchar() != '\n'); 

        switch (option) 
        {
            case 1: 
            case 2: 
            case 3: 
            case 4:
            case 5:
                cauta_dupa_criteriu(option);
                return;
            case 6: 
                return;
            default: 
                printf("Optiune invalida. Apasati 'Enter' pentru a reincerca\n"); 
                while (getchar() != '\n');
        }
    } while (option != 6);
}

void submeniu_imprumuta() 
{
    int option;
    do {
        clear_screen();
        printf("--- Submeniu Imprumuta ---\n");
        printf("1. Imprumuta o carte\n");
        printf("2. Intoarcere la meniul principal\n");
        printf("Alege o optiune: ");
        if (scanf("%d", &option) != 1) {
            option = 0;
        }
        while (getchar() != '\n'); 

        switch (option) {
            case 1: imprumuta_carte(); break;
            case 2: return;
            default: printf("Optiune invalida. (Apasati 'Enter' pentru a reveni la meniu)\n"); while (getchar() != '\n');
        }
    } while (option != 2);
}

void submeniu_returneaza() 
{
    int option;
    do {
        clear_screen();
        printf("--- Submeniu Returneaza ---\n");
        printf("1. Returneaza o carte\n");
        printf("2. Intoarcere la meniul principal\n");
        printf("Alege o optiune: ");
        if (scanf("%d", &option) != 1) {
            option = 0;
        }
        while (getchar() != '\n'); 

        switch (option) 
        {
            case 1: returneaza_carte(); break;
            case 2: return;
            default: printf("Optiune invalida. (Apasati 'Enter' pentru a reveni la meniu)\n"); while (getchar() != '\n');
        }
    } while (option != 2);
}

void menu() 
{
    int option;
    do {
        clear_screen();
        printf("--- Meniu Principal ---\n");
        printf("1. Adauga carte\n");
        printf("2. Cauta carte\n");
        printf("3. Imprumuta carte\n");
        printf("4. Returneaza carte\n");
        printf("5. Iesire\n");
        printf("Alege o optiune: ");
        if (scanf("%d", &option) != 1) {
            option = 0;
        }
        while (getchar() != '\n'); 
        switch (option) {
            case 1: submeniu_adauga(); break;
            case 2: submeniu_cauta(); break;
            case 3: submeniu_imprumuta(); break;
            case 4: submeniu_returneaza(); break;
            case 5: printf("EXIT!\n"); while (getchar() != '\n'); break;
            default: printf("Optiune invalida. (Apasati 'Enter' pentru a reveni la meniu)\n");  while (getchar() != '\n');
        }
    } while (option != 5);
}

int main() 
{
    load_carte();
    menu();
    return 0;
}
