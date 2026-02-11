import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;
import java.util.Arrays;

public class Unmarshaller {
    public static BankRequest unmarshall(byte[] payload) {
        ByteBuffer buf = ByteBuffer.wrap(payload);
        BankRequest req = new BankRequest();

        req.methodId = buf.getInt();
        req.reqId    = buf.getInt();

        // Switch purely to determine WHICH fields to read
        switch (req.methodId) {
            case 1: // Open Account
                req.name = getString(buf);
                req.password = getString(buf);
                req.currency = getString(buf);
                req.amount = buf.getDouble();
                break;
            case 2: // Close Account
                req.name = getString(buf);
                req.accNum = buf.getInt();
                req.password = getString(buf);
                break;
            case 3: // Deposit
            case 4: // Withdraw
                req.name = getString(buf);
                req.accNum = buf.getInt();
                req.password = getString(buf);
                req.currency = getString(buf);
                req.amount = buf.getDouble();
                break;
            case 5: // Monitor
                req.monitorInterval = buf.getInt();
                break;
        }
        return req;
    }

    private static String getString(ByteBuffer buf) {
        int len = buf.getInt();
        byte[] b = new byte[len];
        buf.get(b);
        return new String(b);
    }
}