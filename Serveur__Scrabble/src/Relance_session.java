
public class Relance_session extends Thread  {
//** par exemple 2 minutes
	Serveur Mon_serveur;
	static int time ;
	static int time_rest;
	
public Relance_session(Serveur Mon_serv, int t) {
	Mon_serveur = Mon_serv;
	time = t;
	time_rest = t;
}

@Override
public void run() {
		for (int i = 0; i < time; i++) {
			time_rest--;

			try { sleep(1000); } catch (InterruptedException e) { }
		}
		//** nouvelle session
		Mon_serveur.start_session(false);
		this.stop();
}


public static int get_time() { return time_rest; }	
public void arret(){ this.stop();  }	
}
