import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOError;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PipedInputStream;
import java.io.PipedOutputStream;
import java.nio.Buffer;
import java.util.Random;
import java.util.concurrent.atomic.AtomicBoolean;

class RilevatoreAria extends Thread {
    final private AtomicBoolean running = new AtomicBoolean(false);
    private PipedOutputStream pos;

    public RilevatoreAria(PipedOutputStream pos) {
        this.pos = pos;
        this.running.set(false);
    }

    public void run() {
        running.set(true);
        BufferedWriter bw = new BufferedWriter(new OutputStreamWriter(pos));
        Random random = new Random();
        while (running.get()) {
            long timestamp = System.currentTimeMillis();
            double value = 0 + random.nextDouble() * 110;

            // invio i dati raccolti attraverso la pipe
            try {
                bw.write(timestamp + "");
                bw.newLine();
                bw.write(value + "");
                bw.newLine();
                bw.flush();
            } catch (IOException e) {
                e.printStackTrace();
            }

            try {
                Thread.sleep(12000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    public void fermaRilevatoreAria() {
        running.set(false);
    }
}

public class Prova20210426 {
    public static void main(String[] args) {
        PipedInputStream pis = new PipedInputStream();
        PipedOutputStream pos = null;
        try {
            pos = new PipedOutputStream(pis);
        } catch (IOException e) {
            e.printStackTrace();
            System.exit(-1);
        }

        // creo oggetto rilevatoAria
        RilevatoreAria rilevatoreAria = new RilevatoreAria(pos);
        rilevatoreAria.start();

        // parte dedicata alla lettura
        BufferedReader br = new BufferedReader(new InputStreamReader(pis));
        int i = 0;
        long lastTimeStamp = System.currentTimeMillis();
        while (i < 8) {
            Long timestamp = null;
            Double value = null;
            try {
                timestamp = Long.parseLong(br.readLine());
                value = Double.parseDouble(br.readLine());
            } catch (Exception e) {

            }

            if (timestamp > lastTimeStamp + 15000) {
                System.out.println("Attenzione");
            }

            lastTimeStamp = timestamp;

            if (value < 20) {
                System.out.println("Basso");
            } else if (value >= 20 && value < 75) {
                System.out.println("Medio");
            } else {
                System.out.println("Alto");
            }

            i++;
        }

        rilevatoreAria.fermaRilevatoreAria();

        try {
            rilevatoreAria.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}
