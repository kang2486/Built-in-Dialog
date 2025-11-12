#include "calculator.h"
#include <QGridLayout>
#include <QDebug>

Calculator::Calculator(QWidget *parent)
    : QWidget(parent), currentValue(0), storedValue(0), newInput(true)
{
    setWindowTitle(QStringLiteral("計算機"));
    
    // Create display
    display = new QLineEdit("0");
    display->setReadOnly(true);
    display->setAlignment(Qt::AlignRight);
    display->setMaxLength(15);
    
    QFont displayFont = display->font();
    displayFont.setPointSize(18);
    display->setFont(displayFont);
    display->setMinimumHeight(50);
    
    // Create digit buttons (0-9)
    for (int i = 0; i < 10; ++i) {
        digitButtons[i] = new QPushButton(QString::number(i));
        digitButtons[i]->setMinimumSize(60, 60);
        QFont font = digitButtons[i]->font();
        font.setPointSize(14);
        digitButtons[i]->setFont(font);
        connect(digitButtons[i], SIGNAL(clicked()), this, SLOT(digitClicked()));
    }
    
    // Create operator buttons
    QStringList operators = {"+", "-", "*", "/"};
    for (int i = 0; i < 4; ++i) {
        operatorButtons[i] = new QPushButton(operators[i]);
        operatorButtons[i]->setMinimumSize(60, 60);
        QFont font = operatorButtons[i]->font();
        font.setPointSize(14);
        operatorButtons[i]->setFont(font);
        connect(operatorButtons[i], SIGNAL(clicked()), this, SLOT(operatorClicked()));
    }
    
    // Create equal button
    equalButton = new QPushButton("=");
    equalButton->setMinimumSize(60, 60);
    QFont equalFont = equalButton->font();
    equalFont.setPointSize(14);
    equalButton->setFont(equalFont);
    connect(equalButton, SIGNAL(clicked()), this, SLOT(equalClicked()));
    
    // Create clear button
    clearButton = new QPushButton("C");
    clearButton->setMinimumSize(60, 60);
    QFont clearFont = clearButton->font();
    clearFont.setPointSize(14);
    clearButton->setFont(clearFont);
    connect(clearButton, SIGNAL(clicked()), this, SLOT(clearClicked()));
    
    // Create point button
    pointButton = new QPushButton(".");
    pointButton->setMinimumSize(60, 60);
    QFont pointFont = pointButton->font();
    pointFont.setPointSize(14);
    pointButton->setFont(pointFont);
    connect(pointButton, SIGNAL(clicked()), this, SLOT(pointClicked()));
    
    // Create layout
    QGridLayout *layout = new QGridLayout;
    
    // Add display at top
    layout->addWidget(display, 0, 0, 1, 4);
    
    // Add buttons in calculator layout
    // Row 1: 7 8 9 /
    layout->addWidget(digitButtons[7], 1, 0);
    layout->addWidget(digitButtons[8], 1, 1);
    layout->addWidget(digitButtons[9], 1, 2);
    layout->addWidget(operatorButtons[3], 1, 3); // /
    
    // Row 2: 4 5 6 *
    layout->addWidget(digitButtons[4], 2, 0);
    layout->addWidget(digitButtons[5], 2, 1);
    layout->addWidget(digitButtons[6], 2, 2);
    layout->addWidget(operatorButtons[2], 2, 3); // *
    
    // Row 3: 1 2 3 -
    layout->addWidget(digitButtons[1], 3, 0);
    layout->addWidget(digitButtons[2], 3, 1);
    layout->addWidget(digitButtons[3], 3, 2);
    layout->addWidget(operatorButtons[1], 3, 3); // -
    
    // Row 4: 0 . C +
    layout->addWidget(digitButtons[0], 4, 0);
    layout->addWidget(pointButton, 4, 1);
    layout->addWidget(clearButton, 4, 2);
    layout->addWidget(operatorButtons[0], 4, 3); // +
    
    // Row 5: = (spans 4 columns)
    layout->addWidget(equalButton, 5, 0, 1, 4);
    
    setLayout(layout);
    resize(280, 400);
}

Calculator::~Calculator()
{
}

void Calculator::digitClicked()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    QString digit = button->text();
    
    if (newInput) {
        display->setText(digit);
        newInput = false;
    } else {
        if (display->text() == "0") {
            display->setText(digit);
        } else {
            display->setText(display->text() + digit);
        }
    }
}

void Calculator::operatorClicked()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    QString op = button->text();
    
    if (!currentOperator.isEmpty()) {
        equalClicked();
    }
    
    storedValue = display->text().toDouble();
    currentOperator = op;
    newInput = true;
}

void Calculator::equalClicked()
{
    if (currentOperator.isEmpty()) {
        return;
    }
    
    currentValue = display->text().toDouble();
    double result = 0;
    
    if (currentOperator == "+") {
        result = storedValue + currentValue;
    } else if (currentOperator == "-") {
        result = storedValue - currentValue;
    } else if (currentOperator == "*") {
        result = storedValue * currentValue;
    } else if (currentOperator == "/") {
        if (currentValue != 0) {
            result = storedValue / currentValue;
        } else {
            display->setText("Error");
            currentOperator.clear();
            newInput = true;
            return;
        }
    }
    
    display->setText(QString::number(result));
    currentOperator.clear();
    newInput = true;
}

void Calculator::clearClicked()
{
    display->setText("0");
    currentValue = 0;
    storedValue = 0;
    currentOperator.clear();
    newInput = true;
}

void Calculator::pointClicked()
{
    if (newInput) {
        display->setText("0.");
        newInput = false;
    } else if (!display->text().contains(".")) {
        display->setText(display->text() + ".");
    }
}
