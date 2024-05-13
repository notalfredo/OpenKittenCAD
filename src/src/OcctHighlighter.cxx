// Copyright (c) 2020 OPEN CASCADE SAS
//
// This file is part of the examples of the Open CASCADE Technology software library.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE

#include "OcctHighlighter.h"

#include <Standard_WarningsDisable.hxx>
#include <QFont>
#include <QStringList>
#include <Standard_WarningsRestore.hxx>

#include <QColor>
#include <QDebug>
#include <iostream>


OcctHighlighter::OcctHighlighter(QTextDocument* theParent)
: QSyntaxHighlighter (theParent)
{
    QStringList keywords;
    keywords
        << "\\bfn\\b" << "\\blet\\b" << "\\bconst\\b" << "\\bbreak\\b" << "\\bcontinue\\b" << "\\breturn\\b"
        << "\\band\\b" << "\\bor\\b" << "\\bfor\\b"  << "\\bif\\b" << "\\belif\\b" << "\\belse\\b" << "\\bmod\\b" ;


    QStringList operators;
    operators 
        << "==" << "!=" << ">=" << "<=" << ":=" << "->" << "\\|>";


    QStringList types;
    types
        << "\\bshape\\b" << "\\bpoint\\b" << "\\bedge\\b" << "\\bvoid\\b" <<  "\\bnumber\\b";


    QColor operatorColor = QColor(220, 20, 60);
    QColor keywordColor = QColor(255, 127, 80);
    QColor typeColor = QColor(201, 181, 255);
    QColor functionColor = QColor(239, 207, 227);
    QColor commentColor = QColor(149, 165, 166);


    HighlightingRule aRule;

    myOperatorFormat.setForeground(operatorColor);
    myOperatorFormat.setFontWeight(QFont::Bold);
    foreach (const QString& aPattern, operators)
    {
        aRule.myPattern = QRegExp(aPattern);
        aRule.myFormat = myOperatorFormat;
        myHighlightingRules.append(aRule);
    }


    myKeywordFormat.setForeground(keywordColor);
    myKeywordFormat.setFontWeight(QFont::Bold);
    foreach (const QString& aPattern, keywords)
    {
        aRule.myPattern = QRegExp(aPattern);
        aRule.myFormat = myKeywordFormat;
        myHighlightingRules.append(aRule);
    }


    typeFormat.setForeground(typeColor);
    typeFormat.setFontWeight(QFont::Bold);
    foreach (const QString& aPattern, types)
    {
        aRule.myPattern = QRegExp(aPattern);
        aRule.myFormat = typeFormat;
        myHighlightingRules.append(aRule);
    }


    identifierFormat.setForeground(functionColor);
    identifierFormat.setFontWeight(QFont::Bold);
    aRule.myPattern = QRegExp(QLatin1String("\\b[A-Za-z0-9_]+(?=\\()"));
    aRule.myFormat = identifierFormat;
    myHighlightingRules.append(aRule);


    mySingleLineCommentFormat.setForeground(commentColor);
    aRule.myPattern = QRegExp(QLatin1String("//[^\n]*"));
    aRule.myFormat = mySingleLineCommentFormat;
    myHighlightingRules.append(aRule);


    myMultiLineCommentFormat.setForeground(commentColor);
    myCommentStartExpression = QRegExp(QLatin1String("/\\*"));
    myCommentEndExpression = QRegExp(QLatin1String("\\*/"));
}

void OcctHighlighter::highlightBlock (const QString& theText)
{
    foreach (const HighlightingRule &rule, myHighlightingRules) {
        qDebug() << rule.myPattern;

        QRegExp expression(rule.myPattern);
        int index = expression.indexIn(theText);

        while(index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.myFormat);
            index = expression.indexIn(theText, index + length);
        }
    }

    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1) {
        startIndex = theText.indexOf(myCommentStartExpression);
    }

    while (startIndex >= 0) {
        int endIndex = myCommentEndExpression.indexIn(theText, startIndex);
        int commentLength;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = theText.length() - startIndex;
        } 
        else {
            commentLength = endIndex - startIndex
                + myCommentEndExpression.matchedLength();
        }
        setFormat(startIndex, commentLength, myMultiLineCommentFormat);
        startIndex = myCommentEndExpression.indexIn(theText, startIndex + commentLength);
    }
}
