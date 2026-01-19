#include <ISHantillyBuilder.h>
#include <ParserMain.h>

#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QPointer>
#include <QPushButton>
#include <QRadioButton>
#include <QSlider>
#include <QSpinBox>
#include <QTabWidget>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>
#include <QtTest>

// Mock Builder returning real widgets for runtime interaction tests
class MockBuilder : public ISHantillyBuilder {
public:
    QList<QPointer<QWidget>> createdWidgets;

    ~MockBuilder() {
        for (auto w : createdWidgets) {
            if (w && !w->parentWidget()) {
                delete w;
            }
        }
    }

    template <typename T>
    T* createReal(const QString& name) {
        T* w = new T();
        w->setObjectName(name);
        createdWidgets.append(w);
        return w;
    }

    PushButtonWidget* buildPushButton(const QString& title, const QString& name) override {
        Q_UNUSED(title);
        Q_UNUSED(name);
        return nullptr;
    }

    QWidget* buildWindow(const Sbx::Models::WindowConfig& config) override {
        lastWindow = config;
        called = true;
        auto* w = createReal<QWidget>(config.name);
        w->setLayout(new QVBoxLayout());
        return w;
    }

    QWidget* buildButton(const Sbx::Models::ButtonConfig& config) override {
        lastButton = config;
        called = true;
        return createReal<QPushButton>(config.name);
    }

    QWidget* buildLabel(const Sbx::Models::LabelConfig& config) override {
        lastLabel = config;
        called = true;
        return createReal<QLabel>(config.name);
    }

    QWidget* buildCheckBox(const Sbx::Models::CheckBoxConfig& config) override {
        lastCheckBox = config;
        called = true;
        return createReal<QCheckBox>(config.name);
    }

    QWidget* buildRadioButton(const Sbx::Models::RadioButtonConfig& config) override {
        lastRadioButton = config;
        called = true;
        return createReal<QRadioButton>(config.name);
    }

    QWidget* buildComboBox(const Sbx::Models::ComboBoxConfig& config) override {
        lastComboBox = config;
        called = true;
        return createReal<QWidget>(config.name); // Simplified
    }

    QWidget* buildList(const Sbx::Models::ListConfig& config) override {
        lastList = config;
        called = true;
        return createReal<QWidget>(config.name); // Simplified
    }

    QWidget* buildSpinBox(const Sbx::Models::SpinBoxConfig& config) override {
        lastSpinBox = config;
        called = true;
        return createReal<QSpinBox>(config.name);
    }

    QWidget* buildSlider(const Sbx::Models::SliderConfig& config) override {
        lastSlider = config;
        called = true;
        return createReal<QSlider>(config.name);
    }

    QWidget* buildLineEdit(const Sbx::Models::LineEditConfig& config) override {
        lastLineEdit = config;
        called = true;
        return createReal<QLineEdit>(config.name);
    }

    QWidget* buildTextEdit(const Sbx::Models::TextEditConfig& config) override {
        lastTextEdit = config;
        called = true;
        return createReal<QTextEdit>(config.name);
    }

    QWidget* buildGroupBox(const Sbx::Models::GroupBoxConfig& config) override {
        lastGroupBox = config;
        called = true;
        auto* w = createReal<QWidget>(config.name);
        w->setLayout(new QVBoxLayout());
        return w;
    }

    QWidget* buildFrame(const Sbx::Models::FrameConfig& config) override {
        lastFrame = config;
        called = true;
        auto* w = createReal<QWidget>(config.name);
        w->setLayout(new QVBoxLayout());
        return w;
    }

    QWidget* buildTabWidget(const Sbx::Models::TabWidgetConfig& config) override {
        lastTabWidget = config;
        called = true;
        auto* w = new QTabWidget();
        w->setObjectName(config.name);
        createdWidgets.append(w);
        return w;
    }

    // Stubs for remaining methods
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
    QWidget* buildCalendar(const Sbx::Models::CalendarConfig& config) override {
        Q_UNUSED(config);
        return nullptr;
    }
    QWidget* buildSeparator(const Sbx::Models::SeparatorConfig& config) override {
        Q_UNUSED(config);
        return nullptr;
    }
    QLayout* buildLayoutStructure(const Sbx::Models::SbxLayoutConfig& config) override {
        Q_UNUSED(config);
        return nullptr;
    }

    // State capture for verification
    Sbx::Models::ButtonConfig lastButton;
    Sbx::Models::LabelConfig lastLabel;
    Sbx::Models::CheckBoxConfig lastCheckBox;
    Sbx::Models::RadioButtonConfig lastRadioButton;
    Sbx::Models::ComboBoxConfig lastComboBox;
    Sbx::Models::ListConfig lastList;
    Sbx::Models::WindowConfig lastWindow;
    Sbx::Models::SpinBoxConfig lastSpinBox;
    Sbx::Models::SliderConfig lastSlider;
    Sbx::Models::LineEditConfig lastLineEdit;
    Sbx::Models::TextEditConfig lastTextEdit;
    Sbx::Models::GroupBoxConfig lastGroupBox;
    Sbx::Models::FrameConfig lastFrame;
    Sbx::Models::TabWidgetConfig lastTabWidget;
    bool called = false;
};

class TestParser : public QObject {
    Q_OBJECT

private slots:
    void testParseAddButton();
    void testParseAddLabel();
    void testParseAddCheckBox();
    void testParseAddRadioButton();
    void testParseAddComboBox();
    void testParseAddListBox();
    void testParseAddWindow();
    void testParseAddSpinBox();
    void testParseAddSlider();
    void testParseAddTextBox();
    void testParseAddTextView();
    void testParseHierarchicalLayout();
    void testRuntimeInteraction();
};

void TestParser::testRuntimeInteraction() {
    MockBuilder builder;
    ParserMain parser(&builder);

    // 1. Create a checkbox
    parser.processLine("add checkbox \"Turbo\" chk1");
    QCheckBox* cb = qobject_cast<QCheckBox*>(builder.createdWidgets.last());
    QVERIFY(cb != nullptr);
    QCOMPARE(cb->isChecked(), false);

    // 2. Set it
    parser.processLine("set checked chk1");
    QCOMPARE(cb->isChecked(), true);

    // 3. Unset it
    parser.processLine("unset checked chk1");
    QCOMPARE(cb->isChecked(), false);

    // 4. Create spinbox and set value
    parser.processLine("add spinbox \"Count\" sb1");
    QSpinBox* sb = qobject_cast<QSpinBox*>(builder.createdWidgets.last());
    parser.processLine("set value sb1 42");
    QCOMPARE(sb->value(), 42);
}

void TestParser::testParseAddButton() {
    MockBuilder builder;
    ParserMain parser(&builder);
    parser.processLine("add button \"Click Me\" btn_test checkable checked");
    QVERIFY(builder.called);
    QCOMPARE(builder.lastButton.text, QString("Click Me"));
    QCOMPARE(builder.lastButton.name, QString("btn_test"));
    QCOMPARE(builder.lastButton.checkable, true);
    QCOMPARE(builder.lastButton.checked, true);
}

void TestParser::testParseAddLabel() {
    MockBuilder builder;
    ParserMain parser(&builder);
    parser.processLine("add label \"Hello World\" lbl1 wordwrap");
    QVERIFY(builder.called);
    QCOMPARE(builder.lastLabel.text, QString("Hello World"));
    QCOMPARE(builder.lastLabel.name, QString("lbl1"));
    QCOMPARE(builder.lastLabel.wordWrap, true);
}

void TestParser::testParseAddCheckBox() {
    MockBuilder builder;
    ParserMain parser(&builder);
    parser.processLine("add checkbox \"Turbo\" chk1 checked");
    QVERIFY(builder.called);
    QCOMPARE(builder.lastCheckBox.text, QString("Turbo"));
    QCOMPARE(builder.lastCheckBox.checked, true);
}

void TestParser::testParseAddRadioButton() {
    MockBuilder builder;
    ParserMain parser(&builder);
    parser.processLine("add radiobutton \"Option 1\" rb1 checked");
    QVERIFY(builder.called);
    QCOMPARE(builder.lastRadioButton.text, QString("Option 1"));
    QCOMPARE(builder.lastRadioButton.checked, true);
}

void TestParser::testParseAddComboBox() {
    MockBuilder builder;
    ParserMain parser(&builder);
    parser.processLine("add dropdownlist \"Choice\" combo1");
    QVERIFY(builder.called);
    QCOMPARE(builder.lastComboBox.name, QString("combo1"));
}

void TestParser::testParseAddListBox() {
    MockBuilder builder;
    ParserMain parser(&builder);
    parser.processLine("add listbox \"Select\" list1 selection");
    QVERIFY(builder.called);
    QCOMPARE(builder.lastList.name, QString("list1"));
    QCOMPARE(builder.lastList.multipleSelection, true);
}

void TestParser::testParseAddWindow() {
    MockBuilder builder;
    ParserMain parser(&builder);
    parser.processLine("add window \"My App\" main_win width 1024 height 768");
    QVERIFY(builder.called);
    QCOMPARE(builder.lastWindow.title, QString("My App"));
    QCOMPARE(builder.lastWindow.width, 1024);
    QCOMPARE(builder.lastWindow.height, 768);
}

void TestParser::testParseAddSpinBox() {
    MockBuilder builder;
    ParserMain parser(&builder);
    parser.processLine("add spinbox \"Count\" sb1 minimum 10 maximum 50 value 20 step 5");
    QVERIFY(builder.called);
    QCOMPARE(builder.lastSpinBox.name, QString("sb1"));
    QCOMPARE(builder.lastSpinBox.min, 10);
    QCOMPARE(builder.lastSpinBox.max, 50);
    QCOMPARE(builder.lastSpinBox.value, 20);
    QCOMPARE(builder.lastSpinBox.step, 5);
}

void TestParser::testParseAddSlider() {
    MockBuilder builder;
    ParserMain parser(&builder);
    parser.processLine("add slider \"Volume\" sl1 vertical value 75");
    QVERIFY(builder.called);
    QCOMPARE(builder.lastSlider.name, QString("sl1"));
    QCOMPARE(builder.lastSlider.orientation, 2); // Vertical
    QCOMPARE(builder.lastSlider.value, 75);
}

void TestParser::testParseAddTextBox() {
    MockBuilder builder;
    ParserMain parser(&builder);
    parser.processLine("add textbox \"Initial Value\" tb1 password placeholder \"Enter secret\"");
    QVERIFY(builder.called);
    QCOMPARE(builder.lastLineEdit.text, QString("Initial Value"));
    QCOMPARE(builder.lastLineEdit.passwordMode, true);
    QCOMPARE(builder.lastLineEdit.placeholder, QString("Enter secret"));
}

void TestParser::testParseAddTextView() {
    MockBuilder builder;
    ParserMain parser(&builder);
    parser.processLine("add textview \"Long content...\" tv1 readonly");
    QVERIFY(builder.called);
    QCOMPARE(builder.lastTextEdit.text, QString("Long content..."));
    QCOMPARE(builder.lastTextEdit.readOnly, true);
}

void TestParser::testParseHierarchicalLayout() {
    MockBuilder builder;
    ParserMain parser(&builder);

    // 1. Create Window (Root)
    parser.processLine("add window \"Main\" win");
    QVERIFY(builder.called);

    // 2. Add GroupBox (should be added to Window)
    parser.processLine("add groupbox \"Group\" grp1");
    QCOMPARE(builder.lastGroupBox.title, QString("Group"));

    // 3. Add Button (should be added to GroupBox)
    parser.processLine("add button \"Inside\" btn1");
    QCOMPARE(builder.lastButton.text, QString("Inside"));

    // 4. End GroupBox context
    parser.processLine("end");

    // 5. Add Button (should be added to Window now)
    parser.processLine("add button \"Outside\" btn2");
    QCOMPARE(builder.lastButton.text, QString("Outside"));
}

QTEST_MAIN(TestParser)
#include "tst_parser.moc"