
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
 

#include "Function_Connection.h"
#include "Function_Main.h"

    int sock;
    struct sockaddr_in server;

//************************************************************************************************************
//*******************************    variable fenetre connexion **********************************************
      GtkWidget *fenetre_connexion = NULL;
      GtkBuilder *builder_connexion = NULL;
      GError *error_Chargement_fichier_connexion = NULL;
      gchar *filename_connexion = NULL;
      GtkWidget *connexion_lbl_error;
      char buf_connect[500] ;

      char My_Name[200];

static void cb_quit (GtkWidget * p_wid, gpointer p_data) { gtk_main_quit (); } 
void msg_error(char *msg) { gtk_label_set_text(GTK_LABEL(connexion_lbl_error), msg); }

void subString(const char *chaine, int debut, int fin, char *result){ 
    result[fin+1-debut]='\0';
    memcpy(result,(char *)chaine+debut,fin+1-debut); 
    
}

static void cb_ok (GtkWidget * p_wid, gpointer p_data)
        {
           
           char  con[256]    = "CONNEXION/";
           strcpy(My_Name , p_data);
           const char  * p_text    = p_data;
           strcat(con, p_text);

          if (p_text != NULL ) { 
                        
                        sock = socket(AF_INET , SOCK_STREAM , 0);
                        if (sock == -1) { msg_error("Could not create socket"); }
                        server.sin_addr.s_addr = inet_addr("127.0.0.1");
                        server.sin_family = AF_INET;
                        server.sin_port = htons(2017);
                        if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
                        {
                            msg_error("Error de connexion avec le serveur ");
                             
                        }else{
                           strcat(con, "/\n");
                           int leng = strlen(con);
                           int vv = write(sock,con, leng );
                           fflush(stdout);
                    if (vv != 0)
                    {
                     
                   
                           int v = read(sock,buf_connect,500);
                   if (v != 0)
                   {
                             
                         
                           if (strncmp(buf_connect,"REFUS/",5) == 0)
                           {
                             msg_error("Ce nom de joueur est exist");
                           }else{
                            char tray[300];
                            char tirage[10];
                            char score[200] ;
                            char phase[10];
                            char time_rest[5];
                            char tmp[300];
                              //** BIENVENUE/placement/tirage/scores/phase/temps/
                            subString(buf_connect, 10 , 234 , tray);
                            subString(buf_connect, 236 , 242 , tirage);
                            subString(buf_connect, 244 , strlen(buf_connect)-3 , tmp);
                            char t[200] = "";
                            int pos = 244 ;
                            //***********************************
                            for (int i = 244; i < strlen(buf_connect)-2; ++i)
                            {
                              pos++;
                              if (buf_connect[i] != '/')
                              {
                                 char str_tmp[5] = "";
                                sprintf(str_tmp, "%c", buf_connect[i]);
                                strcat(t,str_tmp);
                                
                              }else{

                                break;
                              }
                            }
                           strcpy(score, t); 
                           strcpy(t,"");

                           //************************************
                           for (int i = pos; i < strlen(buf_connect)-2; ++i)
                           {
                             pos++;
                             if (buf_connect[i] != '/')
                              {
                                 char str_tmp[5] = "";
                                sprintf(str_tmp, " %c", buf_connect[i]);
                                strcat(t,str_tmp);
                               
                              }else{
                                break;
                              }
                           }
                           strcpy(phase, t); 
                           strcpy(t,"");

                           //*************************************

                          
                           for (int i = pos; i < strlen(buf_connect)-2; ++i)
                           {
                             pos++;
                             if (buf_connect[i] != '/')
                              {
                                 char str_tmp[5] = "";
                                sprintf(str_tmp, " %c", buf_connect[i]);
                                strcat(t,str_tmp);
                               
                              }else{
                                break;
                              }
                           }
                           strcpy(time_rest, t); 
                           strcpy(t,"");
                           //*** ouvert la fenetre
                           
                                                     Open_window(sock, My_Name, tirage, tray, score, phase, time_rest);
                           }
                           
                            
                        }
                }
                }
           }
        }

void Open_Connection(){
/* Ouverture du fichier Glade de la fenêtre connexion */
      builder_connexion = gtk_builder_new();
      filename_connexion =  g_build_filename ("Window/Connection.glade", NULL);
      gtk_builder_add_from_file (builder_connexion, filename_connexion, &error_Chargement_fichier_connexion);
      g_free (filename_connexion);
      
      fenetre_connexion = GTK_WIDGET(gtk_builder_get_object (builder_connexion, "WindowConnexion"));
// les evenements     ******************************************* 
       connexion_lbl_error = GTK_WIDGET(gtk_builder_get_object(builder_connexion, "Lab_Connexion"));
       g_signal_connect (G_OBJECT (fenetre_connexion), "destroy", (GCallback)gtk_main_quit, NULL);

       g_signal_connect ( gtk_builder_get_object (builder_connexion, "Quitter_Connexion"),
                           "clicked", G_CALLBACK (cb_quit), NULL );

       g_signal_connect ( gtk_builder_get_object (builder_connexion, "Valider_Connexion"), "clicked", G_CALLBACK (cb_ok),
                          (gpointer) gtk_entry_get_text ( GTK_ENTRY (gtk_builder_get_object (builder_connexion, "Name_Connexion")) )
                       );
       gtk_widget_show_all (fenetre_connexion);
       gtk_main();

}