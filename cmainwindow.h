#ifndef CMAINWINDOW_H
#define CMAINWINDOW_H

#include <QMainWindow>
#include "cserialport.h"
#include "ssettings.h"

const QString DIALOG_FINISH_EDITING = "Вы действительно хотите завершить настройку МПД?";
const QString STATUS_NO_PORTS = "Нет доступных устройств для подключения";

QT_BEGIN_NAMESPACE namespace Ui
{
    class CMainWindow;
}
QT_END_NAMESPACE

class CMainWindow : public QMainWindow
{
    Q_OBJECT
    enum EPage { eStartPage, eMainPage };
    enum ESettingsMode { eHidden, eViewMode, eEditMode, eWaitMode };

public:
    CMainWindow(QWidget *parent = nullptr);
    ~CMainWindow();

    // GUI - functions
private slots:
    ///   void changeSerialState(QString textName, EState st);
    void on_wgt_model_clicked(QString nameEl);
    void on_pb_radio_clicked();
    void on_pb_startConfigure_clicked();
    void on_pb_closeSettings_clicked();

    void enableSettings(bool st);

    void on_pb_edit_clicked();
    //! пользователь захотел подключить/отключить интерфейс
    void wantChangeStateReceived(QString ifaceName, EState st);
    //! чтение настроек, пришедших с МПД
    void readyRead();

    void on_pb_finishConfigure_clicked();

    void on_pb_cancel_clicked();

    void on_pb_accept_clicked();

private:
    ///!устанавливает вид страницы
    void setView(EPage page);
    ///! устанавливает вид настроек (только для eMainPage)
    void setMode(ESettingsMode mode);
    ///! обновляет комбо-бокс с доступными портами (только для eStartPage)
    void updateAvaliablePorts();
    ///! заполняет поля настроек для заданного интерфейса
    void loadSettings(QString nameElement);
    ///! запрашивает от МПД текущие настройки
    void requestCurrentSettings(ESettingsType type = eNoSection);
    ///! обновляет текущие настройки в соответствии с ответом МПД
    void currentSettingsReceived(QList<SSettings> settings);
    ///! Заполняет настройки
    void fillSettings(QString nameEl);

private:
    Ui::CMainWindow *ui;
    //!*** Fields for GUI
    ESettingsMode m_mode { eHidden };
    EPage m_page { eStartPage };
    //! Selected unit
    SSettings m_current;
    QList<SSettings> m_setts;

    //!*** Fields for external connections
    CSerialPort m_serial;
};
#endif // CMAINWINDOW_H
