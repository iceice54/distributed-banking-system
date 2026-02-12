import java.net.InetAddress;
import java.util.HashMap;
import java.util.Objects;

public class BankService {
    // In-memory database (for simplicity)
    private static HashMap<Integer, Account> accounts = new HashMap<>();
    private static int nextAccNum = 1000;

    public static BankResponse process(BankRequest req, InetAddress clientIP, int clientPort) {
        BankResponse resp = new BankResponse();
        resp.reqId = req.reqId;

        if ((req.methodId == 1 || req.methodId == 3 || req.methodId == 4) && req.currency == null) {
            resp.status = 0;
            resp.message = "Invalid Currency Code.";
            return resp;
        }

        switch (req.methodId) {
            case 1: {// Open account
                nextAccNum++;
                accounts.put(nextAccNum, new Account(req.name, req.password, req.currency, req.amount));
                resp.status = 1; // Success
                resp.message = "Success: Account created. Account number: " + nextAccNum;
                break;
            }
            case 2: {// Close account
                if (!accounts.containsKey(req.accNum)) {
                    resp.status = 0;
                    resp.message = "Error: Invalid account number";
                    break;
                }
                Account acc = accounts.get(req.accNum);
                if (Objects.equals(acc.getName(), req.name) & Objects.equals(acc.getPassword(), req.password)) {
                    resp.status = 1; // Success
                    resp.message = "Account closed. Account number: " + req.accNum;
                } else {
                    resp.status = 0;
                    resp.message = "Error: Invalid credentials";
                }
                break;
            }
            case 3: {// Withdraw
                if (accounts.containsKey(req.accNum)) {
                    Account acc = accounts.get(req.accNum);
                    if (Objects.equals(acc.getName(), req.name) && Objects.equals(acc.getPassword(), req.password)
                            && Objects.equals(acc.getCurrency(), req.currency)) {
                        acc.balance -= req.amount;
                        resp.status = 1;
                        resp.balance = acc.balance;
                    }
                } else {
                    resp.status = 0;
                    resp.message = "Invalid Account ID";
                }
                break;
            }
            case 4: {// Deposit
                if (accounts.containsKey(req.accNum)) {
                    Account acc = accounts.get(req.accNum);
                    if (Objects.equals(acc.getName(), req.name) && Objects.equals(acc.getPassword(), req.password)
                            && Objects.equals(acc.getCurrency(), req.currency)) {
                        acc.balance += req.amount;
                        resp.status = 1;
                        resp.balance = acc.balance;
                    } else {
                        resp.status = 0;
                        resp.message = "Invalid Account ID";
                    }
                    break;
                }
            }
            case 5: // Monitor
                // Register this client for updates
                //MonitorManager.addClient(clientIP, clientPort, req.monitorInterval);
                resp.status = 1;
                resp.message = "Monitoring started for " + req.monitorInterval + "s";
                break;
        }
        return resp;
    }
}
