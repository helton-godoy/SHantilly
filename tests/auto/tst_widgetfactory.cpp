#include <QtTest>
#include "widget_factory.h"
#include <QPushButton>

class TestWidgetFactory : public QObject
{
    Q_OBJECT

private slots:
    void testCreateButton();
};

void TestWidgetFactory::testCreateButton()
{
    // This should fail because currently it returns nullptr
    QWidget* btn = WidgetFactory::create("button");
    QVERIFY(btn != nullptr);
    
    // If it were not null, we would check type
    if (btn) {
        QCOMPARE(QString(btn->metaObject()->className()), QString("QPushButton"));
        delete btn;
    }
}

QTEST_MAIN(TestWidgetFactory)
#include "tst_widgetfactory.moc"
