#include <QtTest>
#include <ShowboxBuilder.h>
#include <WidgetConfigs.h>
#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QListWidget>
#include <QTableWidget>

class TestShowboxBuilder : public QObject
{
    Q_OBJECT

private slots:
    void testCreateWindow();
    void testBuildLayout();
    void testBuildControls();
    void testBuildItemBasedWidgets();
    void testPerformance();
};

void TestShowboxBuilder::testCreateWindow()
{
    ShowboxBuilder builder;
    Showbox::Models::WindowConfig config;
    config.title = "Test Window";
    config.width = 400;
    config.height = 300;
    
    QWidget* window = builder.buildWindow(config);
    
    QVERIFY(window != nullptr);
    QCOMPARE(window->windowTitle(), QString("Test Window"));
    // Note: window geometry might not be exactly 400x300 until shown, 
    // but resize() should set the values.
    QCOMPARE(window->width(), 400);
    QCOMPARE(window->height(), 300);
    
    delete window;
}

void TestShowboxBuilder::testBuildLayout()
{
    ShowboxBuilder builder;
    Showbox::Models::LayoutConfig config;
    config.type = Showbox::Models::LayoutConfig::HBox;
    config.spacing = 10;
    config.margin = 15;
    
    QLayout* layout = builder.buildLayout(config);
    
    QVERIFY(layout != nullptr);
    QCOMPARE(layout->spacing(), 10);
    QCOMPARE(layout->contentsMargins().left(), 15);
    QVERIFY(qobject_cast<QHBoxLayout*>(layout) != nullptr);
    
    delete layout;
}

void TestShowboxBuilder::testBuildControls()
{
    ShowboxBuilder builder;
    
    // Test Button
    Showbox::Models::ButtonConfig btnConfig;
    btnConfig.name = "btn1";
    btnConfig.text = "Click Me";
    QWidget* btn = builder.buildButton(btnConfig);
    QVERIFY(btn != nullptr);
    QCOMPARE(qobject_cast<QPushButton*>(btn)->text(), QString("Click Me"));
    delete btn;
    
    // Test Label
    Showbox::Models::LabelConfig lblConfig;
    lblConfig.name = "lbl1";
    lblConfig.text = "Hello World";
    QWidget* lbl = builder.buildLabel(lblConfig);
    QVERIFY(lbl != nullptr);
    QCOMPARE(qobject_cast<QLabel*>(lbl)->text(), QString("Hello World"));
    delete lbl;
    
    // Test LineEdit
    Showbox::Models::LineEditConfig leConfig;
    leConfig.name = "le1";
    leConfig.placeholder = "Enter name";
    QWidget* le = builder.buildLineEdit(leConfig);
    QVERIFY(le != nullptr);
    QCOMPARE(qobject_cast<QLineEdit*>(le)->placeholderText(), QString("Enter name"));
    delete le;
}

void TestShowboxBuilder::testBuildItemBasedWidgets()
{
    ShowboxBuilder builder;
    
    // Test ComboBox
    Showbox::Models::ComboBoxConfig cbConfig;
    cbConfig.name = "cb1";
    cbConfig.items << "Item 1" << "Item 2" << "Item 3";
    cbConfig.currentIndex = 1;
    QWidget* cb = builder.buildComboBox(cbConfig);
    QVERIFY(cb != nullptr);
    QCOMPARE(qobject_cast<QComboBox*>(cb)->count(), 3);
    QCOMPARE(qobject_cast<QComboBox*>(cb)->currentIndex(), 1);
    delete cb;
    
    // Test List
    Showbox::Models::ListConfig listConfig;
    listConfig.name = "list1";
    listConfig.items << "Option A" << "Option B";
    QWidget* list = builder.buildList(listConfig);
    QVERIFY(list != nullptr);
    QCOMPARE(qobject_cast<QListWidget*>(list)->count(), 2);
    delete list;
    
    // Test Table
    Showbox::Models::TableConfig tableConfig;
    tableConfig.name = "table1";
    tableConfig.headers << "Col 1" << "Col 2";
    tableConfig.rows << (QStringList() << "R1C1" << "R1C2");
    QWidget* table = builder.buildTable(tableConfig);
    QVERIFY(table != nullptr);
    QCOMPARE(qobject_cast<QTableWidget*>(table)->columnCount(), 2);
    QCOMPARE(qobject_cast<QTableWidget*>(table)->rowCount(), 1);
    QCOMPARE(qobject_cast<QTableWidget*>(table)->item(0, 0)->text(), QString("R1C1"));
    delete table;
}

void TestShowboxBuilder::testPerformance()
{
    ShowboxBuilder builder;
    Showbox::Models::ButtonConfig config;
    config.name = "btn";
    
    QBENCHMARK {
        for (int i = 0; i < 100; ++i) {
            delete builder.buildButton(config);
        }
    }
}

QTEST_MAIN(TestShowboxBuilder)
#include "tst_showbox_builder.moc"
