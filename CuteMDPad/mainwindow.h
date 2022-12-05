#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include "webviewcontent.h"
#include <QTextDocument>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    WebViewContent m_WebViewContent;
    void SetupEditor();
    void InitViews();
    bool LoadMDFile(const QString & filePath = ":/default.md");
    QTextDocument *doc;
    bool isModified() const;
    QString m_FilePath = ":/default.md";
public slots:
    void UpdateEditorToHtml();
    void UpdateEditorToHtmlEx();
private slots:
    void onFileNew();
    void onFileOpen();
    void onFileSave();
    void onFileSaveAs();
};
#endif // MAINWINDOW_H
