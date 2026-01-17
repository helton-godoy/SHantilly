#ifndef CLIBUILDER_H
#define CLIBUILDER_H

#include <IShowboxBuilder.h>
#include <QObject>
#include <QDialog>
#include <QVBoxLayout>

class CLIBuilder : public QObject, public IShowboxBuilder
{
    Q_OBJECT
public:
    explicit CLIBuilder(QObject *parent = nullptr);
    
    // Legacy
    PushButtonWidget* buildPushButton(const QString &title, const QString &name) override;

    // New Architecture
    QWidget* buildWindow(const Showbox::Models::WindowConfig& config) override;
    QWidget* buildButton(const Showbox::Models::ButtonConfig& config) override;
    QWidget* buildLabel(const Showbox::Models::LabelConfig& config) override;
    QWidget* buildLineEdit(const Showbox::Models::LineEditConfig& config) override;
    QWidget* buildComboBox(const Showbox::Models::ComboBoxConfig& config) override;
    QWidget* buildList(const Showbox::Models::ListConfig& config) override;
    QWidget* buildTable(const Showbox::Models::TableConfig& config) override;
    QWidget* buildProgressBar(const Showbox::Models::ProgressBarConfig& config) override;
    QWidget* buildChart(const Showbox::Models::ChartConfig& config) override;
    QWidget* buildCheckBox(const Showbox::Models::CheckBoxConfig& config) override;
    QWidget* buildRadioButton(const Showbox::Models::RadioButtonConfig& config) override;
    QWidget* buildCalendar(const Showbox::Models::CalendarConfig& config) override;
    QWidget* buildSeparator(const Showbox::Models::SeparatorConfig& config) override;
    QWidget* buildSpinBox(const Showbox::Models::SpinBoxConfig& config) override;
    QWidget* buildSlider(const Showbox::Models::SliderConfig& config) override;
    QWidget* buildTextEdit(const Showbox::Models::TextEditConfig& config) override;
    QWidget* buildGroupBox(const Showbox::Models::GroupBoxConfig& config) override;
    QWidget* buildFrame(const Showbox::Models::FrameConfig& config) override;
    QWidget* buildTabWidget(const Showbox::Models::TabWidgetConfig& config) override;
    QLayout* buildLayout(const Showbox::Models::LayoutConfig& config) override;

private:
    QDialog *m_window;
    QVBoxLayout *m_layout;
};

#endif // CLIBUILDER_H
