package engine;

import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.rmi.server.UnicastRemoteObject;
import java.io.FileOutputStream;
import java.util.HashMap;
import java.lang.split;
import java.lang.Integer.parseInt;
import compute.Compute;

public class ComputeEngine implements Compute {

    public HashMap<String,Profile> map = null;

    public ComputeEngine() {

        // De-serialization
        try {
            FileInputStream fis = new FileInputStream("data.ser");
            ObjectInputStream ois = new ObjectInputStream(fis);
            map = (HashMap) ois.readObject();
            ois.close();
            fis.close();
        } catch(IOException ioe) {
            ioe.printStackTrace();
            return;
        }

        // Call executeRequest()
        super();

        // Serialization
        try {
            FileOutputStream fos =
            new FileOutputStream("data.ser");
            ObjectOutputStream oos = new ObjectOutputStream(fos);
            oos.writeObject(map);
            oos.close();
            fos.close();
        } catch(IOException ioe) {
            ioe.printStackTrace();
            return;
        }

    }

    public String executeRequest(String request) {
        String response;

        try {
            
            // Split request into type and data
            String[] input = s.split(" ", 2);

            // Send help information
            if (input[0].equals("help"){
                return "Opções disponíveis: \n" +
                       "1 - listar todas as pessoas formadas em um determinado curso; \n" +
                       "2 - listar as habilidades dos perfis que moram em uma determinada cidade; \n" +
                       "3 - acrescentar uma nova experiência em um perfil; \n" +
                       "4 - dado o email do perfil, retornar sua experiência; \n" +
                       "5 - listar todas as informações de todos os perfis; \n" +
                       "6 - dado o email de um perfil, retornar suas informações."
            }
            
            // Select request type
            switch(input[0].parseInt()) {
                case 1:
                    System.out.println("retrieving name by course...");
                    response = namesByCourse(input[1]);
                    System.out.println("names retrieved");
                    break;
                case 2:
                    System.out.println("retrieving habilities by city...");
                    response = habilitiesByCity(input[1]);
                    System.out.println("habilities retrieved");
                    break;
                case 3:
                    System.out.println("adding experience...");
                    response = addExperience(input[1]);
                    break;
                case 4:
                    System.out.println("retrieving experiences...");
                    response = getExperience(input[1]);
                    System.out.println("experiences retrieved");
                    break;
                case 5:
                    System.out.println("sending all profiles...");
                    response = getAllProfiles(input[1]);
                    System.out.println("all profiles sent");
                    break;
                case 6:
                    System.out.println("retrieving profile...");
                    response = getProfile(input[1]);
                    System.out.println("profile sent");
                    break;
                default:
                    System.out.println("Invalid option");
            }
        } catch(NumberFormatException nfe) {
            return "Invalid option: " + nfe.getMessage();
        }
        
        return response;
    }

    // TODO: Option functions
    
    public String namesByCourse(String data) {
        String response;

        return response;
    }

    public String habilitiesByCity(String data) {
        String response;

        return response;
    }

    public String addExperience(String data) {
        String response;

        return response;
    }

    public String getExperience(String data) {
        String response;

        return response;
    }

    public String getAllProfiles(String data) {
        String response;

        return response;
    }

    public String getProfile(String data) {
        String response;

        return response;
    }

    public static void main(String[] args) {
        if (System.getSecurityManager() == null) {
            System.setSecurityManager(new SecurityManager());
        }
        try {
            String name = "Compute";
            Compute engine = new ComputeEngine();
            Compute stub = (Compute) UnicastRemoteObject.exportObject(engine, 0);
            Registry registry = LocateRegistry.getRegistry();
            registry.rebind(name, stub);
            System.out.println("ComputeEngine bound");
        } catch (Exception e) {
            System.err.println("ComputeEngine exception:");
            e.printStackTrace();
        }
    }
}
