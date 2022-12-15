#include "mainwindow.h"
#include "Qsci/qsciapis.h"
#include "Qsci/qscilexermarkdown.h"
#include "ui_mainwindow.h"
#include <Qsci/qsciscintilla.h>
#include <QWebChannel>
#include <QFile>
#include <QFileDialog>
#include <QWebEnginePage>
#include "QDebug"
#include <QTextDocument>
#include <QFile>
#include <QFileDialog>
#include <QFontDatabase>
#include <QMessageBox>
#include <iostream>
#include <map>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    InitEditor();
    InitViews();
    InitConnections();
    InitEditMenu();
    LoadMDFile();//Load Default File;


}

MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief MainWindow::InitEditor
 */
void MainWindow::InitEditor()
{
    QsciLexerMarkdown *mdLexer = new QsciLexerMarkdown;

    //Setup appearance
    //to do :provide setting dialog and save to file
    QFont font("Microsoft Yahei", 10, QFont::Normal);
    ui->Editor->setFont(font);
    ui->Editor->setMarginsFont(font);
    ui->Editor->setLexer(mdLexer);
    ui->Editor->setMarginWidth(0,30);
    ui->Editor->setMarginLineNumbers(0, true);
    ui->Editor->setBraceMatching(QsciScintilla::SloppyBraceMatch);
    ui->Editor->setTabWidth(4);
    ui->Editor->setMarginType(1, QsciScintilla::SymbolMargin);
    QsciAPIs *apis = new QsciAPIs(mdLexer);

    apis->prepare();


}


void MainWindow::InitEditMenu()
{
    QMenu *menu;
    QAction *act;

    m_EditMenu = ui->menubar->addMenu(tr("&Edit"));

    act = new QAction(tr("&Copy"), this);
    act->setShortcut(QKeySequence::StandardKey::Copy);
    connect(act, &QAction::triggered, ui->Editor,&QsciScintilla::copy);
    connect(ui->Editor,SIGNAL(copyAvailable(bool)), act, SLOT(setEnabled(bool)));
    m_EditMenu->addAction(act);

    act = new QAction(tr("Cu&t"), this);
    act->setShortcut(QKeySequence::StandardKey::Cut);
    connect(act, &QAction::triggered, ui->Editor,&QsciScintilla::cut);
    connect(ui->Editor,SIGNAL(copyAvailable(bool)), act, SLOT(setEnabled(bool)));
    m_EditMenu->addAction(act);

    act = new QAction(tr("&Paste"), this);
    act->setShortcut(QKeySequence::StandardKey::Paste);
    connect(act, &QAction::triggered, ui->Editor,&QsciScintilla::paste);
    m_EditMenu->addAction(act);

    m_EditMenu->addSeparator();

    m_MenuUndoAction = new QAction(tr("Undo"), this);
    m_MenuUndoAction->setShortcut(QKeySequence::StandardKey::Undo);
    connect(m_MenuUndoAction, &QAction::triggered, ui->Editor,&QsciScintilla::undo);
    m_MenuUndoAction->setEnabled(false);
    m_EditMenu->addAction(m_MenuUndoAction);

    m_MenuRedoAction = new QAction(tr("Redo"), this);
    m_MenuRedoAction->setShortcut(QKeySequence::StandardKey::Redo);
    connect(m_MenuRedoAction, &QAction::triggered, ui->Editor,&QsciScintilla::redo);
    m_MenuRedoAction->setEnabled(false);
    m_EditMenu->addAction(m_MenuRedoAction);

    connect(ui->Editor,SIGNAL(textChanged()), this, SLOT(UpdateMenuUndoRedoEnable()));

    m_EditMenu->addSeparator();


    menu = m_EditMenu->addMenu("Insert");

    menu = menu->addMenu("Heading");

    act = new QAction(("Heading 1"), this);
    connect(act,&QAction::triggered,this,&MainWindow::onInsertActionTriggered);
    menu->addAction(act);

    act = new QAction(("Heading 2"), this);
    connect(act,&QAction::triggered,this,&MainWindow::onInsertActionTriggered);
    menu->addAction(act);

    act = new QAction(("Heading 3"), this);
    connect(act,&QAction::triggered,this,&MainWindow::onInsertActionTriggered);
    menu->addAction(act);

    act = new QAction(("Heading 4"), this);
    connect(act,&QAction::triggered,this,&MainWindow::onInsertActionTriggered);
    menu->addAction(act);

    act = new QAction(("Heading 5"), this);
    connect(act,&QAction::triggered,this,&MainWindow::onInsertActionTriggered);
    menu->addAction(act);

    menu =(QMenu*) menu->parent();

    act = new QAction(("Bold"), this);
    connect(act,&QAction::triggered,this,&MainWindow::onInsertActionTriggered);
    menu->addAction(act);

    act = new QAction(("Italic"), this);
    connect(act,&QAction::triggered,this,&MainWindow::onInsertActionTriggered);
    menu->addAction(act);

    act = new QAction(("Block quote"), this);
    connect(act,&QAction::triggered,this,&MainWindow::onInsertActionTriggered);
    menu->addAction(act);

    act = new QAction(("Ordered list"), this);
    connect(act,&QAction::triggered,this,&MainWindow::onInsertActionTriggered);
    menu->addAction(act);

    act = new QAction(("Unordered list"), this);
    connect(act,&QAction::triggered,this,&MainWindow::onInsertActionTriggered);
    menu->addAction(act);

    act = new QAction(("Image"), this);
    connect(act,&QAction::triggered,this,&MainWindow::onInsertActionTriggered);
    menu->addAction(act);

    act = new QAction(("Link"), this);
    connect(act,&QAction::triggered,this,&MainWindow::onInsertActionTriggered);
    menu->addAction(act);

    act = new QAction(("Horizontal rule"), this);
    connect(act,&QAction::triggered,this,&MainWindow::onInsertActionTriggered);
    menu->addAction(act);

    //Editor Context Menu
    ui->Editor->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->Editor, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(ShowContextMenu(const QPoint &)));
}

void MainWindow::UpdateMenuUndoRedoEnable()
{
    qDebug()<<"triggerd"<<Qt::endl;
    m_MenuRedoAction->setEnabled(ui->Editor->isRedoAvailable());
    m_MenuUndoAction->setEnabled(ui->Editor->isUndoAvailable());
}
/**
 * @brief MainWindow::InitConnections
 *
 */
void MainWindow::InitConnections()
{
    m_MDCommons = * new MDCommons();
    //Text change triggers preview update.
    connect(ui->Editor,&QsciScintilla::textChanged,[this]() {this->m_WebViewContent.SetText(ui->Editor->text());});
    //Modified document leads to title content change.
    connect(ui->Editor,&QsciScintilla::modificationChanged,[this]()
    {
        QString Title = "CuteMDPad";
        if(this->m_FilePath.isEmpty()) Title.append("*");
        else Title.append("("+m_FilePath+(ui->Editor->isModified()?"*":"")+")");
        this->setWindowTitle(Title);
    });

    //Connect actions to function
    //Pt.1 Common file function action
    connect(ui->actionNew_File, &QAction::triggered, this, &MainWindow::onFileNew);
    connect(ui->actionOpen_File, &QAction::triggered, this, &MainWindow::onFileOpen);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::onFileSave);
    connect(ui->actionSave_As, &QAction::triggered, this, &MainWindow::onFileSaveAs);


}

void MainWindow::ShowContextMenu(const QPoint &pos)
{
    m_EditMenu->exec(mapToGlobal(pos));
}
/**
 * @brief MainWindow::InitViews
 */
void MainWindow::InitViews()
{
    QWebChannel *contentChanel = new QWebChannel(this);
    contentChanel->registerObject(QStringLiteral("content"), &m_WebViewContent);
    QWebEnginePage *page = new QWebEnginePage();
    page->setWebChannel(contentChanel);
    ui->Preview->setPage(page);
    ui->Preview->setUrl(QUrl("qrc:/index.html"));

    //keep Spliter size 1:1
    QList<int> sizes;
    sizes << 50000 << 50000;
    ui->splitter->setSizes(sizes);
}
bool MainWindow::LoadMDFile(const QString & filePath)
{
    QFile textFile(filePath);
    bool ret = textFile.open(QIODevice::ReadOnly);
    this->m_FilePath = filePath;
    if(ret)
    {
        ui->Editor->setText(textFile.readAll());
    }
    ui->Editor->setModified(false);
    ui->Editor->setCursorPosition(0,0);
    this->m_MenuUndoAction->setEnabled(false);
    return ret;
}


/**
 * @brief MainWindow::UpdateEditorToHtml
 * Use QTextDocument built-in function to convert MD->HTML
 */
void MainWindow::UpdateEditorToHtml()
{

    QFile css(":/markdown.css");
    css.open(QIODevice::ReadOnly);
    QTextDocument *doc = new QTextDocument();
    doc->setDefaultStyleSheet(css.readAll());
    doc->setMarkdown(ui->Editor->text());
    qDebug()<<"before::"<<Qt::endl<<doc->toHtml()<<Qt::endl;
    doc->setHtml(doc->toHtml());
    qDebug()<<"after::"<<Qt::endl<<doc->toHtml()<<Qt::endl;
    auto html = doc->toHtml();
    qDebug()<<"OUT PUT HTML:"<<Qt::endl<<html<<Qt::endl;
    m_WebViewContent.SetText(html);
}

bool MainWindow::isModified() const
{
    return ui->Editor->isModified();
}

void MainWindow::onFileNew()
{
    if (isModified()) {
        QMessageBox::StandardButton button = QMessageBox::question(this, windowTitle(),
                             tr("You have unsaved changes. Do you want to create a new document anyway?"));
        if (button != QMessageBox::Yes)
            return;
    }

    m_FilePath.clear();
    ui->Editor->setText(tr("## New document"));
    ui->Editor->setModified(false);
}
void MainWindow::onFileOpen()
{
    if (isModified()) {
        QMessageBox::StandardButton button = QMessageBox::question(this, windowTitle(),
                             tr("You have unsaved changes. Do you want to open a new document anyway?"));
        if (button != QMessageBox::Yes)
            return;
    }

    QFileDialog dialog(this, tr("Open MarkDown File"));
    dialog.setMimeTypeFilters({"text/markdown"});
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    if (dialog.exec() == QDialog::Accepted)
        LoadMDFile(dialog.selectedFiles().constFirst());
}



void MainWindow::onFileSave()
{
    if (m_FilePath.isEmpty()) {
        onFileSaveAs();
        return;
    }

    QFile f(m_FilePath);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text))  {
        QMessageBox::warning(this, windowTitle(),
                             tr("Could not write to file %1: %2").arg(
                                 QDir::toNativeSeparators(m_FilePath), f.errorString()));
        return;
    }
    QTextStream str(&f);
    str << ui->Editor->text();

    ui->Editor->setModified(false);
}

void MainWindow::onFileSaveAs()
{
    QFileDialog dialog(this, tr("Save MarkDown File"));
    dialog.setMimeTypeFilters({"text/markdown"});
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setDefaultSuffix("md");
    if (dialog.exec() != QDialog::Accepted)
        return;

    m_FilePath = dialog.selectedFiles().constFirst();
    onFileSave();
}

void MainWindow::onInsertActionTriggered()
{
    if(QAction * act = dynamic_cast<QAction*>(sender()))
    {
        int line = 0;
        int index = 0;
        MDSyntax syntax =m_MDCommons.m_MDSyntaxDict.find(act->text()).value();
        qDebug()<<act->text()<<Qt::endl;
        ui->Editor->getCursorPosition(&line,&index);
        if(index>0)
        {
            line+=1;
            index=0;
        }
        ui->Editor->insertAt(syntax.m_SyntaxText,line,index);
        ui->Editor->setCursorPosition(line+syntax.m_LineOffset,index+syntax.m_CursorOffset);
    }
}

