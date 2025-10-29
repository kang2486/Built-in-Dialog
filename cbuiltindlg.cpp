#include "cbuiltindlg.h"
#include <QGridLayout>
#include <QTextEdit>
#include <QTextCursor>
#include <QTextCharFormat>
#include <QPushButton>
#include <QColorDialog>
#include <QPalette>
#include <QProgressDialog>
#include <QFileDialog>
#include <QApplication>
#include <QFontDialog>

#include <QInputDialog>
#include <QLineEdit>
#include <QDir>

#include <QtPrintSupport/qprinter.h>
#include <QtPrintSupport/qpagesetupdialog.h>
#include <QtPrintSupport/QPrintDialog>
#include <QtPrintSupport/QPrintPreviewDialog>

#include <QPrinter>
#include<QPageSetupDialog>

#include <QPrintDialog>
#include <QErrorMessage>

CBuiltinDlg::CBuiltinDlg(QWidget *parent)
    : QDialog(parent)
{
    QGridLayout *gridLayout = new QGridLayout(this);


    displayTextEdit = new QTextEdit(QStringLiteral("Qt的標準通用對話盒"));
    colorPushBtn    = new QPushButton(QStringLiteral("顏色對話盒"));
    errorPushBtn    = new QPushButton(QStringLiteral("錯誤對話盒"));
    filePushBtn     = new QPushButton(QStringLiteral("檔案對話盒"));
    fontPushBtn     = new QPushButton(QStringLiteral("字體對話盒"));
    inputPushBtn    = new QPushButton(QStringLiteral("輸入對話盒"));
    pagePushBtn     = new QPushButton(QStringLiteral("頁面設定對話盒"));
    progressPushBtn = new QPushButton(QStringLiteral("進度對話盒"));
    printPushBtn    = new QPushButton(QStringLiteral("列印對話盒"));
    textColorPushBtn = new QPushButton(QStringLiteral("設定文字顏色"));


    gridLayout->addWidget(colorPushBtn,    0, 0);
    gridLayout->addWidget(errorPushBtn,    0, 1);
    gridLayout->addWidget(filePushBtn,     0, 2);
    gridLayout->addWidget(fontPushBtn,     1, 0);
    gridLayout->addWidget(pagePushBtn,     1, 1);
    gridLayout->addWidget(printPushBtn,    2, 1);
    gridLayout->addWidget(inputPushBtn,    1, 2);
    gridLayout->addWidget(progressPushBtn, 2, 0);
    gridLayout->addWidget(displayTextEdit, 3, 0, 3, 3);
    gridLayout->addWidget(textColorPushBtn, 2,2);

    setLayout(gridLayout);
    setWindowTitle(QStringLiteral("內建對話框顯示"));
    resize(400, 300);


    connect(colorPushBtn,    SIGNAL(clicked()), this, SLOT(doPushBtn()));
    connect(errorPushBtn,    SIGNAL(clicked()), this, SLOT(doPushBtn()));
    connect(filePushBtn,     SIGNAL(clicked()), this, SLOT(doPushBtn()));
    connect(fontPushBtn,     SIGNAL(clicked()), this, SLOT(doPushBtn()));
    connect(inputPushBtn,    SIGNAL(clicked()), this, SLOT(doPushBtn()));
    connect(progressPushBtn, SIGNAL(clicked()), this, SLOT(doPushBtn()));
    connect(pagePushBtn,     SIGNAL(clicked()), this, SLOT(doPushBtn()));
    connect(printPushBtn,    SIGNAL(clicked()), this, SLOT(doPushBtn()));
    connect(textColorPushBtn,    SIGNAL(clicked()), this, SLOT(doTextColor()));

}


void CBuiltinDlg::doPushBtn()
{
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if (btn == colorPushBtn)
    {
        QPalette palette = displayTextEdit->palette();
        const QColor& color =
            QColorDialog::getColor(palette.color(QPalette::Base),
                                   this, QStringLiteral("設定背景顏色"));

        if (color.isValid())
        {
            palette.setColor(QPalette::Base, color);
            displayTextEdit->setPalette(palette);
        }
    }
    if (btn == errorPushBtn)
    {
        QErrorMessage box(this);
        box.setWindowTitle(QStringLiteral("錯誤訊息盒"));
        box.showMessage(QStringLiteral("錯誤訊息盒會例x: "));
        box.showMessage(QStringLiteral("錯誤訊息盒會例yy: "));
        box.showMessage(QStringLiteral("錯誤訊息盒會例zz: "));
        box.exec();
    }

    if(btn == filePushBtn)
    {
        QString fileName = QFileDialog::getOpenFileName(this,
                                                        QStringLiteral("打開檔案"), ".",
                                                        QStringLiteral("任何檔案 (*.*);;文字檔 (*.txt);;XML檔 (*.xml)"));

        displayTextEdit->setText(fileName);
    }
    if(btn == fontPushBtn)
    {
        bool ok;
        const QFont &font = QFontDialog::getFont(&ok,
                                                 displayTextEdit->font(),
                                                 this,
                                                 QStringLiteral("字體對話盒"));
        if (ok)
            displayTextEdit->setFont(font);
    }
    if (btn == inputPushBtn)
    {
        bool ok;
        QString text = QInputDialog::getText(this,
                                             QStringLiteral("輸入對話盒"),
                                             QStringLiteral("輸入文字"),
                                             QLineEdit::Normal,
                                             QDir::home().dirName(),
                                             &ok);
        if (ok && !text.isEmpty())
            displayTextEdit->setText(text);
    }
    if (btn == pagePushBtn)
    {
        QPrinter printer(QPrinter::HighResolution);
        QPageSetupDialog *dlg = new QPageSetupDialog(&printer, this);
        dlg->setWindowTitle(QStringLiteral("頁面設定對話方塊"));
        if (dlg->exec() == QDialog::Accepted)
        {
        }
    }
    if(btn == progressPushBtn)
    {
        QProgressDialog progress(QStringLiteral("正在複製檔案..."),QStringLiteral("取消"),0,10000,this);
        progress.setWindowTitle(QStringLiteral("進度對話框"));
        progress.show();
        for(int i=0; i<10000; i++)
        {
            progress.setValue(i);
            qApp->processEvents();
            if(progress.wasCanceled())
                break;
            qDebug() << i;
        }
        //_sleep(10000);
    }
    if (btn == printPushBtn)
    {
        QPrinter printer(QPrinter::HighResolution);
        QPrintDialog dialog(&printer, this);
        if (dialog.exec() != QDialog::Accepted)
            return;
    }
}
void CBuiltinDlg::doTextColor()
{
    // 以目前文字顏色為預設
    QColor init = displayTextEdit->textColor();
    QColor color = QColorDialog::getColor(init, this, QStringLiteral("設定文字顏色"));
    if (!color.isValid()) return;
    QTextCursor cur = displayTextEdit->textCursor();
    if (!cur.hasSelection())
        cur.select(QTextCursor::Document);      // 選整份文件(不會閃一下)

    QTextCharFormat fmt;
    fmt.setForeground(color);                   // 設定文字顏色
    cur.mergeCharFormat(fmt);                   // 套用到選取/整篇
    displayTextEdit->mergeCurrentCharFormat(fmt);
    cur.clearSelection();
    displayTextEdit->setTextCursor(cur);
    // 若只想影響新輸入的文字：
    //displayTextEdit->setTextColor(color);

    // 若要把現有整段也一起變色，改用下面三行（擇一保留即可）：
    // displayTextEdit->selectAll();
    // displayTextEdit->setTextColor(color);
    // displayTextEdit->moveCursor(QTextCursor::End);
}

CBuiltinDlg::~CBuiltinDlg() {}
