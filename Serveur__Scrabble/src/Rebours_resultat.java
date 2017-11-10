
public class Rebours_resultat extends Thread  {
	//** par example 5 min
		Serveur Mon_serveur;
		static int time ;
		static int time_rest ;
		
	public Rebours_resultat(Serveur Mon_serv, int time) {
		Mon_serveur = Mon_serv;
		this.time = time;
		time_rest = time;
	}

	@Override
	public void run() {
			for (int i = 0; i < time; i++) {
				time_rest--;

				try { sleep(1000); } catch (InterruptedException e) {  }
			}
			Mon_serveur.End_Result();
			this.stop();
	}

	public static int get_time() { return time_rest; }
	public void arret(){ this.stop();  }
}
