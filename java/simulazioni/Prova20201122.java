import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.PipedInputStream;
import java.io.PipedOutputStream;
import java.io.Serializable;
import java.util.Random;
import java.util.concurrent.atomic.AtomicBoolean;

class Message implements Serializable {
    private int value;

    public Message(int value) {
        this.value = value;
    }

    public int getValue() {
        return value;
    }
}

class Overall {
    private int corretti;
    private int difetti;

    public Overall(int corretti, int difetti) {
        this.corretti = corretti;
        this.difetti = difetti;
    }

    public synchronized int getCorretti() {
        return corretti;
    }

    public synchronized int getDifetti() {
        return difetti;
    }

    public synchronized void incremetaCorretti() {
        corretti++;
    }

    public synchronized void incrementaDifetti() {
        difetti++;
    }
}

class Macchine extends Thread {
    private final AtomicBoolean running = new AtomicBoolean(false);
    private PipedOutputStream pos;
    private int count;

    public Macchine(PipedOutputStream pos) {
        this.pos = pos;
        this.count = 0;
        this.running.set(false);
    }

    public void run() {
        running.set(true);

        ObjectOutputStream oos = null;
        try {
            oos = new ObjectOutputStream(pos);
        } catch (IOException e) {
            e.printStackTrace();
        }

        Random random = new Random();

        while (running.get()) {
            int value = -1000 + random.nextInt(2000);

            // invio l'oggetto al thread Quality
            try {
                Message message = new Message(value);
                oos.writeObject(message);
            } catch (Exception e) {
                e.printStackTrace();
            }

            count++;

            // Metto in sleep il processo per 750ms
            try {
                Thread.sleep(750);
            } catch (InterruptedException e) {
                System.out.println("Processo interrotto: " + e.getStackTrace());
            }
        }
    }

    public void termina() {
        running.set(false);
    }
}

class Quality extends Thread {
    private final AtomicBoolean running = new AtomicBoolean(false);
    private PipedInputStream pis;
    private Overall overall;

    public Quality(PipedInputStream pis, Overall overall) {
        this.pis = pis;
        this.overall = overall;
    }

    public void run() {
        running.set(true);
        ObjectInputStream ois = null;

        try {
            ois = new ObjectInputStream(pis);
        } catch (Exception e) {
            e.printStackTrace();
        }

        Message message = null;

        while (running.get()) {
            try {
                message = (Message) ois.readObject();
            } catch (IOException e) {
                e.printStackTrace();
            } catch (ClassNotFoundException e) {
                e.printStackTrace();
            }

            if (message == null) {
                break;
            }

            int value = message.getValue();
            if (value < 0) {
                // Incremento difetti e stampo errore
                overall.incrementaDifetti();
                System.out.println("Errore");
            } else {
                // Pezzo prodotto correttamente e stampo ok
                overall.incremetaCorretti();
                System.out.println("OK");
            }
        }
    }

    public void termina() {
        running.set(false);
    }
}

public class Prova20201122 {
    public static void main(String[] args) {
        PipedInputStream pis = new PipedInputStream();
        PipedOutputStream pos = null;
        try {
            pos = new PipedOutputStream(pis);
        } catch (IOException e) {
            System.out.println("Errore creazione pipe: ");
            System.exit(-1);
        }

        // istanzio e faccio lo start dei due processi
        Overall overall = new Overall(0, 0);
        Macchine macchine = new Macchine(pos);
        Quality quality = new Quality(pis, overall);

        macchine.start();
        quality.start();

        // verifico ogni tot secondi se la condizione è rispettata altrimenti esco dal
        // ciclo termino i processi e attendo la fine
        while (true) {
            try {
                Thread.sleep(1000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            System.out.println(overall.getCorretti() + " == " + overall.getDifetti());
            if (overall.getCorretti() != 0 && overall.getDifetti() != 0
                    && overall.getCorretti() == overall.getDifetti()) {
                break;
            }
        }

        // Fermo i thread
        macchine.termina();
        quality.termina();

        // Attendo la fine dei due thread
        try {
            macchine.join();
            quality.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}
