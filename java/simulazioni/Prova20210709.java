//Simulazione esame 9 luglio 2021

import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.PipedInputStream;
import java.io.PipedOutputStream;
import java.io.Serializable;
import java.util.Random;
import java.util.concurrent.atomic.AtomicBoolean;

class Consumi implements Serializable {
    private float consumi;

    public Consumi(float consumi) {
        this.consumi = consumi;
    }

    public synchronized float getConsumi() {
        return consumi;
    }

    public synchronized void setConsumi(float consumi) {
        this.consumi = consumi;
    }
}

class SimulaConsumi implements Runnable {
    private final AtomicBoolean running = new AtomicBoolean(false);
    private PipedOutputStream pos = null;

    public SimulaConsumi(PipedOutputStream pos) {
        this.pos = pos;
    }

    public void run() {
        running.set(true);

        // Creo canale comunicazione OOS
        ObjectOutputStream oos = null;
        try {
            oos = new ObjectOutputStream(pos);
        } catch (IOException e) {
            e.printStackTrace();
            System.exit(-2);
        }

        Random random = new Random();
        float value = 0;
        while (running.get()) {
            // generazione dei dati
            value = random.nextFloat() * 30;
            Consumi consumi = new Consumi(value);

            // invio dei dati
            try {
                oos.writeObject(consumi);
                oos.flush();
            } catch (IOException e) {
                e.printStackTrace();
            }

            // Fermo per 1 secondo il thread
            try {
                Thread.sleep(1000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

    }

    public void terminaSimulazioneConsumi() {
        running.set(false);

    }
}

public class Prova20210709 {
    public static void main(String[] args) {
        PipedInputStream pis = new PipedInputStream();
        PipedOutputStream pos = null;
        try {
            pos = new PipedOutputStream(pis);
        } catch (IOException e) {
            e.printStackTrace();
            System.exit(-1);
        }

        // Creo oggetto SimulaConsumi e avvio il suo thread
        SimulaConsumi sc = new SimulaConsumi(pos);
        Thread scThread = new Thread(sc);
        scThread.start();

        // Creo Object input stream
        ObjectInputStream ois = null;
        try {
            ois = new ObjectInputStream(pis);
        } catch (IOException e) {
            e.printStackTrace();
            System.exit(-2);
        }

        // logica di controllo del main
        int count = 0;
        float lastValue = 0;
        while (count < 3) {
            // lettura dalla pipe del valore
            Consumi consumi = null;
            try {
                consumi = (Consumi) ois.readObject();
            } catch (IOException | ClassNotFoundException e) {
                e.printStackTrace();
            }

            float currentValue = consumi.getConsumi();

            // logica decisionale in base al contenuto della pipe
            if (lastValue != 0) {
                if ((currentValue - lastValue) > ((lastValue / 100) * 30)) {
                    System.out.println("WARNING");
                }
            }

            if (currentValue > 20) {
                count++;
            } else {
                count = 0;
            }

            lastValue = currentValue;

            // fermo il main per 900ms
            try {
                Thread.sleep(900);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }

        }

        // Termino e attendo fine thread simula consumi
        sc.terminaSimulazioneConsumi();
        try {
            scThread.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}
