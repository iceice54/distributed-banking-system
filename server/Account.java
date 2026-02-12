public class Account {
    String name;
    String password;
    Currency currency;
    double balance;

    public String getName() {
        return name;
    }

    public double getBalance() {
        return balance;
    }

    public Currency getCurrency() {
        return currency;
    }

    public String getPassword() {
        return password;
    }

    public Account(String name, String password, Currency currency, double balance) {
        this.name = name;
        this.password = password;
        this.currency = currency;
        this.balance = balance;
    }
}
