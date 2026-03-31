import java.net.*;
import java.io.*;
import java.util.Arrays;
import java.util.logging.Logger;

public class BankServer {
    private static final int PORT = 2222;
    private static final int BUFFER_SIZE = 1024;
    private static final Logger logger = Logger.getLogger(BankServer.class.getName());
    private static final double DROP_PROBABILITY = 0.2;

    private static boolean shouldDrop() {
        return Math.random() < DROP_PROBABILITY;
    }

    public static void main(String[] args) {
        // Set up server in at-least-once or at-most-once mode
        if (args.length != 1) {
            System.err.println("Usage: Bankserver <alo|amo>");
            System.exit(1);
        }
        final boolean atMostOnce = args[0].equals("amo");

        try (DatagramSocket socket = new DatagramSocket(PORT)) {
            logger.info("Server is listening on " + InetAddress.getLocalHost().getHostAddress() + ":" + PORT);
            if (atMostOnce) {
                logger.info("Server is using at-most-once semantics.");
            } else {
                logger.info("Server is using at-least-once semantics.");
            }
            final byte[] buffer = new byte[BUFFER_SIZE];

            // Main loop
            while (true) {
                final DatagramPacket requestPacket = new DatagramPacket(buffer, buffer.length);
                socket.receive(requestPacket);

                // Simulate dropped incoming packet (method not processed)
                if (shouldDrop()) {
                    logger.info("[SIMULATE] Dropped request from "
                            + requestPacket.getAddress() + ":" + requestPacket.getPort());
                    continue;
                }

                logger.info("Client at " + requestPacket.getAddress() + ":" + requestPacket.getPort());

                final byte[] requestData = Arrays.copyOf(requestPacket.getData(), requestPacket.getLength());
                final BankRequest req = Unmarshaller.unmarshall(requestData);
                final String key = RequestHistory.makeKey(
                        requestPacket.getAddress(), requestPacket.getPort(), req.reqId);
                byte[] replyData;

                if (atMostOnce && RequestHistory.contains(key)) {
                    // Duplicate request — return cached response
                    logger.info("Duplicate request detected, returning cached response for key: " + key);
                    replyData = RequestHistory.get(key);
                } else {
                    // New request — process it
                    final BankResponse resp = BankService.process(
                            req, requestPacket.getAddress(), requestPacket.getPort(), socket);
                    replyData = Marshaller.marshall(resp);

                    if (atMostOnce) {
                        RequestHistory.put(key, replyData);
                    }
                }

                // Simulate dropped outgoing packet (method already processed)
                // Leads to inconsistent data when using at-least-once semantics
                if (shouldDrop()) {
                    logger.info("[SIMULATE] Dropped reply to "
                            + requestPacket.getAddress() + ":" + requestPacket.getPort());
                    continue;
                }

                socket.send(new DatagramPacket(
                        replyData, replyData.length,
                        requestPacket.getAddress(), requestPacket.getPort()
                ));
            }
        } catch (IOException e) {
            System.out.println("Server error: " + e.getMessage());
        }
    }
}
