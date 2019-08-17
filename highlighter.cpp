/****************************************************************************
**
** Copyright (C) 2019 The Nima Latifi.
** Contact: nima.latifi@gmail.com
**
** This software is a code editor base on exprTK liabrary's keywords.
******************************************************************************/

#include "highlighter.h"
#include <QDebug>


Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    /* keywordFormat */
    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);
    const QString keywordPatterns[] = {
        QStringLiteral("\\b(?i)var\\b")
    };
    for (const QString &pattern : keywordPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);

    }

    /* mathFunction */
    mathFunction.setForeground(QColor(194, 171, 0));
    const QString mathFunctionPatterns[]={
        QStringLiteral("\\b(?i)abs\\b"),
        QStringLiteral("\\b(?i)avg\\b"),
        QStringLiteral("\\b(?i)ceil\\b"),
        QStringLiteral("\\b(?i)clamp\\b"),
        QStringLiteral("\\b(?i)equal\\b"),
        QStringLiteral("\\b(?i)erf\\b"),
        QStringLiteral("\\b(?i)erfc\\b"),
        QStringLiteral("\\b(?i)exp\\b"),
        QStringLiteral("\\b(?i)expml\\b"),
        QStringLiteral("\\b(?i)floor\\b"),
        QStringLiteral("\\b(?i)frac\\b"),
        QStringLiteral("\\b(?i)hypot\\b"),
        QStringLiteral("\\b(?i)iclamp\\b"),
        QStringLiteral("\\b(?i)inrange\\b"),
        QStringLiteral("\\b(?i)log\\b"),
        QStringLiteral("\\b(?i)log10\\b"),
        QStringLiteral("\\b(?i)cloglp\\b"),
        QStringLiteral("\\b(?i)log2\\b"),
        QStringLiteral("\\b(?i)logn\\b"),
        QStringLiteral("\\b(?i)max\\b"),
        QStringLiteral("\\b(?i)min\\b"),
        QStringLiteral("\\b(?i)mul\\b"),
        QStringLiteral("\\b(?i)ncdf\\b"),
        QStringLiteral("\\b(?i)nequal\\b"),
        QStringLiteral("\\b(?i)pow\\b"),
        QStringLiteral("\\b(?i)root\\b"),
        QStringLiteral("\\b(?i)round\\b"),
        QStringLiteral("\\b(?i)roundn\\b"),
        QStringLiteral("\\b(?i)sgn\\b"),
        QStringLiteral("\\b(?i)sqrt\\b"),
        QStringLiteral("\\b(?i)sum\\b"),
        QStringLiteral("\\b(?i)swap\\b"),
        QStringLiteral("\\b(?i)trunc\\b")
    };
    for (const QString &pattern : mathFunctionPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = mathFunction;
        highlightingRules.append(rule);
    }

    /* trigonometry */
    trigonometry.setForeground(QColor(201, 79, 2));
    const QString trigonometryPatterns[]={
        QStringLiteral("\\b(?i)acos\\b"),
        QStringLiteral("\\b(?i)acosh\\b"),
        QStringLiteral("\\b(?i)asin\\b"),
        QStringLiteral("\\b(?i)asinh\\b"),
        QStringLiteral("\\b(?i)atan\\b"),
        QStringLiteral("\\b(?i)atan2\\b"),
        QStringLiteral("\\b(?i)atanh\\b"),
        QStringLiteral("\\b(?i)cos\\b"),
        QStringLiteral("\\b(?i)cosh\\b"),
        QStringLiteral("\\b(?i)cot\\b"),
        QStringLiteral("\\b(?i)csc\\b"),
        QStringLiteral("\\b(?i)sec\\b"),
        QStringLiteral("\\b(?i)sin\\b"),
        QStringLiteral("\\b(?i)sinc\\b"),
        QStringLiteral("\\b(?i)sinh\\b"),
        QStringLiteral("\\b(?i)tan\\b"),
        QStringLiteral("\\b(?i)tanh\\b"),
        QStringLiteral("\\b(?i)deg2rad\\b"),
        QStringLiteral("\\b(?i)deg2grad\\b"),
        QStringLiteral("\\b(?i)rad2deg\\b"),
        QStringLiteral("\\b(?i)grad2deg\\b")
    };

    for (const QString &pattern : trigonometryPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = trigonometry;
        highlightingRules.append(rule);
    }

    /* string Proccessing */
    stringProcessing.setForeground(QColor(2, 140, 156));
    const QString stringProcessingPatterns[]={
        QStringLiteral("\\b(?i)in\\b"),
        QStringLiteral("\\b(?i)like\\b"),
        QStringLiteral("\\b(?i)ilike\\b")
        /*QStringLiteral("[]"),
        QStringLiteral("<=>"),
        QStringLiteral("+="),
        QStringLiteral("+"),
        QStringLiteral(":="),
        QStringLiteral("[r0:r1]")*/
    };
    for (const QString &pattern : stringProcessingPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = stringProcessing;
        highlightingRules.append(rule);
    }

    /* fundamental Types */
    fundamentalTypes.setForeground(QColor(163,155,8));
    const QString fundamentalTypesPatterns[]={
        QStringLiteral("\\b(?i)scalar\\b"),
        QStringLiteral("\\b(?i)vector\\b"),
        QStringLiteral("\\b(?i)string\\b")
    };
    for (const QString &pattern : fundamentalTypesPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = fundamentalTypes;
        highlightingRules.append(rule);
    }

    /* boolean Operators */
    booleanOperators.setForeground(QColor(212, 2, 61));
    const QString booleanOperatorsPatterns[]={
        QStringLiteral("\\b(?i)true\\b"),
        QStringLiteral("\\b(?i)false\\b"),
        QStringLiteral("\\b(?i)and\\b"),
        QStringLiteral("\\b(?i)mand\\b"),
        QStringLiteral("\\b(?i)mor\\b"),
        QStringLiteral("\\b(?i)nand\\b"),
        QStringLiteral("\\b(?i)nor\\b"),
        QStringLiteral("\\b(?i)not\\b"),
        QStringLiteral("\\b(?i)or\\b"),
        QStringLiteral("\\b(?i)xor\\b"),
        QStringLiteral("\\b(?i)xnor\\b"),
        /* QStringLiteral("\\b&\\b"),
        QStringLiteral("\\b|\\b")*/
    };
    for (const QString &pattern : booleanOperatorsPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = booleanOperators;
        highlightingRules.append(rule);
    }

    /* IO Package */
    IOpackage.setForeground(QColor(17, 102, 240));
    const QString IOpackagePatterns[]={

        /* File I/O function */
        QStringLiteral("\\b(?i)open\\b"),
        QStringLiteral("\\b(?i)close\\b"),
        QStringLiteral("\\b(?i)read\\b"),
        QStringLiteral("\\b(?i)write\\b"),
        QStringLiteral("\\b(?i)getline\\b"),
        QStringLiteral("\\b(?i)eof\\b"),

        /* Basic I/O function */
        QStringLiteral("\\bprint\\b"),
        QStringLiteral("\\bprintln\\b"),

        /* vector operation function */
        QStringLiteral("\\ball_true\\b"),
        QStringLiteral("\\ball_false\\b"),
        QStringLiteral("\\bany_true\\b"),
        QStringLiteral("\\bany_false\\b"),
        QStringLiteral("\\bcount\\b"),
        QStringLiteral("\\bcopy\\b"),
        QStringLiteral("\\brotate-left\\b"),
        QStringLiteral("\\brotate-right\\b"),
        QStringLiteral("\\bshift-left\\b"),
        QStringLiteral("\\bshift-right\\b"),
        QStringLiteral("\\bsort\\b"),
        QStringLiteral("\\bnth-element\\b"),
        QStringLiteral("\\biota\\b"),
        QStringLiteral("\\bsumk\\b"),
        QStringLiteral("\\baxpy\\b"),
        QStringLiteral("\\baxpby\\b"),
        QStringLiteral("\\baxpyz\\b"),
        QStringLiteral("\\baxpbyz\\b"),
        QStringLiteral("\\baxpbz\\b"),
        QStringLiteral("\\bdot\\b"),
        QStringLiteral("\\bdotk\\b"),
        };
    for (const QString &pattern : IOpackagePatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = IOpackage;
        highlightingRules.append(rule);
    }

    /* loop And Control Structure */
    loopAndControlStructure.setFontWeight(QFont::Bold);
    loopAndControlStructure.setForeground(QColor(0,0,255));
    const QString loopAndControlStructurePatterns[]={
        QStringLiteral("\\b(?i)if\\b"),
        QStringLiteral("\\b(?i)else\\b"),
        QStringLiteral("\\b(?i)switch\\b"),
        QStringLiteral("\\b(?i)while\\b"),
        QStringLiteral("\\b(?i)repeat\\b"),
        QStringLiteral("\\b(?i)until\\b"),
        QStringLiteral("\\b(?i)for\\b"),
        QStringLiteral("\\b(?i)break\\b"),
        QStringLiteral("\\b(?i)continue\\b"),
        QStringLiteral("\\b(?i)return\\b")
        /* QStringLiteral("?:"),
        QStringLiteral("~"),
        QStringLiteral("*"),
        QStringLiteral("[]"),*/

    };
    for (const QString &pattern : loopAndControlStructurePatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = loopAndControlStructure;
        highlightingRules.append(rule);
    }

    /* Qt Class function*/
    classFormat.setFontWeight(QFont::Bold);
    classFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegularExpression(QStringLiteral("\\bQ[A-Za-z]+\\b"));
    rule.format = classFormat;
    highlightingRules.append(rule);

    /*   functionFormat  */
    /*functionFormat.setFontItalic(true);
    functionFormat.setForeground(Qt::blue);
    rule.pattern = QRegularExpression(QStringLiteral("\\b[A-Za-z0-9_]+(?=\\()"));
    rule.format = functionFormat;
    highlightingRules.append(rule);*/

    /* digits */
    digit.setForeground(QColor(128, 0, 0));
    rule.pattern = QRegularExpression(QStringLiteral("[0-9]"));
    rule.format = digit;
    highlightingRules.append(rule);

    /* assignment signs    */
    assignment.setForeground(QColor(1, 117, 117));
    rule.pattern = QRegularExpression(QStringLiteral("{|}|:=|>|<|\\+=|\\-=|\\*=|\\/=|\\%=|=|==|<>|"
                                                     "\\!=|<=|>=|\\^|\\*|\\+|\\-|\\/|\\%|;|\\(|\\)"));
    rule.format = assignment;
    highlightingRules.append(rule);

    /*   quotationFormat  */
    quotationFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegularExpression(QStringLiteral("\"(.*?)\""));
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    /*  singleQuotationFormat  */
    quotationFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegularExpression(QStringLiteral("\'(.*?)\'"));
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    /* singleLine Comment Format */
    singleLineCommentFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegularExpression(QStringLiteral("//[^\n]*"));
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    /* hashTagCommentFormat*/
    hashTagCommentFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegularExpression(QStringLiteral("#[^\n]*"));
    rule.format = hashTagCommentFormat;
    highlightingRules.append(rule);

    /* multi line Comment Format */
    multiLineCommentFormat.setForeground(Qt::darkGreen);
    commentStartExpression = QRegularExpression(QStringLiteral("/\\*"));
    commentEndExpression = QRegularExpression(QStringLiteral("\\*/"));
}

void Highlighter::highlightBlock(const QString &text )
{
    for (const HighlightingRule &rule : qAsConst(highlightingRules)) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
    setCurrentBlockState(0);
    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = text.indexOf(commentStartExpression);

    while (startIndex >= 0) {
        QRegularExpressionMatch match = commentEndExpression.match(text, startIndex);
        int endIndex = match.capturedStart();
        int commentLength = 0;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex
                            + match.capturedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
    }
}

