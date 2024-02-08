import java.io.*;
import java.net.*;

public class Client{
    public static boolean isNumeric(String n) {
        try {
            int d = Integer.parseInt(n);
        } catch (NumberFormatException e) {
            return false;
        }
        return true;
    }  
    public static boolean VERB = false;
    public static void main(String[] args){
        try{
            if(args.length<4 || !isNumeric(args[0]) || !isNumeric(args[2]) || !isNumeric(args[3])){
                System.out.println("4 arguments attendus : Port UDP, Adresse serveur, Port serveur et mode verbeux (0 ou 1)");
                return;
            }
            if(args[3].equals("0")){
                VERB = false;
            }else if(args[3].equals("1")){
                VERB = true;
            }else{
                System.out.println("L'argument pour le mode verbeux doit soit être 0 ou 1.");
                return;
            }
            Socket sock = new Socket(args[1], Integer.valueOf(args[2]));
            BufferedReader br = new BufferedReader(new InputStreamReader(sock.getInputStream()));
            PrintWriter pw = new PrintWriter(new OutputStreamWriter(sock.getOutputStream()));
            Request rq = new Request(br, pw, sock.getOutputStream(), args[0], sock);
            rq.recvReplyGAME();
            rq.listCommandBeforeGame();
            while(rq.requestBeforeGame()){
            }
            MulticastSocket mso=new MulticastSocket(rq.getPortMulticast());
            Runnable run1 = ()->{
                try{
                    
                    mso.joinGroup(InetAddress.getByName(rq.getIpMulticast()));
                    byte[]data=new byte[250];
                    DatagramPacket paquet=new DatagramPacket(data,data.length);
                    while(true){
                        mso.receive(paquet);
                        String message = new String(paquet.getData(),0,paquet.getLength());
                        if(VERB){
                            System.out.println("[ "+ message +" ]");
                        }
                        if(message.substring(0,6).equals("MESSA ")){
                            System.out.println("Message multicast - " + message.substring(6, 14) + " :"+ message.substring(14, message.length()-3));
                        }else if(message.substring(0,6).equals("GHOST ")){
                            System.out.println("Un fantôme s'est déplacé ! x = " + message.substring(6, 9) + " y = " + message.substring(10, 13) );
                        }
                        else if(message.substring(0,6).equals("SCORE ")){
                            System.out.println("Fantôme attrapé ! - " + message.substring(6, 14) + " : "+ "score =" + message.substring(14, 19) + " x ="+message.substring(19, 23) + " y ="+message.substring(23, 27) );
                        }else if(message.substring(0,6).equals("ENDGA ")){
                            System.out.println("Partie terminée ! Le vainqueur est : "
                                                +message.substring(6,14)
                                                +" avec "+message.substring(15,19)+" points !");
                        }
                        else if(message.substring(0,6).equals("OBTAI ")){
                            System.out.println("Objet obtenu ! - " + message.substring(6, 14) + " :"+ message.substring(14, message.length()-3) );
                        }else if(message.substring(0,6).equals("PENAL ")){
                            System.out.println("Aïe ! le joueur " + message.substring(6, 14) + " est tombé dans un piège ! score ="+ message.substring(14, message.length()-3) );
                        }else if(message.substring(0,6).equals("FGONE ")){
                            System.out.println("Un tricheur a eu tous les fantômes ! - " 
                                                + message.substring(6, 14) 
                                                + " termine avec"+ message.substring(14, message.length()-3)+" points.");
                        }
                        else{
                            System.out.println("Message multicast reçu non conforme : " + message);
                        }
                    }
                }
                catch(SocketException e){
                }
                catch(Exception e){
				    System.out.println(e);
                }
            };
             DatagramSocket dso=new DatagramSocket(Integer.valueOf(args[0]));
             Runnable run2 = ()->{
                try{
                   
                    byte[]data=new byte[250];
                    DatagramPacket paquet=new DatagramPacket(data,data.length);
                    while(true){
                        dso.receive(paquet);
                        String message = new String(paquet.getData(),0,paquet.getLength());
                        if(VERB){
                            System.out.println("[ "+ message +" ]");
                        }
                        if(message.substring(0,6).equals("MESSP ")){
                            System.out.println("Message privé - " + message.substring(6, 14) + " :"+ message.substring(14, message.length()-3));
                        }else{
                            System.out.println("Message privé reçu non conforme : " + message);
                        }
                    }
                }
                catch(SocketException e){
				    
                }
                catch(Exception e){
				    System.out.println(e);
                }
            };
            Thread t1 = new Thread(run1);
            Thread t2 = new Thread(run2);
			t1.start();
            t2.start();
            rq.recvReplyPOSIT();
            rq.listCommandDuringGame();
            while(rq.requestDuringGame()){
                
            }
            br.close(); 
            pw.close();
            mso.close();
            dso.close();
            t1.interrupt();
            t2.interrupt();
        } catch(Exception e){
            e.printStackTrace();
        }
    }
}