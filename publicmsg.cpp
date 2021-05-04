#include "publicmsg.h"
#include "config.h"
#include <QStandardPaths>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QImageWriter>

PublicMSG::PublicMSG(QObject *parent,  QPixmap *originalPixmap)
{
   this->msg=msg;
   this->groupAddress=QHostAddress(groupAddress);
   this->originalPixmap=originalPixmap;
}

void PublicMSG::run() {

    const QString format = "png";
    QString initialPath = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
    if (initialPath.isEmpty())
        initialPath = QDir::currentPath();
    initialPath += tr("/untitled.") + format;

    QFileDialog fileDialog(nullptr, tr("Save As"), initialPath);
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);
    fileDialog.setFileMode(QFileDialog::AnyFile);
    fileDialog.setDirectory(initialPath);

    QStringList mimeTypes;
    foreach (const QByteArray &bf, QImageWriter::supportedMimeTypes())
           mimeTypes.append(QLatin1String(bf));
    fileDialog.setMimeTypeFilters(mimeTypes);
    fileDialog.selectMimeTypeFilter("image/" + format);
    fileDialog.setDefaultSuffix(format);
    fileDialog.activateWindow();

    if (fileDialog.exec() != QDialog::Accepted)
       return;

    const QString fileName = fileDialog.selectedFiles().first();
    if (!originalPixmap->save(fileName)) {
           QMessageBox::warning(nullptr, tr("Save Error"), tr("The image could not be saved to \"%1\".")
                                .arg(QDir::toNativeSeparators(fileName)));
    }
}
