import java.net.*;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.Set;
import java.awt.List;
import java.io.*;

//** Classe associée à chaque joueurs
public class Client_Thread implements Runnable{

	  private Thread thread_jouer; // contiendra le thread du jouer
	  private Socket socket_jouer; // recevra le socket liant au jouer
	  private PrintWriter out; // pour gestion du flux de sortie
	  private BufferedReader in; // pour gestion du flux d'entrée
	  private static Serveur Mon_Serveur; // pour utilisation des méthodes de la classe principale
	  private String user;
	  private static String tray_serv ="";
	  
	  
public Client_Thread(Socket s, Serveur mon_Serveur) {
		socket_jouer = s;
		Mon_Serveur = mon_Serveur;
		
		try
	    {
	      out = new PrintWriter(socket_jouer.getOutputStream());
	      in = new BufferedReader(new InputStreamReader(socket_jouer.getInputStream()));
	     
	    }
	    catch (IOException e){ }
		
		thread_jouer = new Thread(this);
		thread_jouer.start();
}
//*********************************************************************************************
//*********************************************************************************************
//******************************* calculer le scores

public void Calc_Score(String t_bef, String t_aft, String ph, String mot){
	
	
	int tmp = 0;//** le scores 
	int cases_cheres = 1;
	for (int i = 0; i < 225; i++) {
		if (t_bef.charAt(i) != t_aft.charAt(i)) {
			
			if (Fonction.is_triple(i)) {
				cases_cheres *= 3;
			} else if (Fonction.is_double(i)){
				cases_cheres *= 2;
			}
			tmp += Fonction.get_score(t_aft.charAt(i));
		}
	}
	
	tmp *= cases_cheres;
	Mon_Serveur.set_Score(user, tmp, t_aft, mot, ph);
}
//*********************************************************************************************
//*********************************************************************************************

public static String is_correct(ArrayList<Integer> t){
	String reponse = "true";
	
				if (t.size()>1) {
					//** changement dans la ligne 
					if ((t.get(0)/15 == t.get(1)/15)) {
						int ligne = t.get(0)/15;
						//** test tout les autre 
						for (int i = 1; i < t.size(); i++) {
							if ((t.get(i-1)/15 != t.get(i)/15)) {
								reponse = "POS la lettre dans la case "+(t.get(i)/15)+" "+(t.get(i)%15)+" est erronée";
								
							}
						}
						
					}else if(( (t.get(1)-t.get(0)) % 15) == 0){
						//** changement dans les colonne
						int colonne = t.get(0)%15;
						//** test tout les autre 
						for (int i = 1; i < t.size(); i++) {
							if (t.get(i)%15 != colonne) {
								reponse = "POS la lettre dans la case "+(t.get(i)/15)+" "+(t.get(i)%15)+" est erronée";
							}
						}
					}else{
						
						reponse = "POS la lettre dans la case "+(t.get(1)/15)+" "+(t.get(1)%15)+" est erronée";
					}
					
				
				}else if(t.size()==0){
					reponse = "POS Pas de changement";
				}
	
	return reponse;
}

public static String type_changement(ArrayList<Integer> t){
	String ch = "";
	
	if (t.size() == 0){ ch = "rien"; }else if (t.size() == 1) { ch = "un"; }else {
		if ((t.get(0)/15 == t.get(1)/15)) {
			ch = "ligne";
		}else {
			ch = "colonne";
		}
	}
	
	return ch;
}

public static String get_mot_ligne(int pos, String m0){
	String mot = "";
		for (int j = 15*pos; j < 15*pos+15; j++) {
			mot += m0.charAt(j);
		}
	return mot;
}

public static String get_mot_colonne(int pos, String m0){
	String mot = "";
		for (int k = pos; k < 225; k+=15) {
			mot += m0.charAt(k);
		}
		
	return mot;
}

public static ArrayList<String> get_mot(String m0, ArrayList<Integer> t){
	ArrayList<String> tmp = new ArrayList<String>();
	for (int i = 0; i < t.size(); i++) {
		 int ligne = t.get(i)/15;
		 int colonne = t.get(i)%15;

		 tmp.add(get_mot_ligne(ligne,m0));
		 tmp.add(get_mot_colonne(colonne, m0));
		 
	}
	return tmp;
}

public static ArrayList<String> get_mots(ArrayList<String> t){
	String mot = "";
	ArrayList<String> tmp = new ArrayList<String>();
	for (int i = 0; i < t.size(); i++) {
		mot = "";
		int pos = 0;
		for (int j = 0; j < 15; j++) {
			if (t.get(i).charAt(j) != '0') {
				break;
			}else {
				pos++;
			}
		}
		
		for (int j = pos; j < 15; j++) {
			if (t.get(i).charAt(j) == '0') {
				break;
			} else {
				mot += t.get(i).charAt(j);
				pos++;
			}
			
		}
			tmp.add(mot);
		for (int j = pos; j < 15; j++) {
			if (t.get(i).charAt(j) == '0') {
				pos++;
			}else {
				break;
			}
		}
		String mot2 = "";
		for (int j = pos; j < 15; j++) {
			if (t.get(i).charAt(j) == '0') {
				break;
			}else{
				mot2 += t.get(i).charAt(j);
			}
			
		}
		tmp.add(mot2);
	}
	return tmp;
}

public static String get_ensemble_mot(String tray_initial,String tray_received){
	String reponse = "";
	ArrayList<Integer> tab_index = new ArrayList<Integer>() ;
    
    for (int i = 0; i < tray_received.length(); i++) {
		if (tray_received.charAt(i) != tray_initial.charAt(i)) {
			tab_index.add(i);
		}
	}
    
	//**  les case qui sont changer
	String changement = is_correct(tab_index);
	String ty_ch = type_changement(tab_index);
	ArrayList<String> my_list = new ArrayList<String>() ;
				if (changement.equals("true")&&(!ty_ch.equals("un"))) {
			   	 
			   	 if (tab_index.size() > 0) {
			   		 
									my_list = get_mot(tray_received, tab_index) ;//** recup l'ensemble des ligne et colonne								
									my_list = get_mots(my_list);//** recup l'ensemble des mots dans une ligne et colonne
									//*** sup les double
									Set<String> mySet = new HashSet<String>(my_list);
								    my_list = new ArrayList<String>(mySet);
								    //**** garder qui les bon structure 
								    ArrayList<String> tmp = new ArrayList<String>();
									    for (int i = 0; i < my_list.size(); i++) {
									    	if (my_list.get(i).length() > 1) {
									    			tmp.add(my_list.get(i));
											}
										}
								    my_list = tmp;
								    boolean existe = true;
								    String mot =  "";
								    for (int i = 0; i < my_list.size(); i++) {
										if (! Mon_Serveur.is_exist_word(my_list.get(i))) {
											mot = my_list.get(i);
											existe = false;
											break;
										}
									}
								    if (existe) {
								    	reponse += "$";
									    for (int i = 0; i < my_list.size()-1; i++) {
									    	reponse += my_list.get(i)+"*";
										}
									    reponse += my_list.get(my_list.size()-1);
									}else{
										reponse = "DIC "+mot+" n'exite pas ";
									}
								    
					}
					
				} else {
					if (ty_ch.equals("un")) {
						reponse = "POS la lettre est en erreur";
					}else {
						reponse = changement;
					}
					
				}
	return reponse;
}
//*********************************************************************************************
//*********************************************************************************************

//** verification de la proposition
public void Check_the_proposal(String message){
	
		if (Mon_Serveur.get_phase().equals("DEB")) {
			//******** RINVALIDE/raison/
			String raison = "DIC Pas de soumission Dans cette phase DEB";//** au deput on peux pas soumission 
			out.print("RINVALIDE/"+raison+"/\n");
			out.flush();
		} else if(Mon_Serveur.get_phase().equals("RES")){
			//******** RINVALIDE/raison/
			String raison = "DIC Pas de soumission Dans cette phase RES";//** au deput on peux pas soumission
			out.print("RINVALIDE/"+raison+"/\n");
			out.flush();
		}else {
			//** si on est en phase REC ou SOU on peux soumission		
												
							String ph = Mon_Serveur.get_phase();
						
							
							tray_serv = Mon_Serveur.get_tray();//** recup tray initial
							
							String rps = get_ensemble_mot(tray_serv, message);
							if (rps.charAt(0) == '$') {
								
									rps = rps.substring(1, rps.length());
									//******** RATROUVE/user/
									if (ph.equals("REC")) {
										
										Mon_Serveur.End_search(false);//** fin recherche
										Mon_Serveur.send_msg_of_all_EXCEPT_user("RATROUVE/"+user+"/\n", user);
										
									}

									Calc_Score(tray_serv, message, ph, rps);//** calcule de score
								
							} else {

								//******** RINVALIDE/raison/
								out.print("RINVALIDE/"+rps+"/\n");
								out.flush();
							}

									
				
							
					
	}	
}
	
synchronized public void run() {
		String message = "";
		
		try
	    {
			
				 while (true) {
						
					      message = in.readLine();
					      if(!message.equalsIgnoreCase("")) // juste une vérification de principe
					        {
					        	
						        	int index_msg = message.indexOf('/');
						        	String msg = message.substring(0, index_msg);
						        	
						          switch (msg) {
									case "CONNEXION": //*** 	CONNEXION/user/
													  user = message.substring(10, message.length()-1); 
													  Mon_Serveur.add_jouer(user, out); 
													  break;
									
									case "SORT":  //*** 	SORT/user/
										  			socket_jouer.close();  break;  
					
									case "ENVOI":  //*** 	ENVOI/message/
													message = message.substring(6, message.length()-1);
													Mon_Serveur.sendAll__Message(message,user); 
													break;
					
									case "PENVOI":   //*** 	PENVOI/user/message/
										            message = message.substring(7, message.length()); 
												    Mon_Serveur.sendAll__Message_Privee(message, user); break;
												
									case "TROUVE": //** TROUVE/placement/
										            message = message.substring(7, message.length()-1); 
										            Check_the_proposal(message); 
												    break;
					
								   default: break;
								}
						          message = ""; // on vide la chaine de message pour qu'elle soit réutilisée
					        }
			    }
	    } catch (Exception e){ }
		
		finally // finally se produira le plus souvent lors de la deconnexion du client
	    {
		      try
		      {
		      	
		        Serveur.delete_jouer(user); 
		        socket_jouer.close(); // fermeture du socket si il ne l'a pas déjà été (à cause de l'exception levée plus haut)
		      }
		      catch (IOException e){ }
	    }
		
}

	

}
