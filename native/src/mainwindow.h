#pragma once

#include <QMainWindow>

class QComboBox;
class QLabel;
class QListWidget;
class QPlainTextEdit;
class QPushButton;
class SkinManager;

class MainWindow final : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    void buildInterface();
    void buildMenus();
    void applySelectedSkin(int index);

    SkinManager *m_skinManager = nullptr;
    QComboBox *m_skinSelector = nullptr;
    QListWidget *m_contacts = nullptr;
    QLabel *m_contactEmptyState = nullptr;
    QLabel *m_conversationTitle = nullptr;
    QLabel *m_transportState = nullptr;
    QPlainTextEdit *m_composer = nullptr;
    QPushButton *m_sendButton = nullptr;
};
