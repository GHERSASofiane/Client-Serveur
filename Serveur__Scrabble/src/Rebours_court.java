
public class Rebours_court extends Thread  {
	//** par example 2 min

	Serveur Mon_serveur;
	static int time ;
	static int time_rest;
	
public Rebours_court(Serveur Mon_serv, int t) {
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
		Mon_serveur.End_submission();
		this.stop();
}

public static int get_time() { return time_rest; }
public void arret(){ this.stop();  }	
}
