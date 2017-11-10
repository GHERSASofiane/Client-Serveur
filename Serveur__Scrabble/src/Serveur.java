import java.net.*;
import java.io.*;
import java.util.*;
import java.util.Map.Entry;

import javax.swing.plaf.SliderUI;


//****	Classe principale du serveur
public class Serveur {
	
		static Serveur Mon_Serveur = new Serveur();
		static int port =2017; // si pas d'argument : port 2017 par défaut
	//** time 
		private static int time_lance ;//** temps a attend avant de lancer une session 
		private static int time_relance ;//** temps a attend avant de relancer une nouvelle session
		private static int time_search ;//** temps a attend dans la phase de recherche 
		private static int time_subm ;//** temps a attend dans la phase de soumission
		private static int time_result ;//** temps a attend dans la phase de resultat
	//** thread a rebours
		private static Relance_session r_session = new Relance_session(Mon_Serveur, time_relance);
		private static Rebours_interne r_interne = new Rebours_interne(Mon_Serveur, time_lance) ;
		private static Rebours_long r_search = new Rebours_long(Mon_Serveur, time_search);
		private static Rebours_court r_submit = new Rebours_court(Mon_Serveur, time_subm);
		private static Rebours_resultat r_resultat = new Rebours_resultat(Mon_Serveur, time_result);
	//** les boolean
		private static String phase = "DEB"; // REC SOU RES
	//** info des joueurs
		private static Map<String, PrintWriter> tab_print = new HashMap<>();
		private static Map<String, Integer> tab_scor = new HashMap<>();//** le scores de tour
		private static Map<String, Integer> tab_scor_sess = new HashMap<>();//** le scores de la session
		private static Map<String, String> tab_tray = new HashMap<>();//** les tray envoyer a cheque tour par un joueur
	//** info des mieur scors
		private static String Best_word ="";
		private static String Best_player ="";
		private static int Best_score = 0;
		private static int Nb_Tour = 0;//** nombre de tour dans la session 
	
	//*** les donnéer de serveur
	    private static String tray = ""; //** plateau
	    private static String tirage = ""; //** les littre  a utilisée
    //** reservoir de lettres 
	    private static String STR_FOR_tirage = ""; //** notre reservoir de lettre
    


//******************************************************************************************************************************
//******************************************************************************************************************************
//***********************************		 Main	 	*************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

public static void main(String[] args) {
   		
   		
	try {
		
		//***** lire le temps t'attend 
		boolean is_mod = Read_time.is_modifier();
		if (is_mod) {
			time_lance = Read_time.lance();
		    time_relance = Read_time.relance() ;
		    time_search = Read_time.search() ;
		    time_subm = Read_time.subm() ;
		    time_result = Read_time.result();
		} else {

			time_lance = 20 ;
		    time_relance = 120 ;
		    time_search = 300 ;
		    time_subm = 120 ;
		    time_result = 10 ;
		}
		
		
	   		new Commandes_Serveur(Mon_Serveur);
		   	ServerSocket Socket_Serveur = new ServerSocket(port); // ouverture d'un socket serveur
		    Fonction.Welcome(port);		    
		    
		    
		    while (true){ // attente de connexion 
		    	new Client_Thread(Socket_Serveur.accept(),Mon_Serveur); // un client se connecte, un nouveau thread client est lancé
		   	}
   		    
	} catch (Exception e) {}
   		

}	

//******************************************************************************************************************************
//******************************************************************************************************************************
//***********************************		 Set and Get	 	****************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

synchronized public static String get_tray(){ return tray; }
synchronized public static String get_tirage(){ return tirage; }
synchronized public static int getNbjoueurs() { return tab_print.size(); }
synchronized public static String get_phase() { return phase; }

//*** methode d'ajout le score ou de le mettre a jour 
synchronized public static void set_Score(String user, int score, String data, String Word, String ph){ 
	  
	  PrintWriter tmp = tab_print.get(user);
	  if (score >  tab_scor.get(user)) {
					if (ph.equals("SOU")) {
						//******** SVALIDE/
						  tmp.print("SVALIDE/\n");
						  tmp.flush();
					} else{
						//******** RVALIDE/
						  tmp.print("RVALIDE/\n");
						  tmp.flush();
					}
		
		  tab_scor.replace(user, score); 
		  tab_tray.replace(user, data);
		  //** enregistre les best scores et user qui trouve le mot
				  if ( (Best_score < score) || (Best_score == 0) ) {
					  Best_player = user;
					  Best_score = score;
					  Best_word = Word;
				  }
		  
	  }else {
		  
		  String raison = "INF Score avant est meilleur"; 
		if (ph.equals("SOU")) {
			//******** SINVALIDE/raison/
			tmp.print("SINVALIDE/"+raison+"/\n");
			tmp.flush();
		} else {
			//******** RINVALIDE/raison/
			tmp.print("RINVALIDE/"+raison+"/\n");
			tmp.flush();
		}
		
		
	}
} 

//** recuperer les nom et scores de tout les joueurs
public static String get_scores(boolean is_final){
	String message = "";
	if (is_final) {
		
		for(Entry<String, Integer> entry : tab_scor_sess.entrySet()){
			  String cle = entry.getKey();
			  message += cle;
			  message += "*";
			  message += tab_scor_sess.get(cle)+"*";
			  
	     }
	} else {
		
		for(Entry<String, Integer> entry : tab_scor.entrySet()){
			  String cle = entry.getKey();
			  message += cle;
			  message += "*";
			  message += tab_scor.get(cle)+"*";
		}
		
	}
	
	String tmp ="";
	for (int j = 0; j < message.length(); j++) {
		if (j!=message.length()-1) {
			tmp += message.charAt(j);
		}
	}
	message = tmp;
return message;//*** user1*score1*.....*userN*scoreN
}
//*********************************************************************************************************
//**** nouveaux tirage 
static void new_tirage(String tirage_init){ 

		if ((7-tirage_init.length()) > (STR_FOR_tirage.length())) {
			end_session(true);// si buf est vide fin session 
		} else {
			 for (int i = tirage_init.length(); i < 7; i++) {
					int k = (int) (Math.random() * (  STR_FOR_tirage.length() - 0 ));
					tirage_init += STR_FOR_tirage.charAt(k);
					// supprimer l'element k 
					String tmp ="";
					for (int j = 0; j < STR_FOR_tirage.length(); j++) {
						if (j!=k) {
							tmp += STR_FOR_tirage.charAt(j);
						}
					}
					STR_FOR_tirage = tmp;

				}
			tirage = tirage_init;
		}

}



//******************************************************************************************************************************
//******************************************************************************************************************************
//***********************************		 Add and remove player	 	********************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************

synchronized public static void add_jouer(String user, PrintWriter out) {
		  boolean exist = false;
		  
		  for(Entry<String, PrintWriter> entry : tab_print.entrySet()){
					  String cle = entry.getKey();
					  if (user.equals(cle)) { exist = true; }
		  }
		  if (exist) { //************* REFUS/
					  out.print("REFUS/\n");
				      out.flush(); // envoi dans le flux de sortie
		  }else {
			  		tab_print.put(user, out);
			  		tab_scor.put(user, 0);
			  		tab_scor_sess.put(user, 0);
			  		tab_tray.put(user, "");
			  		
						if (getNbjoueurs() == 1) { 
							phase = "DEB";
							STR_FOR_tirage = Fonction.generate_tirage();
							tray = Fonction.generate_tray();
						  	new_tirage("");
						  	r_interne = new Rebours_interne(Mon_Serveur, time_lance);
						  	r_interne.start();
						}
						
							sendAll__CONNECTE_user(user);
		 }
		  
}

synchronized public static void delete_jouer(String user) {
		
		tab_print.remove(user);
		tab_scor.remove(user);
		tab_scor_sess.remove(user);
		tab_tray.remove(user);
		
		if (tab_print.size() == 0) {
			 end_session(false); //*** fermer la session			
		}else {
			sendAll__DECONNEXION_user(user); 
		}
		
}

synchronized public static void sendAll__CONNECTE_user(String user){
		 PrintWriter out;
	for(Entry<String, PrintWriter> entry : tab_print.entrySet()) {
			    String cle = entry.getKey();
			    out = entry.getValue();

			if(out != null){ // sécurité
					    if (!cle.equals(user)){
		//********* CONNECTE/user/

					        out.print("CONNECTE/"+user+"/\n");
					        out.flush(); // envoi dans le flux de sortie
					        
					      }else{ 
		//********* BIENVENUE/placement/tirage/scores/phase/temps/  to do new implement
					    	  int tmp_time = 0;
					    	  if (phase.equals("DEB")) {
					    		  tmp_time = r_interne.get_time();
							  }else if (phase.equals("REC")) {
								  tmp_time = r_search.get_time();
							  }else if (phase.equals("SOU")) {
								  tmp_time = r_submit.get_time();
							  }else if (phase.equals("RES")) {
								  tmp_time = r_resultat.get_time();
							  }

					    	  
					    	String str_send = "BIENVENUE/"+tray+"/"+tirage+"/"+get_scores(false)+"/"+phase+"/"+tmp_time+"/\n";
					    	
					    	out.print(str_send);
					        out.flush();
					    	str_send = "";
					    	
							
					    	}
			}
	}
}

synchronized public static void sendAll__DECONNEXION_user(String user){
		PrintWriter out;

		 for(Entry<String, PrintWriter> entry : tab_print.entrySet()) {
			    String cle = entry.getKey();
			    out = entry.getValue();
			    
				    if (out != null) // sécurité
				      {
				    	//******** DECONNEXION/user/
				        out.print("DECONNEXION/"+user+"/\n");
				        out.flush(); 
				      }

			}
}

//** methode d'envoi un message a tout sauf user	
synchronized public static void send_msg_of_all_EXCEPT_user(String  message, String user){
		
		PrintWriter out;

		 for(Entry<String, PrintWriter> entry : tab_print.entrySet()) {
			    String cle = entry.getKey();
			    out = entry.getValue();
			    
				 if(out != null){   
					    if (!cle.equals(user) ) // sécurité
					      {
						        out.print(message);
						        out.flush(); 
					      }
				 }   
		}
}

synchronized public static boolean is_exist_word(String word){
	boolean rps = false;
		File dataFile = new File("./dic/"+word.charAt(0)+"_dico.txt");
		   	try {
		   	InputStream ips = new FileInputStream(dataFile);
		   	InputStreamReader ipsr = new InputStreamReader(ips);
		   	BufferedReader br = new BufferedReader(ipsr);
		   	String ligne;
		   	while ((ligne = br.readLine()) != null) {
		   		//***
		   		if (word.equals(ligne)) {
					rps = true;
				}
		   	}
		   	
		   	br.close();
		   	} catch (Exception e) { System.out.println(e.toString()); }
return rps;
}

//*********************************************************************************************************
//**** nouvelle session
public static void start_session(boolean is_new){
	phase = "DEB";
	Fonction.generate_scores();
	//** SESSION/
	send_msg_of_all_EXCEPT_user("SESSION/\n", "");
	
	
	if (!is_new) {
		STR_FOR_tirage = Fonction.generate_tirage();
		tray = Fonction.generate_tray();
	  	new_tirage("");
	  	
	  	
	}
		Nb_Tour = 0;

	  	 for(Entry<String, Integer> entry : tab_scor.entrySet()){
			  String cle = entry.getKey();
			  tab_scor.replace(cle, 0);
	      }
	  	 for(Entry<String, Integer> entry : tab_scor_sess.entrySet()){
			  String cle = entry.getKey();
			  tab_scor_sess.replace(cle, 0);
	      } 	
	    	  for(Entry<String, String> entry : tab_tray.entrySet()){
	  		  String cle = entry.getKey();
	  		  tab_tray.replace(cle, "");
	        }
	    	 	
	Start_Tour();    
	 	
}

//*********************************************************************************************************
//**** fin session
	  
static void  end_session(boolean is_vide){
	phase = "DEB";
	if (is_vide) {
		//** to do VAINQUEUR/bilan/
		String bilan = get_scores(true);
	    send_msg_of_all_EXCEPT_user("VAINQUEUR/"+bilan+"/\n", "");
	
	}

	if (getNbjoueurs()!=0) {
		r_session = new Relance_session(Mon_Serveur, time_relance);
		r_session.start();
	}
	r_interne.arret();
	r_search.arret();
	r_session.arret();
	r_submit.arret();
	r_resultat.arret();

}
	  

//*********************************************************************************************************
//**** tour

public static void Start_Tour(){ 
	phase = "DEB";
		
			//** TOUR/plateau/tirage/
			  
			send_msg_of_all_EXCEPT_user("TOUR/"+tray+"/"+tirage+"/\n", "");
		

	  	 for(Entry<String, Integer> entry : tab_scor.entrySet()){
			  String cle = entry.getKey();
			  tab_scor.replace(cle, 0);
	      }
	  	  	
    	  for(Entry<String, String> entry : tab_tray.entrySet()){
  		  String cle = entry.getKey();
  		  tab_tray.replace(cle, "");
        }
    	Best_player = "";
  		Best_score = 0;
  		Best_word ="";	
  		Nb_Tour ++;
		Start_search();
		
}

public static void End_Tour(){ 
	
}

//******************************************************************************************************************************
//******************************************************************************************************************************
//***********************************	 Recherche 	****************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************


public static void Start_search(){ 
	phase = "REC";
	  
	//** thread de 5 min
	r_search = new Rebours_long(Mon_Serveur, time_search);
	r_search.start();
}


public static void End_search(boolean is_thread){ 
	phase = "DEB";
	
	if (is_thread) {
		//** RFIN/
		send_msg_of_all_EXCEPT_user("RFIN/\n", "");
//		try { Thread.sleep(10000); } catch (InterruptedException e) { }
		new_tirage("");
		Start_Tour();
	} else {
		r_search.arret();
		Start_submission();
	}

}


//******************************************************************************************************************************
//******************************************************************************************************************************
//***********************************	 Soumission 	************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************


public static void Start_submission(){ 
	phase = "SOU"; 
	//** thread 2 min
	r_submit = new Rebours_court(Mon_Serveur, time_subm);
	r_submit.start();
}

public static void End_submission(){ 
	phase = "RES";
	//** SFIN/
	send_msg_of_all_EXCEPT_user("SFIN/\n", "");
	
	Start_Result();
}



//******************************************************************************************************************************
//******************************************************************************************************************************
//***********************************	 Result 	************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************


public static void Start_Result(){ 
	phase = "RES";
	String litrre_utiliser ="";
	
	 for(Entry<String, Integer> entry : tab_scor.entrySet()){
		  String cle = entry.getKey();
		  tab_scor_sess.replace(cle, (tab_scor.get(cle)+tab_scor_sess.get(cle)) );
     }
	//** BILAN/mot/vainqueur/scores/
	String scores = get_scores(false);
	send_msg_of_all_EXCEPT_user("BILAN/"+Best_word+"/"+Best_player+"/"+Nb_Tour+"*"+scores+"/\n", "");

	
    if (tab_tray.get(Best_player) != null) {
    	String tmp_tray = tab_tray.get(Best_player);
    	for (int i = 0; i < tray.length(); i++) {
			if (tray.charAt(i) != tmp_tray.charAt(i)) {
				litrre_utiliser += tmp_tray.charAt(i);
			}
		}
    	tray = tmp_tray; //** new best tray 
	}
    for (int j = 0; j < litrre_utiliser.length(); j++) {
		String tmp = "";
	
	    for (int i = 0; i < tirage.length(); i++) {
	    	if (litrre_utiliser.charAt(j) != tirage.charAt(i)) {
	    		tmp += tirage.charAt(i);
	    	}
		}
	   tirage = tmp; 
    }
    new_tirage(tirage);
	r_resultat = new Rebours_resultat(Mon_Serveur, time_result);
	r_resultat.start();
			
}

public static void End_Result(){ 
	phase = "DEB";
	Start_Tour(); 
	}



//******************************************************************************************************************************
//******************************************************************************************************************************
//***********************************	 Le Chat 	****************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************
//******************************************************************************************************************************
	
//****	Message a tout les joueurs	
	synchronized public static void sendAll__Message(String  message, String user){
		PrintWriter out;

		 for(Entry<String, PrintWriter> entry : tab_print.entrySet()) {
			    String cle = entry.getKey();
			    out = entry.getValue();
			    
			 if(out != null){   
			    if (!cle.equals(user)) // sécurité
			      {
			      	// ecriture de la commande
//*********** RECEPTION/message/
			        out.print("RECEPTION/"+message+"/\n");
			        out.flush(); // envoi dans le flux de sortie
			      }
			 }   
			}
	}
	
//****	Message Privée
	synchronized public static void sendAll__Message_Privee(String  chaine, String user_env){
		int index_msg = chaine.indexOf('/');
		String message = chaine.substring(index_msg+1,chaine.length()-1);
  	String user_rec = chaine.substring(0, index_msg);

		PrintWriter out;

		 for(Entry<String, PrintWriter> entry : tab_print.entrySet()) {
			    String cle = entry.getKey();
			    out = entry.getValue();

			 if(out != null){   
			    if (cle.equals(user_rec)) // sécurité
			      {
			      	// ecriture de la commande
//****** PRECEPTION/message/user/
			        out.print("PRECEPTION/"+message+"/"+user_env+"/\n");
			        out.flush(); // envoi dans le flux de sortie
			      }
			 }   
			}
	}



// end
}
