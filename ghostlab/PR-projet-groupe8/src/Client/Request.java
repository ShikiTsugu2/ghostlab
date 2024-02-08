import java.io.*;
import java.net.*;
import java.util.Scanner;
import java.nio.*;

public class Request{
    private BufferedReader br;
    private PrintWriter pw;
    private OutputStream os;
    private int portMulticast;
    private String ipMulticast;
    private String port;
    private boolean registered;
    private Socket s;
    public Request(BufferedReader br, PrintWriter pw, OutputStream os, String port, Socket s){
        this.br = br;
        this.pw = pw;
        this.os = os;
        this.port = port;
        this.registered = false;
        this.s = s;
    }
    
    public int getPortMulticast(){
        return portMulticast;
    }

    public String getIpMulticast(){
        return ipMulticast;
    }

    public void listCommandBeforeGame(){
        System.out.println("NEWPL : Créer une partie");
        System.out.println("GAME? : Afficher la liste des parties");
        System.out.println("LIST? : Afficher la liste des joueurs d'une partie");
        System.out.println("UNREG : Se désincrire d'une partie");
        System.out.println("SIZE? : Demander la taille du labyrinthe d'une partie");
        System.out.println("START : Commencez la partie");
        System.out.println("REGIS : Rejoindre une partie");
        System.out.println("ITMON : Activer le mode ITEMS");
        System.out.println("CHEAT : Activer le mode CHEAT");
        System.out.println("RESIZ : Redimensionner le labyrinthe de la partie");
        System.out.println("COMMA : Afficher la liste des commandes\n");
    }

    public boolean requestBeforeGame(){
        try{
            BufferedReader sys = new BufferedReader(new InputStreamReader(System.in));
            char[] buf = new char[5];
            sys.read(buf,0,5);
            String req = new String(buf);
            if(req.equals("NEWPL")){
                sendRequestNEWPL();
                recvReplyNEWPL();
            }else if(req.equals("GAME?")){
                sendRequestGAME();
                recvReplyGAME();
            }else if(req.equals("LIST?")){
                sendRequestLIST();
                recvReplyLIST();
            }else if(req.equals("UNREG")){
                sendRequestUNREG();
                recvReplyUNREG();
            }else if(req.equals("SIZE?")){
                sendRequestSIZE();
                recvReplySIZE();
            }else if(req.equals("START")){
                if(!registered){
                    System.out.println("Vous devez être inscrit dans une partie");
                    return true;
                }
                sendRequestSTART();
                recvReplySTART();
                return false;
            }else if(req.equals("REGIS")){
                sendRequestREGIS();
                recvReplyREGIS();
            }else if(req.equals("ITMON")){
                sendRequestITMON();
                recvReplyITMON();
            }else if(req.equals("CHEAT")){
                sendRequestCHEAT();
                recvReplyCHEAT();
            }else if(req.equals("RESIZ")){
                sendRequestRESIZ();
                recvReplyRESIZ();
            }else if(req.equals("COMMA")){
                listCommandBeforeGame();
            }else{
                System.out.println("Commande non reconnue");
            }
        } catch(Exception e){
            e.printStackTrace();
        }
        return true;
    }

    public void listCommandDuringGame(){
        System.out.println("UPMOV/RIMOV/LEMOV/DOMOV : Se déplacer dans une direction");
        System.out.println("MALL? : Envoyer un message à tous les joueurs");
        System.out.println("SEND? : Envoyer un message à un joueur");
        System.out.println("GLIS? : Afficher la liste des joueurs de la partie");
        System.out.println("IQUIT : Quitter la partie");
        System.out.println("EXTEN : Afficher les commandes des extensions");
        System.out.println("COMMA : Afficher la liste des commandes\n");
    }

    public void listCommandExtension(){
        System.out.println("ITEM? : Afficher les objets possédés");
        System.out.println("ITTEL : Utiliser un objet TEL");
        System.out.println("ITBON : Utiliser un objet BON");
        System.out.println("ITTRA : Utiliser un objet TRA");
        System.out.println("KILLG : Attraper tous les fantômes");
        System.out.println("LABSH : Afficher le labyrinthe");
        System.out.println("GODME : Activer le mode GOD");
        System.out.println("UNGOD : Désactiver le mode GOD");
        System.out.println("GIVCH : Donner le mode CHEAT à un joueur");
        System.out.println("ITINF : Activer le mode items illimités");
        System.out.println("NOINF : Désactiver le mode items illimités");
        System.out.println("ITTRA : Utiliser un objet TRA\n");
    }
    public boolean requestDuringGame(){
        try{
            BufferedReader sys = new BufferedReader(new InputStreamReader(System.in));
            char[] buf = new char[5];
            sys.read(buf,0,5);
            String req = new String(buf);
            if(req.equals("UPMOV") || req.equals("RIMOV") || req.equals("LEMOV") || req.equals("DOMOV")){
                sendRequestMOV(req);
                return recvReplyMOV();
            }else if(req.equals("MALL?")){
                sendRequestMALL();
                return recvReplyMALL();
            }else if(req.equals("SEND?")){
                sendRequestSEND();
                return recvReplySEND();
            }else if(req.equals("ITEM?")){
                sendRequestITEM();
                return recvReplyITEM();
            }else if(req.equals("ITTEL")){
                sendRequestITTEL();
                return recvReplyITTEL();
            }else if(req.equals("ITBON")){
                sendRequestITBON();
                return recvReplyITBON();
            }else if(req.equals("ITTRA")){
                sendRequestITTRA();
                return recvReplyITTRA();
            }else if(req.equals("KILLG")){
                sendRequestKILLG();
                return recvReplyKILLG();
            }else if(req.equals("LABSH")){
                sendRequestLABSH();
                return recvReplyLABSH();
            }else if(req.equals("GODME")){
                sendRequestGODME();
                return recvReplyGODME();
            }else if(req.equals("UNGOD")){
                sendRequestUNGOD();
                return recvReplyUNGOD();
            }else if(req.equals("GIVCH")){
                sendRequestGIVCH();
                return recvReplyGIVCH();
            }else if(req.equals("ITINF")){
                sendRequestITINF();
                return recvReplyITINF();
            }else if(req.equals("NOINF")){
                sendRequestNOINF();
                return recvReplyNOINF();
            }else if(req.equals("GLIS?")){
                sendRequestGLIS();
                return recvReplyGLIS();
            }else if(req.equals("IQUIT")){
                sendRequestIQUIT();
                return recvReplyIQUIT();
            }else if(req.equals("COMMA")){
                listCommandDuringGame();
            }else if(req.equals("EXTEN")){
                listCommandExtension();
            }else{
                System.out.println("Commande non reconnue");
            }
        } catch(Exception e){
            e.printStackTrace();
        }
        return true;
    }

    public void sendRequestMALL(){
        Scanner in = new Scanner(System.in);
        System.out.println("Entrez un message à envoyer à tous les joueurs :");
        String message = in.nextLine();
        pw.print("MALL? " + message+ "***");
        pw.flush();
    }
     
    public void sendRequestSEND(){
        Scanner in = new Scanner(System.in);
        System.out.println("Entrez l'id du joueur à qui vous voulez envoyer un message (8 caractères exactement) :");
        String id = in.nextLine() + "aaaaaaaa";
        System.out.println("Entrez un message :");
        String message = in.nextLine() ;
        pw.print("SEND? " + id.substring(0,8) + " " +message + "***");
        pw.flush();
    }

    public void sendRequestMOV(String req){
        Scanner in = new Scanner(System.in);
        System.out.println("Entrez la distance à parcourir :");

        while(!in.hasNextInt()){
            in.nextLine();
            System.out.println("Les coordonnées doivent être des entiers compris entres 0 et 999");
        }
        String d = in.nextLine();
        while(d.length()<3){
            d = "0" + d;
        }
        pw.print(req+" "+d.substring(0, 3) +"***");
        pw.flush();
    }

    public void sendRequestSTART() throws Exception{
        pw.print("START***");
        pw.flush();
    }
    public void sendRequestUNREG() throws Exception{
        pw.print("UNREG***");
        pw.flush();
    }

    public void sendRequestSIZE() throws Exception{
        Scanner in = new Scanner(System.in);
        System.out.println("Entrez le numéro de la partie :");
        while(!in.hasNextInt()){
            in.nextLine();
            System.out.println("Veuillez entrer un entier");
        }
        int val = in.nextInt();
        byte m = (byte)val;
        byte[] mess = "SIZE? m***".getBytes();
        mess[6] = m;
        os.write(mess);
        os.flush();
    }

    public void sendRequestGAME() throws Exception{
        pw.print("GAME?***");
        pw.flush();
    }
 
    public void sendRequestGLIS() throws Exception{
        pw.print("GLIS?***");
        pw.flush();
    }
     
    public void sendRequestRESIZ() throws Exception{
        
        Scanner in = new Scanner(System.in);
        //tant que hauteur et largeur ne sont pas entre 10 et 1000 on demande de rentrer une nouvelle valeur
        short h;
        short w;
        byte g;
        System.out.println("Entrez la nouvelle hauteur :");
        while(!in.hasNextInt()){
            in.nextLine();
            System.out.println("La hauteur doit être un entier compris entre 10 et 1000");
            
        }
        h= (short)in.nextInt();
        System.out.println("Entrez la nouvelle largeur :");
        while(!in.hasNextInt()){
            in.nextLine();
            System.out.println("La largeur doit être un entier compris entre 10 et 1000");
        
        }
        w= (short)in.nextInt();
        System.out.println("Entrez le nombre de fantômes :");
        while(!in.hasNextInt()){
            in.nextLine();
            System.out.println("Le nombre de fantômes doit être un entier compris entre 1 et 120");
        }
        g= (byte)in.nextInt();
        byte[] mess = "RESIZ hh ll n***".getBytes();
        mess[6] = (byte)((h >> 8) & 0xff);; 
        mess[7] = (byte)(h & 0xff);
        mess[9 ] = (byte)((w >> 8) & 0xff);
        mess[10] = (byte)(w & 0xff);
        mess[12] =(byte) g;
        os.write(mess);
        os.flush();

    }
    public void sendRequestLIST() throws Exception{
        Scanner in = new Scanner(System.in);
        System.out.println("Entrez le numéro de la partie :");
        while(!in.hasNextInt()){
            in.nextLine();
            System.out.println("Veuillez entrer un entier");
        }
        int val = in.nextInt();
        byte m = (byte)val;
        byte[] mess = "LIST? m***".getBytes();
        mess[6] = m;
        os.write(mess);
        os.flush();
    }

    public void sendRequestNEWPL() throws Exception{
        Scanner in = new Scanner(System.in);
        System.out.println("Entrez un identifiant (8 caractères exactement) :");
        String id = in.nextLine() +"aaaaaaaa";
        pw.print("NEWPL "+id.substring(0,8)+" "+ port +"***");
        pw.flush();
    }

    public void sendRequestREGIS() throws Exception{
        Scanner in = new Scanner(System.in);
        System.out.println("Entrez un identifiant (8 caractères exactement) :");
        String id = in.nextLine() +"aaaaaaaa";
        System.out.println("Entrez le numéro de la partie :");
        while(!in.hasNextInt()){
            in.nextLine();
            System.out.println("Veuillez entrer un entier");
        }
        byte m = (byte)in.nextInt();
        String s = "REGIS "+id.substring(0,8)+" "+port+ " " + "m***";
        byte[] message = s.getBytes();
        message[20] = m;
        os.write(message);
        os.flush();
    }

    public void sendRequestITMON(){
        pw.print("ITMON***");
        pw.flush();
    }
    
    public void sendRequestITEM() throws Exception{
        pw.print("ITEM?***");
        pw.flush();
    }

    public void sendRequestITBON() throws Exception{
        pw.print("ITBON***");
        pw.flush();
    }

    public void sendRequestITTEL() throws Exception{
        Scanner in = new Scanner(System.in);
        String x;
        String y;
        System.out.println("Entrez la coordonnée x de la destination :");
        while(!in.hasNextInt()){
            in.nextLine();
            System.out.println("Les coordonnées doivent être des entiers compris entres 0 et 999");
        }
        x = in.nextLine();
        System.out.println("Entrez la coordonnée y de la destination :");
        while(!in.hasNextInt()){
            in.nextLine();
            System.out.println("Les coordonnées doivent être des entiers compris entres 0 et 999");
        }    
        y = in.nextLine();
        
        
        while(x.length()<3){
            x = "0" + x;
        }
        while(y.length()<3){
            y = "0" + y;
        }
        pw.print("ITTEL " + x.substring(0, 3) + " " + y.substring(0, 3) +"***");
        pw.flush();
    }

    public void sendRequestITTRA() throws Exception{
        Scanner in = new Scanner(System.in);
        String x;
        String y;
        System.out.println("Entrez la coordonnée x de la destination :");
        while(!in.hasNextInt()){
            in.nextLine();
            System.out.println("Les coordonnées doivent être des entiers compris entres 0 et 999");
        }
        x = in.nextLine();
        System.out.println("Entrez la coordonnée y de la destination :");
        while(!in.hasNextInt()){
            in.nextLine();
            System.out.println("Les coordonnées doivent être des entiers compris entres 0 et 999");
        }    
        y = in.nextLine();
        
        
        while(x.length()<3){
            x = "0" + x;
        }
        while(y.length()<3){
            y = "0" + y;
        }
        pw.print("ITTRA " + x.substring(0, 3) + " " + y.substring(0, 3) +"***");
        pw.flush();
    }

    public void sendRequestCHEAT(){
        pw.print("CHEAT***");
        pw.flush();
    }

    public void sendRequestKILLG(){
        pw.print("KILLG***");
        pw.flush();
    }

    public void sendRequestLABSH(){
        pw.print("LABSH***");
        pw.flush();
    }

    public void sendRequestGODME(){
        pw.print("GODME***");
        pw.flush();
    }

    public void sendRequestUNGOD(){
        pw.print("UNGOD***");
        pw.flush();
    }

    public void sendRequestGIVCH() throws Exception{
        Scanner in = new Scanner(System.in);
        System.out.println("Entrez l'identifiant du joueur à qui envoyer le cheat mode (8 caractères exactement) :");
        String id = in.nextLine() +"aaaaaaaa";
        pw.print("GIVCH "+id.substring(0,8)+"***");
        pw.flush();
    }

    public void sendRequestITINF(){
        pw.print("ITINF***");
        pw.flush();
    }

    public void sendRequestNOINF(){
        pw.print("NOINF***");
        pw.flush();
    }

    public void sendRequestIQUIT(){
        pw.print("IQUIT***");
        pw.flush();
    }

    public void recvReplySTART() throws Exception{
        System.out.println("En attente que la partie commence");
        char[] buf = new char[39];
        int recu = 0;
        while(recu<39 && (recu<3 || (buf[recu-3]!='*' || buf[recu-2]!='*' || buf[recu-1]!='*'))){
            recu += br.read(buf, recu, 39-recu);
        }
        String req = new String(buf);
        if(Client.VERB){
            System.out.println("[ "+req+" ]");
        }
        if(req.substring(0,5).equals("WELCO")){
            byte m = (byte) buf[6];
            short h = (short) (buf[8] + buf[9]);
            short w = (short) (buf[11] + buf[12]);
            byte f = (byte) buf[14];
            String ip = req.substring(16,31);
            String port = req.substring(32,36);
            ipMulticast = ip.replaceAll("#", "");
            portMulticast = Integer.valueOf(port);
            System.out.println(m + " " + h + " " + w + " " + f +" "+ ip +" " +port);
        }else{
            System.out.println("Erreur recvReplySTART" + req);
        }
    }

    public boolean recvReplyMOV() throws Exception{
        char[] buf = new char[21];
        int recu = 0;
        while(recu<21 && (recu<3 || (buf[recu-3]!='*' || buf[recu-2]!='*' || buf[recu-1]!='*'))){
            recu += br.read(buf, recu, 21-recu);
        }
        String req = new String(buf);
        if(Client.VERB){
            System.out.println("[ "+req+" ]");
        }
        if(req.substring(0,5).equals("MOVE!")){
            String x = req.substring(6,9);
            String y = req.substring(10,13);
            System.out.println("Vous vous êtes déplacé en position x = "+x+", y = "+y+". Aucun fantome attrapé.");
        }else if(req.substring(0,5).equals("MOVEF")){
            String x = req.substring(6,9);
            String y = req.substring(10,13);
            String p = req.substring(14,18);
            System.out.println("Fantome attrapé ! Vous vous êtes déplacé en position x = "+x+", y = "+y+
            " et vous possèdez "+p+" points !");
        }else if(req.substring(0,8).equals("GOBYE***")){
            System.out.println("Partie terminée ! BYE");
            return false;
        }else{
            System.out.println("Erreur recvReplyMOV " + req);
        }
        return true;
    }

    public void recvReplyNEWPL() throws Exception{
        char[] buf = new char[10];
        int recu = 0;
        while(recu<10 && (recu<3 || (buf[recu-3]!='*' || buf[recu-2]!='*' || buf[recu-1]!='*'))){
            recu += br.read(buf, recu, 10-recu);
        }
        String req = new String(buf);
        if(Client.VERB){
            System.out.println("[ "+req+" ]");
        }
        if(req.substring(0,5).equals("REGOK")){
            registered = true;
            byte m = (byte) buf[6];
            System.out.println("Partie créée ! Vous etes inscrit dans la partie " + m);
        }else if(req.substring(0,8).equals("REGNO***")){
            System.out.println("La partie n'a pas pu être créée");
        }else{
             System.out.println("Erreur recvReplyNEWPL" + req.substring(0,5));
        }
    }

    public void recvReplyRESIZ() throws Exception{
        char[] buf = new char[8];
        br.read(buf,0,8);
        String req = new String(buf);
        if(Client.VERB){
            System.out.println("[ "+req+" ]");
        }
        if(req.equals("RESOK***")){
            System.out.println("Partie redimensionnée !" );
        }else if(req.equals("DUNNO***")){
            System.out.println("Vous n'avez pas le droit de redimensionner le labyrinthe" );
        }else {
            System.out.println("Erreur recvReplyRESIZ " + req);
        }
    }

    public void recvReplyREGIS() throws Exception{
        char[] buf = new char[10];
        int recu = 0;
        while(recu<10 && (recu<3 || (buf[recu-3]!='*' || buf[recu-2]!='*' || buf[recu-1]!='*'))){
            recu += br.read(buf, recu, 10-recu);
        }
        String req = new String(buf);
        if(Client.VERB){
            System.out.println("[ "+req+" ]");
        }
        if(req.substring(0,5).equals("REGOK")){
            registered = true;
            byte m = (byte) buf[6];
            System.out.println("Vous etes inscrit dans la partie " + m);
        }else if(req.substring(0,8).equals("REGNO***")){
            System.out.println("La partie n'existe pas ou vous êtes déjà inscrit dans une partie.");
        }else{
            System.out.println("Erreur recvReplyREGIS" + req.substring(0,5));
        }

    }

    public void recvReplyUNREG() throws Exception{
        char[] buf = new char[10];
        int recu = 0;
        while(recu<10 && (recu<3 || (buf[recu-3]!='*' || buf[recu-2]!='*' || buf[recu-1]!='*'))){
            recu += br.read(buf, recu, 10-recu);
        }
        String req = new String(buf);
        if(Client.VERB){
            System.out.println("[ "+req+" ]");
        }
        if(req.substring(0,5).equals("UNROK")){
            registered = false;
            byte m = (byte) buf[6];
            System.out.println("Vous avez bien été désinscrit de la partie " + m);
        }else if(req.substring(0,8).equals("DUNNO***")){
            System.out.println("Vous n'êtes inscrit dans aucune partie.");
        }else{
            System.out.println("Erreur recvReplyUNREG" + req.substring(0,5));
        }

    }
    public void recvReplySIZE() throws Exception{
        byte[] buf = new byte[16];
        int recu = 0;
        while(recu<16 && (recu<3 || (buf[recu-3]!='*' || buf[recu-2]!='*' || buf[recu-1]!='*'))){
            recu += s.getInputStream().read(buf, recu, 16-recu);
        }
        String req = new String(buf);
        if(Client.VERB){
            System.out.println("[ "+req+" ]");
        }
        if(req.substring(0,5).equals("SIZE!")){
            byte m = (byte) buf[6];
            int h = (((byte) buf[9]) & 0xff) + (((byte)buf[8]) & 0xff) * 0x100;
            int w = (((byte) buf[12]) & 0xff) + (((byte) buf[11]) & 0xff) * 0x100;
            System.out.println("Partie " + m + " : " + "hauteur " + h + " largeur " + w);
        }else if(req.substring(0,8).equals("DUNNO***")){
            System.out.println("La partie n'existe pas.");
        }else{
             System.out.println("Erreur recvReplySIZE!" + req.substring(0,5));
        }
    }
    public void recvReplyGAME() throws Exception{
        char[] buf = new char[10];
        int recu = 0;
        while(recu<10 && (recu<3 || (buf[recu-3]!='*' || buf[recu-2]!='*' || buf[recu-1]!='*'))){
            recu += br.read(buf, recu, 10-recu);
        }
        String req = new String(buf);
        if(req.substring(0,5).equals("GAMES")){
            byte nbGames = (byte) buf[6];
            System.out.println("Il y a actuellement "+nbGames+" parties disponibles.");
            buf = new char[12*nbGames];
            recu = 0;
            while(recu<12*nbGames){
                recu += br.read(buf, recu, 12*nbGames-recu);
            }
            if(Client.VERB){
                System.out.println("[ "+req + new String(buf)+" ]");
            }
            for(int i=0; i<nbGames; i++){
                System.out.println("Partie " + (byte) buf[6+12*i] + " : Nombre de joueurs " + (byte) buf[8+12*i]);
            }
        }else{
            System.out.println("Erreur recvReplyGAME" + req.substring(0,5));
        }
    }
    public void recvReplyLIST() throws Exception{
        char[] buf = new char[12];
        int recu = 0;
        while(recu<12 && (recu<3 || (buf[recu-3]!='*' || buf[recu-2]!='*' || buf[recu-1]!='*'))){
            recu += br.read(buf, recu, 12-recu);
        }
        String req = new String(buf);
        if(req.substring(0,5).equals("LIST!")){
            byte numGame = (byte) buf[6];
            byte nbPlayers = (byte) buf[8];
            System.out.println("Il y a actuellement "+nbPlayers+" joueurs dans la partie " + numGame + ".");
            buf = new char[17*nbPlayers];
            recu = 0;
            while(recu<17*nbPlayers){
                recu += br.read(buf, recu, 17*nbPlayers-recu);
            }
            StringBuffer message = new StringBuffer();
            if(Client.VERB){
                System.out.println("[ "+req + message+" ]");
            }
            message.append(buf, 0, 17*nbPlayers);
            for(int i=0; i<nbPlayers; i++){
                System.out.println(message.substring(6+17*i,14+17*i));
            }
        }
        else if(req.substring(0,8).equals("DUNNO***")){
            if(Client.VERB){
                System.out.println(req);
            }
            System.out.println("La partie demandée n'existe pas." );
        }
        else{
            System.out.println("Erreur recvReplyLIST" + req + req.length());
        }
    }

    public boolean recvReplyGLIS() throws Exception{
        char[] buf = new char[10];
        int recu = 0;
        while(recu<10 && (recu<3 || (buf[recu-3]!='*' || buf[recu-2]!='*' || buf[recu-1]!='*'))){
            recu += br.read(buf, recu, 10-recu);
        }
        String req = new String(buf);
        if(req.substring(0,5).equals("GLIS!")){
            byte nbPlayers = (byte) buf[6];
            System.out.println("Il y a actuellement "+nbPlayers+" joueurs dans la partie courante.");
            buf = new char[30*nbPlayers];
            recu = 0;
            while(recu<30*nbPlayers){
                recu += br.read(buf, recu, 30*nbPlayers-recu);
            }
            StringBuffer message = new StringBuffer();
            if(Client.VERB){
                System.out.println("[ "+req + message+" ]");
            }
            message.append(buf, 0, 30*nbPlayers);
            for(int i=0; i<nbPlayers; i++){
                System.out.println(message.substring(6+30*i,27+30*i));
            }
           
            
        }else if(req.substring(0,8).equals("GOBYE***")){
            System.out.println("Partie terminée ! BYE");
            return false;
        }else{
            System.out.println("Erreur recvReplyGLIS" + req + req.length());
        }
        return true;
    }
    public void recvReplyITMON() throws Exception{
        char[] buf = new char[8];
        int recu = 0;
        while(recu<8 && (recu<3 || (buf[recu-3]!='*' || buf[recu-2]!='*' || buf[recu-1]!='*'))){
            recu += br.read(buf, recu, 8-recu);
        }
        String req = new String(buf);
        if(Client.VERB){
            System.out.println("[ "+req+" ]");
        }
        if(req.equals("ITMOK***")){
            System.out.println("Le mode ITEMS est activé !");
        }else if(req.equals("DUNNO***")){
            System.out.println("Vous n'avez pas le droit d'activer le mode ITEMS ou vous n'êtes pas inscrit dans une partie.");
        }else{
             System.out.println("Erreur recvReplyITMON" + req);
        }
    }

    public void recvReplyPOSIT() throws Exception{
        char[] buf = new char[25];
        int recu = 0;
        while(recu<25 && (recu<3 || (buf[recu-3]!='*' || buf[recu-2]!='*' || buf[recu-1]!='*'))){
            recu += br.read(buf, recu, 25-recu);
        }
        String req = new String(buf);
        if(Client.VERB){
            System.out.println("[ "+req+" ]");
        }
        if(req.substring(0,5).equals("POSIT")){
            System.out.println(req);
        }else{
             System.out.println("Erreur recvReplyPOSIT " + req);
        }
    }

    public boolean recvReplyMALL() throws Exception{
        char[] buf = new char[8];
        int recu = 0;
        while(recu<8 && (recu<3 || (buf[recu-3]!='*' || buf[recu-2]!='*' || buf[recu-1]!='*'))){
            recu += br.read(buf, recu, 8-recu);
        }
        String req = new String(buf);
        if(Client.VERB){
            System.out.println("[ "+req+" ]");
        }
        else if(req.equals("GOBYE***")){
            System.out.println("Partie terminée ! BYE");
            return false;
        }
        if(!req.equals("MALL!***")){
            System.out.println("Erreur recvReplyMALL" + req);
        }
        return true;

    }

    public boolean recvReplySEND() throws Exception{
        char[] buf = new char[8];
        int recu = 0;
        while(recu<8 && (recu<3 || (buf[recu-3]!='*' || buf[recu-2]!='*' || buf[recu-1]!='*'))){
            recu += br.read(buf, recu, 8-recu);
        }
        String req = new String(buf);
        if(Client.VERB){
            System.out.println("[ "+req+" ]");
        }
        if(req.equals("SEND!***")){
            System.out.println("Le message a bien été envoyé !");
        }
        else if(req.equals("NSEND***")){
            System.out.println("Le message n'a pas pu être envoyé");
        }
        else if(req.equals("GOBYE***")){
            System.out.println("Partie terminée ! BYE");
            return false;
        }
        else{
            System.out.println("Erreur recvReplySEND " + req);
        }
        return true;
    }

    public boolean recvReplyITEM() throws Exception{
        char[] buf = new char[14];
        int recu = 0;
        while(recu<14 && (recu<3 || (buf[recu-3]!='*' || buf[recu-2]!='*' || buf[recu-1]!='*'))){
            recu += br.read(buf, recu, 14-recu);
        }
        String req = new String(buf);
        if(Client.VERB){
            System.out.println("[ "+req+" ]");
        }
        if(req.substring(0,6).equals("ITEM! ")){
            byte nbBON = (byte) buf[6];
            byte nbTEL = (byte) buf[8];
            byte nbTRA = (byte) buf[10];
            System.out.println("BON (bonus) : " + nbBON + " TEL (téléporteurs) : " + nbTEL + " TRA (pièges): " + nbTRA);
        }else if(req.substring(0,8).equals("ITOFF***")){
            System.out.println("Le mode ITEMS n'est pas activé.");
        }else if(req.substring(0,8).equals("GOBYE***")){
            System.out.println("Partie terminée ! BYE");
            return false;
        }else{
            System.out.println("Erreur recvReplyITEM " + req);
        }
        return true;
    }

    public boolean recvReplyITBON() throws Exception{
        char[] buf = new char[8];
        int recu = 0;
        while(recu<8 && (recu<3 || (buf[recu-3]!='*' || buf[recu-2]!='*' || buf[recu-1]!='*'))){
            recu += br.read(buf, recu, 8-recu);
        }
        String req = new String(buf);
        if(Client.VERB){
            System.out.println("[ "+req+" ]");
        }
        if(req.equals("BONOK***")){
            System.out.println("Le bonus a été activé !");
        }
        else if(req.equals("NOITM***")){
            System.out.println("Vous ne possédez pas d'objet BON");
        }
        else if(req.equals("ITOFF***")){
            System.out.println("Le mode ITEMS n'est pas activé");
        }
        else if(req.equals("DUNNO***")){
            System.out.println("Vous avez déjà un bonus activé.");
        }
        else if(req.equals("GOBYE***")){
            System.out.println("Partie terminée ! BYE");
            return false;
        }
        else{
            System.out.println("Erreur recvReplyITBON " + req);
        }
        return true;
    }
    public boolean recvReplyITTEL() throws Exception{
        char[] buf = new char[16];
        int recu = 0;
        while(recu<16 && (recu<3 || (buf[recu-3]!='*' || buf[recu-2]!='*' || buf[recu-1]!='*'))){
            recu += br.read(buf, recu, 16-recu);
        }
        String req = new String(buf);
        if(Client.VERB){
            System.out.println("[ "+req+" ]");
        }
        if(req.substring(0,6).equals("TELOK ")){
            System.out.println("Vous vous êtes téléporté à la position x=" + req.substring(6, 9) + " y=" + req.substring(10, 13));
        }else if(req.substring(0,8).equals("NOITM***")){
            System.out.println("Vous ne possédez pas d'objet TEL");
        }else if(req.substring(0,8).equals("DUNNO***")){
            System.out.println("Vous ne pouvez pas vous téléporter à cet endroit");
        }else if(req.substring(0,8).equals("ITOFF***")){
            System.out.println("Le mode ITEMS n'est pas activé");
        }else if(req.substring(0,8).equals("GOBYE***")){
            System.out.println("Partie terminée ! BYE");
            return false;
        }else{
            System.out.println("Erreur recvReplyITTEL " + req);
        }
        return true;
    }

    public boolean recvReplyITTRA() throws Exception{
        char[] buf = new char[16];
        int recu = 0;
        while(recu<16 && (recu<3 || (buf[recu-3]!='*' || buf[recu-2]!='*' || buf[recu-1]!='*'))){
            recu += br.read(buf, recu, 16-recu);
        }
        String req = new String(buf);
        if(Client.VERB){
            System.out.println("[ "+req+" ]");
        }
        if(req.substring(0,6).equals("TRAOK ")){
            System.out.println("Le piège a bien été posé à la position x=" + req.substring(6, 9) + " y=" + req.substring(10, 13));
        }else if(req.substring(0,8).equals("NOITM***")){
            System.out.println("Vous ne possédez pas d'objet TRA");
        }else if(req.substring(0,8).equals("DUNNO***")){
            System.out.println("Vous ne pouvez pas poser un piège à cet endroit");
        }else if(req.substring(0,8).equals("ITOFF***")){
            System.out.println("Le mode ITEMS n'est pas activé");
        }else if(req.substring(0,8).equals("GOBYE***")){
            System.out.println("Partie terminée ! BYE");
            return false;
        }else{
            System.out.println("Erreur recvReplyITTRA " + req);
        }
        return true;
    }

    public void recvReplyCHEAT() throws Exception{
        char[] buf = new char[8];
        int recu = 0;
        while(recu<8 && (recu<3 || (buf[recu-3]!='*' || buf[recu-2]!='*' || buf[recu-1]!='*'))){
            recu += br.read(buf, recu, 8-recu);
        }
        String req = new String(buf);
        if(Client.VERB){
            System.out.println("[ "+req+" ]");
        }
        if(req.equals("CHTOK***")){
            System.out.println("Le mode CHEAT est activé !");
        }else if(req.equals("DUNNO***")){
            System.out.println("Vous n'avez pas le droit d'activer le mode CHEAT ou vous n'êtes pas inscrit dans une partie.");
        }else{
             System.out.println("Erreur recvReplyCHEAT" + req);
        }
    }

    public boolean recvReplyKILLG() throws Exception{
        char[] buf = new char[22];
        int recu = 0;
        while(recu<22 && (recu<3 || (buf[recu-3]!='*' || buf[recu-2]!='*' || buf[recu-1]!='*'))){
            recu += br.read(buf, recu, 22-recu);
        }
        String req = new String(buf);
        if(Client.VERB){
            System.out.println("[ "+req+" ]");
        }
        if(req.substring(0,5).equals("KILAL")){
            System.out.println("Tricheur ! Vous avez tué tous les fantomes et terminez avec "+req.substring(15,19)+" points.");
        }else if(req.substring(0,8).equals("CHOFF***")){
            System.out.println("Vous ne possédez pas le mode CHEAT.");
        }else if(req.substring(0,8).equals("GOBYE***")){
            System.out.println("Partie terminée ! BYE");
            return false;
        }else{
             System.out.println("Erreur recvReplyKILLG" + req);
        }
        return true;
    }

    public boolean recvReplyLABSH() throws Exception{
        byte[] buf1 = new byte[12];
        int recu = 0;
        while(recu<12 && (recu<3 || (buf1[recu-3]!='*' || buf1[recu-2]!='*' || buf1[recu-1]!='*'))){
            recu += s.getInputStream().read(buf1, recu, 12-recu);
        }
        String req1 = new String(buf1);
        if(req1.substring(0,5).equals("SHOW!")){
            short h = (short) ((buf1[6] & 0xff) << 8 | (buf1[7] & 0xff));
            short w = (short) ((buf1[9] & 0xff) << 8 | (buf1[10] & 0xff));
            byte[] buf2 = new byte[h*w+3];
            recu = 0;
            while(recu<h*w+3){
                recu += s.getInputStream().read(buf2, recu, h*w+3-recu);
            }
            String req2 = new String(buf2);
            if(Client.VERB){
                System.out.println("[ "+req1+req2+" ]");
            }
            System.out.println("Voici la disposition actuelle du labyrinthe :");
            for(int i = 0; i<req2.length()-w+3; i+=w){
                System.out.println(req2.substring(i, i+w));
            }
        }else if(req1.substring(0,8).equals("CHOFF***")){
            if(Client.VERB){
                System.out.println("[ "+req1+" ]");
            }
            System.out.println("Vous ne possédez pas le mode CHEAT.");
        }else if(req1.substring(0,8).equals("GOBYE***")){
            if(Client.VERB){
                System.out.println("[ "+req1+" ]");
            }
            System.out.println("Partie terminée ! BYE");
            return false;
        }else{
            System.out.println("Erreur recvReplyLABSH" + req1);
        }
        return true;
    }

    public boolean recvReplyGODME() throws Exception{
        char[] buf = new char[8];
        int recu = 0;
        while(recu<8 && (recu<3 || (buf[recu-3]!='*' || buf[recu-2]!='*' || buf[recu-1]!='*'))){
            recu += br.read(buf, recu, 8-recu);
        }
        String req = new String(buf);
        if(Client.VERB){
            System.out.println("[ "+req+" ]");
        }
        if(req.equals("GODON***")){
            System.out.println("God mode activé.");
        }else if(req.equals("ACTIF***")){
            System.out.println("Votre God mode est déjà actif.");
        }else if(req.equals("CHOFF***")){
            System.out.println("Vous ne possédez pas le mode CHEAT.");
        }else if(req.equals("GOBYE***")){
            System.out.println("Partie terminée ! BYE");
            return false;
        }else{
            System.out.println("Erreur recvReplyGODME" + req);
        }
        return true;
    }

    public boolean recvReplyUNGOD() throws Exception{
        char[] buf = new char[8];
        int recu = 0;
        while(recu<8 && (recu<3 || (buf[recu-3]!='*' || buf[recu-2]!='*' || buf[recu-1]!='*'))){
            recu += br.read(buf, recu, 8-recu);
        }
        String req = new String(buf);
        if(Client.VERB){
            System.out.println("[ "+req+" ]");
        }
        if(req.equals("GDOFF***")){
            System.out.println("God mode désactivé.");
        }else if(req.equals("NOTON***")){
            System.out.println("Votre God mode n'est pas activé.");
        }else if(req.equals("CHOFF***")){
            System.out.println("Vous ne possédez pas le mode CHEAT.");
        }else if(req.equals("GOBYE***")){
            System.out.println("Partie terminée ! BYE");
            return false;
        }else{
            System.out.println("Erreur recvReplyUNGOD" + req);
        }
        return true;
    }

    public boolean recvReplyGIVCH() throws Exception{
        char[] buf = new char[17];
        int recu = 0;
        while(recu<17 && (recu<3 || (buf[recu-3]!='*' || buf[recu-2]!='*' || buf[recu-1]!='*'))){
            recu += br.read(buf, recu, 17-recu);
        }
        String req = new String(buf);
        if(Client.VERB){
            System.out.println("[ "+req+" ]");
        }
        if(req.substring(0,5).equals("GAVE!")){
            String id = req.substring(6,14);
            System.out.println("CHEAT mode envoyé à "+id);
        }else if(req.substring(0,8).equals("HASCH***")){
            System.out.println("Ce joueur a déjà le CHEAT mode actif.");
        }else if(req.substring(0,8).equals("NOPLY***")){
            System.out.println("Aucun joueur avec cet identifiant.");
        }else if(req.substring(0,8).equals("CHOFF***")){
            System.out.println("Vous ne possédez pas le mode CHEAT.");
        }else if(req.substring(0,8).equals("GOBYE***")){
            System.out.println("Partie terminée ! BYE");
            return false;
        }else{
            System.out.println("Erreur recvReplyGIVCH " + req);
        }
        return true;
    }

    public boolean recvReplyITINF() throws Exception{
        char[] buf = new char[8];
        int recu = 0;
        while(recu<8 && (recu<3 || (buf[recu-3]!='*' || buf[recu-2]!='*' || buf[recu-1]!='*'))){
            recu += br.read(buf, recu, 8-recu);
        }
        String req = new String(buf);
        if(Client.VERB){
            System.out.println("[ "+req+" ]");
        }
        if(req.equals("INFON***")){
            System.out.println("Mode item infini activé.");
        }else if(req.equals("ACTIF***")){
            System.out.println("Votre mode d'item infini est déjà activé.");
        }else if(req.equals("CHOFF***")){
            System.out.println("Vous ne possédez pas le mode CHEAT.");
        }else if(req.equals("ITOFF***")){
            System.out.println("Le mode ITEMS n'est pas activé");
        }else if(req.equals("GOBYE***")){
            System.out.println("Partie terminée ! BYE");
            return false;
        }else{
            System.out.println("Erreur recvReplyITINF" + req);
        }
        return true;
    }

    public boolean recvReplyNOINF() throws Exception{
        char[] buf = new char[8];
        int recu = 0;
        while(recu<8 && (recu<3 || (buf[recu-3]!='*' || buf[recu-2]!='*' || buf[recu-1]!='*'))){
            recu += br.read(buf, recu, 8-recu);
        }
        String req = new String(buf);
        if(Client.VERB){
            System.out.println("[ "+req+" ]");
        }
        if(req.equals("INOFF***")){
            System.out.println("Mode item infini désactivé.");
        }else if(req.equals("NOTON***")){
            System.out.println("Votre mode d'item infini n'est pas activé.");
        }else if(req.equals("CHOFF***")){
            System.out.println("Vous ne possédez pas le mode CHEAT.");
        }else if(req.equals("ITOFF***")){
            System.out.println("Le mode ITEMS n'est pas activé");
        }else if(req.equals("GOBYE***")){
            System.out.println("Partie terminée ! BYE");
            return false;
        }else{
            System.out.println("Erreur recvReplyNOINF" + req);
        }
        return true;
    }

    public boolean recvReplyIQUIT() throws Exception{
        char[] buf = new char[8];
        int recu = 0;
        while(recu<8 && (recu<3 || (buf[recu-3]!='*' || buf[recu-2]!='*' || buf[recu-1]!='*'))){
            recu += br.read(buf, recu, 8-recu);
        }
        String req = new String(buf);
        if(Client.VERB){
            System.out.println(req);
        }
        if(req.equals("GOBYE***")){
            System.out.println("vous avez quitté la partie");
            return false;
            
        }
        else{
            System.out.println("Erreur recvReplyIQUIT" + req);
        }
        return true;
    
    }


}