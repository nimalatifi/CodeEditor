#ifndef MYPLAINTEXTEDIT_H
#define MYPLAINTEXTEDIT_H

#include <QPlainTextEdit>
#include <QDebug>

class MyPlainTextEdit : public QPlainTextEdit
{
public:
    MyPlainTextEdit();

    void keyPressEvent ( QKeyEvent * event )
    {
        if( event->key() == Qt::Key_Return )
        {
            // optional: if the QPlainTextEdit should do its normal action
    // even when the return button is pressed, uncomment the following line
            // QPlainTextEdit::keyPressEvent( event )
            qDebug() << "Enter pressed";
            /* do your stuff here */
            event->accept();
        }

            QPlainTextEdit::keyPressEvent( event );
    }
};

#endif // MYPLAINTEXTEDIT_H
