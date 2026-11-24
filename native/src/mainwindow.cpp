#include "mainwindow.h"
#include "skinmanager.h"
#include "useravatar.h"

#include <QAction>
#include <QComboBox>
#include <QDesktopServices>
#include <QFileInfo>
#include <QFileDialog>
#include <QFile>
#include <QImageReader>
#include <QLineEdit>
#include <QPixmap>
#include <QDir>
#include <QFrame>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QLabel>
#include <QListWidget>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QSettings>
#include <QSplitter>
#include <QStatusBar>
#include <QUrl>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_skinManager(new SkinManager(this))
{
    setWindowTitle(QStringLiteral("MSN-WhatsApp-XP"));
    resize(1050, 700);
    setMinimumSize(760, 520);

    buildInterface();
    buildMenus();

    const QSettings settings;
    const QString savedSkin = settings.value(QStringLiteral("appearance/skin"), QStringLiteral("msn-75")).toString();
    const int savedIndex = m_skinSelector->findData(savedSkin);
    m_skinSelector->setCurrentIndex(savedIndex >= 0 ? savedIndex : 0);
    applySelectedSkin(m_skinSelector->currentIndex());

    connect(m_skinSelector, &QComboBox::currentIndexChanged,
            this, &MainWindow::applySelectedSkin);
}

void MainWindow::buildInterface()
{
    auto *central = new QWidget(this);
    auto *outer = new QVBoxLayout(central);
    outer->setContentsMargins(8, 8, 8, 8);
    outer->setSpacing(6);

    auto *top = new QHBoxLayout;
    auto *brand = new QLabel(QStringLiteral("MSN-WhatsApp-XP"), central);
    brand->setObjectName(QStringLiteral("brandTitle"));
    top->addWidget(brand);
    top->addStretch();

    m_skinSelector = new QComboBox(central);
    m_skinSelector->setObjectName(QStringLiteral("skinSelector"));
    for (const auto &skin : m_skinManager->availableSkins())
        m_skinSelector->addItem(skin.displayName, skin.id);
    top->addWidget(new QLabel(QStringLiteral("Apariencia:"), central));
    top->addWidget(m_skinSelector);
    outer->addLayout(top);

    auto *splitter = new QSplitter(Qt::Horizontal, central);
    splitter->setChildrenCollapsible(false);

    auto *contactsPanel = new QFrame(splitter);
    contactsPanel->setObjectName(QStringLiteral("contactsPanel"));
    contactsPanel->setMinimumWidth(220);
    auto *contactsLayout = new QVBoxLayout(contactsPanel);
    contactsLayout->setContentsMargins(8, 8, 8, 8);

    auto *profileRow = new QHBoxLayout;
    m_profileAvatar = new UserAvatar(48, contactsPanel);
    m_profileAvatar->setInitials(QStringLiteral("ME"));
    profileRow->addWidget(m_profileAvatar, 0, Qt::AlignTop);
    auto *profileText = new QVBoxLayout;
    auto *profileName = new QLabel(QStringLiteral("Mi perfil"), contactsPanel);
    profileName->setObjectName(QStringLiteral("profileName"));
    auto *profileHint = new QLabel(QStringLiteral("Foto de usuario · MSN 7.5"), contactsPanel);
    profileHint->setObjectName(QStringLiteral("profileHint"));
    m_accountSelector = new QComboBox(contactsPanel);
    m_accountSelector->setObjectName(QStringLiteral("accountSelector"));
    m_accountSelector->setMinimumWidth(130);
    profileText->addWidget(profileName);
    profileText->addWidget(m_accountSelector);
    profileText->addWidget(profileHint);
    profileText->addStretch();
    profileRow->addLayout(profileText, 1);
    contactsLayout->addLayout(profileRow);

    auto *contactsHeading = new QLabel(QStringLiteral("Contactos"), contactsPanel);
    contactsHeading->setObjectName(QStringLiteral("sectionHeading"));
    contactsLayout->addWidget(contactsHeading);

    m_contacts = new QListWidget(contactsPanel);
    m_contacts->setObjectName(QStringLiteral("contactList"));
    m_contacts->setAccessibleName(QStringLiteral("Lista de contactos"));
    m_contactEmptyState = new QLabel(
        QStringLiteral("Los contactos aparecerán aquí con su foto de usuario, nombre y estado."),
        contactsPanel);
    m_contactEmptyState->setWordWrap(true);
    m_contactEmptyState->setObjectName(QStringLiteral("emptyState"));
    contactsLayout->addWidget(m_contactEmptyState);
    contactsLayout->addWidget(m_contacts, 1);

    auto *conversationPanel = new QFrame(splitter);
    conversationPanel->setObjectName(QStringLiteral("conversationPanel"));
    auto *conversationLayout = new QVBoxLayout(conversationPanel);
    conversationLayout->setContentsMargins(12, 12, 12, 12);

    auto *conversationHeader = new QHBoxLayout;
    auto *conversationAvatar = new UserAvatar(40, conversationPanel);
    conversationAvatar->setInitials(QStringLiteral("?"));
    conversationHeader->addWidget(conversationAvatar);
    m_conversationTitle = new QLabel(QStringLiteral("Conversación"), conversationPanel);
    m_conversationTitle->setObjectName(QStringLiteral("conversationTitle"));
    conversationHeader->addWidget(m_conversationTitle, 1);
    conversationLayout->addLayout(conversationHeader);

    auto *history = new QWidget(conversationPanel);
    history->setObjectName(QStringLiteral("messageHistory"));
    auto *historyLayout = new QVBoxLayout(history);
    historyLayout->addStretch();
    auto *emptyTitle = new QLabel(QStringLiteral("Tu espacio de conversación"), history);
    emptyTitle->setAlignment(Qt::AlignCenter);
    emptyTitle->setObjectName(QStringLiteral("emptyTitle"));
    auto *emptyDescription = new QLabel(
        QStringLiteral("La interfaz ya no depende de WhatsApp Web. El historial y el envío se incorporarán mediante un conector independiente."),
        history);
    emptyDescription->setWordWrap(true);
    emptyDescription->setAlignment(Qt::AlignCenter);
    emptyDescription->setObjectName(QStringLiteral("emptyState"));
    historyLayout->addWidget(emptyTitle);
    historyLayout->addWidget(emptyDescription);
    historyLayout->addStretch();
    conversationLayout->addWidget(history, 1);

    m_transportState = new QLabel(
        QStringLiteral("Sin conexión · Núcleo y transporte pendientes"),
        conversationPanel);
    m_transportState->setObjectName(QStringLiteral("transportState"));
    conversationLayout->addWidget(m_transportState);

    m_composer = new QPlainTextEdit(conversationPanel);
    m_composer->setObjectName(QStringLiteral("messageComposer"));
    m_composer->setPlaceholderText(QStringLiteral("La redacción se habilitará al integrar un transporte."));
    m_composer->setMaximumHeight(90);
    m_composer->setEnabled(false);
    conversationLayout->addWidget(m_composer);

    auto *sendRow = new QHBoxLayout;
    sendRow->addStretch();
    m_sendButton = new QPushButton(QStringLiteral("Enviar"), conversationPanel);
    m_sendButton->setEnabled(false);
    sendRow->addWidget(m_sendButton);
    conversationLayout->addLayout(sendRow);

    splitter->addWidget(contactsPanel);
    splitter->addWidget(conversationPanel);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 3);
    outer->addWidget(splitter, 1);

    m_storageState = new QLabel(
        QStringLiteral("%1 · %2 cuenta(s) guardada(s)")
            .arg(m_storage.storageDescription())
            .arg(m_storage.accounts().size()),
        central);
    m_storageState->setObjectName(QStringLiteral("storageState"));
    outer->addWidget(m_storageState);

    setCentralWidget(central);
    statusBar()->showMessage(QStringLiteral("Interfaz nativa · Sin transporte conectado"));
}

void MainWindow::buildMenus()
{
    auto *fileMenu = menuBar()->addMenu(QStringLiteral("&Archivo"));
    auto *addAccountAction = fileMenu->addAction(QStringLiteral("Agregar cuenta..."));
    connect(addAccountAction, &QAction::triggered, this, &MainWindow::addAccount);

    auto *changePhotoAction = fileMenu->addAction(QStringLiteral("Cambiar foto de perfil..."));
    connect(changePhotoAction, &QAction::triggered, this, &MainWindow::changeProfilePhoto);

    auto *openDataAction = fileMenu->addAction(QStringLiteral("Abrir carpeta de datos"));
    connect(openDataAction, &QAction::triggered, this, [this] {
        QDesktopServices::openUrl(QUrl::fromLocalFile(m_storage.dataRoot()));
    });

    fileMenu->addSeparator();
    auto *quitAction = fileMenu->addAction(QStringLiteral("Salir"));
    connect(quitAction, &QAction::triggered, this, &QWidget::close);

    auto *viewMenu = menuBar()->addMenu(QStringLiteral("&Apariencia"));
    auto *reloadSkinsAction = viewMenu->addAction(QStringLiteral("Actualizar skins externos"));
    connect(reloadSkinsAction, &QAction::triggered, this, [this] {
        const QString current = m_skinSelector->currentData().toString();
        m_skinManager->reload();
        m_skinSelector->clear();
        for (const auto &skin : m_skinManager->availableSkins())
            m_skinSelector->addItem(skin.displayName, skin.id);
        const int index = m_skinSelector->findData(current);
        m_skinSelector->setCurrentIndex(index >= 0 ? index : 0);
        applySelectedSkin(m_skinSelector->currentIndex());
    });

    auto *helpMenu = menuBar()->addMenu(QStringLiteral("A&yuda"));
    auto *aboutAction = helpMenu->addAction(QStringLiteral("Acerca de MSN-WhatsApp-XP"));
    connect(aboutAction, &QAction::triggered, this, [this] {
        statusBar()->showMessage(
            QStringLiteral("Base nativa en desarrollo. Las cuentas se registran localmente; el inicio de sesión y transporte aún no existen."),
            8000);
    });
}

void MainWindow::addAccount()
{
    bool accepted = false;
    const QString name = QInputDialog::getText(
        this, QStringLiteral("Agregar cuenta"),
        QStringLiteral("Nombre para identificar esta cuenta de WhatsApp:"),
        QLineEdit::Normal, {}, &accepted);
    if (!accepted)
        return;

    StoredAccount account;
    QString error;
    if (!m_storage.addAccount(name, &account, &error)) {
        QMessageBox::warning(this, QStringLiteral("No se pudo guardar la cuenta"), error);
        return;
    }

    refreshAccounts();
    m_storageState->setText(
        QStringLiteral("%1 · %2 cuenta(s) guardada(s)")
            .arg(m_storage.storageDescription())
            .arg(m_storage.accounts().size()));
    statusBar()->showMessage(
        QStringLiteral("Perfil local «%1» creado. El inicio de sesión de WhatsApp aún no está integrado.")
            .arg(account.displayName), 8000);
}

void MainWindow::applySelectedSkin(int index)
{
    if (index < 0)
        return;

    const QString skinId = m_skinSelector->itemData(index).toString();
    if (!m_skinManager->apply(skinId, qApp))
        return;

    QSettings settings;
    settings.setValue(QStringLiteral("appearance/skin"), skinId);
}

void MainWindow::refreshAccounts()
{
    const QString currentId = m_accountSelector->currentData().toString();
    const QVector<StoredAccount> accounts = m_storage.accounts();
    m_accountSelector->blockSignals(true);
    m_accountSelector->clear();
    for (const StoredAccount &account : accounts)
        m_accountSelector->addItem(account.displayName, account.id);
    int index = m_accountSelector->findData(currentId);
    if (index < 0 && m_accountSelector->count() > 0)
        index = 0;
    m_accountSelector->setCurrentIndex(index);
    m_accountSelector->blockSignals(false);
    selectAccount(index);
}

void MainWindow::selectAccount(int index)
{
    if (index < 0 || index >= m_accountSelector->count()) {
        m_profileAvatar->setPhoto(QString());
        m_profileAvatar->setInitials(QStringLiteral("ME"));
        return;
    }

    const QString accountId = m_accountSelector->itemData(index).toString();
    for (const StoredAccount &account : m_storage.accounts()) {
        if (account.id != accountId)
            continue;
        const QString avatarPath = QDir(account.directory).filePath(QStringLiteral("profile/avatar.png"));
        m_profileAvatar->setPhoto(avatarPath);
        if (!QFileInfo::exists(avatarPath))
            m_profileAvatar->setInitials(account.displayName.left(2));
        return;
    }
}

void MainWindow::changeProfilePhoto()
{
    const int index = m_accountSelector->currentIndex();
    if (index < 0) {
        QMessageBox::information(this, QStringLiteral("Foto de perfil"),
                                 QStringLiteral("Primero agrega una cuenta local."));
        return;
    }

    const QString source = QFileDialog::getOpenFileName(
        this, QStringLiteral("Elegir foto de perfil"), {},
        QStringLiteral("Imágenes (*.png *.jpg *.jpeg *.bmp *.webp)"));
    if (source.isEmpty())
        return;

    QImageReader reader(source);
    const QImage image = reader.read();
    if (image.isNull()) {
        QMessageBox::warning(this, QStringLiteral("Foto no válida"), reader.errorString());
        return;
    }

    const QString accountId = m_accountSelector->currentData().toString();
    for (const StoredAccount &account : m_storage.accounts()) {
        if (account.id != accountId)
            continue;
        const QString destination = QDir(account.directory).filePath(QStringLiteral("profile/avatar.png"));
        if (!QDir().mkpath(QFileInfo(destination).absolutePath()) || !image.save(destination, "PNG")) {
            QMessageBox::warning(this, QStringLiteral("No se pudo guardar"),
                                 QStringLiteral("No fue posible guardar la foto en el perfil de esta cuenta."));
            return;
        }
        m_profileAvatar->setPhoto(destination);
        statusBar()->showMessage(QStringLiteral("Foto de perfil guardada para «%1».").arg(account.displayName), 6000);
        return;
    }
}
