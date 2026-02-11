public class BankRequest {
    public int methodId;
    public int reqId;

    // Optional fields (some might be null/0 depending on the method)
    public String name;
    public String password;
    public String currency;
    public int accNum;
    public double amount;
    public int monitorInterval;

    // Constructor for easy setup
    public BankRequest() {}
}