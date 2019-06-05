package client;

import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.util.Scanner;
import compute.Compute;

public class Client {

    public String request;

    public static void main(String args[]) {
        if (System.getSecurityManager() == null) {
            System.setSecurityManager(new SecurityManager());
        }
        try {
            String name = "Compute";
            Registry registry = LocateRegistry.getRegistry(args[0]);
            Compute comp = (Compute) registry.lookup(name);

            // Print help
            System.out.println(comp.executeRequest("help"));

            Scanner scanner = new Scanner(System.in);

            while(true){
                
                // Read client input
                request = scanner.nextLine();

                // Exit if requested
                if(request.equals("exit")){
                    break;
                }

                // Send request to server and print response
                String response = comp.executeRequest(this.request);
                System.out.println(response);                

            }

        } catch (Exception e) {
            System.err.println("Client exception:");
            e.printStackTrace();
        }
    }
}
