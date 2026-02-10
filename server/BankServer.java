import javax.xml.crypto.Data;
import java.net.*;
import java.io.*;
import java.util.Arrays;

public class BankServer {
    public static void main(String[] args) {
        DatagramSocket socket = null;
        try {
            socket = new DatagramSocket(2222);
            byte[] buffer = new byte[1000];
            System.out.println("Server running");
            while(true){
                DatagramPacket request = new DatagramPacket(buffer, buffer.length);
                socket.receive(request);
                DatagramPacket reply = new DatagramPacket(
                        request.getData(), request.getLength(),
                        request.getAddress(), request.getPort());
                String message = new String(request.getData(), 0, request.getLength());
                System.out.println("Request received: " + message);
                socket.send(reply);
            }
        } catch (IOException e) {
            throw new RuntimeException(e);
        } finally {
            if (socket != null) socket.close();
        }
    }
}
