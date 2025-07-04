import java.lang.Thread;
import java.io.FileWriter;
import java.io.IOException;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingDeque;

public class javaLogger implements AutoCloseable {
    // Não estou a implementar niveis como debug ou Info pois não foi pedido
    // Caso fosse implementar possivelmente usaria um Enum para definir os niveis
    private FileWriter fileWriter;
    private BlockingQueue<String> logQueue;
    private Thread logThread;
    private boolean isRunning;
    public javaLogger(String filename) {
        try {
            this.fileWriter = new FileWriter(filename, true);
            this.logQueue = new LinkedBlockingDeque<>();
            this.logThread= new Thread(this::writetoFIle);
            this.isRunning= true;
            logThread.start();
        } catch (IOException e) {
            System.err.println("Error opening file: " + filename);
            e.printStackTrace();
        }
    }

    public void log(String message) {
        try {
            logQueue.put(message);
        } catch (InterruptedException e) {
            System.err.println("Thread interrupted while logging message: " + message);
            Thread.currentThread().interrupt();
        }
    }
    
    private void writetoFIle(){
          while (isRunning) {
            try {
                String message = logQueue.take();
                fileWriter.write(message + "\n");
                fileWriter.flush();
            } catch (InterruptedException e) {
                logThread.interrupt();
                break;
            } catch (IOException e) {
                System.err.println("Error writing to file: " + e.getMessage());
            }
        }
    }

    @Override
    public void close() throws Exception {
        isRunning=false;
        try{
            if(!logThread.isInterrupted()|| logThread.isAlive()){
            logThread.interrupt();
            logThread.join(1000);
            }
            if(fileWriter !=null){
                fileWriter.close();
            }
        } catch (IOException | InterruptedException e) {
            System.err.println("Failed to close the class:" + e.getMessage());
        }
    }
        



    

    public static void main(String[] args) {
        
    }
    
}


