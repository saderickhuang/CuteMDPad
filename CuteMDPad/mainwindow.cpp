#include "mainwindow.h"
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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    SetupEditor();
    InitViews();
    LoadMDFile();//Load Default File;

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SetupEditor()
{
    //Setup appearance
    //to do :provide setting dialog and save to file
    QFont font("Microsoft Yahei", 10, QFont::Normal);
    ui->Editor->setFont(font);
    ui->Editor->setMarginsFont(font);
    ui->Editor->setLexer(new QsciLexerMarkdown(this));
    ui->Editor->setMarginWidth(0,10);
    ui->Editor->setMarginLineNumbers(0, true);
    ui->Editor->setBraceMatching(QsciScintilla::SloppyBraceMatch);
    ui->Editor->setTabWidth(4);
    ui->Editor->setMarginType(1, QsciScintilla::SymbolMargin);
    //ui->Editor->setMarginWidth(1, "00000");
    //Setup signals
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
    return ret;
}
void MainWindow::InitViews()
{
    QWebChannel *contentChanel = new QWebChannel(this);
    contentChanel->registerObject(QStringLiteral("content"), &m_WebViewContent);
    QWebEnginePage *page = new QWebEnginePage();
    page->setWebChannel(contentChanel);
    ui->Preview->setPage(page);
    ui->Preview->setUrl(QUrl("qrc:/index.html"));
    //connect(ui->Editor,&QsciScintilla::textChanged,this,&MainWindow::UpdateEditorToHtml);
    connect(ui->Editor,&QsciScintilla::textChanged,[this]() {this->m_WebViewContent.SetText(ui->Editor->text());});
    connect(ui->Editor,&QsciScintilla::modificationChanged,[this]()
    {
        QString Title = "CuteMDPad";
        if(this->m_FilePath.isEmpty()) Title.append("*");
        else Title.append("("+m_FilePath+(ui->Editor->isModified()?"*":"")+")");
        this->setWindowTitle(Title);
    });
    //keep Spliter size 1:1
    QList<int> sizes;
    sizes << 50000 << 50000;
    ui->splitter->setSizes(sizes);

    connect(ui->actionNew_File, &QAction::triggered, this, &MainWindow::onFileNew);
    connect(ui->actionOpen_File, &QAction::triggered, this, &MainWindow::onFileOpen);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::onFileSave);
    connect(ui->actionSave_As, &QAction::triggered, this, &MainWindow::onFileSaveAs);
}
/**
 * @brief MainWindow::UpdateEditorToHtml
 * Use QTextDocument built-in function to convert MD->HTML
 */
void MainWindow::UpdateEditorToHtml()
{

    QFile css(":/markdown2.css");
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

void MainWindow::UpdateEditorToHtmlEx()
{

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
