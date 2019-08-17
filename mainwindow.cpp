#include <QtWidgets>
#include <QPlainTextEdit>
#include "mainwindow.h"
#include "highlighter.h"
#include <QTextCharFormat>
#include <QVector>
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
    setWindowTitle(tr("Nima CodeEditor"));
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About NIMA CodeEditor"),
                       tr("<p>The <b>NIMA CodeEditor</b> is a Qt based editor and support" \
                   " some features like syntax highlighting, bracket matching, indentation, auto close bracket and ... ." \
                   " exprTK library's keywords supported by editor highlight rule, but you can modify it and replace your own keywords." \
                   " highlighting rules using regular expressions and is based from Qt QSyntaxhighlighter example</p><br/><b>Contact:</b>"\
                   " </br> <p>nima.latifi@gmail.com</p> "));
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
            bracketsMatching();
            textLenght=editor->toPlainText().length();
            editor->blockSignals(oldState);
        }
    }
}
void MainWindow::setupEditor()
{
    QFont font;
    font.setFamily("DroidSansMono");
    font.setFixedPitch(true);
    font.setPointSize(10);
    editor = new QPlainTextEdit();
    editor->setFont(font);
    int tabstop = 4;
    QFontMetricsF fm (editor->font());
    auto stopWidth = tabstop * fm.width(' ');
    auto letterSpacing = (ceil(stopWidth) - stopWidth) / tabstop;
    font.setLetterSpacing(QFont::AbsoluteSpacing, letterSpacing);
    editor->setFont(font);
    editor->setTabStopWidth(ceil(stopWidth));
    QColor color =QColor(250, 250, 250);
    QPalette p = editor->palette();
    p.setColor(QPalette::Base, color);
    editor->setPalette(p);
    rExp.setPatternSyntax(QRegExp::RegExp);
    rExp.setPattern("\\{|\\}|\\(|\\)|\\[|\\]");  // rExp is an regExpression to match brackets : (){}[] .
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
    qDebug()<<"////myCursorChanged////";
    findMatchedPairBrackets();
    editor->blockSignals(oldState);
}
void MainWindow::myTextChanged()
{
    oldState = editor->blockSignals(true);
    qDebug()<<"////myTextChanged/////";
    if (editor->toPlainText()!="")
    {
        bracketsMatching();
        indentation();
    }
    else {
        textLenght=0;
        newTextLenght=0;
    }
    editor->blockSignals(oldState);
}

void MainWindow::bracketsMatching()
{
    qDebug() << "--------bracketsMatching()---------";
    //highlight those brackets which have not opening or closing matched Pair.
    newTextLenght=editor->toPlainText().length();
    tmpCursor=editor->textCursor();
    lastCursorPosition=editor->textCursor().position();
    resetToDefaultEditor();                 // turn off all highlighted brackets
    if(!areBracketsBalanced(editor)){        //collect all brackets in to a vector and check their balance status(has matchedPair or not).
        for (int i=0;i<bracketsVector.length();i++)
        {
            if(bracketsVector[i].balancedFlag==false)
            {
                highlightUnMatchedPairBrackets(bracketsVector[i].cursorposition); //highlight those brackets which are not balanced (has not any matched pair)
            }
        }
    }
}
bool MainWindow::areBracketsBalanced(QPlainTextEdit  *editor){
    refreshBracketsVector(editor);//push all brackets in bracketsVector
    if(!bracketsVector.isEmpty()){
        return  isBalanced(); //move along bracketsVector and find any element which has not any match Pair.(it means text is not balanced)
    }
    else{
        return true;
    }
}
bool MainWindow::AreMatchedPair(QString opening, QString closing)
{
    if(opening == '(' && closing == ')') return true;
    else if(opening == '{' && closing == '}') return true;
    else if(opening == '[' && closing == ']') return true;
    return false;
}
void MainWindow::insertElement(QString str , int pos){
    /* insertElement(bracket character , bracket position in the text)
     * insert element(a bracket) in verctor. there is a special mechanism to insert bracket in vector.                         *
     * we need to know about each bracket informations such as its position in the text and its pair position                 *
     * in the bracketsVector. so while inserting bracket in bracketsVector we should follow these steps:                          *
     * 1- if it is an open bracket, directly push in bracketsVector and openVector- no need to find its pair in this step       *
     *  - Notic: while inserting in openVector we need to know its bracketsVector Index too.                                   *
     * 2- if it is a close bracket, first we need to find its pair bracket. in order to find its pair,looking for it in        *
     *  - openVector. if you find a pair bracket, first push it to bracketsVector then remove it from openBracketVector. in this *
     *  - step set the required information befor push in bracketsVector*/
    if (isCloseBracket(str)){
        if (openBracketsVector.isEmpty()){
            pushElementInVector(&bracketsVector, str, pos, -1, -1, bracketsVector.size(), false);
        }
        else{
            for (int i = openBracketsVector.size()-1; i >= 0; i--){
                if(AreMatchedPair(openBracketsVector[i].character,str)){
                    bracketsVector[openBracketsVector[i].bracketsIndex].balancedFlag = true;
                    bracketsVector[openBracketsVector[i].bracketsIndex].pairClosBracketIndexInGlobalVector = bracketsVector.size();
                    pushElementInVector(&bracketsVector, str, pos, openBracketsVector[i].bracketsIndex, -1, bracketsVector.size(), true);
                    openBracketsVector.remove(i);
                    returnValue = true;
                    break;
                }
                else {
                    returnValue = false;
                }
            }
            if(!returnValue){//if there is no pair for closeBracket in the text.
                pushElementInVector(&bracketsVector, str, pos, -1, -1, bracketsVector.size(), false);
            }
        }
    }
    else {
        int bracketsIndex = bracketsVector.size();
        pushElementInVector(&bracketsVector, str, pos, -1, -1, bracketsIndex, false);
        pushElementInVector(&openBracketsVector, str, pos, -1, -1, bracketsIndex, false);
    }
}
void MainWindow::pushElementInVector( QVector <node> *v, QString st, int pos, int pob, int pcb, int gi, bool flg){
    element.character = st;
    element.cursorposition = pos;
    element.pairOpenBracketIndexInGlobalVector = pob;
    element.pairClosBracketIndexInGlobalVector = pcb;
    element.bracketsIndex = gi;
    element.balancedFlag = flg;
    v->push_back(element);
}
bool MainWindow::isEnter(QString str){
    if (str == "\xa")
        return true;
    else{
        return false;
    }
}
bool MainWindow::isBalanced(){
    qDebug()<<"---isBalanced()";
    /* if there is any bracket in text which has not match bracket (), it means that text is not balanced. as we collect      *
     * all the brackets in bracketsVector and set thir blanced status(has pair or not) in previous steps, so move along        *
     * bracketsVector andcheck their balancedFlag. if there is one bracket with False balanced flag, it return false. */
    for(int i = 0; i <= bracketsVector.size()-1; i++){
        if(bracketsVector[i].balancedFlag == false){
            returnValue=false;
            break;
        }
    }
    return returnValue;
}
bool MainWindow::isOpenBracket(QString str){
    if ( str == "{" or str == "[" or str == "(" )
        return true;
    else {
        return false;
    }
}
bool MainWindow::isCloseBracket(QString str){
    if( str == "}" or str == "]" or str == ")" )
        return true;
    else {
        return false;
    }
}
QString MainWindow::returnPair(QString str){
    if( str == "{" )
        return "}";
    else if ( str == "[" )
        return "]";
    else if ( str == "(" )
        return ")";
    else if ( str == "}" )
        return "{";
    else if ( str == "]" )
        return "[";
    else if ( str == ")" )
        return "(";
}
QString MainWindow::ignorComment(QString strTmp)
{
    CommentRuleVector.clear();

    /* singleLine Comment Format */
    commentRule.pattern = QRegularExpression(QStringLiteral("//[^\n]*"));
    CommentRuleVector.append(commentRule);

    /* qutaition*/
    commentRule.pattern = QRegularExpression(QStringLiteral("\"(.*?)\""));
    CommentRuleVector.append(commentRule);

    /* singleQutaition*/
    commentRule.pattern = QRegularExpression(QStringLiteral("\'(.*?)\'"));
    CommentRuleVector.append(commentRule);

    /* hashTag Comment*/
    commentRule.pattern = QRegularExpression(QStringLiteral("#[^\n]*"));
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
            for(int i = match.capturedStart(); i <= match.capturedEnd()-1; i++)
            {
                strTmp.replace(i, 1, "0");
            }
        }
    }
    int startIndex = 0, j = 0;
    startIndex = strTmp.indexOf(commentStartExpression);
    while (startIndex >= 0 and startIndex<strTmp.length()) {
        QRegularExpressionMatch match = commentEndExpression.match(strTmp, startIndex);
        int endIndex = match.capturedStart();
        int commentLength = 0;
        if (endIndex == -1)
        {
            commentLength = strTmp.length() - startIndex;
            for( j = startIndex; j < strTmp.length(); j++ )
            {
                strTmp.replace( j, 1, "0" );
            }
            break;
        }
        else
        {
            commentLength = endIndex - startIndex + match.capturedLength();
        }
        startIndex = strTmp.indexOf(commentStartExpression, startIndex + commentLength);
    }
    return strTmp;
}

//for cursor changed signal
int MainWindow::binarySearch(QVector<node> arr, int l, int r, int x){
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
int MainWindow::findClosestBracket(QVector<node> arr, int n, node target){
    // Corner cases
    if (target.cursorposition < arr[0].cursorposition)
        return -1;
    if (target.cursorposition == arr[0].cursorposition)
        return 0;
    if (target.cursorposition >= arr[n - 1].cursorposition)
        return arr[n - 1].bracketsIndex;

    // Doing binary search
    int i = 0, j = n, mid = 0;
    while (i < j) {
        mid = (i + j) / 2;

        if (arr[mid].cursorposition == target.cursorposition)
            return arr[mid].bracketsIndex;

        /* If target is less than array element,
            then search in left */
        if (target.cursorposition < arr[mid].cursorposition) {

            // If target is greater than previous
            // to mid, return closest of two
            if (mid > 0 && target.cursorposition > arr[mid - 1].cursorposition)
                return arr[mid - 1].bracketsIndex;
            /* Repeat for left half */
            j = mid;
        }

        // If target is greater than mid
        else {
            if (mid < n - 1 && target.cursorposition < arr[mid + 1].cursorposition)
                return arr[mid].bracketsIndex;

            // update i
            i = mid + 1;
        }
    }

    // Only single element left after search
    return arr[mid].bracketsIndex;
}
void MainWindow::indentation()
{
     qDebug()<<"*****indentation()*****";
    //auto indent in blocks afer open brackets or close or Enter keyPress
    newTextLenght = editor->toPlainText().length();
    tmpCursor = editor->textCursor();
    lastCursorPosition = editor->textCursor().position();
    if ( (textLenght - newTextLenght) < 0 ){ //if no delete or backspace
         qDebug()<<"**indent-1-**";
         qDebug()<<"**indent-1-lcp:**"<<lastCursorPosition;
         if(lastCursorPosition>0){
             if ( isEnter( QString( editor->toPlainText()[lastCursorPosition-1]) ) ){
                 qDebug()<<"**indent-2-**";
                 tabString = "";
                 tabCount = 0;
                 if ( !bracketsVector.isEmpty() ){
                     element.character = "Enter";
                     element.cursorposition = lastCursorPosition;
                     tabCount = indentLevel( findClosestBracket( bracketsVector, bracketsVector.size(), element ) );
                 }
                 for ( int i = 0; i < tabCount; i++ ){
                     tabString += "    ";
                 }
                 tmpCursor.movePosition( QTextCursor::Right, QTextCursor::KeepAnchor, 1 );
                 if( isCloseBracket( tmpCursor.selectedText().trimmed() ) ){
                     tabString += "\n";
                     for (int i = 0; i < tabCount-1; i++){
                         tabString += "    ";
                     }
                 }
                 else{
                     tmpCursor.movePosition( QTextCursor::Left, QTextCursor::MoveAnchor, 1 );
                 }
                 tmpCursor.setPosition( lastCursorPosition );
                 tmpCursor.insertText( tabString );
                 tmpCursor.setPosition( lastCursorPosition + ( tabCount * 4 ) );
                 editor->setTextCursor( tmpCursor );
                 newTextLenght = editor->toPlainText().length();
                 qDebug()<<"indentation: "<<newTextLenght;
             }
             if ( isCloseBracket( QString( editor->toPlainText()[lastCursorPosition-1] ) ) ){ qDebug()<<"**indent-3-**";
                 tmpCursor.setPosition( lastCursorPosition - 5 );
                 tmpCursor.movePosition( QTextCursor::Right, QTextCursor::KeepAnchor, 4 );
                 if( tmpCursor.selectedText().trimmed() == "" ){
                     tmpCursor.removeSelectedText();
                     tmpCursor.movePosition( QTextCursor::Right, QTextCursor::MoveAnchor, 1 );
                 }
                 else{
                     tmpCursor.setPosition( lastCursorPosition );
                 }
                 editor->setTextCursor(tmpCursor);
                 newTextLenght=editor->toPlainText().length();
             }
             if ( isOpenBracket( QString(editor->toPlainText()[lastCursorPosition-1] ) ) ){ qDebug()<<"**indent-4-**";
                 tmpCursor.setPosition( lastCursorPosition );
                 tmpCursor.insertText( returnPair( QString( editor->toPlainText()[lastCursorPosition-1] ) ) );
                 if( areBracketsBalanced( editor ) ){
                     resetToDefaultEditor();
                 }
                 tmpCursor.setPosition( lastCursorPosition );
                 editor->setTextCursor( tmpCursor );
                 newTextLenght = editor->toPlainText().length();
             }
             qDebug()<<"**indent-5-**";
         }
    }
    qDebug()<<"ind: " << editor->toPlainText();
   // bracketsMatching();
    textLenght=newTextLenght;
}
void MainWindow::findMatchedPairBrackets(){
    /* highlight matchedpair bracket when cursor move next to a bracket*/
     qDebug()<<"*****findMatchedPairBrackets()*****";
    tmpCursor = editor->textCursor();
    lastCursorPosition = editor->textCursor().position();
    index = -1;
    if(tmpCursor.selectedText().length() == 0){           /* if there is not any selection in text*/
        if(cursurPosFlag == true){                        /* if there is any highlighted bracket in text and user move away from last bracket,
                                                           *  then turn off all of brackets and rerender the text */
            resetToDefaultEditor();
            bracketsMatching();
            cursurPosFlag=false;
        }
        if(!bracketsVector.isEmpty() and lastCursorPosition > 0){
            if (isCloseBracket(selectPreviouschar( lastCursorPosition).character ) or
                isOpenBracket(selectPreviouschar( lastCursorPosition).character )){
                    refreshBracketsVector(editor);
                index = binarySearch(bracketsVector, 0, bracketsVector.size()-1, target.cursorposition);
            }
            if(index >= 0 and bracketsVector[index].cursorposition == lastCursorPosition and
                bracketsVector[index].character == target.character){
                highlightMatchedPairBrackets(index, bracketsVector[index].character);
            }
        }
    }
}
void MainWindow::highlightUnMatchedPairBrackets(int pos)
{
    setFormat(unMatchedPairBracketsFormat(),pos);
    editor->setExtraSelections(myExtraSelections); //append highlighted brackets format to the text.
}
void MainWindow::highlightMatchedPairBrackets(int i , QString ch)
{
    if(isOpenBracket(ch)){
        if (bracketsVector[i].pairClosBracketIndexInGlobalVector != -1){
            setFormat(matchedPairBracketsFormat(),bracketsVector[bracketsVector[i].pairClosBracketIndexInGlobalVector].cursorposition);
            setFormat(matchedPairBracketsFormat(),lastCursorPosition);
            cursurPosFlag = true;
        }
        editor->setExtraSelections(myExtraSelections);
    }
    else if (isCloseBracket(ch)){
        if (bracketsVector[i].pairOpenBracketIndexInGlobalVector != -1){
            setFormat(matchedPairBracketsFormat(),bracketsVector[bracketsVector[i].pairOpenBracketIndexInGlobalVector].cursorposition);
            setFormat(matchedPairBracketsFormat(),lastCursorPosition);
            cursurPosFlag = true;
        }
        editor->setExtraSelections(myExtraSelections);
    }
}
struct MainWindow::node MainWindow::selectPreviouschar(int cursorPosition){
    tmpCursor.setPosition(cursorPosition);
    tmpCursor.movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor, 1);
    target.cursorposition=cursorPosition;
    target.character = tmpCursor.selectedText();
    return target;
}
void MainWindow::resetToDefaultEditor() {
    //turn back all highlighted brackets format to default format.
    myExtraSelections.clear();
    editor->setExtraSelections(myExtraSelections);
}
QTextCharFormat MainWindow::matchedPairBracketsFormat(){
    resetFormat.setBackground(QColor(153, 204, 98,180));
    resetFormat.setForeground(Qt::red);
    return resetFormat;
}
QTextCharFormat MainWindow::unMatchedPairBracketsFormat(){
    resetFormat.setBackground(QColor(50, 50, 50,80));
    resetFormat.setForeground(QColor(200, 0, 0));
    return resetFormat;
}
int MainWindow::indentLevel(int index){
    // calculate current cursor position depth from root block. each inner block is one level.
    int depth = 0;
    if (index == -1){
        depth = 0;
    }
    else{
        for (int i = index; i >= 0; i--){
            if (isOpenBracket(bracketsVector[i].character)){
                depth++;
            }
            else if (isCloseBracket(bracketsVector[i].character) and bracketsVector[i].balancedFlag==true){
                depth--;
            }
        }
        if (depth < 0)
            depth = 0;
    }
    return depth;
}

void MainWindow::refreshBracketsVector(QPlainTextEdit  *editor){
    //move alonge the text, find and insert brackets in to bracketsVector
    str = editor->toPlainText();
    bracketsVector.clear();
    openBracketsVector.clear();
    pos=0;
    returnValue=true;
    qDebug() << "refresh: ========================";
    str = ignorComment(editor->toPlainText());
    qDebug() << "refresh: " << str;
    while (pos >= 0) {
        qDebug() << "refresh:  pos" << pos;
        pos = rExp.indexIn(str , pos);// rExp is an regExpression to match brackets : (){}[] .
        if (pos >= 0) {
            ++pos;      // move along in str
            tmpString = rExp.capturedTexts().join("");
            insertElement(tmpString,pos);//insert bracket(element) to bracketsVector/openBracketsVector
            qDebug() << "refresh:  after insertElement" ;
        }
    }
}

void MainWindow::setFormat(QTextCharFormat format,int pos){
    QTextEdit::ExtraSelection selection;
    selection.format = format ;
    selection.cursor = tmpCursor;
    selection.cursor.clearSelection();
    selection.cursor.setPosition(pos);
    selection.cursor.movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor, 1);
    myExtraSelections.append(selection);
}
























