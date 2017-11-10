
public class Rebours_interne extends Thread  {
//** 20 s par example 
	Serveur Mon_serveur;
	static int time ;
	static int time_rest;
public Rebours_interne(Serveur Mon_serv, int t) {
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
		//** commence une session
		Mon_serveur.start_session(true);
		this.stop();
}

public static int get_time() { return time_rest; }
public void arret(){ this.stop();  }	
}
