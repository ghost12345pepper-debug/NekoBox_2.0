#ifndef DIALOG_PING_SETTINGS_H
#define DIALOG_PING_SETTINGS_H

#include <QDialog>
#include <QCheckBox>
#include <QComboBox>
#include <QListWidget>

class DialogPingSettings : public QDialog {
    Q_OBJECT

public:
    explicit DialogPingSettings(QWidget *parent = nullptr);

private:
    QCheckBox *chkAutoCheck;
    QCheckBox *chkIncludeMulti;
    QListWidget *listServers;
    QComboBox *comboInterval;
    QCheckBox *chkCheckAllNow;
    QComboBox *comboTestTypeAll;
    QComboBox *comboTestTypeSelected;

    void updateUiState();
    void loadSettings();
    void saveSettings();
};

#endif // DIALOG_PING_SETTINGS_H
