#ifndef UICONTROLLER_H
#define UICONTROLLER_H

#include <QMainWindow>
#include <QPushButton>


class UiController {
public:
    UiController();
    ~UiController();

    void setButtonFeatures(QPushButton* button);
};

#endif // UICONTROLLER_H
