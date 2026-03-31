import java.net.InetAddress;
import java.util.HashMap;
import java.util.Map;

public class RequestHistory {

    private static final Map<String, byte[]> history = new HashMap<>();

    // Key is clientIP:port:reqId
    public static String makeKey(InetAddress address, int port, int reqId) {
        return address.getHostAddress() + ":" + port + ":" + reqId;
    }

    public static boolean contains(String key) {
        return history.containsKey(key);
    }

    public static byte[] get(String key) {
        return history.get(key);
    }

    public static void put(String key, byte[] response) {
        history.put(key, response);
    }
}