import javax.xml.crypto.Data;
import java.net.*;
import java.io.*;
import java.util.Arrays;

public class BankServer {
    private static final int PORT = 2222;

    public static void main(String[] args) {
        DatagramSocket socket = null;
        try {
            socket = new DatagramSocket(PORT);
            System.out.println("Server is listening on " + InetAddress.getLocalHost().getHostAddress() + ":" + PORT);

            while (true) {
                byte[] buffer = new byte[1024];
                DatagramPacket requestPacket = new DatagramPacket(buffer, buffer.length);
                socket.receive(requestPacket);

                byte[] requestData = Arrays.copyOf(requestPacket.getData(), requestPacket.getLength());

                System.out.print("Client: " + requestPacket.getAddress() + ":" + requestPacket.getPort() + " ");

                BankRequest req = Unmarshaller.unmarshall(requestData);

                BankResponse resp = BankService.process(req, requestPacket.getAddress(), requestPacket.getPort());

//                byte[] replyData = Marshaller.marshall(resp);
//
//                DatagramPacket reply = new DatagramPacket(
//                        replyData, replyData.length,
//                        requestPacket.getAddress(), requestPacket.getPort()
//                );
//                socket.send(reply);
            }
        } catch (IOException e) {
            System.out.println("Client disconnected: " + e.getMessage());
        } finally {
            if (socket != null) socket.close();
        }
    }
}
