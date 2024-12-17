//SImulazione esame 20210616

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.PipedInputStream;
import java.io.PipedOutputStream;
import java.io.Serializable;
import java.util.Random;
import java.util.concurrent.atomic.AtomicBoolean;

class Rilevazione implements Serializable {
    private int valore;
    private long timestamp;

    public Rilevazione(int valore, long timestamp) {
        this.valore = valore;
        this.timestamp = timestamp;
    }

    public synchronized int getValore() {
        return valore;
    }

    public synchronized long getTimestamp() {
        return timestamp;
    }

    public synchronized void setTimestamp(long timestamp) {
        this.timestamp = timestamp;
    }

    public synchronized void setValore(int valore) {
        this.valore = valore;
    }
}

class GeneraRilevazione extends Thread {
    private final AtomicBoolean running = new AtomicBoolean(false);
    private PipedOutputStream pos = null;

    public GeneraRilevazione(PipedOutputStream pos) {
        this.pos = pos;
    }

    public void run() {
        running.set(true);
        ObjectOutputStream oos = null;
        try {
            oos = new ObjectOutputStream(pos);
        } catch (IOException e) {
            e.printStackTrace();
            System.exit(-2);
        }

        Random random = new Random();
        int value;
        long timestamp;

        while (running.get()) {
            // generazione dei valori
            value = random.nextInt(200);
            timestamp = System.currentTimeMillis();
            Rilevazione rilevazione = new Rilevazione(value, timestamp);

            // Invio dell'oggetto al main
            try {
                oos.writeObject(rilevazione);
                oos.flush();
            } catch (IOException e) {
                System.err.println("Errore invio della rilevazione: " + e.getMessage());
            }

            try {
                Thread.sleep(3000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }

        }
    }

    public void terminaRilevazione() {
        running.set(false);

        // interruzione immediata
        Thread.interrupted();
    }

}

public class Prova20210616 {
    public static void main(String[] args) {
        // Crezione delle pipe
        PipedInputStream pis = new PipedInputStream();
        PipedOutputStream pos = null;
        try {
            pos = new PipedOutputStream(pis);
        } catch (IOException e) {
            e.printStackTrace();
            System.exit(-1);
        }

        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        int soglia = 0;
        while (true) {
            try {
                System.out.println("Inserisci il valore della sooglia (integer): ");
                soglia = Integer.parseInt(br.readLine());
                break;
            } catch (NumberFormatException e) {
                System.out.println("Errore nel parsing, assicurati di inserire un numero di tipo integer");
                continue;
            } catch (IOException e) {
                System.out.println("Errore lettura da tastiera");
                break;
            }
        }

        // Creazione degli oggetti e avvio processo
        GeneraRilevazione gr = new GeneraRilevazione(pos);
        gr.start();

        // Objcect input stream per leggere dalla pipe
        ObjectInputStream ois = null;
        try {
            ois = new ObjectInputStream(pis);
        } catch (Exception e) {
            e.printStackTrace();
        }

        int i = 0;
        int nOverSoglia = 0;
        while (i < 10) {
            // Lettura dalla pipe
            Rilevazione r = null;
            try {
                r = (Rilevazione) ois.readObject();
            } catch (ClassNotFoundException e) {
                System.err.println("Classe non trovata" + e.getMessage());
            } catch (IOException e) {
                e.printStackTrace();
            }

            int value = r.getValore();
            if (value > soglia) {
                nOverSoglia++;
            }

            if (value <= 50) {
                System.out.println("Basso");
            } else if (value < 100 && value > 50) {
                System.out.println("Moderato");
            } else {
                System.out.println("Alto");
            }

            if (nOverSoglia == 3) {
                System.out.println(nOverSoglia + " Attenzione");
                nOverSoglia = 0;
            }

            i++;
        }

        // Termino thread e attendo la sua conclusione
        gr.terminaRilevazione();
        try {
            gr.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

    }
}