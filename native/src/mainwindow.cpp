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
#include <QIcon>
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

namespace {
QIcon iconForSkin(const QString &id)
{
    if (id == QStringLiteral("msn-75")) return QIcon(QStringLiteral(":/icons/msn-75.svg"));
    if (id == QStringLiteral("icq-2000b")) return QIcon(QStringLiteral(":/icons/icq-2000b.svg"));
    if (id == QStringLiteral("yahoo-messenger-9")) return QIcon(QStringLiteral(":/icons/yahoo-messenger.svg"));
    if (id == QStringLiteral("wlm-2009")) return QIcon(QStringLiteral(":/icons/wlm-2009.svg"));
    if (id == QStringLiteral("aim-7") || id == QStringLiteral("aim-classic")) return QIcon(QStringLiteral(":/icons/aim-7.svg"));
    return {};
}

QLabel *heading(const QString &text, QWidget *parent, const QString &objectName)
{
    auto *label = new QLabel(text, parent);
    label->setObjectName(objectName);
    return label;
}

QPushButton *disabledTool(const QString &text, QWidget *parent, const QString &objectName)
{
    auto *button = new QPushButton(text, parent);
    button->setObjectName(objectName);
    button->setEnabled(false);
    button->setToolTip(QStringLiteral("Pendiente de integrar con Messenger Core"));
    return button;
}
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_skinManager(new SkinManager(this))
{
    setMinimumSize(760, 520);
    const QSettings settings;
    m_currentSkinId = settings.value(QStringLiteral("appearance/skin"),
                                     QStringLiteral("msn-75")).toString();

    buildInterface();
    buildMenus();
    const int savedIndex = m_skinSelector->findData(m_currentSkinId);
    m_skinSelector->setCurrentIndex(savedIndex >= 0 ? savedIndex : 0);
    applySelectedSkin(m_skinSelector->currentIndex());

    connect(m_skinSelector, &QComboBox::currentIndexChanged,
            this, &MainWindow::applySelectedSkin);
}

void MainWindow::buildInterface()
{
    auto *central = new QWidget(this);
    auto *outer = new QVBoxLayout(central);
    outer->setContentsMargins(7, 7, 7, 7);
    outer->setSpacing(5);
    m_outerLayout = outer;

    auto *top = new QHBoxLayout;
    auto *appLabel = heading(QStringLiteral("MSN-WhatsApp-XP"), central,
                             QStringLiteral("applicationIdentity"));
    top->addWidget(appLabel);
    top->addStretch();

    m_skinSelector = new QComboBox(central);
    m_skinSelector->setObjectName(QStringLiteral("skinSelector"));
    for (const auto &skin : m_skinManager->availableSkins())
        m_skinSelector->addItem(iconForSkin(skin.id), skin.displayName, skin.id);
    auto *themeLabel = new QLabel(QStringLiteral("Tema:"), central);
    top->addWidget(themeLabel);
    top->addWidget(m_skinSelector);
    const bool classicWindow = m_currentSkinId == QStringLiteral("msn-75")
        || m_currentSkinId == QStringLiteral("icq-2000b");
    if (classicWindow) {
        appLabel->hide();
        themeLabel->hide();
        m_skinSelector->hide();
    }
    outer->addLayout(top);

    m_storageState = new QLabel(central);
    m_storageState->setObjectName(QStringLiteral("storageState"));
    outer->addWidget(m_storageState);

    setCentralWidget(central);
    rebuildSkinWorkspace();
    refreshAccounts();
    m_storageState->setText(
        QStringLiteral("%1 · %2 cuenta(s) local(es)")
            .arg(m_storage.storageDescription())
            .arg(m_storage.accounts().size()));
}

void MainWindow::rebuildSkinWorkspace()
{
    if (!m_outerLayout)
        return;

    if (m_workspace) {
        m_outerLayout->removeWidget(m_workspace);
        delete m_workspace;
        m_workspace = nullptr;
    }

    m_accountSelector = nullptr;
    m_contacts = nullptr;
    m_contactEmptyState = nullptr;
    m_conversationTitle = nullptr;
    m_transportState = nullptr;
    m_profileAvatar = nullptr;
    m_composer = nullptr;
    m_sendButton = nullptr;

    auto *splitter = new QSplitter(Qt::Horizontal, centralWidget());
    splitter->setObjectName(QStringLiteral("messengerWorkspace"));
    splitter->setChildrenCollapsible(false);

    const bool msn = m_currentSkinId == QStringLiteral("msn-75");
    const bool icq = m_currentSkinId == QStringLiteral("icq-2000b");

    auto *contactsPanel = new QFrame(splitter);
    contactsPanel->setObjectName(msn ? QStringLiteral("msnContactWindow")
                                    : icq ? QStringLiteral("icqContactWindow")
                                          : QStringLiteral("contactsPanel"));
    contactsPanel->setMinimumWidth(220);
    auto *contactsLayout = new QVBoxLayout(contactsPanel);
    contactsLayout->setContentsMargins(msn ? 7 : 6, msn ? 7 : 6, msn ? 7 : 6, msn ? 7 : 6);
    contactsLayout->setSpacing(msn ? 5 : 4);

    if (msn) {
        auto *profilePanel = new QFrame(contactsPanel);
        profilePanel->setObjectName(QStringLiteral("msnProfilePanel"));
        auto *profileRow = new QHBoxLayout(profilePanel);
        profileRow->setContentsMargins(5, 5, 5, 5);
        profileRow->setSpacing(7);
        m_profileAvatar = new UserAvatar(48, profilePanel);
        m_profileAvatar->setObjectName(QStringLiteral("msnProfileAvatar"));
        m_profileAvatar->setInitials(QStringLiteral("ME"));
        profileRow->addWidget(m_profileAvatar, 0, Qt::AlignTop);

        auto *identity = new QVBoxLayout;
        identity->setSpacing(2);
        identity->addWidget(heading(QStringLiteral("Mi nombre para mostrar"),
                                    profilePanel, QStringLiteral("msnDisplayName")));
        m_accountSelector = new QComboBox(profilePanel);
        m_accountSelector->setObjectName(QStringLiteral("accountSelector"));
        m_accountSelector->setMinimumWidth(125);
        identity->addWidget(m_accountSelector);
        auto *presence = heading(QStringLiteral("● Sin conexión"), profilePanel,
                                 QStringLiteral("msnPresence"));
        identity->addWidget(presence);
        identity->addWidget(heading(QStringLiteral("<Escribe un mensaje personal>"),
                                    profilePanel, QStringLiteral("msnPersonalMessage")));
        profileRow->addLayout(identity, 1);
        contactsLayout->addWidget(profilePanel);

        contactsLayout->addWidget(heading(QStringLiteral("Contactos"),
                                          contactsPanel, QStringLiteral("msnContactHeading")));
        contactsLayout->addWidget(heading(QStringLiteral("▼ Conectados"),
                                          contactsPanel, QStringLiteral("msnGroupHeading")));
        m_contacts = new QListWidget(contactsPanel);
        m_contacts->setObjectName(QStringLiteral("msnContactList"));
        m_contacts->setAccessibleName(QStringLiteral("Contactos de MSN Messenger"));
        m_contactEmptyState = heading(
            QStringLiteral("La lista se llenará cuando Messenger Core sincronice tus contactos."),
            contactsPanel, QStringLiteral("msnEmptyState"));
        m_contactEmptyState->setWordWrap(true);
        contactsLayout->addWidget(m_contactEmptyState);
        contactsLayout->addWidget(m_contacts, 1);
        contactsLayout->addWidget(heading(QStringLiteral("▼ No conectados"),
                                          contactsPanel, QStringLiteral("msnGroupHeading")));
        auto *addContact = disabledTool(QStringLiteral("Agregar un contacto..."),
                                         contactsPanel, QStringLiteral("msnAddContact"));
        contactsLayout->addWidget(addContact);

        auto *conversationPanel = new QFrame(splitter);
        conversationPanel->setObjectName(QStringLiteral("msnConversationWindow"));
        auto *conversationLayout = new QVBoxLayout(conversationPanel);
        conversationLayout->setContentsMargins(8, 8, 8, 8);
        conversationLayout->setSpacing(5);

        auto *conversationHeader = new QFrame(conversationPanel);
        conversationHeader->setObjectName(QStringLiteral("msnConversationHeader"));
        auto *headerLayout = new QHBoxLayout(conversationHeader);
        headerLayout->setContentsMargins(5, 4, 5, 4);
        auto *conversationAvatar = new UserAvatar(40, conversationHeader);
        conversationAvatar->setInitials(QStringLiteral("?"));
        conversationAvatar->setObjectName(QStringLiteral("msnConversationAvatar"));
        headerLayout->addWidget(conversationAvatar);
        m_conversationTitle = heading(QStringLiteral("Conversación"),
                                      conversationHeader, QStringLiteral("msnConversationTitle"));
        headerLayout->addWidget(m_conversationTitle, 1);
        headerLayout->addWidget(heading(QStringLiteral("Desconectado"),
                                        conversationHeader, QStringLiteral("msnConversationPresence")));
        conversationLayout->addWidget(conversationHeader);

        auto *history = new QFrame(conversationPanel);
        history->setObjectName(QStringLiteral("msnMessageHistory"));
        auto *historyLayout = new QVBoxLayout(history);
        historyLayout->addStretch();
        auto *empty = heading(QStringLiteral("Selecciona un contacto para iniciar una conversación"),
                              history, QStringLiteral("msnHistoryEmpty"));
        empty->setAlignment(Qt::AlignCenter);
        empty->setWordWrap(true);
        historyLayout->addWidget(empty);
        historyLayout->addStretch();
        conversationLayout->addWidget(history, 1);
        m_transportState = heading(QStringLiteral("Sin conexión · Messenger Core pendiente"),
                                   conversationPanel, QStringLiteral("msnTransportState"));
        conversationLayout->addWidget(m_transportState);

        auto *toolbar = new QHBoxLayout;
        toolbar->addWidget(disabledTool(QStringLiteral("Fuente"), conversationPanel, QStringLiteral("msnToolButton")));
        toolbar->addWidget(disabledTool(QStringLiteral("Emoticonos"), conversationPanel, QStringLiteral("msnToolButton")));
        toolbar->addWidget(disabledTool(QStringLiteral("Guiño"), conversationPanel, QStringLiteral("msnToolButton")));
        toolbar->addWidget(disabledTool(QStringLiteral("Zumbido"), conversationPanel, QStringLiteral("msnToolButton")));
        toolbar->addStretch();
        conversationLayout->addLayout(toolbar);
        m_composer = new QPlainTextEdit(conversationPanel);
        m_composer->setObjectName(QStringLiteral("msnMessageComposer"));
        m_composer->setPlaceholderText(QStringLiteral("Escribe un mensaje"));
        m_composer->setMaximumHeight(82);
        m_composer->setEnabled(false);
        conversationLayout->addWidget(m_composer);
        auto *sendRow = new QHBoxLayout;
        sendRow->addStretch();
        m_sendButton = disabledTool(QStringLiteral("Enviar"), conversationPanel,
                                    QStringLiteral("msnSendButton"));
        sendRow->addWidget(m_sendButton);
        conversationLayout->addLayout(sendRow);
        splitter->addWidget(contactsPanel);
        splitter->addWidget(conversationPanel);
        splitter->setStretchFactor(0, 1);
        splitter->setStretchFactor(1, 2);
    } else if (icq) {
        auto *identityPanel = new QFrame(contactsPanel);
        identityPanel->setObjectName(QStringLiteral("icqIdentityPanel"));
        auto *identityLayout = new QVBoxLayout(identityPanel);
        identityLayout->setContentsMargins(5, 4, 5, 4);
        identityLayout->setSpacing(3);
        auto *identityTop = new QHBoxLayout;
        m_profileAvatar = new UserAvatar(28, identityPanel);
        m_profileAvatar->setInitials(QStringLiteral("IC"));
        m_profileAvatar->setObjectName(QStringLiteral("icqFlowerAvatar"));
        identityTop->addWidget(m_profileAvatar);
        m_accountSelector = new QComboBox(identityPanel);
        m_accountSelector->setObjectName(QStringLiteral("accountSelector"));
        m_accountSelector->setMinimumWidth(100);
        identityTop->addWidget(m_accountSelector, 1);
        identityLayout->addLayout(identityTop);
        auto *uin = heading(QStringLiteral("ICQ#: pendiente"), identityPanel, QStringLiteral("icqUin"));
        identityLayout->addWidget(uin);
        contactsLayout->addWidget(identityPanel);

        auto *icqToolbar = new QHBoxLayout;
        icqToolbar->addWidget(disabledTool(QStringLiteral("Main"), contactsPanel, QStringLiteral("icqToolbarButton")));
        icqToolbar->addWidget(disabledTool(QStringLiteral("Services"), contactsPanel, QStringLiteral("icqToolbarButton")));
        contactsLayout->addLayout(icqToolbar);
        contactsLayout->addWidget(heading(QStringLiteral("My Contact List"),
                                          contactsPanel, QStringLiteral("icqContactHeading")));

        auto *contactArea = new QHBoxLayout;
        m_contacts = new QListWidget(contactsPanel);
        m_contacts->setObjectName(QStringLiteral("icqContactList"));
        m_contacts->setAccessibleName(QStringLiteral("ICQ 2000b contact list"));
        m_contactEmptyState = heading(
            QStringLiteral("Contacts will appear here after the contact-list service is connected."),
            contactsPanel, QStringLiteral("icqEmptyState"));
        m_contactEmptyState->setWordWrap(true);
        contactArea->addWidget(m_contactEmptyState, 1);
        contactArea->addWidget(m_contacts, 4);
        auto *quickTools = new QVBoxLayout;
        quickTools->addWidget(disabledTool(QStringLiteral("ICQuick"), contactsPanel,
                                           QStringLiteral("icqQuickButton")));
        quickTools->addWidget(disabledTool(QStringLiteral("Search"), contactsPanel,
                                           QStringLiteral("icqQuickButton")));
        quickTools->addStretch();
        contactArea->addLayout(quickTools);
        contactsLayout->addLayout(contactArea, 1);
        auto *statusLine = heading(QStringLiteral("● Offline"),
                                   contactsPanel, QStringLiteral("icqStatusLine"));
        contactsLayout->addWidget(statusLine);

        auto *conversationPanel = new QFrame(splitter);
        conversationPanel->setObjectName(QStringLiteral("icqMessageSession"));
        auto *conversationLayout = new QVBoxLayout(conversationPanel);
        conversationLayout->setContentsMargins(6, 6, 6, 6);
        conversationLayout->setSpacing(5);
        m_conversationTitle = heading(QStringLiteral("Message Session"),
                                      conversationPanel, QStringLiteral("icqSessionTitle"));
        conversationLayout->addWidget(m_conversationTitle);
        auto *history = new QFrame(conversationPanel);
        history->setObjectName(QStringLiteral("icqMessageHistory"));
        auto *historyLayout = new QVBoxLayout(history);
        historyLayout->addStretch();
        auto *empty = heading(QStringLiteral("Select a contact to open a message session."),
                              history, QStringLiteral("icqHistoryEmpty"));
        empty->setAlignment(Qt::AlignCenter);
        empty->setWordWrap(true);
        historyLayout->addWidget(empty);
        historyLayout->addStretch();
        conversationLayout->addWidget(history, 1);
        m_transportState = heading(QStringLiteral("Not connected · Messenger Core pending"),
                                   conversationPanel, QStringLiteral("icqTransportState"));
        conversationLayout->addWidget(m_transportState);
        m_composer = new QPlainTextEdit(conversationPanel);
        m_composer->setObjectName(QStringLiteral("icqMessageComposer"));
        m_composer->setPlaceholderText(QStringLiteral("Message"));
        m_composer->setMaximumHeight(82);
        m_composer->setEnabled(false);
        conversationLayout->addWidget(m_composer);
        auto *sendRow = new QHBoxLayout;
        sendRow->addStretch();
        m_sendButton = disabledTool(QStringLiteral("Send"), conversationPanel,
                                    QStringLiteral("icqSendButton"));
        sendRow->addWidget(m_sendButton);
        conversationLayout->addLayout(sendRow);
        splitter->addWidget(contactsPanel);
        splitter->addWidget(conversationPanel);
        splitter->setStretchFactor(0, 1);
        splitter->setStretchFactor(1, 2);
    } else {
        auto *profileRow = new QHBoxLayout;
        m_profileAvatar = new UserAvatar(48, contactsPanel);
        m_profileAvatar->setInitials(QStringLiteral("ME"));
        profileRow->addWidget(m_profileAvatar, 0, Qt::AlignTop);
        auto *profileText = new QVBoxLayout;
        profileText->addWidget(heading(QStringLiteral("Mi perfil"), contactsPanel,
                                       QStringLiteral("profileName")));
        m_accountSelector = new QComboBox(contactsPanel);
        m_accountSelector->setObjectName(QStringLiteral("accountSelector"));
        profileText->addWidget(m_accountSelector);
        profileText->addWidget(heading(QStringLiteral("Perfil local"), contactsPanel,
                                       QStringLiteral("profileHint")));
        profileRow->addLayout(profileText, 1);
        contactsLayout->addLayout(profileRow);
        contactsLayout->addWidget(heading(QStringLiteral("Contactos"), contactsPanel,
                                          QStringLiteral("sectionHeading")));
        m_contacts = new QListWidget(contactsPanel);
        m_contacts->setObjectName(QStringLiteral("contactList"));
        m_contactEmptyState = heading(
            QStringLiteral("Los contactos aparecerán al integrar Messenger Core."),
            contactsPanel, QStringLiteral("emptyState"));
        m_contactEmptyState->setWordWrap(true);
        contactsLayout->addWidget(m_contactEmptyState);
        contactsLayout->addWidget(m_contacts, 1);

        auto *conversationPanel = new QFrame(splitter);
        conversationPanel->setObjectName(QStringLiteral("conversationPanel"));
        auto *conversationLayout = new QVBoxLayout(conversationPanel);
        conversationLayout->setContentsMargins(12, 12, 12, 12);
        m_conversationTitle = heading(QStringLiteral("Conversación"), conversationPanel,
                                      QStringLiteral("conversationTitle"));
        conversationLayout->addWidget(m_conversationTitle);
        auto *history = new QFrame(conversationPanel);
        history->setObjectName(QStringLiteral("messageHistory"));
        auto *historyLayout = new QVBoxLayout(history);
        historyLayout->addStretch();
        auto *empty = heading(QStringLiteral("El historial se incorporará con Messenger Core."),
                              history, QStringLiteral("emptyTitle"));
        empty->setAlignment(Qt::AlignCenter);
        empty->setWordWrap(true);
        historyLayout->addWidget(empty);
        historyLayout->addStretch();
        conversationLayout->addWidget(history, 1);
        m_transportState = heading(QStringLiteral("Sin conexión · Núcleo y transporte pendientes"),
                                   conversationPanel, QStringLiteral("transportState"));
        conversationLayout->addWidget(m_transportState);
        m_composer = new QPlainTextEdit(conversationPanel);
        m_composer->setObjectName(QStringLiteral("messageComposer"));
        m_composer->setPlaceholderText(QStringLiteral("La redacción se habilitará al integrar un transporte."));
        m_composer->setMaximumHeight(90);
        m_composer->setEnabled(false);
        conversationLayout->addWidget(m_composer);
        auto *sendRow = new QHBoxLayout;
        sendRow->addStretch();
        m_sendButton = disabledTool(QStringLiteral("Enviar"), conversationPanel,
                                    QStringLiteral("sendButton"));
        sendRow->addWidget(m_sendButton);
        conversationLayout->addLayout(sendRow);
        splitter->addWidget(contactsPanel);
        splitter->addWidget(conversationPanel);
        splitter->setStretchFactor(0, 1);
        splitter->setStretchFactor(1, 3);
    }

    m_workspace = splitter;
    m_outerLayout->insertWidget(1, m_workspace, 1);
    if (m_accountSelector) {
        connect(m_accountSelector, &QComboBox::currentIndexChanged,
                this, &MainWindow::selectAccount);
    }
}

void MainWindow::buildMenus()
{
    menuBar()->clear();

    auto addAccountActions = [this](QMenu *menu) {
        auto *addAccountAction = menu->addAction(QStringLiteral("Agregar cuenta..."));
        connect(addAccountAction, &QAction::triggered, this, &MainWindow::addAccount);
        auto *changePhotoAction = menu->addAction(QStringLiteral("Cambiar foto de perfil..."));
        connect(changePhotoAction, &QAction::triggered, this, &MainWindow::changeProfilePhoto);
    };
    auto addSkinChoices = [this](QMenu *menu) {
        for (const auto &skin : m_skinManager->availableSkins()) {
            auto *choice = menu->addAction(skin.displayName);
            choice->setCheckable(true);
            choice->setChecked(skin.id == m_currentSkinId);
            connect(choice, &QAction::triggered, this, [this, id = skin.id] {
                const int index = m_skinSelector->findData(id);
                if (index >= 0)
                    m_skinSelector->setCurrentIndex(index);
            });
        }
    };
    auto addAppearanceAction = [this](QMenu *menu) {
        auto *reload = menu->addAction(QStringLiteral("Actualizar temas externos"));
        connect(reload, &QAction::triggered, this, [this] {
            const QString current = m_skinSelector->currentData().toString();
            m_skinManager->reload();
            m_skinSelector->clear();
            for (const auto &skin : m_skinManager->availableSkins())
                m_skinSelector->addItem(iconForSkin(skin.id), skin.displayName, skin.id);
            const int index = m_skinSelector->findData(current);
            m_skinSelector->setCurrentIndex(index >= 0 ? index : 0);
            applySelectedSkin(m_skinSelector->currentIndex());
        });
    };

    if (m_currentSkinId == QStringLiteral("icq-2000b")) {
        auto *mainMenu = menuBar()->addMenu(QStringLiteral("&Main"));
        addAccountActions(mainMenu);
        mainMenu->addAction(QStringLiteral("Preferences..."))->setEnabled(false);
        mainMenu->addSeparator();
        auto *folder = mainMenu->addAction(QStringLiteral("Open data folder"));
        connect(folder, &QAction::triggered, this, [this] {
            QDesktopServices::openUrl(QUrl::fromLocalFile(m_storage.dataRoot()));
        });
        mainMenu->addSeparator();
        auto *skinChoices = mainMenu->addMenu(QStringLiteral("Change Skin"));
        addSkinChoices(skinChoices);
        mainMenu->addSeparator();
        auto *quit = mainMenu->addAction(QStringLiteral("Exit"));
        connect(quit, &QAction::triggered, this, &QWidget::close);

        auto *services = menuBar()->addMenu(QStringLiteral("&Services"));
        services->addAction(QStringLiteral("Contact List"))->setEnabled(false);
        services->addAction(QStringLiteral("Find Users..."))->setEnabled(false);
        auto *skin = menuBar()->addMenu(QStringLiteral("Change &Skin"));
        addSkinChoices(skin);
        skin->addSeparator();
        addAppearanceAction(skin);
        auto *help = menuBar()->addMenu(QStringLiteral("&Help"));
        help->addAction(QStringLiteral("About ICQ 2000b"))->setEnabled(false);
    } else {
        auto *fileMenu = menuBar()->addMenu(QStringLiteral("&Archivo"));
        addAccountActions(fileMenu);
        auto *openDataAction = fileMenu->addAction(QStringLiteral("Abrir carpeta de datos"));
        connect(openDataAction, &QAction::triggered, this, [this] {
            QDesktopServices::openUrl(QUrl::fromLocalFile(m_storage.dataRoot()));
        });
        fileMenu->addSeparator();
        auto *quitAction = fileMenu->addAction(QStringLiteral("Salir"));
        connect(quitAction, &QAction::triggered, this, &QWidget::close);

        auto *contactsMenu = menuBar()->addMenu(QStringLiteral("&Contactos"));
        contactsMenu->addAction(QStringLiteral("Agregar un contacto..."))->setEnabled(false);
        auto *actionsMenu = menuBar()->addMenu(QStringLiteral("&Acciones"));
        actionsMenu->addAction(QStringLiteral("Enviar un mensaje instantáneo"))->setEnabled(false);
        auto *toolsMenu = menuBar()->addMenu(QStringLiteral("&Herramientas"));
        auto *skinChoices = toolsMenu->addMenu(QStringLiteral("Skin"));
        addSkinChoices(skinChoices);
        toolsMenu->addSeparator();
        addAppearanceAction(toolsMenu);
        auto *helpMenu = menuBar()->addMenu(QStringLiteral("A&yuda"));
        helpMenu->addAction(QStringLiteral("Acerca de MSN-WhatsApp-XP"))->setEnabled(false);
    }
}

void MainWindow::addAccount()
{
    bool accepted = false;
    const QString name = QInputDialog::getText(
        this, QStringLiteral("Agregar cuenta"),
        QStringLiteral("Nombre para identificar esta cuenta:"),
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
        QStringLiteral("%1 · %2 cuenta(s) local(es)")
            .arg(m_storage.storageDescription())
            .arg(m_storage.accounts().size()));
    statusBar()->showMessage(
        QStringLiteral("Perfil local «%1» creado. El inicio de sesión y transporte aún no están integrados.")
            .arg(account.displayName), 8000);
}

void MainWindow::applySelectedSkin(int index)
{
    if (index < 0 || !m_skinSelector)
        return;

    const QString skinId = m_skinSelector->itemData(index).toString();
    if (!m_skinManager->apply(skinId, qApp))
        return;

    m_currentSkinId = skinId;
    setWindowIcon(iconForSkin(skinId));
    if (skinId == QStringLiteral("msn-75"))
        setWindowTitle(QStringLiteral("MSN Messenger"));
    else if (skinId == QStringLiteral("icq-2000b"))
        setWindowTitle(QStringLiteral("ICQ"));
    else
        setWindowTitle(QStringLiteral("MSN-WhatsApp-XP"));

    rebuildSkinWorkspace();
    buildMenus();
    refreshAccounts();

    QSettings settings;
    settings.setValue(QStringLiteral("appearance/skin"), skinId);
}

void MainWindow::refreshAccounts()
{
    if (!m_accountSelector)
        return;

    const QString currentId = m_selectedAccountId;
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
    if (!m_profileAvatar || !m_accountSelector)
        return;

    if (index < 0 || index >= m_accountSelector->count()) {
        m_selectedAccountId.clear();
        m_profileAvatar->setPhoto(QString());
        m_profileAvatar->setInitials(m_currentSkinId == QStringLiteral("icq-2000b")
                                         ? QStringLiteral("IC") : QStringLiteral("ME"));
        return;
    }

    const QString accountId = m_accountSelector->itemData(index).toString();
    m_selectedAccountId = accountId;
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
    if (!m_accountSelector || m_accountSelector->currentIndex() < 0) {
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
