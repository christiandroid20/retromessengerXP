#include "mainwindow.h"
#include "skinmanager.h"

#include <QAction>
#include <QComboBox>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QMenu>
#include <QMenuBar>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QSettings>
#include <QSplitter>
#include <QStatusBar>
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
    auto *contactsHeading = new QLabel(QStringLiteral("Contactos"), contactsPanel);
    contactsHeading->setObjectName(QStringLiteral("sectionHeading"));
    contactsLayout->addWidget(contactsHeading);

    m_contacts = new QListWidget(contactsPanel);
    m_contacts->setObjectName(QStringLiteral("contactList"));
    m_contacts->setAccessibleName(QStringLiteral("Lista de contactos"));
    m_contactEmptyState = new QLabel(
        QStringLiteral("Los contactos aparecerán cuando conectemos un transporte de mensajería."),
        contactsPanel);
    m_contactEmptyState->setWordWrap(true);
    m_contactEmptyState->setObjectName(QStringLiteral("emptyState"));
    contactsLayout->addWidget(m_contactEmptyState);
    contactsLayout->addWidget(m_contacts, 1);

    auto *conversationPanel = new QFrame(splitter);
    conversationPanel->setObjectName(QStringLiteral("conversationPanel"));
    auto *conversationLayout = new QVBoxLayout(conversationPanel);
    conversationLayout->setContentsMargins(12, 12, 12, 12);

    m_conversationTitle = new QLabel(QStringLiteral("Conversación"), conversationPanel);
    m_conversationTitle->setObjectName(QStringLiteral("conversationTitle"));
    conversationLayout->addWidget(m_conversationTitle);

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

    setCentralWidget(central);
    statusBar()->showMessage(QStringLiteral("Interfaz nativa · Sin transporte conectado"));
}

void MainWindow::buildMenus()
{
    auto *fileMenu = menuBar()->addMenu(QStringLiteral("&Archivo"));
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
            QStringLiteral("Versión nativa en reconstrucción. Aún no hay cuentas ni transporte conectados."),
            8000);
    });
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
