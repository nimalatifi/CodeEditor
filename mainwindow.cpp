#include <QtWidgets>
#include <QPlainTextEdit>
#include "mainwindow.h"
#include "highlighter.h"
#include <QTextCharFormat>
#include <QVector>
#include <vector>
#include <algorithm>
#include <iostream>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupFileMenu();
    setupHelpMenu();
    setupEditor();
    setCentralWidget(editor);
    connect(editor, SIGNAL(textChanged()), this, SLOT(myTextChanged()));
    connect(editor, SIGNAL(cursorPositionChanged()), this, SLOT(myCursorChanged()));
    setWindowTitle(tr("Syntax Highlighter"));
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Syntax Highlighter"),
                       tr("<p>The <b>Syntax Highlighter</b> example shows how " \
                   "to perform simple syntax highlighting by subclassing " \
                   "the QSyntaxHighlighter class and describing " \
                   "highlighting rules using regular expressions.</p>"));
}



void MainWindow::newFile()
{
    editor->clear();
}

void MainWindow::openFile(const QString &path)
{
    QString fileName = path;
    if (fileName.isNull())
        fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", "C++ Files (*.cpp *.h *.txt)");
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QFile::ReadOnly | QFile::Text))
        {
            oldState = editor->blockSignals(true);
            editor->setPlainText(file.readAll());
            textLenght=editor->toPlainText().length();
            editor->blockSignals(oldState);
        }
    }
}

void MainWindow::setupEditor()
{
    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(10);
    editor = new QPlainTextEdit;
    editor->setFont(font);

    // added by nima
    //QColor color = QColorDialog::getColor(Qt::gray,this); // in here your color pallete will open..
    QColor color =QColor(240, 240, 240);
    QPalette p = editor->palette();
    p.setColor(QPalette::Base, color);
    editor->setPalette(p);
    rExp.setPatternSyntax(QRegExp::RegExp);
    rExp.setPattern("\\{|\\}|\\(|\\)|\\[|\\]");

    //end added by nima

    highlighter = new Highlighter(editor->document());
    QFile file("mainwindow.h");
    if (file.open(QFile::ReadOnly | QFile::Text))
        editor->setPlainText(file.readAll());
    textLenght=editor->toPlainText().length();
}

void MainWindow::setupFileMenu()
{
    QMenu *fileMenu = new QMenu(tr("&File"), this);
    menuBar()->addMenu(fileMenu);
    fileMenu->addAction(tr("&New"), this,
                        &MainWindow::newFile, QKeySequence::New);
    fileMenu->addAction(tr("&Open..."),
                        this, [this](){ openFile(); }, QKeySequence::Open);
    fileMenu->addAction(tr("E&xit"), qApp,
                        &QApplication::quit, QKeySequence::Quit);
}

void MainWindow::setupHelpMenu()
{
    QMenu *helpMenu = new QMenu(tr("&Help"), this);
    menuBar()->addMenu(helpMenu);
    helpMenu->addAction(tr("&About"), this, &MainWindow::about);
    helpMenu->addAction(tr("About &Qt"), qApp, &QApplication::aboutQt);
}

void MainWindow::myCursorChanged()
{
    oldState = editor->blockSignals(true);
    cursorChangedBM();
    editor->blockSignals(oldState);
}


void MainWindow::myTextChanged()
{
    oldState = editor->blockSignals(true);
    if (editor->toPlainText()!="")
    {
        braketMatching();
        indentation();
    }
    else {
        textLenght=0;
        newTextLenght=0;
    }

editor->blockSignals(oldState);
}



