import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PipedInputStream;
import java.io.PipedOutputStream;
import java.nio.charset.Charset;
import java.util.Arrays;

class FromInput_v2 extends Thread {
    private PipedOutputStream pos;

    FromInput_v2(PipedOutputStream pos) {
        this.pos = pos;
    }

    public void run() {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        String message = null;
        try {
            while ((message = br.readLine()) != null) {
                BufferedWriter bw = new BufferedWriter(new OutputStreamWriter(pos));
                bw.write(message);
                bw.newLine();
                bw.flush();
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}

class ToOutput_v2 extends Thread {
    private PipedInputStream pis;

    ToOutput_v2(PipedInputStream pis) {
        this.pis = pis;
    }

    public void run() {
        String messageRead;
        try {
            BufferedReader br = new BufferedReader(new InputStreamReader(pis));
            while ((messageRead = br.readLine()) != null) {
                System.out.println("Il messaggio letto dalla pipe: " + messageRead);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}

public class Esercizio2_v2 {
    public static void main(String[] args) {
        PipedInputStream pis = new PipedInputStream();
        try {
            PipedOutputStream pos = new PipedOutputStream(pis);

            FromInput_v2 fi = new FromInput_v2(pos);
            ToOutput_v2 to = new ToOutput_v2(pis);

            to.start();
            fi.start();

        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
