
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

//************************************************************************************************************
//**************************   Import File  ******************************************************************
 

#include "Function/Function_Main.h"
#include "Function/Function_Connection.h"


//************************************************************************************************************
//**************************   Main  *************************************************************************

int main(int argc, char *argv [])
    {

/* Initialisation de la librairie Gtk. */
      gtk_init(&argc, &argv);
      Open_Connection();
  
      return 0;
    }

