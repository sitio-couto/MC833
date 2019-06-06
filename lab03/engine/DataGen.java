package engine;

import java.util.HashMap;
import java.util.List;
import java.util.LinkedList;
import java.io.*;
import engine.Profile;

public class DataGen {

    public HashMap<String,Profile> map = null;

    public DataGen() {
        
        // Instanciate the profiles
        List<String> profile1_exp = new LinkedList<String>();
        profile1_exp.add("Servidores Linux");
        profile1_exp.add("Segurança em redes");
        profile1_exp.add("Criptografia PGP");
        Profile profile1 = new Profile("carlos@unicamp.br", "Carlos", "Merenda", "Campinas", "Engenharia de Computação", "Criptografia, Servidores", profile1_exp);

        List<String> profile2_exp = new LinkedList<String>();
        profile2_exp.add("Redesign de interfaces");
        profile2_exp.add("Acessibilidade em sites");
        Profile profile2 = new Profile("joao@unicamp.br", "João", "Santos", "São Paulo", "Ciência da Computação", "UI, Acessibilidade", profile2_exp);

        // Build the HashMap
        map.put("carlos@unicamp.br", profile1);
        map.put("joao@unicamp.br", profile2);

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
}
