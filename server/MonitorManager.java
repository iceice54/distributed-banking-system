import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.util.ArrayList;
import java.util.List;
import java.util.logging.Logger;

public class MonitorManager {

    private static final Logger logger = Logger.getLogger(MonitorManager.class.getName());

    private record ClientEntry(InetAddress address, int port, long expiryMs) {}

    private static final List<ClientEntry> clients = new ArrayList<>();

    public static void register(InetAddress address, int port, int durationSeconds) {
        final long expiry = System.currentTimeMillis() + (durationSeconds * 1000L);
        clients.add(new ClientEntry(address, port, expiry));
        logger.info("Registered monitor client: " + address + ":" + port
                + " for " + durationSeconds + "s");
    }

    public static synchronized void notifyClients(DatagramSocket socket, byte[] update) {
        final long now = System.currentTimeMillis();
        clients.removeIf(c -> c.expiryMs() < now);

        for (ClientEntry client : clients) {
            try {
                final DatagramPacket packet = new DatagramPacket(
                        update, update.length,
                        client.address(), client.port()
                );
                socket.send(packet);
                logger.info("Notified monitor client: " + client.address() + ":" + client.port());
            } catch (IOException e) {
                logger.warning("Failed to notify monitor client: "
                        + client.address() + " - " + e.getMessage());
            }
        }
    }

}
