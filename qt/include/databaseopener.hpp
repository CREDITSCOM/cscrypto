#ifndef DATABASE_OPENER_HPP
#define DATABASE_OPENER_HPP

namespace cscrypto {
namespace gui {

class DatabaseOpener {
public:
    DatabaseOpener(const char* dbName);
    bool isOpened() {
        return dbOpened_;
    }

private:
    bool dbOpened_;
};

} // namespace gui
} // namespace cscrypto
#endif // DATABASE_OPENER_HPP