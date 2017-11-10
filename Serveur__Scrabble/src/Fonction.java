import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import java.util.Map.Entry;


public class Fonction {
	public static Map<Character, Integer> score = new HashMap<>();

//*********************************************************************************************************
//****	score de chaque littre 
	static void generate_scores(){
		 
		score.put('A', 1);score.put('B', 3);score.put('C', 3);score.put('D', 2);score.put('E', 1);score.put('F', 4);
		score.put('G', 2);score.put('H', 4);score.put('I', 1);score.put('J', 8);score.put('K', 10);score.put('L', 1);
		score.put('M', 2);score.put('N', 1);score.put('O', 1);score.put('P', 3);score.put('Q', 8);score.put('R', 1);
		score.put('S', 1);score.put('T', 1);score.put('U', 1);score.put('V', 4);score.put('W', 10);score.put('X', 10);
		score.put('Y', 10);score.put('Z', 10);
		   	
}
	
static Integer get_score(char c){
	return score.get(c);
}

static boolean is_triple(int index){
	boolean reponse = false;
		ArrayList<Integer> index_triple = new ArrayList<>();
		
		index_triple.add(0);index_triple.add(7);index_triple.add(14);
		index_triple.add(105);index_triple.add(119);index_triple.add(210);
		index_triple.add(217);index_triple.add(224);
		if (index_triple.contains(index)) {
			reponse = true;
		}
	return reponse;
}

static boolean is_double(int index){
	boolean reponse = false;
		ArrayList<Integer> index_double = new ArrayList<>();
		
		index_double.add(16);index_double.add(28);index_double.add(32);index_double.add(42);index_double.add(48);index_double.add(56);
		index_double.add(64);index_double.add(70);index_double.add(80);index_double.add(84);index_double.add(96);index_double.add(98);
		index_double.add(126);index_double.add(128);index_double.add(140);index_double.add(144);index_double.add(154);index_double.add(160);
		index_double.add(168);index_double.add(176);index_double.add(182);index_double.add(192);index_double.add(196);index_double.add(208);
		if (index_double.contains(index)) {
			reponse = true;
		}
	return reponse;
}
 //*********************************************************************************************************
  //****	affiche le message d'accueil 
  static void Welcome(Integer port)
  {
	    System.out.println("------------------------");
	    System.out.println("Serveur : Par GHERSA Sofiane -- CHOULLIT Naim");
	    System.out.println("Copyright : 2017 -- github.com/GHERSASofiane/Client-Serveur");
	    System.out.println("Derniere version : 11/04/2017");
	    System.out.println("------------------------");
	    System.out.println("Demarre sur le port : "+port.toString());
	    System.out.println("------------------------");
	    System.out.println("Quitter : tapez \"quit\"");
	    System.out.println("Nombre de connectes : tapez \"total\"");
	    System.out.println("------------------------");
	    
  }
 

//*********************************************************************************************************
//****	remplir STR_FOR_tirage la on stock les littre pour tirage 
static String generate_tirage(){
  		  String tmp ="";
  	    	Map<Character, Integer> tab_car = new HashMap<>();
  	    	tab_car.put('A', 8);tab_car.put('B', 5);tab_car.put('C', 5);tab_car.put('D', 5);tab_car.put('E', 7);tab_car.put('F', 4);
  	    	tab_car.put('G', 5);tab_car.put('H', 6);tab_car.put('I', 8);tab_car.put('J', 4);tab_car.put('K', 4);tab_car.put('L', 4);
  	    	tab_car.put('M', 5);tab_car.put('N', 5);tab_car.put('O', 6);tab_car.put('P', 4);tab_car.put('Q', 4);tab_car.put('R', 4);
  	    	tab_car.put('S', 4);tab_car.put('T', 4);tab_car.put('U', 6);tab_car.put('V', 3);tab_car.put('W', 3);tab_car.put('X', 3);
  	    	tab_car.put('Y', 2);tab_car.put('Z', 2);
  	    	
  	    	for(Entry<Character, Integer> entry : tab_car.entrySet()) {
  			    char cle = entry.getKey();
  			    int nbr = entry.getValue();
  			    for (int i = 0; i < nbr; i++) {
  			    	tmp += cle;
  			    	
  				}
  			    
  	    	}
return tmp;    	
}	  

  	  
//*********************************************************************************************************
//****  generation de tray au nouvelle session
  	  	    
static String generate_tray() {
	String tmp = "";
  		for (int i = 0; i < 225; i++) {
  			tmp += '0';
  		}
return tmp;		
}
  		  

	  
}
