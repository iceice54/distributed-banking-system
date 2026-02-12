import java.nio.ByteBuffer;

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
                req.currency = getCurrency(buf);
                req.amount = buf.getDouble();
                System.out.println("Open account");
                break;
            case 2: // Close Account
                req.name = getString(buf);
                req.accNum = buf.getInt();
                req.password = getString(buf);
                System.out.println("Close account");
                break;
            case 3: // Withdraw
            case 4: // Deposit
                req.name = getString(buf);
                req.accNum = buf.getInt();
                req.password = getString(buf);
                req.currency = getCurrency(buf);
                req.amount = buf.getDouble();
                if (req.methodId == 3) {
                    System.out.println("Withdraw");
                } else {
                    System.out.println("Deposit");
                }
                break;
            case 5: // Monitor
                req.monitorInterval = buf.getInt();
                System.out.println("Monitor");
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

    private static Currency getCurrency(ByteBuffer buf) {
        String string = getString(buf);
        try {
            return Currency.valueOf(string.trim().toUpperCase());
        } catch (IllegalArgumentException e) {
            System.out.println("Unknown currency: " + string);
            return null;
        }
    }
}
