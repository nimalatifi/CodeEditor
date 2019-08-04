/****************************************************************************
**
** Copyright (C) 2019 The Nima Latifi.
** Contact: nima.latifi@gmail.com
**
** This software is a code editor base on exprTK liabrary's keywords.
****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "highlighter.h"
#include <QPlainTextEdit>
#include <QMainWindow>
#include <QStack>
#include <string>
#include <QDebug>
#include <algorithm>
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
    QPlainTextEdit *editor,*editor2;
    Highlighter *highlighter;
    QString text,tmpString,str;
    struct node{
        QString character;
        int cursorposition;
        int childIndexInVector;
        int parentIndexInVector;
        int globalIndex;
        bool balancedFlag;
    };
    // for ignoring braket in comments
    struct CommentRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    CommentRule commentRule;
    QVector<CommentRule> CommentRuleVector;
    QRegularExpression commentStartExpression;
    QRegularExpression commentEndExpression;
    // *********************************
    struct node element,target;
    QStack<node>  objStack;
    QVector <node> objVector,globalVector;
    bool returnValue;
    QTextCharFormat format,tmpFormat;
    QTextCursor tmpCursor,tmpCursor2 ;
    QRegExp rExp;
    QString tabString;
    bool oldState,cursurPosFlag=false;
    int lastCursorPosition=0,pos=0,tabCount=0,textLenght=0,newTextLenght=0,index=-1;
public:
    MainWindow(QWidget *parent = 0);
    bool ArePair(QString opening,QString closing)
    {
        if(opening == '(' && closing == ')') return true;
        else if(opening == '{' && closing == '}') return true;
        else if(opening == '[' && closing == ']') return true;
        return false;
    }
    //الگوریتم اول
    bool AreParanthesesBalanced(QPlainTextEdit  *editor)
    {
        str=editor->toPlainText();
        objStack.clear();
        objVector.clear();
        globalVector.clear();
        pos=0;
        while (pos >= 0) {
            pos = rExp.indexIn(str, pos);
            if (pos >= 0) {
                ++pos;      // move along in str
                tmpString=rExp.capturedTexts().join("");
                if(tmpString== '(' || tmpString == '{' || tmpString == '[')
                {
                    element.character=tmpString;
                    element.cursorposition=pos;
                    element.parentIndexInVector=0;
                    element.childIndexInVector=0;
                    element.globalIndex=globalVector.size();
                    objStack.push(element);
                    globalVector.push_back(element);
                }
                else if(tmpString == ')' || tmpString == '}' || tmpString == ']')
                {
                    if(objStack.empty())
                    {
                        element.character=tmpString;
                        element.cursorposition=pos;
                        element.parentIndexInVector=0;
                        element.childIndexInVector=0;
                        element.globalIndex=globalVector.size();
                        objStack.push(element);
                        globalVector.push_back(element);
                    }
                    else if( !ArePair(objStack.top().character,tmpString))
                    {
                        element.character=tmpString;
                        element.cursorposition=pos;
                        element.parentIndexInVector=0;
                        element.childIndexInVector=0;
                        element.globalIndex=globalVector.size();
                        objStack.push(element);
                        globalVector.push_back(element);
                    }
                    else
                    {
                        element.character=tmpString;
                        element.cursorposition=pos;
                        element.parentIndexInVector=objStack.top().globalIndex;//*******************//
                        element.childIndexInVector= 0;
                        element.globalIndex=globalVector.size();
                        globalVector.push_back(element);
                        globalVector[objStack.top().globalIndex].childIndexInVector= globalVector.size()-1;
                        globalVector[globalVector.size()-1].parentIndexInVector=objStack.top().globalIndex;
                        objStack.pop();
                    }

                }
            }
        }
        returnValue= objStack.isEmpty() ? true:false;
        if (!objStack.isEmpty())
        {
            for (int j=objStack.size();j>0;j--)
            {
                element.character=objStack.top().character;
                element.cursorposition=objStack.top().cursorposition;
                element.globalIndex=globalVector.size();
                element.childIndexInVector=0;
                element.parentIndexInVector=0;
                objVector.push_back(objStack.top());
                objStack.pop();
            }
        }
        return returnValue;

    }
    //الگوریتم دوم- اصلی
    bool APB(QPlainTextEdit  *editor)
    {
        str=editor->toPlainText();
        globalVector.clear();
        objVector.clear();
        pos=0;
        returnValue=true;
        str=ignorComment(editor->toPlainText());
        qDebug()<<"APB: "<<str;
        while (pos >= 0) {
            pos = rExp.indexIn(str, pos);
            if (pos >= 0) {
                ++pos;      // move along in str
                tmpString=rExp.capturedTexts().join("");
                insertElements(tmpString,pos);
            }
        }
        if(!globalVector.isEmpty())
        {
            return  isBalanced();
        }
        else
        {
            return true;
        }
    }

    void refreshGlobalVector(QPlainTextEdit  *editor)
    {
        str=editor->toPlainText();
        globalVector.clear();
        objVector.clear();
        pos=0;
        returnValue=true;
        str=ignorComment(editor->toPlainText());
        qDebug()<<"refresh: "<<str;

        while (pos >= 0) {
            pos = rExp.indexIn(str, pos);
            if (pos >= 0) {


                ++pos;      // move along in str


                tmpString=rExp.capturedTexts().join("");
                insertElements(tmpString,pos);

            }
        }
    }
    void  insertElements(QString str , int pos)
    {
        if (isCloseBraket(str))
        {
            if (objVector.isEmpty())
            {
                element.character=str;
                element.cursorposition=pos;
                element.parentIndexInVector=-1;
                element.childIndexInVector=-1;
                element.globalIndex=globalVector.size();
                element.balancedFlag=false;
                globalVector.push_back(element);
            }
            else
            {
                for (int i=objVector.size()-1;i>=0;i--) {
                    if(ArePair(objVector[i].character,str))
                    {
                        element.character=str;
                        element.cursorposition=pos;
                        element.parentIndexInVector=objVector[i].globalIndex;
                        element.childIndexInVector=-1;
                        element.globalIndex=globalVector.size();
                        element.balancedFlag=true;
                        globalVector[objVector[i].globalIndex].balancedFlag=true;
                        globalVector[objVector[i].globalIndex].childIndexInVector=globalVector.size();
                        globalVector.push_back(element);
                        objVector.remove(i);
                        returnValue=true;
                        break;
                    }
                    else {
                        returnValue=false;
                    }
                }
                if(!returnValue)
                {
                    element.character=str;
                    element.cursorposition=pos;
                    element.parentIndexInVector=-1;
                    element.childIndexInVector=-1;
                    element.globalIndex=globalVector.size();
                    element.balancedFlag=false;
                    globalVector.push_back(element);
                }
            }
        }
        else {
            element.character=str;
            element.cursorposition=pos;
            element.parentIndexInVector=-1;
            element.childIndexInVector=-1;
            element.globalIndex=globalVector.size();
            element.balancedFlag=false;
            globalVector.push_back(element);
            objVector.push_back(element);
        }
    }
    bool isEnter(QString str)
    {
        if (str=="\xa")
            return true;
        else {
            return false;
        }
    }

    bool isBalanced()
    {
        for(int i=0;i<=globalVector.size()-1;i++)
        {
            if(globalVector[i].balancedFlag==false)
            {
                returnValue=false;
                break;
            }
        }
        return returnValue;

    }
    bool isOpenBraket(QString str)
    {
        if (str=="{" or str=="[" or str=="(")
            return true;
        else {
            return false;
        }
    }
    bool isCloseBraket(QString str)
    {
        if (str=="}" or str=="]" or str==")" )
            return true;
        else {
            return false;
        }
    }
    QString returnPair(QString str)
    {
        if(str=="{")
            return "}";
        else if (str=="[")
            return "]";
        else if (str=="(")
            return ")";
        else if (str=="}")
            return "{";
        else if (str=="]")
            return "[";
        else if (str==")")
            return "(";
    }

    QString ignorComment(QString strTmp)
    {
        CommentRuleVector.clear();
        /* singleLine Comment Format */
        commentRule.pattern = QRegularExpression(QStringLiteral("//[^\n]*"));
        CommentRuleVector.append(commentRule);

        /* qutaition*/
        commentRule.pattern= QRegularExpression(QStringLiteral("\"(.*?)\""));
        CommentRuleVector.append(commentRule);

        /* singleQutaition*/
        commentRule.pattern= QRegularExpression(QStringLiteral("\'(.*?)\'"));
        CommentRuleVector.append(commentRule);


        /* multi line Comment Format */
        commentRule.pattern = QRegularExpression(QStringLiteral("/\\*([^*]|[\r\n]|(\\*+([^*/]|[\r\n])))*\\*+/"));
        CommentRuleVector.append(commentRule);

        commentStartExpression = QRegularExpression(QStringLiteral("/\\*"));
        commentEndExpression = QRegularExpression(QStringLiteral("\\*/"));



        for (const CommentRule &commentRule : qAsConst(CommentRuleVector)) {
            QRegularExpressionMatchIterator matchIterator = commentRule.pattern.globalMatch(strTmp);
            while (matchIterator.hasNext()) {
                QRegularExpressionMatch match = matchIterator.next();
                for(int i=match.capturedStart();i<=match.capturedEnd()-1;i++)
                {
                    strTmp.replace(i,1,"0");
                }
            }
        }

        int startIndex = 0, j=0;
        startIndex = strTmp.indexOf(commentStartExpression);
        while (startIndex >= 0 and startIndex<strTmp.length()) {
            QRegularExpressionMatch match = commentEndExpression.match(strTmp, startIndex);
            int endIndex = match.capturedStart();
            int commentLength = 0;
            if (endIndex == -1)
            {
                commentLength = strTmp.length() - startIndex;

                for( j=startIndex;j<strTmp.length();j++)
                {
                    strTmp.replace(j,1,"0");
                }

                break;
            }
            else
            {
                commentLength = endIndex - startIndex+ match.capturedLength();
                //                        for( j=startIndex;j<=startIndex + commentLength;j++)
                //                        {
                //                            strTmp.replace(j,1,"0");
                //                        }
            }
            startIndex = strTmp.indexOf(commentStartExpression, startIndex + commentLength);
        }
        return strTmp;
    }

    //for cursor changed event
    int binarySearch(QVector<node> arr, int l, int r, int x)
    {
        if (r >= l) {
            int mid = l + (r - l) / 2;

            // If the element is present at the middle
            // itself
            if (arr[mid].cursorposition == x)
                return mid;

            // If element is smaller than mid, then
            // it can only be present in left subarray
            if (arr[mid].cursorposition > x)
                return binarySearch(arr, l, mid - 1, x);

            // Else the element can only be present
            // in right subarray
            return binarySearch(arr, mid + 1, r, x);
        }

        // We reach here when element is not
        // present in array
        return -1;
    }
    //for indentation
    int findClosestBraket(QVector<node> arr, int n, node target)
    {
        // Corner cases
        if (target.cursorposition < arr[0].cursorposition)
            return -1;
        if (target.cursorposition == arr[0].cursorposition)
            return 0;
        if (target.cursorposition >= arr[n - 1].cursorposition)
            return arr[n - 1].globalIndex;

        // Doing binary search
        int i = 0, j = n, mid = 0;
        while (i < j) {
            mid = (i + j) / 2;

            if (arr[mid].cursorposition == target.cursorposition)
                return arr[mid].globalIndex;

            /* If target is less than array element,
            then search in left */
            if (target.cursorposition < arr[mid].cursorposition) {

                // If target is greater than previous
                // to mid, return closest of two
                if (mid > 0 && target.cursorposition > arr[mid - 1].cursorposition)
                    return arr[mid - 1].globalIndex;
                /* Repeat for left half */
                j = mid;
            }

            // If target is greater than mid
            else {
                if (mid < n - 1 && target.cursorposition < arr[mid + 1].cursorposition)
                    return arr[mid].globalIndex;

                // update i
                i = mid + 1;
            }
        }

        // Only single element left after search
        return arr[mid].globalIndex;
    }
    void braketMatching()
    {
        newTextLenght=editor->toPlainText().length();
        //  oldState = editor->blockSignals(true);
        tmpCursor=editor->textCursor();
        format= editor->QPlainTextEdit::currentCharFormat();
        format.setBackground(QColor(240,240,240,255));
        lastCursorPosition=editor->textCursor().position();
        if(!APB(editor)){
            tmpCursor.setPosition( 0);
            tmpCursor.movePosition(QTextCursor::End, QTextCursor::KeepAnchor,1);
            tmpCursor.mergeCharFormat(format);
            tmpFormat.setBackground(QColor(200,200,200,255));
            for (int i=0;i<globalVector.length();i++)
            {
                if(globalVector[i].balancedFlag==false)
                {
                    tmpCursor.setPosition( globalVector[i].cursorposition);
                    tmpCursor.movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor, 1);
                    tmpCursor.mergeCharFormat(tmpFormat);
                }
            }
            tmpCursor.setPosition(lastCursorPosition);
            tmpCursor.movePosition(QTextCursor::NextCharacter,QTextCursor::MoveAnchor,0);
            editor->setTextCursor(tmpCursor);
        }
        else {
            tmpCursor.setPosition( 0);
            tmpCursor.movePosition(QTextCursor::End, QTextCursor::KeepAnchor,1);
            tmpCursor.mergeCharFormat(format);
            tmpCursor.setPosition(lastCursorPosition);
            tmpCursor.setCharFormat(format);
            editor->setTextCursor(tmpCursor);
        }

        //editor->blockSignals(oldState);
    }
    void indentation()
    {
        newTextLenght=editor->toPlainText().length();
        //oldState = editor->blockSignals(true);
        tmpCursor=editor->textCursor();
        format= editor->QPlainTextEdit::currentCharFormat();
        format.setBackground(QColor(240,240,240,255));
        lastCursorPosition=editor->textCursor().position();
        if ((textLenght-newTextLenght)<0)
        {
            if ( isEnter(QString(editor->toPlainText()[lastCursorPosition-1]) ))
            {
                tabString="";
                tabCount=0;
                if (!globalVector.isEmpty())
                {
                    element.character="Enter";
                    element.cursorposition=lastCursorPosition;
                    tabCount=indentLevel( findClosestBraket(globalVector,globalVector.size(),element));
                }
                else
                {
                    tabCount=0;
                }
                for (int i=0;i<=tabCount;i++)
                {
                    tabString+="    ";
                }
                tmpCursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor,1);
                if( isCloseBraket(tmpCursor.selectedText().trimmed()))
                {
                    tabString+="\n";
                    for (int i=0;i<tabCount-1;i++)
                    {
                        tabString+="    ";
                    }
                }
                else
                {
                    tmpCursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor,1);
                }
                tmpCursor.setPosition(lastCursorPosition);
                tmpCursor.insertText(tabString);
                tmpCursor.setPosition(lastCursorPosition+(tabCount*4));
                editor->setTextCursor(tmpCursor);
                newTextLenght=editor->toPlainText().length();
            }

            if ( isCloseBraket(QString( editor->toPlainText()[lastCursorPosition-1])))
            {
                tmpCursor.setPosition(lastCursorPosition-5);
                tmpCursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor,4);
                if(tmpCursor.selectedText().trimmed()=="")
                {
                    tmpCursor.removeSelectedText();
                    tmpCursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor,1);
                }
                else
                {
                    tmpCursor.setPosition(lastCursorPosition);
                }

                editor->setTextCursor(tmpCursor);
                newTextLenght=editor->toPlainText().length();
            }
            if ( isOpenBraket(QString(editor->toPlainText()[lastCursorPosition-1])))
            {
                tmpCursor.setPosition(lastCursorPosition);
                tmpCursor.insertText(returnPair(QString(editor->toPlainText()[lastCursorPosition-1])));
                if(APB(editor)){
                    format.setBackground(QColor(240,240,240,255));
                    tmpCursor.setPosition( 0);
                    tmpCursor.movePosition(QTextCursor::End, QTextCursor::KeepAnchor,1);
                    tmpCursor.mergeCharFormat(format);
                    tmpCursor.setPosition(lastCursorPosition);
                    tmpCursor.movePosition(QTextCursor::NextCharacter,QTextCursor::MoveAnchor,0);
                    tmpCursor.setCharFormat(format);
                    editor->setTextCursor(tmpCursor);
                }
                tmpCursor.setPosition(lastCursorPosition);
                editor->setTextCursor(tmpCursor);
                newTextLenght=editor->toPlainText().length();
            }
        }
        // editor->blockSignals(oldState);

        textLenght=newTextLenght;

    }
    void cursorChangedBM()
    {

        tmpCursor=editor->textCursor();
        lastCursorPosition=editor->textCursor().position();
        if(tmpCursor.selectedText().length()==0)
        {
            if(cursurPosFlag==true)
            {
                tmpFormat.setBackground(QColor(240,240,240,255));
                tmpFormat.setForeground(Qt::black);
                tmpCursor.setPosition( 0);
                tmpCursor.movePosition(QTextCursor::End, QTextCursor::KeepAnchor,1);
                tmpCursor.mergeCharFormat(tmpFormat);
                tmpCursor.setPosition(lastCursorPosition);
                editor->setTextCursor(tmpCursor);
                braketMatching();
                // refreshGlobalVector(editor);
                cursurPosFlag=false;
            }

            if(globalVector.isEmpty())
            {
                cursurPosFlag=false;
            }
            else
            {

                if(lastCursorPosition>0){
                    tmpCursor.setPosition(lastCursorPosition);
                    tmpCursor.movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor, 1);
                    target.cursorposition=lastCursorPosition;
                    target.character=tmpCursor.selectedText();
                    if (isCloseBraket(target.character) or isOpenBraket(target.character))
                    {
                        refreshGlobalVector(editor);
                        index=binarySearch(globalVector,0,globalVector.size()-1,target.cursorposition);
                    }
                    else
                    {
                        tmpCursor.setPosition(lastCursorPosition);
                        tmpCursor.movePosition(QTextCursor::NoMove,QTextCursor::MoveAnchor);
                        tmpFormat.setBackground(QColor(240,240,240,255));
                        tmpFormat.setForeground(Qt::black);
                        editor->setTextCursor(tmpCursor);
                        index=-1;
                        cursurPosFlag=false;
                    }

                    if(index>=0)
                    {
                        if(globalVector[index].cursorposition==lastCursorPosition and globalVector[index].character==target.character)
                        {
                            if(isOpenBraket( globalVector[index].character))
                            {
                                if (globalVector[index].childIndexInVector!=-1)
                                {
                                    tmpFormat.setBackground(QColor(153, 204, 98,180));
                                    tmpFormat.setForeground(Qt::red);
                                    tmpCursor.mergeCharFormat(tmpFormat);
                                    tmpCursor.setPosition(globalVector[globalVector[index].childIndexInVector].cursorposition);
                                    tmpCursor.movePosition(QTextCursor::PreviousCharacter,QTextCursor::KeepAnchor);
                                    tmpCursor.mergeCharFormat(tmpFormat);
                                    tmpCursor.setPosition(lastCursorPosition);
                                    tmpCursor.movePosition(QTextCursor::NoMove,QTextCursor::MoveAnchor);
                                    tmpFormat.setBackground(QColor(240,240,240,255));
                                    tmpFormat.setForeground(Qt::black);
                                    tmpCursor.mergeCharFormat(tmpFormat);
                                    editor->setTextCursor(tmpCursor);
                                    cursurPosFlag=true;
                                }
                            }
                            else if( isCloseBraket( globalVector[index].character))
                            {
                                if (globalVector[index].parentIndexInVector!=-1)
                                {
                                    tmpFormat.setBackground(QColor(153, 204, 98,180));
                                    tmpFormat.setForeground(Qt::red);
                                    tmpCursor.mergeCharFormat(tmpFormat);
                                    tmpCursor.setPosition(globalVector[globalVector[index].parentIndexInVector].cursorposition);
                                    tmpCursor.movePosition(QTextCursor::PreviousCharacter,QTextCursor::KeepAnchor);
                                    tmpCursor.mergeCharFormat(tmpFormat);
                                    tmpCursor.setPosition(lastCursorPosition);
                                    tmpCursor.movePosition(QTextCursor::NoMove,QTextCursor::MoveAnchor);
                                    tmpFormat.setBackground(QColor(240,240,240,255));
                                    tmpFormat.setForeground(Qt::black);
                                    tmpCursor.mergeCharFormat(tmpFormat);
                                    editor->setTextCursor(tmpCursor);
                                    cursurPosFlag=true;
                                }
                            }
                        }
                        else {
                            tmpCursor.setPosition(lastCursorPosition);
                            tmpCursor.movePosition(QTextCursor::NoMove,QTextCursor::MoveAnchor);
                            tmpFormat.setBackground(QColor(240,240,240,255));
                            tmpFormat.setForeground(Qt::black);
                            editor->setTextCursor(tmpCursor);
                            cursurPosFlag=false;
                        }
                    }
                    else
                    {
                        tmpCursor.setPosition(lastCursorPosition);
                        tmpCursor.movePosition(QTextCursor::NoMove,QTextCursor::MoveAnchor);
                        tmpFormat.setBackground(QColor(240,240,240,255));
                        tmpFormat.setForeground(Qt::black);
                        editor->setTextCursor(tmpCursor);
                        cursurPosFlag=false;
                    }
                }
                else
                {
                    cursurPosFlag=false;
                }
            }

        }



    }

    int indentLevel(int index)
    {
        int depth=0;
        if (index==-1)
        {
            depth=0;
        }
        else
        {
            for (int i=index;i>=0;i--)
            {
                if (isOpenBraket( globalVector[i].character))
                {
                    depth++;
                }
                else if (isCloseBraket(globalVector[i].character))
                {
                    depth--;
                }
            }
            if (depth<0)
                depth=0;
        }
        return depth;
    }
public slots:
    void about();
    void newFile();
    void openFile(const QString &path = QString());
    void myTextChanged();
    void myCursorChanged();


};
#endif // MAINWINDOW_H
