#include "uicontroller.h"

void UiController::setButtonFeatures(QPushButton* button) {
    button->setStyleSheet(
        "QPushButton {"
        "  background-color: #3498db;"
        "  color: white;"
        "  font-size: 18px;" /* Increases the font size */
        "  border-radius: 5px;"
        "  min-width:  160px;" /* Sets a minimum width */
        "  min-height: 60px;" /* Sets a minimum height */
        "}"
        "QPushButton:hover {"
        "  background-color: #2980b9;"
        "}"
        "QPushButton:pressed {"
        "  background-color: #1e5f7f;"
        "}"
    );
}
