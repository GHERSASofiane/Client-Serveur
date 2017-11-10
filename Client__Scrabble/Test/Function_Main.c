
//********************************************************************************************************************************************************
//********************************************************************************************************************************************************
//********************************************************************************************************************************************************
//******************                 projet réalisé par GHERSA Sofiane                                    ************************************************
//******************                 dernier version le : 11/04/2017                                      ************************************************
//******************                 Paris 6 ( UPMC ) : MASTER 1 STL                                      ************************************************
//******************                 le projet est sur : https://github.com/GHERSASofiane/Client-Serveur  ************************************************
//********************************************************************************************************************************************************
//********************************************************************************************************************************************************
//********************************************************************************************************************************************************
#include <gtk/gtk.h>
#include <unistd.h>
#include <stdio.h> 
#include <string.h>    
#include <sys/socket.h>    
#include <arpa/inet.h> 
#include <string.h> 
#include <pthread.h>

#include "Function_Main.h"

int id_test = 0;
//************************************************************************************************************
//*******************************    variable thread et structure  *******************************************
struct Parms_Tour { char Tour[400] ; };
struct Parms_Tour thread1_args;

pthread_t thr;
pthread_t thread_Time;
pthread_t Thread_Recev_Commandes;
pthread_t thr_TRAY;
pthread_t Thread_Bilan;
pthread_t thread_maj_message;

pthread_t  thread_fonc_test; 

//************************************************************************************************************
//*******************************    les int et char    ******************************************************
int sock ;
int temps_att = 0 ;
int is_msg_priv = 0;
int my_score ;
char bilan_recev[500];
char Tampo_char[5] ;
char message_recuper[2000];
char message_a_envoi[2000];
char message_tmp[200];
char My_name_j[200];
char My_name[200];
char Proposed_tray[400];
char Text_tray_validate[400];
char Proposed_solution[20];
char time_reste[10];
char  msg_pour[256]    = "";
char  msg_body[256]    = "";
char Buf_Param_CONECT[200];

//************************************************************************************************************
//*******************************    les pointeurs    ********************************************************
typedef struct tirage_jeu{ 
  GtkWidget** tab_tirage;
}tirage_jeu;
tirage_jeu* tj;

typedef struct tray_s{ 
  GtkWidget** tab_tray_s;
}tray_s;
tray_s* tray_s_v;

typedef struct tab_tirage{ 
  GtkWidget** champs;
}tab_tirage;
tab_tirage* my_tab_tirage;

typedef struct tab_message{ 
  GtkWidget** champs;
}tab_message;
tab_message* my_tab_message;

typedef struct tab_s{ 
  GtkWidget** champs;
}tab_s;
tab_s* my_tab_s;

typedef struct tab_j{ 
  GtkWidget** champs;
}tab_j;
tab_j* my_tab_j;
 
//************************************************************************************************************
//*******************************    les GtkWidget    ********************************************************
GtkWidget *Tampo_Button , *C_Test;
GtkWidget *Afficher_message;
GtkWidget *fenetre_principale = NULL;
GtkBuilder *builder_principale = NULL;
GError *error_Chargement_fichier_principale = NULL;
GtkLevelBar *Time;
GtkWidget *pVBox_tray;
GtkWidget *labal_erreur, *labal_timer,  *lbl_Joueur;
GtkWidget *is_m , *is_n_m;
GtkWidget *M_m , *M_j , *M_t , *Aff_sco;



//************************************************************************************************************
//*******************************    fonction d'extraire une chaine d'une autre  *****************************
 
    void sub_String(const char *chaine, int debut, int fin, char *result){ 
        result[fin+1-debut]='\0';
        memcpy(result,(char *)chaine+debut,fin+1-debut); 
        
    }


//**********************************************************************************************************************************************
//**********************************************************************************************************************************************
                                                              // la Bar de temps
//**********************************************************************************************************************************************
//**********************************************************************************************************************************************
void *thread_time(void *arg) {
  
      gtk_level_bar_set_max_value(Time , temps_att);

      for (int i =0; i < temps_att; ++i)
     {
             sleep(1);
             gtk_level_bar_set_value( Time,i);
             char Title[30] = "Time remaining : ";
             char str_tmp[5] = "";
             sprintf(str_tmp, "%d", temps_att-i);
             strcat(Title, str_tmp);
             strcat(Title, " s");

             gtk_label_set_text(GTK_LABEL(labal_timer),Title);
     }

    (void) arg;

    pthread_exit(NULL);

}

//**********************************************************************************************************************************************
//**********************************************************************************************************************************************
                                                                // Envoi des messages
//**********************************************************************************************************************************************
//**********************************************************************************************************************************************
 void Send_Message(){  // Envoi de message 
        if (strstr(msg_pour, "Tout") )
        {
            char  msg[300] = "ENVOI/";
            strcat(msg,msg_body);
            strcat(msg,"/\n");
            int vv = write(sock,msg, strlen(msg) );
            fflush(stdout);
            if (vv != 0)
            {}
        }else{
            char  msg[300] = "PENVOI/";
            strcat(msg,msg_pour);
            strcat(msg,"/");
            strcat(msg,msg_body);
            strcat(msg,"/\n");
            int vv = write(sock,msg, strlen(msg) );
            fflush(stdout);
            if (vv != 0)
            {}
        }
           
           strcpy(msg_body,"");
           strcpy(msg_pour,"");
 }
 
static void snd_msg_dist (GtkWidget * p_wid, gpointer p_data)
        {
           const char  * p_text    = p_data;
           strcat(msg_pour, p_text);
           Send_Message();
        }

static void snd_msg_bod (GtkWidget * p_wid, gpointer p_data)
        {
           const char  * p_text    = p_data;
           strcat(msg_body, p_text);
        }
//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
                                                                //   Afficher les message recu  
//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
void maj_interface_message(char data[100]){//***** affiche message recu sur interface 
      int tmp = 0 ;
       
      for (int i = 0; i < 14; ++i)
      {
       
        if (strlen(gtk_label_get_text((gpointer)  my_tab_message->champs[i] )) == 0)
        {
         break;
        }
         tmp ++ ;
      }
      if (tmp >= 14)
      {
          for (int i = 1; i < 14; ++i)
          {
            gtk_label_set_text( (gpointer)  my_tab_message->champs[i-1] , 
              gtk_label_get_text((gpointer)  my_tab_message->champs[i] ) );
          }
          gtk_label_set_text((gpointer)  my_tab_message->champs[13] , data);
      }else{
       gtk_label_set_text((gpointer)  my_tab_message->champs[tmp] , data);
      }
 
}

void *maj_message(void *arg){

        char data[200] = "";
              if (is_msg_priv == 1)
              {
                
                          char body_msg[100] = "";
                          char log_msg[100] = "";
                          char tmp_msg[100] = "";
                          int seq = 0 ;

                          for (int i = 0; i < strlen(message_tmp); ++i)
                          {
                              if (message_tmp[i] == '/')
                               {
                                 break;
                               }else{
                                body_msg[i] = message_tmp[i] ;
                               }
                               seq ++ ;
                          }

                   sub_String(message_tmp, seq+1 ,strlen(message_tmp),log_msg);     
                strcpy(data , log_msg);
                strcat(data , " : ");
                strcat(data , body_msg);
                strcat(data , "\n");
              }else{
                strcpy(data , "Pub : ");
                strcat(data , message_tmp);
                strcat(data , "\n");
              }

        maj_interface_message(data);
       
        (void) arg;
        pthread_exit(NULL); 
      
}



//**********************************************************************************************************************************************
//**********************************************************************************************************************************************
                                                              // liste des jouers
//**********************************************************************************************************************************************
//**********************************************************************************************************************************************
void Thread_Players(char b[400] , int is_con) {

        char etat[200] = "";

        if (is_con == 1)
        {
          strcat( etat , " Connexion : " );
        }else{
          strcat( etat , " Deconnection : " );
        }

        
        strcat(etat , b);
        gtk_label_set_text((gpointer) lbl_Joueur , etat );
}

//**********************************************************************************************************************************************
//**********************************************************************************************************************************************
                                                              // Afficher le scor de tout les joueur
//**********************************************************************************************************************************************
//**********************************************************************************************************************************************


void Affiche_scor(char data[800]){

        for (int i = 0; i < 15; ++i)
        { 
        gtk_label_set_text((gpointer)  my_tab_j->champs[i] , "");
        gtk_label_set_text((gpointer)  my_tab_s->champs[i] , "");
        }
          int ind = 0 ;
          char j[40] = "" ;
          char s[40] = "" ;
          int id_j = 0 ;
          int id_s = 0 ;
          int id_aff = 0 ;

          while(data[ind-1] != '/'){

                id_j = 0 ;
                id_s = 0 ;
                strcpy( j, "");
                strcpy( s, "");
      //******  recup le joueur i
                for (int i = ind; i < strlen(data); ++i)
                {
                  if (data[i] == '*')
                  {
                    break;
                  }else{
                    
                    j[id_j] = data[i]; id_j ++ ;
                  }
                  ind ++ ;
                }
                ind ++ ;
      //******  recup le score de joueur i
                for (int i = ind; i < strlen(data); ++i)
                {
                  if (data[i] == '*')
                  {
                    break;
                  }else if (data[i] == '/')
                  {
                     break;
                  }else {
                    s[id_s] = data[i]; id_s ++ ;
                  }
                  ind ++ ;
                }
                ind ++ ;
                
                              if ( id_aff <= 14 )
                              {
                                gtk_label_set_text((gpointer)  my_tab_j->champs[id_aff] , j);
                                gtk_label_set_text((gpointer)  my_tab_s->champs[id_aff] , s);
                                id_aff ++ ;
                              }

          }
}

//**********************************************************************************************************************************************
//**********************************************************************************************************************************************
                                                              // Afficher le bilan
//**********************************************************************************************************************************************
//**********************************************************************************************************************************************

void Affich_my_score(char data[800]){
      char scores[800] = "";
          char j[40] = "" ;
          char s[40] = "" ;
          int ind = 0;

         int id_j = 0 ;
         int id_s = 1 ;
                strcpy( j, "");
                strcpy( s, "\t");
      //******  recup le joueur i
                for (int i = ind; i < strlen(data); ++i)
                {
                  if (data[i] == '*')
                  {
                    break;
                  }else{
                    
                    j[id_j] = data[i]; id_j ++ ;
                  }
                  ind ++ ;
                }
                ind ++ ;
      //******  recup le score de joueur i
                for (int i = ind; i < strlen(data); ++i)
                {
                  if (data[i] == '*')
                  {
                    break;
                  }else if (data[i] == '/')
                  {
                     break;
                  }else {
                    s[id_s] = data[i]; id_s ++ ;
                  }
                  ind ++ ;
                }
                ind ++ ;

      strcat( s, "\t");
      sub_String( data, ind, strlen(data), scores);
      gtk_label_set_text((gpointer)  Aff_sco , s);
      Affiche_scor(scores);
}
//**********************************************************************************************************************************************
//**********************************************************************************************************************************************
                                                              // Afficher le bilan
//**********************************************************************************************************************************************
//**********************************************************************************************************************************************

void Affiche_Bilan(char data[1000]){
     //***  BILAN/mot/vainqueur/scores/
      char data_trait[1000] = "";
      char mot[30] = "";
      char vainqueur[30] = "";
      char nb_tour[30] = "";
      char scores[800] = "";
      int pos = 0 ;
      sub_String(data,6,strlen(data)-2,data_trait);
      //*** extraire le mot 
      for (int i = 0; i < strlen(data_trait); ++i)
      {
        if (data_trait[i] == '/')
        {
          break;
        }else if (data_trait[i] == '*')
        {
          break;
        }else{
          mot[i] = data_trait[i];
        }

        pos ++ ;
      }

       if (data_trait[pos] == '*')
      {
        for (int i = pos; i < strlen(data_trait); ++i)
        {
          if (data_trait[i] == '/')
          {
            break;
          }
          pos ++ ;
        }
      }
 //*** extraire le  vainqueur
      pos ++ ;
      int id_v = 0 ;
      for (int i = pos; i < strlen(data_trait); ++i)
      {
        if (data_trait[i] == '/')
        {
          break;
        }else if (data_trait[i] == '*')
        {
          break;
        }else{
          vainqueur[id_v] = data_trait[i];id_v ++ ;
        }

        pos ++ ;
      }
      pos ++ ;
      id_v = 0;
      for (int i = pos; i < strlen(data_trait); ++i)
      {
        if (data_trait[i] == '*')
        {
          break;
        }else{
            nb_tour[id_v] = data_trait[i];id_v ++ ;
        }
        pos ++ ;
      }
      sub_String( data_trait, pos+1, strlen(data_trait), scores);

                    gtk_label_set_text( (gpointer) M_m  , mot);
                    gtk_label_set_text( (gpointer) M_j  , vainqueur);
                    gtk_label_set_text( (gpointer) M_t  , nb_tour);
                    Affich_my_score(scores);

              if (strcmp(vainqueur , My_name_j) == 0 )
              {
               gtk_label_set_text ( (gpointer) is_m, "vous être le meilleur");
              }else{
                gtk_label_set_text ( (gpointer) is_m, "");
              }
}

//********************************************************************************************************************************
//******************************************** fonction remplir le tray **********************************************************
//********************************************************************************************************************************
 //******* insere un mot dans un testentry 
void insert_tray(GtkEntry *entry, char text[200]){
  gtk_entry_set_text (entry, text);
}



void Fill_Tray(char replys[250]){ //***** remplir le plateau
      char carac[2];
      int pos = 0 ;
      int ind = 0 ;

      for (int i = 0; i < 15; ++i)
      {
        
                char c_i[3];
                sprintf(c_i, "%d", i);
          for (int j = 0; j < 15; ++j)
          {
            carac[0] = replys[pos];
            pos++;
            char  tray[20] = "Plateau_";
            strcat(tray, c_i);  
            strcat(tray, "_");
            char c_j[3];
            sprintf(c_j, "%d", j);
            strcat(tray, c_j);

                  tj->tab_tirage[ind] = NULL;
            if(carac[0] != '0'){ 
                  insert_tray(GTK_ENTRY (gtk_builder_get_object(builder_principale, tray)), carac);
                  tj->tab_tirage[ind] = GTK_WIDGET(gtk_builder_get_object(builder_principale, tray)); 
                                          
                                  
            }
            
              ind++;
          }
      }
}

//********************************************************************************************************************************
//******************************************** fonction remplir la bare des proposition ******************************************
//********************************************************************************************************************************
void Fill_Print(){ //**** remplir les littre pour le tirage 

      for (int i = 0; i < 7; ++i)
      {
        char  tray[20] = "tray_";// le nom de case a remplir
        char id[2];
        sprintf(id, "%d", i);
        strcat(tray, id);
        char car[2] ;
        car[0] = Proposed_solution[i];// le caracter i 
        insert_tray(GTK_ENTRY (gtk_builder_get_object(builder_principale, tray)), car);

      }
}

//********************************************************************************************************************************
//******************************************** send **********************************************************
//********************************************************************************************************************************
void send_tray_proposite(char Text[200]){
                           
       int vv = write(sock,Text, strlen(Text) );
       fflush(stdout);
       strcpy(Text_tray_validate, "");
       if (vv != 0){}

}

//********************************************************************************************************************************
//******************************************** fonction valider le tray **********************************************************
//********************************************************************************************************************************
static void Validate_tray (GtkWidget *pButton, gpointer data) {

            strcpy(Text_tray_validate, "");
            GtkWidget *pTempEntry;
           
            const gchar *sText;
            strcat(Text_tray_validate, "TROUVE/");

        for (int i = 0; i < 225; ++i)
         {
                
                
                /* Recuperation du texte contenu dans le GtkEntry */
                           sText = gtk_entry_get_text(GTK_ENTRY(tray_s_v->tab_tray_s[i]));
                          if (strlen(sText) == 0 )
                          {
                                strcat(Text_tray_validate, "0");
                          }else{
                                strcat(Text_tray_validate, sText);
                          }

                
         }

        
          strcat(Text_tray_validate, "/\n");
          send_tray_proposite(Text_tray_validate);
     
} 


void *Mise_J_Plat(void *arg);
///***************************************************************************************************************************
///***************************************************************************************************************************
///****************************************************   initialiser le tray   **********************************************
///***************************************************************************************************************************
///***************************************************************************************************************************

void *New_Tour(void* parameters){

      struct Parms_Tour* commande = (struct Parms_Tour*) parameters;

      char tmp[400];
      strcpy(tmp , commande->Tour);
      char p[300] ="";
      char t[10] ="";
      sub_String(tmp,5,229,p);
      sub_String(tmp,231,strlen(tmp)-3,t);
      
      strcpy(Proposed_tray,p);

          //***   mettre a jour la table 

         pthread_cancel(thr_TRAY);
         if(pthread_create(&thr_TRAY, NULL, Mise_J_Plat, NULL ) == -1) { }
         
         strcpy(Proposed_solution,t);
              
          //***   mettre a jour la table de tirage 

        for (int i = 0; i < 7; ++i)
        {
          char car[2] ="" ;
          car[0] = Proposed_solution[i];
          gtk_entry_set_text(GTK_ENTRY (my_tab_tirage->champs[i]), car);
        }
              
        pthread_exit(NULL);
}

//**********************************************************************************************************************************************
//**********************************************************************************************************************************************
                                                              // reception des cammandes de serveur
//**********************************************************************************************************************************************
//**********************************************************************************************************************************************
void thread_recev_commandes(char buf_recev[1000]){

        char Buf_Param[1000] = "";

              if (strstr(buf_recev, "SESSION") != NULL){ // ***** SESSION/
                      gtk_label_set_text(GTK_LABEL(labal_erreur),"Début d'une nouvelle Session");
                      pthread_cancel(thread_Time);
                      temps_att = 300;
                      if(pthread_create(&thread_Time, NULL, thread_time, NULL) == -1) { }
              }else

              // if (strcmp(commande , "VAINQUEUR") == 0){// ***** VAINQUEUR/bilan/
              // printf(" VAINQUEUR/bilan/ !\n");
               //strcpy(bilan_recev, "");
              // sub_String(buf_recev, 10, strlen(buf_recev)-3,bilan_recev);
              // if(pthread_create(&Thread_Bilan, NULL, Maj_Bilan, NULL ) == -1) { }
              // }else

              if (strstr(buf_recev, "TOUR") != NULL){// ***** TOUR/plateau/tirage/
                      gtk_label_set_text(GTK_LABEL(labal_erreur),"Phase de recherche");
                      pthread_cancel(thread_Time);
                      temps_att = 300;
                      if(pthread_create(&thread_Time, NULL, thread_time, NULL) == -1) { }
                      strcpy(thread1_args.Tour , buf_recev);
                      pthread_cancel(thr);
                      if(pthread_create(&thr, NULL, New_Tour, &thread1_args ) == -1) { }
              }else

              if (strstr(buf_recev, "RVALIDE") != NULL){// ***** RVALIDE/
                      pthread_cancel(thread_Time);
                      temps_att = 120;
                      if(pthread_create(&thread_Time, NULL, thread_time, NULL) == -1) { }
                     gtk_label_set_text(GTK_LABEL(labal_erreur),"Phase de recherche : votre réponse est validé ");
              }else

              if (strstr(buf_recev, "RINVALIDE") != NULL){// ***** RINVALIDE/raison/
                char aff[50] = "Phase de recherche : "; 
                      strcpy(Buf_Param,"");
                      sub_String(buf_recev,13,strlen(buf_recev)-3,Buf_Param);
                      strcat(aff, Buf_Param);
                      gtk_label_set_text(GTK_LABEL(labal_erreur),aff);
              }else

              if (strstr(buf_recev, "RATROUVE") != NULL){// ***** RATROUVE/user/
                      pthread_cancel(thread_Time);
                      temps_att = 120;
                      if(pthread_create(&thread_Time, NULL, thread_time, NULL) == -1) { }
                      strcpy(Buf_Param,"");
                      sub_String(buf_recev,9,strlen(buf_recev)-3, Buf_Param);
                      strcat(Buf_Param," : a trouve un mot. fin de phase recherche ");
                      gtk_label_set_text(GTK_LABEL(labal_erreur),Buf_Param);
              }else


              if (strstr(buf_recev, "RFIN") != NULL){// ***** RFIN/ 
                      pthread_cancel(thread_Time);
                      temps_att = 120;
                      if(pthread_create(&thread_Time, NULL, thread_time, NULL) == -1) { }
                      gtk_label_set_text(GTK_LABEL(labal_erreur),"Fin de la phase de recherche "); 
              }else

              if (strstr(buf_recev, "SVALIDE") != NULL){// ***** SVALIDE/
                      gtk_label_set_text(GTK_LABEL(labal_erreur),"Phase de soumission : votre réponse est validé ");
              }else

              if (strstr(buf_recev, "SINVALIDE") != NULL){// ***** SINVALIDE/raison/ 
                      char aff[50] = "Phase de soumission : "; 
                      strcpy(Buf_Param,""); 
                      sub_String(buf_recev,13,strlen(buf_recev)-3,Buf_Param);
                      strcat(aff, Buf_Param);
                      gtk_label_set_text(GTK_LABEL(labal_erreur),aff);
              }else

              if (strstr(buf_recev, "SFIN") != NULL){// ***** SFIN/  
                      pthread_cancel(thread_Time);
                      gtk_label_set_text(GTK_LABEL(labal_erreur),"Fin Soumission. Passer au Resultats.");
              } else

              if (strstr(buf_recev, "BILAN") != NULL){// ***** BILAN/mot/vainqueur/scores/
                      pthread_cancel(thread_Time);
                      temps_att = 10;
                      if(pthread_create(&thread_Time, NULL, thread_time, NULL) == -1) { }
                      gtk_label_set_text(GTK_LABEL(labal_erreur),"Phase de Resultats ");
                      Affiche_Bilan(buf_recev);
              }else 

              if ( strstr(buf_recev, "DECONNEXION") != NULL ){// ***** DECONNEXION/user/  
                      strcpy(Buf_Param_CONECT,"");
                      sub_String(buf_recev,12,strlen(buf_recev)-3,Buf_Param_CONECT);
                      Thread_Players(Buf_Param_CONECT , 0);
              }else

  
              if ( strstr(buf_recev, "CONNECTE") != NULL){// ***** CONNECTE/user/ 
                      strcpy(Buf_Param_CONECT,"");
                      sub_String(buf_recev,9,strlen(buf_recev)-3,Buf_Param_CONECT);
                      Thread_Players(Buf_Param_CONECT , 1);
              }else
 
              if (strstr(buf_recev, "PRECEPTION") != NULL){// ***** PRECEPTION/message/user/
                      is_msg_priv = 1;
                      strcpy(Buf_Param,"");
                      strcpy(message_tmp,"");
                      sub_String(buf_recev,11,strlen(buf_recev)-3,message_tmp);
                      if(pthread_create(&thread_maj_message, NULL, maj_message, NULL ) == -1) { }
              }else

              if (strstr(buf_recev, "RECEPTION") != NULL){// ***** RECEPTION/message/
                      is_msg_priv = 0;
                      strcpy(Buf_Param,"");
                      strcpy(message_tmp,"");
                      sub_String(buf_recev,10,strlen(buf_recev)-3,message_tmp);
                      if(pthread_create(&thread_maj_message, NULL, maj_message, NULL ) == -1) { }
              } 
 



}

void initialiser_var();


int is_modif_autorise(gpointer data){
      int rp = 1 ;
      for (int i = 0; i < 225; ++i)
      {
        if (tj->tab_tirage[i] == data)
        {
          rp = 0;
        }
      }
      
      return rp ;
  
}

void trait_signal(GtkWidget *pButton, gpointer data){
   
  int i = strlen(Tampo_char);

  if(is_modif_autorise(data) == 0){ //** tester si la modif est autorise 

        strcpy(Tampo_char, "");
        Tampo_Button = NULL;

  }else{
              //**** si le tampo est vide
      if (i == 0)
      {

         strcpy(Tampo_char, gtk_entry_get_text (data));
         Tampo_Button = data;
           

      }else{   //** si le tampo n'est pas vide 

             if(strlen(gtk_entry_get_text (data)) == 0){
                    
                     gtk_entry_set_text(data, Tampo_char);
                     gtk_entry_set_text((gpointer)Tampo_Button, "");
                     strcpy(Tampo_char, "");
              }else{
                
                  strcpy(Tampo_char, "");
                  Tampo_Button = NULL;
              }
        
       }
         
  }
  
  
}

void add_signal(){

        for (int i = 0; i < 7; ++i)
        {
          
                    char B[10] = "B_" ;
                     
                        char T_B[3];
                        sprintf(T_B, "%d", i);
                        strcat(B, T_B);

                    char T[10] = "tray_" ;

                        char T_T[3];
                        sprintf(T_T, "%d", i);
                        strcat(T, T_T);
                        
                      g_signal_connect ( GTK_WIDGET(gtk_builder_get_object(builder_principale, B)) , "clicked", G_CALLBACK (trait_signal), 
                                        (gpointer)  GTK_ENTRY (gtk_builder_get_object (builder_principale, T ))  );
        }

       for (int i = 0; i < 15; ++i)
       {
               char ind_i[5] = "" ;
               sprintf(ind_i, "%d", i);

         for (int j = 0; j < 15; ++j)
         {
               char ind_j[5] = "";
               sprintf(ind_j, "%d", j);
              
               char B[10] = "B_";
               strcat(B, ind_i); strcat(B, "_");  strcat(B, ind_j);
              

               char T[20] = "Plateau_";
               strcat(T, ind_i); strcat(T, "_");  strcat(T, ind_j);

             g_signal_connect ( GTK_WIDGET(gtk_builder_get_object(builder_principale, B)) , "clicked", G_CALLBACK (trait_signal), 
                               (gpointer)  GTK_ENTRY (gtk_builder_get_object (builder_principale, T ))  );
         }
       }
  
}

//**********************************************************************************************************************************************
//**********************************************************************************************************************************************
                                                              // jeux de tests
//**********************************************************************************************************************************************
//**********************************************************************************************************************************************

void fonc_test(int i ) {
  
switch (i)

{

case 0:
gtk_label_set_text( (gpointer) C_Test , "SESSION/");
thread_recev_commandes("SESSION/\n");
break;

case 1:
gtk_label_set_text( (gpointer) C_Test , "RECEPTION/Salut tout .... /");
thread_recev_commandes("RECEPTION/Salut tout .... /\n");
break;
case 2: 
gtk_label_set_text( (gpointer) C_Test , "RECEPTION/Salut ca va ?? /");
thread_recev_commandes("RECEPTION/Salut ca va ?? /\n");
break;

case 3:
gtk_label_set_text( (gpointer) C_Test , "PRECEPTION/Salut Monsieur ca va ??? /TOTO/");
thread_recev_commandes("PRECEPTION/Salut Monsieur ca va ??? /TOTO/\n");
break;
case 4:
  gtk_label_set_text( (gpointer) C_Test , "CONNECTE/GHERSA/");
  thread_recev_commandes("CONNECTE/GHERSA/\n");
break;
case 5:
  gtk_label_set_text( (gpointer) C_Test , "CONNECTE/SOFIANE/");
  thread_recev_commandes("CONNECTE/SOFIANE/\n");
break;
case 6:
 gtk_label_set_text( (gpointer) C_Test , "DECONNEXION/GHERSA/");
  thread_recev_commandes("DECONNEXION/GHERSA/\n");
break;
case 7:
gtk_label_set_text( (gpointer) C_Test , "RINVALIDE/POS le caracter la case 5 10 erreur/");
  thread_recev_commandes("RINVALIDE/POS le caracter la case 5 10 erreur/\n");
break;
case 8:
gtk_label_set_text( (gpointer) C_Test , "RINVALIDE/DIC mot n'existe pas /");
  thread_recev_commandes("RINVALIDE/DIC mot n'existe pas /\n");
break;
case 9:
gtk_label_set_text( (gpointer) C_Test , "RFIN/");
  thread_recev_commandes("RFIN/\n");
break;
case 10:
gtk_label_set_text( (gpointer) C_Test , "SESSION/");
thread_recev_commandes("SESSION/\n");
break;
case 11:
gtk_label_set_text( (gpointer) C_Test , "RVALIDE/");
  thread_recev_commandes("RVALIDE/\n");
break;
case 12:  
 gtk_label_set_text( (gpointer) C_Test , "CONNECTE/TOTO/");
  thread_recev_commandes("CONNECTE/TOTO/\n");
break;
case 13:
gtk_label_set_text( (gpointer) C_Test , "PRECEPTION/ Qui a trouve ce mot ?? /TATA/");
thread_recev_commandes("PRECEPTION/ Qui a trouve ce mot ?? /TATA/\n");
break;
case 14:
gtk_label_set_text( (gpointer) C_Test , "SINVALIDE/INF Score avant est meilleur/");
thread_recev_commandes("SINVALIDE/INF Score avant est meilleur/\n");
break;
case 15:
 gtk_label_set_text( (gpointer) C_Test , "SVALIDE/");
thread_recev_commandes("SVALIDE/\n");
break;
case 16:
gtk_label_set_text( (gpointer) C_Test , "SFIN/");
thread_recev_commandes("SFIN/\n");
break;
case 17:
 gtk_label_set_text( (gpointer) C_Test , "BILAN/BONJOUR/GHERSA/5*Login*100*JOUEUR_1*SCORES_1*JOUEUR_2*SCORES_2*JOUEUR_3*SCORES_3/");
thread_recev_commandes("BILAN/BONJOUR/GHERSA/5*Login*100*JOUEUR_1*SCORES_1*JOUEUR_2*SCORES_2*JOUEUR_3*SCORES_3/\n");
break;
case 18:
 gtk_label_set_text( (gpointer) C_Test , "BILAN/BONJOUR/GHERSA/6*Login*150*KASDI*10*SOFIANE*66*HACENE*99/");
thread_recev_commandes("BILAN/SALUT/Login/6*Login*150*KASDI*10*SOFIANE*66*HACENE*99/\n");
break;
case 19:
 gtk_label_set_text( (gpointer) C_Test , "BILAN/BONJOUR/GHERSA/7*Login*50*JOUEUR_1*SCORES_1*JOUEUR_2*SCORES_2*JOUEUR_3*SCORES_3/");
thread_recev_commandes("BILAN/BONJOUR/GHERSA/7*Login*50*JOUEUR_1*SCORES_1*JOUEUR_2*SCORES_2*JOUEUR_3*SCORES_3/\n");
break;
default:  

 gtk_label_set_text( (gpointer) C_Test , "***  FIN DE TEST ***");
  break;

}
id_test ++ ;


}


static void Lancer_tests (GtkWidget *pButton, gpointer data){

fonc_test(id_test);


}
//**********************************************************************************************************************************************
//**********************************************************************************************************************************************
                                                              // Main de window
//**********************************************************************************************************************************************
//**********************************************************************************************************************************************
void Open_window(int skt, char My_n[400],char rep[20], char tr[400], char sc[400], char ph[400], char time_rest[5]){


          strcpy(Tampo_char , "");

      //********** initialese les table 

      ///**** table des cases interdit a modifier 
        tj = (tirage_jeu*) malloc(sizeof(tirage_jeu));
        tj->tab_tirage = (GtkWidget**) malloc(sizeof(GtkWidget*)*225);
      ////*** table des cases de tray 
        tray_s_v = (tray_s*) malloc(sizeof(tray_s));
        tray_s_v->tab_tray_s = (GtkWidget**) malloc(sizeof(GtkWidget*)*225);
      ////******* table des message 
        my_tab_message = (tab_message*) malloc(sizeof(tab_message));
        my_tab_message->champs = (GtkWidget**) malloc(sizeof(GtkWidget*)*25);
      ////******* table des cases de tirage 
        my_tab_tirage = (tab_tirage*) malloc(sizeof(tab_tirage));
        my_tab_tirage->champs = (GtkWidget**) malloc(sizeof(GtkWidget*)*7);
      ////******* table des joueur 
        my_tab_j = (tab_j*) malloc(sizeof(tab_j));
        my_tab_j->champs = (GtkWidget**) malloc(sizeof(GtkWidget*)*15);
      ////******* table des scores 
        my_tab_s = (tab_s*) malloc(sizeof(tab_s));
        my_tab_s->champs = (GtkWidget**) malloc(sizeof(GtkWidget*)*15);
 

//**** LES VARIABLE RECU 
        sock = skt ;
        strcpy(My_name , "  "); 
        strcat(My_name , My_n);
        strcpy(My_name_j , My_n);
        strcat(My_name , "  : "); 
        strcpy(Proposed_solution, rep);
        strcpy(Proposed_tray, tr);
        my_score = 0;
        strcpy(time_reste, time_rest);
        temps_att = 300;//*** le temps a attendre 

        builder_principale = gtk_builder_new();
        gtk_builder_add_from_file (builder_principale, "Window/MainTest.glade", NULL);
        fenetre_principale = GTK_WIDGET(gtk_builder_get_object(builder_principale, "window_main"));
// recuperer les lien vers les label    *******************************
        g_signal_connect (G_OBJECT (fenetre_principale), "destroy", (GCallback)gtk_main_quit, NULL);
        labal_erreur =  GTK_WIDGET(gtk_builder_get_object(builder_principale, "MSG_Serveur"));
        labal_timer =  GTK_WIDGET(gtk_builder_get_object(builder_principale, "lbl_Time"));
        lbl_Joueur =  GTK_WIDGET(gtk_builder_get_object(builder_principale, "lbl_Joueur"));
        Afficher_message = GTK_WIDGET(gtk_builder_get_object(builder_principale, "Receive"));
        Aff_sco = GTK_WIDGET(gtk_builder_get_object(builder_principale, "My_score" ));
C_Test = GTK_WIDGET(gtk_builder_get_object(builder_principale, "C_Test" ));
        M_m = GTK_WIDGET(gtk_builder_get_object(builder_principale, "M_Mot"));
        M_j = GTK_WIDGET(gtk_builder_get_object(builder_principale, "M_Joueur"));
        M_t = GTK_WIDGET(gtk_builder_get_object(builder_principale, "N_Tour"));

        is_m = GTK_WIDGET(gtk_builder_get_object(builder_principale, "is_m"));
        gtk_label_set_text ( (gpointer) is_m, "");


        initialiser_var();//*** initialiser les variable pour le tray 
        add_signal();//*** affecter le signal pour tout les button
    

//******************************************************
// signal pour les message 
        g_signal_connect ( gtk_builder_get_object (builder_principale, "Send"), "clicked", G_CALLBACK (snd_msg_bod),
                                (gpointer) gtk_entry_get_text ( GTK_ENTRY (gtk_builder_get_object (builder_principale, "Send_Message")) )  );


        g_signal_connect ( gtk_builder_get_object (builder_principale, "Send"), "clicked", G_CALLBACK (snd_msg_dist),
                                (gpointer) gtk_entry_get_text ( GTK_ENTRY (gtk_builder_get_object (builder_principale, "Pour")) )  );

//******** signal pour valider le plateau   ****************************************************************************************

       pVBox_tray = (gpointer) gtk_builder_get_object (builder_principale, "pVBox_tray");
       g_signal_connect ( gtk_builder_get_object (builder_principale, "Validate"),
                           "clicked", G_CALLBACK (Lancer_tests), NULL );


//******************************* message bienvenue    
        
        Fill_Print();// remplir la bar de tirage 
        Fill_Tray(Proposed_tray);// remplir le plateau


  
//******** lancer la bare de temps    ****************************************************************************************

        Time = (gpointer)gtk_builder_get_object (builder_principale, "Time");


        gtk_builder_connect_signals(builder_principale, NULL);
        g_object_unref(builder_principale);
        gtk_widget_show(fenetre_principale);                
        gtk_main();

}


///***************************************************************************************************************************
///***************************************************************************************************************************
///****************************************************   METTRE A JOUR LES VARIABLE **********************************************
///***************************************************************************************************************************
///***************************************************************************************************************************

void initialiser_var(){


      //*** table des ponteur vers les case de tirage 

        int ind = 0;
        for (int i = 0; i < 15; ++i)
         {
                 char ind_i[5] = "" ;
                 sprintf(ind_i, "%d", i);

                 for (int j = 0; j < 15; ++j)
                 {
                       char ind_j[5] = "";
                       sprintf(ind_j, "%d", j);
                                      

                       char T[20] = "Plateau_";
                       strcat(T, ind_i); strcat(T, "_");  strcat(T, ind_j);

                        tray_s_v->tab_tray_s[ind] = GTK_WIDGET(gtk_builder_get_object(builder_principale, T)) ;
                        ind ++;
                     
                 }
         }
      //*** table des ponteur vers les case de tirage 

              my_tab_tirage->champs[0] = GTK_WIDGET(gtk_builder_get_object(builder_principale, "tray_0" ));
              my_tab_tirage->champs[1] = GTK_WIDGET(gtk_builder_get_object(builder_principale, "tray_1" ));
              my_tab_tirage->champs[2] = GTK_WIDGET(gtk_builder_get_object(builder_principale, "tray_2" ));
              my_tab_tirage->champs[3] = GTK_WIDGET(gtk_builder_get_object(builder_principale, "tray_3" ));
              my_tab_tirage->champs[4] = GTK_WIDGET(gtk_builder_get_object(builder_principale, "tray_4" ));
              my_tab_tirage->champs[5] = GTK_WIDGET(gtk_builder_get_object(builder_principale, "tray_5" ));
              my_tab_tirage->champs[6] = GTK_WIDGET(gtk_builder_get_object(builder_principale, "tray_6" ));

             
      //*** table des message a afficher 

                 for (int i = 0; i < 14; ++i)
                  {

                        char T_dst[30] = "Champs_message_" ;
                        char T_T_dst[3];
                        sprintf(T_T_dst, "%d", i);
                        strcat(T_dst, T_T_dst);

                    my_tab_message->champs[i] = (gpointer)  gtk_builder_get_object (builder_principale, T_dst) ;
                  }



      //*** table des joueurs avec leurs scores

                 for (int i = 0; i < 15; ++i)
                  {

                        char T_dst[30] = "N_J_" ;
                        char T_dst_s[30] = "S_J_" ;
                        char T_T_dst[3];
                        sprintf(T_T_dst, "%d", i);
                        strcat(T_dst, T_T_dst);
                        strcat(T_dst_s, T_T_dst);

                    my_tab_j->champs[i] = (gpointer)  gtk_builder_get_object (builder_principale, T_dst) ;
                    my_tab_s->champs[i] = (gpointer)  gtk_builder_get_object (builder_principale, T_dst_s) ;
                  }


      //*** champs de nom de joueur ET son score

                gtk_label_set_text((gpointer) gtk_builder_get_object (builder_principale, "My_name") , My_name );

                  

}

///***************************************************************************************************************************
///***************************************************************************************************************************
///****************************************************   METTRE A JOUR LE TRAY **********************************************
///***************************************************************************************************************************
///***************************************************************************************************************************
void m_j(char car, GtkWidget *p){
   char str_tmp[5] = "";
   str_tmp[0] = car;
          if (car != '0')
          {
            // gtk_editable_set_editable (GTK_EDITABLE (p), FALSE);
            gtk_entry_set_text(GTK_ENTRY (p), str_tmp);
          }else{
            // gtk_editable_set_editable (GTK_EDITABLE (p), TRUE);
            gtk_entry_set_text(GTK_ENTRY (p), "");
          }
}

void *Mise_J_Plat(void *arg){
            
  int ind = 0 ;
  for (int i = 0; i < 225; ++i) {

        m_j(Proposed_tray[i] , tray_s_v->tab_tray_s[i] );
        tj->tab_tirage[i] = NULL;
        char sText[10] = "";
              /* Recuperation du texte contenu dans le GtkEntry */
              strcpy(sText, gtk_entry_get_text(GTK_ENTRY(tray_s_v->tab_tray_s[i])));
              
              if (strlen(sText) == 0 )
              {
                
              }else{
                   
                   tj->tab_tirage[ind] = tray_s_v->tab_tray_s[i]; 
                    ind++;
              }

  }
                     
  //********************
  (void) arg;
  pthread_exit(NULL);
}