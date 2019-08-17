/****************************************************************************
**
** Copyright (C) 2019 The Nima Latifi.
** Contact: nima.latifi@gmail.com
**
** This software is a code editor base on exprTK liabrary's keywords.
******************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "highlighter.h"
#include <QPlainTextEdit>
#include <QMainWindow>
#include <QStack>
#include <string>
#include <QDebug>
#include <algorithm>
//#include <myeditor.h>
using namespace std;

QT_BEGIN_NAMESPACE
class QTextEdit;
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    void setupEditor();
    void setupFileMenu();
    void setupHelpMenu();
    QPlainTextEdit *editor;
    Highlighter *highlighter;
    QString text, tmpString, str, tabString;
    QList<QTextEdit::ExtraSelection> myExtraSelections;
    //each bracket is a node,
    struct node{
        QString character;                     /*hold the bracket charachter. "{,}" or "[,]" or "(,)"*/
        int cursorposition;                    /*hold the bracket position inside the text*/
        int pairClosBracketIndexInGlobalVector;  /*if this node is an open bracket ,then this field will be set by it's pair bracket position
                                                * in bracketsvector, if there is not any pair then it will be set by -1*/
        int pairOpenBracketIndexInGlobalVector;  /*if character type is an close bracket , this field will hold it's pair bracket position
                                                * in bracketsvector, if there is not any pair it will be set by -1*/
        int bracketsIndex;                       /* the bracket position in bracketsvector*/
        bool balancedFlag;                     /* if this node has a pair in the text, so this filed is TRUE else it is FALSE*/
    };
    // for ignoring bracket in comments
    struct CommentRule{
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    CommentRule commentRule;
    QVector<CommentRule> CommentRuleVector;
    QRegularExpression commentStartExpression;
    QRegularExpression commentEndExpression;
    struct node element, target;
    QVector <node> openBracketsVector, bracketsVector;/* openBracketsVector is a temporary vector which hold just open      *
                                                   * brackets for balancing.                                            *
                                                   *  bracketsVector is a vector which all brackets whill be placed there */
    bool returnValue;
    QTextCharFormat format, tmpFormat, resetFormat;
    QTextCursor tmpCursor;
    QRegExp rExp;
    bool oldState, cursurPosFlag=false;
    int lastCursorPosition = 0, pos = 0, tabCount = 0, textLenght = 0, newTextLenght = 0, index = -1;

public:
    MainWindow(QWidget *parent = nullptr);
    bool AreMatchedPair(QString opening, QString closing);
    bool areBracketsBalanced(QPlainTextEdit  *editor);
    void refreshBracketsVector(QPlainTextEdit  *editor);
    void insertElement(QString str, int pos);
    void pushElementInVector( QVector <node> *v, QString st, int pos, int pob, int pcb, int gi, bool flg);
    bool isEnter(QString str);
    bool isBalanced();
    bool isOpenBracket(QString str);
    bool isCloseBracket(QString str);
    QString returnPair(QString str);
    QString ignorComment(QString strTmp);

    //for cursor changed signal
    int binarySearch(QVector<node> arr, int l, int r, int x);

    //for indentation
    int findClosestBracket(QVector<node> arr, int n, node target);
    void bracketsMatching();
    void indentation();
    void findMatchedPairBrackets();
    void highlightUnMatchedPairBrackets(int pos);
    void highlightMatchedPairBrackets(int i , QString ch);
    struct node selectPreviouschar(int cursorPosition);
    void resetToDefaultEditor();
    QTextCharFormat matchedPairBracketsFormat();
    QTextCharFormat unMatchedPairBracketsFormat();
    int indentLevel(int index);
    void setFormat(QTextCharFormat format,int pos);
public slots:
    void about();
    void newFile();
    void openFile(const QString &path = QString());
    void myTextChanged();
    void myCursorChanged();

};
#endif // MAINWINDOW_H
