#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QNvmeBlockDevice.h>
#include <QThread>
#include <QTimer>

#define output(s) ui->plainTextEdit->appendPlainText(s)

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->refreshButton, &QPushButton::pressed, this, &MainWindow::refreshDevices);
    connect(ui->removeButton, &QPushButton::pressed, this, &MainWindow::removeSelectedDevice);
    connect(ui->rescanButton, &QPushButton::pressed, this, &MainWindow::rescanPciBus);
    connect(ui->commonInfoButton, &QPushButton::pressed, this, &MainWindow::commonInfo);
    connect(ui->healthInfoButton, &QPushButton::pressed, this, &MainWindow::healthInfo);
    connect(ui->readTestButton, &QPushButton::pressed, this, &MainWindow::readTest);

    refreshDevices();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::refreshDevices()
{
    ui->deviceBox->clear();
    const QStringList devices = QNvmeBlockDevice::availableDevices();
    foreach (const QString dev, devices)
        ui->deviceBox->addItem(dev);
}

void MainWindow::removeSelectedDevice()
{
    ui->plainTextEdit->clear();

    QNvmeBlockDevice nvme;
    const QString dev = ui->deviceBox->currentText();

    if (!nvme.open(dev)) {
        output("Device opening error!");
        return;
    }

    QNvmePciDeviceInfo info = nvme.pciDeviceInfo();

    nvme.close(); // Important! You have to close device before removing!

    const QString location = info.location().toString();
    const QString path = QString("/sys/bus/pci/devices/%1/remove").arg(location);

    QFile file(path);
    if (!file.open(QFile::WriteOnly | QFile::ExistingOnly)) {
        output("System device opening error! Do you run this application as sudo user?");
        return;
    }

    const char c = '1';
    file.write(&c, sizeof(c)); // Same as "echo 1 > /sys/bus/pci/devices/.../remove"
    file.close();

    QThread::msleep(3000);
    refreshDevices();

    output("SUCCESS! Selected device can be physically disconnected now.");
}

void MainWindow::rescanPciBus()
{
    ui->plainTextEdit->clear();

    const QString path = "/sys/bus/pci/rescan";

    QFile file(path);
    if (!file.open(QFile::WriteOnly | QFile::ExistingOnly)) {
        output("System device opening error! Do you run this application as sudo user?");
        return;
    }

    const char c = '1';
    file.write(&c, sizeof(c)); // Same as "echo 1 > /sys/bus/pci/rescan"
    file.close();

    QThread::msleep(3000);
    refreshDevices();

    output("SUCCESS! See available devices list.");
}

void MainWindow::commonInfo()
{
    ui->plainTextEdit->clear();

    QNvmeBlockDevice nvme;
    const QString dev = ui->deviceBox->currentText();

    if (!nvme.open(dev)) {
        output("Device opening error!");
        return;
    }

    output("*** COMMON INFO ***\n");

    QNvmeDriverVersion drvVer = nvme.driverVersion();
    output(QString("Driver version: %1\n").arg(drvVer.toString()));

    QNvmePciDeviceInfo devInfo = nvme.pciDeviceInfo();
    output(QString("PCI device location (domain:bus:slot.function): %1")
           .arg(devInfo.location().toString()));
    output(QString("PCI device ID (vendor:device:class:revision): %1")
           .arg(devInfo.id().toString()));
    output(QString("PCI device link speed: %1\n").arg(devInfo.link()
                                                      .toString()));

    QNvmeControllerVersion ctrlVer = nvme.controllerVersion();
    output(QString("NVMe controller version: %1")
           .arg(ctrlVer.toString()));

    QNvmeControllerState ctrlState = nvme.controllerState();
    output(QString("NVMe controller state: enabled - %1, ready - %2, fatal - %3\n")
           .arg(ctrlState.enabled() ? "YES" : "NO")
           .arg(ctrlState.ready() ? "YES" : "NO")
           .arg(ctrlState.fatal() ? "YES" : "NO"));

    QNvmeControllerInfo ctrlInfo = nvme.controllerInfo();
    if (!nvme.lastError().success()) {
        output("\nDevice communication error!\n");
        return;
    }
    output(QString("Model name: %1")
           .arg(ctrlInfo.modelName()));
    output(QString("Firmware revision: %1")
           .arg(ctrlInfo.firmwareRevision()));
    output(QString("Serial number: %1")
           .arg(ctrlInfo.serialNumber()));
    output(QString("Capacity: %1 GB")
           .arg(ctrlInfo.totalCapacity() / (1000.0 * 1000.0 * 1000.0), 0, 'f', 1));
    output(QString("Maximum data transef size: %1 bytes\n")
           .arg(ctrlInfo.maxDataTransferSize()));

    if (ctrlInfo.namespaceCount() == 0)
        output("Namespaces are not found!");

    for (int i = 1; i <= ctrlInfo.namespaceCount(); i++) {
        output(QString("Namespace %1 details:").arg(i));

        QNvmeNamespaceInfo nsInfo = nvme.namespaceInfo(i);
        if (!nvme.lastError().success()) {
            output("\nDevice communication error!\n");
            return;
        }
        output(QString("-> Total size: %1 sectors")
               .arg(nsInfo.totalSize()));
        output(QString("-> Sector size: %1 bytes\n")
               .arg(nsInfo.sectorSize()));
    }
}

void MainWindow::healthInfo()
{
    ui->plainTextEdit->clear();

    QNvmeBlockDevice nvme;
    const QString dev = ui->deviceBox->currentText();

    if (!nvme.open(dev)) {
        output("Device opening error!");
        return;
    }

    output("*** HEALTH INFO ***\n");

    QNvmeLogPageHealthInfo health = nvme.logPageHealthInfo();
    if (!nvme.lastError().success()) {
        output("\nDevice communication error!\n");
        return;
    }

    output(QString("Critical warning: %1")
           .arg(QNvmeLogPageHealthInfo::criticalWarningToString(health.criticalWarning()).join("; ")));
    output(QString("Composite temperature: %1 °C")
           .arg(health.compositeTemperature() - 273));
    output(QString("Available spare: %1 %")
           .arg(health.availableSpare()));
    output(QString("Available spare threshold: %1 %")
           .arg(health.availableSpareThreshold()));
    output(QString("Percentage used: %1 %")
           .arg(health.percentageUsed()));
    output(QString("Data units read: %1 (%2 GiB)")
           .arg(health.dataUnitsRead()).arg(health.dataUnitsRead() * 512 * 1000 / (1024 * 1024 * 1024)));
    output(QString("Data units written: %1 (%2 GiB)")
           .arg(health.dataUnitsWritten()).arg(health.dataUnitsWritten() * 512 * 1000 / (1024 * 1024 * 1024)));
    output(QString("Host read commands: %1")
           .arg(health.hostReadCommands()));
    output(QString("Host write commands: %1")
           .arg(health.hostWriteCommands()));
    output(QString("Controller busy time: %1 m")
           .arg(health.controllerBusyTime()));
    output(QString("Power cycles: %1")
           .arg(health.powerCycles()));
    output(QString("Power on hours: %1")
           .arg(health.powerOnHours()));
    output(QString("Unsafe shutdowns: %1")
           .arg(health.unsafeShutdowns()));
    output(QString("Media and data integrity errors: %1")
           .arg(health.mediaAndDataIntegrityErrors()));
    output(QString("Number of error information log entries: %1")
           .arg(health.numberOfErrorInformationLogEntries()));
    output(QString("Warning composite temperature time: %1 m")
           .arg(health.warningCompositeTemperatureTime()));
    output(QString("Critical composite temperature time: %1 m")
           .arg(health.criticalCompositeTemperatureTime()));
    int n = 0;
    foreach (int t, health.temperatureSensors()) {
        n++;
        if (t != 0)
            output(QString("Temperature sensor #%1: %2 °C")
                   .arg(n).arg(t - 273));
        else
            output(QString("Temperature sensor #%1: no sensor")
                   .arg(n));
    }
    output(QString("Thermal management temperature 1 transition count: %1")
           .arg(health.TMT1TransitionCount()));
    output(QString("Thermal management temperature 2 transition count: %1")
           .arg(health.TMT2TransitionCount()));
    output(QString("Total time for thermal management temperature 1: %1 s")
           .arg(health.totalTimeForTMT1()));
    output(QString("Total time for thermal management temperature 2: %1 s")
           .arg(health.totalTimeForTMT2()));
}

void MainWindow::readTest()
{
    ui->plainTextEdit->clear();

    QNvmeBlockDevice nvme;
    const QString dev = ui->deviceBox->currentText();

    if (!nvme.open(dev)) {
        output("Device opening error!");
        return;
    }

    output("*** READ TEST ***\n");

    QNvmeControllerInfo ctrlInfo = nvme.controllerInfo();
    if (!nvme.lastError().success()) {
        output("\nDevice communication error!\n");
        return;
    }

    if (ctrlInfo.namespaceCount() == 0) {
        output("Namespaces are not found!");
        return;
    }

    for (int i = 1; i <= ctrlInfo.namespaceCount(); i++) {
        QNvmeNamespaceInfo nsInfo = nvme.namespaceInfo(i);
        if (!nvme.lastError().success()) {
            output("\nDevice communication error!\n");
            return;
        }

        // You should never request the data more than maxDataTransferSize()!!!
        QByteArray buffer(ctrlInfo.maxDataTransferSize(), 0);
        const quint32 count = buffer.size() / nsInfo.sectorSize();
        quint64 offset = 0;

        int t = 5000; // Time for testing in ms
        QTimer timer;
        timer.setSingleShot(true);
        timer.start(t);

        while (timer.remainingTime() > 0) {
            nvme.read(offset, count, buffer.data(), buffer.size(), i);

            // Basic error handling...
            // In case of err.type == QNvmeError::DeviceTimeout you can try:
            // - nvme.controllerReset()
            // - device power cycle using example above
            QNvmeError err = nvme.lastError();
            if (!err.success()) {
                    output(err.toString());
                    return;
            }

            offset += count;
        }

        double speed = (offset * nsInfo.sectorSize()) / (t * 1000.0);
        output(QString("Namespace %1 average read speed: %2 MB/sec\n")
               .arg(i)
               .arg(speed, 0, 'f', 1));
    }
}

