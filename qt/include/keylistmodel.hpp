#ifndef KEY_LIST_MODEL_HPP
#define KEY_LIST_MODEL_HPP

#include <QAbstractListModel>

#include <common.hpp>

namespace cscrypto {
namespace gui {

class KeyListModel : public QAbstractListModel {
    Q_OBJECT

public:
    explicit KeyListModel(QObject* parent = nullptr);

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex& parent) const override;

    void addKeyPair(const KeyPair& keys);
    KeyPair getKeyPair(const QModelIndex& index);

private:
    std::vector<KeyPair> keys_;
};

} // namespace gui
} // namespace cscrypto
#endif // KEY_LIST_MODEL_HPP