import java.net.InetAddress;
import java.util.HashMap;

public class BankService {
    // In-memory database (for simplicity)
    private static HashMap<Integer, Account> accounts = new HashMap<>();
    private static int nextAccNum = 1000;

    public static BankResponse process(BankRequest req, InetAddress clientIP, int clientPort) {
        BankResponse resp = new BankResponse();
        resp.reqId = req.reqId;

        switch (req.methodId) {
            case 1: // Open
                int newId = nextAccNum++;
                accounts.put(newId, new Account(req.name, req.amount, req.password));
                resp.status = 1; // Success
                resp.message = "Account Created. ID: " + newId;
                break;

            case 3: // Deposit
                if (accounts.containsKey(req.accNum)) {
                    Account acc = accounts.get(req.accNum);
                    // Verify Password logic here...
                    acc.balance += req.amount;
                    resp.status = 1;
                    resp.balance = acc.balance;
                } else {
                    resp.status = 0; // Fail
                    resp.message = "Invalid Account ID";
                }
                break;

//            case 5: // Monitor
//                // Register this client for updates
//                MonitorManager.addClient(clientIP, clientPort, req.monitorInterval);
//                resp.status = 1;
//                resp.message = "Monitoring started for " + req.monitorInterval + "s";
//                break;
        }
        return resp;
    }
}