import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PipedInputStream;
import java.io.PipedOutputStream;
import java.util.concurrent.atomic.AtomicBoolean;

class VerificaInput {
    private int qta;

    public VerificaInput() {
        this.qta = 0;
    }

    public synchronized void incrementaStringheSospette() {
        qta++;
    }

    public synchronized int getNumeroStringheSospette() {
        return qta;
    }
}

class InputUtente implements Runnable {
    private PipedOutputStream pos;
    private final AtomicBoolean running = new AtomicBoolean(false);

    public InputUtente(PipedOutputStream pos) {
        this.pos = pos;
    }

    public void run() {
        running.set(true);
        // Stream per leggere da tastiera
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));

        // Stream di tipo buffered
        final BufferedWriter bw = new BufferedWriter(new OutputStreamWriter(pos));
        while (running.get()) {
            try {
                // lettura input da tastiera
                String line = br.readLine();

                // Comunicazione tramite buffered con thread scansiona
                bw.write(line);
                bw.newLine();
                bw.flush();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

    }

    public void termina() {
        running.set(false);
    }
}

class ScansionaInput implements Runnable {
    private final AtomicBoolean running = new AtomicBoolean(false);
    private PipedInputStream pis;
    VerificaInput vi;

    public ScansionaInput(PipedInputStream pis, VerificaInput vi) {
        this.pis = pis;
        this.vi = vi;
    }

    public void run() {
        running.set(true);
        BufferedReader br = new BufferedReader(new InputStreamReader(pis));
        String line = null;

        while (running.get()) {
            try {
                line = br.readLine();
            } catch (IOException e) {
                System.out.println("Errore lettura dalla pipe: " + e.getMessage());
            }

            if (line.equals("abcde") || line.equals("1234")) {
                System.out.println("PERICOLO");
                vi.incrementaStringheSospette();
            } else {
                System.out.println("OK");
            }

        }
    }

    public void termina() {
        running.set(false);
    }
}

public class Prova20200619 {
    public static void main(String[] args) {
        PipedInputStream pis = new PipedInputStream();
        PipedOutputStream pos = null;
        try {
            pos = new PipedOutputStream(pis);
        } catch (IOException e) {
            System.err.println("Errore creazione pipe: " + e.getMessage());
            System.exit(-1);
        }

        VerificaInput vi = new VerificaInput();

        InputUtente iu = new InputUtente(pos);
        Thread tiu = new Thread(iu);
        tiu.start();

        ScansionaInput si = new ScansionaInput(pis, vi);
        Thread tsi = new Thread(si);
        tsi.start();

        while (vi.getNumeroStringheSospette() <= 3) {
            try {
                Thread.sleep(200);
            } catch (InterruptedException e) {
                System.exit(-1);
            }
        }

        iu.termina();
        si.termina();

        try {
            tiu.join();
            tsi.join();
        } catch (InterruptedException e) {
            System.err.println("Attendo terminazione thread");
        }
    }
}