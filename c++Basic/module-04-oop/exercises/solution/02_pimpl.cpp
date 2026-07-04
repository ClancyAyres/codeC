// 模块 3 习题答案: Pimpl 模式
// 规范: Core Guidelines P.8, C.131

#include <iostream>
#include <memory>
#include <string>
#include <map>
#include <utility>

class ConfigManager {
public:
    ConfigManager();
    ~ConfigManager();

    ConfigManager(ConfigManager&&) noexcept;
    ConfigManager& operator=(ConfigManager&&) noexcept;

    ConfigManager(const ConfigManager&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;

    std::string get(const std::string& key) const;
    void set(const std::string& key, const std::string& value);

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

struct ConfigManager::Impl {
    std::map<std::string, std::string> data_;
};

ConfigManager::ConfigManager() : impl_(std::make_unique<Impl>()) {}
ConfigManager::~ConfigManager() = default;
ConfigManager::ConfigManager(ConfigManager&&) noexcept = default;
ConfigManager& ConfigManager::operator=(ConfigManager&&) noexcept = default;

std::string ConfigManager::get(const std::string& key) const {
    auto it = impl_->data_.find(key);
    if (it != impl_->data_.end()) {
        return it->second;
    }
    return {};
}

void ConfigManager::set(const std::string& key, const std::string& value) {
    impl_->data_[key] = value;
}

int main() {
    ConfigManager cfg;
    cfg.set("host", "localhost");
    cfg.set("port", "8080");

    std::cout << "host: " << cfg.get("host") << '\n';
    std::cout << "port: " << cfg.get("port") << '\n';
    std::cout << "missing: '" << cfg.get("missing") << "'\n";

    ConfigManager cfg2 = std::move(cfg);
    std::cout << "moved host: " << cfg2.get("host") << '\n';

    return 0;
}
