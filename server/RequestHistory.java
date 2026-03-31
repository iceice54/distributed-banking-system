import java.net.InetAddress;
import java.util.LinkedHashMap;
import java.util.Map;

public class RequestHistory {

    private static final Map<String, byte[]> history = new LinkedHashMap<>();

    // Key is clientIP:port:reqId — uniquely identifies a request
    public static String makeKey(InetAddress address, int port, int reqId) {
        return address.getHostAddress() + ":" + port + ":" + reqId;
    }

    public static synchronized boolean contains(String key) {
        return history.containsKey(key);
    }

    public static synchronized byte[] get(String key) {
        return history.get(key);
    }

    public static synchronized void put(String key, byte[] response) {
        history.put(key, response);
    }
}