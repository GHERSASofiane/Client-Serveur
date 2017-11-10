import java.io.*;

public class Commandes_Serveur implements Runnable{

	Serveur Mon_Serv ;
	BufferedReader Console_in ;
	String Commande = "";
	Thread Thread_Comm;
//****	Constructeur
	public Commandes_Serveur(Serveur serv) {
		Mon_Serv = serv;
		Console_in = new BufferedReader(new InputStreamReader(System.in));
		Thread_Comm = new Thread(this);
		Thread_Comm.start();
	}
//****	Attend les commande de console	
	public void run() {
		try {
			while ((Commande = Console_in.readLine()) != null) {
				
				if (Commande.equalsIgnoreCase("quit")) {// la commande quit
					System.exit(0);
				}else if (Commande.equalsIgnoreCase("total")) {// la commande total
					 System.out.println("Nombre de connectes : "+Mon_Serv.getNbjoueurs());
			         System.out.println("-----------------------------");
				}else {
					  System.out.println("Cette commande n'est pas supportee");
			          System.out.println("Quitter : \"quit\"");
			          System.out.println("Nombre de joueurs : \"total\"");
			          System.out.println("----------------------------");
				}
				 System.out.flush();
			}
		} catch (Exception e) {}
		
	}

}
