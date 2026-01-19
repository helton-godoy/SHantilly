#include <QtTest>
#include <ISHantillyBuilder.h>
#include <WidgetConfigs.h>
#include <QWidget>

// Mock implementation to verify interface signature
class MockBuilder : public ISHantillyBuilder {
public:
    QWidget* buildWindow(const Sbx::Models::WindowConfig& config) override {
        Q_UNUSED(config);
        return nullptr;
    }
    
    QWidget* buildButton(const Sbx::Models::ButtonConfig& config) override {
        Q_UNUSED(config);
        return nullptr;
    }
    
    QWidget* buildLabel(const Sbx::Models::LabelConfig& config) override {
        Q_UNUSED(config);
        return nullptr;
    }

    QWidget* buildLineEdit(const Sbx::Models::LineEditConfig& config) override {
        Q_UNUSED(config);
        return nullptr;
    }

    QWidget* buildComboBox(const Sbx::Models::ComboBoxConfig& config) override {
        Q_UNUSED(config);
        return nullptr;
    }

    QWidget* buildList(const Sbx::Models::ListConfig& config) override {
        Q_UNUSED(config);
        return nullptr;
    }

    QWidget* buildTable(const Sbx::Models::TableConfig& config) override {
        Q_UNUSED(config);
        return nullptr;
    }

    QWidget* buildProgressBar(const Sbx::Models::ProgressBarConfig& config) override {
        Q_UNUSED(config);
        return nullptr;
    }

    QWidget* buildChart(const Sbx::Models::ChartConfig& config) override {
        Q_UNUSED(config);
        return nullptr;
    }

    QWidget* buildCheckBox(const Sbx::Models::CheckBoxConfig& config) override {
        Q_UNUSED(config);
        return nullptr;
    }

    QWidget* buildRadioButton(const Sbx::Models::RadioButtonConfig& config) override {
        Q_UNUSED(config);
        return nullptr;
    }

    QWidget* buildCalendar(const Sbx::Models::CalendarConfig& config) override {
        Q_UNUSED(config);
        return nullptr;
    }

    QWidget* buildSeparator(const Sbx::Models::SeparatorConfig& config) override {
        Q_UNUSED(config);
        return nullptr;
    }

    QWidget* buildSpinBox(const Sbx::Models::SpinBoxConfig& config) override {
        Q_UNUSED(config);
        return nullptr;
    }

    QWidget* buildSlider(const Sbx::Models::SliderConfig& config) override {
        Q_UNUSED(config);
        return nullptr;
    }

    QWidget* buildTextEdit(const Sbx::Models::TextEditConfig& config) override {
        Q_UNUSED(config);
        return nullptr;
    }

    QWidget* buildGroupBox(const Sbx::Models::GroupBoxConfig& config) override {
        Q_UNUSED(config);
        return nullptr;
    }

    QWidget* buildFrame(const Sbx::Models::FrameConfig& config) override {
        Q_UNUSED(config);
        return nullptr;
    }

    QWidget* buildTabWidget(const Sbx::Models::TabWidgetConfig& config) override {
        Q_UNUSED(config);
        return nullptr;
    }

    QLayout* buildLayoutStructure(const Sbx::Models::SbxLayoutConfig& config) override {
        Q_UNUSED(config);
        return nullptr;
    }

    // Legacy support
    PushButtonWidget* buildPushButton(const QString &title, const QString &name) override {
        Q_UNUSED(title);
        Q_UNUSED(name);
        return nullptr;
    }
};

class TestInterfaceBuilder : public QObject
{
    Q_OBJECT
private slots:
    void testInterfaceExistence();
};

void TestInterfaceBuilder::testInterfaceExistence()
{
    MockBuilder builder;
    QVERIFY(true);
}

QTEST_MAIN(TestInterfaceBuilder)
#include "tst_interface_builder.moc"