#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../cJSON/cJSON.h"
//defition de la structure de hero et de ses sous structures
typedef struct {
        int intelligence;
        int strength;
        int speed;
        int durability;
        int power;
        int combat;
    }pwst;
typedef struct{
    char gender[10];
    char race[50];
    char weight[10];
    char height[10];
    char eyecolor[10];
    char haircolor[10];
}app;
typedef struct {
    int id;
    char name[50];
    int idapi;
    char slug[50];
    pwst powerstats;
    app appearance;
} Hero;

//affiche le menu avec les fonctions principales
void afficheMenuPrincipal(){
    printf("+---------------------SUPER-HEROS---------------------+\n");
    printf("|1: Afficher la liste de tous les heros               |\n");
    printf("|2: Afficher les caracteristiques en detail d'un hero |\n");
    printf("|3: Recherche avancee d'un hero                       |\n");
    printf("|4: Comparaison de 2 heros                            |\n");
    printf("|5: Gestion des favoris                               |\n");
    printf("|6: Quizz                                             |\n");
    printf("|7: Plus d'informations sur les heros                 |\n");
    printf("|0: Quitter le programme                              |\n");
    printf("+-----------------------------------------------------+\n");
    printf("Quel est votre choix ?\n>");
}
//affiche des liens utiles pour obtenir des informations suplementaires sur les heros
void afficheLiens(){
    printf("+-----------------RESSOURCES-EN-LIGNE-----------------+\n");
    printf("|Marvel    : https://www.marvel.com/characters        |\n");
    printf("|Wikipedia : https://fr.wikipedia.org/                |\n");
    printf("|IMDb      : https://www.imdb.com/                    |\n");
    printf("|DC        : https://www.dc.com/characters            |\n");
    printf("+-----------------------------------------------------+\n");
}

//converti le tableau correspondant au caracteristique d un filtre en chaine de caractere pour pouvoir l'afficher lisiblement
void etatTexte(int etat[], char txt[]){
    char intstr[3];
    char symbole;
    if (etat[0]==0) strcpy(txt,"OFF");
    else {
        if (etat[1]==1){
            symbole='<';
        }
        else if (etat[1]==2){
            symbole='>';
        }
        else if (etat[1]==3){
            symbole='=';
        }
        sprintf(txt,"%c%3d",symbole,etat[2]);
    }
}

//affiche le menu avec les fonctions principales de recherche ainsi que l etat actuel des filtres
void afficheMenuRecherche(int filtres[6][3]){
    char intel[8], force[8], vit[8], dura[8], puiss[8], combat[8];
    etatTexte(filtres[0],intel);
    etatTexte(filtres[1],force);
    etatTexte(filtres[2],vit);
    etatTexte(filtres[3],dura);
    etatTexte(filtres[4],puiss);
    etatTexte(filtres[5],combat);
    printf("+----------------------RECHERCHE----------------------+\n");
    printf("|1: Rechercher un hero par nom                        |\n");
    printf("|2: Modifier les filtres                              |\n");
    printf("|3: Recherche d'apres les filtres                     |\n");
    printf("+-----------------------FILTRES-----------------------+\n");
    printf("|Intelligence: %-4s                                   |\n",intel);
    printf("|Force: %-4s                                          |\n",force);
    printf("|Vitesse: %-4s                                        |\n",vit);
    printf("|Durabilite: %-4s                                     |\n",dura);
    printf("|Puissance: %-4s                                      |\n",puiss);
    printf("|Combat: %-4s                                         |\n",combat);
    printf("+-----------------------------------------------------+\n");
    printf("|0: Quitter la Recherche                              |\n");
    printf("+-----------------------------------------------------+\n");
    printf("Quel est votre choix ?\n>");
}

//affiche le premier menu de modification des filtres
void afficheMenuFiltres(){
    printf("+---------------MODIFICATION-DES-FILTRES--------------+\n");
    printf("|1:Intelligence                                       |\n");
    printf("|2:Force                                              |\n");
    printf("|3:Vitesse                                            |\n");
    printf("|4:Durabilite                                         |\n");
    printf("|5:Puissance                                          |\n");
    printf("|6:Combat                                             |\n");
    printf("|0:Quitter la modification                            |\n");
    printf("+-----------------------------------------------------+\n");
    printf("Quel est votre choix ?\n>");
}

//affiche le menu de modification d'un filtre particulier
void afficheMenuDetailFiltre(int filtre[3]){
    char etat[8];
    etatTexte(filtre,etat);
    printf("+--------------------DETAIL-FILTRE--------------------+\n");
    printf("|1:Modifer operateur                                  |\n");
    printf("|2:Modifier Valeur                                    |\n");
    printf("|3:Modifier etat on/off                               |\n");
    printf("+---------------------ETAT-ACTUEL---------------------+\n");
    printf("|                        %-4s                         |\n");
    printf("+-----------------------------------------------------+\n");
    printf("|0:Quitter la modification                            |\n");
    printf("+-----------------------------------------------------+\n");
    printf("Quel est votre choix ?\n>");
}

//fonction presentes sur la feuille donnee au premier tp afin de pouvoir extraire les donnees de SuperHero.json
int readTextFile(const char filename[], char buffer[], int maxSize){
    FILE *f = fopen(filename,"r");
    if (f == NULL) return 0;

    int n = fread(buffer, 1, maxSize-1, f);
    buffer[n]='\0';

    fclose(f);
    return 1;
}
int getIntOrDefault(cJSON *obj, const char field[], int def){
    cJSON*x=cJSON_GetObjectItemCaseSensitive(obj,field);
    if (cJSON_IsNumber(x)) return x->valueint;
    return def;
}
void getStringOrDefault(cJSON*obj, const char field[], char dest[], int destSize, const char def[]){
    cJSON *x = cJSON_GetObjectItemCaseSensitive(obj, field);
    if (cJSON_IsString(x)&& x->valuestring != NULL){
        strncpy(dest,x->valuestring, destSize-1);
        dest[destSize-1]='\0';
    } else {
        strncpy(dest,def,destSize-1);
        dest[destSize-1]='\0';
    }
}

//affiche l'id et le nom de chaque hero present dans le tableau passe en parametre de taille size passee en parametre
void afficheHeros(Hero heros[], int size){
    printf("\n");
    for (int i=0; i<size; i++){
        printf("id : %d\t nom : %s\n", heros[i].id, heros[i].name);
    }
    printf("\n");
}

//extrait les donnees de SuperHeros.json pour remplir le tableau heros pour pouvoir y acceder facilement
void rempliHeros(cJSON *array, Hero heros[], int size){
    for (int i=0; i<size; i++){
        cJSON *obj = cJSON_GetArrayItem(array,i);
        heros[i].id=getIntOrDefault(obj,"id",-1);
        getStringOrDefault(obj,"name",heros[i].name,50,"inconnu");
        heros[i].idapi=getIntOrDefault(obj,"id-api",-1);
        getStringOrDefault(obj,"slug",heros[i].slug,50,"inconnu");
        cJSON *pwst;
        pwst=cJSON_GetObjectItemCaseSensitive(obj,"powerstats");
        heros[i].powerstats.combat=getIntOrDefault(pwst,"combat",-1);
        heros[i].powerstats.durability=getIntOrDefault(pwst,"durability",-1);
        heros[i].powerstats.intelligence=getIntOrDefault(pwst,"intelligence",-1);
        heros[i].powerstats.power=getIntOrDefault(pwst,"power",-1);
        heros[i].powerstats.speed=getIntOrDefault(pwst,"speed",-1);
        heros[i].powerstats.strength=getIntOrDefault(pwst,"strength",-1);
        cJSON *app=NULL;
        app = cJSON_GetObjectItemCaseSensitive(obj,"appearance");
        getStringOrDefault(app,"gender",heros[i].appearance.gender,10,"inconnu");
        getStringOrDefault(app,"race",heros[i].appearance.race,50,"inconnu");
        getStringOrDefault(app,"eyeColor",heros[i].appearance.eyecolor,10,"inconnu");
        getStringOrDefault(app,"hairColor",heros[i].appearance.haircolor,10,"inconnu");
        if (app!=NULL){
            cJSON *height;
            height = cJSON_GetObjectItemCaseSensitive(app,"height");
            cJSON *h=NULL;
            h = cJSON_GetArrayItem(height,1);
            strncpy(heros[i].appearance.height, h->valuestring, 9);
            cJSON *weight;
            weight = cJSON_GetObjectItemCaseSensitive(app,"weight");
            cJSON *w;
            w = cJSON_GetArrayItem(weight,1);
            strncpy(heros[i].appearance.weight, w->valuestring, 9);
        }
        else {
            strncpy(heros[i].appearance.height,"inconnu",9);
            strncpy(heros[i].appearance.weight,"inconnu",9);
        }
    }
}


//affiche toutes les caracteristiques d'un hero present dans le tableau passe en parametre de taille size passee en parametre dont on saisi l'id
void afficheDetailHero(Hero heros[], int size){
    int id, ok=0;     
    printf("Saisir l'identifiant du hero (0 pour quitter) :\n>");
    scanf("%d",&id);
    while (ok==0){
        if(id==0) return;
        if (id<=size && id>0){
            ok=1;
        }
        else{
            printf("l'identifiant saisi est invalide veuillez en saisir un a nouveau :\n>");
            scanf("%d",&id);
        }
    }
    printf("\nnom : %s\nid : %d\nidapi : %d\nslug : %s\n",heros[id-1].name,heros[id-1].id, heros[id-1].idapi, heros[id-1].slug);
    printf("\nPOWERSTATS\ncombat : %d\ndurabilite : %d\nintelligence : %d\npuissance : %d\nvitesse : %d\nforce : %d\n",heros[id-1].powerstats.combat, heros[id-1].powerstats.durability, heros[id-1].powerstats.intelligence,heros[id-1].powerstats.power,heros[id-1].powerstats.speed, heros[id-1].powerstats.strength);
    printf("\nAPPARENCE\ncouleur des yeux : %s\ngenre : %s\ncouleur des cheveux : %s\ntaille : %s\nrace : %s\npoids : %s\n\n", heros[id-1].appearance.eyecolor, heros[id-1].appearance.gender, heros[id-1].appearance.haircolor, heros[id-1].appearance.height, heros[id-1].appearance.race, heros[id-1].appearance.weight);
}

//affiche cote a cote une powerstat en particulier pour permettre la comparaison entre les 2 heros et affiche si la stat d'un des heros est inconnue, ce qui empeche la comparaison
void compareStat(char nomStat[],char nom1[],char nom2[],int stat1,int stat2){
    if(stat1==-1) printf("%s : comparaison impossible la statistique est inconnue pour %s\n",nomStat, nom1);
    else if(stat2==-1) printf("%s : comparaison impossible la statistique est inconnue pour %s\n",nomStat, nom2);
    else if (stat1<stat2){
        printf("%-9s : %3d (-)\t%s:%3d (+)\n",nomStat,stat1,nomStat,stat2);
    }
    else if (stat1>stat2){
        printf("%-9s : %3d (+)\t%s : %3d (-)\n",nomStat,stat1,nomStat,stat2);;
    }
    else {
        printf("%-9s : %3d (=)\t%s : %3d (=)\n",nomStat,stat1,nomStat,stat2);
    }
}

//affiche les powerstats cote a cote 2 heros dont on saisi l'id, presents dans le tableau passe en parametre de taille size passee en parametre, et affiche si la powerstat du heros 1 est supperieure ou inferieure a celle du heros 2
void compareHero(Hero heros[], int size){
    int id1, id2, ok=0;
    printf("Sasir l'identifiant du hero 1 (0 pour quitter):\n>");
    scanf("%d",&id1);
    while (ok==0){
        if(id1==0) return;
        if (id1<=size && id1>0){
            ok=1;
        }
        else{
            printf("l'identifiant saisi est invalide veuillez en saisir un a nouveau (0 pour quitter) :\n>");
            scanf("%d",&id1);
        }
    }
    printf("Sasir l'identifiant du hero 2 :\n>");
    scanf("%d",&id2);
    ok=0;
    while (ok==0){
        if(id2==0) return;
        if (id2<=size && id2>0 && id2!=id1){
            ok=1;
        }
        else{
            printf("l'identifiant saisi est invalide veuillez en saisir un a nouveau :\n>");
            scanf("%d",&id2);
        }
    }
    printf("\n%-18s\t%s\n",heros[id1-1].name,heros[id2-1].name);
    compareStat("combat",heros[id1-1].name,heros[id2-1].name,heros[id1-1].powerstats.combat,heros[id2-1].powerstats.combat);
    compareStat("durabilite",heros[id1-1].name,heros[id2-1].name,heros[id1-1].powerstats.durability,heros[id2-1].powerstats.durability);
    compareStat("intelligence",heros[id1-1].name,heros[id2-1].name,heros[id1-1].powerstats.intelligence,heros[id2-1].powerstats.intelligence);
    compareStat("puissance",heros[id1-1].name,heros[id2-1].name,heros[id1-1].powerstats.power,heros[id2-1].powerstats.power);
    compareStat("vitesse",heros[id1-1].name,heros[id2-1].name,heros[id1-1].powerstats.speed,heros[id2-1].powerstats.speed);
    compareStat("force",heros[id1-1].name,heros[id2-1].name,heros[id1-1].powerstats.strength,heros[id2-1].powerstats.strength);
    printf("\n");
}

//affiche tous les heros presents dans le tableau passe en parametre de taille size passee en parametre correspondant au nom saisi
void rechercheNom(Hero heros[], int size){
    char nom[50];
    int n=0;
    printf("Quel est le nom du heros que vous souhaitez rechercher ? (0 pour quitter)\n>");
    scanf(" %[^\n]",nom); // %[^\n] lit tout sauf les retours a la ligne pour eviter de skip les espaces
    if (strcmp(nom,"0")==0) return;
    printf("id des heros correspondants (vous pourrez afficher plus de details grace a la fonction 3 du menu principal):\n");
    for (int i=0; i<size; i++){
        if (strcmp(nom,heros[i].name)==0){
            printf("id : %d\n", heros[i].id);
            n++;
        }
    }
    if (n==0) printf("Aucun Hero ne correspond a ce nom (veillez a respecter les espaces et majuscules)\n");
}

//modifie les caracteristique du filtre passe en parametre
void modifierFiltreDetail(int filtre[3]){
    int choix=-1, choix2, ok;
    while (choix!=0){
        choix2=-1;
        ok=0;
        afficheMenuDetailFiltre(filtre);
        scanf("%d",&choix);
        switch (choix)
        {
        case 1: 
        while (ok==0){
            printf("Quel operateur utiliser ?\n");
            printf("[1] <\n");
            printf("[2] >\n");
            printf("[3] =\n>");
            scanf("%d",&choix2);
            if (choix2<4&&choix2>0){
                ok=1;
                filtre[1]=choix2;
            }
            else printf("la valeur doit etre un entier compris entre 1 et 3\n>");
        }
        break;
        case 3:
        while (ok==0){
            printf("Quel etat souhaitez vous attribuer au filte ?\n");
            printf("[0] OFF\n");
            printf("[1] ON\n>");
            scanf("%d",&choix2);
            if (choix2<2&&choix2>=0){
                ok=1;
                filtre[0]=choix2;
            }
            else printf("la valeur doit etre un entier compris entre 0 et 1\n>");
        }
        break;
        case 2:
        while (ok==0){
            printf("saisir la valeur souhaitee\n>");
            scanf("%d",&choix2);
            if (choix2>0){
                ok=1;
                filtre[2]=choix2;
            }
            else printf("la valeur doit etre un entier superieur ou egal a 0\n>");
        }
        break;
        default: printf("la valeur doit etre un entier compris entre 0 et 3\n>");break;
        }
    }
}

//modifie l etat des filtres passes en parametres pour permettre la recherche filtree
void modifierFiltres(int filtres[6][3]){
    int choix=-1;
    while (choix !=0){
        afficheMenuFiltres();
        scanf("%d",&choix);
        switch (choix)
        {
        case 0 : break;
        case 1: modifierFiltreDetail(filtres[0]);break;
        case 2: modifierFiltreDetail(filtres[1]);break;
        case 3: modifierFiltreDetail(filtres[2]);break;
        case 4: modifierFiltreDetail(filtres[3]);break;
        case 5: modifierFiltreDetail(filtres[4]);break;
        case 6: modifierFiltreDetail(filtres[5]);break;
        default: printf("la valeur doit etre un entier compris entre 0 et 6\n"); break;
        }
    }
}

//affiche tous les heros presents dans le tableau passe en parametre de taille size passee en parametre correspondants au caracteristiques des filtres passes en parametres 
void rechercheFiltree(int filtres[6][3], Hero heros[], int size){
    Hero copieTab[size];
    int n=0;
    for (int i=0; i<size; i++){
        copieTab[i]=heros[i];
    }
    for(int i=0; i<size;i++){
        if (filtres[0][0]==1){
            if (filtres[0][1]==1){//<
                if (heros[i].powerstats.intelligence>=filtres[0][2])copieTab[i].id=-1;
            }
            else if (filtres[0][1]==2){//>
                if (heros[i].powerstats.intelligence<=filtres[0][2])copieTab[i].id=-1;
            }
            else if (filtres[0][1]==3){//=
                if (heros[i].powerstats.intelligence!=filtres[0][2])copieTab[i].id=-1;
            }
            if (heros[i].powerstats.intelligence==-1)copieTab[i].id=-1;
        }
        if (filtres[1][0]==1){
            if (filtres[1][1]==1){//<
                if (heros[i].powerstats.strength>=filtres[1][2])copieTab[i].id=-1;
            }
            else if (filtres[1][1]==2){//>
                if (heros[i].powerstats.strength<=filtres[1][2])copieTab[i].id=-1;
            }
            else if (filtres[1][1]==3){//=
                if (heros[i].powerstats.strength!=filtres[1][2])copieTab[i].id=-1;
            }
            if (heros[i].powerstats.strength==-1)copieTab[i].id=-1;
        }
        if (filtres[2][0]==1){
            if (filtres[2][1]==1){//<
                if (heros[i].powerstats.speed>=filtres[2][2])copieTab[i].id=-1;
            }
            else if (filtres[2][1]==2){//>
                if (heros[i].powerstats.speed<=filtres[2][2])copieTab[i].id=-1;
            }
            else if (filtres[2][1]==3){//=
                if (heros[i].powerstats.speed!=filtres[2][2])copieTab[i].id=-1;
            }
            if (heros[i].powerstats.speed==-1)copieTab[i].id=-1;
        }
        if (filtres[3][0]==1){
            if (filtres[3][1]==1){//<
                if (heros[i].powerstats.durability>=filtres[3][2])copieTab[i].id=-1;
            }
            else if (filtres[3][1]==2){//>
                if (heros[i].powerstats.durability<=filtres[3][2])copieTab[i].id=-1;
            }
            else if (filtres[3][1]==3){//=
                if (heros[i].powerstats.durability!=filtres[3][2])copieTab[i].id=-1;
            }
            if (heros[i].powerstats.durability==-1)copieTab[i].id=-1;
        }
        if (filtres[4][0]==1){
            if (filtres[4][1]==1){//<
                if (heros[i].powerstats.power>=filtres[4][2])copieTab[i].id=-1;
            }
            else if (filtres[4][1]==2){//>
                if (heros[i].powerstats.power<=filtres[4][2])copieTab[i].id=-1;
            }
            else if (filtres[4][1]==3){//=
                if (heros[i].powerstats.power!=filtres[4][2])copieTab[i].id=-1;
            }
            if(heros[i].powerstats.power==-1)copieTab[i].id=-1;
        }
        if (filtres[5][0]==1){
            if (filtres[5][1]==1){//<
                if (heros[i].powerstats.combat>=filtres[5][2])copieTab[i].id=-1;
            }
            else if (filtres[5][1]==2){//>
                if (heros[i].powerstats.combat<=filtres[5][2])copieTab[i].id=-1;
            }
            else if (filtres[5][1]==3){//=
                if (heros[i].powerstats.combat!=filtres[5][2])copieTab[i].id=-1;
            }
            if (heros[i].powerstats.combat==-1)copieTab[i].id=-1;
        }
    }
    printf("\n");
    printf("id des heros correspondants (vous pourrez afficher plus de details grace a la fonction 3 du menu principal):\n");
    for (int i=0; i<size; i++){
        if(copieTab[i].id!=-1){
            printf("id : %d\n",copieTab[i].id);
            n++;
        }
    }
    if (n==0)printf("aucun hero ne correspond a ces filtres\n");
    printf("\n");
}

//affiche tous les heros correspondant a un nom ou a des filtres selon leurs powerstats
void rechercheAvancee(Hero heros[], int size){
    int filtres[6][3]={{0,1,0},{0,1,0},{0,1,0},{0,1,0},{0,1,0},{0,1,0}},choix=-1; //stocke l'etat des filtres (chaque ligne correspond a une powerStat, la colonne 1 a l'etat on/off, la colonne 2 au signe de comparaison et la colonne 3 a la valeur a comparer)
    while(choix!=0){
        afficheMenuRecherche(filtres);
        scanf("%d",&choix);
        switch (choix)
        {
        case 0 : break;
        case 1: rechercheNom(heros, size); break;
        case 2 : modifierFiltres(filtres); break;
        case 3 : rechercheFiltree(filtres, heros,size); break;
        default:printf("la valeur doit etre un entier compris entre 0 et 3\n>"); break;
        }
    }
}


//affiche le menu de la gestion des favoris
void afficheMenuFav(){
    printf("+-------------------GESTION-FAVORIS-------------------+\n");
    printf("|1:AfficherFavoris                                    |\n");
    printf("|2:Ajouter un hero favoris                            |\n");
    printf("|3:Supprimer un hero favoris                          |\n");
    printf("|0:Quitter la gestion des favoris                     |\n");
    printf("+-----------------------------------------------------+\n");
    printf("Quel est votre choix ?\n>");
}

//affiche le nom et l id de chaque hero dans le tableau heros passe en parametre de taille sizefav passee en parametre dont l id est stocke dans le tableau fav passe en parametre
void afficheFav(int fav[], int sizeFav, Hero heros[]){
    if (sizeFav==0) printf("Aucun favoris\n");
    else {
        printf("\n");
        for (int i=0; i<sizeFav; i++){
            printf("id : %d\t nom : %s\n", heros[fav[i]-1].id, heros[fav[i]-1].name);
            printf("\n");
        }
    }
}

//extrait tous les elements dans un vecteur d entier de type cJSON pour les stocker dans le tableau passe en parametre de taille size passee en parametre
void rempliInt(cJSON *array, int tab[], int size, char Item[]){
    if (size!=0){
        for (int i=0; i<size; i++){
            cJSON *obj = cJSON_GetArrayItem(array,i);
            tab[i]=getIntOrDefault(obj,Item,-1);
        }
    }
}

//ajoute dans le fichier fav.json et dans le tableau fav passe en parametre un id saisi par l utilisateur
void ajoutFav(int fav[],int sizeFav, int size, cJSON *rootFav){
    int id, ok = 0;
    printf("Entrez l'id du heros a ajouter aux favoris (saisir 0 pour quitter):\n>");
    scanf("%d",&id);
    while (ok!=1){
        if (id==0) return;
        if (id<=size && id>0){
            ok=1;
        }
        else {
            printf("Entrez un id valide (saisir 0 pour quitter):\n>");
            scanf("%d",&id);
        }
        if (ok == 1){
            for(int i=0;i<sizeFav; i++){
                if (fav[i]==id){
                    ok=0;
                }
            }
            if (ok==0){
                printf("L'id saisi est deja dans les favoris, entrez en un autre (saisir 0 pour quitter):\n>");
                scanf("%d",&id);
            }
        }
    }
    //ajout dans le fichier fav.json
    cJSON *newfav=cJSON_CreateObject();
    cJSON_AddNumberToObject(newfav,"id",id);
    cJSON_AddItemToArray(rootFav,newfav);
    FILE *f = fopen("../data/fav.json","w");
    fprintf(f,"%s",cJSON_Print(rootFav));
    fclose(f);
}

//supprime dans le fichier fav.json et dans le tableau fav passe en parametre un id saisi par l utilisateur
void suppFav(int fav[],int sizeFav, int size, cJSON *rootFav){
    int id, ok = 0;
    printf("Entrez l'id du heros a supprimer des favoris (saisir 0 pour quitter):\n>");
    scanf("%d",&id);
    while (ok!=1){
        if (id>0 && id<=size){
            ok=1;
        }
        else{
            printf("Entrez un id valide (saisir 0 pour quitter):\n>");
            scanf("%d",id);
        }
        if (ok==1){
            ok = 0;
            for(int i=0; i<sizeFav;i++){
                if (fav[i]==id) ok = 1;
            }
            if (ok==0){
                printf("L'id saisi n'est pas dans les favoris, entrez en un autre (0 POUR quitter):\n");
                scanf("%d",&id);
            }
        }
    }
    for (int i=0;i<sizeFav;i++){
        cJSON *obj = cJSON_GetArrayItem(rootFav,i);
        if (getIntOrDefault(obj,"id",-1)==id) cJSON_DeleteItemFromArray(rootFav,i);
    }
    FILE *f = fopen("../data/fav.json","w");
    fprintf(f,"%s",cJSON_Print(rootFav));
    fclose(f);
}


//permet la gestions des heros favoris de l utilisateur
void gestionFav(Hero heros[],int size){
    int choix = -1;
    char bufferFav[100000];
    int sizeFav;
    cJSON *rootFav = NULL;
    
    
    while (choix!=0){
        //extraction des id presents dans fav.json afin de permettre leur manipulation
        if (readTextFile("../data/fav.json",bufferFav,100000)){
            rootFav=cJSON_Parse(bufferFav);
        }
        if (rootFav != NULL) sizeFav = cJSON_GetArraySize(rootFav);
        int fav[sizeFav];
        rempliInt(rootFav, fav, sizeFav,"id");

        afficheMenuFav();
        scanf("%d",&choix);
        switch (choix)
        {
        case 1: afficheFav(fav, sizeFav, heros); break;
        case 2: ajoutFav(fav, sizeFav, size, rootFav); break;
        case 3: suppFav(fav, sizeFav, size, rootFav); break;
        default:printf("la valeur doit etre un entier compris entre 0 et 3\n>"); break;
        }
    }
}

//renvoie le meilleur score present dans le tableau scores passe en parametre de taille sizescores passee en parametre
int getMaxScore(int scores[],int sizescores){
    if (sizescores==0) return 0;
    int max = scores[0];
    for(int i=1; i<sizescores; i++){
        if (max<scores[i]) max=scores[i];
    }
    return max;
}

//affiche le menu du quizz ainsi que le meilleur score
void afficheMenuQuizz(int best){
    printf("+------------------------QUIZZ------------------------+\n");
    printf("|1: Lancer le Quizz                                   |\n");
    printf("|2: Afficher les scores                               |\n");
    printf("|0: Quitter le quizz                                  |\n");
    printf("+-----------------------------------------------------+\n");
    printf("|Meilleur score:%d/5                                   |\n",best);
    printf("+-----------------------------------------------------+\n");
    printf("Quel est votre choix ?\n>");
}
//ajoute le score passe en parametre dans le fichier scores.json
void ajouterScore(int score, cJSON *rootScore){
    cJSON *newscore=cJSON_CreateObject();
    cJSON_AddNumberToObject(newscore,"score",score);
    cJSON_AddItemToArray(rootScore,newscore);
    FILE *f = fopen("../data/scores.json","w");
    fprintf(f,"%s",cJSON_Print(rootScore));
    fclose(f);
}

//lance un partie du quizz et ajoute le score dans scores.json
//une partie est composée de 5 question, les caracteristiques physiques d un heros sont affichees et 4 heros sont proposés l utilisateur doit deviner a quel hero correspondent les powerstats
void nouveauQuizz(Hero heros[],int size, cJSON *rootScore){
    int score=0, i=0, choix=1,ok1, ok2, ok3, ok4,okrep, posGagnante;
    Hero gagnant, h1, h2, h3, reponses[4],temp;

    while (choix!=0 && i<5){
        ok1 = 0;
        ok2 = 0;
        ok3 = 0;
        ok4 = 0;
        okrep =0;
        while (ok1==0){
        gagnant=heros[rand() % size];
            if (strcmp(gagnant.appearance.eyecolor,"inconnu")!=0&&strcmp(gagnant.appearance.gender,"inconnu")!=0&&strcmp(gagnant.appearance.haircolor,"inconnu")!=0&&strcmp(gagnant.appearance.height,"inconnu")!=0&&strcmp(gagnant.appearance.race,"inconnu")!=0&&strcmp(gagnant.appearance.weight,"inconnu")!=0){
                ok1=1;
                reponses[0]=gagnant;
            }
        }
        while (ok2==0){
            h1=heros[rand() % size];
            if (gagnant.id!=h1.id){
                ok2=1;
                reponses[1]=h1;
            }
        }
        while (ok3==0){
            h2=heros[rand() % size];
            if (h2.id!=h1.id&&gagnant.id!=h2.id){
                ok3=1;
                reponses[2]=h2;
            }
        }
        while (ok4==0){
            h3=heros[rand() % size];
            if (h1.id!=h3.id&&h2.id!=h3.id&&gagnant.id!=h3.id){
                ok4=1;
                reponses[3]=h3;
            }
        }
        //mets la positions du choix gagnant au hasard
        posGagnante=rand()%4;
        if (posGagnante!=0){
            temp=reponses[posGagnante];
            reponses[posGagnante]=reponses[0];
            reponses[0]=temp;
        }
        printf("Question %d/5\tscore:%d\n\n",i+1,score);
        printf("A quel heros appartient ces caracteristiques ? \n");
        printf("Couleur des yeux : %s\ngenre : %s\ncouleur des cheveux : %s\ntaille : %s\nrace : %s\npoids : %s\n\n\n",gagnant.appearance.eyecolor,gagnant.appearance.gender,gagnant.appearance.haircolor,gagnant.appearance.height,gagnant.appearance.race,gagnant.appearance.weight);
        printf("[1]%-18s\t[2]%s\n",reponses[0].name,reponses[1].name);
        printf("[3]%-18s\t[4]%s\n",reponses[2].name,reponses[3].name);
        printf("[0]-quitter\n>");
        scanf("%d",&choix);
        while (okrep==0){
            if (choix<5&&choix>=0) okrep=1;
            else{
                printf("Veuillez saisir un entier compris entre 0 et 4\n>");
                scanf("%d",&choix);
            }
        }
        if (choix!=0){
            if (choix==posGagnante+1){
                score+=1;
                printf("\nBONNE REPONSE ! (%s)\n", gagnant.name);
            }
            else{
                printf("\nMauvaise reponse, il fallait donner la reponse suivante : %s\n",gagnant.name);
            }
        }
        i+=1;
    }
    printf("\npartie terminee, score : %d/5\n\n",score);
    ajouterScore(score, rootScore);
}

//affiche tous les scores des parties precedentes presents dans le tableau scores passe en parametre de taille sizescores passee en parametre
void afficheScore(int scores[], int sizeScores){
    if (sizeScores>0){
        printf("\n");
        for(int i=0;i<sizeScores;i++){
            printf("Partie %d : %d/5\n", i+1, scores[i]);
        }
        printf("\n");
    }
    else{
        printf("\n");
        printf("Aucun score connu\n");
        printf("\n");
    }
}

//permet a l utilisateur de faire un quizz sur les caracteristiques physiques des heros et stocke chaque score dans scores.json
void quizz(Hero heros[],int size){
    int choix=-1,best;
    char bufferScores[100000];
    int sizeScores;
    cJSON *rootScores = NULL;
    while (choix!=0){
        //extraction des donnees de scores.json
        if (readTextFile("../data/scores.json",bufferScores,100000)){
        rootScores=cJSON_Parse(bufferScores);
        }
        if (rootScores != NULL) sizeScores = cJSON_GetArraySize(rootScores);
        int scores[sizeScores];
        rempliInt(rootScores, scores, sizeScores,"score");

        best=getMaxScore(scores,sizeScores);
        afficheMenuQuizz(best);
        scanf("%d",&choix);
        switch (choix)
        {
        case 1: nouveauQuizz(heros,size,rootScores); break;
        case 2 : afficheScore(scores,sizeScores); break;
        case 0 : break;
        default:printf("la valeur doit etre un entier compris entre 0 et 2\n>"); break;
        }
    }
}

int main(){
    //initialisation pour la generation de nombre aleatoire
    srand(time(NULL));

    int choix = -1;
    char buffer[100000];
    int size;
    cJSON *root = NULL;

    //extraction des donnees de SuperHeros.json
    if (readTextFile("../data/SuperHeros.json",buffer,100000)){
        root=cJSON_Parse(buffer);
        }
    if (root != NULL) size = cJSON_GetArraySize(root);
    Hero heros[size];
    rempliHeros(root, heros, size);


    while(choix!=0){
        afficheMenuPrincipal();
        scanf("%d",&choix);
        switch (choix)
        {
        case 0 : break;
        case 1 : afficheHeros(heros,size); break;
        case 2 : afficheDetailHero(heros, size); break;
        case 3 : rechercheAvancee(heros, size);break;
        case 4 : compareHero(heros,size);break;
        case 5 : gestionFav(heros, size); break;
        case 6 : quizz(heros,size); break;
        case 7 : afficheLiens(); break;
        default: printf("la valeur doit etre un entier compris entre 0 et 6\n>"); break;
        }

    }
    return 0;
}