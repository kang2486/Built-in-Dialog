#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>

class Calculator : public QWidget
{
    Q_OBJECT

public:
    Calculator(QWidget *parent = nullptr);
    ~Calculator();

private slots:
    void digitClicked();
    void operatorClicked();
    void equalClicked();
    void clearClicked();
    void pointClicked();

private:
    QLineEdit *display;
    QPushButton *digitButtons[10];
    QPushButton *operatorButtons[4]; // +, -, *, /
    QPushButton *equalButton;
    QPushButton *clearButton;
    QPushButton *pointButton;
    
    double currentValue;
    double storedValue;
    QString currentOperator;
    bool newInput;
};

#endif // CALCULATOR_H
