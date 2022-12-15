#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include "mdcommons.h"
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
    /*Member function declare*/
    void InitEditor();
    void InitViews();
    void InitConnections();
    void InitEditMenu();
    bool LoadMDFile(const QString & filePath = ":/default.md");
    bool isModified() const;

    /*Member var declare*/
    QTextDocument *doc;
    WebViewContent m_WebViewContent;
    QString m_FilePath = ":/default.md";
    MDCommons m_MDCommons;
    QMenu* m_EditMenu;
    QAction* m_MenuUndoAction;
    QAction* m_MenuRedoAction;
public slots:
    void UpdateEditorToHtml();

private slots:
    void onFileNew();
    void onFileOpen();
    void onFileSave();
    void onFileSaveAs();
    void onInsertActionTriggered();
    void ShowContextMenu(const QPoint &pos);
    void UpdateMenuUndoRedoEnable();
};
#endif // MAINWINDOW_H
