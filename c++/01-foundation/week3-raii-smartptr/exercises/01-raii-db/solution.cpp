#include <iostream>
#include <string>
#include <utility>

class DatabaseConnection {
    std::string dbName_;
    bool connected_;

public:
    explicit DatabaseConnection(const std::string& dbName)
        : dbName_(dbName), connected_(true) {
        std::cout << "[RAII] Database '" << dbName_ << "' opened\n";
    }

    ~DatabaseConnection() {
        if (connected_) {
            std::cout << "[RAII] Database '" << dbName_ << "' closed\n";
        }
    }

    DatabaseConnection(const DatabaseConnection&) = delete;
    DatabaseConnection& operator=(const DatabaseConnection&) = delete;

    DatabaseConnection(DatabaseConnection&& other) noexcept
        : dbName_(std::move(other.dbName_)),
          connected_(std::exchange(other.connected_, false)) {
    }

    DatabaseConnection& operator=(DatabaseConnection&& other) noexcept {
        if (this == &other) return *this;

        if (connected_) {
            std::cout << "[RAII] Database '" << dbName_ << "' closed\n";
        }

        dbName_ = std::move(other.dbName_);
        connected_ = std::exchange(other.connected_, false);
        return *this;
    }

    bool isConnected() const { return connected_; }
    const std::string& dbName() const { return dbName_; }

    void execute(const std::string& sql) {
        if (connected_) {
            std::cout << "[SQL] " << sql << '\n';
        }
    }
};

int main() {
    std::cout << "=== Test 1: Basic RAII ===\n";
    {
        DatabaseConnection db("mydb");
        std::cout << "  connected: " << db.isConnected() << " (expect 1)\n";
        db.execute("SELECT * FROM users");
    }
    std::cout << "  db should be closed now\n\n";

    std::cout << "=== Test 2: Move constructor ===\n";
    {
        DatabaseConnection db1("source_db");
        DatabaseConnection db2 = std::move(db1);
        std::cout << "  db1.connected: " << db1.isConnected() << " (expect 0)\n";
        std::cout << "  db2.connected: " << db2.isConnected() << " (expect 1)\n";
        std::cout << "  db2.name: " << db2.dbName() << " (expect source_db)\n";
    }
    std::cout << '\n';

    std::cout << "=== Test 3: Move assignment ===\n";
    {
        DatabaseConnection db1("first_db");
        DatabaseConnection db2("second_db");
        std::cout << "  Before move-assign:\n";
        db2 = std::move(db1);
        std::cout << "  db1.connected: " << db1.isConnected() << " (expect 0)\n";
        std::cout << "  db2.connected: " << db2.isConnected() << " (expect 1)\n";
        std::cout << "  db2.name: " << db2.dbName() << " (expect first_db)\n";
    }
    std::cout << '\n';

    std::cout << "=== Test 4: Execute after move ===\n";
    {
        DatabaseConnection db1("moved_db");
        DatabaseConnection db2 = std::move(db1);
        db2.execute("INSERT INTO logs VALUES (1, 'hello')");
    }
    std::cout << '\n';

    std::cout << "All tests done.\n";
    return 0;
}
