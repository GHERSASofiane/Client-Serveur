import java.util.Scanner;


public class Read_time {

	//*********************************************************************************************************
	//****	lire le temps d'attend avant de commencer une session 
	static int lance(){
		Scanner sc = new Scanner(System.in);
		int data;
			do {
				System.out.println("Veuillez saisir le temps Compte-a-rebours interne en second :");
				data = sc.nextInt();
			} while (data <= 0);
		
	return data ;
	}
	
	//*********************************************************************************************************
	//****	lire le temps d'attend avant de relance une	nouvelle session
	static int relance(){
		Scanner sc = new Scanner(System.in);
		int data;
			do {
				System.out.println("Veuillez saisir le temps d'attend avant de relance une	nouvelle session en second :");
				data = sc.nextInt();
			} while (data <= 0);
		
	return data ;
	}


	
	//*********************************************************************************************************
	//****	lire le temps d'attend compte a  rebours  long
	static int search(){
		Scanner sc = new Scanner(System.in);
		int data;
			do {
				System.out.println("Veuillez saisir le temps de compte a  rebours  long second :");
				data = sc.nextInt();
			} while (data <= 0);
		
	return data ;
	}


	
	//*********************************************************************************************************
	//****	lire le temps d'attend compte a  rebours  court
	static int subm(){
		Scanner sc = new Scanner(System.in);
		int data;
			do {
				System.out.println("Veuillez saisir le temps de compte a  rebours  court second :");
				data = sc.nextInt();
			} while (data <= 0);
		
	return data ;
	}



	
	//*********************************************************************************************************
	//****	lire le temps d'attend a la phase de resultats
	static int result(){
		Scanner sc = new Scanner(System.in);
		int data;
			do {
				System.out.println("Veuillez saisir le temps d'attend a la phase de resultats second :");
				data = sc.nextInt();
			} while (data <= 0);
		
	return data ;
	}


	
	//*********************************************************************************************************
	//****	lire le temps d'attend a la phase de resultats
	static boolean is_modifier(){
		Scanner sc = new Scanner(System.in);
		String data;
			do {
				System.out.println("voulez vous modifier le temps des phase ? OUI / NON");
				data = sc.next();
			} while ((!data.equals("OUI")) && (!data.equals("NON")));
		
			if (data.equals("OUI")) {
				return true ;
			} else {
				return false ;
			}
	
	}

	
	
	
	
	
}
