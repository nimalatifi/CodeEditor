/****************************************************************************
**
** Copyright (C) 2019 The Nima Latifi.
** Contact: nima.latifi@gmail.com
**
** This software is a code editor base on exprTK liabrary's keywords.
******************************************************************************/
#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>

QT_BEGIN_NAMESPACE
class QTextDocument;
QT_END_NAMESPACE


class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    Highlighter(QTextDocument *parent=0  );


protected:
    void highlightBlock(const QString &text  ) override;

private:
    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };

    QVector<HighlightingRule> highlightingRules;

    QRegularExpression commentStartExpression;
    QRegularExpression commentEndExpression;

    QTextCharFormat keywordFormat;
    QTextCharFormat classFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat singleQuotationFormat;
    QTextCharFormat functionFormat;
    QTextCharFormat mathFunction;
    QTextCharFormat trigonometry;
    QTextCharFormat stringProcessing;
    QTextCharFormat booleanOperators;
    QTextCharFormat loopAndControlStructure;
    QTextCharFormat fundamentalTypes;
    QTextCharFormat IOpackage;
    QTextCharFormat digit;
    QTextCharFormat assignment;
     QTextCharFormat hashTagCommentFormat;



};
//! [0]

#endif // HIGHLIGHTER_H
