#pragma once

#include "appstorage.h"
#include <QMainWindow>
#include <QString>

class QComboBox;
class QLabel;
class QVBoxLayout;
class QListWidget;
class QPlainTextEdit;
class QPushButton;
class SkinManager;
class UserAvatar;
class QWidget;

class MainWindow final : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    void buildInterface();
    void rebuildSkinWorkspace();
    void buildMenus();
    void applySelectedSkin(int index);
    void addAccount();
    void refreshAccounts();
    void changeProfilePhoto();
    void selectAccount(int index);

    AppStorage m_storage;
    SkinManager *m_skinManager = nullptr;
    QComboBox *m_skinSelector = nullptr;
    QComboBox *m_accountSelector = nullptr;
    QListWidget *m_contacts = nullptr;
    QLabel *m_contactEmptyState = nullptr;
    QLabel *m_conversationTitle = nullptr;
    QLabel *m_transportState = nullptr;
    QLabel *m_storageState = nullptr;
    UserAvatar *m_profileAvatar = nullptr;
    QPlainTextEdit *m_composer = nullptr;
    QPushButton *m_sendButton = nullptr;
    QWidget *m_workspace = nullptr;
    QVBoxLayout *m_outerLayout = nullptr;
    QString m_currentSkinId = QStringLiteral("msn-75");
    QString m_selectedAccountId;
};
