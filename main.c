#include <allegro.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Math.h>
#define PI 3.1415926535897932384626433832795

typedef struct players
{
    char nom[16];
    int score;
    int head;
    char role;
    //position initiale du logo [1600, 900]
    int position_y;
    int position_x;

    //playing cards
    BITMAP * cards[7];
    int card_type[7];

    //debuff cards
    BITMAP * cards_debuff[4];
    int card_type_debuff[4];
    int cards_debuff_place;

}t_players;

typedef struct pioche
{
    BITMAP * Elements[71];
    int card_type[71];
    int Sommet;
    int nbPlaces;
    int empty;
}Pioche;


///prototypes
///===========================================================================GENERAL
void delay(int number_of_milis);
///===========================================================================PILES
void initPile (Pioche* PtPile);
void empiler(Pioche * ptTour, BITMAP* card[19], int occurences[19][2]);
void depiler(Pioche * ptTour);
///===========================================================================BITMAPS
void load_Bitmap();
void destruction(BITMAP* images[30], BITMAP* tete[10], BITMAP* cartes[19], BITMAP* or[5], BITMAP* sprite[15]);
///===========================================================================MENU, SAISIE
void menus(BITMAP* images[30], BITMAP* tete[10], BITMAP* cartes[19], BITMAP* or[5], BITMAP* sprite[15]);
void newgame(BITMAP* images[30], BITMAP* tete[10], BITMAP* cartes[19], BITMAP* or[5], BITMAP* sprite[15]);
void playerselection(int players, BITMAP* images[30], BITMAP* tete[10], BITMAP* cartes[19], BITMAP* or[5], BITMAP* sprite[15]);
void saisie(FONT* averia, BITMAP* images[30], t_players player[5], int playernumber, int i, int x, int y);
int logo(int mouse_x, int mouse_y);
///===========================================================================INTERFACE ET JEU
void initialisation(BITMAP* or[5], int tab_or[4], int occurences[19][2], int tab[7][5], int numberofplayers, t_players player[5]);
void initialisation2(int tab_or[4], int occurences[19][2], t_players player[5]);
void interface(t_players player[5], int numberofplayers, FONT* averia, BITMAP* images[30], BITMAP* tete[10], BITMAP* cartes[19], BITMAP* or[5], BITMAP* sprite[15], int game_saved);
void checkcard(t_players player[5], int turn, Pioche *pioche, BITMAP* buffer, BITMAP* temp, int c);
void afficheplateau(int tab[7][5] ,  BITMAP * cartes[17],  BITMAP * buffer);
int verification_placement(int tab[7][5], int x, int y, int round, int type);
void animation(BITMAP* buffer, BITMAP* temp, int c, t_players player[5], int turn, int j);
void carte_role(BITMAP* temp, BITMAP* images[30], t_players player[5], int turn);
void affichage_tour(FONT* averia, BITMAP* buffer, BITMAP* images[30], BITMAP* tete[10], BITMAP* or[5], t_players player[5], int numberofplayers, int turn, int tab_or[4]);
///===========================================================================FIN DU JEU
int continuer_demande(BITMAP* images[30], FONT* averia);
void finpartie_gameover(BITMAP*images[30],t_players player[5]);
void finpartie_saboteur(t_players player[5], int numberofplayers, BITMAP* sprite[15], FONT* averia);
void finpartie_mineur(t_players players[5], int numberofplayers, BITMAP* sprite[15]);
///===========================================================================DRAG AND DROP DEPLACEMENT
int draganddrop_galerie(BITMAP * temp, BITMAP* images[30], t_players player[5], int turn, int tab[7][5], int numero, int round, int debuff);
int draganddrop_action(BITMAP *temp, t_players player[5], int turn, int numero, int numberofplayers, int tab[7][5], BITMAP* images[30], BITMAP* or[4], int tab_or[4]);
void draganddrop_retours(BITMAP* temp,t_players player[5], int turn, int numero);
int draganddrop_tunnel(BITMAP *temp, t_players player[5], int turn, int numero, int tab[7][5]);
void deplacement_logo(BITMAP* temp, BITMAP* tete[10], t_players player[5], int tab[7][5], int oldturn);
///===========================================================================SAUVEGARDE
void cumul_score(t_players player[5], int numberofplayers);
int loadgame(t_players player[5], int numberofplayers, Pioche * pioche, int tab[7][5]);
int save_game(t_players player[5], int numberofplayers, Pioche * pioche, int tab[7][5]);


///===========================================================================GENERAL

int main()
{

    ///initialisations
    srand(time(NULL));
    allegro_init();
    install_timer();
    install_mouse();
    install_keyboard();
    install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL);
    set_color_depth(32);


    ///ouverture de la fenetre de jeu
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, 1600, 900, 0, 0);
    set_window_title("Saboteur");

    set_close_button_callback(allegro_exit);


    ///appel a la fonction load _bitmap
    load_Bitmap();

    return 0;
}END_OF_MAIN();


void delay(int number_of_milis)
{
    //fonction de internert site dans les sources
    // Storing start time
    clock_t start_time = clock();

    // looping till required time is not achieved
    while (clock() < start_time + number_of_milis)
        ;
}


///===========================================================================PILES

void initPile (Pioche* PtPile)
{
    PtPile->Sommet = 0;
    PtPile->nbPlaces = 71;
}


void empiler(Pioche * ptTour, BITMAP* card[19], int occurences[19][2])
{
    int alea=0;
    int sortie=0;

    ///boucle
    while(sortie!=1)//boucle qui oblige d'affecter un nombre aleatoire
    {
        alea=(rand() % (18-1 + 1) + 1);//nombre aletoire

        if(occurences[alea][0]>=1)//si il reste des occurences
        {
            ptTour->Elements[ptTour->Sommet] = card[alea];//le nouvel element d'aindice 'sommet' prends pour image une carte aleatoire
            ptTour->card_type[ptTour->Sommet]=occurences[alea][1];//sauvegarde du jnumero (type) de la carte dans la pile

            occurences[alea][0]--;//decrementation du nombre d'eoccurences
            (ptTour->Sommet)++;//somment s'incremente
            printf("%d\n\n", ptTour->Sommet);
            sortie=1;//fin de la boucle
        }

        else sortie=0;// la boucle recommence avec un nouveau nombre aleatoire
    }//fin du while
}


void depiler(Pioche * ptTour)
{
    if (ptTour->Sommet>0)
    {
        ptTour->Elements[ptTour->Sommet]=NULL;//suprime l'element
        ptTour->Sommet--;
    }
    else
        ptTour->empty=1;
}

///===========================================================================BITMAPS

void load_Bitmap()
{

    int i;
    BITMAP* images[30];
    BITMAP* tete[10];
    BITMAP* cartes[19];
    BITMAP* or[5];
    BITMAP* sprite[15];

    for(i=0; i<30; i++)
        images[i]=NULL;

    for(i=0; i<10; i++)
        tete[i]=NULL;

    for(i=0; i<18; i++)
        cartes[i]=NULL;

    for(i=0; i<5; i++)
        or[i]=NULL;



    images[0] = load_bitmap("cursor.BMP", NULL);
    images[1] = load_bitmap("image_menu.bmp", NULL);
    images[2] = load_bitmap("image_nombre_joueurs.BMP", NULL);
    images[3] = load_bitmap("image_nom_joueurs.BMP", NULL);
    images[4] = load_bitmap("heads.BMP", NULL);
    images[5] = load_bitmap("start.bmp", NULL);
    images[6] = load_bitmap("plateau.BMP", NULL);
    images[7] = load_bitmap("back.BMP", NULL);
    images[8] = load_bitmap("miner.BMP", NULL);
    images[9] = load_bitmap("Saboteur.BMP", NULL);
    images[10] = load_bitmap("back_miner.BMP", NULL);
    images[11] = load_bitmap("back.BMP", NULL);
    images[12] = load_bitmap("gameover.BMP", NULL);
    images[13] = load_bitmap("demande_fin.BMP", NULL);

    for(i=0; i<14; i++)
    {
        if(images[i]==NULL)
        {
            printf("images NULL %d", i);
            exit(0);
        }
    }


    ///tetes
    tete[1] = load_bitmap("cow.bmp", NULL);
    tete[2] = load_bitmap("creeper.bmp", NULL);
    tete[3] = load_bitmap("enderman.bmp", NULL);
    tete[4] = load_bitmap("pig.bmp", NULL);
    tete[5] = load_bitmap("sheep.bmp", NULL);
    tete[6] = load_bitmap("spider.bmp", NULL);
    tete[7] = load_bitmap("stelet.bmp", NULL);
    tete[8] = load_bitmap("steve.bmp", NULL);
    tete[9] = load_bitmap("zombie.bmp", NULL);

    for(i=1; i<=9; i++)
    {
        if(tete[i]==NULL)
        {
            printf("tete NULL %d", i);
            exit(0);
        }
    }


    ///cartes
    cartes[1] = load_bitmap("4.bmp", NULL);
    cartes[2] = load_bitmap("straight.bmp", NULL);
    cartes[3] = load_bitmap("T-1.bmp", NULL);
    cartes[4] = load_bitmap("T-2.bmp", NULL);
    cartes[5] = load_bitmap("end.bmp", NULL);

    //action casses
    cartes[6] = load_bitmap("tunnel_broken.bmp", NULL);
    cartes[7] = load_bitmap("All_broken.bmp", NULL);

    //action reparrees
    cartes[8] = load_bitmap("torch_pick.bmp", NULL);
    cartes[9] = load_bitmap("torch_cart.bmp", NULL);
    cartes[10] = load_bitmap("cart_pick.bmp", NULL);

    //action torche
    cartes[11] = load_bitmap("torch_lit.bmp", NULL);
    cartes[12] = load_bitmap("torch_broken.bmp", NULL);

    //action pioche
    cartes[13] = load_bitmap("pickaxe_.bmp", NULL);
    cartes[14] = load_bitmap("pickaxe_broken.bmp", NULL);

    //action minecart
    cartes[15] = load_bitmap("minecart_final.bmp", NULL);
    cartes[16] = load_bitmap("minecart_card_broken.bmp", NULL);

    //action map
    cartes[17] = load_bitmap("map.bmp", NULL);
    //action depioche
    cartes[18] = load_bitmap("depiocher.BMP", NULL);

    for(i=1; i<=18; i++)
    {
        if(cartes[i]==NULL)
        {
            printf("carte NULL %d", i);
            exit(0);
        }
    }



    ///or
    or[1] = load_bitmap("1_gold.bmp", NULL);
    or[2] = load_bitmap("2_gold.bmp", NULL);
    or[3] = load_bitmap("3_gold.bmp", NULL);
    or[4] = load_bitmap("back_gold.bmp", NULL);


    for(i=1; i<=4; i++)
    {
        if(or[i]==NULL)
        {
            printf("or NULL %d", i);
            exit(0);
        }
    }

    ///sprites fin d'ecran
    sprite[1] = load_bitmap("vachesprite.bmp", NULL);
    sprite[2] = load_bitmap("creepersprite.bmp", NULL);
    sprite[3] = load_bitmap("endermansprite.bmp", NULL);
    sprite[4] = load_bitmap("cochonsprite.bmp", NULL);
    sprite[5] = load_bitmap("moutonsprite.bmp", NULL);
    sprite[6] = load_bitmap("araigneesprite.bmp", NULL);
    sprite[7] = load_bitmap("squelettesprite.bmp", NULL);
    sprite[8] = load_bitmap("stevesprite.bmp", NULL);
    sprite[9] = load_bitmap("zombiesprite.bmp", NULL);
    sprite[10] = load_bitmap("caillousprite.bmp", NULL);
    sprite[11] = load_bitmap("pepitesprite.bmp", NULL);
    sprite[12] = load_bitmap("fonddecran1.bmp", NULL);
    sprite[13] = load_bitmap("fonddecran2.bmp", NULL);
    sprite[14] = load_bitmap("wagonsprite.bmp", NULL);


    for(i=1; i<=14; i++)
    {
        if(sprite[i]==NULL)
        {
            printf("sprite NULL %d", i);
            exit(0);
        }
    }

    menus(images, tete, cartes, or, sprite);
}


void destruction(BITMAP* images[30], BITMAP* tete[10], BITMAP* cartes[19], BITMAP* or[5], BITMAP* sprite[15])
{

    int i;
    ///destruction

    for(i=0; i<=13; i++)
    {
        destroy_bitmap(images[i]);
    }

    for(i=1; i<=18; i++)
    {
        destroy_bitmap(cartes[i]);
    }

    for(i=1; i<=9; i++)
    {
        destroy_bitmap(tete[i]);
    }

    for(i=1; i<=4; i++)
    {
        destroy_bitmap(or[i]);
    }

    for(i=1; i<=14; i++)
    {
        destroy_bitmap(sprite[i]);
    }

}


///===========================================================================MENU, SAISIE

void menus(BITMAP* images[30], BITMAP* tete[10], BITMAP* cartes[19], BITMAP* or[5], BITMAP* sprite[15])
{
    ///fonts
    FONT* averia = load_font("averia_18_B.PCX", NULL, NULL);

    ///variable pour loadgame
    t_players player[5];
    int numberofplayers=0, sortie=0;

    ///BITMAPS
    BITMAP* buffer = create_bitmap(SCREEN_W, SCREEN_H);

    ///GAME LOOP
while(sortie==0)
{
    draw_sprite(buffer, images[1], 0, 0);///fond decran
    draw_sprite(buffer, images[0], mouse_x-6, mouse_y);///souri

    ///new game
    if(mouse_x > 75 && mouse_x < 333 && mouse_y > 425 && mouse_y < 455 && mouse_b==1)
    {
        mouse_b=0;
        delay(200);
        newgame(images, tete, cartes, or, sprite);
        delay(200);
    }

    ///continue
    if(mouse_x > 75 && mouse_x < 333 && mouse_y > 485 && mouse_y < 515 && mouse_b==1)
    {
        interface(player, numberofplayers, averia, images, tete, cartes, or, sprite, 1);
    }

    ///options
    if(mouse_x > 75 && mouse_x < 333 && mouse_y > 545 && mouse_y < 575 && mouse_b==1)
    {

    }

    ///scores
    if(mouse_x > 75 && mouse_x < 333 && mouse_y > 605 && mouse_y < 635 && mouse_b==1)
    {

    }

    ///exit
    if(mouse_x > 75 && mouse_x < 333 && mouse_y > 820 && mouse_y < 850 && mouse_b==1)
    {
        destruction(images, tete, cartes,or, sprite);
        exit(0);
    }

    ///Images
    draw_sprite(screen, buffer, 0, 0);
    clear(buffer);
}

    ///destruction
    destroy_bitmap(buffer);
    destruction(images, tete, cartes, or, sprite);

}END_OF_FUNCTION(menus)


void newgame(BITMAP* images[30], BITMAP* tete[10], BITMAP* cartes[19], BITMAP* or[5], BITMAP* sprite[15])
{
    ///BITMAPS
    BITMAP* buffer = create_bitmap(SCREEN_W, SCREEN_H);
    int sortie=0;
    mouse_b=0;
    ///GAME LOOP
while(sortie==0)
{
    draw_sprite(buffer, images[2], 0, 0);
    draw_sprite(buffer, images[0], mouse_x-6, mouse_y);

    ///2 joueurs
    if(mouse_x > 75 && mouse_x < 333 && mouse_y > 425 && mouse_y < 455 && mouse_b==1)
    {
        mouse_b=0;
        delay(200);
        playerselection(2, images, tete, cartes, or, sprite);
        break;
    }

    ///3 joueurs
    if(mouse_x > 75 && mouse_x < 333 && mouse_y > 485 && mouse_y < 515 && mouse_b==1)
    {
        mouse_b=0;
        delay(200);
        playerselection(3, images, tete, cartes, or, sprite);
        break;
    }

    ///4 joueurs
    if(mouse_x > 75 && mouse_x < 333 && mouse_y > 545 && mouse_y < 575 && mouse_b==1)
    {
        mouse_b=0;
        delay(200);
        playerselection(4, images, tete, cartes, or, sprite);
        break;
    }

    ///retour
    if(mouse_x > 75 && mouse_x < 333 && mouse_y > 605 && mouse_y < 635 && mouse_b==1)
    {
        mouse_b=0;
        delay(200);
        //menus(images, tete, cartes, or);
        break;
    }

    ///exit
    if(mouse_x > 75 && mouse_x < 333 && mouse_y > 820 && mouse_y < 850 && mouse_b==1)
    {
        destruction(images, tete, cartes,or, sprite);
        exit(0);
    }

    ///Images

    draw_sprite(screen, buffer, 0, 0);
    clear(buffer);
}

    ///destruction
    destroy_bitmap(buffer);


}END_OF_FUNCTION(newgame)


int logo(int mouse_x, int mouse_y)
{
    int head=0;
    if(mouse_x > 25 && mouse_x < 146 && mouse_y > 420 && mouse_y < 541)
    {
        head=1;
    }

    if(mouse_x > 147 && mouse_x < 267 && mouse_y > 420 && mouse_y < 541)
    {
        head=2;
    }

    if(mouse_x > 268 && mouse_x < 389 && mouse_y > 420 && mouse_y < 541)
    {
        head=3;
    }

    if(mouse_x > 25 && mouse_x < 146 && mouse_y > 542 && mouse_y < 663)
    {
        head=4;
    }

    if(mouse_x > 147 && mouse_x < 267 && mouse_y > 542 && mouse_y < 663)
    {
        head=5;
    }

    if(mouse_x > 268 && mouse_x < 389 && mouse_y > 542 && mouse_y < 663)
    {
        head=6;
    }

    if(mouse_x > 25 && mouse_x < 146 && mouse_y > 664 && mouse_y < 785)
    {
        head=7;
    }

    if(mouse_x > 147 && mouse_x < 267 && mouse_y > 664 && mouse_y < 785)
    {
        head=8;
    }

    if(mouse_x > 268 && mouse_x < 389 && mouse_y > 664 && mouse_y < 785)
    {
        head=9;
    }
    //sinon retourn 0 et la fonction se repete
    return head;
}END_OF_FUNCTION(logo)


void playerselection(int players, BITMAP* images[30], BITMAP* tete[10], BITMAP* cartes[19], BITMAP* or[5], BITMAP* sprite[15])
{

    ///structure des joueurs et variables
    t_players player[5];
    int i,j,playernumber,x,y,cleared=0;

    ///BITMAPS
    BITMAP* buffer = create_bitmap(SCREEN_W, SCREEN_H);

    ///fonts
    FONT* averia = load_font("averia_18_B.PCX", NULL, NULL);

    ///initialisations
    strcpy(player[1].nom,"Player 1");
    strcpy(player[2].nom,"Player 2");
    strcpy(player[3].nom,"Player 3");
    strcpy(player[4].nom,"Player 4");

    for(i=1; i<5; i++)
    {
        player[i].head=0;
        player[i].score=0;
        player[i].cards_debuff_place=0;
        player[i].position_y=525;
        player[i].position_x=750;

        for(j=0; j<6; j++)
            player[i].card_type[j]=0;

        for(j=0; j<3; j++)
            player[i].card_type_debuff[j]=0;
    }

    mouse_b=0;
///GAME LOOP
while(!key[KEY_ESC])//sortie selement si exit ou touts les joueurs ont finit la selection
{
    clear_keybuf();//libere le buffer du clavier

    ///affichage ici pour avoir un ordre d'affichage et ne pas avoir de trensparence et flickering
    draw_sprite(buffer, images[3], 0, 0);
    draw_sprite(buffer, images[0], mouse_x-6, mouse_y);

    ///affichage des noms
    textprintf_ex(buffer, averia, 80, 430, makecol(217,179,64), -1, "%s", player[1].nom);
    textprintf_ex(buffer, averia, 80, 488, makecol(217,179,64), -1, "%s", player[2].nom);

    if(players>=3)//si il y a 3 joueurs ou plus
        textprintf_ex(buffer, averia, 80, 548, makecol(217,179,64), -1, "%s", player[3].nom);

    if(players>=4)//si il y a 4 joueurs ou plus
        textprintf_ex(buffer, averia, 80, 608, makecol(217,179,64), -1, "%s", player[4].nom);


    /// joueur 1
    if(mouse_x > 75 && mouse_x < 333 && mouse_y > 425 && mouse_y < 455 && mouse_b==1)
    {
        //intialisations
        draw_sprite(buffer, images[3], 0, 0);//affiche l'ecran pour supprimer la saisie precedente
        x=80; i=0; playernumber=1; y=430;//variables position, compteur, le numero du joueur
        player[playernumber].head=0;

        mouse_b=0;
        //sous programme de saisie
        saisie(averia, images, player, playernumber, i, x, y);
        mouse_b=0;
        delay(200);
    }

    /// joueur 2
    if(mouse_x > 75 && mouse_x < 333 && mouse_y > 485 && mouse_y < 515 && mouse_b==1)
    {
        draw_sprite(buffer, images[3], 0, 0);
        x=80; i=0; playernumber=2; y=488;//variables
        player[playernumber].head=0;

        mouse_b=0;
        //sous programme de saisie
        saisie(averia, images, player, playernumber, i, x, y);
        mouse_b=0;
        delay(200);
    }

    /// joueur 3
    if(mouse_x > 75 && mouse_x < 333 && mouse_y > 545 && mouse_y < 575 &&  players>=3 && mouse_b==1)
    {
        draw_sprite(buffer, images[3], 0, 0);
        x=80; i=0; playernumber=3; y=548;//variables
        player[playernumber].head=0;

        mouse_b=0;
        //sous programme de saisie
        saisie(averia, images, player, playernumber, i, x, y);
        mouse_b=0;
        delay(200);
    }

    /// joueur 4
    if(mouse_x > 75 && mouse_x < 333 && mouse_y > 605 && mouse_y < 635 && players>=4 && mouse_b==1)
    {
        draw_sprite(buffer, images[3], 0, 0);
        x=80; i=0; playernumber=4; y=608;//variables
        player[playernumber].head=0;

        mouse_b=0;
        //sous programme de saisie
        saisie(averia, images, player, playernumber, i, x, y);
        mouse_b=0;
        delay(200);
    }

    /// retour
    if(mouse_x > 75 && mouse_x < 333 && mouse_y > 665 && mouse_y < 695 && mouse_b==1)
    {
        mouse_b=0;
        delay(200);
        break;
    }

    ///exit
    if(mouse_x > 75 && mouse_x < 333 && mouse_y > 820 && mouse_y < 850 && mouse_b==1)
    {
        destruction(images, tete, cartes,or,sprite);
        exit(0);
    }

    ///verification de la condition de sortie, si tout les joueurs ont effectuer leur saisie
    cleared=0;
    for(i=1; i<players+1; i++)
    {
        if(player[i].head>0)
            cleared++;
    }

    ///appel a la fonction suivante
    if(cleared==players)
    {
        interface(player, players, averia, images, tete, cartes, or, sprite, 0);
        mouse_b=0;
        delay(200);
        break;
    }


    ///Images
    draw_sprite(screen, buffer, 0, 0);
    clear(buffer);
}//fin du while


}END_OF_FUNCTION(playerselection)


void saisie(FONT* averia, BITMAP* images[30], t_players player[5], int playernumber, int i, int x, int y)
{
    int clignotant=0;
    BITMAP* buffer = create_bitmap(SCREEN_W, SCREEN_H);
    clear_bitmap(buffer);
    player[playernumber].nom[0]='\0';

    //saisie du nom
    while(!key[KEY_ENTER] && !key[KEY_SPACE] && !key[KEY_ESC] && i<15)//tant que enter n'est pas ppuyer ou la longeur ne depasse pas 15 carcteres
    {
        draw_sprite(buffer,images[3],0,0);
        textprintf_ex(buffer, averia, x, y, makecol(217,179,64), -1, "%s", player[playernumber].nom);//affiche le carctere en live
        draw_sprite(screen, buffer, 0, 0);//refresh l'ecran
        while(!keypressed())
        {
            draw_sprite(buffer,images[3],0,0);
            textprintf_ex(buffer, averia, x, y, makecol(217,179,64), -1, "%s", player[playernumber].nom);//affiche le carctere en live
            if(clignotant%30==0)
            {
                textprintf_ex(buffer, averia, x+i*12, y, makecol(217,179,64), -1, "|");//affiche le carctere en live
            }
            draw_sprite(screen, buffer, 0, 0);//refresh l'ecran
            clear_bitmap(buffer);
            clignotant++;
        }

        player[playernumber].nom[i]=(readkey()&0xFF);//lecture du carctere
        if(key[KEY_ENTER] || key[KEY_SPACE])//si c'est enter
            player[playernumber].nom[i]='\0';//le carctere est nul

        else//sinon
            player[playernumber].nom[i+1]='\0';//le prochain est la fin de la chaine de carcteres


        draw_sprite(buffer,images[3],0,0);
        textprintf_ex(buffer, averia, x, y, makecol(217,179,64), -1, "%s", player[playernumber].nom);//affiche le carctere en live
        draw_sprite(screen, buffer, 0, 0);//refresh l'ecran
        i++;//prochain caractere
    }

    mouse_b=0;
    //choix du logo
    do
    {
        if(mouse_b==1)//si appui de la sourri
            player[playernumber].head=logo(mouse_x, mouse_y);
        draw_sprite(buffer, images[2], 0, 0);
        draw_sprite(buffer, images[4], 25, 420);
        draw_sprite(buffer, images[0], mouse_x-6, mouse_y);
        draw_sprite(screen, buffer, 0, 0);//refresh l'ecra
        clear(buffer);
    }while(player[playernumber].head==0);//boucle tant que l'utilisateur n'a pas chosiit de logo
    mouse_b=0;//pour ne pas cliquer a travers l'image
}


///===========================================================================INTERFACE ET JEU

void checkcard(t_players player[5], int turn, Pioche *pioche, BITMAP* buffer, BITMAP* temp, int c)
{
    int i;
    for(i=0; i<6; i++)
    {
        if(player[turn].card_type[i]==0)
        {
            printf("avant modif: %d\n", pioche->Sommet);
            player[turn].cards[i]=pioche->Elements[pioche->Sommet];//affection de l'imege de la carte
            player[turn].card_type[i] = pioche->card_type[pioche->Sommet];//affectation du type de la carte
            depiler(&*pioche);//depile l'element
            printf("apres modif: %d\n", pioche->Sommet);
            animation(buffer, temp, c, player, turn, i);//sous programme d'animation des cartes
        }
    }
}


void animation(BITMAP* buffer, BITMAP* temp, int c, t_players player[5], int turn, int j)
{

    float x1,y1;

    x1=545; y1=510;
    temp=create_bitmap(SCREEN_W,SCREEN_H);
    blit(screen,temp,0,0,0,0,SCREEN_W,SCREEN_H);//copie de l'ecran pour garder toutes les saisies

    while(y1>=70)//tant que la crate n'est pas a la bonne position
    {
        draw_sprite(buffer, temp, 0, 0);
        draw_sprite(buffer, player[turn].cards[j], x1, y1);
        draw_sprite(screen, buffer, 0, 0);
        clear(buffer);
        y1=y1-10;
        x1=x1-(12.1-j*2.5);//trajectoire des cartes pour s'aligner
    }
    draw_sprite(buffer, player[turn].cards[j], 11+j*110, 70);//affiche chaque carte

    destroy_bitmap(temp);
}


void carte_role(BITMAP* temp, BITMAP* images[30], t_players player[5], int turn)
{
    BITMAP* buffer = create_bitmap(SCREEN_W, SCREEN_H);
    int i=0;
    float x_=545;
    float y_=510;

    ///deplacement de la carte
    while(y_>=70)
    {
        draw_sprite(buffer, temp, 0, 0);//dessine le'cran sauvegarder
        draw_sprite(buffer, images[10], x_, y_);//dessine la carte avec les coordonnes au centre de la carte

        x_=x_-(12.1-6*2.5);//trajectoire des cartes pour s'aligner
        y_=y_-10;

        draw_sprite(screen, buffer, 0, 0);
        clear(buffer);
        i++;
    }
    ///affichage de la carte pendans quelques seocoonde
    while(i<200)
    {
        draw_sprite(buffer, temp, 0, 0);//dessine le'cran sauvegarder
        if(player[turn].role=='M')
            draw_sprite(buffer, images[8], 671, 70);//dessine la carte avec les coordonnes au centre de la carte
        if(player[turn].role=='S')
            draw_sprite(buffer, images[9], 671, 70);//dessine la carte avec les coordonnes au centre de la carte
        draw_sprite(screen, buffer, 0, 0);
        clear(buffer);
        i++;
    }
    clear(temp);
}


int carte_action_casse(t_players player[5], int turn, BITMAP* cartes[19])
{
    ///ce systeme "rends la monaie", il rendra les cartes action nom utilisees car ce n'est pas precise dans les regles
    int i, j;
    for(i=0; i<3; i++)
    {
        ///==========================carte tout casser=============================================================
        if(player[turn].card_type_debuff[i]==7)
        {
            for(j=0; j<3; j++)
            {
                //torch pick repaired
                if(player[turn].card_type_debuff[j]==8)
                {
                    //remplace la carte tout casser par broken cart
                    player[turn].card_type_debuff[i]=16;
                    player[turn].cards_debuff[i]=cartes[16];

                    //supprime la carte torch pick repaired
                    player[turn].card_type_debuff[j]=0;
                    player[turn].cards_debuff[j]=NULL;
                }

                //torch cart repaired
                if(player[turn].card_type_debuff[j]==9)
                {
                    //remplace la carte tout casser par broken pick
                    player[turn].card_type_debuff[i]=14;
                    player[turn].cards_debuff[i]=cartes[14];

                    //supprime la carte torch cart repaired
                    player[turn].card_type_debuff[j]=0;
                    player[turn].cards_debuff[j]=NULL;
                }

                //cart pick  repaired
                if(player[turn].card_type_debuff[j]==10)
                {
                    //remplace la carte tout casser par broken torch
                    player[turn].card_type_debuff[i]=12;
                    player[turn].cards_debuff[i]=cartes[12];

                    //supprime la carte cart pick repaired
                    player[turn].card_type_debuff[j]=0;
                    player[turn].cards_debuff[j]=NULL;
                }

                //torch repaired
                if(player[turn].card_type_debuff[j]==11)
                {
                    //remplace la carte tout casser par pick casser
                    player[turn].card_type_debuff[i]=14;
                    player[turn].cards_debuff[i]=cartes[14];

                    //remplace la carte torch repaired par cart broken
                    player[turn].card_type_debuff[j]=16;
                    player[turn].cards_debuff[j]=cartes[16];
                }

                //pick repaired
                if(player[turn].card_type_debuff[j]==11)
                {
                    //remplace la carte tout casser par torch casser
                    player[turn].card_type_debuff[i]=12;
                    player[turn].cards_debuff[i]=cartes[12];

                    //remplace la carte torch repaired par cart broken
                    player[turn].card_type_debuff[j]=16;
                    player[turn].cards_debuff[j]=cartes[16];
                }

                //cart repaired
                if(player[turn].card_type_debuff[j]==11)
                {
                    //remplace la carte tout casser par pick casser
                    player[turn].card_type_debuff[i]=14;
                    player[turn].cards_debuff[i]=cartes[14];

                    //remplace la carte torch repaired par torch casser
                    player[turn].card_type_debuff[j]=12;
                    player[turn].cards_debuff[j]=cartes[12];
                }
            }//fin du for
        }//fin du if

        ///==========================carte torch broken=============================================================
        if(player[turn].card_type_debuff[i]==12)
        {
            for(j=0; j<3; j++)
            {
                //torch repaired
                if(player[turn].card_type_debuff[j]==11)
                {
                    //supprime la carte torch casser
                    player[turn].card_type_debuff[i]=0;
                    player[turn].cards_debuff[i]=NULL;

                    //supprime la carte torch repaired
                    player[turn].card_type_debuff[j]=0;
                    player[turn].cards_debuff[j]=NULL;
                }

                //torch pick repaired
                if(player[turn].card_type_debuff[j]==8)
                {
                    //remplace la carte torch casser par pick
                    player[turn].card_type_debuff[i]=13;
                    player[turn].cards_debuff[i]=cartes[13];

                    //supprime la carte torch pick repaired
                    player[turn].card_type_debuff[j]=0;
                    player[turn].cards_debuff[j]=NULL;
                }

                //torch cart repaired
                if(player[turn].card_type_debuff[j]==9)
                {
                    //remplace la carte torch casser par cart
                    player[turn].card_type_debuff[i]=15;
                    player[turn].cards_debuff[i]=cartes[15];

                    //supprime la carte torch cart repaired
                    player[turn].card_type_debuff[j]=0;
                    player[turn].cards_debuff[j]=NULL;
                }
            }//fin du for
        }//fin du if

        ///==========================carte pick broken=============================================================
        if(player[turn].card_type_debuff[i]==14)
        {
            for(j=0; j<3; j++)
            {
                //pick repaired
                if(player[turn].card_type_debuff[j]==13)
                {
                    //supprime la carte pick casser
                    player[turn].card_type_debuff[i]=0;
                    player[turn].cards_debuff[i]=NULL;

                    //supprime la carte pick repaired
                    player[turn].card_type_debuff[j]=0;
                    player[turn].cards_debuff[j]=NULL;
                }

                //torch pick repaired
                if(player[turn].card_type_debuff[j]==8)
                {
                    //remplace la carte pick casser par torch
                    player[turn].card_type_debuff[i]=11;
                    player[turn].cards_debuff[i]=cartes[11];

                    //supprime la carte torch pick repaired
                    player[turn].card_type_debuff[j]=0;
                    player[turn].cards_debuff[j]=NULL;
                }

                //pick cart repaired
                if(player[turn].card_type_debuff[j]==10)
                {
                    //remplace la carte pick casser par cart
                    player[turn].card_type_debuff[i]=15;
                    player[turn].cards_debuff[i]=cartes[15];

                    //supprime la carte pick cart repaired
                    player[turn].card_type_debuff[j]=0;
                    player[turn].cards_debuff[j]=NULL;
                }
            }//fin du for
        }//fin du if

        ///==========================carte cart broken=============================================================
        if(player[turn].card_type_debuff[i]==16)
        {
            for(j=0; j<3; j++)
            {
                //cart repaired
                if(player[turn].card_type_debuff[j]==15)
                {
                    //supprime la carte cart casser
                    player[turn].card_type_debuff[i]=0;
                    player[turn].cards_debuff[i]=NULL;

                    //supprime la carte cart repaired
                    player[turn].card_type_debuff[j]=0;
                    player[turn].cards_debuff[j]=NULL;
                }

                //torch cart repaired
                if(player[turn].card_type_debuff[j]==9)
                {
                    //remplace la carte cart casser par torch
                    player[turn].card_type_debuff[i]=11;
                    player[turn].cards_debuff[i]=cartes[11];

                    //supprime la carte torch cart repaired
                    player[turn].card_type_debuff[j]=0;
                    player[turn].cards_debuff[j]=NULL;
                }

                //pick cart repaired
                if(player[turn].card_type_debuff[j]==10)
                {
                    //remplace la carte cart casser par pick
                    player[turn].card_type_debuff[i]=13;
                    player[turn].cards_debuff[i]=cartes[13];

                    //supprime la carte pick cart repaired
                    player[turn].card_type_debuff[j]=0;
                    player[turn].cards_debuff[j]=NULL;
                }
            }//fin du for
        }//fin du if

        ///verifie si il reste des cartes action casse qui empechent le placement de cartes galerie
        //                  tout est casse                   torche casse                         pickaxe broken                           minecart broken
        if(player[turn].card_type_debuff[i]==7 || player[turn].card_type_debuff[i]==12 || player[turn].card_type_debuff[i]==14 || player[turn].card_type_debuff[i]==16)
            return 1;
    }//fin du for(i=0; i<player[turn].cards_debuff_place; i++)
    return 0;
}


void carte_action_gauche(t_players player[5], int turn)
{
    int h,k;
    ///met toutes les cartes action a gauche
    for(h=0; h<3; h++)
    {
        if(player[turn].card_type_debuff[h] == 0)
        {
            k=h;
            while(player[turn].card_type_debuff[h] == 0)
            {
                k++;
                if(k==3)//sortie si k=6 car il y a que 6 cartes action au total actives
                    break;

                if(player[turn].card_type_debuff[k] != 0)
                {
                    player[turn].card_type_debuff[h] = player[turn].card_type_debuff[k];
                    player[turn].cards_debuff[h] = player[turn].cards_debuff[k];

                    player[turn].card_type_debuff[k] = 0;
                    player[turn].cards_debuff[k] = NULL;
                }
            }
        }
    }
    player[turn].cards_debuff_place=0;
    ///reinitialise l'emplacement de la derniere carte
    for(h=0; h<3; h++)
    {
        if(player[turn].card_type_debuff[h]!=0)
            player[turn].cards_debuff_place++;
    }
}


void initialisation(BITMAP* or[5], int tab_or[4], int occurences[19][2], int tab[7][5], int numberofplayers, t_players player[5])
{
    int i,j;

    ///initialisation du tableau
    for(i=0; i<7; i++)
        for(j=0; j<5; j++)
            tab[i][j]=0;

    ///initialisation des cartes saboteur ou miner
    j=rand() % (numberofplayers-1 + 1) + 1; //genere un nombre aleatoire de 1 a nombre de joeurs, ce nombre sera le saboteur
    player[j].role='S';

    ///affectation des cartes mineur
    for(i=1; i<=numberofplayers; i++)
    {
        if(i!=j)//tant que i est different de j le joueur prend le role de miner
            player[i].role='M';
    }

}


void initialisation2(int tab_or[4], int occurences[19][2], t_players player[5])
{
    int i,j;
    ///occurences originales
    for(i=1; i<=5; i++)//initialisation du nombre d'occurences
        occurences[i][0]=9;

    for(i=6; i<=18; i++)//initialisation du nombre d'occurences
        occurences[i][0]=2;

    for(i=1; i<=18; i++)//intitialisation du numero de carte(chaque type de carte a un numero)
        occurences[i][1]=i;

    for(i=0; i<4; i++)
        tab_or[i]=0;

    ///initialisation des cartes d'or aleatoires
    while(tab_or[1]==tab_or[2] || tab_or[1]==tab_or[3] || tab_or[2]==tab_or[3])
    {
        tab_or[1]=(rand() % (3-1 + 1) + 1);//nombre aletoire
        tab_or[2]=(rand() % (3-1 + 1) + 1);//nombre aletoire
        tab_or[3]=(rand() % (3-1 + 1) + 1);//nombre aletoire
    }

    ///initialisation des joueurs et de leur cartes
    for(i=1; i<5; i++)
    {
        player[i].cards_debuff_place=0;
        player[i].position_y=525;
        player[i].position_x=750;

        for(j=0; j<6; j++)
        {
            player[i].card_type[j]=0;
            player[i].cards[j]=NULL;
        }

        for(j=0; j<3; j++)
        {
            player[i].card_type_debuff[j]=0;
            player[i].cards_debuff[j]=NULL;
        }
    }

}


void interface(t_players player[5], int numberofplayers, FONT* averia, BITMAP* images[30], BITMAP* tete[10], BITMAP* cartes[19], BITMAP* or[5], BITMAP* sprite[15], int game_saved)
{
    ///BITMAPS
    BITMAP* temp=create_bitmap(SCREEN_W,SCREEN_H);//cree un ecran de sauvegarde
    BITMAP* buffer = create_bitmap(SCREEN_W, SCREEN_H);

    ///variables
    int turn=1,round=0,oldturn=0,debuff=0,turns_of_players_number=0,i,j,x,k=0,h=0, fin_partie=0;
    unsigned long ticks=0;
    int c=makecol(0,0,0);
    int occurences[19][2];
    Pioche pioche;
    int tab[7][5];//tableau du plateau
    int tab_or[4];//tableau des cartes d'or aletoires


while(fin_partie==0)
{
    ///initialisation de partie continuee
    turn=1; round=0; oldturn=0; debuff=0; turns_of_players_number=0; k=0; h=0; fin_partie=0; ticks=0;

    ///initialisation du jeu sans dependance de partie sauvegardee
    initialisation2(tab_or, occurences, player);

    ///partie sauvegardee
    if(game_saved==1)
    {
        ///charge les donnes du txt
        numberofplayers=loadgame(player, numberofplayers, &pioche, tab);

        ///initialise les cartes des joeuurs
        for(i=1; i<=numberofplayers; i++)
        {
            ///cartes
            for(j=0; j<6; j++)
            {
                player[i].cards[j]=cartes[player[i].card_type[j]];
            }

            ///cartes debuff
            for(j=0; j<3; j++)
            {
                 player[i].cards_debuff[j]=cartes[player[i].card_type_debuff[j]];
            }
        }

        ///initialise la pioche
        for(i=0; i<pioche.Sommet; i++)
        {
            pioche.Elements[i]=cartes[pioche.card_type[i]];
        }
        pioche.Sommet--;

        ///initialise les tours
        turn=1;
        turns_of_players_number++;
        round=1;
        ticks++;

        ///initialise la premiere carte du tableau
        tab[0][2]=1;
    }

    ///partie normale
    else
    {
        ///initialisation des occurences des cartes et de leur type des cartes d'or aleatoires et du tableau du plateau
        initialisation(or, tab_or, occurences, tab, numberofplayers, player);

        ///pioche aleatoire
        initPile(&pioche);
        //empilement avec nombre aleatoire
        for (i=0; i<71; i++)//original 71 59 modifie
        {
            empiler(&pioche, cartes, occurences);
        }
        pioche.Sommet--;//car apres le dernier element empiler il y a toujours sommet++
    }

    game_saved=0;


    ///Exit LOOP
while(!key[KEY_ESC])//tant que echap nest pas appuyer
{
    clear_keybuf();//libere le buffer du clavier


    game_saved=0;
    ///sauvegarde de partie
    if(key[KEY_P])
        game_saved=save_game(player, numberofplayers, &pioche, tab);


    ///sortie de la boucle de jeu et retours au menu
    if(game_saved==1)
        break;


    ///verifie le tour
    if(turn>numberofplayers)//si le tour atteints le nombre de joueurs, il se reinitialise
    {
        turn=1;
        turns_of_players_number++;//le nombre de tour jouer avec touts les joueurs
    }

    ///affichage en debut de tour
    affichage_tour(averia, buffer, images, tete, or, player, numberofplayers, turn, tab_or);

    ///nunero de cartes restatntes
    textprintf_ex(buffer, averia, 580, 610, makecol(255,50,50), -1, "%d", pioche.Sommet);

    ///supprime les cartes du joueur precedent
    if(oldturn!=turn)//supprime les csrtes precedentes pour laisserplace aux csrtes du nouvel joueur
        draw_sprite(screen, buffer, 0, 0);//update le buffer pour supprimer les cartes du joueur precedent

    oldturn=turn;//ancien tour prends la valeur du courrant pour comparaison

    ///verifie si le joueur a bien 6 carte et les anime
    checkcard(player, turn, &pioche, buffer, temp, c);//verifie si le joueur a besoin d'une carte et l'anime

    ///affichage des cartes du joueur courrant
    if(ticks>0)
    {
        for(j=0; j<6; j++)///cartes galerie
            if(player[turn].card_type[j]!=0)
                draw_sprite(buffer, player[turn].cards[j], 11+j*110, 70);

        for(j=0; j<3; j++)///cartes action
            if(player[turn].card_type_debuff[j]!=0)
                draw_sprite(buffer, player[turn].cards_debuff[j], 11+j*110, 230);
    }

    ///affcecte a chaque joueur son role avec animation
    if(turns_of_players_number==0)//selement si le premier tour durant lequel les cartes sont distribuees
    {
        blit(screen,temp,0,0,0,0,SCREEN_W,SCREEN_H);//copie de l'ecran pour garder toutes les saisies
        carte_role(temp, images, player, turn);
    }

if(turns_of_players_number>0)//c'est pour au debur animer toutes les cdarte s a chque joueur et selement apres le jeu commence avec le joueur numero 1
{
    ///met toutes les cartes action a gauche et compte le nouvel emplacement de la nouvelle carte action
    carte_action_gauche(player, turn);

    ///verifie si le joueur a un outil casse et compense les outils reparer avec les outils casses et deplace les carte action actives a gauche
    debuff=0;
    debuff=carte_action_casse(player, turn, cartes);

    ///deplacement de la carte drag & drop selement si le joeur n'est pas debuffed (donc il ne peut pas placer de carte galerie
    k=0; h=0; x=0;
    if(mouse_x >= 11 && mouse_x < 671 && mouse_y > 70 && mouse_y < 220 && mouse_b==2)
    {
        x=(mouse_x-11)/110;//numero de la carte type 0,1,2,3,4,5

        printf("carte en main : %d\n", player[turn].card_type[x]);
        blit(screen,temp,0,0,0,0,SCREEN_W,SCREEN_H);//copie de l'ecran pour garder toutes les saisies

        ///drag and drop carte galerie
        if(player[turn].card_type[x]<=5)
        {
            k=draganddrop_galerie(temp, images, player, turn, tab, x, round, debuff);//appel de la fonction
            if(k==2)//si c;estai une carte suprimmee
            {
                turn++;
            }
            else
            {
                turn=turn+k;//incremention du tour
                round=round+k;//incrementation du nombre de rounds
            }
        }

        ///drag and drop carte action
        if(player[turn].card_type[x]>=6)
        {
            h=draganddrop_action(temp, player, turn, x, numberofplayers, tab, images, or, tab_or);//appel de la fonctio
            if(h==2 || round==0)//si c;estai une carte suprimmee ou durant le premier round
            {
                turn++;
            }
            else
            {
                turn=turn+h;//incremention du tour
                round=round+h;//incrementation du nombre de rounds
            }
            ///round=round+k; a revoir ici
        }

        ///animatyion de retours du drag and drop si la carte a un mauvais placement
        if(k==0 && h==0 && debuff==0)
        {
            draganddrop_retours(temp, player, turn, x);
        }
        clear(temp);
    }

    ///affiche le plateau avec les cartes joueees
    afficheplateau(tab, cartes, buffer);

    ///deplacement du joueur sur le plateau
    if(k==1)//selement si la carte a ete placee;
    {
        blit(buffer,temp,0,0,0,0,SCREEN_W,SCREEN_H);//copie de l'ecran pour garder toutes les saisies
        deplacement_logo(temp, tete, player, tab, oldturn);///deplacement en cours de develeopement (Mathys)
    }

    ///affichage de toutes les tete sur le plateau
    for(i=1; i<=numberofplayers; i++)
    {
        draw_sprite(buffer, tete[player[i].head], player[i].position_x-22, player[i].position_y-22);
    }


    ///Forfait game over TEST SELEMENT POUR TESTER
    if(mouse_x > 740 && mouse_x < 850 && mouse_y > 15 && mouse_y < 55 && mouse_b==1)
    {
        finpartie_mineur(player, numberofplayers, sprite);
        goto label;
    }

   ///VERIFIE FIN DE PARTIE
    k=1;
    for(i=0; i<3; i++)
    {
        ///victoire mineurs
        if((tab[6][i*2]==1 || tab[6][i*2]==2 || tab[6][i*2]==4) && player[oldturn].role=='M')///si un mineur a ouvert une galerie sur une pepite d'or
        {
            ///victoire mineur
            printf("Mineur");

            ///compte du score
            for(j=1; j<=numberofplayers; j++)
            {
                if(k==i)///si variable est egale au numero de la carte gagante
                    k++;

                if(j==turn)///le le player n question est celui qui a gagner
                    player[j].score=player[j].score+tab_or[i];

                else if(player[j].role=='M')///disctribue les scores restants au autres mineurs
                {
                    player[j].score=player[j].score+tab_or[k];
                    k++;
                }
                printf("\nlayer %d score: %d\n", j, player[j].score);
            }
            finpartie_mineur(player, numberofplayers, sprite);
            clear_keybuf();//libere le buffer du clavier
            break;
        }

        ///victoire saboteur
        if((tab[6][i*2]==1 || tab[6][i*2]==2 || tab[6][i*2]==4) && player[oldturn].role=='S')///si un saboteur a ouvert une galerie sur une pepite d'or
        {
            ///victoire saboteur
            printf("saboteur");
            player[oldturn].score=player[oldturn].score+6;
            finpartie_saboteur(player, numberofplayers, sprite, averia);
            break;
        }

        ///game over
        if(pioche.empty==1)///si la pioche se vide
        {
            ///game over
            printf("game over");
            finpartie_gameover(images, player);
            break;
        }
    }

    ///sortie de la boucle de jeu et retours au menu
    if(tab[6][i*2]==1 || tab[6][i*2]==2 || tab[6][i*2]==4 || pioche.empty==1)//nested loops commee ca je sort du for et du while en suite
    {
        label:
        clear_keybuf();//libere le buffer du clavier
        fin_partie=continuer_demande(images, averia);
        break;
    }

    mouse_b=0;//pour nettoyer le buffer sourie
}//fin du if(turns_of_players_number>0)

    if(turns_of_players_number==0)//si ce'est le premier tour d'affichage
        turn++;

    ///Images
    draw_sprite(buffer, images[0], mouse_x-6, mouse_y);
    draw_sprite(screen, buffer, 0, 0);
    clear(buffer);

    ticks++;//incrementaion de la boucle
}//fin du while(end==0)

if(fin_partie==1 || key[KEY_ESC] || game_saved==1)///sortie si le jeu ne continue plus
{
    cumul_score(player, numberofplayers);
    break;
}


clear_bitmap(buffer);
clear_bitmap(temp);
}///fin du while(fin_partie==0) pour les parties qui vont continuer apres une victoire

    ///destruction des bitmaps
    destroy_bitmap(buffer);
    destroy_bitmap(temp);

}END_OF_FUNCTION(interface)


void affichage_tour(FONT* averia, BITMAP* buffer, BITMAP* images[30], BITMAP* tete[10], BITMAP* or[5], t_players player[5], int numberofplayers, int turn, int tab_or[4])
{
    int i, y, x, j;
    ///initialisation du plateau
    draw_sprite(buffer, images[6], 0, 0);
    draw_sprite(buffer, images[5], 700, 450);

    //place les cartes d'or
    for(i=1; i<=3; i++)
    {
        draw_sprite(buffer, or[tab_or[i]], 1500, 150+(i-1)*300);
        draw_sprite(buffer, or[4], 1500, 150+(i-1)*300);
    }

    ///affichage de la pioche et de la depioche
    draw_sprite(buffer, images[11], 545, 675);///depioche
    draw_sprite(buffer, images[7], 545, 510);///pioche


    ///affichage des noms et des scores et des logo et des cartes
    //le joueur en cours et les bouttons d'interraction
    textprintf_ex(buffer, averia, 755, 17, makecol(217,179,64), -1, "Forfeit");
    textprintf_ex(buffer, averia, 25, 17, makecol(217,179,64), -1, "%s", player[turn].nom);
    textprintf_ex(buffer, averia, 325, 17, makecol(217,179,64), -1, "%d", player[turn].score);
    textprintf_ex(buffer, averia, 500, 17, makecol(217,179,64), -1, "%c", player[turn].role);
    draw_sprite(buffer, tete[player[turn].head], 450, 12);


    //les joueurs en attente de leur tour
    y=525;
    for(i=1; i<=numberofplayers; i++)
    {
        if(i!=turn)
        {
            textprintf_ex(buffer, averia, 25, y, makecol(217,179,64), -1, "%s", player[i].nom);
            textprintf_ex(buffer, averia, 325, y, makecol(217,179,64), -1, "%d", player[i].score);
            draw_sprite(buffer, tete[player[i].head], 450, y-5);
            y=y+127;
        }
    }

    ///le joueurs en penalite
    x=25;
    for(i=1; i<=numberofplayers; i++)
    {
        for(j=0; j<3; j++)
        {
            ///verifie si il a une carte qui empenche l'emplacement des cartes galeries
            if(player[i].card_type_debuff[j]==7 || player[i].card_type_debuff[j]==12 || player[i].card_type_debuff[j]==14 || player[i].card_type_debuff[j]==16)
            {
                textprintf_ex(buffer, averia, x, 442, makecol(217,179,64), -1, "%s", player[i].nom);
                x=x+225;
            }
        }
    }
}


void afficheplateau(int tab[7][5] ,  BITMAP * cartes[17],  BITMAP * buffer)
{
    int i,j;
    for(i=0; i<7; i++)
    {
        for(j=0; j<5; j++)
        {
            if(tab[i][j]!=0)//affiche selemtn les casrtes qui sont stoquees dans le tableau
            {
                draw_sprite(buffer, cartes[tab[i][j]], 800+i*100, 150+j*150);
            }
        }
    }
}


int verification_placement(int tab[7][5], int x, int y, int round, int type)
{
    ///verification premiere carte
    if(round==0)//si la premiere carte
    {
        if(x!=0 || y!=2 || type==3 || type==5)//forcement a cote de la case depart
            return 0;
        else return 1;
    }

    ///verification de la continuation du chemin
    if((type==1 || type==2 || type==4 || type==5) && x>0)//verification a gauche de la carte 1 2 4 et 5
        if(tab[x-1][y]==1 || tab[x-1][y]==2 || tab[x-1][y]==3 || tab[x-1][y]==4)
        {
            printf("type de carte en x-1 %d\n", tab[x-1][y]);
            printf("%d  %d tab: %d\n", x,y, tab[x][y]);
            return 1;
        }


    if((type==1 || type==2 || type==3 || type==4) && x<7)//a droite des cartes 1 2 3 4
        if(tab[x+1][y]==1 || tab[x+1][y]==2 || tab[x+1][y]==4 || tab[x+1][y]==5)
        {
            printf("type de carte en x+1%d\n", tab[x+1][y]);
            printf("%d  %d tab: %d\n", x,y, tab[x][y]);
            return 1;
        }

    if((type==1 || type==3 || type==4) && y>0)//en haut des cartes 1 3 4
        if(tab[x][y-1]==1 || tab[x][y-1]==3)
        {
            printf("type de carte en y-1 %d\n", tab[x][y-1]);
            printf("%d  %d tab: %d\n", x,y, tab[x][y]);
            return 1;
        }

    if((type==1 || type==3)&& y<4)//en bas de cartes 1 3
        if(tab[x][y+1]==1 || tab[x][y+1]==3 || tab[x][y+1]==4)
        {
            printf("type de carte en y+1 %d\n", tab[x][y+1]);
            printf("%d  %d tab: %d\n", x,y, tab[x][y]);
            return 1;
        }

    return 0;//si on arrive jusqua la doncaucune des conditions de sont validdes et donc retourne 0 et le joueur doit choisir un nouveau emplacement
}


///===========================================================================DRAG AND DROP DEPLACEMENTS

int draganddrop_galerie(BITMAP * temp, BITMAP* images[30], t_players player[5], int turn, int tab[7][5], int numero, int round, int debuff)
{
    BITMAP* buffer = create_bitmap(SCREEN_W, SCREEN_H);
    int x,y,verif=0;
    while(mouse_b==2)//tant que le clixk droit est appuye
    {
        draw_sprite(buffer, temp, 0, 0);//dessine le'cran sauvegarder
        draw_sprite(buffer, player[turn].cards[numero], mouse_x-50, mouse_y-75);//dessine la carte avec les coordonnes au centre de la carte
        x=(int)(mouse_x-800)/100;//coordonnes arrondies pour sortir 0,1,2,3,4,5,6
        y=(int)(mouse_y-150)/150;//de meme pour 0,1,2,3,4
        draw_sprite(screen, buffer, 0, 0);
        clear(buffer);
    }

    ///dans la depioche
    if(mouse_x>=545 && mouse_x<=645 && mouse_y>=675 && mouse_y<=825)//si la carte est en depioche
    {
        clear_bitmap(images[11]);
        //clear(images[11]);
        //images[11]=player[turn].cards[numero];
        blit(player[turn].cards[numero],images[11],0,0,0,0,100,150);

        player[turn].cards[numero]=NULL;
        player[turn].card_type[numero]=0;
        return 2;//retourne 2 donc le tour s'incremente mais les rounds jouer ne s'incrementesnt pas, c'est pour laisser la place au placement de la premiere carte si la premiere action est de depiocher sa carte
    }

    ///sur le plateau
    verif=verification_placement(tab, x, y, round, player[turn].card_type[numero]);//verificatyion du bon placement de la carte galerie
    if(tab[x][y]==0 && x>=0 && x<=6 && y>=0 && y<=4 && verif==1 && debuff==0)//apres que le cliqu droit est lacher et la carte est lachee dans le plateau et la place est vide
    {
        tab[x][y]=player[turn].card_type[numero];//le tableau du plateau prends le type de la carte et les coordonnes arrondies de mouse_x et mouse_y
        player[turn].cards[numero]=NULL;
        player[turn].card_type[numero]=0;
        return 1;//retourne 1 donc le tour s'incremente
    }

    printf("\n\nX: %d, Y: %d carte avant: %d\n\n", x,y, tab[x][y]);
    return 0;//retoure 0 donc le tour reste le meme
}


int draganddrop_action(BITMAP *temp, t_players player[5], int turn, int numero, int numberofplayers, int tab[7][5], BITMAP* images[30], BITMAP* or[4], int tab_or[4])
{
    BITMAP* buffer = create_bitmap(SCREEN_W, SCREEN_H);
    int y,i,k,x;
    int black_color = makecol(0, 0, 0);
    while(mouse_b==2)//tant que le clixk droit est appuye
    {
        draw_sprite(buffer, temp, 0, 0);//dessine le'cran sauvegarder
        draw_sprite(buffer, player[turn].cards[numero], mouse_x-50, mouse_y-75);//dessine la carte avec les coordonnes au centre de la carte
        draw_sprite(screen, buffer, 0, 0);
        clear(buffer);
    }

    ///le joueur bouge sa carte en depioche
    if(mouse_x>=545 && mouse_x<=645 && mouse_y>=675 && mouse_y<=825)
    {
        clear_bitmap(images[11]);
        //images[11]=player[turn].cards[numero];
        blit(player[turn].cards[numero],images[11],0,0,0,0,100,150);

        player[turn].cards[numero]=NULL;
        player[turn].card_type[numero]=0;
        return 2;//retourne 1 donc le tour s'incremente
    }

    ///carte tunnel casse eboulement
    if(player[turn].card_type[numero] == 6 && mouse_x >= 800 && mouse_x <= 1600 && mouse_y >= 150 && mouse_y <= 900)//apres que le cliqu droit est lacher et la carte est lachee dans le logo
    {
        printf("========BROKEN TUNNEL %d==========\n", player[turn].card_type[numero]);
        x=(int)(mouse_x-800)/100;//coordonnes arrondies pour sortir 0,1,2,3,4,5,6
        y=(int)(mouse_y-150)/150;//de meme pour 0,1,2,3,4

        //si l'emplacement est vide ou c'est une carte type 3, ou la carte d'avant est vide ou la carte d'en haut est de type 3 ou 1 ou la carte d'en bas est de type 3 ou 1 ou c'est la case depart
        if(tab[x][y]==0 || tab[x][y]==3 || tab[x-1][y]==0 || tab[x][y+1]==3 || tab[x][y+1]==1 || tab[x][y-1]==3 || tab[x][y-1]==1 || (x==0 && y==2))//verifie le bon placement de la carte tunnel casse
            return 0;//sortie et la carte ne se place pas

        tab[x][y]=5;//sinon la crte galerie devient impasse
        player[turn].cards[numero]=NULL;
        player[turn].card_type[numero]=0;
        return 1;//retourne 1 donc le tour s'incremente
    }

    ///carte depioche de 3
    k=0;
    if(player[turn].card_type[numero] == 18 && mouse_x >= 450 && mouse_x <= 495 && mouse_y >= 12 && mouse_y <= 57)//apres que le cliqu droit est lacher et la carte est lachee dans le logo du joueur en cours
    {
        player[turn].cards[numero]=NULL;
        player[turn].card_type[numero]=0;

        while(k<3)///ici verifie aussi que le joueur a bien 3 cartes qui restent
        {
            x=(mouse_x-11)/110;//numero de la carte type 0,1,2,3,4,5
            if(mouse_x >= 11 && mouse_x < 671 && mouse_y > 70 && mouse_y < 220 && mouse_b==2 && player[turn].card_type[x]!=0)
            {
                while(mouse_b==2)//tant que le clixk droit est appuye
                {
                    draw_sprite(buffer, temp, 0, 0);//dessine le'cran sauvegarder
                    rectfill(buffer, x*110+11, 70, (x+1)*110+11, 220, black_color);
                    draw_sprite(buffer, player[turn].cards[x], mouse_x-50, mouse_y-75);//dessine la carte avec les coordonnes au centre de la carte
                    draw_sprite(screen, buffer, 0, 0);
                    clear(buffer);
                }

                if(mouse_x>=545 && mouse_x<=645 && mouse_y>=675 && mouse_y<=825)
                {
                    blit(screen,temp,0,0,0,0,SCREEN_W,SCREEN_H);//copie de l'ecran pour garder toutes les saisies
                    images[11]=player[turn].cards[x];//la depioche prends la carte suprimee
                    player[turn].cards[x]=NULL;
                    player[turn].card_type[x]=0;
                    k++;
                }
            }
            draw_sprite(buffer, temp, 0, 0);//dessine le'cran sauvegarder
            draw_sprite(buffer, images[0], mouse_x-6, mouse_y);//dessine la carte avec les coordonnes au centre de la carte
            draw_sprite(screen, buffer, 0, 0);
            clear(buffer);
        }
        return 2;//retourne 1 donc le tour s'incremente
    }

    ///carte map
    if(player[turn].card_type[numero] == 17 && mouse_x>=1500)
    {
        for(i=0; i<3; i++)
        {
            if(mouse_y>=150+i*300 && mouse_y<=300+i*300)
            {
                draw_sprite(buffer, temp, 0, 0);//dessine le'cran sauvegarder
                draw_sprite(buffer, or[tab_or[i+1]], 1500, 150+i*300);//dessine la carte avec les coordonnes au centre de la carte
                draw_sprite(screen, buffer, 0, 0);
                delay(2000);
                clear(buffer);
                player[turn].cards[numero]=NULL;
                player[turn].card_type[numero]=0;
                return 1;
            }
        }
        return 0;
    }

///cartes actions a d;atres joueurs
if(player[turn].card_type[numero]>=7 && player[turn].card_type[numero]<=16)
{
    //joueur en cours action normale
    if(mouse_x >= 450 && mouse_x <= 495 && mouse_y >= 12 && mouse_y <= 57)//apres que le cliqu droit est lacher et la carte est lachee dans le logo
    {
        if(player[turn].cards_debuff_place==3)//pas plus que 3 cartes action a cote d'un joueur
            return 0;

        player[turn].cards_debuff[player[turn].cards_debuff_place] = player[turn].cards[numero];//carte debuff prends la carte
        player[turn].card_type_debuff[player[turn].cards_debuff_place] = player[turn].card_type[numero];//le type est sauvegarder

        player[turn].cards[numero]=NULL;
        player[turn].card_type[numero]=0;
        return 1;//retourne 1 donc le tour s'incremente
    }

    //autres joueurs tres complique je n'arrive plus a comprendre ce que jai ecris mais ca fonctionne principe: la boucle verifie si les coordonnes du point de relachement du boutton correspondent a des coordonnes qui contiennent une image de tete d'un joueur autre que le jouer en cours
    k=1;y=525;//525 point de depart des images de tete
    for(i=1; i<=numberofplayers; i++)
    {
        if(i!=turn)
        {
            //printf("%d\n", y);//verification
            if(mouse_x >= 450 && mouse_x <= 495 && mouse_y >= y-5 && mouse_y <= y+40)
            {
                if(player[i].cards_debuff_place==3)//pas plus que 3 cartes action a cote d'un joueur
                    return 0;

                player[i].cards_debuff[player[i].cards_debuff_place]=player[turn].cards[numero];//carte debuff prends la carte qui a ete drag and drop
                player[i].card_type_debuff[player[i].cards_debuff_place]=player[turn].card_type[numero];//le type est sauvegarder
                player[i].cards_debuff_place++;

                player[turn].cards[numero]=NULL;
                player[turn].card_type[numero]=0;
                return 1;//retourne 1 donc le tour s'incremente
            }
            y=525+k*127;//incremente les coordonnes
            k++;
        }
    }
}//sin du if(player[turn].card_type[numero]>=7 && player[turn].card_type[numero]<=16)


    return 0;
}


void draganddrop_retours(BITMAP* temp,t_players player[5], int turn, int numero)
{
    BITMAP* buffer = create_bitmap(SCREEN_W, SCREEN_H);
    int y=mouse_y;
    int x=mouse_x;
    int soustraction=10*(mouse_x-(11+numero*110))/(mouse_y-70);

    while(y>=70)
    {
        draw_sprite(buffer, temp, 0, 0);//dessine le'cran sauvegarder
        draw_sprite(buffer, player[turn].cards[numero], x, y);//dessine la carte avec les coordonnes au centre de la carte

        x=x-soustraction;
        y=y-10;

        draw_sprite(screen, buffer, 0, 0);
        clear(buffer);
    }
}


void deplacement_logo(BITMAP* temp, BITMAP* tete[10], t_players player[5], int tab[7][5], int oldturn)
{
    BITMAP* buffer2 = create_bitmap(SCREEN_W, SCREEN_H);
    int i;

    ///position d'arrivee
    int x_nouveau = (int)850+((mouse_x-800)/100)*100;//en coordeonnes[800,1600]
    int y_nouveau = (int)225+((mouse_y-150)/150)*150;

    ///position intiale
    int x_logo = (int)(player[oldturn].position_x-800)/100;//position du logo en [0,6]
    int y_logo = (int)(player[oldturn].position_y-150)/150;//de meme pour 0,1,2,3,4

    i=0;
    ///recherche du meilleur trajet x_position -> x_carte_posee, y_position -> y_carte_posee
    while((player[oldturn].position_y != y_nouveau || player[oldturn].position_x != x_nouveau) && i<1600)
    {
        draw_sprite(buffer2, temp, 0, 0);//dessine le'cran sauvegarder
        draw_sprite(buffer2, tete[player[oldturn].head], player[oldturn].position_x-22, player[oldturn].position_y-22);

        ///cas simple aligne
        if(player[oldturn].position_x < x_nouveau && player[oldturn].position_y == y_nouveau)
            player[oldturn].position_x++;//en coordeonnes[800,1600]

        if(player[oldturn].position_x > x_nouveau && player[oldturn].position_y == y_nouveau)
            player[oldturn].position_x--;

        if(player[oldturn].position_y < y_nouveau && player[oldturn].position_x == x_nouveau)
            player[oldturn].position_y++;

        if(player[oldturn].position_y > y_nouveau && player[oldturn].position_x == x_nouveau)
            player[oldturn].position_y--;

        ///cas complique avec un changement de direction
        if(player[oldturn].position_x < x_nouveau && player[oldturn].position_y < y_nouveau)//si l'arrivve est en bas a droite
        {
            if(tab[x_logo+1][y_logo]==1 || tab[x_logo+1][y_logo]==2 || tab[x_logo+1][y_logo]==4)
                player[oldturn].position_x++;

            if(tab[x_logo][y_logo+1]==1 || tab[x_logo][y_logo+1]==3 || tab[x_logo][y_logo+1]==4)
                player[oldturn].position_y++;
        }

        if(player[oldturn].position_x < x_nouveau && player[oldturn].position_y > y_nouveau)//si l'arrivve est en haut a droite
        {
            if(tab[x_logo+1][y_logo]==1 || tab[x_logo+1][y_logo]==2 || tab[x_logo+1][y_logo]==4)
                player[oldturn].position_x++;

            if(tab[x_logo][y_logo-1]==1 || tab[x_logo][y_logo-1]==3)
                player[oldturn].position_y--;

        }

        if(player[oldturn].position_x > x_nouveau && player[oldturn].position_y > y_nouveau)//si l'arrivve est en haut a gauche
        {
            if(tab[x_logo-1][y_logo]==1 || tab[x_logo-1][y_logo]==2 || tab[x_logo-1][y_logo]==3 ||tab[x_logo-1][y_logo]==4)
                player[oldturn].position_x--;

            if(tab[x_logo][y_logo-1]==1 || tab[x_logo][y_logo-1]==3)
                player[oldturn].position_y--;
        }

        if(player[oldturn].position_x > x_nouveau && player[oldturn].position_y < y_nouveau)//si l'arrivve est en bas a gauche
        {
            if(tab[x_logo-1][y_logo]==1 || tab[x_logo-1][y_logo]==2 || tab[x_logo-1][y_logo]==3 ||tab[x_logo-1][y_logo]==4)
                player[oldturn].position_x--;

            if(tab[x_logo][y_logo+1]==1 || tab[x_logo][y_logo+1]==4)
                player[oldturn].position_y++;
        }

        i++;//sortie si le programe bug (ce que est probable)
        draw_sprite(screen, buffer2, 0, 0);
        clear(buffer2);
    }
    player[oldturn].position_x = x_nouveau;
    player[oldturn].position_y = y_nouveau;
}


///===========================================================================FIN DU JEU

int continuer_demande(BITMAP* images[30], FONT* averia)
{
    BITMAP* buffer = create_bitmap(SCREEN_W, SCREEN_H);
    while(1==1)
    {
        if(mouse_x>460 && mouse_x<725 && mouse_y>400 && mouse_y<440 && mouse_b==1)///continue
            return 0;

        if(mouse_x>885 && mouse_x<1155 && mouse_y>400 && mouse_y<440 && mouse_b==1)///,aine menu
            return 1;

        draw_sprite(buffer, images[13], 0, 0);
        draw_sprite(buffer, images[0], mouse_x-6, mouse_y);
        textprintf_ex(buffer, averia, 470, 405, makecol(217,179,64), -1, "Continue");
        textprintf_ex(buffer, averia, 895, 405, makecol(217,179,64), -1, "Main Menu");

        draw_sprite(screen, buffer, 0, 0);
        clear(buffer);
    }
}


void finpartie_gameover(BITMAP*images[30], t_players player[5])
{
    ///BITMAPS
    BITMAP* buffer = create_bitmap(SCREEN_W, SCREEN_H);

    ///GAME LOOP
    draw_sprite(buffer, images[12], 0, 0);
    draw_sprite(screen, buffer, 0, 0);
    delay(2000);
    clear(buffer);
    destroy_bitmap(buffer);
}


void finpartie_saboteur(t_players player[5], int numberofplayers, BITMAP* sprite[15], FONT* averia)
{
    ///variables
    install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL);
    int play_sample(const SAMPLE *spl, int vol, int pan, int freq, int loop);
    BITMAP* tetes[5];
    BITMAP* buffer = create_bitmap(SCREEN_W, SCREEN_H);
    BITMAP* temp = create_bitmap(SCREEN_W, SCREEN_H);
    SAMPLE* SndFx ;
    SndFx=load_sample("ennemi.wav");
    float z=0;
    int alea,i,k=0,o=2,colballe,ligballe,ticks=0, score_jeu=0;

    ///initialisation
    t_players saboteur;
    t_players mineur1;
    t_players mineur2;
    t_players mineur3;
    saboteur.position_x=400;
    saboteur.position_y=200;
    mineur1.position_x=1000;
    mineur1.position_y=0;
    mineur2.position_x=1000;
    mineur2.position_y=200;
    mineur3.position_x=1000;
    mineur3.position_y=400;

    ///Chargement des sprites
    for(i=1; i<=numberofplayers; i++)
    {
        if(player[i].role=='S')
        {
            player[i].position_x=200;
            player[i].position_y=200;
            tetes[1]=sprite[player[i].head];
        }
        else
        {
            player[i].position_x=500;
            player[i].position_y=200*k;
            k++;
            tetes[o]=sprite[player[i].head];
            o++;
        }
    }
    while(!key[KEY_ESC])
    {
        ///Affichage du premier fond d'écran
        draw_sprite(buffer,sprite[12],0,0);

        ///Chargement d'un chiffre aléatoire entre 0 et 3 si selement il y a eu 100 iteration de la boucle
        if(ticks%100==0)
            alea=rand()%(3+1);

        printf("x: %d\n", mineur1.position_x);
        printf("y: %d\n", mineur1.position_y);

        ///condition de depacement de l'ecran
        //sur x >1600
        if(mineur1.position_x>1500)
            mineur1.position_x=1500;
        if(mineur2.position_x>1500)
            mineur2.position_x=1500;
        if(mineur3.position_x>1500)
            mineur3.position_x=1500;

        //sur x <1000
        if(mineur1.position_x<1000)
            mineur1.position_x=1000;
        if(mineur2.position_x<1000)
            mineur2.position_x=1000;
        if(mineur3.position_x<1000)
            mineur3.position_x=1000;

        //sur y>900
        if(mineur1.position_y>890)
            mineur1.position_y=890;
        if(mineur2.position_y>890)
            mineur2.position_y=890;
        if(mineur3.position_y>890)
            mineur3.position_y=890;

        //sur y<0
        if(mineur1.position_y<0)
            mineur1.position_y=0;
        if(mineur2.position_y<0)
            mineur2.position_y=0;
        if(mineur3.position_y<0)
            mineur3.position_y=0;

        ///deplacement
        if(alea==0)
        {
            mineur1.position_x++;
            mineur2.position_x--;
            mineur3.position_y++;
        }
        if(alea==1)
        {
            mineur1.position_x--;
            mineur2.position_x++;
            mineur3.position_y--;
        }
        if(alea==2)
        {
            mineur1.position_y++;
            mineur2.position_y--;
            mineur3.position_x++;
        }
        if(alea==3)
        {
            mineur1.position_y--;
            mineur2.position_y++;
            mineur3.position_x--;
        }

        ///Mouvement du saboteur en fonction des flèches du clavier
        if(key[KEY_UP])
        {
            saboteur.position_y=saboteur.position_y-2;
            if(saboteur.position_y==0)
            {
                saboteur.position_y=saboteur.position_y+2;
            }
        }
        if(key[KEY_DOWN])
        {
            saboteur.position_y=saboteur.position_y+2;
            if(saboteur.position_y+200==SCREEN_H)
            {
                saboteur.position_y=saboteur.position_y-2;
            }
        }
        if(key[KEY_RIGHT])
        {
            saboteur.position_x=saboteur.position_x+2;
            if(saboteur.position_x+100==800)
            {
                saboteur.position_x=saboteur.position_x-2;
            }
        }
        if(key[KEY_LEFT])
        {
            saboteur.position_x=saboteur.position_x-2;
            if(saboteur.position_x+50==0)
            {
                saboteur.position_x=saboteur.position_x+2;
            }
        }

        ///Tir d'un caillou avec la barre espace
        if(key[KEY_SPACE])
        {
            colballe=saboteur.position_y+100;
            ligballe=saboteur.position_x+125;
            z=-0.1*saboteur.position_x/40;
            blit(screen,temp,0,0,0,0,SCREEN_W,SCREEN_H);
            while(colballe<saboteur.position_y+101)
            {
                ligballe++;
                colballe=200*sin(0.0025*ligballe+z+PI)+(saboteur.position_y+100);
                draw_sprite(buffer,temp,0,0);
                draw_sprite(buffer,sprite[10],ligballe,colballe);
                draw_sprite(screen,buffer,0,0);
                clear_bitmap(buffer);
                if(ligballe>mineur1.position_x && ligballe<mineur1.position_x+200 && colballe>mineur1.position_y && colballe<mineur1.position_y+200)
                {
                    score_jeu++;
                    play_sample(SndFx,255,128,1000,0);
                    break;
                }
                else if(ligballe>mineur2.position_x && ligballe<mineur2.position_x+200 && colballe>mineur2.position_y && colballe<mineur2.position_y+200 && numberofplayers>2)
                {
                    score_jeu++;
                    play_sample(SndFx,255,128,1000,0);
                    break;
                }
                else if(ligballe>mineur3.position_x&&ligballe<mineur3.position_x+200 && colballe>mineur3.position_y && colballe<mineur3.position_y+200 && numberofplayers>3)
                {
                    score_jeu++;
                    play_sample(SndFx,255,128,1000,0);
                    break;
                }
            }
            colballe=saboteur.position_y+100;
        }

        ///affichage du texte:
         textprintf_ex(buffer, averia, 70, 17, makecol(217,179,64), -1, "SCORE : %d", score_jeu);

        ///Affichage des sprites
        draw_sprite(buffer,tetes[1],saboteur.position_x,saboteur.position_y);

        if(numberofplayers==2)
        {
            draw_sprite(buffer,tetes[1], saboteur.position_x, saboteur.position_y);
            draw_sprite(buffer,tetes[2], mineur1.position_x, mineur1.position_y);
        }
        else if(numberofplayers==3)
        {
            draw_sprite(buffer,tetes[1], saboteur.position_x, saboteur.position_y);
            draw_sprite(buffer,tetes[2], mineur1.position_x, mineur1.position_y);
            draw_sprite(buffer,tetes[3], mineur2.position_x, mineur2.position_y);
        }
        else
        {
            draw_sprite(buffer,tetes[1], saboteur.position_x, saboteur.position_y);
            draw_sprite(buffer,tetes[2], mineur1.position_x, mineur1.position_y);
            draw_sprite(buffer,tetes[3], mineur2.position_x, mineur2.position_y);
            draw_sprite(buffer,tetes[4], mineur3.position_x, mineur3.position_y);
        }
        draw_sprite(screen,buffer,0,0);

        ticks++;
    }///fin du while
}


void finpartie_mineur(t_players player[5], int numberofplayers, BITMAP* sprite[15])
{
    ///variables
    BITMAP* tetes[5];
    BITMAP* buffer = create_bitmap(SCREEN_W, SCREEN_H);
    BITMAP* temp = create_bitmap(SCREEN_W, SCREEN_H);
    int i,k=0,o=2,x1=765,y1=400,x2=800,y2=400,vitesse=2;

    ///initialisation
    t_players mineur1;
    t_players mineur2;
    t_players mineur3;
    mineur1.position_x=500;
    mineur1.position_y=300;
    mineur2.position_x=700;
    mineur2.position_y=160;
    mineur3.position_x=925;
    mineur3.position_y=250;

    ///Chargement des sprites
    for(i=1; i<=numberofplayers; i++)
    {
        if(player[i].role=='S')
        {
            player[i].position_x=200;
            player[i].position_y=200;
            tetes[1]=sprite[player[i].head];
        }
        else
        {
            player[i].position_x=500;
            player[i].position_y=200*k;
            k++;
            tetes[o]=sprite[player[i].head];
            o++;
        }
    }
    while(!key[KEY_ESC])
    {
        ///Affichage du deuxième fond d'écran
        draw_sprite(buffer,sprite[13],0,0);
        ///Affichage des sprites
        draw_sprite(buffer,sprite[14],725,350);
        draw_sprite(buffer,sprite[11],x1,y1);
        draw_sprite(buffer,sprite[11],x2,y2);
        if(numberofplayers==2)
        {
            draw_sprite(buffer,tetes[2],mineur1.position_x,mineur1.position_y);
        }
        else if(numberofplayers==3)
        {
            draw_sprite(buffer,tetes[2],mineur1.position_x,mineur1.position_y);
            draw_sprite(buffer,tetes[3],mineur2.position_x,mineur2.position_y);
        }
        else
        {
            draw_sprite(buffer,tetes[2],mineur1.position_x,mineur1.position_y);
            draw_sprite(buffer,tetes[3],mineur2.position_x,mineur2.position_y);
            draw_sprite(buffer,tetes[4],mineur3.position_x,mineur3.position_y);
        }
        draw_sprite(screen,buffer,0,0);
        blit(screen,temp,0,0,0,0,SCREEN_W,SCREEN_H);

        ///Mouvement des pépites d'or en cloche de manière symétrique
        while(y1>350)
        {
            x1=x1-vitesse;
            x2=x2+vitesse;
            y1=y1-vitesse;
            y2=y2-vitesse;
            draw_sprite(buffer,temp,0,0);
            draw_sprite(buffer,sprite[11],x1,y1);
            draw_sprite(buffer,sprite[11],x2,y2);
            draw_sprite(screen,buffer,0,0);
            clear_bitmap(buffer);
        }
        while(y1<425)
        {
            x1=x1-vitesse;
            x2=x2+vitesse;
            y1=y1+vitesse;
            y2=y2+vitesse;
            draw_sprite(buffer,temp,0,0);
            draw_sprite(buffer,sprite[11],x1,y1);
            draw_sprite(buffer,sprite[11],x2,y2);
            draw_sprite(screen,buffer,0,0);
            clear_bitmap(buffer);
        }
        x1=765;
        y1=400;
        x2=800;
        y2=400;
        rest(100);
    }
}


///===========================================================================SAUVEGARDE

void cumul_score(t_players player[5], int numberofplayers)
{
    int i;
    FILE *fptr;
    fptr = fopen("Scores.txt","a");

    if(fptr == NULL)
    {
        printf("Error!");
        exit(1);
    }

    fprintf(fptr,"\n");

    for(i=1; i<=numberofplayers; i++)
    {
        ///nom score tete role
        fprintf(fptr,"%s  Score: %d  Role: %c\n",player[i].nom, player[i].score, player[i].role);
    }
    fprintf(fptr,"\n");

    fclose(fptr);
}


int save_game(t_players player[5], int numberofplayers, Pioche * pioche, int tab[7][5])
{
    int i,j;
    FILE *fptr;
    fptr = fopen("Save.txt","w");

    if(fptr == NULL)
    {
        printf("Error!");
        exit(1);
    }

    ///joueurs
    fprintf(fptr,"%d\n",numberofplayers);

    for(i=1; i<=numberofplayers; i++)
    {
        ///nom score tete role
        fprintf(fptr,"%s\n",player[i].nom);
        fprintf(fptr,"%d\n",player[i].score);
        fprintf(fptr,"%d\n",player[i].head);
        fprintf(fptr,"%c\n",player[i].role);


        ///cartes
        for(j=0; j<6; j++)
        {
            fprintf(fptr,"%d\n",player[i].card_type[j]);
        }


        ///cartes debuff
        for(j=0; j<3; j++)
        {
            fprintf(fptr,"%d\n",player[i].card_type_debuff[j]);
        }


        ///positions
        fprintf(fptr,"%d\n",player[i].position_x);
        fprintf(fptr,"%d\n",player[i].position_y);
    }

    ///pioche
    fprintf(fptr,"%d\n",pioche->Sommet);

    for(i=0; i<pioche->Sommet; i++)
    {
        fprintf(fptr,"%d\n",pioche->card_type[i]);
    }

    ///plateau
    for(i=0; i<7; i++)
        for(j=0; j<5; j++)
            fprintf(fptr,"%d\n",tab[i][j]);

    fclose(fptr);
    //sortie de la boucle

    return 1;
}


int loadgame(t_players player[5], int numberofplayers, Pioche * pioche, int tab[7][5])
{

    ///variables
    int i,j;

    FILE *fptr;
    fptr = fopen("Save.txt","r");

    if(fptr == NULL)
    {
        printf("Error!");
        exit(1);
    }

    ///joueurs
    fscanf(fptr, "%d", &numberofplayers);

    for(i=1; i<=numberofplayers; i++)
    {
        ///nom score tete role
        fscanf(fptr,"%s", player[i].nom);
        fscanf(fptr,"%d", &player[i].nom);
        fscanf(fptr,"%d", &player[i].head);
        fscanf(fptr,"%c", &player[i].role);
        fscanf(fptr,"%c", &player[i].role);

        ///cartes
        for(j=0; j<6; j++)
        {
            fscanf(fptr,"%d", &player[i].card_type[j]);
        }


        ///cartes debuff
        for(j=0; j<3; j++)
        {
            fscanf(fptr,"%d", &player[i].card_type_debuff[j]);
        }

        ///positions
        fscanf(fptr,"%d\n", &player[i].position_x);
        fscanf(fptr,"%d\n", &player[i].position_y);

    }

    ///pioche
    fscanf(fptr,"%d",&pioche->Sommet);

    for(i=0; i<pioche->Sommet; i++)
    {
        fscanf(fptr,"%d", &pioche->card_type[i]);
    }

    ///plateau
    for(i=0; i<7; i++)
        for(j=0; j<5; j++)
            fscanf(fptr,"%d", &tab[i][j]);

    fclose(fptr);
    //sortie de la boucle

    pioche->nbPlaces = 71;





    printf("%d\n", numberofplayers);
    for(i=1; i<=numberofplayers; i++)
    {
        ///nom score tete role
        printf("%s\n",player[i].nom);
        printf("%d\n",player[i].score);
        printf("%d\n",player[i].head);
        printf("%c\n",player[i].role);


        ///cartes
        for(j=0; j<6; j++)
        {
            printf("%d\n",player[i].card_type[j]);
        }


        ///cartes debuff
        for(j=0; j<3; j++)
        {
            printf("%d\n",player[i].card_type_debuff[j]);
        }


        ///positions
        printf("%d\n",player[i].position_x);
        printf("%d\n",player[i].position_y);
    }

    ///pioche
    printf("%d\n",pioche->Sommet);

    for(i=0; i<pioche->Sommet; i++)
    {
        printf("%d\n",pioche->card_type[i]);
    }

    ///plateau
    for(i=0; i<7; i++)
        for(j=0; j<5; j++)
            printf("%d\n",tab[i][j]);

    return numberofplayers;

}

