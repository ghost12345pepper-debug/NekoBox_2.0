#include "dialog_ping_settings.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QDialogButtonBox>
#include "main/NekoGui.hpp"

DialogPingSettings::DialogPingSettings(QWidget *parent) : QDialog(parent) {
    setWindowTitle(tr("Ping Test Settings"));
    resize(400, 500);

    auto mainLayout = new QVBoxLayout(this);

    chkAutoCheck = new QCheckBox(tr("Проверять автоматически"), this);
    mainLayout->addWidget(chkAutoCheck);

    chkIncludeMulti = new QCheckBox(tr("Включить сюда несколько серверов"), this);
    mainLayout->addWidget(chkIncludeMulti);

    listServers = new QListWidget(this);
    mainLayout->addWidget(listServers);

    auto layoutInterval = new QHBoxLayout();
    layoutInterval->addWidget(new QLabel(tr("Интервал времени:")));
    comboInterval = new QComboBox(this);
    comboInterval->addItem("5 минут", 5);
    comboInterval->addItem("10 минут", 10);
    comboInterval->addItem("30 минут", 30);
    comboInterval->addItem("1 час", 60);
    comboInterval->addItem("12 часов", 720);
    comboInterval->addItem("24 часа", 1440);
    layoutInterval->addWidget(comboInterval);
    mainLayout->addLayout(layoutInterval);

    chkCheckAllNow = new QCheckBox(tr("Проверять сразу же все сервера"), this);
    mainLayout->addWidget(chkCheckAllNow);

    auto layoutTypeAll = new QHBoxLayout();
    layoutTypeAll->addWidget(new QLabel(tr("Тип теста для всех серверов:")));
    comboTestTypeAll = new QComboBox(this);
    comboTestTypeAll->addItem("URL Test", 0);
    comboTestTypeAll->addItem("TCP Ping", 1);
    layoutTypeAll->addWidget(comboTestTypeAll);
    mainLayout->addLayout(layoutTypeAll);

    auto layoutTypeSel = new QHBoxLayout();
    layoutTypeSel->addWidget(new QLabel(tr("Тип теста для выделенных серверов:")));
    comboTestTypeSelected = new QComboBox(this);
    comboTestTypeSelected->addItem("URL Test", 0);
    comboTestTypeSelected->addItem("TCP Ping", 1);
    layoutTypeSel->addWidget(comboTestTypeSelected);
    mainLayout->addLayout(layoutTypeSel);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    mainLayout->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, this, [=]() {
        saveSettings();
        accept();
    });
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    connect(chkAutoCheck, &QCheckBox::toggled, this, &DialogPingSettings::updateUiState);
    connect(chkIncludeMulti, &QCheckBox::toggled, this, &DialogPingSettings::updateUiState);

    // populate servers
    // NekoGui::profileManager->CurrentGroup()->profiles -> but it's complex, we'll leave list empty or add dummy for now, then hook up to ProfileManager later
    
    loadSettings();
    updateUiState();
}

void DialogPingSettings::updateUiState() {
    bool enabled = chkAutoCheck->isChecked();
    chkIncludeMulti->setEnabled(enabled);
    listServers->setEnabled(enabled && chkIncludeMulti->isChecked());
    comboInterval->setEnabled(enabled);
    chkCheckAllNow->setEnabled(enabled);
    comboTestTypeAll->setEnabled(enabled);
    comboTestTypeSelected->setEnabled(enabled);
}

void DialogPingSettings::loadSettings() {
    chkAutoCheck->setChecked(NekoGui::dataStore->auto_ping_enabled);
    chkIncludeMulti->setChecked(NekoGui::dataStore->auto_ping_include_multi);
    
    int intervalType = NekoGui::dataStore->auto_ping_interval_type;
    int idx = comboInterval->findData(intervalType);
    if (idx >= 0) comboInterval->setCurrentIndex(idx);
    
    chkCheckAllNow->setChecked(NekoGui::dataStore->auto_ping_check_all);
    
    int typeAll = NekoGui::dataStore->auto_ping_type_all;
    idx = comboTestTypeAll->findData(typeAll);
    if (idx >= 0) comboTestTypeAll->setCurrentIndex(idx);
    
    int typeSel = NekoGui::dataStore->auto_ping_type_selected;
    idx = comboTestTypeSelected->findData(typeSel);
    if (idx >= 0) comboTestTypeSelected->setCurrentIndex(idx);
}

void DialogPingSettings::saveSettings() {
    NekoGui::dataStore->auto_ping_enabled = chkAutoCheck->isChecked();
    NekoGui::dataStore->auto_ping_include_multi = chkIncludeMulti->isChecked();
    NekoGui::dataStore->auto_ping_interval_type = comboInterval->currentData().toInt();
    NekoGui::dataStore->auto_ping_check_all = chkCheckAllNow->isChecked();
    NekoGui::dataStore->auto_ping_type_all = comboTestTypeAll->currentData().toInt();
    NekoGui::dataStore->auto_ping_type_selected = comboTestTypeSelected->currentData().toInt();
    NekoGui::dataStore->Save();
}
