import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PipedInputStream;
import java.io.PipedOutputStream;
import java.nio.charset.Charset;
import java.util.Arrays;

class FromInput extends Thread {
    private PipedOutputStream pos;

    FromInput(PipedOutputStream pos) {
        this.pos = pos;
    }

    public void run() {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        String message;
        try {
            while ((message = br.readLine()) != null) {
                byte[] bytes = message.getBytes(Charset.forName("UTF-8"));
                // Scrivo dal primo byte fino alla fine
                pos.write(bytes, 0, bytes.length);
                /*
                 * Forzo la scrittura sulla pipe in modo
                 * tale che scriva immediatamente senza attendere
                 */
                pos.flush();
            }
            pos.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}

class ToOutput extends Thread {
    private PipedInputStream pis;

    ToOutput(PipedInputStream pis) {
        this.pis = pis;
    }

    public void run() {
        byte[] buffer = new byte[1024];
        int nRead = 0;
        try {
            /*
             * Se la nRead < o = 0 allora in tal caso vuol dire
             * che la pipe è stata chiusa
             */
            while ((nRead = pis.read(buffer)) > 0) {
                /*
                 * Perchè l'array buffer ha lunghezza 1024, invece message
                 * buffer ha lunghezza la lunghezza della stringa
                 */
                byte messageBuffer[] = Arrays.copyOfRange(buffer, 0, nRead);
                String message = new String(messageBuffer, 0, messageBuffer.length);
                System.out.println(message);
            }
            pis.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}

public class Esercizio2_v1 {
    public static void main(String[] args) {
        PipedInputStream pis = new PipedInputStream();
        try {
            PipedOutputStream pos = new PipedOutputStream(pis);

            FromInput fi = new FromInput(pos);
            ToOutput to = new ToOutput(pis);

            to.start();
            fi.start();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
